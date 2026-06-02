#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "ctrace.h"
#include "ctrace_test.h"

void ctrace_test_main (void)
{
    static uint32_t counter = 0;

    if (counter % 8 == 0) {
        __builtin_ctrace_emit_marker(0xde);
        __builtin_ctrace_emit_marker(0xad);
        __builtin_ctrace_emit_marker(0xbe);
        __builtin_ctrace_emit_marker(0xef);
    } else if (counter % 8 == 1) {
        __builtin_ctrace_emit_pc();
        __builtin_ctrace_emit_nop();
#if 0
        __builtin_ctrace_emit_sync();
#endif
    } else if (counter % 8 == 2) {
        __builtin_ctrace_emit_data_u8(0);
        __builtin_ctrace_emit_data_u16(0);
        __builtin_ctrace_emit_data_u32(0);
        __builtin_ctrace_emit_data_u64(0);
    } else if (counter % 8 == 3) {
        __builtin_ctrace_emit_data_u8(UINT8_MAX);
        __builtin_ctrace_emit_data_u16(UINT16_MAX);
        __builtin_ctrace_emit_data_u32(UINT32_MAX);
        __builtin_ctrace_emit_data_u64(UINT64_MAX);
    } else if (counter % 8 == 4) {
        __builtin_ctrace_emit_data_s8(INT8_MIN);
        __builtin_ctrace_emit_data_s16(INT16_MIN);
        __builtin_ctrace_emit_data_s32(INT32_MIN);
        __builtin_ctrace_emit_data_s64(INT64_MIN);
    } else if (counter % 8 == 5) {
        __builtin_ctrace_emit_data_s8(INT8_MAX);
        __builtin_ctrace_emit_data_s16(INT16_MAX);
        __builtin_ctrace_emit_data_s32(INT32_MAX);
        __builtin_ctrace_emit_data_s64(INT64_MAX);
    } else if (counter % 8 == 6) {
        __builtin_ctrace_emit_data_u32(0xdeadbeef);

        uint32_t available_extensions = __builtin_csr_read(CSR_MISA);
        if (available_extensions & (1 << 5))
            __builtin_ctrace_emit_data_f32(0.5f);
        if (available_extensions & (1 << 3))
            __builtin_ctrace_emit_data_f64(2.2);
    } else if (counter % 8 == 7) {
        __builtin_ctrace_emit_data_p32(NULL);
        __builtin_ctrace_emit_data_p32(&ctrace_test_main);
    }

    counter++;
}
