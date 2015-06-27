/* 
 * This program creates a maze of given size and solves the 
 * same using the DFS and BFS graph traversal algorithms.
 */

#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <cstdlib>
#include <time.h>
using namespace std;

/* Each cell in the matrix will have the data, index of the cell in the maze, and a list of the neighbour nodes */

typedef struct node node;
struct node
{
	char data;
	pair<int, int> index;
	vector<node*> neighbours;
};


vector< vector<node*> > buildMaze(int r, int c);
node* create(char c, int i, int j);
vector<node*>  findNeighbours(vector< vector<node*> > maze, node* currentCell, int row, int column);
bool hasWalls(vector< vector<node*> > maze, int i, int j);
bool dfs(vector< vector<node*> > maze, int row, int column, int i, int j);
void bfs(vector< vector<node*> > maze, int row, int column);
void print(vector< vector<node*> > maze, int row, int column);
void highlightpath(vector< vector<node*> > maze, int row, int column);

/*
 *  Counter to keep track of the nodes visited during traversal
 */
int cnt = 0;

int main(void)
{
	/* 
	 * Representing as row X column as it's more generic and be extended to
	 * solve non-square mazes as well.
	 * Since the problem statement mentions about square mazes, setting column = row
	 */
	int row;
	int column;

	cout<<"Enter the dimension of the square matrix: ";
	cin>>row;
	column = row;

	cout<<endl;

	/* Defining an array of nodes that make up the maze */
	vector< vector<node*> > maze;

	/*
	 * This function builds the maze
	 * (1) - Creates a square matrix of required size
	 * (2) - Randomly breaks the walls to create a maze 
	 */

	maze = buildMaze(row, column);

	/* Prints the maze */
	print(maze, row, row);

	cout<<"DFS : "<<endl;

	/* Solves the maze by using DFS graph traversal algorithm */
	dfs(maze, row, column, 1, 1);

	/* 
	 * Highlights the path from the source to the destination by 
	 * removing the visisted nodes which are not in the path 
	 */
	highlightpath(maze, row, column);

	print(maze, row, column);	

	cout<<"BFS : "<<endl;

	/* Solves the maze by using BFS graph traversal algorithm */
	bfs(maze, row, column);

	highlightpath(maze, row, column);

	print(maze, row, column); 	
}

/* Builds the maze */
vector< vector<node*> > buildMaze(int row, int column)
{
	vector< vector<node*> > maze;

	for(int i = 0; i <= 2 * row; i++)
	{
		vector<node*> tmp;
		for(int j = 0; j <= 2 * column; j++)
		{
			if(i%2 == 0)
			{
				if(j%2 == 0)
				{
					/* Creates a node */
					node* tmpNode = create('+', i, j);

					tmp.push_back(tmpNode);
				}
				else
				{
					node* tmpNode = create('-', i, j);

					tmp.push_back(tmpNode);
				}
			}
			else
			{
				if(j%2 == 0)
				{
					node* tmpNode = create('|', i, j);

					tmp.push_back(tmpNode);
				}
				else
				{
					node* tmpNode = create(' ', i, j);

					tmp.push_back(tmpNode);
				}
			}
		}
		maze.push_back(tmp);
	}

	/* Initializing an integer that holds the total number of cells */
	int totalCells = row * column;
	
	/* Initializing an integer to track the number of visited cells*/
	int visitedCells = 1;
	
	/* Stack to hold the selected neighbour of the current cell */
	stack<node*> cellStack;

	node* currentCell;
	currentCell = maze[1][1];
	srand (time(NULL));

	while(visitedCells < totalCells)
	{
		vector<node*> neighbours = findNeighbours(maze, currentCell, row, column);

		if(neighbours.size() != 0)
		{
			int tmp = rand() % neighbours.size();
			int i = neighbours[tmp]->index.first;
			int j = neighbours[tmp]->index.second;
			currentCell->neighbours.push_back(maze[i][j]);

			if(i == currentCell->index.first)
			{
				if(j > currentCell->index.second)
				{
					maze[i][j-1]->data = ' ';
				}
				else
				{
					maze[i][j+1]->data = ' ';
				}
			}
			else
			{
				if(i > currentCell->index.first)
				{
					maze[i-1][j]->data = ' ';
				}
				else
				{
					maze[i+1][j]->data = ' ';
				}
			}

			
			cellStack.push(currentCell);
			currentCell = maze[i][j];
			visitedCells++;
		}
		else
		{
			currentCell = cellStack.top();
			cellStack.pop();
		}

	}
	maze[0][1]->data = ' ';
	maze[2 * row][(2 * column) - 1]->data = ' ';
	return maze;
}


/* Creates a node and returns it */
node* create(char c, int i, int j)
{
	node* tmp = new node();
	tmp->data = c;
	tmp->index.first = i;
	tmp->index.second = j;
	return tmp;
}

/* Returns the list of neighbor cells whose walls are intact */
vector<node*> findNeighbours(vector< vector<node*> > maze, node* currentCell, int row, int column)
{
	/* Array to stores the neighbours of each cell */
	vector<node*> neighbours;
	int i = currentCell->index.first; 
	int j = currentCell->index.second;

	if((i-2) > 0 && hasWalls(maze, i-2, j))
	{
		neighbours.push_back(maze[i-2][j]);
	}

	if((i+2)< 2 * row &&hasWalls(maze, i+2, j))
	{
		neighbours.push_back(maze[i+2][j]);
	}

	if((j-2) > 0 && hasWalls(maze, i, j - 2))
	{
		neighbours.push_back(maze[i][j-2]);
	}

	if((j+2)< 2 * column &&hasWalls(maze, i, j + 2))
	{
		neighbours.push_back(maze[i][j+2]);
	}

	return neighbours;
}

/* Function to check if all the walls are intact */
bool hasWalls(vector< vector<node*> > maze, int i, int j)
{
	if(
		maze[i-1][j]->data == '-' && 
		maze[i+1][j]->data == '-' &&
		maze[i][j-1]->data == '|' && 
		maze[i][j+1]->data == '|' 
		)
	{
		return true;
	}
	return false;
}

/* Using DFS traversal to find the path from source to destination. If a node 
 * with no neighbours is encountered, we backrack and try to explore the other
 * un-explored neighbours of the parent node. We do this recursively until we
 * find the path to the destination.The path is marked while the recusrsion
 * stack unwinds after reaching the destination. 
*/
bool dfs(vector< vector<node*> > maze, int row, int column, int i, int j)
{
	node* currentNode = maze[i][j];

	if(i == 2 * row - 1 && j == 2*column - 1)
	{
		currentNode->data = (cnt % 10 + 48);

		print(maze, row, column);

		currentNode->data = '#';
		return true;
	}	

	currentNode->data = (cnt % 10 + 48);
	cnt++;

	for(unsigned int i = 0; i < currentNode->neighbours.size(); i++)
	{
		if(dfs(maze, row, column, currentNode->neighbours[i]->index.first, currentNode->neighbours[i]->index.second))
		{
			currentNode->data = '#';
			return true;
		}
	}

	return false;
}


/* Using DFS traversal to find the path from source to destination. Once
 * the destination is found, the path needs to traced back to the source node.
 * Once we find the destination, we need to find the path we took from the source to the destination
 * Since we used BFS, we would have visited all the nodes in the previous level before reaching the destination.
 * i.e., we don't know which of the visited nodes are in the path between source and destination.
 * To accomplish this, we keep a mapping which tells us the parent of each node in the graph
 * Since there are no cycles in the graph (by definition), each node has only one parent.
 * By following the parent chain from the destination all the way upto the soruce, we get the path. */
void bfs(vector< vector<node*> > maze, int row, int column)
{
	cnt = 0;
	node* currentNode = maze[1][1];

	/* Queue to maintain the order in which the nodes needs to be visited */
	queue<node*> traversedCell;

	/* Hash map to store the parents of the visited cells */
	map<node*, node*> parent;
	map<node*, node*>::iterator it;

	traversedCell.push(currentNode);

	while(!(traversedCell.back()->index.first == (2 * row - 1) && 
			traversedCell.back()->index.second == ((2 * column) - 1)))
	{
		traversedCell.front()->data = (cnt % 10 + 48);
		node *tmp = traversedCell.front();
		cnt++;
		for(unsigned int i = 0; i < tmp->neighbours.size(); i++)
		{
			traversedCell.push(tmp->neighbours[i]);
			parent.insert(make_pair(tmp->neighbours[i],tmp));
			if(tmp->neighbours[i]->index.first == (2 * row) - 1 &&
				tmp->neighbours[i]->index.second == (2 * column) - 1)
			{
				break;
			}
			
		}
		traversedCell.pop();
	}

	while(!traversedCell.empty())
	{
		traversedCell.front()->data = (cnt % 10 + 48);
		cnt++;
		traversedCell.pop();
	}

	print(maze, row, column);

	maze[2 * row - 1][2 * column - 1]->data = '#';

	it = parent.find(maze[2 * row - 1][2 * column - 1]);

	while(true)
	{
		it->second->data = '#';
		it = parent.find(it->second);
		if(it == parent.end())
		{
			break;
		}
	}
}

void print(vector< vector<node*> > maze, int row, int column)
{
	for(int i = 0; i <= 2*row; i++)
	{
		for(int j = 0; j <= 2*column; j++)
		{
			cout<<maze[i][j]->data;
		}
		cout<<endl;
	}
	cout<<endl;
}

void highlightpath(vector< vector<node*> > maze, int row, int column)
{
	for(int i = 1; i <= 2*row; i += 2)
	{
		for(int j = 1; j <= 2*column; j += 2)
		{
			if(maze[i][j]->data != '#')
			{
				maze[i][j]->data = ' ';
			}
		}
	}
	cout<<endl;
}


