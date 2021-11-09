# C++ Compiler Using Bison

This school project creates a compiler that is able to convert a high level programming language into assembly-like instructions using [Bison](https://www.gnu.org/software/bison/). Check "inputs" folder for code samples and its corresponding "expected" folder for outputs. 

The high-level programming language code contains a data section at the top of each example that defines all the variables that are used in the main section. This allows us to more easily allocate and manage the necessary registers in the backend. 

To run the code with test cases, simply run and build with cmake.

The Procc.l flex file defines all of the tokens for lexical scanning and the Procc.y defines the grammar for Syntax Analysis. Some supported features include:
- variable instantiation and assignment
- a struct like "data" object
- arrays
- basic arithmetic (+, -, /, *)
- if/else statements and, by nature, comparison operators
- while loops
- Pre Increment
- Decrement

## Parts of the Compiler

This compiler runs a frontend and backend. There is no step for optimizing code.

##### Frontend

The frontend consists of the following:
- Lexical Analysis (Scanning) - reads through the file and ensures each token is valid-
- Syntax Analysis (Parsing) - ensures series of tokens follow grammar rules. An enum is associated with each token that defines its use case. It is here that we also generate an  IR which, in our case, is an Abstract Syntax Tree.

##### Backend
- Instruction Selection
- Register Allocation (first done with an infinite number of virtual registers, and then assigned to the seven actual registers r1 - r7)
- Instruction Scheduling


Part of the ITP435 Curriculum at the University of Southern California.



