#include <timer.h> 
#include <debug.h> 
#include <common.h> 
#include <idt.h> 
#include <console.h>

void timer_callback(pt_regs *regs)
{
    static uint32_t tick = 0;
    printk_color(rc_black, rc_light_cyan, "Tick: %d\n", tick++);
}

void init_timer(uint32_t frequency)
{
    // 注册时间相关的处理函数
    register_interrupt_handler(IRQ0, timer_callback);

    // Intel 8253/8254 PIT芯片 I/O 端口地址范围是 40h~43h
    // 输入频率为 1193180，frequency 即每秒中断次数
    uint32_t divisor = 1193180 / frequency;

    outb(0x43, 0x36);

    uint8_t low = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

    outb(0x40, low);
    outb(0x40, high);
}
