#include "node.h"


Node::Node(int nx, int ny, std::string ncol)
{
	x = nx;
	y = ny;
	col = ncol;
	
	for(int i = 0; i < 4; i++)
	{
		neighbors.push_back("none");
	}
}

//get
std::string Node::getC()
{
	return col;
}
std::vector<std::string> Node::getN()
{
	return neighbors;
}
int Node::getX()
{
	return x;
}
int Node::getY()
{
	return y;
}
int* Node::getLN()
{
	return lastNode;
}
int Node::getW()
{
	return which;
}


//update
void Node::updateC(std::string ncol)
{
	col = ncol;
}
void Node::updateLN(int nx, int ny)
{
	lastNode[0] = nx; lastNode[1] = ny;
}
void Node::updateN(std::vector<std::vector<Node>> grid)
{
	//up
	if(y - 1 >= 0)
	{
		neighbors[0] = grid[y - 1][x].getC();
	} else
	{
		neighbors[0] = "none";
	}
	//down
	if(y + 1 < 60)
	{
		neighbors[2] = grid[y + 1][x].getC();
	} else
	{
		neighbors[2] = "none";
	}
	//left
	if(x - 1 >= 0)
	{
		neighbors[3] = grid[y][x - 1].getC();
	} else
	{
		neighbors[3] = "none";
	}
	//right
	if(x + 1 < 80)
	{
		neighbors[1] = grid[y][x + 1].getC();
	} else
	{
		neighbors[1] = "none";
	}
}
void Node::updateW(int nw)
{
	which = nw;
}
