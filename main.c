#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>

//This program's functionality is to process movie information files based on the user's inputs

//Function that processes a single line of the file
void processLine(char *curr, char new[24]){
    char filename[64];
    //Tokenize the line with ',' as the delimiter
    char *title = strtok(curr, ",");
    sprintf(filename, "%s/%s.txt", new, strtok(NULL, ","));

    //opens new file based on year and appends to the file
    FILE *file = fopen(filename, "a");
    fprintf(file, "%s\n", title);
    fclose(file);
    //chmod 0640 = rw-r-----
    chmod(filename, 0640);
}

//Processes the file chosen by the user
void process(char *filename){
    //Create new directory
    char new[24];
    srand((unsigned int) time(NULL));

    //Generate new directory name using the format "./caugheya.movies." and then a random number between 0 and 99999
    sprintf(new, "./caugheya.moves.%i", (int) random() % 100000);
    //Make new directory and set permissions to 0750 (rwxr-x---)
    mkdir(new, 0750);

    //Open file
    char *curr = NULL;
    size_t len = 0;
    //Open file in read mode
    FILE *file = fopen(filename, "r");
    printf("Now processing the chosen file named %s\n", filename);

    //Loop that iterates through the lines of the file
    getline(&curr, &len, file);
    while(getline(&curr, &len, file) != -1){
        //Call processLine() to process the line of the file into the new directory
        processLine(curr, new);
    }

    //Closes file
    fclose(file);
    printf("Created directory with name %s\n", new);
}



//Depending on the user input (1 or 2), the function will retrieve the largest or smallest file in the current directory
void getFile(int choice, char* filename){
    //Initialization
    // Open directory
    DIR* curr = opendir(".");
    struct dirent *xDir;
    choice--;
    off_t size;
    //Set size to the maximum value of type off_t
    if(choice == 1){
        size = LLONG_MAX;
    }
    //Otherwise, set to 0
    else{
        size = 0;
    }
    struct stat dirStat;

    //Loop until no more entries in the directory
    while((xDir = readdir(curr)) != NULL){
        //Check to see if the entry name starts with 'movies_' and ends with '.csv'
        if(!strncmp(xDir->d_name,"movies_", 7) && !strcmp(xDir->d_name + strlen(xDir->d_name) - 4, ".csv")){
            stat(xDir->d_name, &dirStat);

            //Find smallest or largest file
            if((!choice && dirStat.st_size > size) || (choice && dirStat.st_size < size)){
                //Change size to new value
                size = dirStat.st_size;
                //Copy filename to array
                memset(filename, '\0', 256 * sizeof(char));
                strcpy(filename, xDir->d_name);
            }
        }
    }
    //Close directory
    closedir(curr);
}

//Depending on the user input (1, 2, or 3), the function will retrieve the file by choosing the largest, smallest or by prompting the user for the name of the file
int getName(int choice){
    //Allows up to 299 characters + null terminator to be entered for filename
    char filename[300];
    //If option 3 was selected
    if(choice==3){  
        printf("\nEnter the complete file name: ");
        scanf("%s", filename);

        //If file not found
        if (access(filename, 0)){
            printf("The file %s was not found. Please try again\n", filename);
            return false;
        }
    }
    //Calls getFile function to find the largest or smallest file 
    else{
        getFile(choice, filename);
    }

    //Process File
    process(filename);
    return true;
}

//Menu function to ask user for a file to process
void fileToProcess(){
    int choice = 0;
    bool found = false;

    //Menu
    while(!found){
        printf("\nWhich file do you want to process?\n");
        printf("Enter 1 to pick the largest file\n");
        printf("Enter 2 to pick the smallest file\n");
        printf("Enter 3 to specify the name of a file\n");
        printf("Enter a choice from 1 to 3: ");
        scanf("%i", &choice);  

        if(choice > 0 && choice < 4){
            found = getName(choice);
        }
        else{
            printf("You entered an incorrect choice. Please try again\n");
        }
    }
}

//Main function 
int main(int argc, const char * argv[]){
    int choice = 0;
    //Menu
    while(choice != 2){
        printf("\n1. Select file to process\n");
        printf("2. Exit the program\n");
        printf("Enter a choice 1 or 2: ");
        scanf("%i", &choice);
        //If 1 entered, jump to fileToProcess function
        if(choice == 1){
            fileToProcess();
        }
        //If value other than 1 or 2 entered
        else if(choice != 2){
            printf("You've entered an incorrect choice. Please try again\n");
        }
    }
}