#include <kernel.h>

extern elf_t kernel_elf;

void handle_panic()
{

}

void handle_assert()
{

}

void panic(const char *msg)
{
  kprintf("PANIC: %s\n", msg);
  kprint_stacktrace();
  kprintf("*****\n\n");
  for(;;) {};
}

void kprint_stacktrace()
{
  uint32_t *ebp, *eip;
  asm volatile ("mov %%ebp, %0" : "=r" (ebp));
  while (ebp)
  {
    eip = ebp+1;
    kprintf("   [0x%x] %s\n", *eip, elf_lookup_symbol(*eip, &kernel_elf));
    ebp = (uint32_t*) *ebp;
  }
}

