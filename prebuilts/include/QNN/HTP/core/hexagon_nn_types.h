#pragma once
//==============================================================================
// @brief Collection of types used by various external/API headers
//
// Copyright (c) 2024 Qualcomm Technologies, Inc.
// All Rights Reserved.
// Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//==============================================================================
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// We need this so, as on Windows, long is just 32-bits.  This way, Long is consistently 64-bits on
// 64-bit architectures (x86, aarch64 on Linux, Android, Windows, QNX, etc.).
typedef ptrdiff_t Long;

///
/// @brief Max number of PMU events HexNN can sample
///
#define HEXAGON_NN_MAX_PMU_EVENTS 8

///
/// @brief Type for 64b (virtual) address
///
typedef uint64_t hexagon_nn_wide_address_t;

///
/// @brief A visual marker for an address whose contents (the thing this points
/// to) are immutable
/// @details For example a pointer to a shared weights table. The table has a
/// list of near/far pointers whose contents (weights) are considered immutable
///
typedef uint64_t hexagon_nn_wide_address_const_t;

///
/// @brief Used to specify thread types when calling hexagon_nn_set_thread_count
/// and hexagon_nn_get_thread_count.
///
enum hexagon_nn_thread_type_t {
    // Use these enums to specify the type of thread for hexagon_nn_set_thread_count.
    VecThread = 0,
    MtxThread = 1,
    EltThread = 2,
    // Use this for `count` to specify that the maximum available number of threads should be used.
    MaxOsThreads = 1001,
};

enum MemContentType {
    Standard = 0,
    Weight = 1,
    WeightDLBC = 2,
    WeightReplaceable = 3,
};

#ifdef __cplusplus
}
#endif
