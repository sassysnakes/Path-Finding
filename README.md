# Path-Finding

**Use**:

Left click to set down walls, start and end. The first two clicks represent the start and the end nodes, the rest are walls.

Right click to take down walls, start and end. if take off the start and the end they will be your next click.

Press c to clear the whole board and press return to see algorihm. Once algorithm is finished you have to click to go back to editor mode.


**Read**:

Blue -> start nodes

Orange -> end node

Red -> closed node(computer finished checking node)

Green -> open node(computer is checking node)

White -> blank node

Purple -> path

Black -> wall node


**Scores**:

Hscore -> distance from a node to end

Gscore -> distance from a node to start

Fscore -> total score


**Compile and run**:

Download SDL2 and g++

Compile with g++ *.cpp -o main -lSDL2

Run file by double clicking or on linux using: ./main in folder directory
