/* from https: //github.com/aduros/wasm4/blob/main/examples/watris/src/utils.c */

#pragma once

char *itoa(char *dest, int i);

int rand();

int min(int a, int b);
int max(int a, int b);
int fake_fround(float x);
int digitsofbase10(int num, int counter);