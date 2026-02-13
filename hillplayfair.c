/*
Assignment:
hillplayfair - Hill cipher followed by Playfair cipher

Author: Latrell Kong

Language: C

To Compile:
  gcc  -O2 -std=c11   -o hillplayfair hillplayfair.c

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
#include <ctype.h>


void processPlaintext(char *plaintext, char *newText);
void printWrapped(char *text);

int main(int argc, char *argv[])
{
  printf("argc = %d\n", argc);

  for (int i = 0; i < argc; i++)
  {
    printf("argv[%d] = %s\n", i, argv[i]);
  }

  printf("\n");

  // initializing files to open
  FILE *hillKeyFile = NULL;
  FILE *plaintextFile = NULL;
  FILE *playfairKeyFile = NULL;

  // checking if all arguments were provided
  if (argc == 5)
  {
    // arg 1 has to be the command encrypt
    if (strcmp(argv[1], "encrypt") != 0)
    {
      return 1;
    }

    // printing mode if encryption is present
    printf("Mode: \nEncryption Mode\n");

    // opening all files to read
    hillKeyFile = fopen(argv[2], "r");
    if (hillKeyFile == NULL)
    {
      printf("key file could not be opened\n");
      return 1;
    }

    plaintextFile = fopen(argv[3], "r");
    if (plaintextFile == NULL)
    {
      printf("plaintext file could not be opened\n");
      return 1;
    }

    playfairKeyFile = fopen(argv[4], "r");
    if (playfairKeyFile == NULL)
    {
      printf("playfair key file could not be opened\n");
      return 1;
    }
  }

  else
  {
    printf("Invalid number of arguments provided.\n");
    printf("Try: ./00_args 123\n");
    return 1;
  }

  // opening hill key file for the matrix
  int nHill = 0;
  if(fscanf(hillKeyFile,"%d", &nHill) != 1  || nHill < 2 || nHill > 9)
  {
    printf("invalid n value in hill key file\n");
    return 1;
  }

  // parsing through file to fill out matrix key
  int hillMatrix[nHill][nHill];
  for(int i = 0; i < nHill; i++)
  {
    for(int j = 0; j < nHill; j++)
    {
      if(fscanf(hillKeyFile, "%d", &hillMatrix[i][j]) != 1)
      {
        printf("invalid matrix in hill key file\n");
        return 1;
      }
    }
  }

  // variables to take in plaintext
  char plaintext[10000];
  int temp = 0;
  int tempChar;

  // parsing through plaintext file using fgetc for every character
  while ((tempChar = fgetc(plaintextFile)) != EOF)
  {
    plaintext[temp] = (char)tempChar;
    temp++;
  }
  plaintext[temp] = '\0';

  char playfairKey[100];
  if(fscanf(playfairKeyFile, "%99s", playfairKey) != 1)
  {
    printf("error getting playfair key");
  }

  printf("\nOriginal Plaintext:\n%s\n", plaintext);

  char preprocessPlaintext[10000];
  processPlaintext(plaintext, preprocessPlaintext);

  printf("Preprocessed Plaintext:\n");
  printWrapped(preprocessPlaintext);

  printf("Hill Cipher Key Dimension:\n%d\n\n", nHill);
  printf("Hill Cipher Key Matrix:\n");
  for(int i = 0; i < nHill; i++)
  {
    for(int j = 0; j < nHill; j++)
    {
      printf("  %d  ", hillMatrix[i][j]);
    }
    printf("\n\n");
  }

  int plainLength = strlen(preprocessPlaintext);
  int pad = nHill - (plainLength % nHill);
  if((plainLength % nHill) != 0)
  {
    for(int i = 0; i < pad; i++)
    {
      preprocessPlaintext[plainLength + i] = 'X';
    }
    preprocessPlaintext[plainLength + pad] = '\0';
  }

  printf("Padded Hill Cipher Plaintext:\n");
  printWrapped(preprocessPlaintext);


  return 0;
}

void processPlaintext(char *plaintext, char *newText)
{
  int counter = 0;
  for(int i = 0; plaintext[i] != '\0'; i++)
  {
    if((plaintext[i] >= 'A' && plaintext[i] <= 'Z') || (plaintext[i] >= 'a' && plaintext[i] <= 'z'))
    {
      newText[counter] = (char)toupper(plaintext[i]);
      counter++;
    }
  }
  newText[counter] = '\0';
  return;
}

void printWrapped(char *text)
{
  int counter = 0;
  for(int i = 0; text[i] != '\0'; i++)
  {
    printf("%c ",text[i]);
    counter++;

    if(counter == 80)
    {
      printf("\n");
      counter = 0;
    }
  }
  printf("\n");
}