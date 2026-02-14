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
#include <stdbool.h>



void processPlaintext(char *plaintext, char *newText);
void printWrapped(char *text);
void hillCipher(int n, int hillMatrix[][9], char *text, char *newText);
void processPlayKey(char *playfairKey, char *processed);
void createPlayfair(char *key, char matrix[][5]);

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

  printf("Preprocessed Plaintext:\n\n");
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

  char hillText[10000];
  hillCipher(nHill, hillMatrix, preprocessPlaintext, hillText);

  printf("\nCiphertext after Hill Cipher:\n");
  printWrapped(hillText);


  char playMatrix[5][5];
  char processedPlayfairKey[strlen(playfairKey) + 1];
  printf("\nPlayfair Keyword:\n%s\n\n", playfairKey);
  processPlayKey(playfairKey, processedPlayfairKey);
  createPlayfair(processedPlayfairKey, playMatrix);

  printf("Playfair Table\n");
  for(int i = 0; i < 5; i++)
  {
    for(int j = 0; j < 5; j++)
    {
      printf("%c", (char)playMatrix[i][j]);
    }
    printf("\n");
  }

  char readyToPlay[10000];
  processCipher(hillText, readyToPlay);

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

void hillCipher(int n, int hillMatrix[][9], char *text, char *newText)
{
  int loopSize = strlen(text) / n;
  int tempArray[n];
  int intText[n];
  for(int i = 0; i < loopSize; i++)
  {
    for(int j = 0; j < n; j++)
    {
      tempArray[j] = ((int)text[i * n + j] - 65);
    }

    // looping for k-rows of matrix
    for(int k = 0; k < n; k++)
    {
      int sum = 0;
      // looping for f-columns 
      for(int f = 0; f < n; f++)
      {
        sum += hillMatrix[k][f] * tempArray[f];
      }
      intText[k] = sum % 26;
    }

    for(int h = 0; h < n; h++)
    {
      newText[i * n + h] = (char)(intText[h] + 65);
    }
  }
  newText[loopSize * n] = '\0';
}

void processPlayKey(char *playfairKey, char *processed)
{
  bool nonDup[26] = {false};
  int counter = 0;
  int loop = strlen(playfairKey);
  for(int i = 0; i < loop; i++)
  {
    playfairKey[i] = (char)toupper(playfairKey[i]);
    if(playfairKey[i] == 'J')
    {
      playfairKey[i] = 'I';
    }
    int temp = (int)playfairKey[i] - 65;
    if(!nonDup[temp])
    {
      processed[counter] = playfairKey[i];
      counter++;
      nonDup[temp] = true;
    }
  }
  processed[counter] = '\0';
}

void createPlayfair(char *key, char matrix[][5])
{
  bool alphaTaken[26] = {false};
  int row = 0;
  int column = 0;
  int loop = strlen(key);

  for(int i = 0; i < loop; i++)
  {
    matrix[row][column] = key[i];
    column++;
    if(column == 5)
    {
      row++;
      column = 0;
    }

    int temp = (int)key[i] - 65;
    if(!alphaTaken[temp])
    {
      alphaTaken[temp] = true;
    }
  }

  for(int i = 0; i < 26; i++)
  {
    if(i == ('J' - 65))
    {
      continue;
    }

    if(!alphaTaken[i])
    {
      matrix[row][column] = (char)(i + 65);
      alphaTaken[i] = true;
      column++;
      if(column == 5)
      {
        row++;
        column = 0;
      }
    }
  }
  
}

processCipher(char *oldText, char *newText)
  {
    int loop = strlen(oldText);
    int counter = 0;
    for(int i = 0; i < loop; i++)
    {
      newText[counter] = oldText[i];
      counter++;

      if((i + 1 < loop) && (oldText[i] == oldText[i + 1]))
      {
        if(oldText[i] == 'X')
        {
          newText[counter] = 'Z';
          counter++;
          i++;
          continue;
        }

        newText[counter] = 'X';
        counter++;
        i++;
      }
    }

    if((counter % 2) != 0)
    {
      newText[counter] = 'X';
      counter++;
    }
    newText[counter] = '\0';
  }