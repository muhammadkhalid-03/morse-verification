#include "utils.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <openssl/md5.h>
#include <pthread.h> 

// A lock
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Array of results for threads to modify the same global struct
expandable_array_t* results;

// Arguments given to the threads
typedef struct decrypt_args {
  int index;
  expandable_string_t * input;
} decrypt_args_t;

/**
 * A function designed to be given to a thread and find all posibilities to decrypt a message
 *     shares these messages by sending them to the global variable `results` at the index 
 *     given in args
 * 
 * @param args assumed to be of type decrypt_args_t
 */
void * decrypt_word_in_thread(void * args) {
  decrypt_args_t *thread_args = (decrypt_args_t *) args;
  char * new;
  expandable_string_t * message;
  // For every possible combination of the input, find the splitting, 
  //    decode the splitting, check if it is in the dectionary, and
  //    add it to an expandable_array_t
  for (int i = 0; i < pow(2, strlen(thread_args->input->string) -1); i++) {
    new = splitMorse(thread_args->input, i);
    message = decryptWord(new);
    add(message, '\n');
    message->thread = thread_args->index;
    // If the word is in the dictionary, add it to the results
    if(search_table(message->string)) {
      pthread_mutex_lock(&mutex); 
      arrayAdd(results, message);
      pthread_mutex_unlock(&mutex);
    } else {
      // results will free all valid strings, so we must free all invalid strings here
      free(message->string);
    }
    free(message);
    free(new);
  }
  return NULL;
}



int main(int argc, char** argv) {
  // Initialize where the input from the user will go
  expandable_string_t *input;

  // Determine which branch we are going down
  if (argc == 2 && strcmp(argv[1], "text") == 0) {
    // For user input for complex morse code (no spaces)

    // Initialize a place to hold the input
      input = malloc(sizeof(expandable_string_t));
      input->size = 0;
      input->capacity = STRING_CAPACITY;
      input->string = malloc(sizeof(char) * STRING_CAPACITY);
    
    printf("Type message in Morse code: ");
    char chc = getchar();
    while(chc != '\n') {
      // Ignore all spaces in the input
      if (chc != ' ') {
        add(input, chc); 
      }
      chc = getchar();
    }
  } else if (argc == 2 && (strcmp(argv[1], "simple") == 0)) {
    // Standard morse solver, requires correct spacing
    standardMorseSolver();
    return 0;
  } else if (argc == 3 && (strcmp(argv[1], "file") == 0)) {
    // For complex morse code from files

      // Initialize a place to hold the input
      input = malloc(sizeof(expandable_string_t));
      input->size = 0;
      input->capacity = STRING_CAPACITY;
      input->string = malloc(sizeof(char) * STRING_CAPACITY);

      // Read from the file
      FILE* inputFile = fopen(argv[2], "r");
      if (inputFile == NULL) {
        fprintf(stderr, "Failed to open input file %s.\n", argv[1]);
        perror(NULL);
        exit(2);
      }
      char ch = fgetc(inputFile);
      while (ch != EOF){
        if (ch != ' ' && ch != '\n') {
          add(input, ch); 
        }
        ch = fgetc(inputFile);
      }
      fclose(inputFile);
  } else if (argc == 2 && strcmp(argv[1], "file") == 0) {
    printf("Please add the filename after \"file\" \n");
      return 1;
  } else {
    // If there are more than arguments, prompt for a valid input
    printf("Please run with either \"file\" + filename or \"text\" \n");
    return 1;
  }




  // Initialize our threads to use later 
  pthread_mutex_init(&mutex, NULL);
  // Initialize the global results array
  results = calloc(1, sizeof(expandable_array_t));
  results->size = 0;
  results->capacity = 8;
  results->strings = calloc(8, sizeof(expandable_string_t));


  // Make a dictionary as a hashtable from our file
    FILE* file = fopen("american-english2", "r"); //use american-english2 b/c first one contains duplicates
    char line[256];

    if (file != NULL) {
        while (fgets(line, sizeof(line), file)) {
            add_to_hash_table(line);
        }
        fclose(file);
    }


  //split strings without strtok() b/c of null terminator issue
  char** splitMorseStrings = split_string(input->string, "/");
  int wordCount = get_length(splitMorseStrings);

  // Initalize our threads and their arguments
  pthread_t threads[wordCount];
  decrypt_args_t thread_args[wordCount];
  for (int i = 0; i < wordCount; i++) {
    thread_args[i].index = i;
    thread_args[i].input = malloc(sizeof(expandable_string_t));
    thread_args[i].input->size = strlen(splitMorseStrings[i]);
    thread_args[i].input->capacity = STRING_CAPACITY;
    thread_args[i].input->string = malloc(strlen(splitMorseStrings[i])+1);
    strcpy(thread_args[i].input->string, splitMorseStrings[i]);
    // Start running the threads
    if (pthread_create(&threads[i], NULL, decrypt_word_in_thread, &thread_args[i]) != 0) {
      perror("Failed to create thread");
      exit(1);
    }
  }
  // Wait for all threads to end and add their input to results
  for (int i = 0; i < wordCount; i++) {
    pthread_join(threads[i], NULL); 
  }

// Count how many solutions are valid
int validSolutions = 0;
// Print results in the correct sentance order
  for(int i = 0; i < wordCount; i++) {
    printf("Word %d: \n         ", i);
    for (int j = 0; j < results->size; j++){
      if (results->strings[j].thread == i) {
        int len = results->strings[j].size -1;
        // get rid of the newline at the end of the word
        results->strings[j].string[results->strings[j].size -1] = '\0';
        printf("%s  ", results->strings[j].string);
        validSolutions++;
      }
    }
    // If there were no valid words, let the user know
    if (validSolutions == 0) {
        printf("No valid English word for the morse sequence");
      }
    validSolutions = 0;
    printf("\n");
  }

  // free all of the remaining malloced pieces of memory
  free(splitMorseStrings);
  for (int i = 0; i < wordCount; i++) {
    free(thread_args[i].input->string);
    free(thread_args[i].input);
  }
  arrayFree(results);
  free(input->string);
  free(input);
  free_hash_table();
  return 0;
}