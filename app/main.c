#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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


  char cmd_path[900];
  char exe_path[900];
  char *path = getenv("PATH");



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
    char first_arg[100];
    char input_copy[100];
                



    fgets(input, 100, stdin);
    char result[150];
    char arg_error_result[150];
    input[strlen(input)-1]='\0';
    snprintf(result, sizeof(result), "%s: command not found", input);

    strcpy(first_arg, input);
    strcpy(input_copy, input);
    char *sep = strchr(first_arg, ' ');
    if (sep != NULL) {
      *sep = '\0';
    }


    if (strcmp(input, "exit 0") == 0) {
      found = 1;
      exit(0);
    }

    if (strncmp(input, "echo", 4) == 0) {
      found = 1;
      printf("%s\n", input + 5);
    }


      char cmd[100];
      strncpy(cmd, input + 5, sizeof(cmd) - 1);


      for (int i = 0; i < num_of_cmds; i++) {
        if (strcmp(cmd, cmd_mapping[i].command) == 0) {
          if (strcmp(cmd_mapping[i].typeval, "builtin") == 0) {
            printf("%s is a shell builtin\n", cmd);
            found = 1;
          }
        }
      }
      if (!found) {
        if (strncmp(input, "type", 4) == 0) {
        char *token = strtok(input_copy, " ");
        token = strtok(NULL, " ");
        if (token == NULL) {
            printf("type: argument required\n");
            continue;
        }
        strcpy(first_arg, token);
    }

    char path_copy[1024];
    strncpy(path_copy, path, sizeof(path_copy) - 1);
    path_copy[sizeof(path_copy) - 1] = '\0';

    char *split_path = strtok(path_copy, ":");
    while (split_path != NULL) {
        snprintf(cmd_path, sizeof(cmd_path), "%s/%s", split_path, first_arg);

        if (strncmp(input, "type", 4) == 0) {
            if (access(cmd_path, F_OK) == 0) {
                printf("%s is %s\n", first_arg, cmd_path);
                found = 1;
                break;
            }
        } else if (access(cmd_path, X_OK) == 0) {
            pid_t pid = fork();
            if (pid == 0) {
                char *args[20];
                char *arg_token = strtok(input_copy, " ");
                int argc = 0;
                while (arg_token != NULL) {
                    args[argc++] = arg_token;
                    arg_token = strtok(NULL, " ");
                }
                args[argc] = NULL;
                execvp(cmd_path, args);
                perror("execvp failed");
                exit(1);
            } else if (pid > 0) {
                int status;
                waitpid(pid, &status, 0);
            }
            found = 1;
            break;
        }
        split_path = strtok(NULL, ":");
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
