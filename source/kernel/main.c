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

  void *a = malloc(32);
  void *b = malloc(32);
  void *c = malloc(32);
  kprintf("kmalloced a= %x\n", (uint32_t)a);
  kprintf("kmalloced b= %x\n", (uint32_t)b);
  kprintf("kmalloced c= %x\n", (uint32_t)c);

  free(b);
  b = malloc(32);
  kprintf("kmalloced b= %x\n", (uint32_t)b);

  for(;;);

  return 0xba110017;
}
