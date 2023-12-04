#include <iostream>
#include <iomanip> // For std::setw
#include <fstream>
#include <string>

using namespace std;

const unsigned short MAX_WIDTH = 80;
const unsigned short MAX_HEIGHT = 80;
bool grid[MAX_WIDTH][MAX_HEIGHT];

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
            if (col >= 0 && col < gridWidth && row >= 0 && row < gridHeight && !(i == 0 && j == 0))
            {
                liveNeighbors += grid[col][row];
            }
        }
    }

    return liveNeighbors;
}

void shiftGridRight()
{
    for (int x = MAX_WIDTH - 1; x >= 0; x--)
    {
        for (int y = 0; y < MAX_HEIGHT; y++)
        {
            grid[x + 1][y] = grid[x][y];
        }
    }
}

void shiftGridDown()
{
    for (int y = MAX_HEIGHT - 1; y >= 0; y--)
    {
        for (int x = 0; x < MAX_WIDTH; x++)
        {
            grid[x][y + 1] = grid[x][y];
        }
    }
}

void expandGridIfNeeded()
{
    bool expandRight = false, expandDown = false, expandLeft = false, expandUp = false;

    // Check right border
    for (unsigned short y = 0; y < gridHeight; y++)
    {
        if (!grid[gridWidth - 1][y] && countLiveNeighbors(gridWidth - 1, y, gridWidth, gridHeight) == 3)
        {
            expandRight = true;
        }
    }

    // Check bottom border
    for (unsigned short x = 0; x < gridWidth; x++)
    {
        if (!grid[x][gridHeight - 1] && countLiveNeighbors(x, gridHeight - 1, gridWidth, gridHeight) == 3)
        {
            expandDown = true;
        }
    }

    // Check left border
    for (unsigned short y = 0; y < gridHeight; y++)
    {
        if (!grid[0][y] && countLiveNeighbors(0, y, gridWidth, gridHeight) == 3)
        {
            expandLeft = true;
        }
    }

    // Check top border
    for (unsigned short x = 0; x < gridWidth; x++)
    {
        if (!grid[x][0] && countLiveNeighbors(x, 0, gridWidth, gridHeight) == 3)
        {
            expandUp = true;
        }
    }

    // Perform expansion
    if (expandLeft && gridWidth < MAX_WIDTH)
    {
        ++gridWidth;
        shiftGridRight();
    }
    if (expandUp && gridHeight < MAX_HEIGHT)
    {
        ++gridHeight;
        shiftGridDown();
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
            row++;
        }
        file.close();
    }
    else
    {
        cout << "Unable to open file" << endl;
    }
}

void loadGameFromFile()
{
    string fileName;
    cout << "Enter filename: ";
    cin >> fileName;
    loadGridFromFile(fileName);
}

void startNewGame()
{

}

void drawBoard()
{
    cout << "  ";
    for (unsigned short col = 0; col < gridWidth; ++col)
    {
        if (col + 1 == 1 || col + 1 == gridWidth)
        {
            cout << setw(2) << col + 1;
        }
        else
        {
            cout << setw(2) << ' ';
        }
    }
    cout << endl;

    for (unsigned short row = 0; row < gridHeight; ++row)
    {
        if (row + 1 == 1 || row + 1 == gridHeight)
        {
            cout << setw(2) << row + 1;
        }
        else
        {
            cout << setw(2) << ' ';
        }

        for (unsigned short col = 0; col < gridWidth; ++col)
        {
            char cell = grid[col+1][row+1] ? '@' : '-';
            cout << setw(2) << cell;
        }

        cout << endl;
    }
}

void simulateLife()
{
    expandGridIfNeeded();

    bool tempGrid[MAX_WIDTH][MAX_HEIGHT] = { false };

    for (unsigned short x = 0; x < gridWidth; x++)
    {
        for (unsigned short y = 0; y < gridHeight; y++)
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
    for (unsigned short x = 0; x < gridWidth; x++)
    {
        for (unsigned short y = 0; y < gridHeight; y++)
        {
            grid[x][y] = tempGrid[x][y];
        }
    }
}

void resizeGrid()
{
}

void toggleCell()
{
}

void clearGrid()
{
}

void randomizeGrid()
{
}

void saveToFile()
{
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

int menuLoop()
{
    bool startedGame = false;
    while (!startedGame)
    {
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
            startedGame = true;
            return 1;
        case 2:
            loadGameFromFile();
            startedGame = true;
            return 1;
        case 3:
            return -1;
        default:
            clearScreen();
            cout << "Invalid choice. Please try again.\n";
        }
    }
}

int main()
{ 
    if (menuLoop() == -1)
    {
		return 0;
	}

	gameLoop();

	return 0;
}