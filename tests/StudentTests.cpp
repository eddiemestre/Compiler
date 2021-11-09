#include "catch.hpp"
#include "SrcMain.h"
#include <string>

// Helper function declarations (don't change these)
extern bool CheckFileMD5(const std::string& fileName, const std::string& expected);
extern bool CheckTextFilesSame(const std::string& fileNameA, 
	const std::string& fileNameB);

//TEST_CASE("Student Grammar Tests", "[student]")
//{
//	SECTION("Test01")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/test01.pcc"
//		};
//		REQUIRE(ProcessCommandArgs(2, argv) == 0);
//	}
//	SECTION("Test02")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/test02.pcc"
//		};
//		REQUIRE(ProcessCommandArgs(2, argv) == 0);
//	}
//	SECTION("Test03")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/test03.pcc"
//		};
//		REQUIRE(ProcessCommandArgs(2, argv) == 0);
//	}
//	SECTION("Test04")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/test04.pcc"
//		};
//		REQUIRE(ProcessCommandArgs(2, argv) == 0);
//	}
//	SECTION("Test05")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/test05.pcc"
//		};
//		REQUIRE(ProcessCommandArgs(2, argv) == 0);
//	}
//	SECTION("Test06")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/test06.pcc"
//		};
//		REQUIRE(ProcessCommandArgs(2, argv) == 0);
//	}
//	SECTION("Fibonacci")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/fibonacci.pcc"
//		};
//		REQUIRE(ProcessCommandArgs(2, argv) == 0);
//	}
//	SECTION("Star")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/star.pcc"
//		};
//		REQUIRE(ProcessCommandArgs(2, argv) == 0);
//	}
//}

//TEST_CASE("Student AST Tests", "[student]")
//{
//	SECTION("Test01")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/test01.pcc",
//			"ast"
//		};
//		REQUIRE(ProcessCommandArgs(3, argv) == 0);
//		bool result = CheckTextFilesSame("ast.txt", "expected/test01.ast.txt");
//		REQUIRE(result);
//	}
//	SECTION("Test02")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/test02.pcc",
//			"ast"
//		};
//		REQUIRE(ProcessCommandArgs(3, argv) == 0);
//		bool result = CheckTextFilesSame("ast.txt", "expected/test02.ast.txt");
//		REQUIRE(result);
//	}
//	SECTION("Test03")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/test03.pcc",
//			"ast"
//		};
//		REQUIRE(ProcessCommandArgs(3, argv) == 0);
//		bool result = CheckTextFilesSame("ast.txt", "expected/test03.ast.txt");
//		REQUIRE(result);
//	}
//	SECTION("Test04")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/test04.pcc",
//			"ast"
//		};
//		REQUIRE(ProcessCommandArgs(3, argv) == 0);
//		bool result = CheckTextFilesSame("ast.txt", "expected/test04.ast.txt");
//		REQUIRE(result);
//	}
//	SECTION("Test05")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/test05.pcc",
//			"ast"
//		};
//		REQUIRE(ProcessCommandArgs(3, argv) == 0);
//		bool result = CheckTextFilesSame("ast.txt", "expected/test05.ast.txt");
//		REQUIRE(result);
//	}
//	SECTION("Test06")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/test06.pcc",
//			"ast"
//		};
//		REQUIRE(ProcessCommandArgs(3, argv) == 0);
//		bool result = CheckTextFilesSame("ast.txt", "expected/test06.ast.txt");
//		REQUIRE(result);
//	}
//	SECTION("Fibonacci")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/fibonacci.pcc",
//			"ast"
//		};
//		REQUIRE(ProcessCommandArgs(3, argv) == 0);
//		bool result = CheckTextFilesSame("ast.txt", "expected/fibonacci.ast.txt");
//		REQUIRE(result);
//	}
//	SECTION("Star")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/star.pcc",
//			"ast"
//		};
//		REQUIRE(ProcessCommandArgs(3, argv) == 0);
//		bool result = CheckTextFilesSame("ast.txt", "expected/star.ast.txt");
//		REQUIRE(result);
//	}
//}

//TEST_CASE("Student Emit Tests", "[student]")
//{
//	SECTION("Test01")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/test01.pcc",
//			"emit"
//		};
//		REQUIRE(ProcessCommandArgs(3, argv) == 0);
//		bool result = CheckTextFilesSame("emit.txt", "expected/test01.emit.txt");
//		REQUIRE(result);
//	}
//	SECTION("Test02")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/test02.pcc",
//			"emit"
//		};
//		REQUIRE(ProcessCommandArgs(3, argv) == 0);
//		bool result = CheckTextFilesSame("emit.txt", "expected/test02.emit.txt");
//		REQUIRE(result);
//	}
//	SECTION("Test03")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/test03.pcc",
//			"emit"
//		};
//		REQUIRE(ProcessCommandArgs(3, argv) == 0);
//		bool result = CheckTextFilesSame("emit.txt", "expected/test03.emit.txt");
//		REQUIRE(result);
//	}
//	SECTION("Test04")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/test04.pcc",
//			"emit"
//		};
//		REQUIRE(ProcessCommandArgs(3, argv) == 0);
//		bool result = CheckTextFilesSame("emit.txt", "expected/test04.emit.txt");
//		REQUIRE(result);
//	}
//	SECTION("Test05")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/test05.pcc",
//			"emit"
//		};
//		REQUIRE(ProcessCommandArgs(3, argv) == 0);
//		bool result = CheckTextFilesSame("emit.txt", "expected/test05.emit.txt");
//		REQUIRE(result);
//	}
//	SECTION("Test06")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/test06.pcc",
//			"emit"
//		};
//		REQUIRE(ProcessCommandArgs(3, argv) == 0);
//		bool result = CheckTextFilesSame("emit.txt", "expected/test06.emit.txt");
//		REQUIRE(result);
//	}
//	SECTION("Fibonacci")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/fibonacci.pcc",
//			"emit"
//		};
//		REQUIRE(ProcessCommandArgs(3, argv) == 0);
//		bool result = CheckTextFilesSame("emit.txt", "expected/fibonacci.emit.txt");
//		REQUIRE(result);
//	}
//	SECTION("Star")
//	{
//		const char* argv[] = {
//			"tests/tests",
//			"input/star.pcc",
//			"emit"
//		};
//		REQUIRE(ProcessCommandArgs(3, argv) == 0);
//		bool result = CheckTextFilesSame("emit.txt", "expected/star.emit.txt");
//		REQUIRE(result);
//	}
//}
//
TEST_CASE("Student Register Allocation Tests", "[student]")
{
	SECTION("Test02")
	{
		const char* argv[] = {
			"tests/tests",
			"input/test02.pcc",
			"reg"
		};
		REQUIRE(ProcessCommandArgs(3, argv) == 0);
		bool resultReg = CheckTextFilesSame("reg.txt", "expected/test02.reg.txt");
		REQUIRE(resultReg);
		bool resultEmit = CheckTextFilesSame("emit.txt", "expected/test02.reg.emit.txt");
		REQUIRE(resultEmit);
	}
	SECTION("Test03")
	{
		const char* argv[] = {
			"tests/tests",
			"input/test03.pcc",
			"reg"
		};
		REQUIRE(ProcessCommandArgs(3, argv) == 0);
		bool resultReg = CheckTextFilesSame("reg.txt", "expected/test03.reg.txt");
		REQUIRE(resultReg);
		bool resultEmit = CheckTextFilesSame("emit.txt", "expected/test03.reg.emit.txt");
		REQUIRE(resultEmit);
	}
	SECTION("Test04")
	{
		const char* argv[] = {
			"tests/tests",
			"input/test04.pcc",
			"reg"
		};
		REQUIRE(ProcessCommandArgs(3, argv) == 0);
		bool resultReg = CheckTextFilesSame("reg.txt", "expected/test04.reg.txt");
		REQUIRE(resultReg);
		bool resultEmit = CheckTextFilesSame("emit.txt", "expected/test04.reg.emit.txt");
		REQUIRE(resultEmit);
	}
	SECTION("Test05")
	{
		const char* argv[] = {
			"tests/tests",
			"input/test05.pcc",
			"reg"
		};
		REQUIRE(ProcessCommandArgs(3, argv) == 0);
		bool resultReg = CheckTextFilesSame("reg.txt", "expected/test05.reg.txt");
		REQUIRE(resultReg);
		bool resultEmit = CheckTextFilesSame("emit.txt", "expected/test05.reg.emit.txt");
		REQUIRE(resultEmit);
	}
	SECTION("Test06")
	{
		const char* argv[] = {
			"tests/tests",
			"input/test06.pcc",
			"reg"
		};
		REQUIRE(ProcessCommandArgs(3, argv) == 0);
		bool resultReg = CheckTextFilesSame("reg.txt", "expected/test06.reg.txt");
		REQUIRE(resultReg);
		bool resultEmit = CheckTextFilesSame("emit.txt", "expected/test06.reg.emit.txt");
		REQUIRE(resultEmit);
	}
	SECTION("Fibonacci")
	{
		const char* argv[] = {
			"tests/tests",
			"input/fibonacci.pcc",
			"reg"
		};
		REQUIRE(ProcessCommandArgs(3, argv) == 0);
		bool resultReg = CheckTextFilesSame("reg.txt", "expected/fibonacci.reg.txt");
		REQUIRE(resultReg);
		bool resultEmit = CheckTextFilesSame("emit.txt", "expected/fibonacci.reg.emit.txt");
		REQUIRE(resultEmit);
	}
	SECTION("Star")
	{
		const char* argv[] = {
			"tests/tests",
			"input/star.pcc",
			"reg"
		};
		REQUIRE(ProcessCommandArgs(3, argv) == 0);
		bool resultReg = CheckTextFilesSame("reg.txt", "expected/star.reg.txt");
		REQUIRE(resultReg);
		bool resultEmit = CheckTextFilesSame("emit.txt", "expected/star.reg.emit.txt");
		REQUIRE(resultEmit);
	}
}
