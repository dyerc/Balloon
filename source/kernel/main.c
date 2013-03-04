#include <kernel.h>

elf_t kernel_elf;

int kernel_main(multiboot_t *mb)
{
  init_gdt();
  init_idt();

  init_console();

  //init_timer(20);

  kernel_elf = elf_from_multiboot(mb);

  // Init memory manager
  init_paging();

  asm volatile("sti");

  uint32_t *ptr = (uint32_t*)0xA000000;
  uint32_t do_fault = *ptr;

  for(;;);

  return 0xba110017;
}
