/* holes.cpp - File to find all the parts of the crack pattern belonging to a
* dead end and function for finding all the “smallest” holes (i.e. the smallest
* glass pieces made by the crack pattern). Needs to include holes.h and
* Point.h. Executed from graphics.cpp.
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
#include "Point.h"
#include "holes.h"
#include <cmath>

using namespace std;

namespace splitter {
	void findDeadEnds(vector<Point*>* deadEnds, vector<Point*>* allPoints) {
		
		int counter = 0;
		int notDeadEnd = 0;

		// Check if there are any dead ends
		if (deadEnds->size() == 0)
			return;
		
		// Loop trough all dead ends
		for (auto i = 0; i < deadEnds->size(); i++) {
			
			const auto& id = deadEnds->at(i)->getID();
			auto p = allPoints->at(id);

			// If number of connections in one point is larger than four,
			// it must be a hole, due to the number of possible cracks from
			// an airbubble.
			if (p->getNumConnections() >= 5)
				continue;
					
			int j = p->getNumConnections();
					
			while(j > 0 && j < 5) {
				auto counter = 0;

				// Checks how many connections already set to dead
				// end. If the number of dead ends are one less than
				// the number of connections then the connection is
				// also a dead end.
				for (int connection = 0; connection < p->getNumConnections(); connection++) {

					if (p->getEndFlag(connection)) {
						counter++;
					} else {
						notDeadEnd = connection;
					}
				}
				if (counter == p->getNumConnections() - 1) {
					p->setEndFlag(notDeadEnd, true);
					p = p->getPoint(notDeadEnd);
				} else {
					break;
				}
				j = p->getNumConnections();
			}
			
		}
		
	}

	void findHoles(vector<vector<Point*>* >* holes, vector<Point*>* allPoints) {
	
		bool checkedAll = false;
		float polygonAngle = 0.0f;
		float thisAngle = 0.0f;
		float nextAngle = 0.0f;
		float firstAngle = 0.0f;
		int pos = 0;
		Point* firstPoint;
		Point* thisPoint;
		Point* prevPoint;
		Point* nextPoint = nullptr;
		Point* p;
		vector<Point*>* holePoints = new vector<Point*>;;
		
		// Loop through all points
		for (int i = 0; i < allPoints->size(); i++) {
			p = allPoints->at(i);
			// For every point check all the connections
			for (int connection = 0; connection < p->getNumConnections(); connection++) {
				// If the connection is already visited the path is not
				// availible, go to next connection. Also checks if it’s a
				// dead end.
				if (p->getPath(connection) && !p->getEndFlag(connection)) {
			
					polygonAngle = 0.0f;
					firstPoint = p;
				
					// Indicate that the connection has been visited
					firstPoint->setPath(connection, false);
					thisPoint = firstPoint->getPoint(connection);
					thisAngle = fmod(firstPoint->getAngles()->at(connection) + 180, 360);
					firstAngle = firstPoint->getAngles()->at(connection);
					prevPoint = firstPoint;
					holePoints->push_back(firstPoint);
				
					// Stays iside of while loop during caluculations of one
					// hole
					while (true) {
					
						// Loop trough the connections to find the previous
						// point because it’s angle is in intress.
						for (int j = 0; j < thisPoint->getNumConnections(); j++) {
							if (prevPoint == thisPoint->getPoint(j)) {
								pos = j;
								break;
							}
						}

						// Get the left most connection from which connection
						// comming from. Only allowed to use a connection that
						// is not allready tried or is set to be a dead end.
						for (int k = pos - 1; ; k--) {
							if (k < 0){
								k = thisPoint->getNumConnections()-1;
							}
							if (k == pos) {
								checkedAll = true;
								break;
							}
							if (thisPoint->getPath(k) && !thisPoint->getEndFlag(k)) {
								nextPoint = thisPoint->getPoint(k);
								nextAngle = thisPoint->getAngles()->at(k);
								thisPoint->setPath(k, false);
								break;
							}
						}
						// If all of the connections is already visited
						// break the while loop
						if (checkedAll) {
							checkedAll = false;
							break;
						}

						// Sum the inner angles of the polygon
						polygonAngle += fmod(360 + thisAngle - nextAngle, 360.0f);
					
						// Save the point and go to the next one.
						holePoints->push_back(thisPoint);
						prevPoint = thisPoint;
						thisAngle = fmod(nextAngle + 180.0f, 360.0f);
						thisPoint = nextPoint;
					
						// If the hole is completed
						if (thisPoint == firstPoint) {
							polygonAngle += fmod(360 + thisAngle - firstAngle, 360.0f);
							break;
						}
					}
				}
				// Check if the polygon was circumfered counter clockwise,
				// must be a bit wider limit because of some faults in
				// calculation with many decimals.
				if (polygonAngle > 179.9 * (holePoints->size() - 2) &&
					polygonAngle < 180.1 * (holePoints->size() - 2)) {
					holes->push_back(holePoints);
					holePoints = new vector<Point*>;
				} else {
					holePoints->clear();
				}
			}
		}
	}
}