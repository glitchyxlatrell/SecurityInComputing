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

// including all needed C libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// function prototypes
void processPlaintext(char *plaintext, char *newText);
void printWrapped(char *text);
void hillCipher(int n, int hillMatrix[n][n], char *text, char *newText);
void processPlayKey(char *playfairKey, char *processed);
void createPlayfair(char *key, char matrix[][5]);
void processCipher(char *oldText, char *newText);
void finalPlayfair(char *text, char matrix[][5], char *newText);

int main(int argc, char *argv[])
{
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
    // showing error if invalid number of arguments provided
    printf("Invalid number of arguments provided.\n");
    printf("Try: ./00_args 123\n");
    return 1;
  }

  // opening hill key file for the matrix
  int nHill = 0;
  if (fscanf(hillKeyFile, "%d", &nHill) != 1 || nHill < 2 || nHill > 9)
  {
    printf("invalid n value in hill key file\n");
    return 1;
  }

  // parsing through file to fill out matrix key
  int hillMatrix[nHill][nHill];
  for (int i = 0; i < nHill; i++)
  {
    for (int j = 0; j < nHill; j++)
    {
      if (fscanf(hillKeyFile, "%d", &hillMatrix[i][j]) != 1)
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
  if (fscanf(playfairKeyFile, "%99s", playfairKey) != 1)
  {
    printf("error getting playfair key");
  }

  // closing all files
  fclose(hillKeyFile);
  fclose(plaintextFile);
  fclose(playfairKeyFile);

  // printing original plaintext
  printf("\nOriginal Plaintext:\n%s\n", plaintext);

  // processing and printing plaintext
  char preprocessPlaintext[10000];
  processPlaintext(plaintext, preprocessPlaintext);
  printf("Preprocessed Plaintext:\n");
  printWrapped(preprocessPlaintext);

  // printing hill cipher dimension and matrix
  printf("\nHill Cipher Key Dimension:\n%d\n\n", nHill);
  printf("Hill Cipher Key Matrix:\n");
  for (int i = 0; i < nHill; i++)
  {
    for (int j = 0; j < nHill; j++)
    {
      printf("%d  ", hillMatrix[i][j]);
    }
    printf("\n\n");
  }

  // padding plaintext for hill cipher to be ready for hill cipher
  int plainLength = strlen(preprocessPlaintext);
  int pad = nHill - (plainLength % nHill);
  if ((plainLength % nHill) != 0)
  {
    for (int i = 0; i < pad; i++)
    {
      preprocessPlaintext[plainLength + i] = 'X';
    }
    preprocessPlaintext[plainLength + pad] = '\0';
  }

  // printing padded text
  printf("Padded Hill Cipher Plaintext:\n");
  printWrapped(preprocessPlaintext);

  // calling hill cipher and printing result
  char hillText[10000];
  hillCipher(nHill, hillMatrix, preprocessPlaintext, hillText);
  printf("\nCiphertext after Hill Cipher:\n");
  printWrapped(hillText);

  // preprocessing the playfair cipher key and creating matrix
  char playMatrix[5][5];
  char processedPlayfairKey[strlen(playfairKey) + 1];
  processPlayKey(playfairKey, processedPlayfairKey);
  printf("\nPlayfair Keyword:\n%s\n\n", processedPlayfairKey);
  createPlayfair(processedPlayfairKey, playMatrix);

  // printing playfair table
  printf("Playfair Table:\n");
  for (int i = 0; i < 5; i++)
  {
    for (int j = 0; j < 5; j++)
    {
      printf(" %c ", (char)playMatrix[i][j]);
    }
    printf("\n");
  }

  // preprocessing result of hill cipher
  char readyToPlay[10000];
  processCipher(hillText, readyToPlay);

  // calling playfair cipher
  char finalText[10000];
  finalPlayfair(readyToPlay, playMatrix, finalText);
  
  // printing final result
  printf("Ciphertext after Playfair:\n");
  printWrapped(finalText);
  return 0;
}

// function to preprocess plaintext
void processPlaintext(char *plaintext, char *newText)
{
  int counter = 0;

  // parsing through plaintext, and only grabbing letters, making them uppercase
  for (int i = 0; plaintext[i] != '\0'; i++)
  {
    if ((plaintext[i] >= 'A' && plaintext[i] <= 'Z') || (plaintext[i] >= 'a' && plaintext[i] <= 'z'))
    {
      newText[counter] = (char)toupper(plaintext[i]);
      counter++;
    }
  }
  newText[counter] = '\0';
  return;
}

// function to print results following 80 character rules
void printWrapped(char *text)
{
  int counter = 0;
  
  // parsing and printing every char, new line once counter hits 0
  for (int i = 0; text[i] != '\0'; i++)
  {
    printf("%c", text[i]);
    counter++;

    // resets counter
    if (counter == 80)
    {
      printf("\n");
      counter = 0;
    }
  }
  printf("\n");
}

// function to complete hill cipher
void hillCipher(int n, int hillMatrix[n][n], char *text, char *newText)
{
  // to parse through every block of size n
  int loopSize = strlen(text) / n;
  int tempArray[n];
  for (int i = 0; i < loopSize; i++)
  {

    // putting values in tempArray for vector multiplication
    for (int j = 0; j < n; j++)
    {
      tempArray[j] = ((int)text[i * n + j] - 65);
    }

    // looping for k-rows of matrix
    for (int k = 0; k < n; k++)
    {
      int sum = 0;
      // looping for f-columns
      for (int f = 0; f < n; f++)
      {
        // vector multiplication
        sum += hillMatrix[k][f] * tempArray[f];
      }
      // putting result of each row in correct spot of newText
      newText[i * n + k] = (char)((sum % 26) + 65);
    }

  }
  // null terminator
  newText[loopSize * n] = '\0';
}

// function to preprocess playfair key
void processPlayKey(char *playfairKey, char *processed)
{ 
  // creating boolean array for alphabet to track dupes
  bool nonDup[26] = {false};
  int counter = 0;
  int loop = strlen(playfairKey);
  
  // parsing through each char of key
  for (int i = 0; i < loop; i++)
  {
    // making every char uppercase and replacing every 'J' with 'I'
    playfairKey[i] = (char)toupper(playfairKey[i]);
    if (playfairKey[i] == 'J')
    {
      playfairKey[i] = 'I';
    }

    // getting 0-25 value of letter for boolean look-up table
    int temp = (int)playfairKey[i] - 65;
    if (!nonDup[temp])
    {
      // if non-dupe, adding to preprocessed key and setting boolean value to true
      processed[counter] = playfairKey[i];
      counter++;
      nonDup[temp] = true;
    }
  }

  // null terminator
  processed[counter] = '\0';
}

// function to create the playfair matrix
void createPlayfair(char *key, char matrix[][5])
{
  // boolean look-up table to track dupes
  bool alphaTaken[26] = {false};
  int row = 0;
  int column = 0;
  int loop = strlen(key);

  // parsing through every char in key
  for (int i = 0; i < loop; i++)
  {
    // checking if dupe just in case
    int temp = (int)key[i] - 65;
    if (!alphaTaken[temp])
    {
      // if not, adding char to matrix and incrementing row and column accurately
      matrix[row][column] = key[i];
      column++;
      if (column == 5)
      {
        row++;
        column = 0;
      }

      // setting look-up table value to true
      alphaTaken[temp] = true;
    }
  }

  // parsing through alphabet
  for (int i = 0; i < 26; i++)
  {

    // skipping J
    if (i == ('J' - 65))
    {
      continue;
    }

    // if letter has not been used, adding letter to matrix and incrementing row and column accordingly
    if (!alphaTaken[i])
    {
      matrix[row][column] = (char)(i + 65);
      alphaTaken[i] = true;
      column++;
      if (column == 5)
      {
        row++;
        column = 0;
      }
    }
  }
}

// function to preprocess the result of hill cipher to prepare for playfair cipher
void processCipher(char *oldText, char *newText)
{
  int counter = 0;
  int length = strlen(oldText);

  // parsing through every char of oldText
  for(int i = 0; i < length; i++)
  {
    // replacing every 'J' with 'I'
    if(oldText[i] == 'J')
    {
      oldText[i] = 'I';
    }
  }

  int loop = strlen(oldText);

  // parsing through new oldText
  for (int i = 0; i < loop; i++)
  {
    // putting first letter encountered in digraph
    newText[counter] = oldText[i];
    counter++;

    // if next char is not null-terminator and the same as current char
    if ((i + 1 < loop) && (oldText[i] == oldText[i + 1]))
    {
      // if current char is 'X', second letter of digraph is 'Z'
      if (oldText[i] == 'X')
      {
        newText[counter] = 'Z';
        counter++;
      }
      else
      {
      // otherwise, second letter of digraph is padded with 'X'
      newText[counter] = 'X';
      counter++;
      }
    }

    // current char and next char are not the same
    else if(i + 1 < loop)
    { 
      // put next char in second position of digraph and increment counter and i
      newText[counter] = oldText[i+1];
      counter++;
      i++;
    }
  }
  
  // checking if there is a sole letter left after checking for repetitions
  if ((counter % 2) != 0)
  {
    // if last letter is 'Z', pad with 'X'
    if(newText[counter - 1] == 'Z')
    {
      newText[counter] = 'X';
      counter++;
    }
    else
    {
      // otherwise, pad with 'Z'
      newText[counter] = 'Z';
      counter++;
    }
  }

  // null terminator
  newText[counter] = '\0';
}

// function to complete playfair cipher
void finalPlayfair(char *text, char matrix[][5], char *newText)
{
  int loop = strlen(text) / 2;
  char tempDi[2];

  // parsing through each digraph
  for(int i = 0; i < loop; i ++)
  {
    // initializing variables for use
    int firstX = 0;
    int firstY = 0;
    int secondX = 0;
    int secondY = 0;

    // putting digraph values in temp
    tempDi[0] = text[i * 2];
    tempDi[1] = text[i * 2 + 1];

    // parsing through playfair matrix
    for(int j = 0; j < 5; j++)
    {
      for(int h = 0; h < 5; h++)
      {

        // setting variables to correct positions, based on matrix
        if(tempDi[0] == matrix[j][h])
        {
          firstX = j;
          firstY = h;
        }

        if(tempDi[1] == matrix[j][h])
        {
          secondX = j;
          secondY = h;
        }
      }
    }

    // if in same row
    if(firstX == secondX)
    {
      // replacing with char to the right (with wrap-around)
      newText[i * 2] = matrix[firstX][(firstY + 1) % 5];
      newText[i * 2 + 1] = matrix[firstX][(secondY + 1) % 5];
    }
    
    // if in same column
    else if(firstY == secondY)
    {
      // replacing with char below it (with wrap-around)
      newText[i * 2] = matrix[(firstX + 1) % 5][firstY];
      newText[i * 2 + 1] = matrix[(secondX + 1) % 5][secondY];
    }

    // if form a rectangle
    else
    {
      // replacing with char in same row but the column of the other letter in digraph
      newText[i * 2] = matrix[firstX][secondY];
      newText[i * 2 + 1] = matrix[secondX][firstY];
    }
  }

  // null terminator
  newText[loop * 2] = '\0';
}