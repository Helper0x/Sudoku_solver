// 24.02.24
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>

// Variable
int operations = 0;
const int SIZE = 9;


// Creation
int** create_table() {
    int** sudoku = new int* [SIZE];

    for (int i = 0; i < SIZE; ++i) {
        sudoku[i] = new int[SIZE];
    }

    // Initialize the Sudoku grid
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            sudoku[i][j] = 0;
        }
    }

    return sudoku;
}

void read_file(int** &sudoku){

    std::string myText;
    std::ifstream MyReadFile("table.txt");

    int rowIndex = 0;

    while (getline(MyReadFile, myText)) {
        std::istringstream iss(myText);
        std::string token;
        int columnIndex = 0;

        while (getline(iss, token, ' ')) {
            //std::cout << "Row: " << rowIndex << ", Column: " << columnIndex << ", Value: " << token << std::endl;
            sudoku[columnIndex][rowIndex] = atoi(token.c_str());
            columnIndex++;
        }

        rowIndex++;
    }

    MyReadFile.close();

}

void show(int** sudoku) {
    for (int i = 0; i < SIZE; ++i) {
        if (i % 3 == 0 && i != 0) {
            std::cout << "------+-------+------" << std::endl;
        }
        for (int j = 0; j < SIZE; ++j) {
            if (j % 3 == 0 && j != 0) {
                std::cout << "| ";
            }
            std::cout << sudoku[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void clean_memory(int** sudoku) {
    for (int i = 0; i < SIZE; ++i) {
        delete[] sudoku[i];
    }
    delete[] sudoku;
}


// Logic
bool is_cell_valid(int**& sudoku, int row, int col, int number) {

    // Check for row
    for (int x = 0; x <= 8; x++)
        if (sudoku[row][x] == number)
            return false;

    // Check for column
    for (int x = 0; x <= 8; x++)
        if (sudoku[x][col] == number)
            return false;

    // Check if we find the same num in 3x3 cell
    int startRow = row - row % 3;
    int startCol = col - col % 3;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (sudoku[i + startRow][j + startCol] == number)
                return false;

    return true;

}

bool solve(int** sudoku, int row, int col) {

    // Check end
    if (row == SIZE - 1 && col == SIZE)
        return true;

    // Check if column value becomes 9
    if (col == SIZE) {
        row++;
        col = 0;
    }

    // Check if the current position is > 0
    if (sudoku[row][col] > 0)
        return solve(sudoku, row, col + 1);

    for (int num = 1; num <= SIZE; num++){

        // Check if it is safe to place 
        if (is_cell_valid(sudoku, row, col, num)){

            operations++;
            sudoku[row][col] = num;

            //  Checking for next possibility with next
            if (solve(sudoku, row, col + 1))
                return true;
        }

        // Backtracking
        operations++;
        sudoku[row][col] = 0;
    }

    return false;
}


// Main
int main() {

    auto start = std::chrono::high_resolution_clock::now();
    int** sudoku = create_table();
    read_file(sudoku);

    if (solve(sudoku, 0, 0)) {
        printf("Solve \n");
    }
    else {
        printf("Cant solve \n");
    }

    show(sudoku);
    clean_memory(sudoku);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    printf("\nOperations: %d \n", operations);
    std::cout << "\nDuration: " << duration.count() << " microseconds" << std::endl;
    return 0;
}
