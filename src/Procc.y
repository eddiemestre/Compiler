%{
#include "Node.h"
#include <iostream>
#include <string>
extern int yylex();
extern void yyerror(const char* s);

// Global for the program
NProgram* gProgram = nullptr;

// Disable the warnings that Bison creates
#pragma warning(disable: 4065)
%}

/* You should not need to change the union */
%union {
    Node* node;
	NProgram* program;
	NData* data;
	NDecl* decl;
	NBlock* block;
	NStatement* statement;
	NNumeric* numeric;
	NExpr* expr;
	NComparison* comparison;
	std::string* string;
	int token;
}

%error-verbose

/* Tokens/Terminal symbols */
%token <token> TDATA TMAIN TLBRACE TRBRACE TSEMI TLPAREN TRPAREN
%token <token> TLBRACKET TRBRACKET TINC TDEC TEQUALS
%token <token> TADD TSUB TMUL TDIV
%token <token> TLESS TISEQUAL
%token <token> TVAR TARRAY
%token <token> TIF TELSE TWHILE
%token <token> TCOMMA TPENUP TPENDOWN TSETPOS TSETCOLOR TFWD TBACK TROT
%token <string> TINTEGER TIDENTIFIER

/* Types/non-terminal symbols */
%type <program> program
%type <data> data decls
%type <block> main block
%type <decl> decl
%type <numeric> numeric
%type <expr> expr
%type <statement> statement
%type <comparison> cndn

/* Operator precedence */
%left TADD TSUB
%left TMUL TDIV

%%

program		: data main 
				{ 
					std::cout << "Program\n";
					gProgram = new NProgram($1, $2);
				}
;

data		: TDATA TLBRACE TRBRACE
				{
					std::cout << "Data (no decls)\n";
					$$ = new NData();
				}
			| TDATA TLBRACE decls TRBRACE
				{
					std::cout << "Data\n";
					$$ = $3;
				}

;

decls		: decl 
				{
					std::cout << "Single decl\n";
					$$ = new NData();
					$$->AddDecl($1);
				}
			| decls decl
				{
					std::cout << "Multiple decls\n";
					$$->AddDecl($2);


				}
;

decl		: TVAR TIDENTIFIER TSEMI
				{
					std::cout << "Var declaration " << *($2) << '\n';
					$$ = new NVarDecl(*($2));
				}
			| TARRAY TIDENTIFIER TLBRACKET numeric TRBRACKET TSEMI
				{
					std::cout << "Array declaration " << *($2) << '\n';
					$$ = new NArrayDecl(*($2), $4);
				}
;

main		: TMAIN TLBRACE TRBRACE
				{
					std::cout << "Main (no stmts)\n";
					$$ = new NBlock();
				}
			| TMAIN TLBRACE block TRBRACE
				{
					std::cout << "Main\n";
					$$ = $3;
				}
;

statement		: TIDENTIFIER TEQUALS expr TSEMI
				{
					std::cout << "identifier assignment to expression\n";
					$$ = new NAssignVarStmt((*$1), $3);
				}
			| TINTEGER TEQUALS expr TSEMI
				{
					std::cout << "integer assignment to expression\n";
					$$ = new NAssignVarStmt(*$1, $3);
				}
			| TIDENTIFIER TLBRACKET expr TRBRACKET TEQUALS expr TSEMI
				{
					std::cout << "array index assignment\n";
					$$ = new NAssignArrayStmt(*$1, $3, $6);
				}
			| TINC TIDENTIFIER TSEMI
				{
					std::cout << "increment statement\n";
					$$ = new NIncStmt(*$2);
				}
			| TDEC TIDENTIFIER TSEMI
				{
					std::cout << "decrement statement\n";
					$$ = new NDecStmt(*$2);
				}
			| TIF cndn TLBRACE block TRBRACE
				{
					std::cout << "if statement\n";
					$$ = new NIfStmt($2, $4, nullptr);
				}
			| TIF cndn TLBRACE block TRBRACE TELSE TLBRACE block TRBRACE
				{
					std::cout << "if / else statement\n";
					$$ = new NIfStmt($2, $4, $8);
				}
			| TWHILE cndn TLBRACE block TRBRACE
				{
					std::cout << "while statement\n";
					$$ = new NWhileStmt($2, $4);
				}
			| TPENUP TLPAREN TRPAREN TSEMI
				{
					std::cout << "pen up function\n";
					$$ = new NPenUpStmt();
				}
			| TPENDOWN TLPAREN TRPAREN TSEMI
				{
					std::cout << "pen down function\n";
					$$ = new NPenDownStmt();
				}
			| TSETPOS TLPAREN expr TCOMMA expr TRPAREN TSEMI
				{
					std::cout << "set position function\n";
					$$ = new NSetPosStmt($3, $5);
				}
			| TSETCOLOR TLPAREN expr TRPAREN TSEMI
				{
					std::cout << "set color function\n";
					$$ = new NSetColorStmt($3);
				}
			| TFWD TLPAREN expr TRPAREN TSEMI
				{
					std::cout << "forward function\n";
					$$ = new NFwdStmt($3);
				}
			| TBACK TLPAREN expr TRPAREN TSEMI
				{
					std::cout << "back function\n";
					$$ = new NBackStmt($3);
				}
			| TROT TLPAREN expr TRPAREN TSEMI
				{
					std::cout << "rotate function\n";
					$$ = new NRotStmt($3);
				}
;

block		: statement
				{
					std::cout << "single block\n";
					$$ = new NBlock();
					$$->AddStatement($1);
				}
			| block statement
				{
					std::cout << "multiple blocks\n";
					$$->AddStatement($2);
				}
;

expr		: numeric
				{
					std::cout << "Numeric expression\n";
					$$ = new NNumericExpr($1);
				}
			| TIDENTIFIER
				{
					std::cout << "Identifier expression\n";
					$$ = new NVarExpr(*$1);
				}
			| expr TADD expr
				{
					std::cout << "var plus assignment\n";
					$$ = new NBinaryExpr($1, $2, $3);
				}
			| expr TSUB expr
				{
					std::cout << "var minus assignment\n";
					$$ = new NBinaryExpr($1, $2, $3);

				}
			| expr TMUL expr
				{
					std::cout << "var multiply assignment\n";
					$$ = new NBinaryExpr($1, $2, $3);

				}
			| expr TDIV expr
				{
					std::cout << "var divide assignment\n";
					$$ = new NBinaryExpr($1, $2, $3);
				}
			| TLPAREN expr TRPAREN
				{
					std::cout << "expresion inside parenthesis\n";
					$$ = ($2);
				}
			| TIDENTIFIER TLBRACKET expr TRBRACKET
				{
					std::cout << "accessing array index\n";
					$$ = new NArrayExpr(*$1, $3);
				}
;

cndn		: expr TLESS expr
				{
					std::cout << "expression less than other expression\n";
					$$ = new NComparison($1, $2, $3);
				}
			| expr TISEQUAL expr
				{
					std::cout << "expression is equal to other expression\n";
					$$ = new NComparison($1, $2, $3);
				}
;

numeric		: TINTEGER
				{
					std::cout << "Numeric value of " << *($1) << '\n';
					$$ = new NNumeric(*($1));
				}
;

%%
