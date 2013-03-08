#include <kernel.h>

elf_t kernel_elf;
extern void pci_scan();
extern void ide_initialize(uint32_t BAR0, uint32_t BAR1, uint32_t BAR2, uint32_t BAR3, uint32_t BAR4);

int kernel_main(multiboot_t *mb)
{
  init_gdt();
  init_idt();

  init_console();

  init_timer(50);

  kernel_elf = elf_from_multiboot(mb);

  // Init memory manager
  init_paging();
  init_heap();

  asm volatile("sti");

  //pci_scan();
  ide_initialize(0x1F0, 0x3F4, 0x170, 0x374, 0x000);

  for(;;);

  return 0xba110017;
}
