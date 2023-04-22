#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Colors 

#define RED "\033[0;31m"
#define YELLOW "\033[1;33m"
#define GREEN "\033[0;32m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"
#define RESET "\033[0m"


void ajouterVol(FILE *voles);
void removeVol(FILE *voles);
void displayVols(FILE *voles);
int isStringEqual(char strs[20][30], char *str);
void copyFile(FILE *originalFile, FILE *fileCopy);
void sortVols(FILE *voles);

#define COLOR_BOLD_RED "\033[1;31m";

int main() {
    FILE *vols = NULL;
    int choice;

    // Prompt messages codes
    
    printf(PURPLE "Welcome to the Flight Management System!\n\n" RESET);
    printf(BLUE "Please choose an option:\n" RESET);
    printf(BLUE "1- Add a Flight\n" RESET);
    printf(BLUE "2- Remove a Flight\n" RESET);
    printf(BLUE "3- Display Flights\n" RESET);
    printf(BLUE "4- Sort Flights\n\n"  RESET);

    printf(GREEN "Your Choice => "  RESET);

    scanf("%d", &choice);

    switch (choice) {
        case 1:
            ajouterVol(vols);
            break;
        case 2:
            removeVol(vols);
            break;
        case 3:
            displayVols(vols);
            break;
        case 4:
            sortVols(vols);
            break;
        default:
            // Prompt message with red  code
            printf("Choose another number !!" );
            break;
    }

    return 0;
}


void ajouterVol(FILE *voles)
{
    // Open the files for writing and appending
    voles = fopen("vols.txt", "a");
    FILE *sizeOfFile = fopen("size.txt", "w+");

    // Declare variables to store input values
    char villeDepart[20], villeArrivee[20], heureDepart[6], heureArrivee[6];

    // Get the input values from the user
    printf("Departure city: ");
    scanf("%s", villeDepart);

    printf("Departure time: ");
    scanf("%s", heureDepart);

    printf("Arrival city: ");
    scanf("%s", villeArrivee);

    printf("Arrival time: ");
    scanf("%s", heureArrivee);

    // Write the values to the file
    fprintf(voles, "%s %s %s %s", villeDepart, heureDepart, villeArrivee, heureArrivee);
    fputc('\n', voles);

    // Increment the size of the file
    int size;
    fscanf(sizeOfFile, "%d", &size);
    size++;
    fprintf(sizeOfFile, "%d", size);

    // Close the files
    fclose(voles);
    fclose(sizeOfFile);
}


void removeVol(FILE *voles) {
    voles = fopen("vols.txt", "r");
    FILE *tempFile = fopen("newVols.txt", "w");
    FILE *sizeOfFile = fopen("size.txt", "r");

    char vileDepart[20], vileArrive[20], heureDepart[6], heureArrive[6];
    char voleD[20], voleA[20], ch, vole[100], voleToRemove[100];
    int hDepart, mDepart, hArrive, mArrive;
    int size;

    printf(YELLOW "\nEnter the details of the flight to remove:\n\n" RESET);

    printf("Departure city: ");
    scanf("%s", vileDepart);

    printf("Departure time: ");
    scanf("%s", heureDepart);

    printf("Arrival city: ");
    scanf("%s", vileArrive);

    printf("Arrival time: ");
    scanf("%s", heureArrive);

    // Construct the string to search for
    sprintf(voleToRemove, "%s %s %s %s", vileDepart, heureDepart, vileArrive, heureArrive);

    int i = 0;
    while (!feof(voles)) {
        // Read one flight record from the file
        fscanf(voles, "%s %d %c %d %s %d %c %d\n", voleD, &hDepart, &ch, &mDepart, voleA, &hArrive, &ch, &mArrive);

        // Construct the string of the current flight record
        sprintf(vole, "%s %d%c%d %s %d%c%d", voleD, hDepart, ch, mDepart, voleA, hArrive, ch, mArrive);

        // Compare the current flight record with the one to remove
        printf("cmp: %d\n", strcmp(vole, voleToRemove));
        if (!strcmp(vole, voleToRemove)) {
            // Decrement the number of flights and write it to the size file
            size--;
            if (size < 0)
                size = 0;
            sizeOfFile = fopen("size.txt", "w");
            fprintf(sizeOfFile, "%d", size);
            printf(RED "Flight removed.\n" RESET);
        } else {
            // Write the current flight record to the temporary file
            fprintf(tempFile, "%s\n", vole);
            printf("Flight added to temp file.\n");
        }
        printf("i = %d\n", i);
        i++;
    }

    fclose(voles);
    fclose(tempFile);
    fclose(sizeOfFile);

    // Replace the original file with the temporary file
    copyFile(tempFile, voles);
}

void displayVols(FILE *voles) {
    // Open the file for reading
    voles = fopen("vols.txt", "r");
    // Check if file opened successfully
    if (voles == NULL) {
        printf("Error opening file\n");
        return;
    }

     printf(" %-10s  %-10s  %-10s   %-10s \n", "D1", "T1", "D2" , "T2");
     printf("|------------|------------|------------|------------|\n");
    char c;
    // Print the file character by character until end of file is reached
    while((c = fgetc(voles)) != EOF) {
        printf(" %c", c);
    }

    // Close the file
    fclose(voles);
}


int isStringEqual(char strs[20][30], char *str)
{
    for (int i = 0; i < 20; i++)
    {
        if (!strcmp(strs[i], str))
        {
            return 1;
        }
    }
    return 0;
}



void copyFile(FILE *originalFile, FILE *fileCopy)
{
    // Open the original file for reading and the file copy for writing
    originalFile = fopen("newVols.txt", "r");
    fileCopy = fopen("vols.txt", "w");

    // Copy characters from the original file to the file copy until the end of the file is reached
    char c;
    c = fgetc(originalFile);
    while(c != EOF)
    {
        fputc(c, fileCopy);
        c = fgetc(originalFile);
    }

    // Close both files
    fclose(originalFile);
    fclose(fileCopy);
}


void sortVols(FILE *voles)
{
    // Open input and output files
    voles = fopen("vols.txt", "r");
    FILE *newVols = fopen("newVols.txt", "w");
    FILE *sizeOfFile = fopen("size.txt", "r");

    // Declare variables
    char arrStr[20][30];  // Array of strings to store vols
    char ch, voleD[20], voleA[20], vol[100], v[100];
    int i, j, k, minHeure, minMinute, changed;
    int hDepart, mDepart, hArrive, mArrive;

    // Read the size of the input file from the size.txt file
    int size;
    fscanf(sizeOfFile, "%d", &size);

    // Initialize array of strings
    i = size;
    k = 0;
    strcpy(arrStr[0], "!!!"); // Add a sentinel value to the array

    // Loop through each line of the input file
    while(!feof(voles))
    {
        fscanf(voles, "%s %d %c %d %s %d %c %d\n", voleD, &hDepart, &ch, &mDepart, voleA, &hArrive, &ch, &mArrive);
        sprintf(vol, "%s %d%c%d %s %d%c%d", voleD, hDepart, ch, mDepart, voleA, hArrive, ch, mArrive);

        // Find the position to insert the new vol in the array
        for(j = 0; j <= k; j++)
        {
            if(strcmp(vol, arrStr[j]) < 0)
            {
                break;
            }
        }

        // Insert the new vol in the array
        for(int l = k; l >= j; l--)
        {
            strcpy(arrStr[l + 1], arrStr[l]);
        }
        strcpy(arrStr[j], vol);
        k++;
    }

    // Write the sorted vols to the output file
    for(int l = 0; l < k; l++)
    {
        fprintf(newVols, "%s\n", arrStr[l]);
    }

    // Close input and output files
    fclose(voles);
    fclose(newVols);
    fclose(sizeOfFile);

    // Overwrite the input file with the sorted vols
    copyFile(newVols, voles);
}


