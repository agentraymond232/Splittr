/* holes.h - File to find all the parts of the crack pattern belonging to a
* dead end and function for finding all the “smallest” holes (i.e. the smallest
* glass pieces made by the crack pattern). Included in graphics.cpp.
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
using namespace std;
namespace splitter {
/* Finds the part of the crack pattern that has propagated further after a
* hole, or the hole crack pattern if there are no found holes. Saves the
* found dead ends in the deadEnds vector.
*
* Input: (deadEnds) a vector containing the end points of all created
* cracks. An end point is defined as: A point where the bending
* energy is zero.
* Output: void
*/
void findDeadEnds(vector<Point*>* deadEnds, vector<Point*>* allPoints);
/* Finds the smallest pieces in a crack pattern, wich is all the holes (not
* containing any other holes). Saves the points defining the holes in the
* holes vector.
*
* Input: (holes) an empty vector to store the vectors fore the holes in.
* (allPoints) vector containing all the points in the crack
* pattern.
* Output: void
*/
void findHoles(vector<vector<Point*>* >* holes, vector<Point*>* allPoints);
}
