/* Game of Life
*/

#include <stdio.h>
#include <stdlib.h>
#include "twoD.h"

/**
 * Steps the game forward
 * @param rows
 * @param columns
 * @param grid
 * @return
 */
char** step(int rows, int columns, char **grid){
    unsigned int top;
    unsigned int bottom;
    unsigned int left;
    unsigned int right;
    int neighbors;
    char **out = make2Dchar(rows, columns);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            // flags for edges of the grid
            top = 0;
            bottom = 0;
            left = 0;
            right = 0;
            if(i==0){
                top=1;
            }
            if(i==(rows-1)){
                bottom=1;
            }
            if(j==0){
                left=1;
            }
            if(j==(columns-1)){
                right=1;
            }
            // checks all neighbors
            neighbors=0;
            if(!right){
                if(grid[i][j+1]=='x'){
                    neighbors++;
                }
            }
            if(!left){
                if(grid[i][j-1]=='x'){
                    neighbors++;
                }
            }
            if(!top){
                if(grid[i-1][j]=='x'){
                    neighbors++;
                }
            }
            if(!bottom){
                if(grid[i+1][j]=='x'){
                    neighbors++;
                }
            }
            if(!right&&!top){
                if(grid[i-1][j+1]=='x'){
                    neighbors++;
                }
            }
            if(!left&&!top){
                if(grid[i-1][j-1]=='x'){
                    neighbors++;
                }
            }
            if(!right&&!bottom){
                if(grid[i+1][j+1]=='x'){
                    neighbors++;
                }
            }
            if(!left&&!bottom){
                if(grid[i+1][j-1]=='x'){
                    neighbors++;
                }
            }
            // checks if current cell is alive or dead
            if(grid[i][j]=='x') { // Death & survival logic
                if (neighbors <= 1 || neighbors >= 4) {
                    out[i][j] = ' ';
                } else {
                    out[i][j] = 'x';
                }
            } else{ // Birth logic
                if (neighbors==3){
                    out[i][j] = 'x';
                }
                else{
                    out[i][j] = ' ';
                }
            }
        }
    }
    free(grid);
    return out;
}

/**
 * Compares two 2D int arrays of the same specified size
 * @param rows
 * @param columns
 * @param grid1
 * @param grid2
 * @return 1 if the same and 0 if different
 */
int compare(int rows, int columns, char **grid1, char **grid2){
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            if (grid1[i][j]!=grid2[i][j]){
                return 0;
            }
        }
    }
    return 1;
}

/**
 * prints the grid from a char array
 * @param generation
 * @param rows
 * @param columns
 * @param grid
 * @return
 */
int printGrid(int generation, int rows, int columns, char **grid){
    printf("Generation %d\n", generation);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            printf("%c",grid[i][j]);
        }
        printf("\n");
    }
    return 0;
}

/**
 * Fills the specified grid with " " for initialization purposes
 * @param rows
 * @param columns
 * @param grid
 * @return an initialized grid of chars
 */
char** initializeGrid(int rows, int columns, char **grid){
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            grid[i][j]=' ';
        }
    }
    return grid;
}

/** Main function.
 * @param argc Number of words on the command line.
 * @param argv Array of pointers to character strings containing the words on the command line.
 * @return 0 if success, 1 if invalid command line or unable to open file.
 *
 */
int main(int argc, char **argv) {
	printf("Game of Life\n");

    FILE *input; // Stream descriptor for file containing initial grid
    char *inputFileName; // Name of file containing initial grid
    int rows; // Number of rows in the grid
    int columns; // Number of columns in the grid
    int gens; // Number of generations to produce
    int doPrint; // 1 if user wants to print each generation, 0 if not
    int doPause; // 1 if user wants to pause after each generation, 0 if not
    char **currGrid; // A 2D array to hold the current state
    char **pastGrid1; // A 2D array to hold the previous state
    char **pastGrid2; // A 2D array to hold the state 2 cycles before
    int inRows;
    int inColumns;
    int cell;
    int lastGen;

    // See if there are the right number of arguments on the command line
    if ((argc < 5) || (argc > 7)) {
        // If not, tell the user what to enter.
        printf("Usage:\n");
        printf("  ./life rows columns generations inputFile [print] [pause]\n");
        return EXIT_FAILURE;
    }

    // Input processing
	rows = atoi(argv[1]); // Convert from character string to integer.
	columns = atoi(argv[2]);
	gens = atoi(argv[3]);
    // checks that row, column, and generation inputs are positive
    if(rows<1||columns<1||gens<1){
        printf("Row, column, and generation inputs must be greater than 0 to run the simulation");
        return EXIT_FAILURE;
    }
	inputFileName = argv[4];
    doPrint=0;
    doPause=0;
    // Controls the doPrint variable
    if(argc >= 6){
        if(argv[5][0] == 'y'||argv[5][0]=='Y'){
            doPrint=1;
        }
        else if(argv[5][0] == 'n'||argv[5][0]=='N'){
            doPrint=0;
        }
        else{
            printf("Invalid input: use y or n to specify if the program prints");
            return EXIT_FAILURE;
        }
    }
    // Controls the doPause variable
    if(argc == 7){
        if(argv[6][0] == 'y'||argv[6][0]=='Y'){
            doPause=1;
        }
        else if(argv[6][0] == 'n'||argv[6][0]=='N'){
            doPause=0;
        }
        else{
            printf("Invalid input: use y or n to specify if the program pauses");
            return EXIT_FAILURE;
        }
    }

    input = fopen(inputFileName, "r");
    if (!input) {
        printf("Unable to open input file: %s\n", inputFileName);
        return EXIT_FAILURE;
    }

    currGrid = make2Dchar(rows, columns);
    pastGrid1 = make2Dchar(rows, columns);
    pastGrid2 = make2Dchar(rows, columns);

    // Intitializes arrays
    initializeGrid(rows, columns, currGrid);
    initializeGrid(rows, columns, pastGrid1);
    initializeGrid(rows, columns, pastGrid2);

    inColumns=0;
    inRows=0;
    int columnCount;
    // Reads through the input file to determine the size of the input in rows and columns
    while (!feof(input)){
        columnCount=0;
        while(!feof(input)){
            cell=fgetc(input);
            if(cell==10){
                inRows++;
                break;
            }
            else{
                columnCount++;
            }
        }
        if(columnCount>inColumns){
            inColumns=columnCount;
        }
    }

    // Checks input rows
    if(inColumns>columns){
        printf("Too few columns for input");
        return EXIT_FAILURE;
    }
    if(inRows>rows){
        printf("Too few rows for input");
        return EXIT_FAILURE;
    }

    input = fopen(inputFileName, "r");

    // Initial assignment of array values
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            if(j>inColumns){
                inColumns=j;
            }
            if(feof(input)){
                break;
            }
            cell=fgetc(input);
            if(cell==10){
                break;
            }
            else if(cell==120){
                pastGrid1[i][j]='x';
            }
            else{
                pastGrid1[i][j]=' ';
            }
        }
        if(i>inRows){
            inRows=i;
        }
        if(feof(input)){
            break;
        }
    }

    // calculates offsets for centering the inputs
    int offsetRow;
    offsetRow=rows/2-inRows/2;
    int offsetColumn;
    offsetColumn=columns/2-inColumns/2;

    // Offsets the grid
    for(int i=offsetRow, k=0; k<=inRows; i++, k++){
        for (int j=offsetColumn, l=0; l<=inColumns; j++, l++) {
            currGrid[i][j]=pastGrid1[k][l];
        }
    }
    // Clears the gird holding the first set of inputs
    initializeGrid(rows, columns, pastGrid1);

    // Simulation loop
    for (int k = 0; k < gens+1; ++k) {
        if(doPrint){
            printGrid(k,rows,columns,currGrid);
        }
        for (int l = 0; l < rows; ++l) {
            for (int i = 0; i < columns; ++i) {
                pastGrid2[l][i]=pastGrid1[l][i];
            }
        }
        for (int l = 0; l < rows; ++l) {
            for (int i = 0; i < columns; ++i) {
                pastGrid1[l][i]=currGrid[l][i];
            }
        }
        currGrid=step(rows,columns,currGrid);
        lastGen=k;
        if(compare(rows, columns, currGrid, pastGrid2)){
            printf("Program has ended due to repetition at generation %d\n", lastGen);
            break;
        }
        if(doPause){
            printf("Press Enter to continue\n ");
            getchar();
        }
    }
    if(!doPrint){
        printGrid(lastGen,rows,columns,currGrid);
    }

    // Frees up memory on ending the program
    free(currGrid);
    free(pastGrid1);
    free(pastGrid2);
	return EXIT_SUCCESS;
}
