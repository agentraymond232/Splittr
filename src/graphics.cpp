/* graphics.cpp - File to handle all graphical parts except for the tesselation
* of the glass polygon, done in tesselate.cpp. graphics.cpp is executed from
* splitter.cpp and needs to include graphics.h, room.h, carackCreation.h,
* crackStarter.h, Point.h, holes.h and tesselate.h to run.
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

#ifndef GLFW_INCLUDE_GLU
#define GLFW_INCLUDE_GLU

#include <windows.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "room.h"
#include "graphics.h"
#include "crackCreation.h"
#include "CrackStarter.h"
#include "Point.h"
#include "holes.h"
#include "tesselate.h"

using namespace std;
namespace splitter {

	// Point related
	CrackStarter* cs;
	vector<Point*>* allPoints;
	vector<Point*>* deadEnds;
	vector<vector<Point*>* >* holes;

	// Checks if the left resp. the right mouse button has been pressed
	bool mlClickedAlready = false;
	bool mrClickedAlready = false;

	// Define camera position
	float pos[] = {0.0, 6.0, 6.0};
	
	// Constants
	
	// Moving parameters
	const float MOUSE_SENSITIVITY = 0.1f;
	const float WALKING_SPEED = 10.0f;
	
	// Convertion degrees to radians
	const float DEG2RAD = 3.14159265 / 180.0;
	
	// Window size
	int WIDTH = 1024;
	int HEIGHT = 768;
	
	// Impact velocity
	float impactVelocity = 0.0f;
	
	// For the fps
	double t0 = 0.0;
	int frames = 0;
	char titlestring[200];
	
	// To get the frametime
	float frameTime = 0;
	double tOld = 0;
	
	// Angles for the view-vector
	float hAngle = 0.0;
	float vAngle = 0.0;
	
	// Keyboard inputs
	bool upPressed = false;
	bool downPressed = false;
	bool leftPressed = false;
	bool rightPressed = false;
	bool plusPressed = false;
	bool minusPressed = false;
	
	// Mouse coordinates
	int mouseX = 0;
	int mouseY = 0;
	int mouseOldX = 0;
	int mouseOldY = 0;
	
	// Projection
	GLfloat posX, posY, posZ;
	
	// Font data
	GLFONT *font;
	
	// Ambient light
	GLfloat globalLight[] = {0.5, 0.5, 0.5, 1.0};
	
	// Room display list index
	GLuint roomList;
	
	GLFWwindow* initGraphics(int resW, int resH) {
		WIDTH = resW;
		HEIGHT = resH;
		if (glfwInit() == GLFW_FALSE)
			return nullptr;

		//glfwOpenWindow(WIDTH, HEIGHT, 8, 8, 8, 8, 24, 0, GLFW_FULLSCREEN);
		auto window = glfwCreateWindow(640, 480, "My Title", nullptr, nullptr);
		
		if (!window) {
			glfwTerminate();
			return nullptr;
		}
		/* Make the window's context current */
		glfwMakeContextCurrent(window);

		glfwSetWindowPos(window, 200, 200);
		glfwSetInputMode (window, GLFW_CURSOR, GL_FALSE);
		glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glShadeModel(GL_SMOOTH);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalLight);
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glEnable(GL_NORMALIZE);
	
		// Initialize textures
		initTextures();
	
		// Init room display list
		roomList = initRoomDisplayList();
	
		// Used to enable blending.
		glEnable(GL_COLOR_MATERIAL);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
		// Used to prevent aliasing (in blending)
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POINT_SMOOTH);
	
		// Create a new font
		font = fontCreate(wglGetCurrentDC(), "Verdana", 15, 0, 0);
	
		// Init point vector and CrackStarter object
		resetCracks();

		return window;
	}

	void resetCracks() {

		if (allPoints != nullptr) {
			// Delete all points in the vector, and the vector.
			for (int i = 0; i < allPoints->size(); i++) {
				delete allPoints->at(i);
			}
			delete allPoints;
		}

		if (deadEnds != nullptr) {
			// Delete the vector
			delete deadEnds;
		}

		if (holes != nullptr) {
			// Delete the different vectors including the hole points.
			for (int i = 0; i < holes->size(); i++) {
				delete holes->at(i);
			}
			delete holes;
		}

		holes = new vector<vector<Point*>* >;
		
		// Create a new vector including all the points and the points marked as
		// dead ends.
		allPoints = new vector<Point*>;
		deadEnds = new vector<Point*>;
		
		if (cs != nullptr) {
			delete cs;
		}

		// Create a new object that determents the crack pattern.
		cs = new CrackStarter();
		resetTessList();
		mlClickedAlready = false;
		mrClickedAlready = false;
	}

	void initUIInput(GLFWwindow * window) {
		glfwSetWindowSizeCallback(window, reshape);
		glfwSetKeyCallback(window, keyPressed);
		glfwSetMouseButtonCallback(window, updateCrackPattern);
		glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	}

	void showFPS(GLFWwindow* window) {
		double fps;
		// Get current time
		auto t = glfwGetTime(); // Gets number of seconds since glfwInit()
		
		// If one second has passed, or if this is the very first frame
		if ((t-t0) > 1.0 || frames == 0) {
			fps = double(frames) / (t-t0);
			sprintf(titlestring, "World (%.1f FPS)", fps);
			glfwSetWindowTitle(window, titlestring);
			t0 = t;
			frames = 0;
		}
		frames++;
	}
	
	void updateFrameTime() {
		auto t = glfwGetTime();
		frameTime = static_cast<float>(t - tOld);
		tOld = t;
	}
	
	void updateMousePos(GLFWwindow * window) {
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		mouseOldX = mouseX;
		mouseX = x;
		mouseOldY = mouseY;
		mouseY = y;
	}

	void updateDirPos() {
		auto xFactor = (mouseX-mouseOldX) * MOUSE_SENSITIVITY;
		auto yFactor = (mouseY-mouseOldY) * MOUSE_SENSITIVITY;
		if (yFactor + vAngle <= 80.0 && yFactor + vAngle >= -80.0) {
			vAngle = yFactor + vAngle;
		}

		hAngle += xFactor;
		
		if (hAngle >= 360.0) {
			hAngle -= 360.0;
		}

		if (hAngle < 0.0) {
			hAngle = 360.0 + hAngle;
		}

		if (pos[0] <= 13 && pos[0] >= -13 && pos[2] >= -13 && pos[2] <= 13){
			if (pos[0] < 11 && pos[0] > -11 && pos[2] > -2 && pos[2] < 2){
				// Inside the brick wall, not allowed to walk.
				if (pos[2] >= 0) {
					pos[2] = 2;
				} else if (pos[2] < 0) {
					pos[2] = -2;
				}
			} else {
				if (upPressed) {
					pos[0] += frameTime * WALKING_SPEED * sin(DEG2RAD*hAngle);
					pos[2] -= frameTime * WALKING_SPEED * cos(DEG2RAD*hAngle);
				}
				if (downPressed) {
					pos[0] -= frameTime * WALKING_SPEED * sin(DEG2RAD*hAngle);
					pos[2] += frameTime * WALKING_SPEED * cos(DEG2RAD*hAngle);
				}
				if (leftPressed) {
					pos[0] -= frameTime * WALKING_SPEED * cos(DEG2RAD*hAngle);
					pos[2] -= frameTime * WALKING_SPEED * sin(DEG2RAD*hAngle);
				}
				if (rightPressed) {
					pos[0] += frameTime * WALKING_SPEED * cos(DEG2RAD*hAngle);
					pos[2] += frameTime * WALKING_SPEED * sin(DEG2RAD*hAngle);
				}
				if (pos[0] > 13) {
					pos[0] = 13;
				} else if (pos[0] < -13) {
					pos[0] = -13;
				}
				if (pos[2] > 13) {
					pos[2] = 13;
				} else if (pos[2] < -13) {
					pos[2] = -13;
				}
			}
		}
	}
	
	void getOGLPos(int x, int y, GLfloat& posX, GLfloat& posY, GLfloat& posZ) {
		GLint viewport[4];
		GLdouble modelview[16];
		GLdouble projection[16];
		GLfloat winX, winY, winZ;
		GLdouble tempX, tempY, tempZ;
		
		glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
		glGetDoublev( GL_PROJECTION_MATRIX, projection );
		glGetIntegerv( GL_VIEWPORT, viewport );
		winX = static_cast<float>(x);
		winY = static_cast<float>(viewport[3]) - static_cast<float>(y);
		glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
		gluUnProject(winX, winY, winZ, modelview, projection, viewport,
			&tempX, &tempY, &tempZ);
		posX = tempX;
		posY = tempY;
		posZ = tempZ;
	}

	void updateImpactVelocity() {
		if (plusPressed) {
			if (impactVelocity < 20.0f) {
				impactVelocity += 10.0f * frameTime;
			} else if (impactVelocity >= 20.0f && impactVelocity < 40.0f) {
				impactVelocity += 50.0f * frameTime;
			}
		}
		if (minusPressed) {
			if (impactVelocity > 0.1f && impactVelocity <= 20.0f) {
				impactVelocity -= 10.0f * frameTime;
			} else if (impactVelocity > 20.0f) {
				impactVelocity -= 50.0f * frameTime;
			}
		}
	}

	void updateCrackPattern(GLFWwindow* window, int button, int action, int bits) {

		if (button == GLFW_MOUSE_BUTTON_LEFT && 
			action == GLFW_PRESS && pos[2] > 0) {
			if (!mlClickedAlready) {
				getOGLPos(WIDTH/2, HEIGHT/2, posX, posY, posZ);
				if (posX <= 3.5 && posX >= -3.5 &&
					posY <= 8.5 && posY >= 1.5 &&
					posZ <= 0.1 && posZ >= -0.1) {
					
					// Test if the glass breaks, propagate cracks etc.
					cs->crackTester(impactVelocity, posX, posY, -3.5f, 8.5f, 3.5f, 1.5f);
					if (cs->getAngles() != nullptr) {
						mlClickedAlready = true;
						createCrack(cs, allPoints, deadEnds);
					}
				}
			}
		}
		if (button == GLFW_MOUSE_BUTTON_RIGHT && 
			action == GLFW_PRESS && pos[2] > 0) {
			if (mlClickedAlready && !mrClickedAlready) {
				findDeadEnds(deadEnds, allPoints);
				findHoles(holes, allPoints);
				mrClickedAlready = true;
			}
		}
	}

	GLFONT * fontCreate(HDC hdc, const char *typeface, 
		int height, int weight, DWORD italic) {
		
		GLFONT *font; // Font data pointer
		font = new GLFONT; // Memory allocation
		HFONT fontid; // Windows font ID
		int charset; // Character set code
		
		// Allocate display lists
		if ((font->base = glGenLists(256)) == 0) {
			delete font;
		return (nullptr);
		}

		charset = ANSI_CHARSET;
		
		// Load the font
		fontid = CreateFont(height, 0, 0, 0, weight, italic, FALSE, FALSE,
			charset, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
			DRAFT_QUALITY, DEFAULT_PITCH, typeface);
		SelectObject(hdc, fontid);
		
		// Create bitmaps for each character
		wglUseFontBitmaps(hdc, 0, 256, font->base);
		
		// Get the width and height information for each character
		GetCharWidth(hdc, 0, 255, font->widths);
		font->height = height;
		return (font);
	}
	
	void fontDelete(GLFONT *font) {
		if (font == static_cast<GLFONT *>(0)) {
			return;
		}
		glDeleteLists(font->base, 256);
		free(font);
	}

	void fontPuts(GLFONT *font,const char *s) {
		if (font == static_cast<GLFONT *>(0) || s == nullptr) {
			return;
		}
		glPushAttrib(GL_LIST_BIT);
		glListBase(font->base);
		glCallLists(strlen(s), GL_UNSIGNED_BYTE, s);
		glPopAttrib();
	}

	void fontPrintf(GLFONT *font, int align, char *format, ...) {
		va_list ap; // Argument pointer
		char s[1024], *ptr; // Output string, Pointer into string
		int width; // Width of string in pixels
		if (font == static_cast<GLFONT *>(0) || format == static_cast<char *>(0)) {
			return;
		}
		// Format the string
		va_start(ap, format);
		vsprintf(static_cast<char *>(s), format, ap);
		va_end(ap);
		
		// Figure out the width of the string in pixels...
		for (ptr = s, width = 0; *ptr; ptr ++) {
			width += font->widths[*ptr];
		}

		// Adjust the bitmap position as needed to justify the text
		if (align < 0) {
			glBitmap(0, 0, 0, 0, -width, 0, nullptr);
		} else if (align == 0) {
			glBitmap(0, 0, 0, 0, -width / 2, 0, nullptr);
		}
	
		// Draw the string
		fontPuts(font, s);
	}
	
	void draw(GLFWwindow * window) {
	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		updateFrameTime();
		updateMousePos(window);
		updateDirPos();
		updateImpactVelocity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		// Draw the crosshair centered on the screen, center alignment
		glPushMatrix();
		glColor3f(1.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, 0.0f, -1.5f);
		glRasterPos2i(0,0);
		fontPrintf(font, 0, "X");
		glPopMatrix();
		
		// Draw text in lower right corner, left alignment
		glPushMatrix();
		glColor3f(1.0f, 0.0f, 0.0f);
		glTranslatef(1.0f, -1.0f, -1.5f);
		glRasterPos2i(0,0);
		fontPrintf(font, 1, "Impact velocity: %4.0f", impactVelocity);
		glPopMatrix();
		
		// Rotate and translate the world
		glRotatef(vAngle, 1.0, 0.0, 0.0);
		glRotatef(hAngle, 0.0, 1.0, 0.0);
		glTranslatef(-pos[0], -pos[1], -pos[2]);
		
		// Execute the room display list
		glCallList(roomList);
		if (allPoints->size() > 1) {
			drawCracks(allPoints);
		}
		drawWindow(holes);
		glFlush();
	}

	void drawCracks(vector<Point*>* allPoints) {

		Point* p1;
		Point* p2;

		glEnable(GL_BLEND);
		glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
		glLineWidth(0.5f);

		for (int i = 0; i < allPoints->size(); i++) {
			p1 = allPoints->at(i);
			for (int j = 0; j < p1->getNumConnections(); j++) {
				p2 = p1->getPoint(j);
				if (holes->size() == 0) {
					// Draw all the cracks belonging to holes
					if (!p1->getTestFlag(j) && !p1->getEndFlag(j)) {
						glPushMatrix();
						glBegin(GL_LINES);
						glVertex3f(p1->getX(), p1->getY(), 0.0f);
						glVertex3f(p2->getX(), p2->getY(), 0.0f);
						glEnd();
						glPopMatrix();
					}
				}

				// Draw the rest of the cracks, i.e. the dead ends
				if (p1->getTestFlag(j) == false && p1->getEndFlag(j)== true) {
					glPushMatrix();
					glBegin(GL_LINES);
					glVertex3f(p1->getX(), p1->getY(), 0.0f);
					glVertex3f(p2->getX(), p2->getY(), 0.0f);
					glEnd();
					glPopMatrix();
				}

				// Switch the flag, indicating that the connection has been
				// drawn, or just resetting it.
				p1->setTestFlag(j, !(p1->getTestFlag(j)));
			}
		}
		glDisable(GL_BLEND);
	}

	void reshape(GLFWwindow* window, int w, int h) {
		glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
		glfwSetCursorPos(window, 0, 0);
		mouseX = 0;
		mouseOldX = 0;
		mouseY = 0;
		mouseOldY = 0;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(
		80.0,
		static_cast<double>(w)/ static_cast<double>(h),
		1.0,
		300
		);
	}

	void keyPressed(GLFWwindow* window, int key, int scancode, int action, int bits) {
		if (action == GLFW_PRESS) {
			switch (key) {
			case GLFW_KEY_UP: {
				upPressed = true;
				break;
			}
			case GLFW_KEY_DOWN: {
				downPressed = true;
				break;
			}
			case GLFW_KEY_LEFT: {
				leftPressed = true;
				break;
			}
			case GLFW_KEY_RIGHT: {
				rightPressed = true;
				break;
			}
			case 'W': {
				upPressed = true;
				break;
			}
			case 'S': {
				downPressed = true;
				break;
			}
			case 'A': {
				leftPressed = true;
				break;
			}
			case 'D': {
				rightPressed = true;
				break;
			}
			case GLFW_KEY_KP_ADD: {
				plusPressed = true;
				break;
			}
			case GLFW_KEY_KP_SUBTRACT: {
				minusPressed = true;
				break;
			}
			default:
				break;
			}
		}
		if (action == GLFW_RELEASE) {
			switch (key) {
			case GLFW_KEY_UP: {
				upPressed = false;
				break;
			}
			case GLFW_KEY_DOWN: {
				downPressed = false;
				break;
			}
			case GLFW_KEY_LEFT: {
				leftPressed = false;
				break;
			}
			case GLFW_KEY_RIGHT: {
				rightPressed = false;
				break;
			}
			case 'W': {
				upPressed = false;
				break;
			}
			case 'S': {
				downPressed = false;
				break;
			}
			case 'A': {
				leftPressed = false;
				break;
			}
			case 'D': {
				rightPressed = false;
				break;
			}
			case GLFW_KEY_KP_ADD: {
				plusPressed = false;
				break;
			}
			case GLFW_KEY_KP_SUBTRACT: {
				minusPressed = false;
				break;
			}
			case GLFW_KEY_SPACE: {
				resetCracks();
				break;
			}
			default:
				break;
			}
		}
	}
	void deinitGraphics() {
		fontDelete(font);
		glfwTerminate();
	}
}
#endif
