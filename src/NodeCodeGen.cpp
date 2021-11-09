#include "Node.h"
#include "parser.hpp"
#include <algorithm>

void NBlock::CodeGen(CodeContext& context)
{
	for (auto& i : mStatements) {
		i->CodeGen(context);
	}
}

void NData::CodeGen(CodeContext& context)
{
	for (auto& i : mDecls) {
		i->CodeGen(context);
	}
}

void NProgram::CodeGen(CodeContext& context)
{
	mData->CodeGen(context);
	mMain->CodeGen(context);
	// at this point, the final stage is to exit the program
	Ops pro("exit");
	context.opsVector.emplace_back(pro);
}

void NNumeric::CodeGen(CodeContext& context)
{
}

void NVarDecl::CodeGen(CodeContext& context)
{
	// add push operation for new variable
	Ops var("push");
	var.params.emplace_back("r0");
	context.opsVector.emplace_back(var);
	
	// place the variable into the map and increase the stack counter
	context.varTracker[mName] = context.lastStackIndex;
	context.lastStackIndex++;
	
}

void NArrayDecl::CodeGen(CodeContext& context)
{
	// add push operation for every element in array
	for (int i = 0; i < mSize->GetValue(); i++) {
		Ops var("push");
		var.params.emplace_back("r0");
		context.opsVector.emplace_back(var);
		context.lastStackIndex++;
	}
	// add to map of variables
	context.varTracker[mName] = context.lastStackIndex - mSize->GetValue();
	
}

void NNumericExpr::CodeGen(CodeContext& context)
{
	// assigns the current register to the numeric
	std::string val = std::to_string(mNumeric->GetValue());
	mResultRegister = "%" + std::to_string(context.lastVRegIndex);
	context.lastVRegIndex++;
	Ops num("movi");
	num.params.emplace_back(mResultRegister);
	num.params.emplace_back(val);
	context.opsVector.emplace_back(num);
}

void NVarExpr::CodeGen(CodeContext& context)
{
	// loads the necessary value from the stack
	Ops var("loadi");
	mResultRegister = "%" + std::to_string(context.lastVRegIndex);
	context.lastVRegIndex++;
	var.params.emplace_back(mResultRegister);
	var.params.emplace_back(std::to_string(context.varTracker.find(mName)->second));
	context.opsVector.emplace_back(var);
}

void NBinaryExpr::CodeGen(CodeContext& context)
{
	mLhs->CodeGen(context);
	mRhs->CodeGen(context);
	// addition
	if (mType == TADD) {
		Ops bin("add");
		mResultRegister = "%" + std::to_string(context.lastVRegIndex);
		context.lastVRegIndex++;
		bin.params.emplace_back(mResultRegister);
		bin.params.emplace_back(mLhs->GetResultRegister());
		bin.params.emplace_back(mRhs->GetResultRegister());
		context.opsVector.emplace_back(bin);
	}
	// multiplication
	if (mType == TMUL) {
		Ops bin("mul");
		mResultRegister = "%" + std::to_string(context.lastVRegIndex);
		context.lastVRegIndex++;
		bin.params.emplace_back(mResultRegister);
		bin.params.emplace_back(mLhs->GetResultRegister());
		bin.params.emplace_back(mRhs->GetResultRegister());
		context.opsVector.emplace_back(bin);
	}
	// subtraction
	if (mType == TSUB) {
		Ops bin("sub");
		mResultRegister = "%" + std::to_string(context.lastVRegIndex);
		context.lastVRegIndex++;
		bin.params.emplace_back(mResultRegister);
		bin.params.emplace_back(mLhs->GetResultRegister());
		bin.params.emplace_back(mRhs->GetResultRegister());
		context.opsVector.emplace_back(bin);
	}
	// division
	if (mType == TDIV) {
		Ops bin("div");
		mResultRegister = "%" + std::to_string(context.lastVRegIndex);
		context.lastVRegIndex++;
		bin.params.emplace_back(mResultRegister);
		bin.params.emplace_back(mLhs->GetResultRegister());
		bin.params.emplace_back(mRhs->GetResultRegister());
		context.opsVector.emplace_back(bin);
	}


}

void NArrayExpr::CodeGen(CodeContext& context)
{
	// grab the value from an index of the array
	mSubscript->CodeGen(context);
	mResultRegister = "%" + std::to_string(context.lastVRegIndex);
	std::string mov = mResultRegister;
	context.lastVRegIndex++;
	int base = context.varTracker.find(mName)->second;
	Ops arrEx("movi");
	arrEx.params.emplace_back(mov);
	arrEx.params.emplace_back(std::to_string(base));
	context.opsVector.emplace_back(arrEx);

	//calculate offset
	std::string tempo = mSubscript->GetResultRegister();
	std::string s = tempo.substr(1);
	int a = std::stoi(s);
	int address = a + base;

	// add values
	Ops add("add");
	mResultRegister = "%" + std::to_string(context.lastVRegIndex);
	std::string addReg = mResultRegister;
	context.lastVRegIndex++;
	add.params.emplace_back(mResultRegister);
	std::string addressstr = "%" + std::to_string(address);
	add.params.emplace_back(mov);
	add.params.emplace_back(tempo);
	context.opsVector.emplace_back(add);

	// load a value from the stack
	Ops load("load");
	mResultRegister = "%" + std::to_string(context.lastVRegIndex);
	context.lastVRegIndex++;
	load.params.emplace_back(mResultRegister);
	load.params.emplace_back(addReg);
	context.opsVector.emplace_back(load);
	
}

void NAssignVarStmt::CodeGen(CodeContext& context)
{
	mRhs->CodeGen(context);

	// store a register of data on the stack
	Ops varSt("storei");
	varSt.params.emplace_back(std::to_string(context.varTracker.find(mName)->second));
	varSt.params.emplace_back(mRhs->GetResultRegister());
	context.opsVector.emplace_back(varSt);


}

void NAssignArrayStmt::CodeGen(CodeContext& context)
{
	mRhs->CodeGen(context);
	mSubscript->CodeGen(context);
	// grab the value from an index of the array
	std::string mov = "%" + std::to_string(context.lastVRegIndex);
	context.lastVRegIndex++;
	std::string base1 = std::to_string(context.varTracker.find(mName)->second);

	// set a value to a register
	Ops first("movi");
	first.params.emplace_back(mov);
	first.params.emplace_back(base1);
	context.opsVector.emplace_back(first);


	// calculate base/offset
	int base = context.varTracker.find(mName)->second;
	std::string tempo = mSubscript->GetResultRegister();
	std::string s = tempo.substr(1);
	int a = std::stoi(s);
	int address = a + base;

	std::string add = "%" + std::to_string(context.lastVRegIndex);
	context.lastVRegIndex++;
	std::string addressstr = "%" + std::to_string(address);

	// add two register values together
	Ops second("add");
	second.params.emplace_back(add);
	second.params.emplace_back(mov);
	second.params.emplace_back(tempo);
	context.opsVector.emplace_back(second);

	// store the value of a second register in the space of a first register
	Ops arr("store");
	arr.params.emplace_back(add);
	arr.params.emplace_back(mRhs->GetResultRegister());
	context.opsVector.emplace_back(arr);
}

void NIncStmt::CodeGen(CodeContext& context)
{
	// retreive data from stack
	Ops load("loadi");
	std::string resultReg = "%" + std::to_string(context.lastVRegIndex);
	context.lastVRegIndex++;
	load.params.emplace_back(resultReg);
	load.params.emplace_back(std::to_string(context.varTracker.find(mName)->second));
	context.opsVector.emplace_back(load);

	// increment
	Ops inc("inc");
	//context.varTracker[mName] = val;
	inc.params.emplace_back(resultReg);
	context.opsVector.emplace_back(inc);

	// store new value
	Ops store("storei");
	store.params.emplace_back(std::to_string(context.varTracker.find(mName)->second));
	store.params.emplace_back(resultReg);
	context.opsVector.emplace_back(store);


}

void NDecStmt::CodeGen(CodeContext& context)
{
	// retreive data from stack
	Ops load("loadi");
	std::string resultReg = "%" + std::to_string(context.lastVRegIndex);
	context.lastVRegIndex++;
	load.params.emplace_back(resultReg);
	load.params.emplace_back(std::to_string(context.varTracker.find(mName)->second));
	context.opsVector.emplace_back(load);

	// decrement
	Ops dec("dec");
	//context.varTracker[mName] = val;
	dec.params.emplace_back(resultReg);
	context.opsVector.emplace_back(dec);

	// store new value
	Ops store("storei");
	store.params.emplace_back(std::to_string(context.varTracker.find(mName)->second));
	store.params.emplace_back(resultReg);
	context.opsVector.emplace_back(store);
}

void NComparison::CodeGen(CodeContext& context)
{
	mLhs->CodeGen(context);
	mRhs->CodeGen(context);
	
	// equals
	if (mType == TISEQUAL) {
		Ops cmpeq("cmpeq");
		std::string l = mLhs->GetResultRegister();
		std::string r = mRhs->GetResultRegister();
		cmpeq.params.emplace_back(l);
		cmpeq.params.emplace_back(r);
		context.opsVector.emplace_back(cmpeq);
	}
	// less
	if (mType == TLESS) {
		Ops cmplt("cmplt");
		std::string l = mLhs->GetResultRegister();
		std::string r = mRhs->GetResultRegister();
		cmplt.params.emplace_back(l);
		cmplt.params.emplace_back(r);
		context.opsVector.emplace_back(cmplt);
	}


}

void NIfStmt::CodeGen(CodeContext& context)
{

	mComp->CodeGen(context);

	// set data to register
	Ops movi("movi");
	std::string first = "%" + std::to_string(context.lastVRegIndex);
	context.lastVRegIndex++;
	std::string qu = "??";
	movi.params.emplace_back(first);
	movi.params.emplace_back(qu);
	context.opsVector.emplace_back(movi);
	int temp = context.opsVector.size();

	// if false, sets the pc to the reg
	Ops jnt("jnt");
	jnt.params.emplace_back(first);
	context.opsVector.emplace_back(jnt);


	// if block
	mIfBlock->CodeGen(context);


	// no else block
	if (mElseBlock == nullptr) {
		qu = std::to_string(context.opsVector.size());
		context.opsVector[temp - 1].params[1] = qu;
	}
	// else block
	else {
		// movi
		Ops elsemov("movi");
		std::string elsefi = "%" + std::to_string(context.lastVRegIndex);
		context.lastVRegIndex++;
		std::string re = "??";
		elsemov.params.emplace_back(elsefi);
		elsemov.params.emplace_back(re);
		context.opsVector.emplace_back(elsemov);
		int tempelse = context.opsVector.size();
		
		// jmp
		// sets pc to the reg
		Ops jmp("jmp");
		jmp.params.emplace_back(elsefi);
		context.opsVector.emplace_back(jmp);

		// fix up 1st address
		std::string fStr = std::to_string(context.opsVector.size());
		context.opsVector[temp - 1].params[1] = fStr;

		// else
		mElseBlock->CodeGen(context);

		// fix up 2nd address
		std::string sStr = std::to_string(context.opsVector.size());
		context.opsVector[tempelse - 1].params[1] = sStr;
	}
}

void NWhileStmt::CodeGen(CodeContext& context)
{
	int orig = context.opsVector.size();
	mComp->CodeGen(context);

	Ops mov("movi");
	std::string first = "%" + std::to_string(context.lastVRegIndex);
	context.lastVRegIndex++;
	std::string qu = "??";
	mov.params.emplace_back(first);
	mov.params.emplace_back(qu);
	context.opsVector.emplace_back(mov);
	int temp = context.opsVector.size();

	Ops jnt("jnt");
	jnt.params.emplace_back(first);
	context.opsVector.emplace_back(jnt);

	mBlock->CodeGen(context);

	Ops mov2("movi");
	std::string second = "%" + std::to_string(context.lastVRegIndex);
	context.lastVRegIndex++;
	mov2.params.emplace_back(second);
	mov2.params.emplace_back(std::to_string(orig));
	context.opsVector.emplace_back(mov2);

	Ops jmp("jmp");
	jmp.params.emplace_back(second);
	context.opsVector.emplace_back(jmp);

	std::string sStr = std::to_string(context.opsVector.size());
	context.opsVector[temp - 1].params[1] = sStr;
}

void NPenUpStmt::CodeGen(CodeContext& context)
{
	Ops penUp("penup");
	context.opsVector.emplace_back(penUp);
}

void NPenDownStmt::CodeGen(CodeContext& context)
{
	Ops penUp("pendown");
	context.opsVector.emplace_back(penUp);
}

void NSetPosStmt::CodeGen(CodeContext& context)
{
	// genereate x, y expr
	mXExpr->CodeGen(context);
	mYExpr->CodeGen(context);

	Ops movTx("mov");
	movTx.params.emplace_back("tx");
	movTx.params.emplace_back(mXExpr->GetResultRegister());
	context.opsVector.emplace_back(movTx);

	Ops movTy("mov");
	movTy.params.emplace_back("ty");
	movTy.params.emplace_back(mYExpr->GetResultRegister());
	context.opsVector.emplace_back(movTy);
}

void NSetColorStmt::CodeGen(CodeContext& context)
{
	mColor->CodeGen(context);

	Ops movTc("mov");
	movTc.params.emplace_back("tc");
	movTc.params.emplace_back(mColor->GetResultRegister());
	context.opsVector.emplace_back(movTc);
}

void NFwdStmt::CodeGen(CodeContext& context)
{
	mParam->CodeGen(context);

	Ops fwd("fwd");
	fwd.params.emplace_back(mParam->GetResultRegister());
	context.opsVector.emplace_back(fwd);
}

void NBackStmt::CodeGen(CodeContext& context)
{
	mParam->CodeGen(context);

	Ops back("back");
	back.params.emplace_back(mParam->GetResultRegister());
	context.opsVector.emplace_back(back);
}

void NRotStmt::CodeGen(CodeContext& context)
{
	mParam->CodeGen(context);

	Ops add("add");
	add.params.emplace_back("tr");
	add.params.emplace_back("tr");
	add.params.emplace_back(mParam->GetResultRegister());
	context.opsVector.emplace_back(add);
}
