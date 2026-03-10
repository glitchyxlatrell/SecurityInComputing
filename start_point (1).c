/*
Assignment:
crcxor - CRC + XOR Algorithm Implementation

Author: Latrell Kong

Language: C

To Compile:
  gcc  -O2 -std=c11   -o crcxor crcxor.c

To Execute (on Eustis):
  ./crcxor <message_file> <crc_algorithm>

where:
  <message_file> is the path to the input text file
  <crc_algorithm> is 3, 4, or 8 (for CRC-3, CRC-4, or CRC-8)

Notes:
  - Implements CRC-3, CRC-4, and CRC-8 algorithms
  - Implements the XOR (LRC) checksum in addition to CRC
  - Processes plain text messages and computes CRC values
  - Outputs all intermediate steps and final CRC values
  - Tested on Eustis.

Class: CIS3360 - Security in Computing - Spring 2026

Instructor: Jie Lin, Ph.D.

Due Date: 3/23/26
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
