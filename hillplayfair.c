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
#include <string.h>

int main(int argc, char *argv[])
{
  printf("argc = %d\n", argc);

  for (int i = 0; i < argc; i++)
  {
    printf("argv[%d] = %s\n", i, argv[i]);
  }

  printf("\n");

  if (argc == 5)
  {
    if (strcmp(argv[1], "encrypt") != 0)
    {
      return 1;
    }

    FILE *hillKeyFile = fopen(argv[2], "r");
    if (hillKeyFile == NULL)
    {
      printf("key file could not be opened");
      return 1;
    }

    FILE *plaintextFile = fopen(argv[3], "r");
    if (plaintextFile == NULL)
    {
      printf("plaintext file could not be opened");
      return 1;
    }

    FILE *playfairKeyFile = fopen(argv[4], "r");
    if (playfairKeyFile == NULL)
    {
      printf("playfair key file could not be opened");
      return 1;
    }
  }

  else
  {
    printf("Invalid number of arguments provided.\n");
    printf("Try: ./00_args 123\n");
  }

  








  return 0;
}
