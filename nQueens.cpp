/*
Written by Jonathan Bedingfield
Extension of 8 queens, but now for N queens
More dynamic memory allocation
Recursive solutions generator instead of loops

Correct according to Wikipedia rules

Still a simple solver.  Nothing fancy, yet
*/

#include <iostream>
#include <vector>
using namespace std;

int n;
bool** board;

bool noConflicts(int, int); //checks for conflicts above before setting down a queen
void printBoard(bool**); //helper function to visualise the board
bool** createNewBoardWithInit(); //allocates a bool** of size nxn, initalises all elements to 0, returns a pointer to it
bool** createNewBoardNoInit(); //allocates a boo** of size nxn, no initalisation, returns a pointer to it
bool** copyIntoNewBoard(); //allocates a bool** and copies board into it, then returns a pointer
void deleteBoardInstance(bool**); //helper to dealocate memory
void findSolutionsRecursive(int); //generates solutions recursivly
void removeAllNonUniqueSolutions(); //removes duplicates
bool hasVariant(int); //checks for any rotation or reflection combination
void cleanUpMemory(); //cleans up memory allocated before exiting program

vector<bool**> solutions;

int queensPlaced = 0;

int main()
{
	cout << "Enter the size board to solve" << endl;
	cin >> n;
	
	board = createNewBoardWithInit();
	
	findSolutionsRecursive(0);
	
	cout << solutions.size() << " unique solutions" << endl;
	
	cleanUpMemory();
    
    return 0;
}

/*
Takes in a 2d boolian pointer of size nxn

Checks against the list of known solutions

returns 0 if no variants found
returns 1 if any variant found
*/
bool hasVariant(bool** solutionToCheck)
{
	if(solutions.empty()) //nothing to compare to
	{
		return 0;
	}
	else
	{
		int upperBound = n - 1;
		//generate all variant holders
		bool** selfReflection = createNewBoardNoInit();
		
		bool** ninty = createNewBoardNoInit();
		bool** nintyReflection = createNewBoardNoInit();
		
		bool** oneEighty = createNewBoardNoInit();
		bool** oneEightyReflection = createNewBoardNoInit();
		
		bool** twoSeventy = createNewBoardNoInit();
		bool** twoSeventyReflection = createNewBoardNoInit();
		
		//translate data into rotation holders
		for(int i = 0; i < n; i++)
		{
			for(int j = 0; j < n; j++)
			{
				ninty[i][j] = solutionToCheck[upperBound-j][i];
				oneEighty[i][j] = solutionToCheck[upperBound-i][upperBound-j];
				twoSeventy[i][j] = solutionToCheck[j][upperBound-i];
			}
		}
		
		//translate rotations and origional into reflection holders
		for(int i = 0; i < n; i++)
		{
			for(int j = 0; j < n; j++)
			{
				selfReflection[i][j] = solutionToCheck[i][upperBound-j];
				nintyReflection[i][j] = ninty[i][upperBound-j];
				oneEightyReflection[i][j] = oneEighty[i][upperBound-j];
				twoSeventyReflection[i][j] = twoSeventy[i][upperBound-j];
			}
		}
		
		bool selfReflectionFound;
		bool nintyFound;
		bool nintyReflectionFound;
		bool oneEightyFound;
		bool oneEightyReflectionFound;
		bool twoSeventyFound;
		bool twoSeventyReflectionFound;
		
		//check all variants against all solutions
		for(int i = 0; i < solutions.size(); i++)
		{
			selfReflectionFound = 1;
			nintyFound = 1;
			nintyReflectionFound = 1;
			oneEightyFound = 1;
			oneEightyReflectionFound = 1;
			twoSeventyFound = 1;
			twoSeventyReflectionFound = 1;
			
			for(int j = 0; j < n; j++)
			{
				for(int k = 0; k < n; k++)
				{
					if(solutions[i][j][k] != selfReflection[j][k])
					{
						selfReflectionFound = 0;
					}
					if(solutions[i][j][k] != ninty[j][k])
					{
						nintyFound = 0;
					}
					if(solutions[i][j][k] != nintyReflection[j][k])
					{
						nintyReflectionFound = 0;
					}
					if(solutions[i][j][k] != oneEighty[j][k])
					{
						oneEightyFound = 0;
					}
					if(solutions[i][j][k] != oneEightyReflection[j][k])
					{
						oneEightyReflectionFound = 0;
					}
					if(solutions[i][j][k] != twoSeventy[j][k])
					{
						twoSeventyFound = 0;
					}
					if(solutions[i][j][k] != twoSeventyReflection[j][k])
					{
						twoSeventyReflectionFound = 0;
					}
				}
			}
			
			//if any dupicate was found
			if(selfReflectionFound || nintyFound || nintyReflectionFound || oneEightyFound || oneEightyReflectionFound || twoSeventyFound || twoSeventyReflectionFound)
			{
				//memory cleanup before returning
				deleteBoardInstance(selfReflection);
				deleteBoardInstance(ninty);
				deleteBoardInstance(nintyReflection);
				deleteBoardInstance(oneEighty);
				deleteBoardInstance(oneEightyReflection);
				deleteBoardInstance(twoSeventy);
				deleteBoardInstance(twoSeventyReflection);
				
				return 1;
			}
		}
		
		//memory cleanup before returning
		deleteBoardInstance(selfReflection);
		deleteBoardInstance(ninty);
		deleteBoardInstance(nintyReflection);
		deleteBoardInstance(oneEighty);
		deleteBoardInstance(oneEightyReflection);
		deleteBoardInstance(twoSeventy);
		deleteBoardInstance(twoSeventyReflection);
		
		return 0;
	}
}

void findSolutionsRecursive(int depth)
{
	if(depth != n) //if we are not at full depth, work on placement of queen
	{
		for(int i = 0; i < n; i++)
		{
			if(i > 0) //prevent index out of bounds
			{
				board[depth][i-1] = 0; //pickup last queen
			}
			
			if(noConflicts(depth, i)) //we can place a queen here
			{
				board[depth][i] = 1;
				
				if(depth == n - 1)
				{
					if(!hasVariant(board))
					{
						solutions.push_back(copyIntoNewBoard());
					}
				}
				
				findSolutionsRecursive(depth + 1);
			}
			
			if(i == (n-1)) //end of for loop
			{
				board[depth][i] = 0; //pickup queen before new iteration starts
			}
		}
	}
}

/*
Returns true if there are no conflicts, false otherwise

Since the algorithm works "top to bottom" from the chess board
there is only a need to look "above" when searching for conflicts

NOTE: there is fat to be trimmed here.  Could probably use one for loop to do all of this
*/
bool noConflicts(int row, int col)
{
	//check for conflicts directly above
	for(int i = row; i >= 0; i--)
	{
		if(board[i][col])
		{
			return 0;
		}
	}
	
	//check for conflicts up and left
	for(int i = 1; row - i >= 0 && col - i >= 0; i++)
	{
		if(board[row - i][col - i])
		{
			return 0;
		}
	}
	
	//check for conflicts up and right
	for(int i = 1; row - i >= 0 && col + i < n; i++)
	{
		if(board[row - i][col + i])
		{
			return 0;
		}
	}
	
	return 1;
}

/*
Dynamically allocates an n x n board
initialises all elements to 0
returns a pointer to it
*/
bool** createNewBoardWithInit()
{
	bool** newBoard = new bool*[n]; //allocate memory
	
	for(int i = 0; i < n; i++)
	{
		newBoard[i] = new bool[n]; //allocate memory
		
		for(int j = 0; j < n; j++)
		{
			newBoard[i][j] = 0; //initialise memory just allocated
		}
	}
	
	return newBoard;
}

/*
Dynamically allocates an n x n board
all elements are uninitialised
returns a pointer to it
*/
bool** createNewBoardNoInit()
{
	bool** newBoard = new bool*[n]; //allocate memory
	
	for(int i = 0; i < n; i++)
	{
		newBoard[i] = new bool[n]; //allocate memory
	}
	
	return newBoard;
}

/*
Helper function for dynamic memory allocation

takes in nothing and returns a pointer to an uninitialized 8x8 boolian array
*/
bool** copyIntoNewBoard()
{
	bool** newBoard = new bool*[n];
	for(int i = 0; i < n; i++)
	{
		newBoard[i] = new bool[n];
	}
	
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
		{
			newBoard[i][j] = board[i][j];
		}
	}
	
	return newBoard;
}

/*
Helper function to cleanup dynamically allocated 8x8 boards

takes in pointer and returns nothing
*/
void deleteBoardInstance(bool** oldBoard)
{
	for(int i = 0; i < n; i++)
	{
		delete[] oldBoard[i];
	}
	delete[] oldBoard;
}

/*
Helper function to visualise board.

prints 8x8 boards only
takes in pointer and returns nothing
*/
void printBoard(bool** boardPointer)
{
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
		{
			cout << boardPointer[i][j] << " ";
		}
		
		cout << endl;
	}
}

/*
Cleans up the solutions vectors dynamically allocated bool boards
*/
void cleanUpMemory()
{
	for(int i = 0; i < solutions.size(); i++)
	{
		deleteBoardInstance(solutions[i]);
	}
	solutions.clear();
	
	deleteBoardInstance(board);
}
