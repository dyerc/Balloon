#ifndef KERNEL_H
#define KERNEL_H

#define KERNEL_VERSION  "0.1.0"

#include <types.h>
#include <arch.h>
#include <multiboot.h>
#include <memory.h>
#include <elf.h>
#include <fs.h>
#include <task.h>

#define ASSERT(b) ((b) ? (void)0 : panic(__LINE__))
#define assert(b)
#define spin_lock(f)
#define spin_unlock(f)
#define PANIC(m)    panic(m);

void kprintf(const char *msg, ...);
void kprint_stacktrace();

/******************/
// IO
/******************/

inline uint8_t inb(uint16_t port);
inline void outb(uint16_t port, uint8_t data);
inline uint16_t inw(uint16_t port);
inline void outw(uint16_t port, uint16_t data);
inline uint32_t inl( uint16_t port );
inline void outl(uint16_t port, uint32_t data);
inline void inws(uint16_t* buffer, size_t count, uint16_t port);
inline void outws(const uint16_t* buffer, size_t count, uint16_t port);
inline void inls(uint32_t* buffer, size_t count, uint16_t port);
inline void outls(const uint32_t* buffer, size_t count, uint16_t port);

/******************/
// Console
/******************/

void init_console();
void console_puts(char *txt);
void console_clear();
void console_putchar(char c);

void init_timer(uint32_t freq);

void sleep(uint32_t msec);

#endif
