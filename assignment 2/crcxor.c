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
#include <string.h>


// function prototypes
void processPlaintext(char *plaintext, char *newText);
void getDecHex(char *preMessage, char *newDec, char *newHex, char arr[16][8]);
void getBin(char *preprocessMessage, char *binaryMessage);
void getCrc(char *message, int type, char *crcVal);
void checkSum(char *message, char *sumBin);

// main
int main(int argc, char *argv[])
{

  // look-up table for ASCII values
  char ascii_table[16][8] = {
      {'\0', '\0', ' ', '0', '@', 'P', '`', 'p'},
      {'\0', '\0', '!', '1', 'A', 'Q', 'a', 'q'},
      {'\0', '\0', '"', '2', 'B', 'R', 'b', 'r'},
      {'\0', '\0', '#', '3', 'C', 'S', 'c', 's'},
      {'\0', '\0', '$', '4', 'D', 'T', 'd', 't'},
      {'\0', '\0', '%', '5', 'E', 'U', 'e', 'u'},
      {'\0', '\0', '&', '6', 'F', 'V', 'f', 'v'},
      {'\0', '\0', '\'', '7', 'G', 'W', 'g', 'w'},
      {'\0', '\0', '(', '8', 'H', 'X', 'h', 'x'},
      {'\0', '\0', ')', '9', 'I', 'Y', 'i', 'y'},
      {'\0', '\0', '*', ':', 'J', 'Z', 'j', 'z'},
      {'\0', '\0', '+', ';', 'K', '[', 'k', '{'},
      {'\0', '\0', ',', '<', 'L', '\\', 'l', '|'},
      {'\0', '\0', '-', '=', 'M', ']', 'm', '}'},
      {'\0', '\0', '.', '>', 'N', '^', 'n', '~'},
      {'\0', '\0', '/', '?', 'O', '_', 'o', '\0'}
    };

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

  // closing message file
  fclose(messageFile);

  // printing original message
  printf("\nThe original message:\n%s\n\n", message);

  // processing and printing message
  char preprocessMessage[10000];
  processPlaintext(message, preprocessMessage);
  printf("The preprocessed message (invisible characters removed):\n%s\n\n", preprocessMessage);

  // obtaining and printing decimal and hex representation of message
  char decimalMessage[10000];
  char hexMessage[10000];
  getDecHex(preprocessMessage, decimalMessage, hexMessage, ascii_table);

  printf("The decimal representation of the preprocessed message:\n%s\n\n", decimalMessage);
  printf("The hex representation of the preprocessed message:\n%s\n\n", hexMessage);

  // obtaining and printing binary representation of message
  char binaryMessage[10000];
  char paddedBinary[10000];
  paddedBinary[0] = '\0';
  getBin(preprocessMessage, binaryMessage);
  printf("The binary representation of the preprocessed message:\n%s\n\n", binaryMessage);

  // padding binary representation of message
  strcat(paddedBinary, binaryMessage);
  switch(crcType)
  {
    case 3:
      strcat(paddedBinary, " 000");
      break;
    case 4:
      strcat(paddedBinary, " 0000");
      break;
    case 8:
      strcat(paddedBinary, " 00000000");
      break; 
  }

  // printing binary representation
  printf("The binary representation of the original message prepared for CRC computation\n");
  printf("(padded with %d zeros):\n%s\n\n", crcType, paddedBinary);

  // initializing to store crc value in binary and hex
  char crcBin[9];
  long crcHex = 0;

  // calculating crc value and printing in binary
  getCrc(paddedBinary, crcType, crcBin);
  printf("The crc value for the chosen crc algorithm in binary:\n%s\n\n", crcBin);

  // transforming binary value into hex and printing
  crcHex = strtol(crcBin, NULL, 2);
  if(crcType == 8)
  {
    printf("The crc value for the chosen crc algorithm in hex:\n%02lX\n\n", crcHex);
  }
  else
  {
    printf("The crc value for the chosen crc algorithm in hex:\n%lX\n\n", crcHex);
  }

  // appending crc hex value to hex rep of message
  char addHex[10];
  char sumInput[10000];
  char finalHex[10000];
  sumInput[0] = '\0';

  // in case of leading 0's in crc 8
  if(crcType == 8)
  {
    sprintf(addHex, "%02lX", crcHex);
  }
  else
  {
    sprintf(addHex, "%lX", crcHex);
  }
  strcat(sumInput, hexMessage);
  strcat(sumInput, addHex);

  // removing spaces in hex rep
  int counter = 0;

  // parsing through representation, removing spaces
  for (int i = 0; sumInput[i] != '\0'; i++)
  {
    if (isalnum(sumInput[i]))
    {
      finalHex[counter++] = (sumInput[i]);
    }
  }
  finalHex[counter] = '\0';


  // appending crc bin value to binary rep of message
  if(crcType == 3)
  {
    strcat(binaryMessage, "0");
  }
  strcat(binaryMessage, crcBin);


  // preparing final binary value for checksum
  counter = 0;
  char finalBin[10000];
  finalBin[0] = '\0';

  // adding space after every 4 bits for binary rep
  for(int i = 0; binaryMessage[i] != '\0'; i++)
  {
    if((i % 4) == 0 && i != 0)
    {
      finalBin[counter++] = ' ';
    }
    finalBin[counter++] = binaryMessage[i];
  }
  strcat(finalBin, " ");

  // printing bin and hex rep with crc value appended
  printf("The input in hex for this XOR checksum computation:\n%s\n\n", finalHex);
  printf("The input in binary for this XOR checksum computation:\n%s\n\n", finalBin);

  // obtaining checksum binary value
  char binSum[5];
  binSum[0] = '\0';
  checkSum(binaryMessage, binSum);

  // printing binary checksum value
  printf("The xor checksum value for the chosen crc algorithm in binary:\n%s\n\n", binSum);


  // transforming binary value into hex and printing
  long sumHex = 0;
  sumHex = strtol(binSum, NULL, 2);
  printf("The xor checksum value for the chosen crc algorithm in hex:\n%lX\n\n", sumHex);

  // appending checksum hex value to hex rep of message
  char addSumHex[10];
  char finalOutput[10000];
  finalOutput[0] = '\0';
  sprintf(addSumHex, "%lX", sumHex);
  strcat(finalOutput, finalHex);
  strcat(finalOutput, addSumHex);

  // printing final result
  printf("The final message is going to be transmitted in hex:\n%s\n\n", finalOutput);

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

// function to get decimal and hex value of message
void getDecHex(char *preMessage, char *newDec, char *newHex, char arr[16][8])
{

  // initializing temp and both new strings for strcat
  char temp[10];
  newDec[0] = '\0';
  newHex[0] = '\0';

  // looping through all of preprocessed message
  for(int i = 0; preMessage[i] != '\0'; i++)
  {
    for(int j = 0; j < 16; j++)
    {
      for(int k = 0; k < 8; k++)
      {

        // getting row/column values if found value in ASCII table
        if(preMessage[i] == arr[j][k])
        {

          // ordinal number formula, and storing decimal value in newDec
          int decimal = 16 * k + j;
          sprintf(temp, "%d ", decimal);
          strcat(newDec, temp);

          // storing hex value
          sprintf(temp, "%d", k);
          strcat(newHex, temp);

          // hex letters, if 10 or greater
          if(j > 9)
          {
            switch(j)
            {
              case 10:
                strcat(newHex, "A ");
                break;
              case 11:
                strcat(newHex, "B ");
                break;
              case 12:
                strcat(newHex, "C ");
                break;
              case 13:
                strcat(newHex, "D ");
                break;
              case 14:
                strcat(newHex, "E ");
                break;
              case 15:
                strcat(newHex, "F ");
                break;
            }
          }
          else
          {
            sprintf(temp, "%d ", j);
            strcat(newHex, temp);
          }
          break;
        }
      }
    }
  }
}

// function to get binary representation of message
void getBin(char *preprocessMessage, char *binaryMessage)
{
  // initializing all values and binaryMessage for strcat
  int tempVal = 0;
  int tempBin[8];
  char temp[100];
  binaryMessage[0] = '\0';

  // parsing through all of preprocessed message
  for(int i = 0; preprocessMessage[i] != '\0'; i++)
  {

    // setting tempVal to ascii value of char in preprocessed message
    tempVal = preprocessMessage[i];
    for(int j = 7; j >= 0; j--)
    {
      // doing math to get binary rep
      tempBin[j] = tempVal % 2;
      tempVal = tempVal / 2;
    }

    // going bit by bit in int array to store in binaryMessage
    for(int j = 0; j < 8; j++)
    {
      sprintf(temp, "%d", tempBin[j]);
      strcat(binaryMessage, temp);
    }
  }
}

// function to get crc value
void getCrc(char *message, int type, char *crcVal)
{

  // initializing variables to set up for polynomial division
  int crcPoly[9];
  char noSpaceBin[10000];
  int counter = 0;
  int polyLength = 0;
  int tempChar = 0;
  int xor = 0;

  // creating crc polynomial for division with XOR
  switch(type)
  {
    // creating polynomial for each crc type and setting length
    case 3:
      int temp[] = {1, 1, 0, 1};
      polyLength = 4;
      memcpy(crcPoly, temp, sizeof(temp));
      break;
    case 4:
      int temp1[] = {1, 0, 1, 1, 0};
      polyLength = 5;
      memcpy(crcPoly, temp1, sizeof(temp1));
      break;
    case 8:
      int temp2[] = {1, 0, 0, 1, 1, 0, 1, 0, 1};
      polyLength = 9;
      memcpy(crcPoly, temp2, sizeof(temp2));
      break;
  }

  // removing all spaces from binary message for easier division
  for(int i = 0; message[i] != '\0'; i++)
  {
    if(isdigit(message[i]))
    {
      noSpaceBin[counter++] = message[i];
    }
  }

  // null terminator at end of removed space binary
  noSpaceBin[counter] = '\0';
  
  // completing XOR polynomial division
  int binLength = strlen(noSpaceBin);
  for(int i = 0; i <= binLength - polyLength; i++)
  {
    if(noSpaceBin[i] == '1')
    {
      // turning char '0' into int 0
      for(int j = 0; j < polyLength; j++)
      {
        tempChar = noSpaceBin[i + j] - '0';
        xor = tempChar ^ crcPoly[j];
        noSpaceBin[i + j] = xor + '0';
      }
    }
  }

  // starting from last "type" amount of characters
  int start = strlen(noSpaceBin) - type;
  for(int i = 0; i < type; i++)
  {
    // inserting remainder into crcVal
    crcVal[i] = noSpaceBin[start + i];
  }

  // padding with leading 0's if necessary
  int newBinLength = strlen(crcVal);
  while(newBinLength < type)
  {
    // moving bulk up
    for(int i = newBinLength; i >=0; i--)
    {
      crcVal[i + 1] = crcVal[i];
    }
    // inserting a padding 0
    crcVal[0] = '0';
    newBinLength++;
  }

  // null terminator
  crcVal[type] = '\0';
}

// function to get xor checksum value
void checkSum(char *message, char *sumBin)
{
  // initializing variables to compute XOR checksum
  int tempChar = 0;
  int tempCharTwo = 0;
  int xor = 0;

  // going by groups of 4 bits
  for(int i = 0; message[i] != '\0'; i += 4)
  {
    // setting up for first XOR computation
    if(i == 0)
    {
      for(int j = 0; j < 4; j++)
      {
        sumBin[j] = message[j];
      }
    }
    else
    {
      // looping through group of 4 to xor bits
      for(int j = 0; j < 4; j++)
      {
        tempChar = message[i + j] - '0';
        tempCharTwo = sumBin[j] - '0';
        xor = tempChar ^ tempCharTwo;
        sumBin[j] = xor + '0';
      }
    }
  }

  // null terminator
  sumBin[4] = '\0';
}