
#include <kernel.h>

inline uint8_t inb(uint16_t port)
{
  register uint8_t value;

  __asm__ __volatile__(
    "inb %1, %0\n"
    : "=a" ( value )
    : "dN" ( port )
  );

  return value;
}

inline void outb(uint16_t port, uint8_t data)
{
  __asm__ __volatile__(
    "outb %1, %0\n"
    :
    : "dN" ( port ), "a" ( data )
  );
}

inline uint16_t inw(uint16_t port)
{
  register uint16_t value;

  __asm__ __volatile__(
    "inw %1, %0\n"
    : "=a" ( value )
    : "dN" ( port )
  );

  return value;
}

inline void outw(uint16_t port, uint16_t data)
{
  __asm__ __volatile__(
    "outw %1, %0\n"
    :
    : "dN" ( port ), "a" ( data )
  );
}

inline uint32_t inl( uint16_t port )
{
  register uint32_t value;

  __asm__ __volatile__(
    "inl %1, %0\n"
    : "=a" ( value )
    : "dN" ( port )
  );

  return value;
}

inline void outl(uint16_t port, uint32_t data)
{
  __asm__ __volatile__(
    "outl %1, %0\n"
    :
    : "dN" ( port ), "a" ( data )
  );
}

inline void inws(uint16_t* buffer, size_t count, uint16_t port)
{
  __asm__ __volatile__(
    "rep insw\n"
    : "=c" ( count ), "=D" ( buffer )
    : "d" ( port ), "0" ( count ), "1" ( buffer )
    : "memory"
  );
}

inline void outws(const uint16_t* buffer, size_t count, uint16_t port)
{
  __asm__ __volatile__(
    "rep outsw\n"
    : "=c" ( count ), "=S" ( buffer )
    : "d" ( port ), "0" ( count ), "1" ( buffer )
    : "memory"
  );
}

inline void inls(uint32_t* buffer, size_t count, uint16_t port)
{
  __asm__ __volatile__(
    "rep insl\n"
    : "=c" ( count ), "=D" ( buffer )
    : "d" ( port ), "0" ( count ), "1" ( buffer )
    : "memory"
  );
}

inline void outls(const uint32_t* buffer, size_t count, uint16_t port)
{
  __asm__ __volatile__(
    "rep outsl\n"
    : "=c" ( count ), "=S" ( buffer )
    : "d" ( port ), "0" ( count ), "1" ( buffer )
    : "memory"
  );
}
