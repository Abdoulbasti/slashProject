#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_ARGS_NUMBER 4096
#define MAX_ARGS_STRLEN 4096

char prompt_msg[30];
int last_return_value = 0;
char* args[MAX_ARGS_NUMBER][MAX_ARGS_STRLEN];

int exit_with_val(int val){
    return val;
}

char* prompt_format(){
    prompt_msg[0] = '[';
    prompt_msg[1] = (char)(last_return_value+48);
    prompt_msg[2] = ']';
    return prompt_msg;
}

void* split_line(char* line){
    char *tmp;
    int i = 0;
    tmp = strtok(line, " ");
    while(tmp != NULL){
        write(STDOUT_FILENO, (const void*)tmp, strlen(tmp));
        tmp = strtok(NULL, " ");
    }
}

int main(int argc, char **argv){
    char* line = (char*)NULL;
    while(1){
        line = readline(prompt_format());
        add_history(line);

        split_line(line);
    }
}