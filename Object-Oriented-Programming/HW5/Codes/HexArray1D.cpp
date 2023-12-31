#include "HexArray1D.h"
#include <fstream>


using namespace std;

namespace HexNamespace
{

/////////////// CONSTRUCTORS ///////////////

HexArray1D :: HexArray1D()
						: AbstractHex()
{
	setHexCells();
}

HexArray1D :: HexArray1D(const int size, const int PlayStyle)
						: AbstractHex(size, PlayStyle)
{
	setHexCells();
}

/////////////// BIG THREE ///////////////

HexArray1D :: HexArray1D (const HexArray1D & other)
{
	setSize(other.getSize());
	setHexCells();

	for (int i = 0; i < getSize(); i++)
	{
		for (int j = 0; j < getSize(); j++)
		{
			hexCells[i * getSize() + j] = other.hexCells[i * getSize() + j];
		}
	}
}

HexArray1D :: ~HexArray1D ()
{
	delete [] hexCells;
}

HexArray1D  HexArray1D :: operator = (const HexArray1D & other)
{
	setSize(other.getSize());
	Cell * c = new Cell [getSize()];
	//Cell * c = (Cell*) malloc (1);

	for (int i = 0; i < getSize(); i++)
	{
		for (int j = 0; j < getSize(); j++)
		{
			c[i * getSize() + j] = other.hexCells[i * getSize() + j];
		}
	}

	hexCells = c;
	return *this;
}

/////////////// SETTERS ///////////////

void HexArray1D :: setHexCells ()
{
	hexCells = new Cell[getSize() * getSize()];
	Cell cell;

	for (int i = 0; i < getSize(); i++)
	{
		for (int j = 0; j < getSize(); j++)
		{
			hexCells[i * getSize() + j] = cell;
			hexCells[i * getSize() + j].setRow(i + 1);
			hexCells[i * getSize() + j].setColumn('A' + j);
		}
	}
}

void HexArray1D :: setSize (const int size)
{
	AbstractHex :: setSize(size);
	setHexCells();
	reset();
}

////////////////////////////////////

void HexArray1D :: print ()
{
	int i, k, j;

	cout << " ";
	for (i = 0; i < getSize(); i++)	cout << (char)('a' + i) << " ";
	cout << endl;	

	for (i = 0; i < getSize(); i++)
	{
		cout << i + 1;
		for (k = 0; k < i; k++)	cout << " "; 
		
		for (j = 0; j < getSize(); j++)
		{
			if (hexCells[i * getSize() + j].state == Cell::cellState::X)	cout << "X ";
			else if (hexCells[i * getSize() + j].state == Cell::cellState::O)	cout << "O ";
			else	cout << ". ";
		}
		cout << endl;
	}
}

void HexArray1D :: readFromFile (string fileName)	throw(fileError)
{
	int t, ps, s;
	char str[20], c;
	fileName += 5;

	str[fileName.length() - 1] = '\0';
	ofstream file;
	file.open(fileName);		// Saves cordinates, field size, play style and turn
	
	if (file.fail())	throw fileError();
						
	else
	{
		if (getTurn() == false)	t = 0;
		else if (getTurn() == true)	t = 1;

		for(int i = 0; i < getSize(); i++)
		{
			for (int j = 0; j < getSize(); j++)
			{
				if(hexCells[i * getSize() + j].getCellState() == Cell::cellState::X)	c = 'X';
				else if (hexCells[i * getSize() + j].getCellState() == Cell::cellState::O) c = 'O';
				else	c = '.';
				file << c;
			}
		}

		ps = getPlayStyle();
		s = getSize();

		file  << endl << s  << endl << ps  << endl << t;
		file.close();
	}
}

void HexArray1D :: writeToFile (string fileName)	throw(fileError)
{
	string str, str1, str2, str3, str4;
	fileName += 5;
	int counter = 0, turnFlag, i = 0, j;		// Loads game data
	ifstream file;

	str[fileName.length() - 1] = '\0';

	file.open(fileName);
	if (file.fail())	throw fileError();

	else
	{	
		getline(file, str1);								// Reads file line by line
		getline(file, str2);
		getline(file, str3);
		getline(file, str4);
	}

	setSize(stoi(str2));
	setPlayStyle(stoi(str3));
	turnFlag = stoi(str4);

	if(!turnFlag)	setTurn(false);
	else if(turnFlag)	setTurn(true);

	setSize(getSize());

	for (i = 0; i < getSize(); i++)
	{
		for (j = 0; j < getSize(); j++)
		{
			if(str1[i * getSize() + j] == 'X')	hexCells[i * getSize() + j].setState(Cell::cellState::X);
			if(str1[i * getSize() + j] == 'O')	hexCells[i * getSize() + j].setState(Cell::cellState::O);	
			
			hexCells[i * getSize() + j].setRow(i);
			hexCells[i * getSize() + j].setColumn((char)('A' + j));
		}
	}

	file.close();
} 

void HexArray1D :: reset ()
{
	for (int i = 0; i < getSize(); i++)
	{
		for (int j = 0; j < getSize(); j++)
		{
			hexCells[i * getSize() + j].setState(Cell :: NONE);
			hexCells[i * getSize() + j].setMoveNum(0);
		}
	}
	setTurn(false);
}

void HexArray1D :: play(const int row, const int column)	throw(fullCellError)
{
	if(hexCells[row * getSize() + column].getCellState() == Cell :: cellState :: NONE)
	{
		if(getTurn() == false)
		hexCells[row * getSize() + column].setState(Cell :: cellState :: X);
		else
		hexCells[row * getSize() + column].setState(Cell :: cellState :: O);
		setPlayNum(getPlayNum() + 1);
		hexCells[row * getSize() + column].setMoveNum(getPlayNum());
		setTurn(!getTurn());		
	}
	else	throw fullCellError();
}

void HexArray1D :: play ()			// Generates moves for AI.
{
	int x1, y1;
	bool done = false;
	

	for (int i = 0; i < getSize(); i++)
	{
		for (int j = 0; j < getSize(); j++)
		{
			if (i < getSize() - 1 && !done && hexCells[(i + 1) * getSize() + j].getCellState() == Cell::cellState::O && hexCells[(i + 1) * getSize() + j].getCellState() == Cell::cellState::NONE)
			{
				x1 = i + 1;
				y1 = j;
				done = true;
			}
			else if (i > 0 && !done && hexCells[i * getSize() + j].getCellState() == Cell::cellState::O && hexCells[(i - 1) * getSize() + j].getCellState() == Cell::cellState::NONE)
			{
				x1 = i - 1;
				y1 = j;
				done = true;
			}
			else if (j < getSize() - 1 && !done && hexCells[i * getSize() + j].getCellState() == Cell::cellState::O && hexCells[i * getSize() + j + 1].getCellState() == Cell::cellState::NONE)
			{
				x1 = i;
				y1 = j + 1;
				done = true;				
			}
			else if (j > 0 && !done && hexCells[i * getSize() + j].getCellState() == Cell::cellState::O && hexCells[i * getSize() + j - 1].getCellState() == Cell::cellState::NONE)
			{
				x1 = i;
				y1 = j - 1;
				done = true;				
			}
			else if (i > 0 && j < getSize() - 1 && !done && hexCells[i * getSize() + j].getCellState() == Cell::cellState::O && hexCells[(i - 1) * getSize() + j + 1].getCellState() == Cell::cellState::NONE)
			{
				x1 = i - 1;
				y1 = j + 1;
				done = true;				
			}
			else if (j > 0 && i < getSize() - 1 && !done && hexCells[i * getSize() + j].getCellState() == Cell::cellState::O && hexCells[(i + 1) * getSize() + j - 1].getCellState() == Cell::cellState::NONE)
			{
				x1 = i + 1;
				y1 = j - 1;
				done = true;				
			}
			else if(!done && hexCells[i * getSize() + j].getCellState() == Cell::cellState::X)
			{
				if(j < getSize() - 1 && hexCells[i * getSize() + j + 1].getCellState() != Cell::cellState::X && hexCells[i * getSize() + j + 1].getCellState() != Cell::cellState::O)	
				{
					x1 = i;
					y1 = j + 1;
				}
				else if(j > 0 && hexCells[i * getSize() + j - 1].getCellState() != Cell::cellState::X && hexCells[i * getSize() + j - 1].getCellState() != Cell::cellState::O)
				{
					x1 = i;
					y1 = j - 1;
				}
				else if( i < getSize() - 1 && hexCells[(i + 1) * getSize() + j].getCellState()!= Cell::cellState::X && hexCells[(i + 1) * getSize() + j].getCellState() != Cell::cellState::O)
				{
					x1 = i + 1;
					y1 = j;
				}
				else if(i > 0 && hexCells[(i - 1) * getSize() + j].getCellState() != Cell::cellState::X && hexCells[(i - 1) * getSize() + j].getCellState() != Cell::cellState::O)
				{
					x1 = i - 1;
					y1 = j;
				}
				else if(i > 0 && j < getSize() - 1 && hexCells[(i - 1) * getSize() + j + 1].getCellState() != Cell::cellState::X && hexCells[(i - 1) * getSize() + j + 1].getCellState() != Cell::cellState::O)
				{
					x1 = i - 1;
					y1 = j + 1;
				}
				else if(i < getSize() - 1 && j > 0 && hexCells[(i + 1) * getSize() + j - 1].getCellState() != Cell::cellState::X && hexCells[(i + 1) * getSize() + j - 1].getCellState() != Cell::cellState::O)
				{
					x1 = i + 1;
					y1 = j - 1;
				}

			}			
		}
		
	}

	hexCells[x1 * getSize() + y1].setState(Cell::cellState::O);
	hexCells[x1 * getSize() + y1].setRow(x1 + 1);
	hexCells[x1 * getSize() + y1].setColumn('A' + y1);
	setPlayNum(getPlayNum() + 1);
	hexCells[x1 * getSize() + y1].setMoveNum(getPlayNum());
	
	cout << "Player 2 (Computer) played on : " << char(y1 + 'A') << x1 + 1 << endl; 
	setTurn(false);
}

void HexArray1D :: resetVector(vector <vector<AbstractHex::Cell> > &vec, int limit)
{
	Cell cell;
	vec.resize(getSize());
	for (int i = 0; i < limit; i++)
	{
		vec[i] = vector<Cell> (limit);
		for (int j = 0; j < limit; j++)
		{
			vec[i][j] = cell;		
		}

	}	
}

bool HexArray1D :: didPlayerOneWin(vector<vector<AbstractHex::Cell>> visitedCells, int x, int y, int limit)		// Recursive function for checking if Player One win the game.(Right to Left)
{
	if (y == limit - 1 && hexCells[x * getSize() + y].state == Cell::cellState::X)
	{
		return true;
	}

	if (hexCells[x * getSize() + y].state == Cell::cellState::X && ((visitedCells[x][y].state != Cell::cellState::X) && (visitedCells[x][y].state != Cell::cellState::O)))
	{
		visitedCells[x][y].state = Cell::cellState::X;

		if (y != limit - 1)
		{
			if(didPlayerOneWin(visitedCells, x, y + 1, limit))		return true;
		}

		if (x != limit - 1)
		{
			if(didPlayerOneWin(visitedCells, x + 1, y, limit))		return true;
		}

		if (x != limit - 1 && y != 0 ) 
		{
			if(didPlayerOneWin(visitedCells, x + 1, y - 1, limit))	return true;
		}
			
		if (x != 0)
		{
			if(didPlayerOneWin(visitedCells, x - 1 , y, limit))		return true;
		}

		if (y != 0)
		{
			if(didPlayerOneWin(visitedCells, x, y - 1, limit))		return true;
		}
		
		if (x != 0 && y != limit - 1)
		{
			if(didPlayerOneWin(visitedCells, x - 1 , y + 1, limit))	return true;
		}			
		
		return false;
	}
	x++;

	if (x < limit && y == 0 && didPlayerOneWin (visitedCells, x, y, limit))	return true;
		return false;
}

bool HexArray1D :: didPlayerTwoWin(vector<vector<AbstractHex::Cell>> visitedCells, int x, int y, int limit)		// Recursive function for checking if Player Two win the game.(Top to Bottom)
{
	
	if (x == limit - 1 && hexCells[x * getSize() + y].state == Cell::cellState::O)
	{
		return true;
	}

	if (hexCells[x * getSize() + y].state == Cell::cellState::O && ((visitedCells[x][y].state != Cell::cellState::X) && (visitedCells[x][y].state != Cell::cellState::O)))
	{
		visitedCells[x][y].state = Cell::cellState::O;

		if (y != limit - 1)
		{
			if(didPlayerTwoWin(visitedCells, x, y + 1, limit))		return true;
		}

		if (x != limit - 1)
		{
			if(didPlayerTwoWin(visitedCells, x + 1, y, limit))		return true;
		}

		if (x != limit - 1 && y != limit -1 ) 
		{
			if(didPlayerTwoWin(visitedCells, x + 1, y + 1, limit))	return true;
		}
			
		if (x != 0)
		{
			if(didPlayerTwoWin(visitedCells, x - 1 , y, limit))		return true;
		}

		if (y != 0)
		{
			if(didPlayerTwoWin(visitedCells, x, y - 1, limit))		return true;
		}
		
		if (x != 0 && y != 0)
		{
			if(didPlayerTwoWin(visitedCells, x - 1 , y + 1, limit))	return true;
		}			
		
		return false;
	}
	y++;

	if (y < limit && x == 0 && didPlayerTwoWin (visitedCells, x, y, limit))	return true;
		return false;
}

bool HexArray1D :: isEnd()
{	
	bool isWon = false;
	bool result1, result2;
	int limit = getSize();
	vector <vector<HexVector::Cell> > visited;

	resetVector(visited, limit);

	result1 = didPlayerOneWin(visited, 0, 0, limit);
	result2 = didPlayerTwoWin(visited, 0, 0, limit);

	if(result1)
	{
		cout << "PLAYER 1 WON" << endl;
		return true;
	}
	else if(result2)
	{
		cout << "PLAYER 2 WON" << endl;
		return true;
	}
	return false;
}

AbstractHex :: Cell HexArray1D :: operator () (int row,  int column)
{
	return hexCells[row * getSize() + column];
}

string HexArray1D :: lastMove()	throw(noMoveError)
{
	string s1, s2;

	if(getPlayNum() == 0)	throw noMoveError();
	else
	{
		for(int i = 0; i < getSize(); i++)
		{
			for (int j = 0; j < getSize(); j++)
			{	
				if(hexCells[i * getSize() + j].getMoveNum() == getPlayNum())
				{
					s1 = hexCells[i * getSize() + j].getRow() + '0';
					s2 = hexCells[i * getSize() + j].getColumn();
					break;

				}
			} 
		}
		return s1 + s2;
	}
} 

int HexArray1D :: numberofMoves ()
{
	return getPlayNum();
}

}