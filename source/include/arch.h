#ifndef ARCH_H
#define ARCH_H

typedef struct registers
{
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

typedef struct
{
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t base_middle;
  uint8_t access;
  uint8_t granularity;
  uint8_t base_high;

} __attribute__((packed)) gdt_entry_t;

typedef struct
{
  uint16_t limit;
  uint32_t base;
} __attribute__((packed)) gdt_ptr_t;

void init_gdt();


typedef struct
{
  uint16_t base_lo;
  uint16_t sel;
  uint8_t  always0;
  uint8_t  flags;
  uint16_t base_hi;
} __attribute__((packed)) idt_entry_t;

typedef struct
{
  uint16_t limit;
  uint32_t base;
} __attribute__((packed)) idt_ptr_t;

typedef void (*interrupt_handler_t)(registers_t *);
void register_interrupt_handler(uint8_t n, interrupt_handler_t h);

extern void isr0 ();
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void isr255();

#endif
