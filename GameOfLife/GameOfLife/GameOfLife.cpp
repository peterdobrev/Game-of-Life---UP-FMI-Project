/**
*
* Solution to course project #2
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2023/2024
*
* @author Petar Dobrev
* @idnumber 8MI0600344
* @compiler VC
*
*
*
*/


#include <iostream>
#include <iomanip>  // For std::setw
#include <fstream>  // For file I/O
#include <string>   // Used for file I/O
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time() and clock(), used for time delay

using namespace std;

const unsigned short MAX_WIDTH = 80;
const unsigned short MAX_HEIGHT = 80;

bool hasInputErrors() {
    if (cin.fail()) {
        cin.clear(); // Clears error flags
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignores the rest of the current line
        return true;
    }

    return false;
}

void timeDelay(const int milliseconds) {
    long pause;
    clock_t now, then;

    pause = milliseconds * (CLOCKS_PER_SEC / 1000);
    now = clock();
    then = clock();

    while ((now - then) < pause) {
        now = clock();
    }
}

void clearScreen() {
#ifdef _WIN32
    system("CLS");
#else
    system("clear");
#endif
}

unsigned short countLiveNeighbors(const unsigned short x, const unsigned short y, 
                                  const bool grid[MAX_WIDTH + 1][MAX_HEIGHT + 1], 
                                  const unsigned short& gridWidth, const unsigned short& gridHeight) {
    unsigned short liveNeighbors = 0;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int col = x + i;
            int row = y + j;

            // Check the bounds and skip the cell itself
            if (col >= 1 && col <= gridWidth && row >= 1 && row <= gridHeight && !(i == 0 && j == 0)) {
                liveNeighbors += grid[col][row];
            }
        }
    }

    return liveNeighbors;
}

void killCells(unsigned short xStart, unsigned short xEnd, 
               unsigned short yStart, unsigned short yEnd, 
               bool grid[MAX_WIDTH + 1][MAX_HEIGHT + 1]) {
    if (xStart > xEnd || yStart > yEnd) {
        return;
    }

   xEnd = min(xEnd, MAX_WIDTH);
   yEnd = min(yEnd, MAX_HEIGHT);

    for (unsigned short i = xStart; i <= xEnd; i++) {
        for (unsigned short j = yStart; j <= yEnd; j++) {
            grid[i][j] = false;
        }
    }
}

void clearGrid(bool grid[MAX_WIDTH + 1][MAX_HEIGHT + 1]) {
    killCells(0, MAX_WIDTH, 0, MAX_HEIGHT, grid);
}

void shiftGridRight(const unsigned short positions, bool grid[MAX_WIDTH + 1][MAX_HEIGHT + 1], unsigned short& gridWidth) {
    if (gridWidth + positions >= MAX_WIDTH) {
        return; // Shift only to the maximum allowed width
    }

    if (positions == 0) {
        return;
    }

    for (int x = gridWidth + positions; x >= positions; x--) {
        for (int y = 0; y < MAX_HEIGHT; y++) {
            grid[x][y] = grid[x - positions][y];
        }
    }

    gridWidth += positions;

    killCells(0, positions, 0, MAX_HEIGHT, grid);
}

void shiftGridDown(const unsigned short positions, bool grid[MAX_WIDTH + 1][MAX_HEIGHT + 1], unsigned short& gridHeight) {
    if (gridHeight + positions >= MAX_HEIGHT) {
        return; // Shift only to the maximum allowed height
    }

    for (int y = gridHeight + positions; y >= positions; y--) {
        for (int x = 0; x < MAX_WIDTH; x++) {
            grid[x][y] = grid[x][y - positions];
        }
    }

    gridHeight += positions;

    killCells(0, MAX_WIDTH, 0, positions, grid);
}

void expandGridForSimulation(bool grid[MAX_WIDTH + 1][MAX_HEIGHT + 1], unsigned short& gridWidth, unsigned short& gridHeight) {
    bool expandRight = false, expandDown = false, expandLeft = false, expandUp = false;

    // Check right border
    for (unsigned short y = 1; y <= gridHeight; y++) {
        if (gridWidth >= MAX_WIDTH) {
            break; // No expansion if grid width exceeds or equals max width
        }

        if (!grid[gridWidth + 1][y] && countLiveNeighbors(gridWidth + 1, y, grid, gridWidth, gridHeight) == 3) {
            expandRight = true;
        }
    }

    // Check bottom border
    for (unsigned short x = 1; x <= gridWidth; x++) {
        if (gridHeight >= MAX_HEIGHT) {
            break; // No expansion if grid height exceeds or equals max height
        }

        if (!grid[x][gridHeight + 1] && countLiveNeighbors(x, gridHeight + 1, grid, gridWidth, gridHeight) == 3) {
            expandDown = true;
        }
    }

    // Check left border
    for (unsigned short y = 1; y <= gridHeight; y++) {
        if (gridWidth >= MAX_WIDTH) {
            break; // No expansion if grid width exceeds or equals max width
        }

        if (!grid[0][y] && countLiveNeighbors(0, y, grid, gridWidth, gridHeight) == 3) {
            expandLeft = true;
        }
    }

    // Check top border
    for (unsigned short x = 1; x <= gridWidth; x++) {
        if (gridHeight >= MAX_HEIGHT) {
            break; // No expansion if grid height exceeds or equals max height
        }

        if (!grid[x][0] && countLiveNeighbors(x, 0, grid, gridWidth, gridHeight) == 3) {
            expandUp = true;
        }
    }

    // Perform expansion
    if (expandLeft && gridWidth < MAX_WIDTH) {
        ++gridWidth;
        shiftGridRight(1, grid, gridWidth);
    }
    if (expandUp && gridHeight < MAX_HEIGHT) {
        ++gridHeight;
        shiftGridDown(1, grid, gridWidth);
    }
    if (expandRight && gridWidth < MAX_WIDTH) {
        gridWidth++;
    }
    if (expandDown && gridHeight < MAX_HEIGHT) {
        gridHeight++;
    }
}

void loadGridFromFile(const string& fileName, bool grid[MAX_WIDTH + 1][MAX_HEIGHT + 1], 
                      unsigned short& gridWidth, unsigned short& gridHeight) {
    ifstream file(fileName);
    string line;
    unsigned short row = 0;

    if (file.is_open()) {
        while (getline(file, line) && row < MAX_HEIGHT) {
            for (unsigned short col = 0; col < line.length() && col < MAX_WIDTH; col++) {
                grid[col + 1][row + 1] = (line[col] == '@');
            }
            // Update the grid dimensions
            gridWidth = max(gridWidth, min((unsigned short)line.length(), MAX_WIDTH));
            gridHeight = max(gridHeight, (unsigned short)(row + 1));
            row++;
        }
        file.close();
    }
    else {
        cout << "Unable to open file" << endl;
    }
}

void drawBoard(const bool grid[MAX_WIDTH + 1][MAX_HEIGHT + 1], const unsigned short& gridWidth, const unsigned short& gridHeight) {
    cout << "  ";
    for (unsigned short col = 1; col <= gridWidth; ++col) {
        if (col == 1 || col == gridWidth) {
            cout << setw(2) << col;
        }
        else {
            cout << setw(2) << ' ';
        }
    }
    cout << endl;

    for (unsigned short row = 1; row <= gridHeight; ++row) {
        if (row == 1 || row == gridHeight) {
            cout << setw(2) << row;
        }
        else {
            cout << setw(2) << ' ';
        }

        for (unsigned short col = 1; col <= gridWidth; ++col) {
            char cell = grid[col][row] ? '@' : '-';
            cout << setw(2) << cell;
        }

        cout << endl;
    }
}

void simulateLife(bool grid[MAX_WIDTH + 1][MAX_HEIGHT + 1], unsigned short& gridWidth, unsigned short& gridHeight) {
    expandGridForSimulation(grid, gridWidth, gridHeight);

    bool tempGrid[MAX_WIDTH + 1][MAX_HEIGHT + 1] = { false };

    for (unsigned short x = 1; x <= gridWidth; x++) {
        for (unsigned short y = 1; y <= gridHeight; y++) {
            unsigned short liveNeighbors = countLiveNeighbors(x, y, grid, gridWidth, gridHeight);
            bool isAlive = grid[x][y];

            if (isAlive && (liveNeighbors < 2 || liveNeighbors > 3)) {
                tempGrid[x][y] = false; // Cell dies
            }
            else if (!isAlive && liveNeighbors == 3) {
                tempGrid[x][y] = true; // Cell becomes alive
            }
            else {
                tempGrid[x][y] = grid[x][y]; // Remains the same
            }
        }
    }

    // Update the main grid with the new states
    for (unsigned short x = 1; x <= gridWidth; x++) {
        for (unsigned short y = 1; y <= gridHeight; y++) {
            grid[x][y] = tempGrid[x][y];
        }
    }
}

void resizeGrid(unsigned short newX, unsigned short newY, unsigned short& gridWidth, unsigned short& gridHeight) {
    if (newX > 1 && newX <= MAX_WIDTH && newY > 1 && newY <= MAX_HEIGHT)         {
        gridWidth = newX;
        gridHeight = newY;
    }
}

void resizeGridMenu(bool grid[MAX_WIDTH + 1][MAX_HEIGHT + 1], unsigned short& gridWidth, unsigned short& gridHeight) {
    unsigned short newX, newY;

    cout << "Current grid size: " << gridWidth << " x " << gridHeight << endl;
    cout << "Maximum allowed grid size is " << MAX_WIDTH << " x " << MAX_HEIGHT << endl;
    cout << "Enter new width (X) and height (Y): ";
    cin >> newX >> newY;

    while (hasInputErrors() || newX > MAX_WIDTH || newY > MAX_HEIGHT || newX < 1 || newY < 1) {
        cout << "Invalid dimensions. Please enter values between 1 and "
            << MAX_WIDTH << " for width, and between 1 and " << MAX_HEIGHT << " for height." << endl;
        cout << "Enter new width (X) and height (Y): ";
        cin >> newX >> newY;
    }

    resizeGrid(newX, newY, gridWidth, gridHeight);

    killCells(gridWidth + 1, MAX_WIDTH, gridHeight + 1, MAX_HEIGHT, grid);
}

bool isCellValid(const short x, const short y, const unsigned short& gridWidth, const unsigned short& gridHeight, bool canBeNegative = false) {
    bool isXValid, isYValid;

    if (canBeNegative) {
        isXValid = x <= MAX_WIDTH 
            && (1 - x + gridWidth <= MAX_WIDTH); // Check to see if the grid can not be expanded to fit the cell
        isYValid = y <= MAX_HEIGHT
            && (1 - y + gridHeight <= MAX_HEIGHT); // Check to see if the grid can not be expanded to fit the cell
	}
    else         {
    		isXValid = x <= MAX_WIDTH && x >= 1;
		isYValid = y <= MAX_HEIGHT && y >= 1;
    }
    
    return isXValid && isYValid;
}

void toggleCell(const unsigned short x, const unsigned short y, bool grid[MAX_WIDTH + 1][MAX_HEIGHT + 1], const unsigned short& gridWidth, const unsigned short& gridHeight) {
    if (!isCellValid(x, y, gridWidth, gridHeight)) {
		return;
	}
    grid[x][y] = !grid[x][y];
}

void toggleCellMenu(bool grid[MAX_WIDTH + 1][MAX_HEIGHT + 1], unsigned short& gridWidth, unsigned short& gridHeight) {
    short x, y;

    cout << "Maximum allowed grid size is " << MAX_WIDTH << " x " << MAX_HEIGHT << endl;
    cout << "Enter the coordinates to toggle (X Y): ";
    cin >> x >> y;


    while (hasInputErrors() || !isCellValid(x,y, gridWidth, gridHeight, true)) {
        cout << "Invalid coordinates. Please enter values between " << (-1) * (MAX_WIDTH - gridWidth - 1) << " and "
            << MAX_WIDTH << " for X, and between " << (-1) * (MAX_HEIGHT - gridHeight - 1) << " and " << MAX_HEIGHT << " for Y." << endl;
		cout << "Enter the coordinates to toggle (X Y): ";
		cin >> x >> y;
	}

    bool shouldShiftRight = x < 1;
    bool shouldShiftDown = y < 1;

    if (x > gridWidth || y > gridHeight) {
        // If the coordinates are outside the current grid, expand the grid

        if (x > gridWidth) {
            resizeGrid(x, gridHeight, gridWidth, gridHeight);
        }
        if (y > gridHeight) {
            resizeGrid(gridWidth, y, gridWidth, gridHeight);
        }
    }

    // Adjust coordinates if they are negative
    if (x < 1) {
        shiftGridRight(1 - x, grid, gridWidth);
        x = 1;
    }

    if (y < 1) {
        shiftGridDown(1 - y, grid, gridWidth);
        y = 1;
    }

    toggleCell(x, y, grid, gridWidth, gridHeight);
}

void randomizeGrid(bool grid[MAX_WIDTH + 1][MAX_HEIGHT + 1], const unsigned short& gridWidth, const unsigned short& gridHeight) {
    unsigned int N = 0;

    while (hasInputErrors() || N < 1 || N > 10000000) {
        cout << "Enter a number N for randomization (1 in N cells will be alive): ";
        cin >> N;
    }

    // Seed the random number generator
    srand(time(0));

    for (int x = 1; x <= gridWidth; x++) {
        for (int y = 1; y <= gridHeight; y++) {
            if (N == 0) {
                grid[x][y] = false;
            }
            else {
                grid[x][y] = (rand() % N == 0);
            }
        }
    }
}

void saveToFile(const bool grid[MAX_WIDTH + 1][MAX_HEIGHT + 1], const unsigned short& gridWidth, const unsigned short& gridHeight) {
    string fileName;
    cout << "Enter the filename to save the game state: ";
    cin >> fileName;

    ofstream outFile(fileName);

    if (outFile.is_open()) {
        for (unsigned short y = 1; y <= gridHeight; y++) {
            for (unsigned short x = 1; x <= gridWidth; x++) {
                outFile << (grid[x][y] ? '@' : '-');
            }
            outFile << endl;
        }
        outFile.close();
        cout << "Game saved to " << fileName << endl;
    }
    else {
        cout << "Unable to create or write to the file." << endl;
    }
}

void autoPlay(const short steps, const short delay, bool grid[MAX_WIDTH + 1][MAX_HEIGHT + 1], unsigned short& gridWidth, unsigned short& gridHeight) {
    for (unsigned short i = 1; i <= steps; i++) {
        clearScreen();
        drawBoard(grid, gridWidth, gridHeight);
        cout << "Step " << i << " of " << steps << endl;
        simulateLife(grid, gridWidth, gridHeight);
        timeDelay(delay);
    }
}

void autoPlayMenu(bool grid[MAX_WIDTH + 1][MAX_HEIGHT + 1], unsigned short& gridWidth, unsigned short& gridHeight) {
    short steps;
    cout << "Enter the number of steps to simulate: ";
    cin >> steps;

    while (hasInputErrors() || steps < 1) {
		cout << "Invalid number of steps. Please enter a positive number." << endl;
		cout << "Enter the number of steps to simulate: ";
		cin >> steps;
	}

    short delay;
    cout << "Enter the delay between steps (in milliseconds): ";
    cin >> delay;

    while (hasInputErrors() || delay < 0) {
        cout << "Invalid delay. Please enter a non-negative number." << endl;
        cout << "Enter the delay between steps (in milliseconds): ";
        cin >> delay;        
    }

    autoPlay(steps, delay, grid, gridWidth, gridHeight);
}

void gameLoop(bool grid[MAX_WIDTH + 1][MAX_HEIGHT + 1], unsigned short& gridWidth, unsigned short& gridHeight) {
    bool inGame = true;
    while (inGame) {
        clearScreen();
        drawBoard(grid, gridWidth, gridHeight);
        cout << "1. Step Forward\n";
        cout << "2. Auto-Play\n";
        cout << "3. Resize\n";
        cout << "4. Toggle Cell\n";
        cout << "5. Clear\n";
        cout << "6. Randomize\n";
        cout << "7. Save to File\n";
        cout << "8. End Game\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        if (hasInputErrors()) {
            continue; // Skip the rest of the loop iteration
        }

        
        switch (choice) {
            case 1:
                simulateLife(grid, gridWidth, gridHeight);
                break;
            case 2:
                autoPlayMenu(grid, gridWidth, gridHeight);
                break;
            case 3:
                resizeGridMenu(grid, gridWidth, gridHeight);
                break;
            case 4:
                toggleCellMenu(grid, gridWidth, gridHeight);
                break;
            case 5:
                clearGrid(grid);
                break;
            case 6:
                randomizeGrid(grid, gridWidth, gridHeight);
                break;
            case 7:
                saveToFile(grid, gridWidth, gridHeight);
                break;
            case 8:
                inGame = false;
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}

bool fileExists(const string& fileName) {
    ifstream file(fileName);
    return file.good();
}

void loadGameFromFile(bool grid[MAX_WIDTH + 1][MAX_HEIGHT + 1], unsigned short& gridWidth, unsigned short& gridHeight) {
    clearGrid(grid);

    string fileName;
    cout << "Enter filename: ";
    cin >> fileName;

    while (fileName.empty() || !fileExists(fileName)) {
        cout << "Invalid file name or file does not exist.\n";
        cout << "Enter filename: ";
        cin >> fileName;
    }

    loadGridFromFile(fileName, grid, gridWidth, gridHeight);

    gameLoop(grid, gridWidth, gridHeight);
}

void startNewGame(bool grid[MAX_WIDTH + 1][MAX_HEIGHT + 1], unsigned short& gridWidth, unsigned short& gridHeight) {
    clearGrid(grid);

    gameLoop(grid, gridWidth, gridHeight);
}

void menuLoop() {
    bool grid[MAX_WIDTH + 1][MAX_HEIGHT + 1]{ false }; // The grid is 1-based

    unsigned short gridWidth = 16, gridHeight = 8;

    bool exitProgram = false;
    while (!exitProgram) {
        clearScreen();
        cout << "Welcome to Conway's Game of Life\n";
        cout << "1. New Game\n";
        cout << "2. Load File\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        if (hasInputErrors()) {
			continue; // Skip the rest of the loop iteration
		}

        switch (choice) {
            case 1:
                startNewGame(grid, gridWidth, gridHeight);
                break;
            case 2:
                loadGameFromFile(grid, gridWidth, gridHeight);
                break;
            case 3:
                exitProgram = true;
                break;
            default:
                clearScreen();
                cout << "Invalid choice. Please try again.\n";
        }
    }
}

int main() {
    menuLoop();

    return 0;
}