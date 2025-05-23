/**
 * Assignment 2: Simple UNIX Shell
 * @file pcbtable.h
 * @author Julio C. Prudencio
 * @brief This is the main function of a simple UNIX Shell. You may add additional functions in this file for your implementation
 * @version 0.1
 */
// You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

#define MAX_LINE 80 // The maximum length command

/**
 * @brief parse out the command and arguments from the input command separated by spaces
 *
 * @param command
 * @param args
 * @return int
 */
int parse_command(char command[], char *args[])
{

    const char delimiter[] = " \n";           // seperating by spaces
    char *token = strtok(command, delimiter); // using the line above, to separate the given command
    int index = 0;                            // counter, will be set to num_args in main

    while (token != NULL) // traversal
    {
        args[index++] = token; // setting args at the index to the tokens
        token = strtok(NULL, delimiter);
    }

    args[index] = NULL; // Null-terminate the array of arguments

    return index; // Return the number of arguments
}

int launch(char **args, int num_args, bool background)
{
    pid_t pid, wpid; // declaring the pids
    int status;      // not sure but every example i saw did this

    pid = fork(); // forking the process

    if (pid == 0) // making sure its valid
    {
        // Child process
        if (execvp(args[0], args) == -1) // error handling
        {
            printf("Command not found\n");
        }
    }
    else if (pid < 0)
    {
        // Error forking
        perror("Command not\n"); // error handling
    }
    else
    {
        // Parent process
        if (!background) // check to see if it should wait or not
        {
            do
            {
                wpid = waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
    }

    return 1;
}

bool Ampersand(char **args) // function checks for the ampersand to see if we should wait
{
    bool background = false;
    for (int i = 0; args[i] != NULL; i++) // traversal
    {
        if (strstr(args[i], "&") != NULL) // if found
        {
            background = true; // we set the background to true
            args[i] = NULL;    // setting the last to null
            break;
        }
    }
    return background; // return the boolean
}

// TODO: Add additional functions if you need

char **copy(char **args, int num_args, int count) // suppose to handle the histry part
{
    int oldnum = num_args; // we have the prevous numargs

    char **temp = (char **)malloc((num_args + 1) * sizeof(char *)); // dynamic allocation
    for (int i = 0; i < num_args; i++)
    {
        temp[i] = strdup(args[i]); // copying the contents of args to temp
    }
    temp[num_args] = NULL; // making the last one null
    count++;

    return temp;
}

/**
 * @brief The main function of a simple UNIX Shell. You may add additional functions in this file for your implementation
 * @param argc The number of arguments
 * @param argv The array of arguments
 * @return The exit status of the program
 */
int main(int argc, char *argv[])
{
    char command[MAX_LINE];       // the command that was entered
    char *args[MAX_LINE / 2 + 1]; // hold parsed out command line arguments
    int should_run = 1;           /* flag to determine when to exit program */
    // TODO: Add additional variables for the implementation.
    char **temp;
    int count = 0;
    while (should_run)
    {
        printf("osh>");
        fflush(stdout);
        // Read the input command
        fgets(command, MAX_LINE, stdin);

        // Parse the input command
        int num_args = parse_command(command, args);

        temp = copy(args, num_args, count); // this was for the history function

        if (strncmp(args[0], "exit", 1) == 0) // to exit 
        {
            should_run = 1;
            break;
        }

        if (strncmp(args[0], "!!", 1) == 0) // handle when we encounter the !!
        {
            for (int i = 0; temp[i] != NULL; i++) // traversal of temp
            {
                cout << temp[i]; // printing
            }
        }

        bool background = Ampersand(args); // checks if its going to run in background
        launch(args, num_args, background); // launches the function

        // TODO: Add your code for the implementation
        /**
         * After reading user input, the steps are:
         * (1) fork a child process using fork()
         * (2) the child process will invoke execvp()
         * (3) parent will invoke wait() unless command included &
         */
    }
    return 0;
}