#include <kernel.h>

int kernel_main(void* p)
{
  console_init();
  kprintf("Hello World%c", '!');
}
