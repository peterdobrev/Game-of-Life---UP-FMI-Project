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
#include <ctime>    // For time()

using namespace std;

const unsigned short MAX_WIDTH = 80;
const unsigned short MAX_HEIGHT = 80;
bool grid[MAX_WIDTH + 1][MAX_HEIGHT + 1]; // The grid is 1-based

unsigned short gridWidth = 16, gridHeight = 8;

void clearScreen()
{
#ifdef _WIN32
    system("CLS");
#else
    system("clear");
#endif
}

unsigned short countLiveNeighbors(unsigned short x, unsigned short y, unsigned short gridWidth, unsigned short gridHeight)
{
    unsigned short liveNeighbors = 0;

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            int col = x + i;
            int row = y + j;

            // Check the bounds and skip the cell itself
            if (col >= 1 && col <= gridWidth && row >= 1 && row <= gridHeight && !(i == 0 && j == 0))
            {
                liveNeighbors += grid[col][row];
            }
        }
    }

    return liveNeighbors;
}

void killCells(unsigned short xStart, unsigned short xEnd, unsigned short yStart, unsigned short yEnd)
{
    for (unsigned short i = xStart; i <= xEnd; i++)
    {
        for (unsigned short j = yStart; j <= yEnd; j++)
        {
            grid[i][j] = false;
        }
    }
}

void clearGrid()
{
    killCells(0, MAX_WIDTH, 0, MAX_HEIGHT);
}

void shiftGridRight(unsigned short positions)
{
    if (gridWidth + positions >= MAX_WIDTH)
    {
		positions = MAX_WIDTH - gridWidth - 1; // Shift only to the maximum allowed width
	}

    for (int x = gridWidth + positions; x >= positions; x--)
    {
        for (int y = 0; y < MAX_HEIGHT; y++)
        {
            try
            {
                grid[x][y] = grid[x - positions][y];

            }
            catch (const std::exception&)
            {

            }
        }
    }

    killCells(0, positions, 0, MAX_HEIGHT);
}

void shiftGridDown(unsigned short positions)
{
    if (gridHeight + positions >= MAX_HEIGHT)
    {
        positions = MAX_HEIGHT - gridHeight - 1; // Shift only to the maximum allowed height
    }

    for (int y = gridHeight + positions; y >= positions; y--)
    {
        for (int x = 0; x < MAX_WIDTH; x++)
        {
            try
            {
                grid[x][y] = grid[x][y - positions];
            }
            catch (const std::exception&)
            {

            }
        }
    }

    killCells(0, MAX_WIDTH, 0, positions);
}

void expandGridIfNeeded()
{
    bool expandRight = false, expandDown = false, expandLeft = false, expandUp = false;

    // Check right border
    for (unsigned short y = 1; y <= gridHeight; y++)
    {
        if (gridWidth >= MAX_WIDTH)
        {
            break; // No expansion if grid width exceeds or equals max width
        }

        if (!grid[gridWidth + 1][y] && countLiveNeighbors(gridWidth + 1, y, gridWidth, gridHeight) == 3)
        {
            expandRight = true;
        }
    }

    // Check bottom border
    for (unsigned short x = 1; x <= gridWidth; x++)
    {
        if (gridHeight >= MAX_HEIGHT)
        {
			break; // No expansion if grid height exceeds or equals max height
		}

        if (!grid[x][gridHeight + 1] && countLiveNeighbors(x, gridHeight + 1, gridWidth, gridHeight) == 3)
        {
            expandDown = true;
        }
    }

    // Check left border
    for (unsigned short y = 1; y <= gridHeight; y++)
    {
        if (gridWidth >= MAX_WIDTH)
        {
            break; // No expansion if grid width exceeds or equals max width
        }

        if (!grid[0][y] && countLiveNeighbors(0, y, gridWidth, gridHeight) == 3)
        {
            expandLeft = true;
        }
    }

    // Check top border
    for (unsigned short x = 1; x <= gridWidth; x++)
    {
        if (gridHeight >= MAX_HEIGHT)
        {
			break; // No expansion if grid height exceeds or equals max height
		}

        if (!grid[x][0] && countLiveNeighbors(x, 0, gridWidth, gridHeight) == 3)
        {
            expandUp = true;
        }
    }

    // Perform expansion
    if (expandLeft && gridWidth < MAX_WIDTH)
    {
        ++gridWidth;
        shiftGridRight(1);
    }
    if (expandUp && gridHeight < MAX_HEIGHT)
    {
        ++gridHeight;
        shiftGridDown(1);
    }
    if (expandRight && gridWidth < MAX_WIDTH)
    {
        gridWidth++;
    }
    if (expandDown && gridHeight < MAX_HEIGHT)
    {
        gridHeight++;
    }
}

void loadGridFromFile(const string& fileName)
{
    ifstream file(fileName);
    string line;
    unsigned short row = 0;

    if (file.is_open())
    {
        while (getline(file, line) && row < MAX_HEIGHT)
        {
            for (unsigned short col = 0; col < line.length() && col < MAX_WIDTH; col++)
            {
                grid[col+1][row+1] = (line[col] == '@');
            }
            // Update the grid dimensions
            gridWidth = max(gridWidth, min((unsigned short)line.length(), MAX_WIDTH));
            gridHeight = max(gridHeight, (unsigned short)(row + 1));
            row++;
        }
        file.close();
    }
    else
    {
        cout << "Unable to open file" << endl;
    }
}

void drawBoard()
{
    cout << "  ";
    for (unsigned short col = 1; col <= gridWidth; ++col)
    {
        if (col == 1 || col == gridWidth)
        {
            cout << setw(2) << col;
        }
        else
        {
            cout << setw(2) << ' ';
        }
    }
    cout << endl;

    for (unsigned short row = 1; row <= gridHeight; ++row)
    {
        if (row == 1 || row == gridHeight)
        {
            cout << setw(2) << row;
        }
        else
        {
            cout << setw(2) << ' ';
        }

        for (unsigned short col = 1; col <= gridWidth; ++col)
        {
            char cell = grid[col][row] ? '@' : '-';
            cout << setw(2) << cell;
        }

        cout << endl;
    }
}

void simulateLife()
{
    expandGridIfNeeded();

    bool tempGrid[MAX_WIDTH][MAX_HEIGHT] = { false };

    for (unsigned short x = 1; x <= gridWidth; x++)
    {
        for (unsigned short y = 1; y <= gridHeight; y++)
        {
            unsigned short liveNeighbors = countLiveNeighbors(x, y, gridWidth, gridHeight);
            bool isAlive = grid[x][y];

            if (isAlive && (liveNeighbors < 2 || liveNeighbors > 3))
            {
                tempGrid[x][y] = false; // Cell dies
            }
            else if (!isAlive && liveNeighbors == 3)
            {
                tempGrid[x][y] = true; // Cell becomes alive
            }
            else
            {
                tempGrid[x][y] = grid[x][y]; // Remains the same
            }
        }
    }

    // Update the main grid with the new states
    for (unsigned short x = 1; x <= gridWidth; x++)
    {
        for (unsigned short y = 1; y <= gridHeight; y++)
        {
            grid[x][y] = tempGrid[x][y];
        }
    }
}

void resizeGrid()
{
    unsigned short newX, newY;

    cout << "Current grid size: " << gridWidth << " x " << gridHeight << endl;
    cout << "Enter new width (X) and height (Y): ";
    cin >> newX >> newY;

    if (newX > MAX_WIDTH || newY > MAX_HEIGHT || newX < 1 || newY < 1)
    {
        cout << "Invalid dimensions. Please enter values between 1 and "
            << MAX_WIDTH << " for width, and between 1 and " << MAX_HEIGHT << " for height." << endl;
        return;
    }

    gridWidth = newX;
    gridHeight = newY;

    killCells(gridWidth + 1, MAX_WIDTH, gridHeight + 1, MAX_HEIGHT);
}

void toggleCell()
{
    unsigned short x, y;

    cout << "Enter the coordinates to toggle (X Y): ";
    cin >> x >> y;

    if (x > gridWidth || y > gridHeight)
    {
        // If the coordinates are outside the current grid, expand the grid
        if (x >= MAX_WIDTH || y >= MAX_HEIGHT)
        {
            cout << "Coordinates are outside the maximum allowed grid size." << endl;
            return;
        }

        if (x > gridWidth)
        {
            gridWidth = x + 1;
        }
        if (y > gridHeight)
        {
            gridHeight = y + 1;
        }
    }

    grid[x][y] = !grid[x][y];

}

void randomizeGrid()
{
    unsigned int N;
    cout << "Enter a number N for randomization (1 in N cells will be alive): ";
    cin >> N;

    // Seed the random number generator
    srand(time(0));

    for (int x = 1; x <= gridWidth; x++)
    {
        for (int y = 1; y <= gridHeight; y++)
        {
            if (N == 0)
            {
                grid[x][y] = false;
            }
            else
            {
                grid[x][y] = (rand() % N == 0);
            }
        }
    }
}

void saveToFile()
{
    string fileName;
    cout << "Enter the filename to save the game state: ";
    cin >> fileName;

    ofstream outFile(fileName);

    if (outFile.is_open())
    {
        for (unsigned short y = 1; y <= gridHeight; y++)
        {
            for (unsigned short x = 1; x <= gridWidth; x++)
            {
                outFile << (grid[x][y] ? '@' : '-');
            }
            outFile << endl;
        }
        outFile.close();
        cout << "Game saved to " << fileName << endl;
    }
    else
    {
        cout << "Unable to create or write to the file." << endl;
    }   
}

void gameLoop()
{
    bool inGame = true;
    while (inGame)
    {
        clearScreen();
        drawBoard();
        cout << "1. Step Forward\n";
        cout << "2. Resize\n";
        cout << "3. Toggle Cell\n";
        cout << "4. Clear\n";
        cout << "5. Randomize\n";
        cout << "6. Save to File\n";
        cout << "7. End Game\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
            simulateLife();
            break;
        case 2:
            resizeGrid();
            break;
        case 3:
            toggleCell();
            break;
        case 4:
            clearGrid();
            break;
        case 5:
            randomizeGrid();
            break;
        case 6:
            saveToFile();
            break;
        case 7:
            inGame = false;
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
}

void loadGameFromFile()
{
    gridWidth = 16;
    gridHeight = 8;
    clearGrid();

    string fileName;
    cout << "Enter filename: ";
    cin >> fileName;

    loadGridFromFile(fileName);

    gameLoop();
}

void startNewGame()
{
    gridWidth = 16;
    gridHeight = 8;
    clearGrid();

    gameLoop();
}

void menuLoop()
{
    bool exitProgram = false;
    while (!exitProgram)
    {
        clearScreen();
        cout << "Welcome to Conway's Game of Life\n";
        cout << "1. New Game\n";
        cout << "2. Load File\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
            startNewGame();
            break;
        case 2:
            loadGameFromFile();
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

int main()
{
    menuLoop();

    return 0;
}