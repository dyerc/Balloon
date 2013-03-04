
#include <kernel.h>

#define isdigit(c) ((c) >= '0' && (c) <= '9')

void memcpy(void* dest, void* src, uint32_t len)
{
    const uint8_t *sp = (const uint8_t *)src;
    uint8_t *dp = (uint8_t *)dest;
    for(; len != 0; len--) *dp++ = *sp++;
}

void memset(void* dest, uint8_t val, uint32_t len)
{
    uint8_t *temp = (uint8_t *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

void* memmove( void* dest, const void* src, uint32_t n )
{
    char* _dest;
    char* _src;

    if ( dest < src ) {
        _dest = ( char* )dest;
        _src = ( char* )src;

        while ( n-- ) {
            *_dest++ = *_src++;
        }
    } else {
        _dest = ( char* )dest + n;
        _src = ( char* )src + n;

        while ( n-- ) {
            *--_dest = *--_src;
        }
    }

    return dest;
}

uint32_t strlen(const char *txt)
{
    uint32_t len = 0;
    while (txt[len] != 0)
  len++;
    return len;
}

uint32_t strcmp(const char *str1, const char *str2)
{
    while (*str1 && *str2 && (*str1++ == *str2++))
    ;

  if (*str1 == '\0' && *str2 == '\0')
    return 0;

  if (*str1 == '\0')
    return -1;
  else return 1;
}

void reverse(char* s)
{
    char c;
    int32_t i, j = (strlen(s) - 1);

    for (i = 0; i < j; i++, j--)
    {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

int8_t ctoi(char c)
{
    if (c < 48 || c > 57) {
        return(-1);
    }
    return(c-48);
}

/// http://en.wikipedia.org/wiki/Itoa
void itoa(int32_t n, char* s)
{
    int32_t i, sign;
    if ((sign = n) < 0)  // record sign
    {
        n = -n;         // make n positive
    }
    i=0;
    do // generate digits in reverse order
    {
        s[i++] = n % 10 + '0';  // get next digit
    }
    while ((n /= 10) > 0);     // delete it

    if (sign < 0)
    {
        s[i++] = '-';
    }
    s[i] = '\0';
    reverse(s);
}

void i2hex(uint32_t val, char* dest, int32_t len)
{
    char* cp;
    char  x;
    uint32_t n;
    n = val;
    cp = &dest[len];
    while (cp > dest)
    {
        x = n & 0xF;
        n >>= 4;
        *--cp = x + ((x > 9) ? 'A' - 10 : '0');
    }
    dest[len]  ='\0';
}

float atof(const char* s)
{
    int32_t i = 0;
    int8_t sign = 1;
    while(s[i] == ' ' || s[i] == '+' || s[i] == '-')
    {
        if(s[i] == '-')
        {
            sign *= -1;
        }
        i++;
    }

    float val;
    for (val = 0.0; isdigit(s[i]); i++)
    {
        val = 10.0 * val + s[i] - '0';
    }
    if (s[i] == '.')
    {
        i++;
    }
    float pow;
    for (pow = 1.0; isdigit(s[i]); i++)
    {
        val = 10.0 * val + s[i] - '0';
        pow *= 10.0;
    }
    return(sign * val / pow);
}

void ftoa(float f, char* buffer)
{
    if (f < 0.0)
    {
        *buffer = '-';
        ++buffer;
        f = -f;
    }

    char tmp[32];
    int32_t i = f;
    int32_t index = sizeof(tmp) - 1;
    while (i > 0)
    {
        tmp[index] = ('0' + (i % 10));
        i /= 10;
        --index;
    }
    memcpy((void*)buffer, (void*)&tmp[index + 1], sizeof(tmp) - 1 - index);
    buffer += sizeof(tmp) - 1 - index;
    *buffer = '.';
    ++buffer;

    *buffer++ = ((uint32_t)(f * 10.0) % 10) + '0';
    *buffer++ = ((uint32_t)(f * 100.0) % 10) + '0';
    *buffer++ = ((uint32_t)(f * 1000.0) % 10) + '0';
    *buffer++ = ((uint32_t)(f * 10000.0) % 10) + '0';
    *buffer++ = ((uint32_t)(f * 100000.0) % 10) + '0';
    *buffer++ = ((uint32_t)(f * 1000000.0) % 10) + '0';
    *buffer   = '\0';
}
