#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
  // Uncomment this block to pass the first stage
  
  while (1) {
    printf("$ ");
    fflush(stdout);

    // Wait for user input
    
    char input[100];
    fgets(input, 100, stdin);
    char result[150];
    input[strlen(input)-1]='\0';
    snprintf(result, sizeof(result), "%s: command not found", input);

    if (strcmp(input, "exit 0") == 0) {
      exit(0);
    }

    if (strncmp(input, "echo", 4) == 0) {
      printf("%s\n", input + 5);
    }

    else if (input) {
      printf("%s\n", result); 
    }
  }
  
  return 0;
}
