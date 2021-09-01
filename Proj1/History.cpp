#include "History.h"
#include <iostream>

using namespace std;

History::History(int nRows, int nCols) {
	m_rows = nRows;
	m_cols = nCols;
	for (int i = 0; i < m_rows; i++) {
		for (int j = 0; j < m_cols; j++) {
			m_hGrid[i][j] = '.';
		}
	}
}

bool History::record(int r, int c) {
	if (r < 0 || r > MAXROWS || c < 0 || c > MAXCOLS) 
		return false;
	if (m_hGrid[r - 1][c - 1] == '.')
		m_hGrid[r - 1][c - 1] = 'A';
	else if (m_hGrid[r - 1][c - 1] < 'Z')
		m_hGrid[r - 1][c - 1]++;
	else
		return true;
	return true;
}

void History::display() const {
	clearScreen();
	for (int r = 0; r < m_rows; r++)
	{
		for (int c = 0; c < m_cols; c++) {
			cout << m_hGrid[r][c];
		}
		cout << endl;
	}
	cout << endl;
}