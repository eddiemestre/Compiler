#include "Node.h"
#include "parser.hpp"

// This file outputs Node identities to the ast output file based on
// the symbol definitions in Procc.y

// new node identified
// lists additional nodes depending on their depth in the original code. 
// Ex: variables declared in an if statement are of greater 
// depth than variables declared before the if statement
void Node::OutputMargin(std::ostream& stream, int depth) const
{
	for (int i = 0; i < depth; i++)
	{
		stream << "--";
	}
}

// Block Node - Identified as one or more statements
void NBlock::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "Block\n";
	for (auto& stmt : mStatements)
	{
		stmt->OutputAST(stream, depth + 1);
		stream.flush();
	}
}

// Data definition (struct like object at top of code that declares all 
// variables we will use in main code)
void NData::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "Data\n";
	for (auto& decl : mDecls)
	{
		decl->OutputAST(stream, depth + 1);
	}
}

// Program definition (data + main/block)
void NProgram::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "Program\n";
	mData->OutputAST(stream, depth + 1);
	mMain->OutputAST(stream, depth + 1);
}

// Defines any collection of integers 
void NNumeric::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "Numeric: " << mValue << '\n';
}

// Variable Declaration 
void NVarDecl::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "VarDecl: " << mName << '\n';
}

// Array Declaration
void NArrayDecl::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "ArrayDecl: " << mName << '\n';
	mSize->OutputAST(stream, depth + 1);
}

// Numeric Expression (integer is part of expression)
void NNumericExpr::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "NumericExpr\n";
	mNumeric->OutputAST(stream, depth + 1);
}

// Variable Expression (defined variable is part of expression)
void NVarExpr::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "VarExpr: " << mName << '\n';
}

// Binary Expression (arithmetic)
void NBinaryExpr::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	
	char op = ' ';
	switch (mType)
	{
	case TADD:
		op = '+';
		break;
	case TSUB:
		op = '-';
		break;
	case TMUL:
		op = '*';
		break;
	case TDIV:
		op = '/';
		break;
	default:
		break;
	}

	stream << "BinaryExpr: " << op << '\n';
	mLhs->OutputAST(stream, depth + 1);
	mRhs->OutputAST(stream, depth + 1);
}

// Array Expression (defined array is part of expression)
void NArrayExpr::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "ArrayExpr: " << mName << '\n';
	mSubscript->OutputAST(stream, depth + 1);
}

// Var Assignment Statement
void NAssignVarStmt::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "AssignVarStmt: " << mName << '\n';
	mRhs->OutputAST(stream, depth + 1);
}

// Array Assignment Statement
void NAssignArrayStmt::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "AssignArrayStmt: " << mName << '\n';
	mSubscript->OutputAST(stream, depth + 1);
	mRhs->OutputAST(stream, depth + 1);
}

// Increment ++
void NIncStmt::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "IncStmt: " << mName << '\n';
}

// Decrement --
void NDecStmt::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "DecStmt: " << mName << '\n';
}

// Comparison Operators
void NComparison::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);

	std::string op;
	switch (mType)
	{
	case TLESS:
		op = "<";
		break;
	case TISEQUAL:
		op = "==";
		break;
	default:
		break;
	}

	stream << "Comparison: " << op << '\n';
	mLhs->OutputAST(stream, depth + 1);
	mRhs->OutputAST(stream, depth + 1);
}

// If Statement
void NIfStmt::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);

	if (mElseBlock == nullptr)
	{
		stream << "IfStmt (no else)\n";
	}
	else
	{
		stream << "IfStmt (with else)\n";
	}
	
	mComp->OutputAST(stream, depth + 1);
	mIfBlock->OutputAST(stream, depth + 1);
	if (mElseBlock != nullptr)
	{
		mElseBlock->OutputAST(stream, depth + 1);
	}
}

//While Statement
void NWhileStmt::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "WhileStmt\n";
	mComp->OutputAST(stream, depth + 1);
	mBlock->OutputAST(stream, depth + 1);
}

// Lifts Pen (for drawing to screen)
void NPenUpStmt::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "PenUpStmt\n";
}

// Drops Pen onto "canvas" (for drawing to screen)
void NPenDownStmt::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "PenDownStmt\n";
}

// Sets Position of Pen
void NSetPosStmt::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "SetPosStmt\n";
	mXExpr->OutputAST(stream, depth + 1);
	mYExpr->OutputAST(stream, depth + 1);
}

// Sets Color
void NSetColorStmt::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "SetColorStmt\n";
	mColor->OutputAST(stream, depth + 1);
}

// Pen Move forward statement
void NFwdStmt::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "FwdStmt\n";
	mParam->OutputAST(stream, depth + 1);
}

// Pen Back Statement
void NBackStmt::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "BackStmt\n";
	mParam->OutputAST(stream, depth + 1);
}

// Pen Rotate Statement
void NRotStmt::OutputAST(std::ostream& stream, int depth) const
{
	OutputMargin(stream, depth);
	stream << "RotStmt\n";
	mParam->OutputAST(stream, depth + 1);
}
