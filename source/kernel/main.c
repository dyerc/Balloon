#include <kernel.h>

elf_t kernel_elf;

int kernel_main(multiboot_t *mb)
{
  init_gdt();
  init_idt();

  init_console();

  init_timer(20);

  kernel_elf = elf_from_multiboot(mb);

  // Init memory manager
  init_paging();
  init_heap();

  asm volatile("sti");



  for(;;);

  return 0xba110017;
}
