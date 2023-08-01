#include <SDL2/SDL.h>
#include <string>
#include <vector>

class Node{
	public:
	Node(int nx, int ny, int nw, int nmrow, std::string ncol);
	
	int getX();
	int getY();
	int getW();
	
	std::string getCol();
		
	std::vector<std::string> getNeighbors();
	
	
	int gethscore();
	int getgscore();
	int getfscore();
	int* getlastNode(); // x and y of last node
	
	void updateNeighbors(std::vector<std::vector<Node>> grid);
	void updateColor(std::string ncol);
	
	void updateLN(int nx, int ny);
	void updateG(int g);
	void updateH(int h);
	void updateF();
	
	private:
	
	int hscore;
	int gscore;
	int fscore;
	int lastnode[2] = {0, 0};
	
	int x;//x coordinate on row
	int y;//y coordinate on column
	int w;//width and height (square)
	int mrow;//max rows/column
	std::string col;//color of node
	std::vector<std::string> neighbors;	//u, r, d, l (clockwise)
};
