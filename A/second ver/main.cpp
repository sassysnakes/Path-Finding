#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>


#include "Node.h"

//set up global variables
SDL_Window* win;
SDL_Renderer* rend;
const std::string BLUE = "blue";//start
const std::string ORANGE = "orange";//end
const std::string PURPLE = "purple";//path
const std::string WHITE = "white";//empty
const std::string BLACK = "black";//wall
const std::string GREEN = "green";//open
const std::string RED = "red";//closed

int gwheight;
int hwheight;

void draw(std::vector<std::vector<Node>> grid, int width)
{
	for(int i = 0; i < grid.size(); i++)
	{
		for(int c = 0; c < grid[i].size(); c++)
		{
			if(grid[i][c].getCol() == BLUE)
			{
				SDL_SetRenderDrawColor(rend, 0, 0, 255, 255);
			}
			if(grid[i][c].getCol() == ORANGE)
			{
				SDL_SetRenderDrawColor(rend, 255, 165, 0, 255);
			}
			if(grid[i][c].getCol() == PURPLE)
			{
				SDL_SetRenderDrawColor(rend, 128, 0, 128, 255);
			}
			if(grid[i][c].getCol() == WHITE)
			{
				SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
			}
			if(grid[i][c].getCol() == BLACK)
			{
				SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
			}
			if(grid[i][c].getCol() == GREEN)
			{
				SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);
			}
			if(grid[i][c].getCol() == RED)
			{
				SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
			}
			SDL_Rect r = {grid[i][c].getX()*width, grid[i][c].getY()*width, width, width};
			SDL_RenderFillRect(rend, &r);
			SDL_SetRenderDrawColor(rend, 128, 128, 128, 255);
			SDL_RenderDrawRect(rend, &r);
		}
	}
	SDL_RenderPresent(rend);
}

int getDistance(int x1, int y1, int x2, int y2)
{
	return std::pow(std::abs(x1 - x2), 2.) + std::pow(std::abs(y1 - y2), 2.);
}

void retracePath(std::vector<std::vector<Node>> grid, std::vector<std::vector<int>> open_set, int c, int width, int gscore, int start[2])//grid, open_set, node number in open_set, width to draw
{
	//get node that touches end
	int touch_end[2] = {0, 0};
	touch_end[0] = open_set[c][0];
	touch_end[1] = open_set[c][1];
	
	//change node color
	grid[touch_end[1]][touch_end[0]].updateColor(PURPLE);
	
	//get next node
	int* next_node = grid[touch_end[1]][touch_end[0]].getlastNode();
	
	bool make_path = true;
	while(make_path)
	{
		grid[next_node[1]][next_node[0]].updateColor(PURPLE);
		//update next node
		
		next_node = grid[next_node[1]][next_node[0]].getlastNode();
		
		if(next_node[0] == start[0] and next_node[1] == start[1])
		{
			make_path = false;
		}
		
		//draw
		draw(grid, width);
		gscore -= 1;
	}
	
	//keep path up
	bool run = true;
	SDL_Event e;
	
	while(run)
	{
		while(SDL_PollEvent(&e))
		{
			switch(e.type)
			{
				case SDL_MOUSEBUTTONDOWN:
				run = false;
				break;
			}
		}
	}
}

void algorithm(std::vector<std::vector<Node>> grid, int width, int max_nodes)
{
	//set up g score
	int gscore = 0;
	
	//find start and end
	int start[2] = {-1, -1};
	int end[2] = {-1, -1};
	
	for(int i = 0; i < grid.size(); i++)
	{
		for(int c = 0; c < grid.size(); c++)
		{
			if(grid[i][c].getCol() == BLUE)
			{
				start[0] = grid[i][c].getX();
				start[1] = grid[i][c].getY();
			}
			if(grid[i][c].getCol() == ORANGE)
			{
				end[0] = grid[i][c].getX();
				end[1] = grid[i][c].getY();
			}
		}
	}
	
	//set up open_set
	std::vector<std::vector<int>> open_set;//x, y, fscore
	
	//set up priority list
	std::vector<std::vector<int>> prio_set;//place in open_set, fscore
	
	std::vector<int> vecto;
	vecto.push_back(0);
	vecto.push_back(getDistance(end[0], end[1], start[0], start[1]));
	prio_set.push_back(vecto);
	for(int z = 1; z < max_nodes; z++)
	{
		std::vector<int> vect;
		vect.push_back(-1);
		vect.push_back(-1);
		prio_set.push_back(vect);
	}
	
	//set up start node
	std::vector<int> start_set;
	start_set.push_back(start[0]);
	start_set.push_back(start[1]);
	start_set.push_back(gscore+getDistance(end[0], end[1], start[0], start[1]));
	grid[start[0]][start[1]].updateH(getDistance(end[0], end[1], start[0], start[1])*hwheight);
	grid[start[0]][start[1]].updateG(0);
	grid[start[0]][start[1]].updateF();
	open_set.push_back(start_set);
	
	//loop to find path
	while(open_set.size() > 0)
	{
		//set up new list
		std::vector<std::vector<int>> new_set;
		//loop thrue open_set
		for(int i = 0; i < open_set.size(); i++)
		{
			
			//check if node is in priority list
			bool check = false;
			bool can_add = true;
			for(int z = 0; z < max_nodes; z++)
			{
				if(prio_set[z][0] == i)
				{
					check = true;
					grid[open_set[i][1]][open_set[i][0]].updateB();
				} else if(!grid[open_set[i][1]][open_set[i][0]].getU() and prio_set[z][0] != -1 and can_add)
				{
					new_set.push_back(open_set[i]);
					can_add = false;
				}
			}
			
			if(check)
			{
				//change to red
				if(grid[open_set[i][1]][open_set[i][0]].getCol() != BLUE and grid[open_set[i][1]][open_set[i][0]].getCol() != ORANGE)
				{
					grid[open_set[i][1]][open_set[i][0]].updateColor(RED);
				}
				
				//get neighbors, set to green, put in new list, and set scores 
				grid[open_set[i][1]][open_set[i][0]].updateNeighbors(grid);
				std::vector<std::string> nei = grid[open_set[i][1]][open_set[i][0]].getNeighbors();
				
				for(int c = 0; c < nei.size(); c++)
				{
					if(nei[c] == WHITE)
					{
						if(c == 0)//up
						{
							grid[open_set[i][1] -1][open_set[i][0]].updateH(getDistance(end[0], end[1], open_set[i][0], open_set[i][1]-1)*hwheight);
							grid[open_set[i][1] -1][open_set[i][0]].updateG(gscore*gwheight);
							grid[open_set[i][1] -1][open_set[i][0]].updateF();
							
							std::vector<int> lis;
							
							
							lis.push_back(grid[open_set[i][1] -1][open_set[i][0]].getX());
							lis.push_back(grid[open_set[i][1] -1][open_set[i][0]].getY());
							lis.push_back(grid[open_set[i][1] -1][open_set[i][0]].getfscore());
							
							new_set.push_back(lis);
							
							grid[open_set[i][1] -1][open_set[i][0]].updateLN(open_set[i][0], open_set[i][1]);
							grid[open_set[i][1] -1][open_set[i][0]].updateColor(GREEN);
						
						} else if(c == 1)//right
						{
							grid[open_set[i][1]][open_set[i][0] +1].updateH(getDistance(end[0], end[1], open_set[i][0]+1, open_set[i][1])*hwheight);
							grid[open_set[i][1]][open_set[i][0] +1].updateG(gscore*gwheight);
							grid[open_set[i][1]][open_set[i][0] +1].updateF();
							
							std::vector<int> lis;
							
							
							lis.push_back(grid[open_set[i][1]][open_set[i][0] +1].getX());
							lis.push_back(grid[open_set[i][1]][open_set[i][0] +1].getY());
							lis.push_back(grid[open_set[i][1]][open_set[i][0] +1].getfscore());
							
							new_set.push_back(lis);
							
							grid[open_set[i][1]][open_set[i][0] +1].updateLN(open_set[i][0], open_set[i][1]);
							grid[open_set[i][1]][open_set[i][0] +1].updateColor(GREEN);
							
						} else if(c == 2)//down
						{
							grid[open_set[i][1] +1][open_set[i][0]].updateH(getDistance(end[0], end[1], open_set[i][0], open_set[i][1]+1)*hwheight);
							grid[open_set[i][1] +1][open_set[i][0]].updateG(gscore*gwheight);
							grid[open_set[i][1] +1][open_set[i][0]].updateF();
							
							std::vector<int> lis;
							
							
							lis.push_back(grid[open_set[i][1] +1][open_set[i][0]].getX());
							lis.push_back(grid[open_set[i][1] +1][open_set[i][0]].getY());
							lis.push_back(grid[open_set[i][1] +1][open_set[i][0]].getfscore());
							
							new_set.push_back(lis);
							
							grid[open_set[i][1] +1][open_set[i][0]].updateLN(open_set[i][0], open_set[i][1]);
							grid[open_set[i][1] +1][open_set[i][0]].updateColor(GREEN);
						} else if(c == 3)//left
						{
							grid[open_set[i][1]][open_set[i][0] -1].updateH(getDistance(end[0], end[1], open_set[i][0]-1, open_set[i][1])*hwheight);
							grid[open_set[i][1]][open_set[i][0] -1].updateG(gscore*gwheight);
							grid[open_set[i][1]][open_set[i][0] -1].updateF();
							
							std::vector<int> lis;
							
							
							lis.push_back(grid[open_set[i][1]][open_set[i][0] -1].getX());
							lis.push_back(grid[open_set[i][1]][open_set[i][0] -1].getY());
							lis.push_back(grid[open_set[i][1]][open_set[i][0] -1].getfscore());
							
							new_set.push_back(lis);
							
							grid[open_set[i][1]][open_set[i][0] -1].updateLN(open_set[i][0], open_set[i][1]);
							grid[open_set[i][1]][open_set[i][0] -1].updateColor(GREEN);
						}
					}
					if(nei[c] == ORANGE)
					{
						retracePath(grid, open_set, i, width, gscore, start);
						return;
					}
				}
			}
		}
		
		gscore += 1;
		open_set = new_set;
				
		draw(grid, width);
		
		for(int z = 0; z < max_nodes; z++)
		{
			prio_set[z][1] = -1;
			prio_set[z][0] = -1;
		}
		
		for(int z = 0; z < max_nodes; z++)
		{
			for(int p = 0; p < open_set.size(); p++)
			{
				bool already_in_list = false;
				for(int zp = 0; zp < max_nodes; zp++)
				{
					if(prio_set[zp][0] == p)
					{
						already_in_list = true;
					}
				}

				if((prio_set[z][1] > open_set[p][2] or prio_set[z][1] == -1) and !already_in_list)
				{
					prio_set[z][1] = open_set[p][2];
					prio_set[z][0] = p;
				}
			}
		}
	}
	
	//keep path up
	bool run = true;
	SDL_Event e;
	
	while(run)
	{
		while(SDL_PollEvent(&e))
		{
			switch(e.type)
			{
				case SDL_MOUSEBUTTONDOWN:
				run = false;
				break;
			}
		}
	}
}

std::vector<std::vector<Node>> makeGrid(int mrow, int width)
{
	std::vector<std::vector<Node>> grid;
	for(int i = 0; i < mrow; i++)
	{
		std::vector<Node> nodes;
		for(int c = 0; c < mrow; c++)
		{
			nodes.push_back(Node(c, i, width, mrow, WHITE));
		}
		grid.push_back(nodes);
	}
	return grid;
}

int main()
{
	//set up number of rows and width of squares
	int mrow;//max # of rowes
	int width = 1000;//width of window
	std::cout << "Amount of Rows: ";
	std::cin >> mrow;
	
	int width_nodes = width/mrow; //width of square
	
	width = width_nodes*mrow;
	
	//set up max number of nodes in priority list
	int max_nodes;
	std::cout<<"Max number of nodes in priority list (min of 5): ";
	std::cin>>max_nodes;
	if(max_nodes<5)
	{
		max_nodes = 5;
	}
	
	//set weight of gscore and hscore
	std::cout<<"Use integers:"<<std::endl;
	std::cout<<"Hscore weight: ";
	std::cin>>hwheight;
	std::cout<<"Gscore weight: ";
	std::cin>>gwheight;
	
	//init sdl
	if(SDL_Init(SDL_INIT_VIDEO) == -1){
		std::cout << "\nSDL_Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	//set up sdl
	win = SDL_CreateWindow("A* Path Finder", 100, 100, width, width, SDL_WINDOW_SHOWN);
	rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	
	std::vector<std::vector<Node>> grid = makeGrid(mrow, width_nodes);
	
	bool run = true;
	bool start_down = false;
	bool end_down = false;
	bool down_left = false;
	SDL_Event e;
	
	while(run)
	{
		//get events from sdl
		while(SDL_PollEvent(&e))
		{
			switch(e.type)
			{
				case SDL_QUIT:
				run = false;
				break;
				
				case SDL_MOUSEBUTTONDOWN:
				if(e.button.button == SDL_BUTTON_LEFT)
				{
					down_left = true;
				}
				if(e.button.button == SDL_BUTTON_RIGHT)
				{
					//del color
					int nrx = e.motion.x / width_nodes;
					int nry = e.motion.y / width_nodes;
					if(grid[nry][nrx].getCol() == BLUE)
					{
						start_down = false;
					}
					if(grid[nry][nrx].getCol() == ORANGE)
					{
						end_down = false;
					}
					grid[nry][nrx].updateColor(WHITE);
				}
				break;
				
				case SDL_MOUSEBUTTONUP:
				if(e.button.button == SDL_BUTTON_LEFT)
				{
					down_left = false;
				}
				break;
				
				case SDL_KEYDOWN:
				switch(e.key.keysym.sym)
				{
					case SDLK_c:
					//clear
					grid = makeGrid(mrow, width_nodes);
					start_down = false;
					end_down = false;
					break;
					
					case SDLK_RETURN:
					//start algorithm
					if(start_down and end_down)
					{
						algorithm(grid, width_nodes, max_nodes);
					}
					break;
				}
				break;
			}
		}
		
		if(down_left)
		{
			//get square
			int mx, my;
			SDL_GetMouseState(&mx, &my);
			int nrx = mx / width_nodes;
			int nry = my / width_nodes;
			
			//color squares
			if(!start_down)
			{
				//check start
				grid[nry][nrx].updateColor(BLUE);
				start_down = true;
			}else if(!end_down)
			{
				//check end
				if(grid[nry][nrx].getCol() != BLUE)
				{
					grid[nry][nrx].updateColor(ORANGE);
					end_down = true;
				}
			} else
			{	
				//check if square is end or start
				if(grid[nry][nrx].getCol() != ORANGE and grid[nry][nrx].getCol() != BLUE)
				{
					grid[nry][nrx].updateColor(BLACK);
				}
			}
		}
		//draw grid
		draw(grid, width_nodes);
	}
	
	//clean sdl
	SDL_DestroyWindow(win);
	SDL_Quit();
	
	return 0;
}
