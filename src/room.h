/* room.h - File to create all primitives needed in graphics.h. Needs to include
* Point.h to run. Is included in graphics.cpp.
*
* Authors:
* Jesper Carlsson,
* Daniel Enetoft,
* Anders Fjeldstad,
* Kristofer Gärdeborg.
*
* Copyright 2004,
* Jesper Carlsson,
* Daniel Enetoft,
* Anders Fjeldstad,
* Kristofer Gärdeborg.
*
* This file is part of Splitter.
*
* Splitter is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* Splitter is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Splitter; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/
#include <GLFW/glfw3.h>
#include "Point.h"

using namespace std;
namespace splitter {

/* Initialize all textures.
*
* Input: void
* Output: void
*/
void initTextures();
/* Draw the ceiling with a quad object and textures.
*
* Input: void
* Output: void
*/
void drawCeiling();
/* Draw the floor with a quad object and textures.
*
* Input: void
* Output: void
*/
void drawFloor();
/* Draw the surounding walls with a quad object and textures.
*
* Input: void
* Output: void
*/
void drawWalls();
/* Draw the middle wall with a hole for the glass window.
* Applying textures for the block object.
*
* Input: void
* Output: void
*/
void drawWindowFrame();
/* Create a display list containing drawCeiling, drawFloor, drawWalls and
60
* drawWindowFrame.
*
* Input: void
* Output: Index of the display list.
*/
GLuint initRoomDisplayList();
/* Draw the window with a quad object and textures.
*
* Input: (holesVector) a vector containing the point objects defining the
* different holes created by the crack pattern.
* Output: void
*/
void drawWindow(vector<vector<Point*>* >* holesVector);
/* Draw a block with size 2x by 2y by 2z, centered,
* on the local origin, with texture coordinates for each face.
*
* Input: (x) side is 2x
* (y) side is 2y
* (z) side is 2z
* Output: void
*/
void drawTexturedBlock(float x, float y, float z);
/* Draw a quad with size 2x by 2y, centered,
* on the local origin, with texture coordinates for each face.
*
* Input: (x) side is 2x
* (y) side is 2y
* Output: void
*/
void drawTexturedQuad(float x, float y);
}