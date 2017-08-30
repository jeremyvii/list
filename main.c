#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <dirent.h>

typedef struct {
  // Display all hidden files and folders in a directory
  unsigned int all;
} Options;

static Options options;

/**
* Checks if string is valid
*
* @param  char* str Pointer to string to check
*
* @return int       True if string is valid otherwise false
*/
int checkString(char* str) {
  // Get strings length
  int length = strlen(str);
  // Check if string is empty return false if so
  if (length < 1) return 0;
  // Loop over strings character
  for (int i = 0; i < length; i++) {
    // Check if string is above or below ASCII values
    if (isprint(str[i])) {
      // Continue character iteration
      continue;
    } else {
      // Return false
      return 0;
    }
  }
  // No errors where found so return true
  return 1;
}

/**
* Checks if string is "." or ".."
*
* @param  char* str Pointer to string to check
*
* @return int       True if file is "." or ".." otherwise false
*/
int isDot(char* str) {
  return strcmp(str, ".") == 0 || strcmp(str, "..") == 0 || str[0] == '.';
}

/**
* Print contents of directory
*
* @param  char* dirName Pointer to directory name to open
*
* @return void
*/
void printDirectoryContents(char* dirName) {
  printf("%s\n", dirName);
  // Create new directory object
  struct dirent **namelist;
  // Scan directory contents alphabetically
  int dir = scandir(dirName, &namelist, NULL, alphasort);
  // Check if directory exists
  if (dir == -1) {
    perror("scandir");
    exit(EXIT_FAILURE);
  }
  // Iterate over directory contents
  for (int i = 0; i < dir; i++) {
    // Check if file is "." or ".."
    if (!options.all && isDot(namelist[i]->d_name)) {
      // Don't print "." or ".."
    } else {
      // Print file names
      printf("%s\n", namelist[i]->d_name);
    }
    // Free file name from memory
    free(namelist[i]);
  }
  // Free file list from memory
  free(namelist);
}

int main(int argc, char** argv) {
  // Initialize opens integer
  int c;
  // Set option error to 0
  opterr = 0;
  int index;
  // Iterate over options provided
  while ((c = getopt(argc, argv, ":a:")) != -1) {
    // Check which options were passed
    switch (c) {
      case 'a':
        options.all = 1;
        break;
      default:
        abort();
        break;
    }
  }
  if (checkString(argv[optind - 1])) {
    printDirectoryContents(argv[optind - 1]);
  } else {
    printf("Bad string!\n");
    exit(1);
  }
  return 0;
}
