#include "Register.h"
#include <fstream>
#include <map>
#include <utility>

// Generates intervals for each virtual register
void Register::GenerateIntervals(CodeContext& program, std::ofstream& reg) {

	std::map<std::string, std::pair<int, int>> intervalMap;

	// find virtual registers in program
	for (int i = 0; i < program.opsVector.size(); i++) {
		for (int j = 0; j < program.opsVector[i].params.size(); j++) {
			std::string temp = program.opsVector[i].params[j];
			// VRs are identified by the '%'
			if (temp.find('%') != std::string::npos) {	// a VR is...
				// not found
				if (intervalMap.find(temp) == intervalMap.end()) {
					intervalMap[temp] = std::make_pair(i, i);
				}
				//found
				else {
					intervalMap.at(temp).second = i;
				}
			}
		}
	}

	int counter = 0;
	int max = 0;

	while (counter < intervalMap.size()) {
		std::string sub = "%" + std::to_string(counter);

		if (intervalMap.find(sub) != intervalMap.end()) {
			reg << sub << ":" << intervalMap.at(sub).first << "," << intervalMap.at(sub).second << '\n';
			counter++;
			max = intervalMap.at(sub).second;
		}

	}

	// run Linear scan algorithm
	LinearScan(program, reg, intervalMap, max);

}

// Linear Scan algorithm - creates the mapping from virtual registers to real registers
void Register::LinearScan(CodeContext& program, std::ofstream& reg, std::map<std::string, std::pair<int, int>> map, int max) {

	reg << "ALLOCATION:" << '\n';
	std::map<std::string, std::string> middle;
	std::map<std::string, bool> real;

	// prepopulate middle map
	for (int i = 0; i < map.size(); i++) {
		std::string count = "%" + std::to_string(i);
		middle[count] = "";
	}

	// prepopulate real map
	for (int i = 1; i < 8; i++) {
		std::string count = "r" + std::to_string(i);
		real[count] = true;
	}
	// do scanning

	int mainloop = 0;
	int step;
	int check = 0;
	std::map<std::string, int> noCome;
	while (mainloop < map.size()) {	// iterate over interval map

		int k = max;

		std::string mainsub = "%" + std::to_string(mainloop);

		// check intervals
		if (map.find(mainsub) != map.end()) {
			// get where address first appears
			step = map.at(mainsub).first;

			for (int i = 0; i < map.size(); i++) {
				for (int i = 0; i < max; i++) {
					std::string t = "%" + std::to_string(i);
					if (map.at(t).second == step || map.at(t).second < step) {	// expires

						std::string bye = middle.at(t);
						real.at(bye) = true;
					}
				}

				// VECTOR OF PAIRS JUST PUT A '%' BEFORE EVERY i WHEN WRITING
				// TO FILE. DONE. EASY
				// check if VR is already in list, if so, just incremement and 
				// continue, otherwise do the freeing and then add VR to list and 
				// increment
			}
				int realloop = 1;
				while (realloop < real.size() + 1) {
					std::string realsub = "r" + std::to_string(realloop);
					if ((real.find(realsub) != real.end())) {
						if (real.at(realsub)) {
							//register available
							middle[mainsub] = realsub;
							real.at(realsub) = false;
							realloop = real.size() + 1;
						}
					}
					realloop++;
				}
		}
		mainloop++;
		//step++;

	}




		// output reg
		int counter = 0;
		while (counter < middle.size()) {
			std::string sub = "%" + std::to_string(counter);

			if (middle.find(sub) != middle.end()) {
				reg << sub << ":" << middle.at(sub) << '\n';
				counter++;
			}
		}


}
