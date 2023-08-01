#include "Node.h"

Node::Node(int nx, int ny, int nw, int nmrow, std::string ncol)
{
	x = nx;
	y = ny;
	w = nw;
	mrow = nmrow;
	col = ncol;
	for(int i = 0; i < 4;i ++)
	{
		neighbors.push_back("white");
	}
}
//get methods
int Node::getX()
{
	return x;
}
int Node::getY()
{
	return y;
}
int Node::getW()
{
	return w;
}
std::string Node::getCol()
{
	return col;
}
std::vector<std::string>  Node::getNeighbors()
{
	return neighbors;
}
//update methods
void Node::updateColor(std::string ncol)
{
	col = ncol;
}

void Node::updateNeighbors(std::vector<std::vector<Node>> grid)
{
	//up
	if(y - 1 >= 0)
	{
		neighbors[0] = grid[y - 1][x].getCol();
	} else
	{
		neighbors[0] = "none";
	}
	
	//down
	if(y + 1 < mrow)
	{
		neighbors[2] = grid[y + 1][x].getCol();
	} else
	{
		neighbors[2] = "none";
	}
	
	//left
	if(x - 1 >= 0)
	{
		neighbors[3] = grid[y][x - 1].getCol();
	} else
	{
		neighbors[3] = "none";
	}
	
	//right
	if(x + 1 < mrow)
	{
		neighbors[1] = grid[y][x + 1].getCol();
	} else
	{
		neighbors[1] = "none";
	}
}


int Node::gethscore(){return hscore;}
int Node::getgscore(){return gscore;}
int Node::getfscore(){return fscore;}
int* Node::getlastNode(){return lastnode;}

bool Node::getU(){return used;}

void Node::updateF(){fscore = gscore + hscore;}
void Node::updateG(int g){gscore = g;}
void Node::updateH(int h){hscore = h;}
void Node::updateLN(int nx, int ny){lastnode[0] = nx; lastnode[1] = ny;}
void Node::updateB(){used = true;}
