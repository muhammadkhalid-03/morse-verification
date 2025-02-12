#include <math.h>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include <stdbool.h>
#pragma once

#define STRING_CAPACITY 8
#define TABLE_SIZE 4096
#define SUB_TABLE_SIZE 256
#define MAX_ARGS 128


/**
 * node for elements in hash table chaining
 */
typedef struct node {
    char *word;
    struct node *next;
} node_t;

/**
 * Hash table to hold the dictionary
 */
node_t *hash_table[TABLE_SIZE] = {NULL};

typedef struct expandable_string {
  size_t size;
  size_t capacity;
  size_t thread;
  char *string;
} expandable_string_t;

typedef struct expandable_array {
  size_t size;
  size_t capacity;
  expandable_string_t *strings;
} expandable_array_t;


/**
 * hash function for a hash table of size 4096
 * convert MD5 hash to index number in array of size 4096
 * 
 * @param hash the hash to turn into an index
 */
static size_t hash_function(uint8_t *hash) {

    size_t index = 0;
    for (size_t i = 0; i < MD5_DIGEST_LENGTH; i++) {
        index = (index * 256 + hash[i]) % TABLE_SIZE; //shift by one byte to prevent collisions
    }
    return index;
}

/**
 * function to add a word to the hash table
 * computes MD5 hash of input word using MD5() function from OpenSSL library
 * 128 bit (16-byte) hash value stored in hash array of type uint8_t[16]
 *  @param word the words to add to the hash table
 */
void add_to_hash_table(const char *word) {
    uint8_t hash[MD5_DIGEST_LENGTH]; // declare array
    MD5((unsigned char *)word, strlen(word), hash); // MD5() function on input word

    size_t index = hash_function(hash);

    // create a new node
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (!new_node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    new_node->word = strdup(word); // store work in new node

    //check for successfull allocation
    if (!new_node->word) {
        fprintf(stderr, "Memory allocation failed\n");
        free(new_node);
        exit(1);
    }
    new_node->next = hash_table[index];
    hash_table[index] = new_node;
}


/**
 * prints the hash table
 */
static void print_hash_table() {
    for (size_t i = 0; i < TABLE_SIZE; i++) {
        if (hash_table[i] != NULL) {
            printf("Index %zu: ", i);
            node_t *current = hash_table[i];
            while (current != NULL) {
                printf("%s -> ", current->word);
                current = current->next;
            }
            printf("NULL\n");
        }
    }
}

/**
 * frees memory allocated for the hash table
 */
void free_hash_table() {
    for (size_t i = 0; i < TABLE_SIZE; i++) {
        node_t *current = hash_table[i];
        while (current != NULL) {
            node_t *next = current->next;
            free(current->word);
            free(current);
            current = next;
        }
    }
}

/**
 * Searches for a word in the dictionary
 * @param word word that is being searched for
 * @return returns true if word is in the dictionary, and false otherwise.
 */
bool search_table(char* word) {
    uint8_t hash[MD5_DIGEST_LENGTH]; // declare array
    MD5((unsigned char *)word, strlen(word), hash); // MD5() function on input word

    size_t index = hash_function(hash);

    if (hash_table[index] != NULL) {
            node_t *current = hash_table[index];
            while (current != NULL) {
                if (strcmp(current->word, word) == 0){
                    // The word is in the dictionary
                    return true;
                }
                current = current->next;
            }
            // The word is not at its hashed location
            return false;
    } else {
        // The hash is not in the dictionary
        return false;
    }
}


/**
 * Adds ch to the end of array, expands the array if there was not enough space
 * 
 * @param array the array to be added to
 * @param ch the character to add to the array
 */
void add(expandable_string_t *array, char ch) {
  array->size++;
  if (array->size >= array->capacity) {
    array->capacity++;
    array->string = realloc(array->string, sizeof(char) * array->capacity);
  }
  array->string[array->size - 1] = ch;
  array->string[array->size] = '\0';
}

/**
 * Adds str to the end of array, expands the array if there was not enough space
 * 
 * @param array the array to be added to
 * @param str the character to add to the array
 */
void arrayAdd(expandable_array_t *array, expandable_string_t* input) {
  array->size++;
  if (array->size >= array->capacity) {
    array->capacity *= 2;
    array->strings = realloc(array->strings, sizeof(expandable_string_t) * array->capacity);
  }
  array->strings[array->size - 1] = *input;
}

void arrayFree(expandable_array_t* array) {
  for (int i = 0; i < array->size; i++) {
    free(array->strings[i].string);
  }
  free(array->strings);
  free(array);
}


/**
 * Translates a single letter of Morse code to Roman text. 
 * 
 * @return returns the Roman letter a-z represented by the string passed in
 *    if the string was not a valid morse code letter, returns a '?'
 * @param letter a single letter of Morse code using '.' for dot and '-' for 
 *    dash. If other chars are used, it will return a '?'
 */
char morseToRoman(char *letter) {
  if (strcmp(letter, ".") == 0)
    return 'e';
  else if (strcmp(letter, "-") == 0)
    return 't';
  else if (strcmp(letter, "..") == 0)
    return 'i';
  else if (strcmp(letter, ".-") == 0)
    return 'a';
  else if (strcmp(letter, "-.") == 0)
    return 'n';
  else if (strcmp(letter, "--") == 0)
    return 'm';
  else if (strcmp(letter, "...") == 0)
    return 's';
  else if (strcmp(letter, "..-") == 0)
    return 'u';
  else if (strcmp(letter, ".-.") == 0)
    return 'r';
  else if (strcmp(letter, ".--") == 0)
    return 'w';
  else if (strcmp(letter, "-..") == 0)
    return 'd';
  else if (strcmp(letter, "-.-") == 0)
    return 'k';
  else if (strcmp(letter, "--.") == 0)
    return 'g';
  else if (strcmp(letter, "---") == 0)
    return 'o';
  else if (strcmp(letter, "....") == 0)
    return 'h';
  else if (strcmp(letter, "...-") == 0)
    return 'v';
  else if (strcmp(letter, "..-.") == 0)
    return 'f';
  else if (strcmp(letter, ".-..") == 0)
    return 'l';
  else if (strcmp(letter, ".--.") == 0)
    return 'p';
  else if (strcmp(letter, ".---") == 0)
    return 'j';
  else if (strcmp(letter, "-...") == 0)
    return 'b';
  else if (strcmp(letter, "-..-") == 0)
    return 'x';
  else if (strcmp(letter, "-.-.") == 0)
    return 'c';
  else if (strcmp(letter, "-.--") == 0)
    return 'y';
  else if (strcmp(letter, "--..") == 0)
    return 'z';
  else if (strcmp(letter, "--.-") == 0)
    return 'q';
  else
    return '?';
}

/**
 * Takes morseWord in the form of a string like ".... . .-.. .-.. ---" and
 *    turns it to a string in Roman text like "hello". Characters that are 
 *    invalid will give '?' values.
 * 
 * @return returns a pointer to the malloced expandable_string_t that stores the 
 *    translated string
 * @param morseword is a string seperated by spaces
 */
 expandable_string_t * decryptWord(char *morseWord) {
  // Copy morseword so it is not modified
  char* copy = strdup(morseWord);
  if (!copy) {
    fprintf(stderr, "Memory allocation failed\n");
    exit(1);
  }

  // Allocate memory for the struct that will be returned
  expandable_string_t *input = malloc(sizeof(expandable_string_t));
  if (!input) {
    fprintf(stderr, "Memory allocation failed\n");
    free(copy);
    exit(1);
  }

  // Initialize the struct
  input->size = 0;
  input->capacity = STRING_CAPACITY;
  input->string = malloc(sizeof(char) * STRING_CAPACITY);
  if (!input->string) {
    fprintf(stderr, "Memory allocation failed\n");
    free(copy);
    free(input);
    exit(1);
  }

  // For each morse character, add the corresponding letter to the string
  char *morseLetter;
  char *saveptr;
  morseLetter = strtok_r(copy, " ", &saveptr);
  while (morseLetter != NULL) {
    add(input, morseToRoman(morseLetter));
    morseLetter = strtok_r(NULL, " ", &saveptr);
  }

  // Free and return
  free(copy);
  return input;
}


/**
 * Find the length of a char** array
 * @param array the array 
 * @return an int representing the number of elements in the array
 */
int get_length(char** array) {
  int length = 0;
  while(array[length] != NULL) {
    length++;
  }
  return length;
}

/**
 * Splits the string into an array of strings where any delimiter char is found
 * @param str the string to be splitting - not modified
 * @param delimiter the delimiter to split the string at
 * @return an array of strings that were split at the delimiter, user is required to free it
 */
char** split_string(char* str, char* delimiter) {
  char* current_position = str;
  char** arguments = malloc(sizeof(char*) * (MAX_ARGS + 1));
  int total_args = 0;
  while (true) {
    if (total_args >= MAX_ARGS) {
      printf("Maximum Arguments Reached");
      exit(EXIT_FAILURE);
    }
    // Call strpbrk to find the next occurrence of a delimeter
    char* delim_position = strpbrk(current_position, delimiter);
    // if delimiter was at the start, skip it
    if (delim_position == current_position) {
      current_position++;
      continue;
    }

    if (delim_position == NULL) {
      // There were no more delimeters.
      char* delim_position = strpbrk(current_position, "\n");
      // if we have a newline, replace it with null character
      if (delim_position != NULL) {
        *delim_position = '\0';
      }
      arguments[total_args] = current_position;
      // printf("Look here: %ssadf\n", arguments[total_args]);
      total_args++;
      arguments[total_args] = NULL;

      return arguments;
    } else {
      // Overwrite the delimeter with a null terminator so we can print just this fragment
      *delim_position = '\0';
      arguments[total_args] = current_position;
      total_args++;
    }

    // Move our current position in the string to one character past the delimeter
    current_position = delim_position + 1;
  }
}


/**
 * Adds a space to string at the index of spaceLoc.
 * 
 * @pre Assumes string has enough space already for a space to be added
 * @param string a string that will get modified to have an additional space
 * @pre spaceLoc should be between 0 and the length of string
 * @param spaceLoc the index to add a space to
 */
void addSpace(char *string, int spaceLoc) {
  int n = strlen(string);
  char temp = string[spaceLoc];
  string[spaceLoc] = ' ';
  for (int i = spaceLoc + 1; i < n + 1; i++) {
    char shift = string[i];
    string[i] = temp;
    temp = shift;
  }
}

/**
 * Splits a string by spaces with an integer.
 * 0 leaves no spaces, 2^(string length -1) returns a space between every letter
 * There is some integer between 0-2^(string length -1) that leads to any 
 *     particular splitting of the string
 * 
 * @return a string split correctly according to spaces
 * @param original an expandable_string_t that is not modified, but is copied to 
 *    have spaces added to the copy
 * @param intSplit the integer that decides how the string is split
 */
char *splitMorse(expandable_string_t *original, int intSplit) {
  size_t size = original->size;
  size_t max = pow(2, size - 1);
  char *returnString = calloc(2*size, sizeof(char));
  strcpy(returnString, original->string);
  int curPow = max / 2;
  for (int i = 0; i < size - 1; i++) {
    if (intSplit - curPow >= 0) {
      intSplit = intSplit - curPow;
      addSpace(returnString, size - i - 1);
    }
    if (curPow == 1) {
      curPow = 0;
    } else {
      curPow = curPow / 2;
    }
  }
  return returnString;
}

/**
 * Cracks morse code like most morse code solvers.
 * Asks for user input, requires that the user input be in the correct format, 
 *    prints the translation with question marks instead of unknown characters
 *    correct format from user is in the form ".... . .-.. .-.. --- / .... .."
 */
void standardMorseSolver() {
  // Initialize a place to hold the input
      expandable_string_t* input = malloc(sizeof(expandable_string_t));
      input->size = 0;
      input->capacity = STRING_CAPACITY;
      input->string = malloc(sizeof(char) * STRING_CAPACITY);
    // Get user input
    printf("Type message in Morse code: ");
    char chc = getchar();
    while(chc != '\n') {
      add(input, chc); 
      chc = getchar();
    }
    char ** strs = split_string(input->string, "/");
    for (int i = 0; i < get_length(strs); i++) {
      expandable_string_t * message = decryptWord(strs[i]);
      printf("%s\n", message->string);
      free(message->string);
      free(message);
    }
    // Free everything
    free(input->string);
    free(input);
    free(strs);
} 