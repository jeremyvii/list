#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

typedef struct {
  // Display all hidden files and folders in a directory
  unsigned char all;
  // Display all hiddens files but "." and ".."
  unsigned char almost_all;
  // Only displays directories
  unsigned char dir;
} Options;

static Options options;

int  checkString   (char*);
int  isDir         (char*);
int  isDot         (char*);
void printDirectory(char*);

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
        abort();
        break;
    }
  }
  // Check if directory name passed is printable and isn't empty
  if (checkString(argv[argc - 1])) {
    // Print directory
    printDirectory(argv[argc - 1]);
  } else {
    // Display invalid input error
    printf("Input provided is invalid.\n");
    exit(1);
  }
  return 0;
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

int isDir(char* str) {
  if (options.dir) {
    // TODO: Set directory functionality
    char currDir[1024];
    getcwd(currDir, sizeof(currDir));
    printf("%s\n", test);
    strcat(dir, str);
    struct stat sb;
    printf("%s\n", dir);
    printf("%d\n", stat(dir, &sb) == 0);
    return stat(dir, &sb) == 0 && S_ISDIR(sb.st_mode);
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
void printDirectory(char* dirName) {
  // Create new directory object
  struct dirent** namelist;
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
    if (isDot(namelist[i]->d_name)) {
      // Don't print "." or ".."
    } else if (isDir(namelist[i]->d_name)) {
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
