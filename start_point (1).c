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

// function prototypes
void processPlaintext(char *plaintext, char *newText);

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

  // variables to take in message
  char message[10000];
  int temp = 0;
  int tempChar;

  // parsing through plaintext file using fgetc for every character
  while ((tempChar = fgetc(messageFile)) != EOF && temp < 9999)
  {
    message[temp] = (char)tempChar;
    temp++;
  }
  message[temp] = '\0';

  fclose(messageFile);

  // printing original message
  printf("\nThe original message:\n%s\n", message);

  // processing and printing message
  char preprocessMessage[10000];
  processPlaintext(message, preprocessMessage);
  printf("The preprocessed message (invisible characters removed):\n%s\n", preprocessMessage);
  






  
  return 0;
}

// function to preprocess message
void processPlaintext(char *message, char *newText)
{
  int counter = 0;

  // parsing through message, and only grabbing letters or digits, discarding everything else
  for (int i = 0; message[i] != '\0'; i++)
  {
    if (isalnum(message[i]))
    {
      newText[counter] = (message[i]);
      counter++;
    }
  }

  // null terminator at end of message
  newText[counter] = '\0';
  return;
}
