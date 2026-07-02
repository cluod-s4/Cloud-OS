// core/kernel/interrupts/irq.c
// Real IRQ implementation

#include "irq.h"
#include "idt.h"
#include "drivers/screen.h"
#include <stdint.h>

#define PIC1 0x20
#define PIC2 0xA0
#define ICW1 0x11
#define ICW4 0x01

static irq_handler_t irq_handlers[16] = {0};

void irq_remap() {
    outb(PIC1, ICW1);
    outb(PIC2, ICW1);
    outb(PIC1 + 1, 0x20);
    outb(PIC2 + 1, 0x28);
    outb(PIC1 + 1, 0x04);
    outb(PIC2 + 1, 0x02);
    outb(PIC1 + 1, ICW4);
    outb(PIC2 + 1, ICW4);
    outb(PIC1 + 1, 0x00);
    outb(PIC2 + 1, 0x00);
}

void irq_register_handler(int irq, irq_handler_t handler) {
    if (irq >= 0 && irq < 16) {
        irq_handlers[irq] = handler;
    }
}

void irq_handler_wrapper(registers_t regs) {
    int irq = regs.int_no - 32;
    
    if (irq_handlers[irq]) {
        irq_handlers[irq](&regs);
    }
    
    if (irq >= 8) {
        outb(PIC2, 0x20);
    }
    outb(PIC1, 0x20);
}

void irq_init() {
    irq_remap();
    
    idt_set_gate(32, (uint32_t)irq0, 0x08, 0x8E);
    idt_set_gate(33, (uint32_t)irq1, 0x08, 0x8E);
    idt_set_gate(34, (uint32_t)irq2, 0x08, 0x8E);
    idt_set_gate(35, (uint32_t)irq3, 0x08, 0x8E);
    idt_set_gate(36, (uint32_t)irq4, 0x08, 0x8E);
    idt_set_gate(37, (uint32_t)irq5, 0x08, 0x8E);
    idt_set_gate(38, (uint32_t)irq6, 0x08, 0x8E);
    idt_set_gate(39, (uint32_t)irq7, 0x08, 0x8E);
    idt_set_gate(40, (uint32_t)irq8, 0x08, 0x8E);
    idt_set_gate(41, (uint32_t)irq9, 0x08, 0x8E);
    idt_set_gate(42, (uint32_t)irq10, 0x08, 0x8E);
    idt_set_gate(43, (uint32_t)irq11, 0x08, 0x8E);
    idt_set_gate(44, (uint32_t)irq12, 0x08, 0x8E);
    idt_set_gate(45, (uint32_t)irq13, 0x08, 0x8E);
    idt_set_gate(46, (uint32_t)irq14, 0x08, 0x8E);
    idt_set_gate(47, (uint32_t)irq15, 0x08, 0x8E);
    
    screen_print("[+] IRQ initialized\n");
}
