// Fix compilation errors regarding <dirent.h>
#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

// This will be used for storing options based to command. 
struct Options {
  // Display all hidden files and folders in a directory
  unsigned char all;
  // Display all hiddens files but "." and ".."
  unsigned char almost_all;
  // Only displays directories
  unsigned char dir;
};

// Create new instance of options struct
static struct Options options;

// Define function prototypes
void append      (char*, char);
int  checkString (char*);
int  isntDir     (char*);
int  isDot       (char*);
int  (*fnPointer)(const struct dirent**, const struct dirent**);
void list        (char*);
int  noCaseSort  (const struct dirent**, const struct dirent**);
void strToLower  (char*);
void throwError  (char*);

int main(int argc, char** argv) {
  // Initialize opens integer
  int c;
  // Set option error to 0
  opterr = 0;
  // Iterate over options provided
  while ((c = getopt(argc, argv, ":aAd:")) != -1) {
    // Check which options were passed
    switch (c) {
      // Display all case
      case 'a':
        options.all = 1;
        break;
      // Display almost all case
      case 'A':
        options.almost_all = 1;
        break;
      case 'd':
        options.dir = 1;
        break;
      default:
        throwError("Invalid options provided.");
        break;
    }
  }
  // Check if directory name passed is printable and isn't empty
  if (argc > 1 && checkString(argv[argc - 1])) {
    // Print directory
    list(argv[argc - 1]);
  } else {
    // Display invalid input error
    throwError("Input provided is invalid.");
  }
  return 0;
}

/**
 * Appends character to the end of a string
 *
 * @param char* str The string to be appended too
 * @param char  c   The character we are appending
 *
 * @return void
 */
void append(char* str, char c) {
  // Get strings size
  int size = strlen(str);
  // Append character provided to the end of string
  str[size] = c;
  // Append the string ternimator
  str[size + 1] = '\0';
}

/**
 * Checks if string is printable and not empty
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
 * Checks if given string isn't a directory with stat()
 *
 * @param  char* str The file/folder name to check
 *
 * @return int       1 if name isn't a directory, otherwise 0.
 *                   If the "-d" option wasn't passed returns 0 regardless
 */
int isntDir(char* str) {
  // Check if directory flag is set
  if (options.dir) {
    // Intialize struct for checking if path is a directory
    struct stat sb;
    // Get inode attributes from stat
    // TODO: Add case for dots, "." and ".."
    stat(str, &sb);
    // Check if path is a directory and return results
    return S_ISREG(sb.st_mode);
  }
  return 0;
}

/**
 * Checks if string is ".", ".." or a hidden file. Results vary depending on
 * options provided to command
 *
 * @param  char* str Pointer to string to check
 *
 * @return int       True if file is "." or ".." otherwise false
 */
int isDot(char* str) {
  // Number for result recieved (should be 0 or 1)
  unsigned int result;
  // Check which option was passed
  // Display everything including hidden files and directories
  if (options.all) {
    result = 0;
  // Display all hidden files and directories except "." and ".."
  } else if (options.almost_all) {
    result = strcmp(str, ".") == 0 || strcmp(str, "..") == 0;
  // Default case; Don't display hidden files and directories
  } else {
    result = strcmp(str, ".") == 0 || strcmp(str, "..") == 0 || str[0] == '.';
  }
  return result;
}

/**
 * Print contents of directory
 *
 * @param  char* dirName Pointer to directory name to open
 *
 * @return void
 */
void list(char* dirName) {
  // Create new directory object
  struct dirent** namelist;
  // Pass sort function to funtion pointer
  fnPointer = &noCaseSort;
  // Scan directory contents alphabetically
  int dir = scandir(dirName, &namelist, NULL, fnPointer);
  // Check if directory exists
  if (dir == -1) throwError("Directory does not exist!");
  // Iterate over directory contents
  for (int i = 0; i < dir; i++) {
    // Check if file is "." or ".."
    if (isDot(namelist[i]->d_name)) {
      // Don't print "." or ".."
    } else if (isntDir(namelist[i]->d_name)) {
      // Don't display regular files if directory flag is set
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

/**
 * Runs a case insensitive alphabetic string compare
 *
 * This is needed because using alphasort is case sensitive
 *
 * @param  const struct dirent** a First directory name to check
 * @param  const struct dirent** b Second directory name to check
 *
 * @return int                     Result of strcmp
 */
int noCaseSort(const struct dirent** a, const struct dirent** b) {
  // Get values to compare, and cast to character array pointer
  char* nameA = (char*) (*a)->d_name;
  char* nameB = (char*) (*b)->d_name;
  // Copy strings so we don't print the lowercased strings in list()
  char* copyA = strdup(nameA);
  char* copyB = strdup(nameB);
  // Convert characters to lowercase
  strToLower(copyA);
  strToLower(copyB);
  // Compare results
  int result = strcmp(copyA, copyB);
  // Free our allocated memory
  free(copyA);
  free(copyB);
  return result;
}

/**
 * Converts all characters in a string to lowercase
 * 
 * @param  char* str String pointer to convert to lowercase
 *
 * @return void
 */
void strToLower(char* str) {
  // Loop over character array
  for ( ; *str; ++str)
    // Convert character to lowercase
    *str = tolower(*str);
}

/**
 * Throws error message provided
 *
 * Prints message provided and exits with a status of 1
 *
 * @param  char* msg Error message to be printed
 *
 * @return void
 */
void throwError(char* msg) {
  printf("%s\n", msg);
  exit(1);
}
