#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

void print(char *str) { // legit just prints the given string. Works for both variables and straight up text.
    int len = strlen(str);
    write(1, str, len);
}

void printn(char *str) { // exact same but with a newline so i dont have to
    int len = strlen(str);
    write(1, str, len);
    write(1, "\n", 1);
}

char *input() {
    //char *str = (char*)malloc(1024);
    char *str = malloc(1024);
    read(0, str, 1024); // create a big ole buffer
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == '\n') { // if they hit the enter key turn it into a null byte rather then a new line
            str[i] = '\0';
            break;
        }
    }
    return str;
}

char **split(char *str, int *count) {
    char **splitted = malloc(100 * sizeof(char *)); //hopefully 100 words is the max
    *count = 0;
    int index = 0;

    while (str[index] != '\0') { // while we havent reached the end of the string
        while(str[index] == ' ') { // if there is more then one space between words index until it isn't
            index++;
        }
        if (str[index] == '\0') {//if we reached the end of the string break the loop and return the array
            break;
        }
        int start = index; //finds the start of the word
        while(str[index] != ' ' && str[index] != '\0') { //iterates through until the end or a space
            index++;
        }
        int end = index;// saves the ending index
        int len = end - start; // creates a variable sotring the word length
        char *word = malloc(len + 1);

        for (int j = 0; j < len; j++) { //while j is less then the length of the word
            word[j] = str[start + j]; //assign the char at the index
        }
        word[len] = '\0'; //put the nullbyte back on in the very last position in the string
        splitted[*count] = word; // add the new word into the array
        (*count)++;
    }
    return splitted;
}

void userInput() {
    char directory[1024]; //set a big ole buffer
    if (getcwd(directory, 1024) != NULL) { // if you successfully yoink the path
        print(directory); // use my python ish print function
        print(" $");
    }
    else {
        char *errMsg = "getcwd failed";
        printn(errMsg);
    }
}

// prints the working directory
void pwd() {
    char path[1024];//create a buffer for the path
    if (getcwd(path, 1024) != NULL) { //checks to see if getcwd is successful or not
        print("Current directory: ");
        printn(path);
    } else {
        print("getcwd failed");
    }
}

void debugArgs(char **args) {
    int i = 0;
    for (int i = 0; args[i] != NULL; i++) {
        if (i == 0) {
            print("command: ");
            printn(args[i]);
        }
        else {
            print("arg: ");
            printn(args[i]);
        }
    }
}

void cd(char **args) {
    if (args[1] == NULL) { // if they havent put in a directory target directory
        print("invalid input");
    }
    else {
        char *dir = args[1];
        if (chdir(dir) == -1) {
            print("could not cd into directory: ");
            printn(dir);
        } else {
            print("success!! Current working directory is: ");
            pwd();
        }
    }
}

// Simply makes main easier to look at. No other reason for this. returns true if you are exiting the program everything else returns false
bool menu(char **args, int count) {
    if (count == 0) {
        print("no input given");
    } else if (strcmp(args[0], "exit") == 0){
        print("exiting the program.....");
        return true;

    } else if (strcmp(args[0], "pwd")==0) {
        pwd();
        return false;
    } else if (strcmp(args[0], "debugargs")==0) {
        debugArgs(args);
        return false;
    } else if(strcmp(args[0], "cd") == 0) {
        cd(args);
        return false;
    } else {
        print("command not known or functional yet: ");
        printn(args[0]);
        return false;
    }
}

int main() {
    bool done = false; // initialize done value
    while(!done) { // while we aren't done running the program
        userInput();
        char *command = input();

        int count; // initialize the count variable needed for split
        char **args = split(command, &count); // create the argument array based on the string given
        done = menu(args, count); // plug the array into our menu func.

        for (int i = 0; i < count; i++) { // go through and free all elements in the list (malloc was used for each word)
            free(args[i]);
        }
        free(args); // free up both command and args
        free(command);
    }
    return 0;
}
