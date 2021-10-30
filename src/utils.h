/* from https: //github.com/aduros/wasm4/blob/main/examples/watris/src/utils.c */
#include <stdint.h>
#pragma once

struct Lsfr
{
    uint16_t m_start_state;
    uint16_t m_lfsrvalue;
    uint8_t m_period;
};

char *itoa(char *dest, int i);

int rand();

int min(int a, int b);
int max(int a, int b);
int fake_fround(float x);
int digitsofbase10(int num, int counter);
/* from wikipedia https://en.wikipedia.org/wiki/Linear-feedback_shift_register */
void lfsr_start(uint16_t seed, struct Lsfr *lsfr);
uint16_t lfsr_next(struct Lsfr *lsfr);
int abs(int i);