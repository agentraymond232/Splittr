/* crackCreation.h - Contains functions for creating the crack pattern,
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
#include <vector>
#include "Point.h"
#include "CrackStarter.h"

using namespace std;
namespace splitter {
/* Used by safeConnect(). This function checks whether the supplied points
* form lines that intersect closer to the start point than the last found
* intersection.
*
* Input: (point1) First point used in the first line.
* (point2) Second point used in the first line.
* (thisPoint) The start point of the proposed new line.
* (newPoint) The end point of the proposed new line.
* (oldDist) The city-block distance from thisPoint to the nearest
* previously detected intersection. Should be 0 to indicate that no
* intersection has been detected before.
* Output: TRUE if a new intersection is found (closer than any previous
* one),
* FALSE if no intersection or intersection but further away than a
* previous one.
*/
bool findNewIntersection(Point* point1,
Point* point2,
Point* thisPoint,
Point* newPoint,
float* oldDist);
/* Connects a point to another one. If the connection between these points
* is crossing previously made connections, appropriate actions are taken.
*
* Input: (thisPoint) the point in question, where a connection is to
* occur.
* (newPoint) the second proposed point for the connection wanted.
* (allPoint) a vector including all existing points and their
* connections.
* Output: (bool) True if there is a collision between the expected
* connection and any other existing connection. False if no
* collision accured.
* thisPoint will be connected to another point in any case.
*/
bool safeConnect(Point* thisPoint, Point* newPoint, vector<Point*>* allPoints);
/* Propagates a crack until its energy is zero or the crack collides with
* another already existing crack.
*
* Input: (thispoint) point from where the crack segment are to be
* generated
* (prevAngle) the global angle on the previous crack segment
* (bendEnergy) how much bending energy left in the crack
* (allPoints) vector that includes all the different points and
* the internal connections
* (deadEnds) vector that includes all the end points in the
* crackpattern and their connections
* Output: void
*/
void propagate(Point* thisPoint, float prevAngle, float bendEnergy,
vector<Point*>* allPoints,vector<Point*>* deadEnds);
/* Creates the data representation of a crack, from impact to stationary
* phase.
*
* Input: (starter) CrackStarter object with init info for the propagation.
* (allPoints) Vector containing all the crack segment points.
* (deadEnds) Vector that includes all the end points in the
* crackpattern and their connections.
* Output: None.
*/
void createCrack(CrackStarter* starter,
vector<Point*>* allPoints, vector<Point*>* deadEnds );
}