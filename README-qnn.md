# llama.cpp for QNN

- [Background](#background)
- [News](#news)
- [OS](#os)
- [Hardware](#hardware)
- [Android](#android)
- [Windows over ARM](#windows)
- [Q&A](#qa)
- [TODO](#todo)

## Background

Android maintained its position as the leading mobile operating system worldwide in the fourth quarter of 2023 with <b><a  href="https://www.statista.com/statistics/272698/global-market-share-held-by-mobile-operating-systems-since-2009/">a market share of 70.1 percent </a></b> . Qualcomm is No.1 mobile SoC semiconductor company in our planet currently.


**QNN**(Qualcomm Neural Network, aka Qualcomm AI Engine Direct) SDK is verified to work with the following versions of the ML frameworks:

<ul>
<li>TensorFlow: tf-1.15.0, or tf-2.10.1 </li>
<li>TFLite: tflite-2.3.0 </li>
<li> PyTorch: torch-1.13.1</li>
<li> ONNX: onnx-1.11.0 </li>
</ul>


The Qualcomm® AI Engine Direct architecture is designed to be modular and allows for clean separation in the software for different hardware cores/accelerators such as the CPU, GPU and DSP that are designated as backends. Learn more about Qualcomm® AI Engine Direct backends here.

![Screenshot from 2024-04-14 11-42-14](https://github.com/zhouwg/kantv/assets/6889919/5d8de93a-7b02-4d6b-8b7f-19d2f829dd4d)

The Qualcomm® AI Engine Direct backends for different hardware cores/accelerators are compiled into individual core-specific libraries that come packaged with the SDK.


One of the key highlights of Qualcomm® AI Engine Direct is that it provides a unified API to delegate operations such as graph creation and execution across all hardware accelerator backends. This allows users to treat Qualcomm® AI Engine Direct as a hardware abstraction API and port applications easily to different cores.


The Qualcomm® AI Engine Direct API is designed to support an efficient execution model with capabilities such as graph optimizations to be taken care of internally. At the same time however, it leaves out broader functionality such as model parsing and network partitioning to higher level frameworks.

Qualcomm® AI Engine Direct API and the associated software stack provides all the constructs required by an application to construct, optimize and execute network models on the desired hardware accelerator core. Key constructs are illustrated by the Qualcomm AI Engine Direct Components - High Level View diagram.


![qnn-arch](https://github.com/zhouwg/kantv/assets/6889919/4f4881a6-9a91-4477-aeb2-193591375d75)



### Llama.cpp + QNN

The llama.cpp QNN backend(aka ggml-qnn backend) is intented to support **Qualcomm mobile SoC** firstly.


## News

- 01/29/2025---02/13/2025
  - re-launch activity of <a href="https://github.com/zhouwg/kantv/issues/246">refine ggml-qnn backend for latest ggml,whisper.cpp,llama.cpp</a></b>
  - data path works pretty good as expected with whisper.cpp and llama.cpp and test-backend-ops and llama-cli with ggml-qnn backend and verified on Xiaomi14(high-end Qualcomm mobile SoC equipped Android phone)
  - bugfix,santiy check,refine code according to coding stye and pricinple of upstream ggml community
  - Support OPs
    - GGML_OP_ADD
  - ready for the second PR to upstream llama.cpp community

- 05/28/2024---06/15/2024
  - re-launch activity of <a href="https://github.com/ggerganov/llama.cpp/pull/6869">PR in upstream ggml community</a>

- 04/26/2024
  - refine PR according to coding stye and pricinples of upstream ggml community
  - add command line test using <a href="https://github.com/ggerganov/llama.cpp/blob/master/tests/test-backend-ops.cpp">test-backend-ops.cpp</a>
  - refine PR according to comments from reviewer

- 04/24/2024
  - a very beginning <a href="https://github.com/ggerganov/llama.cpp/pull/6869">PR to upstream ggml community</a>
  - data path works fine as expected by <a href="https://github.com/ggerganov/llama.cpp/pull/7641">a workaround approach which not accepted by the author of ggml backend subsystem</a> with whisper.cpp and llama.cpp using QNN backend and verified on both low-end and high-end Android phones based on Qualcomm mobile SoC
  - Support OPs
    - GGML_OP_ADD
    - GGML_OP_MUL
    - GGML_OP_MUL_MAT

 - 03/29/2024---04/24/2024
   - first implementaton of ggml-qnn <a href="https://github.com/zhouwg/kantv/issues/121">PoC:add QNN backend for Qualcomm mobile SoC</a>

 - 03/05/2024---03/16/2024
   - first touch with ggml <a href="https://github.com/zhouwg/kantv/issues/64">PoC:clean-room implementation of real-time AI subtitle for English online-TV(OTT TV)</a>

## OS

| OS                | Status  | Verified                           |
|-------------------|---------|------------------------------------|
| Android           | Support | Android 10, Android 14             |
| Windows over ARM  | TBD     | TBD                                |


## Hardware

### Qualcomm mobile SoC based Android phone

**Verified devices**

| Qualcom mobile SoC                      | Status  | Verified Vendor                       |
|-----------------------------------------|---------|---------------------------------------|
| Qualcomm SM8650-AB Snapdragon 8 Gen 3   | Support | Xiaomi 14                             |
| Qualcomm low-end mobile SoC Series      | Support | Vivo                                  |

### Windows on ARM(Qualcomm desktop SoC)

TBD

## Android

### 1. Setup Environment

Any **mainstream** Android phone equipped with Qualcomm's mobile SoC should be supported by llama.cpp + ggml-qnn. Qualcomm SM8650-AB Snapdragon 8 Gen 3/4 quipped Android phone is preferred.

### 2. Run ggml-qnn backend in command line mode on Android phone

- for QNN backend developers, download and install QNN SDK from Qualcomm offcial website

```
  https://qpm.qualcomm.com/#/main/tools/details/qualcomm_ai_engine_direct

  https://developer.qualcomm.com/software/hexagon-dsp-sdk/tools

```

  the default installation path is /opt/qcom/aistack/qairt/2.31.0.250130/


- for <b>llama.cpp community programmers</b>, using <b>the official llama-cli and test-backend-ops command line tool </b> to verify ggml-qnn backend on Qualcomm mobile SoC equipped Android phone

```
  git clone https://github.com/kantv-ai/llama.cpp
  cd llama.cpp
  git checkout kantvai-ggmlqnn
  ./scripts/build-run-android.sh build          (it'll setup local build envs automatically and build the entire project)
  ./scripts/build-run-android.sh updateqnnlib   (upload Qualcomm's QNN binary runtime libs to Android phone)
  ./scripts/build-run-android.sh run_llamacli   (running llama-cli on Android pohone)
  ./scripts/build-run-android.sh run_testop     (running test-backend-ops on Android phone)

```

- for project programmers, using self-made command line application to verify ggml-qnn backend on Qualcomm mobile SoC equipped Android phone

```
  git clone https://github.com/zhouwg/kantv
  cd kantv
  . build/envsetup.sh
  lunch 1
  cd core/ggml/llamacpp/tests
  ./ggml-qnn-ut.sh build
  ./ggml-qnn-ut.sh updateqnnlib
  ./ggml-qnn-ut.sh GGML_OP_ADD      0 (QNN_CPU) / 1(QNN_GPU) / 2(QNN_NPU)

```

### 3. Run ggml-qnn backend in Android APK on Android phone

pls refer to <a href="./README.md">README.md</a>


![Image](https://github.com/user-attachments/assets/7c46a952-5d0a-4735-9d74-0bb0b4198b12)

![Image](https://github.com/user-attachments/assets/d93b3a64-6161-43fb-8d61-215ef4a68cfb)


## ggml-qnn for WoA(Windows on ARM)

TBD

## Q&A

pls file issue reports on https://github.com/zhouwg/kantv/issues

### **GitHub contribution**:
Please add the **[ggml-qnn]** prefix/tag in issues/PRs titles to help the community check/address them without delay.
