/* room.cpp - File to create all primitives needed in graphics.cpp. Includes
* room.h, Point.h and tesselate.h.
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
#include "room.h"
#include "Point.h"
#include "tesselate.h"

#ifndef SPLITTER_TEXTUREID
#define SPLITTER_TEXTUREID
//For textures, sets number of textures
GLuint textureID[5];
#endif

using namespace std;
namespace splitter {

	/**
	It is a no op.
	*/
	void initTextures() {
	/*
		// Load and set up textures - use GLFW to make this easier
		glEnable(GL_TEXTURE_2D); // Enable texturing
		glGenTextures(5, textureID); // Generate 5 unique texture IDs to use
		glBindTexture(GL_TEXTURE_2D, textureID[0]); // Activate first texture
		glfwLoadTexture2D(“./textures/grycon.tga”, GLFW_BUILD_MIPMAPS_BIT);
		glBindTexture(GL_TEXTURE_2D, textureID[1]); // Activate second texture
		glfwLoadTexture2D(“./textures/yellobrk2.tga”, GLFW_BUILD_MIPMAPS_BIT);
		glBindTexture(GL_TEXTURE_2D, textureID[2]); // Activate third texture
		glfwLoadTexture2D(“./textures/yellobrk3.tga”, GLFW_BUILD_MIPMAPS_BIT);
		glBindTexture(GL_TEXTURE_2D, textureID[3]); // Activate fourth texture
		glfwLoadTexture2D(“./textures/yellobrk4.tga”, GLFW_BUILD_MIPMAPS_BIT);
		glBindTexture(GL_TEXTURE_2D, textureID[4]); // Activate fifth texture
		glfwLoadTexture2D(“./textures/stucco.tga”, GLFW_BUILD_MIPMAPS_BIT);
		*/
	}

	GLuint initRoomDisplayList() {
		auto index = glGenLists(1);
		glNewList(1, GL_COMPILE);
		drawCeiling();
		drawFloor();
		drawWalls();
		drawWindowFrame();
		glEndList();
		return index;
	}

	void drawCeiling() {
		//Draw the ceiling and set new texture
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureID[4]);
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(0.0f, 10.0f, 0.0f);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		drawTexturedQuad(15.0f, 15.0f);
		glPopMatrix();
	}
	void drawFloor() {
		//Draw the floor
		glPushMatrix();
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		drawTexturedQuad(15.0f, 15.0f);
		glPopMatrix();
	}

	void drawWalls() {
		//Draw walls and set new texture
		glBindTexture(GL_TEXTURE_2D, textureID[0]);
		glPushMatrix();
		glTranslatef(15.0f, 5.0f, 0.0f);
		glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
		drawTexturedQuad(15.0f, 5.0f);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-15.0f, 5.0f, 0.0f);
		glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		drawTexturedQuad(15.0f, 5.0f);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(0.0f, 5.0f, -15.0f);
		drawTexturedQuad(15.0f, 5.0f);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(0.0f, 5.0f, 15.0f);
		glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
		drawTexturedQuad(15.0f, 5.0f);
		glPopMatrix();
	}

	void drawWindowFrame() {
		//Draw the window frame
		glBindTexture(GL_TEXTURE_2D, textureID[1]);
		glPushMatrix();
		glTranslatef(6.75f, 3.5f, 0.0f);
		drawTexturedBlock(3.25f, 3.5f, 0.25f);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-6.75f, 3.5f, 0.0f);
		drawTexturedBlock(3.25f, 3.5f, 0.25f);
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, textureID[2]);
		glPushMatrix();
		glTranslatef(9.25f, 8.5f, 0.0f);
		drawTexturedBlock(5.75f, 1.5f, 0.25f);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-9.25f, 8.5f, 0.0f);
		drawTexturedBlock(5.75f, 1.5f, 0.25f);
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, textureID[3]);
		glPushMatrix();
		glTranslatef(0.0f, 9.25f, 0.0f);
		drawTexturedBlock(3.5f, 0.75f, 0.25f);
		//63
		glPopMatrix();
		glPushMatrix();
		glTranslatef(0.0f, 0.75f, 0.0f);
		drawTexturedBlock(3.5f, 0.75f, 0.25f);
		glPopMatrix();
	}

	void drawWindow(vector<vector<Point*>* >* holesVector) {
		//Draw the glass window with blending and disable textures
		glEnable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glPushMatrix();
		glColor4f(1.0f, 1.0f, 1.0f, 0.5f); //need color with alpha channel
		if (holesVector->size() != 0) {
			tesselate(holesVector);
		} else {
			glTranslatef(0.0f, 5.0f, 0.0f);
			drawTexturedQuad(3.5f, 3.5f);
		}
		glPopMatrix();
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	}

	void drawTexturedBlock(float x, float y, float z) {
		glBegin(GL_QUADS);
		// Top face
		glNormal3f(0,1,0);
		glTexCoord2f(0.0f,1.0f); glVertex3f(-x,y,z);
		glTexCoord2f(1.0f,1.0f); glVertex3f(x,y,z);
		glTexCoord2f(1.0f,0.0f); glVertex3f(x,y,-z);
		glTexCoord2f(0.0f,0.0f); glVertex3f(-x,y,-z);
		// Back face
		glNormal3f(0,0,1);
		glTexCoord2f(0.0f,1.0f); glVertex3f(-x,y,z);
		glTexCoord2f(0.0f,0.0f); glVertex3f(-x,-y,z);
		glTexCoord2f(1.0f,0.0f); glVertex3f(x,-y,z);
		glTexCoord2f(1.0f,1.0f); glVertex3f(x,y,z);
		// Front face
		glNormal3f(0,0,-1);
		glTexCoord2f(0.0f,1.0f); glVertex3f(-x,y,-z);
		glTexCoord2f(1.0f,1.0f); glVertex3f(x,y,-z);
		glTexCoord2f(1.0f,0.0f); glVertex3f(x,-y,-z);
		glTexCoord2f(0.0f,0.0f); glVertex3f(-x,-y,-z);
		// Left face
		glNormal3f(1,0,0);
		glTexCoord2f(1.0f,0.0f); glVertex3f(x,y,-z);
		glTexCoord2f(1.0f,1.0f); glVertex3f(x,y,z);
		glTexCoord2f(0.0f,1.0f); glVertex3f(x,-y,z);
		glTexCoord2f(0.0f,0.0f); glVertex3f(x,-y,-z);
		// Right face
		glNormal3f(-1,0,0);
		glTexCoord2f(1.0f,1.0f); glVertex3f(-x,y,z);
		glTexCoord2f(1.0f,0.0f); glVertex3f(-x,y,-z);
		glTexCoord2f(0.0f,0.0f); glVertex3f(-x,-y,-z);
		glTexCoord2f(0.0f,1.0f); glVertex3f(-x,-y,z);
		// Bottom face
		glNormal3f(0,-1,0);
		glTexCoord2f(0.0f,1.0f); glVertex3f(-x,-y,z);
		glTexCoord2f(0.0f,0.0f); glVertex3f(-x,-y,-z);
		glTexCoord2f(1.0f,0.0f); glVertex3f(x,-y,-z);
		glTexCoord2f(1.0f,1.0f); glVertex3f(x,-y,z);
		glEnd();
	}

	void drawTexturedQuad(float x, float y) {
		//64
		glBegin(GL_QUADS);
		// Front face
		glNormal3f(0,0,1);
		glTexCoord2f(0.0f,1.0f); glVertex3f(-x,y,0.0f);
		glTexCoord2f(0.0f,0.0f); glVertex3f(-x,-y,0.0f);
		glTexCoord2f(1.0f,0.0f); glVertex3f(x,-y,0.0f);
		glTexCoord2f(1.0f,1.0f); glVertex3f(x,y,0.0f);
		glEnd();
	}
}

