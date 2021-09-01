#include <queue>
#include <iostream>

using namespace std;

class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};

// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
	queue<Coord> coordQueue;

	int currRow = 0;
	int currCol = 0;

	coordQueue.push(Coord(sr, sc));
	maze[sr][sc] = '*';

	while (!coordQueue.empty())
	{
		currRow = coordQueue.front().r();
		currCol = coordQueue.front().c();
		coordQueue.pop();
		if (currRow == er && currCol == ec)
			return true;
		if (maze[currRow][currCol + 1] == '.')
		{
			coordQueue.push(Coord(currRow, currCol + 1));
			maze[currRow][currCol + 1] = '*';
		}
		if (maze[currRow + 1][currCol] == '.')
		{
			coordQueue.push(Coord(currRow + 1, currCol));
			maze[currRow + 1][currCol] = '*';
		}
		if (maze[currRow][currCol - 1] == '.')
		{
			coordQueue.push(Coord(currRow, currCol - 1));
			maze[currRow][currCol - 1] = '*';
		}
		if (maze[currRow - 1][currCol] == '.')
		{
			coordQueue.push(Coord(currRow - 1, currCol));
			maze[currRow - 1][currCol] = '*';
		}
	}
	return false;
}

int main()
        {
            string maze[10] = {
                "XXXXXXXXXX",
                "X...X..X.X",
                "X.XXX....X",
                "X.X.XXXX.X",
                "XXX......X",
                "X...X.XX.X",
                "X.X.X..X.X",
                "X.XXXX.X.X",
                "X..X...X.X",
                "XXXXXXXXXX"
            };
        
            if (pathExists(maze, 10,10, 4,3, 1,8))
                cout << "Solvable!" << endl;
            else
                cout << "Out of luck!" << endl;
        }
