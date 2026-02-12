/*
 * memory.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "memory.h"

#define MEM_SIZE 1024

static int physical_memory[MEM_SIZE][2];

/*
 * required func to define for project 1
 * returns a pointer to the two-int array at memory address `addr`
 * if addr is out-of-bounds, returns NULL
 */
int* mem_read(int addr)
{
    if (addr < 0 || addr >= MEM_SIZE) {
        fprintf(stderr, "mem_read ERROR: address %d out of bounds (0..%d)\n", addr, MEM_SIZE - 1);
        return NULL;
    }
    return physical_memory[addr];
}

/*
 * required func to define for project 1
 * writes the two-int data into memory at address `addr`
 * if `data` is NULL, writes zeroes
 * if addr is out-of-bounds, does nothing
 */
void mem_write(int addr, int* data)
{
    if (data == NULL) return;
    if (addr < 0 || addr >= MEM_SIZE) return;

    physical_memory[addr][0] = data[0]; //opcode
    physical_memory[addr][1] = data[1]; //argument
}

/**
 * helper function to print memory contents
 */
void mem_print(int addr)
{
    int *p = mem_read(addr);
    if (p == NULL) {
        printf("mem_print: address %d out of bounds\n", addr);
        return;
    }
    printf("mem[%d] = { OP=%d, ARG=%d }\n", addr, p[0], p[1]);
}
