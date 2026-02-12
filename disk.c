/**
 * disk.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "disk.h"
#include "memory.h"

// translation buffer
static int translation[2];

// trim whitespace from both ends of a string
static char *trim(char *s)
{
    char *end;
    while (isspace((unsigned char)*s)) s++;
    if (*s == 0) return s;
    end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return s;
}

/**
 * required func to define for project 1
 * translate textual instruction into opcode+arg.
 * lines starting with '//' or empty after trimming return NULL.
 */
int* translate(char *instruction)
{
    if (instruction == NULL) return NULL;
    char buf[256];
    strncpy(buf, instruction, sizeof(buf)-1);
    buf[sizeof(buf)-1] = '\0';
    char *s = trim(buf);
    if (*s == '\0') return NULL;
    if (s[0] == '/' && s[1] == '/') return NULL;

    /* Tokenize */
    char *tok = strtok(s, " \t\n\r");
    if (!tok) return NULL;
    /* lowercase opcode for comparison */
    for (char *p = tok; *p; ++p) *p = tolower((unsigned char)*p);

    if (strcmp(tok, "exit") == 0) {
        translation[0] = 0; translation[1] = 0; return translation;
    }
    if (strcmp(tok, "load_const") == 0) {
        char *arg = strtok(NULL, " \t\n\r");
        translation[0] = 1;
        translation[1] = arg ? atoi(arg) : 0;
        return translation;
    }
    if (strcmp(tok, "move_from_mbr")    == 0) { translation[0]=2; translation[1]=0; return translation; }
    if (strcmp(tok, "move_from_mar")    == 0) { translation[0]=3; translation[1]=0; return translation; }
    if (strcmp(tok, "move_to_mbr")      == 0) { translation[0]=4; translation[1]=0; return translation; }
    if (strcmp(tok, "move_to_mar")      == 0) { translation[0]=5; translation[1]=0; return translation; }
    if (strcmp(tok, "load_at_addr")     == 0) { translation[0]=6; translation[1]=0; return translation; }
    if (strcmp(tok, "write_at_addr")    == 0) { translation[0]=7; translation[1]=0; return translation; }
    if (strcmp(tok, "add")              == 0) { translation[0]=8; translation[1]=0; return translation; }
    if (strcmp(tok, "multiply")         == 0) { translation[0]=9; translation[1]=0; return translation; }
    if (strcmp(tok, "and")              == 0) { translation[0]=10; translation[1]=0; return translation; }
    if (strcmp(tok, "or")               == 0) { translation[0]=11; translation[1]=0; return translation; }
    if (strcmp(tok, "ifgo")             == 0) { char *arg = strtok(NULL, " \t\n\r"); translation[0]=12; translation[1] = arg ? atoi(arg) : 0; return translation; }
    if (strcmp(tok, "sleep")            == 0) { translation[0]=13; translation[1]=0; return translation; }

    return NULL;
}

/**
 * required func to define for project 1
 * load the program
 * calls translate for each line
 * if translate returns non-null, write to memory at addr using mem_write
 */
void load_prog(char *fname, int addr)
{
    FILE *f = fopen(fname, "r");
    if (!f) {
        fprintf(stderr, "Error opening program file %s\n", fname);
        return;
    }

    char line[512];
    int cur = addr;
    while (fgets(line, sizeof(line), f)) {
        int *t = translate(line);
        if (t) {
            mem_write(cur, t);
            cur++;
        }
    }

    fclose(f);
}

/**
 * required func to define for project 2
 * load multiple programs from a list file
 */
void load_programs(char list_fname[])
{
    FILE *f = fopen(list_fname, "r");
    if (!f) {
        fprintf(stderr, "Error opening program list %s\n", list_fname);
        return;
    }

    char line[512];
    while (fgets(line, sizeof(line), f)) {
        char *p = trim(line);
        if (*p == '\0' || (p[0] == '/' && p[1] == '/')) continue;

        int addr = 0;
        char fname[256];
        if (sscanf(p, "%d %255s", &addr, fname) == 2) {
            printf("  -> loading '%s' at address %d\n", fname, addr);
            load_prog(fname, addr);
        }
    }

    fclose(f);
}
