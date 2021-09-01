#include <stack>
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
	stack<Coord> coordStack;

	int currRow = 0;
	int currCol = 0;

	coordStack.push(Coord(sr, sc));
	maze[sr][sc] = '*';

	while (!coordStack.empty())
	{
		currRow = coordStack.top().r();
		currCol = coordStack.top().c();
		coordStack.pop();
		if (currRow == er && currCol == ec)
			return true;
		if (maze[currRow][currCol + 1] == '.')
		{
			coordStack.push(Coord(currRow, currCol + 1));
			maze[currRow][currCol + 1] = '*';
		}
		if (maze[currRow + 1][currCol] == '.')
		{
			coordStack.push(Coord(currRow + 1, currCol));
			maze[currRow + 1][currCol] = '*';
		}
		if (maze[currRow][currCol - 1] == '.')
		{
			coordStack.push(Coord(currRow, currCol - 1));
			maze[currRow][currCol - 1] = '*';
		}
		if (maze[currRow - 1][currCol] == '.')
		{
			coordStack.push(Coord(currRow - 1, currCol));
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
