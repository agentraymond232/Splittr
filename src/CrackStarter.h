/* CrackStarter.h - This is a class that contains functions that test if
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
#ifndef SPLITTER_CRACKSTARTER
#define SPLITTER_CRACKSTARTER

#include <vector>

using namespace std;

namespace splitter{
	class CrackStarter{
		public:
		CrackStarter();

		/* Initialize the CrackStarter that calls other functions that
		* calculates the crack properties. The input parameters are stored
		* globaly.
		* Input: (hitsize) The amount of energy added to the glas.
		* (placeX, placeY) The coordinates of the impact
		* (CornerOneX, CornerOneY, CornertTwoX, CornerTwoY) The
		* coordinates of two diagonal corners.
		* Output: void
		*/
		void crackTester(float hitSize, float placeX, float placeY, float CornerOneX,
			float CornerOneY, float CornerTwoX, float CornerTwoY);

		/* Returns the bending energy.
		* Input: void
		* Output: vector<float>*
		*/
		vector<float>* getBendingEnergyVector();

		/* Returns a vector containing the different cracks and their
		* distances to the hitpoint.
		* Input: void
		* Output: vector<vector<float>* >* Returns a pointer to a vector
		* containing pointers to other vectors
		*/
		vector<vector<float>* >* getRadialDistance();

		/* Returns the vector that contains the crack angle for the different
		* cracks.
		* Input: void
		* Output: vector<float>* Returns a pointer to a vector
		*/
		vector<float>* getAngles();

		/* Returns the vector with the difference between the last radial
		* cracks.
		* Input: void
		* Output: float
		*/
		vector<float>* getSafeDistance();

		/* Returns the coordinates for the impact, X and Y respectively.
		* Input: void
		* Output: float
		*/
		float getX();
		float getY();
		
		private:
		
		/* Using a Weibull function to calculate the crack limit for the glas.
		* Input: void
		* Output: float
		*/
		float configureBreakLimit();
		
		/* Calculates and returns the real breaklimit for the glas,
		* considering the position of the impact.
		* Input: (hitsize) The amount of energy added to the glas.
		* (placeX, placeY) The coordinates of the impact.
		* (CornerOneX, CornerOneY, CornertTwoX, CornerTwoY) The
		* coordinates of two diagonal corners.
		* Output: float
		*/
		float realBreakLimit(float breakLimit, float placeX, float placeY, 
			float cornerOneX, float cornerOneY, float cornerTwoX, float cornerTwo);

		/* Calculates the number of cracks, considering the hitsize and the
		* real breaklimit. The number of cracks is stored in the global
		* variable numberOfCracks. Zero cracks means that the glas didn’t
		* break.
		* Input: (hitsize) The amount of energy added to the glas.
		* (actualBreakLimit) The breaklimit for the glas.
		* Output: void
		*/
		void crackGenerator(float hitsize, float actualBreakLimit);
		
		/* Calculates the angles for the different cracks. The angle between
		* two cracks is at least 12 degrees. When one angle is calculated,
		* another cracks gets this angle + 180 degrees to create an oposing
		* crack.
		* Input: (angleVector) The vector that will contain the angles.
		* Output: void
		*/
		void angleGenerator(vector<float>* angleVector);
		
		/* Creates an energy for each crack. Uses some randomisation. The
		* energy is divided into two different energies, bending energy and
		* impact energy.
		* Input: (bendingEnergyVector) The vector that the bending energy is
		* stored in.
		* (impactEnergyVector) The vector that the impact energy is
		* stored in.
		* Output: void
		*/
		void energyGenerator(vector<float>* bendingEnergyVector,
			vector<float>* impactEnergyVector);
		
		/* Creates a vector with distances to each radialcrack for each crack
		* and a vector with a safe distance to next crack.
		* Input: (radialCrackVector) A pointer to a vector that for each
		* crack containing points to other vectors in which the radial
		* distances is stores.
		* (safeDistance) A pointer to a vector containing safe
		* distances to next crack.
		* Output: void
		*/
		void radialCrackGenerator(vector<vector<float>* >*& radialCrackVector,
			vector<float>*& safeDistance);
		
		int weibull; //A number between 1-20, every number symbols five percent.
		float breakLimit; //Our windows break limit in the centre of the window.
		float actualBreakLimit; //Break limit where the hit stroke.
		float centerX; //Our windows center point.
		float centerY;
		float distanceToCenter; //The distance from the hit to the center
		float maxDifference; //The maximum difference between angles
		float placeX; //Hit position.
		float placeY;
		float BREAK_LIMIT; //how much the position change the break limit.
		float CRACK_NUMBER; //How many cracks will errupt due to the energy.
		float ENERGY_DIFFER; //How much will the energy differ between the cracks.
		int NORMAL_BREAK; //A window normal break limit.
		int BENDING_INTERVAL; //The energy interval where only bending accurs.
		int CROSS_INTERVAL; //The energy interval where both bending and impact accurs.
		float RADIAL_FACTOR; //Decides the number of cracks.
		int MAX_RADIALS; //Maximum number of radials.
		float DISTANCE_FACTOR; //A distance factor to determine the radial cracks.
		int MAX_CRACKS; //The maximun number of cracks.
		int MIN_CRACK_DIFFERENCE; //minimum difference between the cracks.
		vector<float>* angleVector;
		vector<float>* safeDistance;
		vector<float>* bendingEnergyVector;
		vector<float>* impactEnergyVector;
		vector<vector<float>* >* radialCrackVector;
		int numberOfCracks;
		float crackEnergy;
	};
}
#endif
