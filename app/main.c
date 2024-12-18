#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define DEFAULT_PATH "/home/roghan/.nvm/versions/node/v23.2.0/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/snap/bin"

  struct commands {
    char *echo;
    char *exit;
    char *type;
  };

  struct CmdMapping {
    char *command;
    char *typeval;
  };

int main() {

  int path_given = 0;
  char cmd_path[900];
  char *path = getenv("PATH");
  if (strcmp(path, DEFAULT_PATH)) {
    path_given = 1;
  }


  struct commands type;
  type.echo = "builtin";
  type.exit = "builtin";
  type.type = "builtin";

  struct CmdMapping cmd_mapping[] = {
    {"echo", type.echo},
    {"exit", type.exit},
    {"type", type.type}
  }; 

  int num_of_cmds = sizeof(cmd_mapping) / sizeof(cmd_mapping[0]);

  while (1) {
    int found = 0;
    int accessed = 0;
    printf("$ ");
    fflush(stdout);

    // Wait for user input
    
    char input[100];
    fgets(input, 100, stdin);
    char result[150];
    char arg_error_result[150];
    input[strlen(input)-1]='\0';
    snprintf(result, sizeof(result), "%s: command not found", input);

    if (strcmp(input, "exit 0") == 0) {
      found = 1;
      exit(0);
    }

    if (strncmp(input, "echo", 4) == 0) {
      found = 1;
      printf("%s\n", input + 5);
    }

    if (strncmp(input, "type", 4) == 0) {
      char cmd[6];
      strncpy(cmd, input + 5, sizeof(cmd) - 1);

      if (path_given) {

        char *split_path;
        split_path = strtok(path, ":");
        while (split_path != NULL) {
          snprintf(result, sizeof(result), "%s: command not found", input);
          snprintf(cmd_path, sizeof(cmd_path), "%s/%s", split_path, cmd);
          if (access(cmd_path, X_OK) == 0) {
            printf("%s is %s/%s\n", cmd, split_path, cmd);
            accessed = 1;
          } 
          
          if (!accessed) {
            printf("%s: not found", cmd);
          }
          split_path = strtok(NULL, ",");
        }
        found = 1;
      } else {
          for (int i = 0; i < num_of_cmds; i++) {
            if (strcmp(cmd, cmd_mapping[i].command) == 0) {
              if (strcmp(cmd_mapping[i].typeval, "builtin") == 0) {
                printf("%s is a shell builtin\n", cmd);
                found = 1;
              }
            }
          }
      }


    }

    if (found == 0) {
      if (strncmp(input, "type", 4) == 0) {
        snprintf(arg_error_result, sizeof(arg_error_result), "%s: not found", input);
        printf("%s\n", arg_error_result + 5);
      } else {
        printf("%s\n", result); 
      }
    }
  }
  
  return 0;
}
