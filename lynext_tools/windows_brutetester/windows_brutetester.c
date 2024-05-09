#include "math.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BTN_SIZE 7
#define BTNS {0x01, 0x04, 0x08, 0x0C, 0xA2, 0xEC, 0xE5}
#define LUT_SIZE 4
#define LUT {0x00, 0x01, 0x02, 0x03}
#define STEP_INC 1

size_t lutSpace = 1;

void customCATINT (char* s1, char* format, uint8_t s2)
{
    char* temp = (char*) malloc(sizeof(64));
    strcpy(temp, "");
    sprintf(temp, format, s2);
    strcat(s1, temp);
}

void customCAT (char* s1, char* format, char* s2)
{
    char* temp = (char*) malloc(sizeof(64));
    strcpy(temp, "");
    sprintf(temp, format, s2);
    strcat(s1, temp);
}

void brutePrincetonByButton (uint64_t step, char* candidate) {
    const uint8_t lut[] = LUT; // DIP SWITCH LUT_SIZE = 3 & {0x00, 0x01, 0x03}
    const size_t lutSize = LUT_SIZE;

    const uint8_t btns[] = BTNS;
    const size_t btnSize = BTN_SIZE;

    uint8_t p[8];
    uint64_t total = 0;
    uint64_t _step = (step * STEP_INC);

    for(size_t j = 0; j < 8; j++) {
        total |= lut[_step % lutSize] << (2 * j);
        double sub_step = (double)_step / lutSize;
        _step = (uint64_t)floor(sub_step);
    }
    
    total <<= 8;
    total |= btns[(step * STEP_INC) / (lutSpace)];

    for(size_t i = 0; i < 8; i++) {
        p[i] = (uint8_t)(total >> 8 * (7 - i)) & 0xFF;
    }

    size_t size = sizeof(uint64_t);
    for(size_t i = 0; i < size; i++) {
        if(p[i] != 0) 
        {
            customCATINT(candidate, "%02X ", p[i]);
        } 
        else 
        {
            customCAT(candidate, "%s ", "00");
        }
    }
}

void brutePrincetonByKey (uint64_t step, char* candidate) {
    const uint8_t lut[] = LUT; // DIP SWITCH LUT_SIZE = 3 & {0x00, 0x01, 0x03}
    const size_t lutSize = LUT_SIZE;

    const uint8_t btns[] = BTNS;
    const size_t btnSize = BTN_SIZE;

    uint8_t p[8];
    uint64_t total = 0;
    uint64_t _step = ((step / btnSize) * STEP_INC);

    for(size_t j = 0; j < 8; j++) {
        total |= lut[_step % lutSize] << (2 * j);
        double sub_step = (double)_step / lutSize;
        _step = (uint64_t)floor(sub_step);
    }
    
    total <<= 8;
    total |= btns[(step % btnSize)];

    for(size_t i = 0; i < 8; i++) {
        p[i] = (uint8_t)(total >> 8 * (7 - i)) & 0xFF;
    }

    size_t size = sizeof(uint64_t);
    for(size_t i = 0; i < size; i++) {
        if(p[i] != 0) 
        {
            customCATINT(candidate, "%02X ", p[i]);
        } 
        else 
        {
            customCAT(candidate, "%s ", "00");
        }
    }
}

void bruteCAME (uint64_t step, char* candidate)
{
    uint8_t p[8];
    for(size_t i = 0; i < 8; i++) {
        p[i] = (uint8_t)(step >> 8 * (7 - i)) & 0xFF;
    }

    size_t size = sizeof(uint64_t);
    for(size_t i = 0; i < size; i++) {
        if(p[i] != 0) 
        {
            customCATINT(candidate, "%02X ", p[i]);
        } 
        else 
        {
            customCAT(candidate, "%s ", "00");
        }
    }
}

int main ()
{
    printf("Brute Tester By Lynext\n");
    for (size_t i = 0; i < 8; i++) // lutSpace = LUT_SIZE ^ 8
    {
        lutSpace *= LUT_SIZE;
    }
    
    uint64_t maxSteps = (lutSpace * BTN_SIZE) / STEP_INC;
    //uint64_t maxSteps = 4096;

    FILE *fptr = fopen("brute.txt", "w");

    for (uint64_t i = 0; i < maxSteps; i++)
    {
        char* str = (char*) malloc(sizeof(64));
        strcpy(str, "");
        brutePrincetonByKey(i, str);
        float percent = ((float)i / maxSteps) * 100.0;

        printf("%s | %d (%%%.1f)\n", str, i, percent);
        fprintf(fptr, "%s | %d\n", str, i);
    }
    return 0;
}