# CO MIPS SIMULATOR
## Team Instinct

*"You can't program fear in a Machine, but without Human Instinct it wouldn't survive long"*

CS19B046 - Githin George\
CS19B047 - Harish Pradyot M

## About and Agenda

* Main aim was to gain more understanding of how an ISA actually executes the Instructions at a lower level.
* It also enabled us to gain more experience in the methods innvolved in the development of a tool from bottom up and hone our coding skills.

*What is a Simulator?*
>An application build using any programming language which will simulate/imitate all the functions that are implemented in a MIPS32 ISA.\
The tool which we are developing is a simulator that will run a MIPS32 assembly program and display the Registers and Memory (both the text segement as well as the data segment).

The tool was developed using QTCreator (version  4.14.0) which is a cross-platform IDE which allows developers to create applications across desktop, mobile, and embedded platforms.


## Phase 1 Overview

#### Progress Made:
>* **Parsing** : Instructions are loaded to the Application and the parser runs through the code to extract each line as a token string
  and converts it into a 32 bit Instruction.\
  The Syntax an Semantics of the Assembly Code is also checked and returns a Warning Message and aborts the program on encountering a parse error.\
>* **Execution** :\
  Each intruction is executed in 5 steps i.e.\
  **1. Instruction Fetch (IF) stage** : Fetches the instruction at the current PC address and sets the PC address to the next instruction.\
  **2. Instruction Decode and Register Fetch (ID/RF) stage** : Decodes the 32 bit Instruction code which can be of 3 types R Type, J Type, I type splits the Instruction into specific components and passes on the values to the respective EX (Execute, the next stage) overloaded functions.\
  **3. Execute (EX) stage** : Calculates the result of all Arithmatic and Logical Instructions, Effective Address calcualtion etc.\
  **4. Memory (MEM) stage** : Used to access the memory that has been defined for the ISA either to load a value or store a value in the Memory.\
  **5. Write Back (WB) stage** : Update the value of the destination Register after completing the Execution.


#### Instructions Simulated:
*The list of valid Registers that can be used are displayed on the left Panel of the Application*\
Operation | Type | Syntax
------------ | ------------ | -------------
add | R type | add rd, rs, rt
sub | R Type | sub rd, rs, rt
slt | R Type | slt rd, rs, rt
and | R Type | and rd, rs, rt
or | R Type | or rd, rs, rt


Operation | Type | Syntax
------------ | ------------ | -------------
andi | I type | andi rd, rs, immediate value
ori | I Type | ori rd, rs, immediate value
slti | I Type | slti rd, rs, immediate value
addi | I Type | addi rd, rs, immediate value


Operation | Type | Syntax
------------ | ------------ | -------------
bne | I type | bne rs, rt, value or label
beq | I Type | beq rs, rt, value or label


Operation | Type | Syntax
------------ | ------------ | -------------
j | I type | j value or label
jal | I Type | jal value or label


Operation | Type | Syntax
------------ | ------------ | -------------
lw | R type | lw rd, offset(rs)
sw | R Type | sw rs, offset(rd)


Operation | Type | Syntax
------------ | ------------ | -------------
lw | R type | lw rd, offset(rs)
sw | R Type | sw rs, offset(rd)


Operation | Type | Syntax
------------ | ------------ | -------------
jr | R type | jr ra
lui | I Type | lui rd, immediate value


Operation | Type | Syntax
------------ | ------------ | -------------
sll | R type | sll rd, rs, value
srl | R Type | srl rd, rs, value

#### How to use the Application

1. Incomplete I'll do this later I'll try filling the form now
