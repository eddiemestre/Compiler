#pragma once
#include <vector>
#include <string>
#include <ostream>
#include <map>

// Operations Struct
// This will be used to define the various instructions
struct Ops
{
	std::string op;
	std::vector<std::string> params;

	Ops(std::string str) {
		op = str;
	}
};

// CodeContext
// contains the data needed to store instructions and
// track locations of variables/arrays on stack
struct CodeContext
{
	//  vector to store generated instructions
	std::vector<Ops> opsVector;

	// count of the last virtual register index used
	int lastVRegIndex = 0;

	// count of the last stack index used
	int lastStackIndex = 0;

	// map to track which variables/arrays correspond to which indices on the stack
	std::map<std::string, int> varTracker;
	


	CodeContext() = default;

};

// Node class definition
class Node
{
public:
	virtual void OutputAST(std::ostream& stream, int depth) const = 0;
	virtual void CodeGen(CodeContext& context) = 0;
protected:
	void OutputMargin(std::ostream& stream, int depth) const;
};

class NDecl : public Node
{
};

// Data definition
class NData : public Node
{
public:
	void OutputAST(std::ostream& stream, int depth) const override;
	void CodeGen(CodeContext& context) override;
	void AddDecl(NDecl* decl);
private:
	std::vector<NDecl*> mDecls;
};

class NStatement : public Node
{
};

// Block Definition
class NBlock : public Node
{
public:
	void OutputAST(std::ostream& stream, int depth) const override;
	void CodeGen(CodeContext& context) override;
	void AddStatement(NStatement* statement);
private:
	std::vector<NStatement*> mStatements;
};

// Program Definition
class NProgram : public Node
{
public:
	NProgram(NData* data, NBlock* main)
		:mData(data)
		,mMain(main)
	{ }
	void OutputAST(std::ostream& stream, int depth) const override;
	void CodeGen(CodeContext& context) override;
private:
	NData* mData;
	NBlock* mMain;
};

// Numeric Definition
class NNumeric : public Node
{
public:
	NNumeric(std::string& value)
		:mValue(std::stoi(value))
	{ }
	void OutputAST(std::ostream& stream, int depth) const override;
	void CodeGen(CodeContext& context) override;

	int GetValue() const { return mValue; }
private:
	int mValue;
};

// Variable Declaration Definition
class NVarDecl : public NDecl
{
public:
	NVarDecl(std::string& name)
		:mName(name)
	{ }
	void OutputAST(std::ostream& stream, int depth) const override;
	void CodeGen(CodeContext& context) override;
private:
	std::string mName;
};

// Array Declaration Definition
class NArrayDecl : public NDecl
{
public:
	NArrayDecl(std::string& name, NNumeric* size)
		:mName(name)
		,mSize(size)
	{ }
	void OutputAST(std::ostream& stream, int depth) const override;
	void CodeGen(CodeContext& context) override;
private:
	std::string mName;
	NNumeric* mSize;
};

// Expression Definition
class NExpr : public Node
{
public:
	const std::string& GetResultRegister() const { return mResultRegister; }
protected:
	std::string mResultRegister;
};

// Numeric Exprression Definition
class NNumericExpr : public NExpr
{
public:
	NNumericExpr(NNumeric* num)
		:mNumeric(num)
	{ }
	void OutputAST(std::ostream& stream, int depth) const override;
	void CodeGen(CodeContext& context) override;
private:
	NNumeric* mNumeric;
};

// Variable Expression Definition
class NVarExpr : public NExpr
{
public:
	NVarExpr(std::string& name)
		:mName(name)
	{ }
	void OutputAST(std::ostream& stream, int depth) const override;
	void CodeGen(CodeContext& context) override;
private:
	std::string mName;
};

// Binary Expression Definition
class NBinaryExpr : public NExpr
{
public:
	NBinaryExpr(NExpr* lhs, int type, NExpr* rhs)
		:mLhs(lhs)
		,mRhs(rhs)
		,mType(type)
	{ }
	void OutputAST(std::ostream& stream, int depth) const override;
	void CodeGen(CodeContext& context) override;
private:
	NExpr* mLhs;
	NExpr* mRhs;
	int mType;
};

// Array Expression Definition Definition
class NArrayExpr : public NExpr
{
public:
	NArrayExpr(std::string& name, NExpr* subscript)
		:mName(name)
		,mSubscript(subscript)
	{ }
	void OutputAST(std::ostream& stream, int depth) const override;
	void CodeGen(CodeContext& context) override;
private:
	std::string mName;
	NExpr* mSubscript;
};

// Variable Assignment Statement Definition
class NAssignVarStmt : public NStatement
{
public:
	NAssignVarStmt(std::string& name, NExpr* rhs)
		:mName(name)
		,mRhs(rhs)
	{ }
	void OutputAST(std::ostream& stream, int depth) const override;
	void CodeGen(CodeContext& context) override;
private:
	std::string mName;
	NExpr* mRhs;
};

// Array Assignment Statement Definition
class NAssignArrayStmt : public NStatement
{
public:
	NAssignArrayStmt(std::string& name, NExpr* subscript, NExpr* rhs)
		:mName(name)
		,mSubscript(subscript)
		,mRhs(rhs)
	{ }
	void OutputAST(std::ostream& stream, int depth) const override;
	void CodeGen(CodeContext& context) override;
private:
	std::string mName;
	NExpr* mSubscript;
	NExpr* mRhs;
};

// Increment Definition ++ 
class NIncStmt : public NStatement
{
public:
	NIncStmt(std::string& name)
		:mName(name)
	{ }
	void OutputAST(std::ostream& stream, int depth) const override;
	void CodeGen(CodeContext& context) override;
private:
	std::string mName;
};

// Decrement Definition --
class NDecStmt : public NStatement
{
public:
	NDecStmt(std::string& name)
		:mName(name)
	{ }
	void OutputAST(std::ostream& stream, int depth) const override;
	void CodeGen(CodeContext& context) override;
private:
	std::string mName;
};

// Comparison Definition
class NComparison : public Node
{
public:
	NComparison(NExpr* lhs, int type, NExpr* rhs)
		:mLhs(lhs)
		,mRhs(rhs)
		,mType(type)
	{ }
	void OutputAST(std::ostream& stream, int depth) const override;
	void CodeGen(CodeContext& context) override;
private:
	NExpr* mLhs;
	NExpr* mRhs;
	int mType;
};

// If Statement Definition
class NIfStmt : public NStatement
{
public:
	NIfStmt(NComparison* comp, NBlock* ifBlock, NBlock* elseBlock)
		:mComp(comp)
		,mIfBlock(ifBlock)
		,mElseBlock(elseBlock)
	{ }
	void OutputAST(std::ostream& stream, int depth) const override;
	void CodeGen(CodeContext& context) override;
private:
	NComparison* mComp;
	NBlock* mIfBlock;
	NBlock* mElseBlock;
};

// While Statement Definition
class NWhileStmt : public NStatement
{
public:
	NWhileStmt(NComparison* comp, NBlock* block)
		:mComp(comp)
		,mBlock(block)
	{ }
	void OutputAST(std::ostream& stream, int depth) const override;
	void CodeGen(CodeContext& context) override;
private:
	NComparison* mComp;
	NBlock* mBlock;
};

// Pen Up Statement Definition
class NPenUpStmt : public NStatement
{
public:
	void OutputAST(std::ostream& stream, int depth) const override;
	void CodeGen(CodeContext& context) override;
};

// Pen Down Statement Definition
class NPenDownStmt : public NStatement
{
public:
	void OutputAST(std::ostream& stream, int depth) const override;
	void CodeGen(CodeContext& context) override;
};

// Set Position Statement Definition
class NSetPosStmt : public NStatement
{
public:
	NSetPosStmt(NExpr* x, NExpr* y)
		:mXExpr(x)
		,mYExpr(y)
	{ }
	void OutputAST(std::ostream& stream, int depth) const override;
	void CodeGen(CodeContext& context) override;
private:
	NExpr* mXExpr;
	NExpr* mYExpr;
};

// Set Color Statement Definition
class NSetColorStmt : public NStatement
{
public:
	NSetColorStmt(NExpr* color)
		:mColor(color)
	{ }
	void OutputAST(std::ostream& stream, int depth) const override;
	void CodeGen(CodeContext& context) override;
private:
	NExpr* mColor;
};

// Move Pen Forward Statement Definition
class NFwdStmt : public NStatement
{
public:
	NFwdStmt(NExpr* param)
		:mParam(param)
	{ }
	void OutputAST(std::ostream& stream, int depth) const override;
	void CodeGen(CodeContext& context) override;
private:
	NExpr* mParam;
};

// Move Pen Back Statement Definition
class NBackStmt : public NStatement
{
public:
	NBackStmt(NExpr* param)
		:mParam(param)
	{ }
	void OutputAST(std::ostream& stream, int depth) const override;
	void CodeGen(CodeContext& context) override;
private:
	NExpr* mParam;
};

// Rotate Pen Statement Definition
class NRotStmt : public NStatement
{
public:
	NRotStmt(NExpr* param)
		:mParam(param)
	{ }
	void OutputAST(std::ostream& stream, int depth) const override;
	void CodeGen(CodeContext& context) override;
private:
	NExpr* mParam;
};
