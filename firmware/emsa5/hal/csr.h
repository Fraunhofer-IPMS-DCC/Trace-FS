#ifndef CSR_H
#define CSR_H

#define CSR_MSTATUS  0x300
#define CSR_MISA     0x301
#define CSR_MIE      0x304
#define CSR_MCYCLE   0xb00
#define CSR_MTIME    0xb01
#define CSR_MTIMECMP 0xbc0

typedef unsigned csr_t;

#ifndef __COMPCERT__

static inline uint32_t __builtin_csr_read(csr_t csr)
{
    uint32_t rd;
    asm volatile("csrr %0, %1" : "=r" (rd) : "i" (csr));
    return rd;
}

static inline void __builtin_csr_write(csr_t csr, uint32_t rs)
{
    asm volatile("csrw%i1 %0, %1" : : "i" (csr), "rK" (rs));
}

static inline uint32_t __builtin_csr_readwrite(csr_t csr, uint32_t rs)
{
    uint32_t rd;
    asm volatile("csrrw%i2 %0, %1, %2" : "=r" (rd) : "i" (csr), "rK" (rs));
    return rd;
}

static inline uint32_t __builtin_csr_clear_bits(csr_t csr, uint32_t rs)
{
    uint32_t rd;
    asm volatile("csrrc%i2 %0, %1, %2" : "=r" (rd) : "i" (csr), "rK" (rs));
    return rd;
}

static inline uint32_t __builtin_csr_set_bits(csr_t csr, uint32_t rs)
{
    uint32_t rd;
    asm volatile("csrrs%i2 %0, %1, %2" : "=r" (rd) : "i" (csr), "rK" (rs));
    return rd;
}

#endif /* __COMPCERT__ */

#endif /* CSR_H */
