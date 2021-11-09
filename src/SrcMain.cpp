#include "SrcMain.h"
#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include "Node.h"
#include <fstream>
#include "Register.cpp"

extern int proccparse(); // NOLINT
struct yy_buffer_state; // NOLINT
extern void procc_flush_buffer(yy_buffer_state* b); // NOLINT
extern int procclex_destroy(); // NOLINT
extern FILE* proccin; // NOLINT

extern int gLineNumber;
extern NProgram* gProgram;
bool gSyntaxError = false;

// CHANGE ANYTHING ABOVE THIS LINE AT YOUR OWN RISK!!!!

// takes test cases from "StudentTests.cpp" and runs them
int ProcessCommandArgs(int argc, const char* argv[])
{
	gLineNumber = 1;
	gSyntaxError = false;
	if (argc < 2)
	{
		std::cout << "You must pass the input file as a command line parameter." << std::endl;
		return 1;
	}

	// Read input from the first command line parameter
	proccin = fopen(argv[1], "r");
	if (proccin == nullptr)
	{
		std::cout << "File not found: " << argv[1] << std::endl;
		return 1;
	}
	procc_flush_buffer(nullptr);

	// Start the parse. This is handled by the Bison Parser. Checks Grammar.
	proccparse();

	if (gProgram != nullptr && argc == 3)
	{

		// Part 2 - Generating the Abstract Syntax Tree. 
		// The Abstract Syntax Tree is the Intermediate Representation
		std::ofstream file;
		file.open("ast.txt");
		gProgram->OutputAST(file, 0);
		file.close();

		// Part 3 - Generating assembly code with virtual registers. 
 		std::string temp(argv[2]);
		if (temp.find("emit") != std::string::npos) {
			CodeContext c;
			gProgram->CodeGen(c);

			std::ofstream emit;
			emit.open("emit.txt");
			for (int i = 0; i < c.opsVector.size(); i++) {
				if (c.opsVector[i].op == "penup" || c.opsVector[i].op == "pendown") {
					emit << c.opsVector[i].op << '\n';
				}
				else {
					emit << c.opsVector[i].op << " ";
				}
				if (!c.opsVector[i].params.empty()) {
					for (int j = 0; j < c.opsVector[i].params.size() - 1; j++) {
						emit << c.opsVector[i].params[j] << ",";
					}
					emit << c.opsVector[i].params[c.opsVector[i].params.size() - 1];
					emit << '\n';
				}
				
			}
			emit.close();
		}

		// Part 4 - register allocation with set # of registers (7)
		if (temp.find("reg") != std::string::npos) {
			CodeContext g;
			// call CodeGen on root program node
			gProgram->CodeGen(g);

			Register reg1;

			std::ofstream oreg;
			oreg.open("reg.txt");

			oreg << "INTERVALS:" << '\n';
			// generate intervals for registers
			reg1.GenerateIntervals(g, oreg);

			oreg.close();
		}
	}
	else
	{
		// (Just a useful separator for debug cout statements in grammar)
		std::cout << "**********************************************\n";
	}

	// Close the file stream
	fclose(proccin);
	// Destroy lexer so it reinitializes
	procclex_destroy();
	// Return 1 if syntax error, 0 otherwise
	return static_cast<int>(gSyntaxError);
}

void proccerror(const char* s) // NOLINT
{
	std::cout << s << " on line " << gLineNumber << std::endl;
	gSyntaxError = true;
}
