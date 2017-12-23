/* CrackStarter.cpp - This is a class that contains functions that test if
* there will be any cracks, if so, how many and to what direction they will
* propagate. It also have functions that give each crack circumferential cracks
* at given distances.
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
#include "CrackStarter.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>

using namespace std;

namespace splitter{
	CrackStarter::CrackStarter() { //initials the constants
		BREAK_LIMIT = 1;
		CRACK_NUMBER = 1;
		ENERGY_DIFFER = 0.099;
		NORMAL_BREAK = 10;
		BENDING_INTERVAL = 5;
		CROSS_INTERVAL = 20;
		RADIAL_FACTOR = 20.0f;
		MAX_RADIALS = 10;
		DISTANCE_FACTOR = 3;
		MAX_CRACKS = 30;
		MIN_CRACK_DIFFERENCE = 12;
		angleVector = new vector<float>;
		bendingEnergyVector = new vector<float>;
		impactEnergyVector = new vector<float>;
		radialCrackVector = new vector<vector<float>* >;
		safeDistance = new vector<float>;
	}

	void CrackStarter::crackTester(float hitSize, float placeX, float placeY,
		float cornerOneX, float cornerOneY, float cornerTwoX, float cornerTwoY){
	
		this->placeX = placeX;
		this->placeY = placeY;
		actualBreakLimit = realBreakLimit(configureBreakLimit(),
		placeX, placeY,
		cornerOneX, cornerOneY,
		cornerTwoX, cornerTwoY);
		crackGenerator(hitSize, actualBreakLimit);
		if (numberOfCracks != 0) {
		angleGenerator(angleVector);
		energyGenerator(bendingEnergyVector, impactEnergyVector);
		radialCrackGenerator(radialCrackVector, safeDistance);
		}
		}
		float CrackStarter::configureBreakLimit() {
		weibull = 1 + rand() % 20;
		
		//Decides what breakLimit will be, this is
		//different from time to time
		switch (weibull) {
			case 1: {
				breakLimit = NORMAL_BREAK*0.55;
				break;
			}
			case 2: {
				breakLimit = NORMAL_BREAK*0.65;
				break;
			}
			case 3: {
				breakLimit = NORMAL_BREAK*0.70;
				break;
			}
			case 4: {
				breakLimit = NORMAL_BREAK*0.75;
				break;
			}
			case 5: {
				breakLimit = NORMAL_BREAK*0.80;
				break;
			}
			case 6: {
				breakLimit = NORMAL_BREAK*0.83;
				break;
			}
			case 7: {
				breakLimit = NORMAL_BREAK*0.85;
				break;
			}
			case 8: {
				breakLimit = NORMAL_BREAK*0.87;
				break;
			}
			case 9: {
				breakLimit = NORMAL_BREAK*0.89;
				break;
			}
			case 10: {
				breakLimit = NORMAL_BREAK*0.92;
				break;
			}
			case 11: {
				breakLimit = NORMAL_BREAK*0.95;
				break;
			}
			case 12: {
				breakLimit = NORMAL_BREAK*0.97;
				break;
			}
			case 13: {
				breakLimit = NORMAL_BREAK*1.01;
				break;
			}
			case 14: {
				breakLimit = NORMAL_BREAK*1.03;
				break;
			}
			case 15: {
				breakLimit = NORMAL_BREAK*1,05;
				break;
			}
			case 16: {
				breakLimit = NORMAL_BREAK*1.07;
				break;
			}
			case 17: {
				breakLimit = NORMAL_BREAK*1.10;
				break;
			}
			case 18: {
				breakLimit = NORMAL_BREAK*1.14;
				break;
			}
			case 19: {
				breakLimit = NORMAL_BREAK*1.20;
				break;
			}
			case 20: {
				breakLimit = NORMAL_BREAK*1.30;
				break;
			}
		}
		return breakLimit;
	}

	//This changes the break limit if you don’t hit in the center of the window.
	float CrackStarter::realBreakLimit(float breakLimit, float placeX,
		float placeY, float cornerOneX, float cornerOneY,float cornerTwoX,
		float cornerTwoY) {
		centerX = (cornerTwoX - cornerOneX) / 2 + cornerOneX;
		centerY = (cornerOneY - cornerTwoY) / 2 + cornerOneY;
		distanceToCenter = pow((centerX - placeX), 2) + pow((centerX - placeX), 2);
		actualBreakLimit = breakLimit - (BREAK_LIMIT * sqrt(distanceToCenter));
		return actualBreakLimit;
	}

	void CrackStarter::crackGenerator(float hitsize, float actualBreakLimit) {
		crackEnergy = hitsize - actualBreakLimit;
		if (crackEnergy <= 0){
			numberOfCracks = 0;
		} else {
			//There should always be an even number of cracks.
			numberOfCracks = 2 * int(crackEnergy*CRACK_NUMBER);
			if (numberOfCracks > MAX_CRACKS) {
				numberOfCracks = MAX_CRACKS;
			}
		}
	}

	void CrackStarter::angleGenerator(vector<float>* angleVector) {
		float perfectAngle = 360 / numberOfCracks;
		auto maxDifference = (360 / numberOfCracks) - MIN_CRACK_DIFFERENCE / 2 ;
		float angle;
		for (int i = 0; i < (numberOfCracks/2); i++) {
			// Give each crack an angle.
			angle = maxDifference/2 + perfectAngle * i +
			(rand() % maxDifference - maxDifference / 2);
			if (angle == 0||angle == 90||angle == 180||angle == 270){
				angle += 0.1; // Due to the multiple-of-90-degrees-bug.
			}
			angleVector->push_back(angle);
		}
		for (int i = 0; i < (numberOfCracks / 2); i++) {
		// Cracks continue in both directions.
			angleVector->push_back(angleVector->at(i) + 180);
		}
	}

	//Divides the energy to two differnt types of energy. One that decides how
	//mant circumferential crack there will be and one that decides for how long
	//the crack will go on after them.
	void CrackStarter::energyGenerator(vector<float>* bendingEnergyVector, 
		vector<float>* impactEnergyVector) {
		
		float totalBendingEnergy;
		float totalImpactEnergy;
		float relationEnergy = (crackEnergy - BENDING_INTERVAL) / CROSS_INTERVAL;
		if (relationEnergy <= 0) {
			totalBendingEnergy = crackEnergy;
			totalImpactEnergy = 0;
		} else if (relationEnergy > 0 && relationEnergy < 1) {
			totalImpactEnergy = crackEnergy * relationEnergy;
			totalBendingEnergy = crackEnergy - totalImpactEnergy;
		} else {
			totalImpactEnergy = crackEnergy;
			totalBendingEnergy = 0;
		}

		float impactEnergyPerCrack = totalImpactEnergy / numberOfCracks;
		float bendingEnergyPerCrack = totalBendingEnergy / numberOfCracks;
		
		for (int i = 0; i < numberOfCracks; i++) {
			if (impactEnergyPerCrack > 0) {
				// Randomize the impact energy a bit
				impactEnergyVector->push_back(impactEnergyPerCrack + (fmod(rand(), ENERGY_DIFFER) - ENERGY_DIFFER / 2));
			} else {
				impactEnergyVector->push_back(0);
			}
		
			if (bendingEnergyPerCrack > 0) {
			// Randomize the bending energy a bit
				const float& tempEnergy= bendingEnergyPerCrack + (fmod(rand(), ENERGY_DIFFER) - ENERGY_DIFFER / 2);
				bendingEnergyVector->push_back(tempEnergy < 0 ? 0 : tempEnergy);
			} else {
				bendingEnergyVector->push_back(0);
			}
		}
	}

	//Creates the circumferential cracks distance to the impact point.
	void CrackStarter::radialCrackGenerator( vector<vector<float>* >*& radialCrackVector,
		vector<float>*& safeDistance) {

		float minLast = 0;

		float maxLast = 0;
		int zeroRadials = 0;
		vector<float>* radialVector;
		
		for (int i = 0; i < numberOfCracks; i++) {
			if (impactEnergyVector->at(i) <= 0.0f) { //if there is no energy
				radialCrackVector->push_back(nullptr); //there will be no cracks.
				zeroRadials++;
			} else {
				float radialEnergy = (impactEnergyVector->at(i) * RADIAL_FACTOR);
				int numberOfRadials = static_cast<int>(radialEnergy);
				if (numberOfRadials > MAX_RADIALS) {
					numberOfRadials = MAX_RADIALS;
				}
				float firstDistance = DISTANCE_FACTOR / radialEnergy;
				if (firstDistance > 2.5){
					firstDistance = 2.5;
				}
				radialVector = new vector<float>;
				if (numberOfRadials > 0){
					radialVector->push_back(firstDistance);
					if (numberOfRadials > 1){
						for (int j = 1; j < numberOfRadials; j++) {
							radialVector->push_back(radialVector->at(j-1) + firstDistance /pow(2.0, (double)j));
						}
					}
				} else {
					radialVector = nullptr;
					zeroRadials++;
				}
				radialCrackVector->push_back(radialVector);
			}
		}
		if (zeroRadials == numberOfCracks){
			radialCrackVector = nullptr;
		} else {
			float thisLast;
			float nextLast;
			for (int i = 0; i<numberOfCracks - 1; i++) {
				if (radialCrackVector->at(i) != nullptr) {
					thisLast = radialCrackVector->at(i)->at(radialCrackVector->at(i)->size()-1);
				} else {
					thisLast = 0.0f;
				}
				if (radialCrackVector->at(i + 1) != nullptr) {
					nextLast = radialCrackVector->at(i + 1)->at( radialCrackVector->at(i + 1)->size() - 1);
				} else {
					nextLast = 0.0f;
				}

				if ((nextLast - thisLast) >= 0){
					safeDistance->push_back(nextLast - thisLast);
				} else {
					safeDistance->push_back(0);
				}
			}
			if (radialCrackVector->at(numberOfCracks - 1) != nullptr) {
				thisLast = radialCrackVector->at(numberOfCracks - 1)->at(
				radialCrackVector->at(numberOfCracks - 1)->size() - 1);
			} else {
				thisLast = 0.0f;
			}
			if (radialCrackVector->at(0) != nullptr) {
				nextLast = radialCrackVector->at(0)->at(radialCrackVector->at(0)->size() - 1);
			} else {
			nextLast = 0.0f;
			}
			if ((nextLast - thisLast) >= 0){
				safeDistance->push_back(nextLast - thisLast);
			}
			else {
				safeDistance->push_back(0);
			}
		}
	}

	vector<float>* CrackStarter::getAngles() {
		if (numberOfCracks == 0) {
			return NULL;
		}
		return angleVector;
	}

	vector<float>* CrackStarter::getBendingEnergyVector() {
		return bendingEnergyVector;
	}

	vector<float>* CrackStarter::getSafeDistance() {
		return safeDistance;
	}

	float CrackStarter::getX() {
		return placeX;
	}

	float CrackStarter::getY() {
		return placeY;
	}

	vector<vector<float>* >* CrackStarter::getRadialDistance() {
		return radialCrackVector;
	}
}