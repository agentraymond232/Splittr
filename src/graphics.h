/* graphics.h - File to handle all graphical parts except for the tesselation
* of the glass polygon, done in tesselate.cpp. graphics.h is included in
* splitter.cpp and needs point.h to run.
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
#include <windows.h>
#include <GLFW/glfw3.h>
#include "Point.h"

using namespace std;
namespace splitter {

	//Data types for the bitmap fonts
	typedef struct {

		GLuint base; //Display list number of first character
		int widths[256]; //Width of each character in pixels
		int height; //Height of characters
	} GLFONT;

	/* Initialize and set up the graphical environment.
	*
	* Input: void
	* Output: void
	*/
	GLFWwindow* initGraphics(int resWidth, int resHeight);

	/* Initialize the allPoints and cs objects, and set the clickedAlready
	* flag to false (so that it is possible to create new cracks). This
	* removes all previous cracks.
	*
	* Input: void.
	* Output: void.
	*/
	void resetCracks();

	/* Initialize and set up the input dependent user interface.
	*
	* Input: void
	* Output: void
	*/
	void initUIInput(GLFWwindow *);

	/* Calculate and report frames per second
	* (updated once per second) in the window title bar.
	*
	* Input: void
	* Output: void
	*/
	void showFPS(GLFWwindow*);

	/* Calculates the time since the last frame.
	* This is used to update the inputs from the keyboards.
	* Changes the variable frameTime.
	*
	* Input: void
	* Output: void
	*/
	void updateFrameTime();

	/* Get the mouse position from the screen.
	*
	* Input: void
	* Output: void
	*/
	void updateMousePos(GLFWwindow * window);

	/* Uppdate horizontal movment (xz-plane) and view direction.
	*
	* Input: void
	* Output: void
	*/
	void updateDirPos();

	/* Gets the cooridinate of the point that is projected on the screen
	* coordinates, (x,y).
	*
	* Input: (x) screen x - coordinate
	* (y) screen y - coordinate
	* Output: void
	*/
	void getOGLPos(int x, int y, GLfloat& posX, GLfloat& posY, GLfloat& posZ);

	/* Uppdate the impact velocity applied from the user with the + and -
	* numpad keys.
	*
	* Input: void
	* Output: void
	*/
	void updateImpactVelocity();

	/* Calls function getOGLPos to check if pointing on the glass window.
	* Runs the function crackTester to se if the glass breaks. Also runs
	* createCrack to calculate the crackPattern.
	* Only possible to press the button on one side off the wall.
	*
	* Input: (button) the specified button asigned for the task
	* (action) is the button pressed or released
	* Output: void
	*/
	void updateCrackPattern(GLFWwindow* window, int button, int action, int mods);

	/* Create a bitmap font.
	*
	* Input: (hdc) Device Context
	* (*typeface) Font specification
	* (height) Font height/size in pixels
	* (weight) Weight of font (bold, etc)
	* (italic) Text is italic
	* Output: (GLFONT) The font
	*/
	GLFONT * fontCreate(HDC hdc, const char *typeface,
		int height, int weight, DWORD italic);

	/* Delete the specified font.
	*
	* Input: (*font) Font to delete
	* Output: void
	*/
	void fontDelete(GLFONT *font);

	/* Display a string using the specified font.
	*
	* Input: (*font) Font to use
	* (*s) String to display
	* Output: void
	*/
	void fontPuts(GLFONT *font,const char *s);

	/* Display a formatted string using the specified font.
	*
	* Input: (*font) Font to use
	* (align) Alignment to use
	* (*format) printf() style format string, i.e. text to print
	* (...) Other arguments as necessary
	* Output: void
	*/
	void fontPrintf(GLFONT *font, int align, char *format, ...);

	/* Draw all the graphics to the buffer.
	*
	* Input: void
	* Output: void
	*/
	void draw(GLFWwindow* window);

	/* Draw the crack pattern.
	*
	* Input: (allPoints) the vector containing all points and their
	* connections
	* Output: void
	*/
	void drawCracks(vector<Point*>* allPoints);

	/* Configure camera options.
	*
	* Input: (w) widht of window
	* (h) height of window
	* Output: void
	*/
	void reshape(GLFWwindow* window, int w, int h);

	/* Check keyboard inputs, arrow keys and ‘w’ ‘a’ ‘s’ ‘d’.
	*
	* Input: (key) specifies a key
	* (action) checks if the specified key is pressed or relesed
	* Output: void
	*/
	void keyPressed(GLFWwindow* window, int key, int scancode, int action, int modbits);

	/* Deinitialize all graphical elements.
	*
	* Input: void
	* Output: void
	*/
	void deinitGraphics();
}
