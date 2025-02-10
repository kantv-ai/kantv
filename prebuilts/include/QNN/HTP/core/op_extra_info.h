//==============================================================================
//
// Copyright (c) 2020-2023 Qualcomm Technologies, Inc.
// All Rights Reserved.
// Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//==============================================================================

#ifndef OP_EXTRA_INFO_H
#define OP_EXTRA_INFO_H 1

#include <utility>

#include "interface_defs.h"

namespace hnnx {

/*
    map Op* to a few properties, to avoid the need to keep them in the Op
    object. Currently contains the ID, the gate/done checkpoint indices, and
    the number of scratch outputs. This sctruct is part of the runlist and its 
    memory footprint is important. It is currently 24 bytes:
        opid: 8 bytes
        chkpts: 8 bytes
        for_hlx: 1 bit
        num_scratch_outputs: 5 bits
        padding: 2 bits + 3 bytes
        op_tag: 4 bytes
*/
struct OpExtraInfo {
    using Chkpts = std::pair<int, int>;

    OpId id;
    Chkpts chkpts;
    bool for_hlx : 1;
    unsigned int num_scratch_outputs : 5; // Only valid at prepare time
    const char *op_tag;
    explicit OpExtraInfo(OpId id_in) : id(id_in), chkpts(-1, -1), for_hlx(false), num_scratch_outputs(0) {}
    OpExtraInfo(OpId id_in, int cg, int dc) : id(id_in), chkpts(cg, dc), for_hlx(false), num_scratch_outputs(0) {}
    OpExtraInfo() : OpExtraInfo(0) {}

    bool valid() const { return id != 0; };
    void clear() { id = 0; };
};

} // namespace hnnx

#endif // OP_EXTRA_INFO_H
