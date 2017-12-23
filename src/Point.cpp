/* Point.cpp - Data represenation class for the crack line segment end points.
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
#include "Point.h"
#include <iostream>
#include <vector>

using namespace std;
namespace splitter {

	#ifndef SPLITTER_PI
	#define SPLITTER_PI
	const float PI = 3.14159265;
	#endif

	int Point::numPoints = 0;
	
	Point::Point() {
		ID = Point::numPoints++;
		points = new vector<Point*>;
		angles = new vector<float>;
		testFlags = new vector<bool*>;
		endFlags = new vector<bool*>;
		pathFlags = new vector<bool>;
	}

	Point::Point(float x, float y) {
		this->x = x;
		this->y = y;
		ID = Point::numPoints++;
		points = new vector<Point*>;
		angles = new vector<float>;
		testFlags = new vector<bool*>;
		endFlags = new vector<bool*>;
		pathFlags = new vector<bool>;
	}

	void Point::connect(Point* p) {
	
		// Calculate the angle between this and p
		float tempAngle = 180.0f / PI * atan2(p->getY() - getY(), p->getX() - getX());

		// Make sure tempAngle is in the interval [0, 360[
		tempAngle = fmod((tempAngle + 360.0f), 360.0f);

		bool* tempBool = new bool;
		*tempBool = false;
		bool* tempEnd = new bool;
		*tempEnd = false;
		bool tempPath = true;

		// Store the connection in the two points
		addPoint(p, tempAngle, tempBool, tempEnd, tempPath);
		p->addPoint(this, fmod((tempAngle + 180.0f), 360.0f), tempBool, tempEnd, tempPath);
	}

	void Point::addPoint(Point* p, float angle, bool* testFlag,
		bool* endFlag, bool pathFlag) {
	
		// Find the correct position for the new value
		int insertPos = 0;
		
		for (; insertPos < angles->size() && angles->at(insertPos) < angle; insertPos++) {;}
		
		// Push the rest of the angles (, points and flags) down a click
		if (angles->size() > 0) {
			angles->push_back(angles->at(angles->size() - 1));
			points->push_back(points->at(points->size() - 1));
			testFlags->push_back(testFlags->at(testFlags->size() - 1));
			endFlags->push_back(endFlags->at(endFlags->size() - 1));
			pathFlags->push_back(pathFlags->at(pathFlags->size() - 1));

			for (int i = angles->size() - 2; i > insertPos; i--) {
				angles->at(i) = angles->at(i - 1);
				points->at(i) = points->at(i - 1);
				testFlags->at(i) = testFlags->at(i - 1);
				endFlags->at(i) = endFlags->at(i - 1);
				pathFlags->at(i) = pathFlags->at(i - 1);
			}

			// Insert the new angle/point/flags
			angles->at(insertPos) = angle;
			points->at(insertPos) = p;
			testFlags->at(insertPos) = testFlag;
			endFlags->at(insertPos) = endFlag;
			pathFlags->at(insertPos) = pathFlag;
		} else {
			angles->push_back(angle);
			points->push_back(p);
			testFlags->push_back(testFlag);
			endFlags->push_back(endFlag);
			pathFlags->push_back(pathFlag);
		}
	}

	void Point::disconnect(Point* p) {
		removePoint(p);
		p->removePoint(this);
	}

	void Point::removePoint(Point* p) {
		for (int i = 0; i < points->size(); i++) {
			if (points->at(i) != p)
				continue;

			// Remove p from this, together with angle and flag.
			// Shift the rest of the points/angles/flags down properly.
			if (i < points->size()-1) {
				for (int j = i; j < points->size()-1; j++) {
					points->at(j) = points->at(j+1);
					angles->at(j) = angles->at(j+1);

					testFlags->at(j) = testFlags->at(j+1);
					endFlags->at(j) = endFlags->at(j+1);
				}
			}
			points->pop_back();
			angles->pop_back();
			testFlags->pop_back();
			endFlags->pop_back();
			break;
		}
	}

	void Point::setEndFlag(short i, bool value) {
		if (i < endFlags->size()) {
			*(endFlags->at(i)) = value;
		}
	}

	bool Point::getEndFlag(short i) {
		if (i < endFlags->size()) {
			return *(endFlags->at(i));
		} else {
			return false;
		}
	}

	void Point::setTestFlag(short i, bool value) {
		if (i < testFlags->size()) {
			*(testFlags->at(i)) = value;
		}
	}

	bool Point::getTestFlag(short i) {
		if (i < testFlags->size()) {
			return *(testFlags->at(i));
		} else {
			return false;
		}
	}

	void Point::setPath(short i, bool value) {
		if (i < pathFlags->size()) {
			pathFlags->at(i) = value;
		}
	}

	bool Point::getPath(short i) {
		if (i < pathFlags->size()) {
			return pathFlags->at(i);
		} else {
			return false;
		}
	}

	Point* Point::getPoint(short pos) {
		if (pos < points->size()) {
			return points->at(pos);
		} else {
			return nullptr;
		}
	}
}
