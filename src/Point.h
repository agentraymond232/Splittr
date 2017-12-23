/* Point.h - Data represenation class for the crack line segment end points.
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
#ifndef SPLITTER_POINT
#define SPLITTER_POINT
#include <vector>
using namespace std;
namespace splitter {
	class Point {
		public:
		/* Creates a Point object with unspecified coordinates.
		*
		* Input: void
		* Output: A Point object.
		*/
		Point();
		/* Creates a Point object with the specified coordinates.
		*
		* Input: (x) the new X coordinate.
		* (y) the new Y coordiante.
		* Output: A Point object.
		*/
		Point(float x, float y);
		~Point() { Point::numPoints--; }
		void setXY(float x, float y) { this->x = x; this->y = y; }
		void setX(float x) { this->x = x; }
		void setY(float y) { this->y = y; }
		/* Sets the intersection-test flag at position i to the specified
		* value. Note that the flag is shared between the two connected
		* points, so this method only has to be run on one of the points
		* for changes to take effect on both.
		*
		* Input: (i) the position of the intersection-test flag to alter.
		* (value) the new value of the test flag.
		* Output: void
		*/
		void setTestFlag(short i, bool value);

		/* Sets the dead end flag at position i to the specified
		* value. Note that the flag is shared between the two connected
		* points, so this method only has to be run on one of the points
		* for changes to take effect on both.
		*
		* Input: (i) the position of the dead end flag to alter.
		* (value) the new value of dead end flag.
		* Output: void
		*/
		void setEndFlag(short i, bool value);
		/* This metod is only used from the findHoles metod.
		* Sets the Path flag at position i to the specified value.
		* The path is available to use once from both directions
		* wich meens that every point has its own flag, for everey
		* connection. If the value is TRUE, the path is available to use
		* else (FALSE) the path is closed.
		*
		* Input: (i) the position of the flag to alter
		* (value) the new value of the flag
		* Output: void
		*/
		void setPath(short i, bool value);
		float getX() { return x; }
		float getY() { return y; }
		/* Returns the specified intersection-test flag.
		*
		* Input: (i) the position of the test flag of interest.
		* Output: TRUE if the crack section at position i has been tested
		* before, FALSE otherwise. FALSE is also returned if
		* there is no flag at position i. This happens only if
		* i is larger than [number of points - 1].
		*/
		bool getTestFlag(short i);
		/*
		* Returns the specified dead end flag.
		*
		* Input: (i) the position of the dead end flag of interest.
		* Output: TRUE if the connection belongs to a dead end,
		* FALSE otherwise.
		*/
		bool getEndFlag(short i);
		/* Returns the specified path flag.
		*
		* Input: (i) the position of the path flag of interest.
		* Output: TRUE if the flag is available to use.
		* FALSE if the connection is visited once before, from
		* the same direction
		*/
		bool getPath(short i);
		/* Returns the point’s unique ID
		*
		* Input: void
		* Output: The point’s ID
		*/
		int getID() { return ID; }
		/* Retrieves the number of points connected to this point.
		*
		* Input: void
		* Output: The number of neighbouring (connected) points.
		*/
		short getNumConnections() { return angles->size(); }
		/* Connects this point with the supplied point. The connection
		* is performed on both points. This method uses the addPoint()
		* method of both points.
		*
		* Input: (p) Point object to connect to this point.
		* Output: void
		*/
		void connect(Point* p);
		/* Disconnects the supplied point from this point, and vice versa.
		* This method uses the removePoint() method of both points. If the
		* supplied point and this point are not connected, nothing happens.
		*
		* Input: (p) Point object to disconnect from this point.
		* Output: void
		*/
		void disconnect(Point* p);
		/* Helper method for connect(). Adds the point p to this point.
		*
		* Input: (p) Point object to add to this point.
		* Output: void
		*/
		void addPoint(Point* p, float angle, bool* testFlag, bool* endFlag,
		bool pathFlag);
		/* Helper method for disconnect(). Removes the point p from this
		* point. If p is not connected to this point, nothing happens.
		*
		* Input: (p) Point object to remove from this point.
		* Output: void
		*/
		void removePoint(Point* p);
		vector<Point*>* getPoints() { return points; }
		vector<float>* getAngles() { return angles; }
		/* Returns the connected Point object at position pos.
		*
		* Input: (pos) The position of the Point object of interest.
		* Output: The connected Point object at the specified position.
		* If there is no point at position pos, NULL is returned.
		* This only happens if pos is larger than
		* [number of points - 1].
		*/
		Point* getPoint(short pos);
		static int numPoints; // The total number of existing Point objects.
		private:
		float x;
		float y;
		int ID;
		vector<Point*>* points;
		vector<float>* angles;
		vector<bool*>* testFlags;
		vector<bool*>* endFlags;
		vector<bool>* pathFlags;
	};
}
#endif