//Note: This is made for a 1600x1200 settings might need to be be changed to fit screen

#include "node.h"

#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <iostream>

const std::string WHITE = "white";
const std::string BLUE = "blue";
const std::string GREEN = "green";
const std::string RED = "red";
const std::string PURPLE = "purple";
const std::string BLACK = "black";

SDL_Window* win;
SDL_Renderer* rend;

std::vector<std::vector<Node>> makeGrid()
{
	//make grid
	std::vector<std::vector<Node>> grid;
	for(int i = 0; i < 60; i++)
	{
		std::vector<Node> vect;
		for(int c = 0; c < 80; c++)
		{
			vect.push_back(Node(c, i, WHITE));
		}
		grid.push_back(vect);
	}
	return grid;
}

void setup()
{
	//init sdl
	SDL_Init(SDL_INIT_VIDEO);
	win = SDL_CreateWindow("Double A*", 100, 100, 1600, 1200, SDL_WINDOW_FULLSCREEN|SDL_WINDOW_SHOWN);
	rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
}

void cleanup()
{
	//clean sdl
	rend = 0;
	SDL_DestroyWindow(win);
	SDL_Quit();
}

void draw(std::vector<std::vector<Node>> grid)
{
	//draw grid
	for(int i = 0; i < grid.size(); i++)
	{
		for(int c = 0; c < grid[i].size(); c++)
		{
			if(grid[i][c].getC() == BLUE)
			{
				SDL_SetRenderDrawColor(rend, 0, 0, 255, 255);
			}
			if(grid[i][c].getC() == PURPLE)
			{
				SDL_SetRenderDrawColor(rend, 128, 0, 128, 255);
			}
			if(grid[i][c].getC() == WHITE)
			{
				SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
			}
			if(grid[i][c].getC() == BLACK)
			{
				SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
			}
			if(grid[i][c].getC() == GREEN)
			{
				SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);
			}
			if(grid[i][c].getC() == RED)
			{
				SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
			}
			SDL_Rect r = {grid[i][c].getX()*20, grid[i][c].getY()*20, 20, 20};
			SDL_RenderFillRect(rend, &r);
			SDL_SetRenderDrawColor(rend, 128, 128, 128, 255);
			SDL_RenderDrawRect(rend, &r);
		}
	}
	SDL_RenderPresent(rend);
}

int distance()
{
	//distance to start/end node
	return 0;
}

void retrace(std::vector<std::vector<Node>> grid, int hit1[2], int hit2[2])
{
	//retrace path
	grid[hit1[1]][hit1[0]].updateC(PURPLE);
	grid[hit2[1]][hit2[0]].updateC(PURPLE);

	int* next1 = grid[hit1[1]][hit1[0]].getLN();
	int* next2 = grid[hit2[1]][hit2[0]].getLN();
	
	bool run = true;
	while(run)
	{
		if(grid[next1[1]][next1[0]].getC() != BLUE)
		{
			grid[next1[1]][next1[0]].updateC(PURPLE);
		}
		if(grid[next2[1]][next2[0]].getC() != BLUE)
		{
			grid[next2[1]][next2[0]].updateC(PURPLE);
		}
		
		next1 = grid[next1[1]][next1[0]].getLN();
		next2 = grid[next2[1]][next2[0]].getLN();
		
		draw(grid);
		
		if(grid[next1[1]][next1[0]].getC() == BLUE)
		{
			run = false;
		}
	}
	//keep path up
	run = true;
	while(run)
	{
		SDL_Event e;
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

void algorithm(std::vector<std::vector<Node>> grid)
{
	//find node animation
	
	//set up open list
	std::vector<std::vector<int>> open;//x, y
	
	//set up start nodes
	bool first = true;
	for(int i = 0; i < grid.size(); i++)
	{
		for(int z = 0; z < grid[i].size(); z++)
		{
			if(grid[i][z].getC() == BLUE)
			{
				std::vector<int> vect;
				vect.push_back(z);
				vect.push_back(i);
				open.push_back(vect);
				if(first)
				{
					grid[i][z].updateW(0);
					first = false;
				} else {
					grid[i][z].updateW(1);
				}
			}
		}
	}
	while(open.size() > 0)
	{
		std::vector<std::vector<int>> new_set;
		for(int i = 0; i < open.size(); i++)
		{
			
			//change to red
			if(grid[open[i][1]][open[i][0]].getC() != BLUE)
			{
				grid[open[i][1]][open[i][0]].updateC(RED);
			}
			
			grid[open[i][1]][open[i][0]].updateN(grid);
			std::vector<std::string> nei = grid[open[i][1]][open[i][0]].getN();
			
			for(int c = 0; c < nei.size(); c++)
			{	
				if(nei[c] == WHITE)
				{
						if(c == 0)//up
						{
							grid[open[i][1]-1][open[i][0]].updateW(grid[open[i][1]][open[i][0]].getW());
							grid[open[i][1]-1][open[i][0]].updateC(GREEN);
							grid[open[i][1]-1][open[i][0]].updateLN(open[i][0], open[i][1]);
							
							std::vector<int> lis;
							lis.push_back(open[i][0]);
							lis.push_back(open[i][1]-1);
							new_set.push_back(lis);
						} else if(c == 1)
						{
							grid[open[i][1]][open[i][0]+1].updateW(grid[open[i][1]][open[i][0]].getW());
							grid[open[i][1]][open[i][0]+1].updateC(GREEN);
							grid[open[i][1]][open[i][0]+1].updateLN(open[i][0], open[i][1]);
							
							std::vector<int> lis;
							lis.push_back(open[i][0]+1);
							lis.push_back(open[i][1]);
							new_set.push_back(lis);
						} else if(c == 2)
						{
							grid[open[i][1]+1][open[i][0]].updateW(grid[open[i][1]][open[i][0]].getW());
							grid[open[i][1]+1][open[i][0]].updateC(GREEN);
							grid[open[i][1]+1][open[i][0]].updateLN(open[i][0], open[i][1]);
							
							std::vector<int> lis;
							lis.push_back(open[i][0]);
							lis.push_back(open[i][1]+1);
							new_set.push_back(lis);
						} else if(c == 3)
						{
							grid[open[i][1]][open[i][0]-1].updateW(grid[open[i][1]][open[i][0]].getW());
							grid[open[i][1]][open[i][0]-1].updateC(GREEN);
							grid[open[i][1]][open[i][0]-1].updateLN(open[i][0], open[i][1]);
							
							std::vector<int> lis;
							lis.push_back(open[i][0]-1);
							lis.push_back(open[i][1]);
							new_set.push_back(lis);
						}
						
				}
				if(nei[c] == RED) //or nei[c] == GREEN)
				{
					if(c == 0 and grid[open[i][1]][open[i][0]].getW()!= grid[open[i][1]-1][open[i][0]].getW())
					{
						int hit[2] = {open[i][0],open[i][1]};
						int hit1[2] = {open[i][0],open[i][1]-1};
						retrace(grid, hit, hit1);
						return;
					}else if(c == 1 and grid[open[i][1]][open[i][0]].getW()!= grid[open[i][1]][open[i][0]+1].getW())
					{
						int hit[2] = {open[i][0],open[i][1]};
						int hit1[2] = {open[i][0]+1,open[i][1]};
						retrace(grid, hit, hit1);
						return;
					}else if(c == 2 and grid[open[i][1]][open[i][0]].getW()!= grid[open[i][1]+1][open[i][0]].getW())
					{
						int hit[2] = {open[i][0],open[i][1]};
						int hit1[2] = {open[i][0],open[i][1]+1};
						retrace(grid, hit, hit1);
						return;
					}else if(c == 3 and grid[open[i][1]][open[i][0]].getW()!= grid[open[i][1]][open[i][0]-1].getW())
					{
						int hit[2] = {open[i][0],open[i][1]};
						int hit1[2] = {open[i][0]-1,open[i][1]};
						retrace(grid, hit, hit1);
						return;
					}
				}
			}
		}
		
		open = new_set;
		
		draw(grid);
	}
	
	//keep path up
	bool run = true;
	while(run)
	{
		SDL_Event e;
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

int main()
{
	setup();
	
	//make grid
	std::vector<std::vector<Node>> grid = makeGrid();
	
	//end/start down
	
	bool start_down[2] = {false, false};
	
	//make maze
	bool down = false;//left mouse down
	
	bool run = true;
	while(run)
	{
		SDL_Event e;
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
					down = true;
				}
				if(e.button.button == SDL_BUTTON_RIGHT)
				{
					int nrx = e.motion.x / 20;
					int nry = e.motion.y / 20;
					if(grid[nry][nrx].getC() == BLUE)
					{
						if(start_down[0])
						{
							start_down[0] = false;
							grid[nry][nrx].updateC(WHITE);
						} else
						{
							start_down[1] = false;
							grid[nry][nrx].updateC(WHITE);
						}
					} else
					{
						grid[nry][nrx].updateC(WHITE);
					}
				}
				break;
				
				case SDL_MOUSEBUTTONUP:
				if(e.button.button == SDL_BUTTON_LEFT)
				{
					down = false;
				}
				break;
				
				case SDL_KEYDOWN:
				switch(e.key.keysym.sym)
				{
					case SDLK_c:
					//clear
					grid = makeGrid();
					start_down[0] = false;
					start_down[1] = false;
					break;
					
					case SDLK_RETURN:
					//start algorithm
					if(start_down[0] and start_down[1])
					{
						algorithm(grid);
					}
					break;
				}
				break;
			}
		}
		
		if(down)
		{
			int nrx = e.motion.x / 20;
			int nry = e.motion.y / 20;
			
			//color squares
			if(!start_down[0])
			{
				//check start
				grid[nry][nrx].updateC(BLUE);
				start_down[0] = true;
			}else if(!start_down[1])
			{
				//check end
				if(grid[nry][nrx].getC() != BLUE)
				{
					grid[nry][nrx].updateC(BLUE);
					start_down[1] = true;
				}
			} else
			{	
				//check if square is end or start
				if(grid[nry][nrx].getC() != BLUE)
				{
					grid[nry][nrx].updateC(BLACK);
				}
			}
		}
		draw(grid);
	}
	cleanup();
	return 1;
}
