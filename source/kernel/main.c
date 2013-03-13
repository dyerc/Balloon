#include <kernel.h>

elf_t kernel_elf;
multiboot_t *multiboot;
extern uint32_t placement_pointer;
uint32_t initial_esp;

extern void pci_scan();
extern void ide_initialize(uint32_t BAR0, uint32_t BAR1, uint32_t BAR2, uint32_t BAR3, uint32_t BAR4);
extern int ramfs_mount();

int kernel_main(multiboot_t *mb, uint32_t stack)
{
  multiboot = mb;
  initial_esp = stack;

  init_gdt();
  init_idt();

  init_console();

  kprintf("initial_esp = %x\n\n", initial_esp);

  init_timer(50);

  kernel_elf = elf_from_multiboot(mb);

  uint32_t ramdisk_end = *(uint32_t*)(multiboot->mods_addr+4);
  placement_pointer = ramdisk_end;

  // Init memory manager
  init_paging();
  init_heap();

  asm volatile("sti");

  //pci_scan();
  //ide_initialize(0x1F0, 0x3F4, 0x170, 0x374, 0x000);

  ramfs_mount();

  init_tasking();

  int ret = fork();

  kprintf("fork() returned %x, and getpid() returned %x", ret, getpid());

  return 0xba110017;
}
