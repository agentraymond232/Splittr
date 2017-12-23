/* tesselate.h - Contains functions that tesselates the glass
* according to the holes.
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
#include <vector>
#include <GLFW/glfw3.h>
#include "Point.h"

using namespace std;
using namespace splitter;

/* Resets the objektlist containing the tesselated window
*/
void resetTessList();

/* Tessellates the window with holes into renderable polygons.
* A display list is used since the holes only needs to be tesselated once.
*
* Input: (tessVector) A pointer to a vector with holes, each hole is
* pointer to another vector containing the points that is the hole.
* The points is sorted counter clockwise.
* Output: void
*/
void tesselate(vector<vector<Point*>* >* tessVector);

#if !defined(CALLBACK) && defined(_WIN32)
#define CALLBACK __stdcall
#define GLFW_CALLBACK_DEFINED
#endif

/* Callback functions that is run at appropiate times during the
* tessellation
*/
void CALLBACK beginCallback(GLenum which);
void CALLBACK errorCallback(GLenum errorCode);
void CALLBACK endCallback(void);
void CALLBACK vertexCallback(GLvoid *vertex);
void CALLBACK combineCallback(GLdouble coords[3],
	GLdouble *vertex_data[4],
	GLfloat weight[4], GLdouble **dataOut);