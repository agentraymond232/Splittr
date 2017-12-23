/* tesselate.cpp - Contains functions that tesselates the glass
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
#include <iostream>
#include <vector>
#include <GLFW/glfw3.h>
#include "Point.h"
#include "tesselate.h"
#include <GL/glu.h>

using namespace std;
static GLuint objektList;

void resetTessList() {
	glDeleteLists(objektList, 1);
}

//The callback functions is called during tessellation at appropriate times
void CALLBACK beginCallback(GLenum which) {	
	glBegin(which);
}

void CALLBACK errorCallback(GLenum errorCode) {
	auto estring = gluErrorString(errorCode);
	cerr << "Tessellation Error" <<estring << endl;
	exit(0);
}

void CALLBACK endCallback() {
	glEnd();
}

void CALLBACK vertexCallback(GLvoid *vertex) {
	glVertex3dv(static_cast<GLdouble*>(vertex));
}

void CALLBACK combineCallback(GLdouble coords[3],
	GLdouble *vertex_data[4],
	GLfloat weight[4],
	GLdouble **dataOut ) {

	auto vertex = static_cast<GLdouble *>(malloc(6 * sizeof(GLdouble)));
	if (vertex == nullptr)
		return;
	vertex[0] = coords[0];
	vertex[1] = coords[1];
	vertex[2] = coords[2];
	*dataOut = vertex;
}

void tesselate(vector<vector<Point*>* >* tessVector) {

	if(!glIsList(objektList)) {
		objektList = glGenLists(1);
		GLUtesselator *tobj;
		tobj = gluNewTess();

		//Defines the callback functions
		gluTessCallback(tobj, GLU_TESS_VERTEX,
		reinterpret_cast<void (CALLBACK*)()>(vertexCallback));
		gluTessCallback(tobj, GLU_TESS_BEGIN,
		reinterpret_cast<void (CALLBACK*)()>(beginCallback));
		gluTessCallback(tobj, GLU_TESS_END,
		static_cast<void (CALLBACK*)()>(endCallback));
		gluTessCallback(tobj, GLU_TESS_ERROR,
		reinterpret_cast<void (CALLBACK*)()>(errorCallback));
		gluTessCallback(tobj, GLU_TESS_COMBINE,
		reinterpret_cast<void (CALLBACK*)()>(combineCallback));

		auto vertexVector = new vector<GLdouble**>;
		
		// Stores the window corners in the GLdouble array since
		// that is required by gluTexVertex
		const auto vertexList = new GLdouble*[4];
		vertexList[0] = new GLdouble[3];
		vertexList[0][0] = -3.5;
		vertexList[0][1] = 8.5;
		vertexList[0][2] = 0.0;
		vertexList[1] = new GLdouble[3];
		vertexList[1][0] = 3.5;
		vertexList[1][1] = 8.5;
		vertexList[1][2] = 0.0;
		vertexList[2] = new GLdouble[3];
		vertexList[2][0] = 3.5;
		vertexList[2][1] = 1.5;
		vertexList[2][2] = 0.0;
		vertexList[3] = new GLdouble[3];
		vertexList[3][0] = -3.5;
		vertexList[3][1] = 1.5;
		vertexList[3][2] = 0.0;
		vertexVector->push_back(vertexList);
	
		//Stores all the holes in GLdouble arrays
		for(auto i = 0; i< tessVector->size(); i++) {
			//Creates a double array with the same number of rows as
			//the size of the tessVector
			const auto vList = new GLdouble*[tessVector->at(i)->size()];
			for(auto j = 0; j<tessVector->at(i)->size() ; j++) {
				vList[j] = new GLdouble[3];
				vList[j][0] = tessVector->at(i)->at(j)->getX();
				vList[j][1] = tessVector->at(i)->at(j)->getY();
				vList[j][2] = 0.0;
			}
			vertexVector->push_back(vertexList);
		}
		glNewList(objektList, GL_COMPILE_AND_EXECUTE);
		gluTessBeginPolygon(tobj, nullptr);

		//Using positive rule so since the holes is sorted in counter
		//clockwise order. The glass is defined clockwise
		gluTessProperty(tobj, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_POSITIVE);

		//Draws the window
		gluTessBeginContour(tobj);
		for (auto i = 3; i >= 0; i--) {
			gluTessVertex(tobj,vertexVector->at(0)[i], vertexVector->at(0)[i]);
		}
		gluTessEndContour(tobj);
		
		//Draws the contour of the holes, starts from behind since the
		//holes is defined counter clockwise
		for(auto i = 1 ; i < vertexVector->size(); i++) {
			gluTessBeginContour(tobj);
			for (int j = tessVector->at(i-1)->size()-1 ; j >= 0 ; j--) {
				gluTessVertex(tobj,vertexVector->at(i)[j],
				vertexVector->at(i)[j]);
			}
			gluTessEndContour(tobj);
		}
		gluTessEndPolygon(tobj);
		glEndList();
		gluDeleteTess(tobj);
	}
	else {
	glCallList(objektList);
	}
}