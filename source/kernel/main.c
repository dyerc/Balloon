#include <kernel.h>

int kernel_main(void* p)
{
  console_init();
  console_puts("Hello World!");
}
