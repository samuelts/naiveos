#ifndef __NAIVEOS_ARCH_INCLUDE_GDT_H
#define __NAIVEOS_ARCH_INCLUDE_GDT_H

#include "types.h"

/* 全局描述符类型 */
typedef
struct gdt_entry_t {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed)) gdt_entry_t;

/* gdt */
typedef
struct gdt_ptr_t {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) gdt_ptr_t;

void init_gdt();

extern void gdt_flush(uint32_t);

#endif  // !__NAIVEOS_ARCH_INCLUDE_GDT_H