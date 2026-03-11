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

// importing necessary C libraries
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char *argv[])
{

  // initializing file and variable for command line inputs
  FILE *messageFile = NULL;
  int crcType = 0;

  // checking if all arguments were provided
  if (argc == 3)
  {
    // opening input text file
    messageFile = fopen(argv[1], "r");
    if (messageFile == NULL)
    {
      printf("message file could not be opened\n");
      return 1;
    }

    // checking if crc algorithm integer is valid
    if (isdigit(argv[2][0]))
    {
      crcType = atoi(argv[2]);
      if (crcType != 3 && crcType != 4 && crcType != 8)
      {
        printf("invalid crc algorithm value\n");
        return 1;
      }
    }
    else
    {
      printf("invalid crc algorithm value\n");
      return 1;
    }
  }
  else
  {
    // showing error if invalid number of arguments provided
    printf("Invalid number of arguments provided.\n");
    printf("Try: ./crcxor message.txt 3\n");
    return 1;
  }






  
  return 0;
}
