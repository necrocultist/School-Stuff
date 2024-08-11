#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 250
#define MAX_HISTORY_SIZE 100

char *history[MAX_HISTORY_SIZE];    // Array to store command history
int history_count = 0;  // Counter for the number of commands in history

// Function to separate arguments from a command
void separate_arguments(char *input, char **arguments)
{
    int i = 0;
    char *token = strtok(input, " \n"); // Tokenize based on spaces, tabs, and newline characters
    while (token != NULL)
    {
        arguments[i++] = token;
        token = strtok(NULL, " \n");
    }
    arguments[i] = NULL;
}

// Function to separate multiple commands in a single input line
void separate_commands(char *input, char **commands)
{
    int i = 0;
    char *token = strtok(input, ";\n"); // Tokenize based on semicolons and newline characters
    while (token != NULL)
    {
        // Trim leading and trailing spaces from each command
        while (*token == ' ')
        {
            token++;
        }
        char *end = token + strlen(token) - 1;
        while (end > token && *end == ' ')
        {
            *end-- = '\0';
        }

        commands[i++] = token;
        token = strtok(NULL, ";\n");
    }
    commands[i] = NULL;
}

// Function to separate multiple pipes in a single command
void separate_pipes(char *input, char **pipes)
{
    int i = 0;
    char *token = strtok(input, "|"); // Tokenize based on pipes
    while (token != NULL)
    {
        // Trim leading spaces
        while (*token == ' ')
        {
            token++;
        }

        // Trim trailing spaces
        char *end = token + strlen(token) - 1;
        while (end > token && *end == ' ')
        {
            *end-- = '\0';
        }

        pipes[i++] = token;
        token = strtok(NULL, "|");
    }
    pipes[i] = NULL;
}

// Function to separate multiple lines in a single input
void separate_lines(char *input, char **lines)
{
    int i = 0;
    char *token = strtok(input, "\n"); // Tokenize based on newline characters
    while (token != NULL)
    {
        // Trim leading spaces
        while (*token == ' ')
        {
            token++;
        }

        // Trim trailing spaces
        char *end = token + strlen(token) - 1;
        while (end > token && *end == ' ')
        {
            *end-- = '\0';
        }

        lines[i++] = token;
        token = strtok(NULL, "\n");
    }
    lines[i] = NULL;
}

// Function to add a command to history
void add_to_history(char *command)
{
    // Shift commands in history to make space for the new command
    for (int i = MAX_HISTORY_SIZE - 1; i > 0; i--)
    {
        history[i] = history[i - 1];
    }
    // Allocate memory for the new command and copy it
    history[0] = strdup(command);
    // Increment history count
    if (history_count < MAX_HISTORY_SIZE)
    {
        history_count++;
    }
}

// Function to display the command history
void display_history()
{
    for (int i = history_count - 1; i >= 0; i--)
    {
        printf("%d %s\n", history_count - i, history[i]);
    }
}

// Function to execute the cd command
void execute_cd_command(char *directory)
{
    // Remove trailing newline character if present
    if (directory[strlen(directory) - 1] == '\n')
        directory[strlen(directory) - 1] = '\0';

    // Change the directory
    if (chdir(directory) != 0)
    {
        perror("Error changing directory");
    }
}

// Function to execute the quit command
void execute_quit_command()
{
    exit(EXIT_SUCCESS);
}

// Function to execute the history command
void execute_history_command()
{
    display_history();
}

// Function to execute a non-builtin command
void execute_non_builtin_command(char *command, char *inout) {
    int inpipes[2], outpipes[2];
    if (pipe(outpipes) == -1 || pipe(inpipes) == -1) {
        perror("Error creating pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("Error forking process");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        close(outpipes[0]); // Close the read end of the output pipe
        dup2(outpipes[1], STDOUT_FILENO); // Redirect standard output to the output pipe
        close(outpipes[1]); // Close the write end of the output pipe

        close(inpipes[1]); // Close the write end of the input pipe
        dup2(inpipes[0], STDIN_FILENO); // Redirect standard input to the input pipe
        close(inpipes[0]); // Close the read end of the input pipe

        char **args = (char **)malloc(sizeof(char *));
        separate_arguments(command, args); // Tokenize the command into arguments
        execvp(args[0], args); // Execute the command
        perror("Error executing command");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        close(inpipes[0]); // Close the read end of the input pipe
        write(inpipes[1], inout, strlen(inout)); // Write to the input pipe
        close(inpipes[1]); // Close the write end of the input pipe

        char buffer[1024] = "";
        read(outpipes[0], buffer, sizeof(buffer)); // Read from the output pipe
        close(outpipes[0]); // Close the read end of the output pipe
        close(outpipes[1]); // Close the write end of the output pipe

        strcpy(inout, buffer);
    }
}

// Function to execute commands
void execute_commands(char **commands, char *inout) {
    int i = 0;
    char output[2048] = "";
    int non_builtin_executed = 0; // Flag to track whether a non-built-in command was executed
    
    while (commands[i] != NULL) {
        char temp[2048];
        strcpy(temp, inout);
        add_to_history(commands[i]); // Add command to history

        if (strncmp(commands[i], "cd", 2) == 0) {
            execute_cd_command(commands[i] + 3); // Execute cd command
        } else if (strcmp(commands[i], "quit") == 0) {
            execute_quit_command(); // Execute quit command
        } else if (strcmp(commands[i], "history") == 0) {
            execute_history_command(); // Execute history command
        } else {
            execute_non_builtin_command(commands[i], temp); // Execute non-builtin command
            non_builtin_executed = 1; // Set flag to indicate a non-built-in command was executed
        }
        
        strcat(output, temp);
        i++;
    }
    
    strcpy(inout, output);
    
    if (non_builtin_executed) { // Print "Output:" only if a non-built-in command was executed
        printf("\nOutput: %s\n", inout);
    }
}

// Function to read input from the user
int read_input(char *input) {
    if (!fgets(input, MAX_COMMAND_LENGTH, stdin)) {
        if (feof(stdin)) { // Check if (Ctrl+D)
            printf("\nExiting...\n");
            return 0; // Return 0 to indicate end of input
        } else {
            perror("Error reading input");
            exit(EXIT_FAILURE);
        }
    }
    return 1; // Return 1 to indicate successful input reading
}

// Function to execute commands from a batch file
void execute_batch_file(const char *file_name) {
    FILE *batch_file = fopen(file_name, "r");
    if (!batch_file) {
        perror("Error opening script file");
        exit(EXIT_FAILURE);
    }
    
    // Process batch file
    char input[MAX_COMMAND_LENGTH];
    while (fgets(input, MAX_COMMAND_LENGTH, batch_file) != NULL) {
        printf("\n>> %s", input); // Echo input in batch mode

        // Separate the input line into pipes
        char *pipes[250];
        separate_lines(input, pipes);
        separate_pipes(input, pipes);

        char buffer[2048] = "";

        // Execute commands for each pipe segment
        int i = 0;
        while (pipes[i] != NULL) {
            char *commands[250];
            commands[0] = pipes[i];
            separate_commands(pipes[i], commands); // Tokenize the pipe segment into commands
            execute_commands(commands, buffer);    // Execute commands
            i++;
        }
        // printf("\nOutput: %s\n", buffer);
    }
    
    fclose(batch_file);
}

// Function to execute commands in the current shell
void execute_commands_in_shell(char *input) {
    // Separate the input line into pipes
    char *pipes[250];
    separate_lines(input, pipes);
    separate_pipes(input, pipes);

    char buffer[2048] = "";

    // Execute commands for each pipe segment
    int i = 0;
    while (pipes[i] != NULL) {
        char *commands[250];
        commands[0] = pipes[i];
        separate_commands(pipes[i], commands); // Tokenize the pipe segment into commands
        execute_commands(commands, buffer);    // Execute commands
        i++;
    }
    // printf("\nOutput: %s\n", buffer);
}

// Function to run the shell
void run_shell() {
    char input[MAX_COMMAND_LENGTH];
    while (1) {
        printf("\n>> ");
        if (!read_input(input)) {
            break; // Exit the loop if Ctrl+D is pressed
        }

        // Check if the input begins with "bash"
        if (strncmp(input, "bash", 4) == 0 && (input[4] == ' ' || input[4] == '\n')) {
            // Remove "bash" and the following space or newline from the input
            char *file_name = input + 5;
            if (file_name[0] != '\0' && file_name[strlen(file_name) - 1] == '\n') {
                file_name[strlen(file_name) - 1] = '\0'; // Remove newline if present
            }
            
            execute_batch_file(file_name);
            continue; // Skip the regular interactive mode
        }

        // Execute commands in the current shell
        execute_commands_in_shell(input);
    }
}

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        printf("Usage: %s\n", argv[0]);
        return EXIT_FAILURE;
    }

    run_shell();

    return 0;
}
