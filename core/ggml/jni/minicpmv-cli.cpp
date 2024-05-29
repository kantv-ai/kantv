//ref&author: https://github.com/OpenBMB/llama.cpp/blob/minicpm-v2.5/examples/minicpmv
#include "ggml-jni.h"
#include "ggml.h"
#include "log.h"
#include "common.h"
#include "clip.h"
#include "minicpmv.h"
#include "minicpmv_io.h"
#include "llama.h"

#include <cstdio>
#include <cstdlib>
#include <vector>

static void show_additional_info(int /*argc*/, char ** argv) {
    LOGGD("\n example usage: %s -m <llava-v1.5-7b/ggml-model-q5_k.gguf> --mmproj <llava-v1.5-7b/mmproj-model-f16.gguf> --image <path/to/an/image.jpg> --image <path/to/another/image.jpg> [--temp 0.1] [-p \"describe the image in detail.\"]\n", argv[0]);
    LOGGD("  note: a lower temperature value like 0.1 is recommended for better quality.\n");
}

static void llama_log_callback_logTee(ggml_log_level level, const char * text, void * user_data) {
    (void) level;
    (void) user_data;
    LOGGD("%s", text);
}

struct minicpmv_context * minicpmv_init(gpt_params * params, const std::string & fname, int &n_past){
    auto image_embed_slices = minicpmv_image_embed(params, fname);
    if (!image_embed_slices[0][0]) {
        std::cerr << "error: failed to load image " << fname << ". Terminating\n\n";
        return NULL;
    }

    // process the prompt
    if (params->prompt.empty() && params->interactive == false) {
        LOGGD("prompt should be given or interactive mode should be on");
        return NULL;
    }

    auto model = llava_init(params);
    if (model == NULL) {
        fprintf(stderr, "%s: error: failed to init minicpmv model\n", __func__);
        return NULL;
    }
    const int64_t t_llava_init_start_us = ggml_time_us();
    auto ctx_llava = llava_init_context(params, model);

    const int64_t t_llava_init_end_us = ggml_time_us();
    float t_llava_init_ms = (t_llava_init_end_us - t_llava_init_start_us) / 1000.0;
    LOGGD("\n%s: llava init in %8.2f ms.\n", __func__, t_llava_init_ms);

    const int64_t t_process_image_start_us = ggml_time_us();
    process_image(ctx_llava, image_embed_slices, params, n_past);
    const int64_t t_process_image_end_us = ggml_time_us();
    float t_process_image_ms = (t_process_image_end_us - t_process_image_start_us) / 1000.0;
    LOGGD("\n%s: llama process image in %8.2f ms.\n", __func__, t_process_image_ms);

    llava_image_embed_free_slice(image_embed_slices);
    return ctx_llava;
}

struct llama_sampling_context * llama_init(struct minicpmv_context * ctx_llava, gpt_params * params, std::string prompt, int &n_past, bool is_first = false){
    std::string user_prompt = prompt;
    if (!is_first) user_prompt = "<|begin_of_text|><|start_header_id|>user<|end_header_id|>\n\n" + prompt;
    const int max_tgt_len = params->n_predict < 0 ? 256 : params->n_predict;

    eval_string(ctx_llava->ctx_llama, user_prompt.c_str(), params->n_batch, &n_past, false);
    eval_string(ctx_llava->ctx_llama, "<|eot_id|><|start_header_id|>assistant<|end_header_id|>\n\n", params->n_batch, &n_past, false);
    // generate the response

    LOGGD("\n");

    struct llama_sampling_context * ctx_sampling = llama_sampling_init(params->sparams);
    return ctx_sampling;
}

const char * llama_loop(struct minicpmv_context * ctx_llava,struct llama_sampling_context * ctx_sampling, int &n_past){
    
    const char * tmp = sample(ctx_sampling, ctx_llava->ctx_llama, &n_past);
    return tmp;
}

#ifdef ANDROID
int minicpmv_inference_main(int argc, char ** argv, int backend) {
#else //for build and run MiniCPM-V command line application on Linux
//works fine on Ubuntu20.04
//./minicpmv-cli -m /home/weiguo/models/ggml-model-Q4_K_M.gguf --mmproj /home/weiguo/models/mmproj-model-f16.gguf  --image /home/weiguo/Downloads/airplane.jpeg  -t 4 -p "What is in the image?"
int main(int argc, char ** argv) {
#endif
    ggml_time_init();

    gpt_params params;

    if (!gpt_params_parse(argc, argv, params)) {
        show_additional_info(argc, argv);
        return 1;
    }
#ifdef ANDROID
    if (backend != GGML_BACKEND_GGML) { // GGML_BACKEND_GGML is the original GGML, used to compare performance between QNN backend and original GGML
#ifdef GGML_USE_QNN
        LOGGD("using QNN backend %d", backend);
        params.main_gpu = backend;
        params.n_gpu_layers = 1;
#else
        LOGGW("QNN feature was disabled and backend is not ggml\n");
        GGML_JNI_NOTIFY("QNN feature was disabled and backend is not ggml\n");
        return 1;
#endif
    }
#endif

#ifndef LOG_DISABLE_LOGS
    log_set_target(log_filename_generator("llava", "log"));
    LOGGD("Log start\n");
    log_dump_cmdline(argc, argv);
    llama_log_set(llama_log_callback_logTee, nullptr);
#endif // LOG_DISABLE_LOGS

    if (params.mmproj.empty() || (params.image.empty())) {
        gpt_params_print_usage(argc, argv, params);
        show_additional_info(argc, argv);
        return 1;
    }

    for (auto & image : params.image) {
        int n_past = 0;
        auto ctx_llava = minicpmv_init(&params, image, n_past);

        if (!params.prompt.empty()) {
            LOGGD("<user>%s\n", params.prompt.c_str());
            LOGGD("<assistant>");
            auto ctx_sampling = llama_init(ctx_llava, &params, params.prompt.c_str(), n_past, true);
            const int max_tgt_len = params.n_predict < 0 ? 256 : params.n_predict;
            std::string response = "";
            bool have_tmp = false;
            for (int i = 0; i < max_tgt_len; i++) {
                auto tmp = llama_loop(ctx_llava, ctx_sampling, n_past);
                response += tmp;
                if (strcmp(tmp, "</s>") == 0){
                    if(!have_tmp)continue;
                    else break;
                }
                if (strstr(tmp, "###")) break; // Yi-VL behavior
                have_tmp = true;
                printf("%s", tmp);
#ifdef ANDROID
                kantv_asr_notify_benchmark_c(tmp);
#endif

                if (strstr(response.c_str(), "<user>")) break; // minicpm-v

                fflush(stdout);
            }
            llama_sampling_free(ctx_sampling);
        }else {
            while (true) {
                LOGGD("<user>");
                std::string prompt;
                std::getline(std::cin, prompt);
                LOGGD("<assistant>");
                auto ctx_sampling = llama_init(ctx_llava, &params, prompt, n_past, true);
                const int max_tgt_len = params.n_predict < 0 ? 256 : params.n_predict;
                std::string response = "";
                for (int i = 0; i < max_tgt_len; i++) {
                    auto tmp = llama_loop(ctx_llava, ctx_sampling, n_past);
                    response += tmp;
                    if (strcmp(tmp, "</s>") == 0) break;
                    if (strstr(tmp, "###")) break; // Yi-VL behavior
                    printf("%s", tmp);// mistral llava-1.6
                    if (strstr(response.c_str(), "<user>")) break; // minicpm-v 
                    fflush(stdout);
                }
                llama_sampling_free(ctx_sampling);
            }
        }
        printf("\n");
#ifdef ANDROID
        kantv_asr_notify_benchmark_c("\n[end of text]\n");
#endif
        llama_print_timings(ctx_llava->ctx_llama);        

        ctx_llava->model = NULL;
        llava_free(ctx_llava);
    }

    return 0;
}
