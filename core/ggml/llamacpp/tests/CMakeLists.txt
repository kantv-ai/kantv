# Copyright (c) 2024- KanTV Authors
cmake_minimum_required(VERSION 3.22.1)
project(ggml-qnn-test)

set(CMAKE_VERBOSE_MAKEFILE          on)
set(CMAKE_CXX_STANDARD              17)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#set to ON if target Android phone is equipped with Qualcomm Snapdragon 8 Gen 3
set(TARGET_SNAPDRAGON_8_GEN3        ON)

set(QNN_INC_PATH                    ${QNN_SDK_PATH}/include/QNN)
set(QNN_LIB_PATH                    ${QNN_SDK_PATH}/lib/aarch64-android)
set(LLAMACPP_SRC_DIR                ${CMAKE_SOURCE_DIR}/../)

include_directories(${QNN_INC_PATH})
include_directories(../ggml/include)                # ggml.h
include_directories(../ggml/src)                    # ggml-impl.h
include_directories(../ggml/src)                    # ggml-impl.h
include_directories(../ggml/src/ggml-cpu)           # ggml-cpu-impl.h
include_directories(../ggml/../../llamacpp/common/)          # stb_image.h
include_directories(../../../../prebuilts/include/) # cde_log.h

set(SOURCE_FILES
       ${LLAMACPP_SRC_DIR}/ggml/src/ggml.c
        ${LLAMACPP_SRC_DIR}/ggml/src/ggml-alloc.c
        ${LLAMACPP_SRC_DIR}/ggml/src/ggml-backend.cpp
        ${LLAMACPP_SRC_DIR}/ggml/src/ggml-backend-reg.cpp
        ${LLAMACPP_SRC_DIR}/ggml/src/ggml-quants.c
        ${LLAMACPP_SRC_DIR}/ggml/src/ggml-opt.cpp
        ${LLAMACPP_SRC_DIR}/ggml/src/ggml-threading.cpp
        ${LLAMACPP_SRC_DIR}/ggml/src/gguf.cpp
        ${LLAMACPP_SRC_DIR}/ggml/src/ggml-cpu/cpu-feats-x86.cpp
        ${LLAMACPP_SRC_DIR}/ggml/src/ggml-cpu/ggml-cpu.c
        ${LLAMACPP_SRC_DIR}/ggml/src/ggml-cpu/ggml-cpu.cpp
        ${LLAMACPP_SRC_DIR}/ggml/src/ggml-cpu/ggml-cpu-aarch64.cpp
        ${LLAMACPP_SRC_DIR}/ggml/src/ggml-cpu/ggml-cpu-hbm.cpp
        ${LLAMACPP_SRC_DIR}/ggml/src/ggml-cpu/ggml-cpu-quants.c
        ${LLAMACPP_SRC_DIR}/ggml/src/ggml-cpu/ggml-cpu-traits.cpp
        ${LLAMACPP_SRC_DIR}/ggml/src/ggml-qnn/ggml-qnn.cpp
        ../../../../external/ffmpeg/libavutil/cde_log.c
        ggml-qnn-ut.cpp
)


message("QNN_SDK_PATH         : ${QNN_SDK_PATH}")
message("QNN_INC_PATH         : ${QNN_INC_PATH}")
message("QNN_LIB_PATH         : ${QNN_LIB_PATH}")

add_definitions(-D__ARM_NEON)
add_definitions(-DGGML_USE_QNN)
add_definitions(-DGGML_USE_CPU)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
add_definitions(-DNDEBUG)
add_definitions(-O3)
endif()

if (TARGET_SNAPDRAGON_8_GEN3)
# the below build optimization only verified and works well on Qualcomm SM8650-AB Snapdragon 8 Gen 3
add_definitions(-march=armv8.7-a)
add_definitions(-mcpu=cortex-x1)
add_definitions(-mtune=cortex-x1)

else()
# the below build optimization might be works well on ALL mainstream Android phone based on Qualcomm mobile SoC
add_definitions(-mcpu=cortex-a72)

endif()

add_compile_options("-Wall" "-Wno-sign-compare")

find_library(LOG_LIB log)

link_libraries(${LOG_LIB} android)

add_executable(${TARGET_NAME}
    ${SOURCE_FILES}
)
