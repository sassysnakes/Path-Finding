#include <vector>
#include <string>

class Node
{
	public:
	Node(int nx, int ny, std::string ncol);
	
	int getX();
	int getY();
	int* getLN();
	int getW();
	std::string getC();
	std::vector<std::string> getN();
	
	void updateW(int nw);
	void updateC(std::string ncol);//update color
	void updateLN(int nx, int ny);//update last node
	void updateN(std::vector<std::vector<Node>> grid);//update neighbors
	
	private:
	
	int x;//row
	int y;//column
	
	int lastNode[2] = {-1, -1};
	
	int which;//which start node

	std::string col;
	std::vector<std::string> neighbors;
};
