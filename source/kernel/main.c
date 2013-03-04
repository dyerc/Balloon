#include <kernel.h>

int kernel_main(multiboot_t *mb)
{
  init_gdt();
  init_idt();

  init_console();

  init_timer(20);

  uint32_t a = kmalloc(64);
  uint32_t b = kmalloc(64);
  uint32_t c = kmalloc(128);

  kprintf("1: %x\n", a);
  kprintf("2: %x\n", b);
  kprintf("3: %x\n", c);

  asm volatile("sti");

  //asm volatile ("int $0x4");
}
