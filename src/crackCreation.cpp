/* crackCreation.cpp - Contains functions for creating the crack pattern,
* crack propagation with collision detection between cracks.
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
#include <cstdlib>
#include <vector>

#include "Point.h"
#include "CrackStarter.h"
#include "crackCreation.h"

using namespace std;

#ifndef SPLITTER_PI
	#define SPLITTER_PI
	const float PI = 3.14159265;
#endif

namespace splitter {
	const float STEPSIZE = 0.2f;
	const float ENERGY_LOSS = 0.025f;
	const int MAX_BUBBLE_CRACKS = 3; // Not allowed to be greater than 30
	const int MIN_DIFFERENCE_ANGLE = 12; // Guarantees visibility of cracks

	bool findNewIntersection(Point* point1, Point* point2, Point* thisPoint,
		Point* newPoint, float* oldDist) {
		auto x1 = point1->getX();
		auto y1 = point1->getY();
		auto x2 = point2->getX();;
		auto y2 = point2->getY();;
		auto x3 = thisPoint->getX();
		auto y3 = thisPoint->getY();
		auto x4 = newPoint->getX();
		auto y4 = newPoint->getY();
	
		// Calculate intersection coordinates. If den is 0, there is no
		// intersection between the two lines. If den is not 0, there is an
		// intersection at apoint along the (infinitely long) lines, with
		// coordinates (xnum/den, ynum/den). That point may, however,
		// lie outside of thesupplied points, in which case the intersection
		// is not interesting.
		auto den = x1*y3 - x1*y4 - x2*y3 + x2*y4 - x3*y1 + x4*y1 + x3*y2 - x4*y2;
		auto xnum = x1*x3*y2 - x1*x4*y2 - x2*x3*y1 + x2*x4*y1 - x1*x3*y4 + x1*x4*y3
			+ x2*x3*y4 - x2*x4*y3;

		auto ynum = x1*y2*y3 - x1*y2*y4 - x2*y1*y3 + x2*y1*y4 - x3*y1*y4 
			+ x4*y1*y3 + x3*y2*y4 - x4*y2*y3;
		auto intersection = true;
	
		if (den == 0) {
			intersection = false;
		} else {
			auto x = xnum/den;
			auto y = ynum/den;
		
			// Check if intersection lies between the given points on the lines
			if (((x > x1 && x > x2) ||
				(x < x1 && x < x2) ||
				(x > x3 && x > x4) ||
				(x < x3 && x < x4)) ||
				((y > y1 && y > y2) ||
				(y < y1 && y < y2) ||
				(y > y3 && y > y4) ||
				(y < y3 && y < y4))) {
			
				intersection = false;
			} else {
	
				// Check if this intersection lies closer to the start point
				// of the proposed new line than a previously found one.
				// If so, this is the intersection to remember. If not, it is
				// discarded.
				auto testDist = abs(x3 - x) + abs(y3 - y);

				if (testDist < *oldDist || *oldDist == 0) {
					newPoint->setX(x);
					newPoint->setY(y);
					*oldDist = testDist;
				} else {
					intersection = false;
				}
			}
		}
		return intersection;
	}

	bool safeConnect(Point* thisPoint, Point* newPoint,
		vector<Point*>* allPoints) {
		
		Point *p1 = nullptr;
		Point *p2 = nullptr;
		auto tempDist = 0.0f;
		auto collision = false;
		
		//Marks the connections belonging to the point being checked
		for (auto i = 0; i < thisPoint->getNumConnections(); i++) {
			thisPoint->setTestFlag(i, true);
		}
		
		//Checks for collisions with all other connections
		for(auto i = 0; i < allPoints->size(); i++) {
			if(allPoints->at(i) != thisPoint) {
				for (auto j = 0; j < allPoints->at(i)->getNumConnections(); j++) {
					if(!allPoints->at(i)->getTestFlag(j)) {
						if (findNewIntersection( allPoints->at(i), 
							allPoints->at(i)->getPoint(j),
							thisPoint, newPoint, &tempDist)) {
							
							p1 = allPoints->at(i);
							p2 = allPoints->at(i)->getPoint(j);
							collision = true;
						}
					}
					//Resets the flags for all the connections when arriving to
					//the second point in the connection
					allPoints->at(i)->setTestFlag(j, !(allPoints->at(i)->getTestFlag(j)));
				}
			}
		}

		//In case of a collision connect the new connection with the one
		//colliding with and modify the previous connection. If collision with
		//an exact point, connection is made to the point in question.
		if (collision) {
		
			if (newPoint->getX() == p1->getX() && newPoint->getY() == p1->getY()) {
				thisPoint->connect(p1);
				delete newPoint;
			} else if (newPoint->getX() == p2->getX() &&
				newPoint->getY() == p2->getY()) {
				thisPoint->connect(p2);
				delete newPoint;
			} else {
				thisPoint->connect(newPoint);
				p1->disconnect(p2);
				p1->connect(newPoint);
				p2->connect(newPoint);
				allPoints->push_back(newPoint);
			}
		} else {
			thisPoint->connect(newPoint);
			allPoints->push_back(newPoint);
		}
		return collision;
	}

	void propagate(Point* thisPoint, float prevAngle, float bendEnergy,
		vector<Point*>* allPoints, vector<Point*>* deadEnds) {
		
		// Chance of an airbubble is 10%
		auto chance = rand() % 20;
		auto airBubble = (chance == 1 && bendEnergy >= 0.1)? true: false;
		
		auto numNewCracks = 1;
		auto newAngle = 0.0f;
		auto newX = 0.0f;
		auto newY = 0.0f;
		auto perfectAngle = 0.0f;
		auto maxDifference = 0.0f;
		
		// Temp variables used in loops below
		Point* tempPoint;
		bool collision;
		if (airBubble) {
			// Randomize how many “new” cracks that should start from the
			// airbubble point.
			// Make sure there is no airbubble at the end of a crack,
			// for esthetic reasons only.
			numNewCracks = (bendEnergy < 0.2)? 1 : 1 + rand() % (MAX_BUBBLE_CRACKS - 1);
			
			perfectAngle = 360 / (numNewCracks + 1);
			maxDifference = perfectAngle - (MIN_DIFFERENCE_ANGLE / 2);
		}

		for (auto i = 1; i <= numNewCracks; i++) {
			if (airBubble) {
			//if an air bubble, the new angles are randomized fully,
			//except that they are not allowed to be closer than
			//MIN_DIFFERENCE_ANGLE to each other
			newAngle = perfectAngle * i + (fmod(rand(), maxDifference) -
			maxDifference / 2);
			newAngle += prevAngle + 180.0f;
			
				// In the current implementation there is a bug that causes
				// cracks of angles that are multiples of 90 degrees to
				// somehow bypass the collision detection algorithm. Therefore,
				// the following check is made, just to make sure no angles
				// are allowed to be an exact multiple of 90 degrees.
				if (newAngle == 0 || newAngle == 90 || newAngle == 180 ||
					newAngle == 270){
					newAngle += 0.1;
				}
			} else {
				
				// If not an air bubble, the angle is randomized in a narrow
				// interval
				newAngle = rand() % MIN_DIFFERENCE_ANGLE - MIN_DIFFERENCE_ANGLE / 2;
				newAngle += prevAngle ;
				
				// Same check as above
				if (newAngle == 0 || newAngle == 90 || newAngle == 180 ||
					newAngle == 270){
					newAngle += 0.1;
				}
			}
			
			newAngle = fmod((newAngle + 360.0f), 360.0f);
			
			// Calculate the coordinates for the new point/points
			newX = STEPSIZE * cos(newAngle * PI / 180.0f);
			newY = STEPSIZE * sin(newAngle * PI / 180.0f);
			newX += thisPoint->getX();
			newY += thisPoint->getY();
			tempPoint = new Point(newX, newY);
			collision = safeConnect(thisPoint, tempPoint, allPoints);
			
			if (collision) {
				// In the current implementation, there is no phase where
				// the loose pieces actually fall off the window pane. One could
				// imagine that “spare energy” that is left when a crack
				// joins with another somehow affects how the piece that may
				// result is hurled away. In that case, the calculation of that
				// energy should probably be performed here.
			}
			else {
				if (bendEnergy / numNewCracks - ENERGY_LOSS >= 0) {
					
					// If the energy in the crack is not zero after
					// decreasing it, propagate is run again, with less energy.
					propagate(tempPoint, newAngle,
						bendEnergy / numNewCracks - ENERGY_LOSS, allPoints,
						deadEnds);
				} else {
					deadEnds->push_back(tempPoint);
				}
			}
		}
	}

	void createCrack(CrackStarter* starter, vector<Point*>* allPoints,
		vector<Point*>* deadEnds) {

		// Position the first point and add it to the main Point vector
		auto startPoint = new Point(starter->getX(), starter->getY());
		allPoints->push_back(startPoint);
	
		// Temp variables used below
		float crackCosAngle;
		float crackSinAngle;
		float tempDist;
		float tempX;
		float tempY;
		Point* tempPoint;

		if (starter->getRadialDistance() != nullptr) {
	
			// Create vector for start points of each bending-stress crack
			auto bendStarts = new vector<Point*>;
	
			// Create vector for holding the impact-stress cross-crack points
			auto impactPoints = new vector<vector<Point*>* >;
	
			// Position the impact-stress cross-crack points and bending-stress
			// start points
			for (auto crack = 0; crack < starter->getAngles()->size(); crack++) {
	
				// Check if this radial crack has no cross-cracks, if so
				// continue with the next crack
				if (starter->getRadialDistance()->at(crack) == nullptr) {

					// Place bend-start (extra) point but skip cross cracks
					impactPoints->push_back(nullptr);
					const auto& theta = starter->getAngles()->at(crack) * PI / 180.0f;
					crackCosAngle = cos(theta);
					crackSinAngle = sin(theta);
					tempDist = starter->getSafeDistance()->at(crack) + 0.01;
					tempX = tempDist * crackCosAngle;
					tempY = tempDist * crackSinAngle;
					tempX += starter->getX();
					tempY += starter->getY();
					tempPoint = new Point(tempX, tempY);
					bendStarts->push_back(tempPoint);
					startPoint->connect(tempPoint);
					allPoints->push_back(tempPoint);
					continue;
				}
				impactPoints->push_back(new vector<Point*>);
				const auto& theta = starter->getAngles()->at(crack) * PI / 180.0f;
				crackCosAngle = cos(theta);
				crackSinAngle = sin(theta);
				
				for (auto p = 0; p < starter->getRadialDistance()->at(crack)->size(); p++) {

					tempDist = starter->getRadialDistance()->at(crack)->at(p);
					tempX = tempDist * crackCosAngle;
					tempY = tempDist * crackSinAngle;
					tempX += starter->getX();
					tempY += starter->getY();
					tempPoint = new Point(tempX, tempY);
					impactPoints->at(crack)->push_back(tempPoint);
					allPoints->push_back(tempPoint);
					if (p == 0) {
						startPoint->connect(impactPoints->at(crack)->at(p));
					} else {
						impactPoints->at(crack)->at(p-1)->connect(
						impactPoints->at(crack)->at(p));
					}
				}
				// Position bend start point
				if (starter->getRadialDistance()->at(crack)->size() > 0) {
					int pos = starter->getRadialDistance()->at(crack)->size() - 1;
					tempDist = starter->getRadialDistance()->at(crack)->at(pos);
					tempDist += starter->getSafeDistance()->at(crack) + 0.01;
					tempX = tempDist * crackCosAngle;
					tempY = tempDist * crackSinAngle;
					tempX += starter->getX();
					tempY += starter->getY();
					tempPoint = new Point(tempX, tempY);
					bendStarts->push_back(tempPoint);
					impactPoints->at(crack)->at(pos)->connect(tempPoint);
					allPoints->push_back(tempPoint);
				}
			}
			// Temp variables used below
			float relAngle;
			float compAngle;
			float thisAngle;
			float tempCos;
			float tempSin;
			float length;
			
			// Position all impact-stress cross-cracks. These always start from
			// the predecided points along the radial cracks and take off to the
			// right, until they join with the nearest radial crack.
			for (auto crack = 0; crack < starter->getAngles()->size(); crack++) {
			
				// Check if this radial crack has no cross-cracks, if so
				// continue with the next crack
				if (starter->getRadialDistance()->at(crack) == nullptr)
					continue;
			
				// Aquire index of the nearest radial crack, clockwise
				// orientation
				int prevCrack = crack == 0 ? starter->getAngles()->size() - 1 : crack - 1;

				// Relative angle between this radial crack and the one to the
				// right
				relAngle = starter->getAngles()->at(crack) -
					starter->getAngles()->at(prevCrack);
				relAngle = relAngle >= 0 ? relAngle : 360.0f + relAngle;
			
				// Only create the cross-crack if the angle between the radial
				// cracks is less than or equal to 90 degrees
				if (relAngle <= 90.0f) {

					// Calculate cross-crack global angle
					compAngle = (180.0f - relAngle) / 2.0f;
					thisAngle = compAngle + starter->getAngles()->at(crack) + 180.0f;
					thisAngle = fmod(thisAngle + 360.0f, 360.0f);
					if(thisAngle == 0 || thisAngle == 90 ||
						thisAngle == 180 || thisAngle == 270) {
						thisAngle -= 0.1f;
					}
					const auto& theta = thisAngle * PI / 180.0f;
					tempCos = cos(theta);
					tempSin = sin(theta);
			
					// The length of the cross-crack is initially set to 1.5
					// times the distance from the cross-crack start point to
					// the original impact point. This guarantees the
					// cross-crack to always join with the neighbouring radial
					// crack to the right.
					length = 1.5f * starter->getRadialDistance()->at(crack)
						->at( starter->getRadialDistance()->at(crack)->size() - 1 );
					for (auto p = 0; p < starter->getRadialDistance()->at(crack)->size(); p++) {
						tempX = length * tempCos;
						tempY = length * tempSin;
						tempX += impactPoints->at(crack)->at(p)->getX();
						tempY += impactPoints->at(crack)->at(p)->getY();
						tempPoint = new Point(tempX, tempY);
						safeConnect(impactPoints->at(crack)->at(p), tempPoint, allPoints);
					}
				}
			}

			// Now proceed by propagating cracks caused by bending-stress
			// from all the previously defined start points.
			for (auto i = 0; i < bendStarts->size(); i++) {
				if (starter->getBendingEnergyVector()->at(i) == 0)
					continue;
				propagate(bendStarts->at(i),
				starter->getAngles()->at(i),
				starter->getBendingEnergyVector()->at(i),
				allPoints,
				deadEnds);
			}
		}
		else {
			
			// Temp variables used below
			float startCosAngle;
			float startSinAngle;
			float tempStartX;
			float tempStartY;
			Point* tempStart;
	
			// Create start points for bending-stress cracks
			auto bendStarts = new vector<Point*>;
			
			for (auto i = 0; i < starter->getAngles()->size(); i++) {
				if (starter->getBendingEnergyVector()->at(i) == 0)
					continue;
				const auto& theta = starter->getAngles()->at(i) * PI / 180.0f;
				startCosAngle = cos(theta);
				startSinAngle = sin(theta);
				tempStartX = STEPSIZE * startCosAngle;
				tempStartY = STEPSIZE * startSinAngle;
				tempStartX += starter->getX();
				tempStartY += starter->getY();
				tempStart = new Point(tempStartX, tempStartY);
				startPoint->connect(tempStart);
				bendStarts->push_back(tempStart);
				allPoints->push_back(tempStart);
			}

			// Propagate cracks caused by bending-stress, with start point in
			// the impact point (because of no impact-stress radial/cross
			// cracks).
			for (auto i = 0; i < starter->getAngles()->size(); i++) {
				propagate(bendStarts->at(i), starter->getAngles()->at(i),
					starter->getBendingEnergyVector()->at(i), allPoints, deadEnds);
			}
		}
	}
}