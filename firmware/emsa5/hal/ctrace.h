/**
  * Hardware Abstraction Layer for TRISTAN TRACE_FS demonstrator.
  *
  * (c) 2025-2026 AbsInt Angewandte Informatik GmbH
  *
  * Author:  Simon Wegener
  * License: MIT
  */

#ifndef CTRACE_H
#define CTRACE_H

#include "ct_csr.h"
#include "csr.h"

// address of ACT-CAP command register in CSR space
#define CTRACE_ACT_CAP_CMD_CSR 0xb10

// Build the command word in C
static inline uint32_t __builtin_ctrace_build_command_word (uint32_t command, uint32_t sink, uint32_t data)
{
    uint32_t const _command = (command << CT_CSR_TRACTCAPSTCMD_CMD_Pos) & CT_CSR_TRACTCAPSTCMD_CMD_Msk;
    uint32_t const _sink = (sink << CT_CSR_TRACTCAPSTCMD_SINK_Pos) & CT_CSR_TRACTCAPSTCMD_SINK_Msk;
    uint32_t const _data = (data << CT_CSR_TRACTCAPSTCMD_DIRECTDATA_Pos) & CT_CSR_TRACTCAPSTCMD_DIRECTDATA_Msk;

    return _data | _sink | _command;
}

// example from C-Trace Application Note
static inline void __builtin_ctrace_emit_marker (uint8_t tag)
{
    // Command: DAQ_PC_CURR
    // Sink: NEXUS only
    // Attach user tag in the low 8 bits of DirectData
    uint32_t w = __builtin_ctrace_build_command_word(CT_CSR_TRACTCAPSTCMD_CMD_ACT_CAP_ST_DAQ_PC_CURR, CT_CSR_TRACTCAPSTCMD_SINK_ACT_CAP_ST_SINK_NEXUS, tag);
    __builtin_csr_write(CTRACE_ACT_CAP_CMD_CSR, w);
}

static inline void __builtin_ctrace_emit_pc (void)
{
    __builtin_csr_write(CTRACE_ACT_CAP_CMD_CSR, (CT_CSR_TRACTCAPSTCMD_CMD_ACT_CAP_ST_DAQ_PC_CURR << CT_CSR_TRACTCAPSTCMD_CMD_Pos) | (CT_CSR_TRACTCAPSTCMD_SINK_ACT_CAP_ST_SINK_NEXUS << CT_CSR_TRACTCAPSTCMD_SINK_Pos));
}

static inline void __builtin_ctrace_emit_nop (void)
{
    __builtin_csr_write(CTRACE_ACT_CAP_CMD_CSR, (CT_CSR_TRACTCAPSTCMD_CMD_ACT_CAP_ST_NONE << CT_CSR_TRACTCAPSTCMD_CMD_Pos) | (CT_CSR_TRACTCAPSTCMD_SINK_ACT_CAP_ST_SINK_NEXUS << CT_CSR_TRACTCAPSTCMD_SINK_Pos));
}

static inline void __builtin_ctrace_emit_sync (void)
{
    __builtin_csr_write(CTRACE_ACT_CAP_CMD_CSR, (CT_CSR_TRACTCAPSTCMD_CMD_ACT_CAP_ST_CF_SYNC << CT_CSR_TRACTCAPSTCMD_CMD_Pos) | (CT_CSR_TRACTCAPSTCMD_SINK_ACT_CAP_ST_SINK_NEXUS << CT_CSR_TRACTCAPSTCMD_SINK_Pos));
}

static inline void __builtin_ctrace_emit_data_raw (uint32_t data)
{
    uint32_t w = __builtin_ctrace_build_command_word(CT_CSR_TRACTCAPSTCMD_CMD_ACT_CAP_ST_DAQ_DIRECT_DATA, CT_CSR_TRACTCAPSTCMD_SINK_ACT_CAP_ST_SINK_NEXUS, data);
    __builtin_csr_write(CTRACE_ACT_CAP_CMD_CSR, w);
}

//
// simple data-transfer protocol on top of raw direct data DAQ messages
// bits[23:16] of the direct data are used to indicate the type of the data
// data is only valid according to this protocol if at least one bit of the indicator byte is non-zero
//
#define CTRACE_DATA_WIDTH_MASK          (0x0f0000)
#define CTRACE_DATA_TYPE_MASK           (0x300000)
#define CTRACE_DATA_SEQUENCE_MASK       (0x400000)
#define CTRACE_DATA_OWNERSHIP_MASK      (0x800000)
#define CTRACE_DATA_8BIT_MARKER         ((0x01 << 16) & CTRACE_DATA_WIDTH_MASK)
#define CTRACE_DATA_16BIT_MARKER        ((0x02 << 16) & CTRACE_DATA_WIDTH_MASK)
#define CTRACE_DATA_32BIT_MARKER        ((0x04 << 16) & CTRACE_DATA_WIDTH_MASK)
#define CTRACE_DATA_64BIT_MARKER        ((0x08 << 16) & CTRACE_DATA_WIDTH_MASK)
#define CTRACE_DATA_PTR_MARKER          ((0x00 << 16) & CTRACE_DATA_TYPE_MASK)
#define CTRACE_DATA_INT_MARKER          ((0x10 << 16) & CTRACE_DATA_TYPE_MASK)
#define CTRACE_DATA_UINT_MARKER         ((0x20 << 16) & CTRACE_DATA_TYPE_MASK)
#define CTRACE_DATA_FLOAT_MARKER        ((0x30 << 16) & CTRACE_DATA_TYPE_MASK)
#define CTRACE_DATA_SEQ_END_MARKER      ((0x00 << 16) & CTRACE_DATA_SEQUENCE_MASK)
#define CTRACE_DATA_SEQ_CONTINUE_MARKER ((0x40 << 16) & CTRACE_DATA_SEQUENCE_MASK)
#define CTRACE_DATA_OWNERSHIP_MARKER    ((0x80 << 16) & CTRACE_DATA_OWNERSHIP_MASK)

static inline void __builtin_ctrace_emit_data_u8 (uint8_t const data)
{
    __builtin_ctrace_emit_data_raw(CTRACE_DATA_SEQ_END_MARKER | CTRACE_DATA_UINT_MARKER | CTRACE_DATA_8BIT_MARKER | data);
}

static inline void __builtin_ctrace_emit_data_u16 (uint16_t const data)
{
    __builtin_ctrace_emit_data_raw(CTRACE_DATA_SEQ_END_MARKER | CTRACE_DATA_UINT_MARKER | CTRACE_DATA_16BIT_MARKER | data);
}

static inline void __builtin_ctrace_emit_data_u32 (uint32_t const data)
{
    uint16_t const word0 = (uint16_t)((data >>  0) & 0xffff);
    uint16_t const word1 = (uint16_t)((data >> 16) & 0xffff);
    __builtin_ctrace_emit_data_raw(CTRACE_DATA_SEQ_CONTINUE_MARKER | CTRACE_DATA_UINT_MARKER | CTRACE_DATA_32BIT_MARKER | word0);
    __builtin_ctrace_emit_data_raw(CTRACE_DATA_SEQ_END_MARKER      | CTRACE_DATA_UINT_MARKER | CTRACE_DATA_32BIT_MARKER | word1);
}

static inline void __builtin_ctrace_emit_data_u64 (uint64_t const data)
{
    uint16_t const word0 = (uint16_t)((data >>  0) & 0xffff);
    uint16_t const word1 = (uint16_t)((data >> 16) & 0xffff);
    uint16_t const word2 = (uint16_t)((data >> 32) & 0xffff);
    uint16_t const word3 = (uint16_t)((data >> 48) & 0xffff);
    __builtin_ctrace_emit_data_raw(CTRACE_DATA_SEQ_CONTINUE_MARKER | CTRACE_DATA_UINT_MARKER | CTRACE_DATA_64BIT_MARKER | word0);
    __builtin_ctrace_emit_data_raw(CTRACE_DATA_SEQ_CONTINUE_MARKER | CTRACE_DATA_UINT_MARKER | CTRACE_DATA_64BIT_MARKER | word1);
    __builtin_ctrace_emit_data_raw(CTRACE_DATA_SEQ_CONTINUE_MARKER | CTRACE_DATA_UINT_MARKER | CTRACE_DATA_64BIT_MARKER | word2);
    __builtin_ctrace_emit_data_raw(CTRACE_DATA_SEQ_END_MARKER      | CTRACE_DATA_UINT_MARKER | CTRACE_DATA_64BIT_MARKER | word3);
}

static inline void __builtin_ctrace_emit_data_s8 (int8_t const data)
{
    __builtin_ctrace_emit_data_raw(CTRACE_DATA_SEQ_END_MARKER | CTRACE_DATA_INT_MARKER | CTRACE_DATA_8BIT_MARKER | (uint8_t)data);
}

static inline void __builtin_ctrace_emit_data_s16 (int16_t const data)
{
    __builtin_ctrace_emit_data_raw(CTRACE_DATA_SEQ_END_MARKER | CTRACE_DATA_INT_MARKER | CTRACE_DATA_16BIT_MARKER | (uint16_t)data);
}

static inline void __builtin_ctrace_emit_data_s32 (int32_t const data)
{
    uint16_t const word0 = (uint16_t)((data >>  0) & 0xffff);
    uint16_t const word1 = (uint16_t)((data >> 16) & 0xffff);
    __builtin_ctrace_emit_data_raw(CTRACE_DATA_SEQ_CONTINUE_MARKER | CTRACE_DATA_INT_MARKER | CTRACE_DATA_32BIT_MARKER | word0);
    __builtin_ctrace_emit_data_raw(CTRACE_DATA_SEQ_END_MARKER      | CTRACE_DATA_INT_MARKER | CTRACE_DATA_32BIT_MARKER | word1);
}

static inline void __builtin_ctrace_emit_data_s64 (int64_t const data)
{
    uint16_t const word0 = (uint16_t)((data >>  0) & 0xffff);
    uint16_t const word1 = (uint16_t)((data >> 16) & 0xffff);
    uint16_t const word2 = (uint16_t)((data >> 32) & 0xffff);
    uint16_t const word3 = (uint16_t)((data >> 48) & 0xffff);
    __builtin_ctrace_emit_data_raw(CTRACE_DATA_SEQ_CONTINUE_MARKER | CTRACE_DATA_INT_MARKER | CTRACE_DATA_64BIT_MARKER | word0);
    __builtin_ctrace_emit_data_raw(CTRACE_DATA_SEQ_CONTINUE_MARKER | CTRACE_DATA_INT_MARKER | CTRACE_DATA_64BIT_MARKER | word1);
    __builtin_ctrace_emit_data_raw(CTRACE_DATA_SEQ_CONTINUE_MARKER | CTRACE_DATA_INT_MARKER | CTRACE_DATA_64BIT_MARKER | word2);
    __builtin_ctrace_emit_data_raw(CTRACE_DATA_SEQ_END_MARKER      | CTRACE_DATA_INT_MARKER | CTRACE_DATA_64BIT_MARKER | word3);
}

static inline void __builtin_ctrace_emit_data_f32 (float const data)
{
    union { float f; uint32_t i; } convert;
    convert.f = data;
    uint16_t const word0 = (uint16_t)((convert.i >>  0) & 0xffff);
    uint16_t const word1 = (uint16_t)((convert.i >> 16) & 0xffff);
    __builtin_ctrace_emit_data_raw(CTRACE_DATA_SEQ_CONTINUE_MARKER | CTRACE_DATA_FLOAT_MARKER | CTRACE_DATA_32BIT_MARKER | word0);
    __builtin_ctrace_emit_data_raw(CTRACE_DATA_SEQ_END_MARKER      | CTRACE_DATA_FLOAT_MARKER | CTRACE_DATA_32BIT_MARKER | word1);
}

static inline void __builtin_ctrace_emit_data_f64 (double const data)
{
    union { double f; uint64_t i; } convert;
    convert.f = data;
    uint16_t const word0 = (uint16_t)((convert.i >>  0) & 0xffff);
    uint16_t const word1 = (uint16_t)((convert.i >> 16) & 0xffff);
    uint16_t const word2 = (uint16_t)((convert.i >> 32) & 0xffff);
    uint16_t const word3 = (uint16_t)((convert.i >> 48) & 0xffff);
    __builtin_ctrace_emit_data_raw(CTRACE_DATA_SEQ_CONTINUE_MARKER | CTRACE_DATA_FLOAT_MARKER | CTRACE_DATA_64BIT_MARKER | word0);
    __builtin_ctrace_emit_data_raw(CTRACE_DATA_SEQ_CONTINUE_MARKER | CTRACE_DATA_FLOAT_MARKER | CTRACE_DATA_64BIT_MARKER | word1);
    __builtin_ctrace_emit_data_raw(CTRACE_DATA_SEQ_CONTINUE_MARKER | CTRACE_DATA_FLOAT_MARKER | CTRACE_DATA_64BIT_MARKER | word2);
    __builtin_ctrace_emit_data_raw(CTRACE_DATA_SEQ_END_MARKER      | CTRACE_DATA_FLOAT_MARKER | CTRACE_DATA_64BIT_MARKER | word3);
}

static inline void __builtin_ctrace_emit_data_p32 (void * const data)
{
    union { void * ptr; uint32_t i; } convert;
    convert.ptr = data;
    uint16_t const word0 = (uint16_t)((convert.i >>  0) & 0xffff);
    uint16_t const word1 = (uint16_t)((convert.i >> 16) & 0xffff);
    __builtin_ctrace_emit_data_raw(CTRACE_DATA_SEQ_CONTINUE_MARKER | CTRACE_DATA_PTR_MARKER | CTRACE_DATA_32BIT_MARKER | word0);
    __builtin_ctrace_emit_data_raw(CTRACE_DATA_SEQ_END_MARKER      | CTRACE_DATA_PTR_MARKER | CTRACE_DATA_32BIT_MARKER | word1);
}

//
// AbsInt Use Cases (see CI #679107)
//

// Use case 1: Use HSI to fix loop scaling conflicts
static inline void __builtin_ctrace_add_trace_point (void)
{
    __builtin_ctrace_emit_sync();
}

// Use case 2: Use HSI for ownership trace
static inline void __builtin_ctrace_ownership_trace (uint16_t id)
{
    __builtin_ctrace_emit_data_raw(CTRACE_DATA_OWNERSHIP_MARKER | id);
}

#endif /* CTRACE_H */
