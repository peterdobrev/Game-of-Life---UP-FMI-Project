#include <iostream>
#include <iomanip> // For std::setw
#include <fstream>
#include <string>

using namespace std;

const unsigned short MAX_WIDTH = 80;
const unsigned short MAX_HEIGHT = 24;
bool grid[MAX_WIDTH][MAX_HEIGHT];

unsigned short gridWidth = 16, gridHeight = 8;

void startNewGame()
{

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
                grid[col][row] = (line[col] == '@');
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
            char cell = grid[col][row] ? '@' : '-';
            cout << setw(2) << cell;
        }

        cout << endl;
    }
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

void simulateLife(unsigned short gridWidth, unsigned short gridHeight)
{
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

void clearScreen()
{
#ifdef _WIN32
    system("CLS");
#else
    system("clear");
#endif
}

int main()
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
            break;
        case 2:
            loadGameFromFile();
            startedGame = true;
            break;
        case 3:
            return 0;
        default:
            clearScreen();
            std::cout << "Invalid choice. Please try again.\n";
        }
    }
}