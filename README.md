# KanTV

KanTV("Kan", aka Chinese PinYin "Kan" or Chinese HanZi "看" or English "watch/listen") , an open source project focus on study and practise state-of-the-art AI technology in <b>real scenario</b>(such as online-TV playback and online-TV transcription(real-time subtitle) and online-TV language translation and online-TV video&audio recording works at the same time) on **Android phone/device**, derived from original ![ijkplayer](https://github.com/zhouwg/kantv/tree/kantv-initial) , with much enhancements and new features:

- Watch online TV and local media by my customized ![FFmpeg 6.1](https://github.com/zhouwg/FFmpeg), source code of my customized FFmpeg 6.1 could be found in <a href="https://github.com/zhouwg/kantv/tree/master/external/ffmpeg-6.1"> external/ffmpeg </a>according to <a href="https://ffmpeg.org/legal.html">FFmpeg's license</a>

- Record online TV to automatically generate videos (useful for short video creators to generate short video materials but pls respect IPR of original content creator/provider); record online TV's video / audio content for gather video / audio data which might be required of/useful for AI R&D activity

- AI subtitle(real-time English subtitle for English online-TV(aka OTT TV) by the great & excellent & amazing<a href="https://github.com/ggerganov/whisper.cpp"> whisper.cpp </a>), pls attention Xiaomi 14 or other powerful Android mobile phone is HIGHLY required/recommended for AI subtitle feature otherwise unexpected behavior would happen

- 2D graphic performance

- Set up a customized playlist and then use this software to watch the content of the customized playlist for R&D activity

- UI refactor(closer to real commercial Android application and only English is supported in UI language currently)

- Well-maintained "workbench" for ASR(Automatic Speech Recognition) researchers/developers/programmers who was interested in practise state-of-the-art AI tech(such as [whisper.cpp](https://github.com/ggerganov/whisper.cpp)) in <b>real scenario on Android phone/device</b>(<a href="https://github.com/zhouwg/kantv/issues/64">PoC: realtime AI subtitle for online-TV(aka OTT TV) on Xiaomi 14 finished from 03/05/2024 to 03/16/2024</a>)


- Well-maintained "workbench" for LLM(Large Language Model) researchers/developers who was interested in practise state-of-the-art AI tech(such as [llama.cpp](https://github.com/ggerganov/llama.cpp)) in real scenario on Android phone/device, or Run/experience LLM model(such as llama-2-7b, baichuan2-7b, qwen1_5-1_8b, gemma-2b) on Android phone/device using the magic llama.cpp

- Well-maintained "workbench" for <a href="https://github.com/ggerganov/ggml">GGML</a> beginners to study internal mechanism of GGML inference framework on Android phone/device(<a href="https://github.com/zhouwg/kantv/issues/121">PoC:Qualcomm QNN backend for ggml finished from 03/29/2024 to 04/26/2024</a>)

- Well-maintained <b>turn-key / self-contained</b> project for AI researchers(whom mightbe not familiar with <b>regular Android software development</b>)/developers/beginners focus on edge/device-side AI learning / R&D activity, some AI R&D activities (AI algorithm validation / AI model validation / performance benchmark in ASR, LLM, TTS, NLP, CV......field) could be done by Android Studio IDE + a powerful Android phone very easily


### Software architecture of KanTV Android

(depend on https://github.com/zhouwg/kantv/issues/121)

![kantv-software-arch](https://github.com/zhouwg/kantv/assets/6889919/a75109c2-ee9d-4cec-b70d-08e5369a6e62)


### How to build project


#### Fetch source codes

```

git clone https://github.com/zhouwg/kantv.git

cd kantv

git checkout master

cd kantv

```

#### Setup development environment

##### Option 1: Setup docker environment

- Build docker image
  ```shell
  docker build build -t kantv --build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g) --build-arg USER_NAME=$(whoami)
  ```

- Run docker container
  ```shell
  # map source code directory into docker container
  docker run -it --name=kantv --volume=`pwd`:/home/`whoami`/kantv kantv

  # in docker container
  . build/envsetup.sh

  ./build/prebuild-download.sh
  ```

##### Option 2: Setup local environment


  - <details>
      <summary>Prerequisites</summary>

      <ol>

        Host OS information:

    ```
    uname -a

    Linux 5.8.0-43-generic #49~20.04.1-Ubuntu SMP Fri Feb 5 09:57:56 UTC 2021 x86_64 x86_64 x86_64 GNU/Linux

    cat /etc/issue

    Ubuntu 20.04.2 LTS \n \l

    ```
    - tools & utilities
    ```
    sudo apt-get update
    sudo apt-get install build-essential -y
    sudo apt-get install cmake -y
    sudo apt-get install curl -y
    sudo apt-get install wget -y
    sudo apt-get install python -y
    sudo apt-get install tcl expect -y
    sudo apt-get install nginx -y
    sudo apt-get install git -y
    sudo apt-get install vim -y
    sudo apt-get install spawn-fcgi -y
    sudo apt-get install u-boot-tools -y
    sudo apt-get install ffmpeg -y
    sudo apt-get install openssh-client -y
    sudo apt-get install nasm -y
    sudo apt-get install yasm -y
    sudo apt-get install openjdk-17-jdk -y

    sudo dpkg --add-architecture i386
    sudo apt-get install lib32z1 -y

    sudo apt-get install -y android-tools-adb android-tools-fastboot autoconf \
            automake bc bison build-essential ccache cscope curl device-tree-compiler \
            expect flex ftp-upload gdisk acpica-tools libattr1-dev libcap-dev \
            libfdt-dev libftdi-dev libglib2.0-dev libhidapi-dev libncurses5-dev \
            libpixman-1-dev libssl-dev libtool make \
            mtools netcat python-crypto python3-crypto python-pyelftools \
            python3-pycryptodome python3-pyelftools python3-serial \
            rsync unzip uuid-dev xdg-utils xterm xz-utils zlib1g-dev

    sudo apt-get install python3-pip -y
    sudo apt-get install indent -y
    pip3 install meson ninja

    echo "export PATH=/home/`whoami`/.local/bin:\$PATH" >> ~/.bashrc

    ```

    or run below script accordingly after fetch project's source code

    ```

    ./build/prebuild.sh


    ```

    - Android Studio

      download and install Android Studio manually

      [Android Studio 4.2.1 or latest Android Studio](https://developer.android.google.cn/studio)


    - vim settings


    borrow from http://ffmpeg.org/developer.html#Editor-configuration

    ```
    set ai
    set nu
    set expandtab
    set tabstop=4
    set shiftwidth=4
    set softtabstop=4
    set noundofile
    set nobackup
    set fileformat=unix
    set undodir=~/.undodir
    set cindent
    set cinoptions=(0
    " Allow tabs in Makefiles.
    autocmd FileType make,automake set noexpandtab shiftwidth=8 softtabstop=8
    " Trailing whitespace and tabs are forbidden, so highlight them.
    highlight ForbiddenWhitespace ctermbg=red guibg=red
    match ForbiddenWhitespace /\s\+$\|\t/
    " Do not highlight spaces at the end of line while typing on that line.
    autocmd InsertEnter * match ForbiddenWhitespace /\t\|\s\+\%#\@<!$/

    ```
      </ol>
    </details>


 - Download android-ndk-r26c to prebuilts/toolchain, skip this step if android-ndk-r26c is already exist
    ```
    . build/envsetup.sh

    ./build/prebuild-download.sh

    ```


 - Modify <a href="https://github.com/zhouwg/kantv/blob/master/core/ggml/CMakeLists.txt#L14">ggml/CMakeLists.txt</a> accordingly if target Android device is Xiaomi 14 or Qualcomm Snapdragon 8 Gen 3 SoC based Android phone

 - Modify <a href="https://github.com/zhouwg/kantv/blob/master/core/ggml/CMakeLists.txt#L15">ggml/CMakeLists.txt</a> accordingly if target Android phone is Qualcomm SoC based Android phone and enable QNN backend for inference framework on Qualcomm SoC based Android phone

 - Remove the hardcoded debug flag in Android NDK <a href="https://github.com/android-ndk/ndk/issues/243">android-ndk issue</a>

    ```

    # open $ANDROID_NDK/build/cmake/android.toolchain.cmake for ndk < r23
    # or $ANDROID_NDK/build/cmake/android-legacy.toolchain.cmake for ndk >= r23
    # delete "-g" line
    list(APPEND ANDROID_COMPILER_FLAGS
    -g
    -DANDROID

    ```



#### Build native codes

```shell
. build/envsetup.sh

```

![Screenshot from 2024-04-07 09-45-04](https://github.com/zhouwg/kantv/assets/6889919/44a1f614-902c-48c1-babc-a73511c3a0f6)


#### Build Android APK

- Option 1: Build APK from source code by Android Studio IDE

- Option 2: Build APK from source code by command line

        . build/envsetup.sh
        lunch 1
        ./build-all.sh android

<!--
- Latest prebuit APK could be found here [![Github](https://user-images.githubusercontent.com/6889919/122489234-c13db400-d011-11eb-9d8c-8e4b2555dabe.png)](https://github.com/zhouwg/kantv/raw/master/release/kantv-latest.apk)(the prebuilt APK sometimes might be not available because generate APK from source code is preferrred).
-->


### Run Android APK on Android phone

This project is a learning&research project, so the Android APK will <a href="./docs/compliance-statement.md">not collect/upload user data in Android device</a>. The Android APK should be works well on any <b>mainstream</b> Android phone(report issue in various Android phone to this project is greatly welcomed) and the following four permissions are required:

- Access to storage is required to generate necessary temporary files
- Access to device information is required to obtain current phone network status information, distinguishing whether the current network is Wi-Fi or mobile when playing online TV
- Access to camera is needed for AI Agent
- Access to mic(audio recorder) is needed for AI Agent

<hr>
here is a short video to demostrate AI subtitle by running the great & excellent & amazing<a href="https://github.com/ggerganov/whisper.cpp"> whisper.cpp </a> on a Xiaomi 14 device - <b>fully offline, on-device</b>.

https://github.com/zhouwg/kantv/assets/6889919/2fabcb24-c00b-4289-a06e-05b98ecd22b8

----

here is a screenshot to demostrate LLM inference by running the magic <a href="https://github.com/ggerganov/llama.cpp"> llama.cpp </a> on a Xiaomi 14 device - <b>fully offline, on-device</b>.


![1204894425](https://github.com/zhouwg/kantv/assets/6889919/68f9fe1a-4328-43a0-a180-7584357d66c7)

----

here is a screenshot to demostrate ASR inference by running the excellent <a href="https://github.com/ggerganov/whisper.cpp"> whisper.cpp </a> on a Xiaomi 14 device - <b>fully offline, on-device</b>.


![840460054](https://github.com/zhouwg/kantv/assets/6889919/ed27055b-0685-43f9-837e-3b99f7648fd1)


----
here are some screenshots to demostrate CV inference by running the excellent <a href="https://github.com/Tencent/ncnn"> ncnn </a> on a Xiaomi 14 device - <b>fully offline, on-device</b>.

![2015869763](https://github.com/zhouwg/kantv/assets/6889919/9b4c8325-8f7c-4bea-9cae-ee4627f9d199)

![988568755](https://github.com/zhouwg/kantv/assets/6889919/49c7e22e-0e4c-4ece-b690-04d59ac1382f)


![1730654667](https://github.com/zhouwg/kantv/assets/6889919/83ef3e44-92ee-4c00-b000-840a13097544)

![1301547964](https://github.com/zhouwg/kantv/assets/6889919/07a4cc76-afa7-40cb-80bc-151e77220f4d)

<details>
  <summary>some other screenshots</summary>
  <ol>

![784269893](https://github.com/zhouwg/kantv/assets/6889919/8fe74b2a-21bc-452c-a6bb-5fb7fb2a567a)
![205726588](https://github.com/zhouwg/kantv/assets/6889919/16411854-c67b-4975-9ca1-fabcfe95a62b)


![1714239572](https://github.com/zhouwg/kantv/assets/6889919/20e23c8d-4656-4bb3-bb84-e0cef71cb532)

![1778831978](https://github.com/zhouwg/kantv/assets/6889919/92774cbc-c716-4819-a0c1-6bc0ae495d1d)

![Screenshot_2024_0304_131033](https://github.com/zhouwg/kantv/assets/6889919/6c5bd531-5577-4570-bc87-aa3a87822d6b)

![154248860](https://github.com/zhouwg/kantv/assets/6889919/071ac55c-a5d7-4bd6-aece-83cbc8a487ff)

![1118975128](https://github.com/zhouwg/kantv/assets/6889919/ef2b256c-02fb-4318-a430-b4cd15ed5b44)
![Screenshot_20240301_000609_com cdeos kantv](https://github.com/zhouwg/kantv/assets/6889919/cf3a77ef-1409-4137-8236-487a8de7fe81)

![1966093505](https://github.com/zhouwg/kantv/assets/6889919/44e6d6c7-0ebb-41c0-a576-9f7457e0dd23)

![1179733910](https://github.com/zhouwg/kantv/assets/6889919/eb6ff245-3f04-4689-b998-2b6b06dec432)

![2138671817](https://github.com/zhouwg/kantv/assets/6889919/513a0676-d2f3-464b-be50-436eabd99715)

![1634808790](https://github.com/zhouwg/kantv/assets/6889919/f69ca9a7-5d25-46da-a160-ab00ff059db9)

![991182277](https://github.com/zhouwg/kantv/assets/6889919/46306999-973c-4fc4-b294-48025acf9cf5)

  </ol>
</details>


### Hot topics

- improve the quality of <a href="https://github.com/zhouwg/kantv/issues/121"> Qualcomm QNN backend for GGML</a>

- improve the performance of edge-AI inference on Android phone

- bugfix in UI layer(Java)

- bugfix in native layer(C/C++)

### Contribution

Be sure to review the [opening issues](https://github.com/zhouwg/kantv/issues?q=is%3Aopen+is%3Aissue) before contribute to project KanTV, We use [GitHub issues](https://github.com/zhouwg/kantv/issues) for tracking requests and bugs, please see [how to submit issue in this project ](https://github.com/zhouwg/kantv/issues/1).

Report issue in various Android-based phone or even submit PR to this project is greatly welcomed.

<!--
 **English** is preferred in this project(avoid similar comments in this project:<a href="https://github.com/torvalds/linux/pull/818" target="_blank">https://github.com/torvalds/linux/pull/818</a>). thanks for cooperation and understanding.
-->

### Docs

- [How to verify Qualcomm QNN backend for GGML on Qualcomm mobile SoC based android device](./docs/how-to-verify-ggml-qnn-backend-on-android.md)
- [How to setup customized KanTV server in local dev env](./docs/how-to-setup-customized-kantvserver-in-local.md)
- [How to create customized playlist for kantv apk](./docs/how-to-create-customized-playlist-in-cloud-server.md)
- [How to integrate proprietary/open source codes to project KanTV for personal/proprietary/commercial R&D activity](https://github.com/zhouwg/kantv/issues/74)
- [How to use whisper.cpp and ffmpeg to add subtitle to video](./docs/how-to-use-whispercpp-ffmpeg-add-subtitle-to-video.md)
- [How to reduce the size of apk](./docs/how-to-reduce-the-size-of-apk.md)
- [How to sign apk](./docs/how-to-sign-apk.md)
- [How to validate AI algorithm/model on Android using this project](./docs/how-to-validate-ai-algorithm-model-on-android-using-this-project.md)
- [Why focus on ggml for Android device](./docs/why-focus-on-ggml-for-android.md)
- [Acknowledgement](./docs/acknowledgement.md)
- [ChangeLog](./release/README.md)
- [F.A.Q](./docs/FAQ.md)


### Special Acknowledgement

 <ul>AI inference framework

   <ul>
  <li>
   <a href="https://github.com/ggerganov/ggml">GGML</a> by <a href="https://github.com/ggerganov">Georgi Gerganov</a>
   </li>


  </ul>

  </ul>

 <ul>AI application engine

  <ul>
  <li>
   ASR engine <a href="https://github.com/ggerganov/whisper.cpp">whisper.cpp</a> by <a href="https://github.com/ggerganov">Georgi Gerganov</a>
  </li>

   <li>
  LLM engine <a href="https://github.com/ggerganov/llama.cpp">llama.cpp</a> by <a href="https://github.com/ggerganov">Georgi Gerganov</a>
  </li>

  </ul>

  </ul>



### License

```
Copyright (c) 2021 - 2023 Project KanTV

Copyright (c) 2024 -  Authors of Project KanTV

Licensed under Apachev2.0 or later
```
