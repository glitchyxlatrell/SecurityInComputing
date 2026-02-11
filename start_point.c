/*
Assignment:
hillplayfair - Hill cipher followed by Playfair cipher

Author: <Your Name Here>

Language: C, C++, or Rust (only)

To Compile:
  gcc  -O2 -std=c11   -o hillplayfair hillplayfair.c
  g++  -O2 -std=c++17 -o hillplayfair hillplayfair.cpp
  rustc -O hillplayfair.rs -o hillplayfair

To Execute (on Eustis):
  ./hillplayfair encrypt key.txt plain.txt keyword.txt

where:
  key.txt     = key matrix file
  plain.txt   = plaintext file
  keyword.txt = Playfair keyword file

Notes:
  - Input is text; process A-Z only (case-insensitive).
  - Tested on Eustis.

Class: CIS3360 - Security in Computing - Spring 2026

Instructor: Dr. Jie Lin

Due Date: February 16th 2026
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    printf("argc = %d\n", argc);

    for (int i = 0; i < argc; i++)
    {
        printf("argv[%d] = %s\n", i, argv[i]);
    }

    printf("\n");

    if (argc > 1)
    {
        int x = atoi(argv[1]);   // convert string to int (simple)
        printf("Converted argv[1] to int: %d\n", x);
    }
    else
    {
        printf("No extra arguments provided.\n");
        printf("Try: ./00_args 123\n");
    }

    return 0;
}
