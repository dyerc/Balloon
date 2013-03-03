#ifndef KERNEL_H
#define KERNEL_H

#define KERNEL_VERSION  "0.1.0"

#include <types.h>

#define ASSERT()
#define PANIC(m)

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

void console_init();
void console_puts(char *txt);
void console_clear();

#endif
