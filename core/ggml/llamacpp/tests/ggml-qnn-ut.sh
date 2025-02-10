# Copyright (c) 2024- KanTV Authors
#!/bin/bash

set -e

#QNN SDK could be found at:
#https://www.qualcomm.com/developer/software/qualcomm-ai-engine-direct-sdk
#https://qpm.qualcomm.com/#/main/tools/details/qualcomm_ai_engine_direct
#https://developer.qualcomm.com/software/hexagon-dsp-sdk/tools
#QNN_SDK_PATH=/opt/qcom/aistack/qnn/2.20.0.240223/
QNN_SDK_PATH=/opt/qcom/aistack/qairt/2.31.0.250130/

ANDROID_PLATFORM=android-34

GGML_QNN_UT=ggml-qnn-ut
REMOTE_PATH=/data/local/tmp/


function dump_vars()
{
    echo -e "ANDROID_NDK:          ${ANDROID_NDK}"
    echo -e "QNN_SDK_PATH:         ${QNN_SDK_PATH}"
}


function show_pwd()
{
    echo -e "current working path:$(pwd)\n"
}


function check_qnn_sdk()
{
    if [ ! -d ${QNN_SDK_PATH} ]; then
        echo -e "QNN_SDK_PATH ${QNN_SDK_PATH} not exist, pls check or download it from https://qpm.qualcomm.com/#/main/tools/details/qualcomm_ai_engine_direct...\n"
        exit 1
    fi
}


function check_and_download_ndk()
{
    is_android_ndk_exist=1

    if [ ! -d ${ANDROID_NDK} ]; then
        is_android_ndk_exist=0
    fi

    if [ ! -f ${ANDROID_NDK}/build/cmake/android.toolchain.cmake ]; then
        is_android_ndk_exist=0
    fi

    if [ ${is_android_ndk_exist} -eq 0 ]; then

        if [ ! -f android-ndk-r26c-linux.zip ]; then
            wget --no-config --quiet --show-progress -O android-ndk-r26c-linux.zip  https://dl.google.com/android/repository/android-ndk-r26c-linux.zip
        fi

        unzip android-ndk-r26c-linux.zip

        if [ $? -ne 0 ]; then
            printf "failed to download android ndk to %s \n" "${ANDROID_NDK}"
            exit 1
        fi

        printf "android ndk saved to ${ANDROID_NDK} \n\n"
    else
        printf "android ndk already exist:${ANDROID_NDK} \n\n"
    fi
}


function build_arm64
{
    #cmake -H. -B./out/arm64-v8a -DTARGET_NAME=${GGML_QNN_UT} -DCMAKE_BUILD_TYPE=Debug -DANDROID_ABI=arm64-v8a -DANDROID_PLATFORM=${ANDROID_PLATFORM} -DANDROID_NDK=${ANDROID_NDK}  -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake -DQNN_SDK_PATH=${QNN_SDK_PATH}
    cmake -H. -B./out/arm64-v8a -DTARGET_NAME=${GGML_QNN_UT} -DANDROID_ABI=arm64-v8a -DANDROID_PLATFORM=${ANDROID_PLATFORM} -DANDROID_NDK=${ANDROID_NDK}  -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake -DQNN_SDK_PATH=${QNN_SDK_PATH}

    cd ./out/arm64-v8a
    make

    ls -lah ${GGML_QNN_UT}
    /bin/cp ${GGML_QNN_UT} ../../
    cd -
}


function remove_temp_dir()
{
    if [ -d out ]; then
        echo "remove out directory in `pwd`"
        rm -rf out
    fi
}


function check_qnn_libs()
{
    #reuse the cached qnn libs in Android phone
    adb shell ls ${REMOTE_PATH}/libQnnCpu.so
    if [ $? -eq 0 ]; then
        printf "QNN libs already exist on Android phone\n"
    else
        update_qnn_libs
    fi
}


function update_qnn_libs()
{
        adb push ${QNN_SDK_PATH}/lib/aarch64-android/libQnnSystem.so              ${REMOTE_PATH}/
        adb push ${QNN_SDK_PATH}/lib/aarch64-android/libQnnCpu.so                 ${REMOTE_PATH}/
        adb push ${QNN_SDK_PATH}/lib/aarch64-android/libQnnGpu.so                 ${REMOTE_PATH}/

        #the QNN NPU(aka HTP/DSP) backend only verified on Xiaomi14(Qualcomm SM8650-AB Snapdragon 8 Gen 3) successfully
        adb push ${QNN_SDK_PATH}/lib/aarch64-android/libQnnHtp.so                 ${REMOTE_PATH}/
        adb push ${QNN_SDK_PATH}/lib/aarch64-android/libQnnHtpNetRunExtensions.so ${REMOTE_PATH}/
        adb push ${QNN_SDK_PATH}/lib/aarch64-android/libQnnHtpPrepare.so          ${REMOTE_PATH}/
        adb push ${QNN_SDK_PATH}/lib/aarch64-android/libQnnHtpV75Stub.so          ${REMOTE_PATH}/
        adb push ${QNN_SDK_PATH}/lib/hexagon-v75/unsigned/libQnnHtpV75Skel.so     ${REMOTE_PATH}/
}


function build_ggml_qnn_ut()
{
    show_pwd
    check_and_download_ndk
    check_qnn_sdk
    dump_vars
    remove_temp_dir
    build_arm64
}


function run_ggml_qnn_ut()
{
    check_qnn_libs

    #upload the latest ggml_qnn_test
    adb push ${GGML_QNN_UT} ${REMOTE_PATH}
    adb shell chmod +x ${REMOTE_PATH}/${GGML_QNN_UT}

    case "$ggmlop" in
        GGML_OP_ADD)
            echo "adb shell ${REMOTE_PATH}/${GGML_QNN_UT}  -t GGML_OP_ADD -b $qnnbackend"
            adb shell ${REMOTE_PATH}/${GGML_QNN_UT}  -t GGML_OP_ADD -b $qnnbackend
        ;;

        GGML_OP_MUL)
            adb shell ${REMOTE_PATH}/${GGML_QNN_UT}  -t GGML_OP_MUL -b $qnnbackend
        ;;

        GGML_OP_MUL_MAT)
            adb shell ${REMOTE_PATH}/${GGML_QNN_UT}  -t GGML_OP_MUL_MAT -b $qnnbackend
        ;;

        *)
            printf " \n$arg not supported currently\n"
            show_usage
            exit 1
        ;;
    esac
}


function show_usage()
{
    echo "Usage:"
    echo "  $0 build"
    echo "  $0 updateqnnlib"
    echo "  $0 GGML_OP_ADD      0 (QNN_CPU) / 1(QNN_GPU) / 2(QNN_NPU)"
    #echo "  $0 GGML_OP_MUL_MAT  0 (QNN_CPU) / 1(QNN_GPU) / 2(QNN_NPU)"
    echo -e "\n\n\n"
}


unset ggmlop
unset qnnbackend

#for this project, re-use the existing ANDROID_NDK in prebuilts/toolchain/android-ndk-r26c directly
show_pwd
if [ "x${PROJECT_ROOT_PATH}" == "x" ]; then
    echo "pwd is `pwd`"
    echo "pls run . build/envsetup in project's toplevel directory firstly"
    exit 1
fi
. ${PROJECT_ROOT_PATH}/build/public.sh || (echo "can't find public.sh"; exit 1)

check_qnn_sdk

if [ $# == 0 ]; then
    show_usage
    exit 1
elif [ $# == 1 ]; then
    if [ "$1" == "-h" ]; then
        #avoid upload command line program to Android phone in this scenario
        show_usage
        exit 1
    elif [ "$1" == "help" ]; then
        #avoid upload command line program to Android phone in this scenario
        show_usage
        exit 1
    elif [ "$1" == "build" ]; then
        build_ggml_qnn_ut
        exit 0
    elif [ "$1" == "updateqnnlib" ]; then
        update_qnn_libs
        exit 0
    else
        ggmlop=$1
        qnnbackend=0
    fi
elif [ $# == 2 ]; then
    ggmlop=$1
    qnnbackend=$2
    run_ggml_qnn_ut
else
    show_usage
    exit 1
fi
