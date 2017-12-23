/* splitter.cpp - The main application file of the Splitter project. This file
* contains the main program loop.
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
#include <cstdlib>
#include <ctime>
#include "graphics.h"

using namespace std;
using namespace splitter;

/* Main method, initializes everything, includs the main loop
* that ends when Esc-key is pressed
*/

int main(int argc, char** argv) {
	/* Initialize the library */
	bool running = glfwInit();
	if (!running) return -1;
	
	GLFWwindow* window = initGraphics((argc >= 2) ? atoi(argv[1]) : 1024,
		(argc >= 2) ? atoi(argv[2]) : 768);

	if(!window){
		return -1;
	}

	initUIInput(window);
	srand(time(nullptr));

	while (!glfwWindowShouldClose(window)) {
		showFPS(window);
		draw(window);
		//Swap the drawing buffers
		glfwSwapBuffers(window);
		//Check if esc is pressed or if the window is closed
		running = !glfwGetKey(window, GLFW_KEY_ESCAPE) && (window != nullptr);
		
	}
	deinitGraphics();
	return 0;
}













