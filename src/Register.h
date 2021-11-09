#pragma once

#include <iostream>
#include "Node.h"
#include <fstream>

// Defines Register class
class Register {
public:
	Register() = default;

	void GenerateIntervals(CodeContext& program, std::ofstream& reg);

	void LinearScan(CodeContext& program, std::ofstream& reg, std::map<std::string, std::pair<int, int>> map, int max);

private:
	std::pair<std::string, std::string> realRegisters;
};
