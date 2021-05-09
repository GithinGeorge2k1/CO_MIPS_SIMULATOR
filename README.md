# CO MIPS SIMULATOR
## Team Instinct

*"You can't program fear in a Machine, but without Human Instinct it wouldn't survive long"*

CS19B046 - Githin George\
CS19B047 - Harish Pradyot M

## About and Agenda

* Main aim was to gain more understanding of how an ISA actually executes the Instructions at a lower level.
* It also enabled us to gain more experience in the methods involved in the development of a tool from bottom-up and hone our coding skills.

*What is a Simulator?*
>An application build using any programming language that will simulate/imitate all the functions implemented in an ISA. (MIPS32 ISA)\
The tool which we are developing is a simulator that will run a MIPS32 assembly program and display the Registers and Memory (both the text segment as well as the data segment).

The tool was developed using QTCreator (version  4.14.0), a cross-platform IDE that allows developers to create applications across desktop, mobile, and embedded platforms.


## Phase 1 Overview

#### Progress Made:
>* **Parsing** : Instructions are loaded to the Application, and the parser runs through the code to extract each line as a token string and converts it into a 32-bit Instruction(An integer in essence).\
  The Syntax of the Assembly Code is also checked, and a Warning Message is returned, and the program is aborted on encountering a parse error(In data Section The parser won't register wrong entries as error ->a fault with the parser).
>* **Execution** :\
  Each Instruction is executed in 5 steps, i.e., \
  **1. Instruction Fetch (IF) stage** : Fetches the Instruction at the current PC address and sets the PC address to the next Instruction.\
  **2. Instruction Decode and Register Fetch (ID/RF) stage** : Decodes the 32-bit Instruction code, which can be of 3 types R Type, J Type, I type splits the Instruction into specific components and passes on the values to the respective EX (Execute, the next stage) overloaded functions.\
  **3. Execute (EX) stage** : Calculates the result of all Arithmetic and Logical Instructions, Effective Address calculation, etc.\
  **4. Memory (MEM) stage** : Used to access the Memory that has been defined for the ISA, either to load or store a value in the Memory.\
  **5. Write Back (WB) stage** : Update the value of the destination Register after completing the Execution.


#### Instructions Simulated:
*The list of valid Registers that can be used is displayed on the left panel of the Application*
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

## Phase 2 and Phase 3 Overview

#### Objective:
>* **Pipeline** : The Simulator developed in Phase 1 was extended to incorporate Pipelining.\
  The user has an option to enable/disable ***Data Forwarding*** using the ***Enable Forwarding*** Option.\
  At the end of the execution the simulator displays the Number of Stalls, the IPC (Instructions per Cycle),\
  and the Cache i.e. No of Hits, Misses and Average Access Time of the Cache in terms of Clock Cycles in the Console.\
  No support provided for pseudo instructions and syscalls therefore console has not been extended for taking input or printing output.
  ![Console](https://user-images.githubusercontent.com/70936222/116671858-128cd800-a9bf-11eb-9582-8894a4f84c73.PNG)
  It also displays the list of instructions for which there were stalls in the ***Stall List*** tab.
  ![Stalls](https://user-images.githubusercontent.com/70936222/116671863-13be0500-a9bf-11eb-8699-a4d1546f52cd.PNG)

#### Design Choices:
>1.  **Cache** : The Cache configuration can be set by the user according to the requirement of the simulation.\ 
    There are 2 levels of cache following inclusive caching policy. The Block size must be same.\
    Cache size can be in KB or bytes, Block Size in Bytes, and the Associativity (cannot be more than total number of blocks).\
    The Cache is **Write Back, Write Allocate Cache** .\
    Also, the cache is realized in simulator with the assumption that there is a store queue or a buffer that writes to Main Memory(and from L1 cache to L2) . While       writing to MM, we assumed that all memory read concerning that address is taken from the store queue.\
    Also Loading into Cache is assumed to be done without delay.
    Assuming the System is Word addressable(only multiples of 4 Bytes) the Cache is set for use by the Program.
    ![Cache](https://user-images.githubusercontent.com/70936222/116671854-128cd800-a9bf-11eb-843f-91fbbc5df522.PNG)
     
>2.  **Branch Predictor** : A **Not Taken** branch predictor is used in the Simulator. Which assumes that everytime a branch statement is encountered it will not be taken.\
     And the instruction is either squashed or the Pipeline continues, based on the fact whether the Branch is taken or not taken respectively.
     
#### The App:
![1](https://user-images.githubusercontent.com/70936222/116671851-10c31480-a9bf-11eb-85a3-c84064687510.PNG)
>1. Data Segment and Text Segment:
    ![Data](https://user-images.githubusercontent.com/70936222/116671860-13256e80-a9bf-11eb-96b9-eb0bfc16be66.PNG)
    ![Text](https://user-images.githubusercontent.com/70936222/116671867-14569b80-a9bf-11eb-8f1c-b86088c092c0.PNG)
>2. Timeline:
    An interactive timeline to help you visualize how the pipeline behaves for the User's code and therefore helps in identifying bottlenecks.
    ![Timeline](https://user-images.githubusercontent.com/70936222/116671868-14ef3200-a9bf-11eb-914a-e86cb4628768.PNG)
>3. Data Forwarding: A toggle is provided below the console to Enable or Disable Data-Forwarding. ***EX-MEM*** to ***IDRF-EX*** and ***MEM-WB*** to ***IDRF-EX*** are the 2 types of Data-Forwarding Incorporated in our Design.
    ![Console1](https://user-images.githubusercontent.com/70936222/116742686-5ad6e500-aa15-11eb-803a-421ae415cbb1.PNG)

    
