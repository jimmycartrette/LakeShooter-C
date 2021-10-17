#include <stdint.h>
#include "utils.h"
#include "wasm4.h"

extern struct LSFR lsfr;

static char *itoa_helper(char *dest, int i)
{
    if (i <= -10)
    {
        dest = itoa_helper(dest, i / 10);
    }
    *dest++ = '0' - i % 10;
    return dest;
}

char *itoa(char *dest, int i)
{
    char *s = dest;
    if (i < 0)
    {
        *s++ = '-';
    }
    else
    {
        i = -i;
    }
    *itoa_helper(s, i) = '\0';
    return dest;
}

static unsigned long long seed;
int rand()
{
    seed = 6364136223846793005ULL * seed + 1;
    return seed >> 33;
}

int fake_fround(float x)
{
    return (int)(x + 0.5);
}
int min(int a, int b) { return (a < b) ? a : b; }
int max(int a, int b) { return (a > b) ? a : b; }
int digitsofbase10(int num, int counter)
{
    if (num < 10)
    {
        return counter;
    }
    return digitsofbase10(num / 10, counter + 1);
}
void lfsr_start(uint16_t seed, struct Lsfr *lsfr)
{
    lsfr->m_lfsrvalue = seed;
    lsfr->m_start_state = seed;
}
uint16_t lfsr_next(struct Lsfr *lsfr)
{
    char s[16 + 1];

    unsigned lsb = lsfr->m_lfsrvalue & 1u;
    lsfr->m_lfsrvalue >>= 1;
    if (lsb)
    {
        lsfr->m_lfsrvalue ^= 0xB400u; // taps at 11, 13, 14, 16
    }
    // for (int i = 0; i < 16; i++)
    // {
    //     s[15 - i] = (lsfr->m_lfsrvalue & (1 << i)) ? '1' : '0';
    // }
    // s[16] = '\0';
    // trace(s);
    tracef("%d", lsfr->m_lfsrvalue);
    return lsfr->m_lfsrvalue;
}
int abs(int i)
{
    return i < 0 ? -i : i;
}