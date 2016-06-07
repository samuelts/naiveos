#include <gdt.h> 
#include <string.h>

#define GDT_LENGTH 5

gdt_entry_t gdt_entries[GDT_LENGTH];

gdt_ptr_t gdt_ptr; 

static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);

// 声明内核栈地址
extern uint32_t stack;

void init_gdt()
{
    // 全局描述符表界限 e.g. 从 0 开始，所以总长度减 1
    gdt_ptr.limit = sizeof(gdt_entry_t) * GDT_LENGTH - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;

    // 采用 Intel Flat Mode
    gdt_set_gate(0, 0, 0, 0, 0);        // NULL PTR  
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);     // 指令段
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);     // 数据段
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);     // 用户模式代码段
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);     // 用户模式数据段

    // 加载全局描述符表地址到 GPTR 寄存器
    gdt_flush((uint32_t)&gdt_ptr);
}

static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt_entries[num].base_low	    = (base & 0xFFFF);
    gdt_entries[num].base_middle	= (base >> 0x10) & 0xFF;
    gdt_entries[num].base_high      = (base >> 0x18) & 0xFF;

    gdt_entries[num].limit_low      = (limit & 0xFFFF);
    gdt_entries[num].granularity    = (limit >> 0x10) & 0x0F;

    gdt_entries[num].granularity    |= gran & 0xF0;
    gdt_entries[num].access         = access;
}