#include <kernel.h>

uint32_t tick = 0;

uint32_t wait_till = 0;
uint8_t unlock = 1;

static void timer_callback(registers_t * regs)
{
  tick++;

  if (tick >= wait_till)
    unlock = 1;

  switch_task();
}

void sleep(uint32_t msec)
{
  wait_till = tick + (msec);
  unlock = 0;
  while (unlock == 0);
}

void init_timer(uint32_t freq)
{
  register_interrupt_handler(IRQ0, &timer_callback);

  uint32_t divisor = 1193180 / freq;

  outb(0x43, 0x36);

  uint8_t l = (uint8_t)(divisor & 0xFF);
  uint8_t h = (uint8_t)( (divisor>>8) & 0xFF );

  outb(0x40, l);
  outb(0x40, h);
}
