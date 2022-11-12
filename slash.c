#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_ARGS_NUMBER 4096
#define MAX_ARGS_STRLEN 4096

char prompt_msg[30];
int last_return_value = 0;
char args[MAX_ARGS_NUMBER][MAX_ARGS_STRLEN];
char command[MAX_ARGS_STRLEN];
char error_msg[MAX_ARGS_STRLEN];

char* prompt_format(){
    prompt_msg[0] = '[';
    prompt_msg[1] = (char)(last_return_value+48);
    prompt_msg[2] = ']';
    return prompt_msg;
}

int split_line(char* line){
    char *tmp;
    int i = 0;
    if(strcmp((const char*) line, (const char*) "") == 0){
        strcpy(command, (const char*) "");
        return 0;
    }
    tmp = strtok(line, " ");
    strcpy(command, (const char*) tmp);
    while(tmp != NULL){
        if(i != 0){
            strcpy(args[i-1], (const char*) tmp);
        }
        tmp = strtok(NULL, " ");
        i++;
    }
    return i-1;
}

void fexit(int val){
    exit(val);
}

int interpretation_command(int argc){
    if (strcmp((const char*) command, (const char*) "") == 0){
        return last_return_value;
    }
    if(strcmp((const char*) command, (const char*) "exit") == 0){
        switch (argc)
        {
        case 0:
            fexit(last_return_value);
            break;
        case 1:
            int tmp;
            sscanf(args[0], "%d", &tmp);
            fexit(tmp);
        default:
            
        }
    }
    return 0;
}

int main(int argc, char **argv){
    char* line = (char*)NULL;
    while(1){
        line = readline(prompt_format());
        add_history(line);

        int nb_args = split_line(line);
        interpretation_command(nb_args);
    }
}