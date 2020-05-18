/*
 * Author: Daniel Kopta
 * Updated by: Erin Parker
 * CS 4400, University of Utah
 *
 * Simulator handout
 * A simple x86-like processor simulator.
 * Read in a binary file that encodes instructions to execute.
 * Simulate a processor by executing instructions one at a time and appropriately 
 * updating register and memory contents.
 *
 * Some code and pseudo code has been provided as a starting point.
 *
 * Completed by: Matthew Pham u0952138
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "instruction.h"

// Forward declarations for helper functions
unsigned int get_file_size(int file_descriptor);
unsigned int* load_file(int file_descriptor, unsigned int size);
instruction_t* decode_instructions(unsigned int* bytes, unsigned int num_instructions);
unsigned int execute_instruction(unsigned int program_counter, instruction_t* instructions, 
				 int* registers, unsigned char* memory);
void print_instructions(instruction_t* instructions, unsigned int num_instructions);
void error_exit(const char* message);

// 17 registers
#define NUM_REGS 17
// 1024-byte stack
#define STACK_SIZE 1024

int main(int argc, char** argv)
{
  // Make sure we have enough arguments
  if(argc < 2)
    error_exit("must provide an argument specifying a binary file to execute");

  // Open the binary file
  int file_descriptor = open(argv[1], O_RDONLY);
  if (file_descriptor == -1) 
    error_exit("unable to open input file");

  // Get the size of the file
  unsigned int file_size = get_file_size(file_descriptor);
  // Make sure the file size is a multiple of 4 bytes
  // since machine code instructions are 4 bytes each
  if(file_size % 4 != 0)
    error_exit("invalid input file");

  // Load the file into memory
  // We use an unsigned int array to represent the raw bytes
  // We could use any 4-byte integer type
  unsigned int* instruction_bytes = load_file(file_descriptor, file_size);
  close(file_descriptor);

  unsigned int num_instructions = file_size / 4;


  /****************************************/
  /**** Begin code to modify/implement ****/
  /****************************************/

  // Allocate and decode instructions (left for you to fill in)
  instruction_t* instructions = decode_instructions(instruction_bytes, num_instructions);

  // Optionally print the decoded instructions for debugging
  // Will not work until you implement decode_instructions
  // Do not call this function in your submitted final version
  // print_instructions(instructions, num_instructions);


  // Once you have completed Part 1 (decoding instructions), uncomment the below block
  

  // Allocate and initialize registers
  int* registers = (int*)malloc(sizeof(int) * NUM_REGS);

  // TODO: initialize register values
  /*
    There are 17 registers (see Assignment 1: Register IDs).  The width of each register is 32 bits.  All registers should initially have a     value of 0, except %esp.  %esp is the stack pointer register, and should initially have a value of 1024.  (Note that address 1024 is jus    t beyond the range of valid memory addresses.)
    %esp = register 6
  */
  
  int i;
  for(i = 0; i < NUM_REGS; i++){
    if(i==6)
      registers[i] = 1024;
    else
      registers[i] = 0;
  }
  // Stack memory is byte-addressed, so it must be a 1-byte type
  // TODO allocate the stack memory. Do not assign to NULL.
  unsigned char* memory = (char*)malloc(sizeof(char)*1024);

  // Run the simulation
  unsigned int program_counter = 0;

  // program_counter is a byte address, so we must multiply num_instructions by 4 
  // to get the address past the last instruction
  while(program_counter != num_instructions * 4)
  {
    program_counter = execute_instruction(program_counter, instructions, registers, memory);
  }
  
  
  return 0;
}

/*
 * Decodes the array of raw instruction bytes into an array of instruction_t
 * Each raw instruction is encoded as a 4-byte unsigned int
*/
instruction_t* decode_instructions(unsigned int* bytes, unsigned int num_instructions)
{
  // TODO: Don't return NULL
  instruction_t* retval = (instruction_t *)malloc(sizeof(instruction_t)*num_instructions);

  /*
    bits    meaning
    31-27   opcode
    26-22   reg1
    21-17   reg2
    16      unused
    15-0    immediate
  */

  int i;
  for(i = 0; i < num_instructions;i++){
    unsigned int bits = bytes[i];
    
    retval[i].opcode = (unsigned char)(bits >> 27);
    retval[i].first_register = (unsigned char)((bits >> 22) & 0x1F);
    retval[i].second_register = (unsigned char)((bits >> 17) & 0x1F);
    retval[i].immediate = (unsigned)(bits & 0xFFFF);
   
  }
  // part one done
  // print_instructions(retval,num_instructions);
  return retval;
}


/*
 * Executes a single instruction and returns the next program counter
*/
unsigned int execute_instruction(unsigned int program_counter, instruction_t* instructions, int* registers, unsigned char* memory)
{
  // program_counter is a byte address, but instructions are 4 bytes each
  // divide by 4 to get the index into the instructions array
  instruction_t instr = instructions[program_counter / 4];
  
  int CF = 0;
  int ZF = 0;
  int SF = 0;
  int OF = 0;
  int comp = 0;
  long compOF = 0;
  int condition_code = 0;

  switch(instr.opcode)
    {
    case subl:
      registers[instr.first_register] = registers[instr.first_register] - instr.immediate;
      break;
    case addl_reg_reg:
      registers[instr.second_register] = registers[instr.first_register] + registers[instr.second_register];
      break;
    case addl_imm_reg:
      registers[instr.first_register] = registers[instr.first_register] + instr.immediate;
      break;
    case imull:
      registers[instr.second_register] = registers[instr.first_register] * registers[instr.second_register];
      break;
    case shrl:
      registers[instr.first_register] = ((unsigned int)registers[instr.first_register]>>1);
      break;
    case movl_reg_reg:
      registers[instr.second_register]= registers[instr.first_register];
      break;
    case movl_deref_reg:
      registers[instr.second_register] = *((int*)&memory[registers[instr.first_register]+ instr.immediate]);
      break;
    case movl_reg_deref:
      *((int*) & memory[registers[instr.second_register] + instr.immediate]) = registers[instr.first_register];
      break;
    case movl_imm_reg:
      registers[instr.first_register] = (int)instr.immediate;
      break;
    case cmpl:
      /*
	The cmpl instruction sets the condition codes (CF, ZF, SF, OF) corresponding to certain bits in the %eflags register.  The conditional jup        instructions read these codes and either branch to the target or move to the subsequent instruction, based on the appropriate flags.

	When the cmpl instruction is executed, reg2 - reg1 is performed to determine these condition codes:
      
	Condition	   %eflags bit
	CF (carry)	   If reg2 - reg1 results in unsigned overflow, CF is true      0

	ZF (zero)          If reg2 - reg1 == 0, ZF is true                              6
	SF (sign)	   If most significant bit of (reg2 - reg1) is 1, SF is true	7
        OF (overflow)	   If reg2 - reg1 results in signed overflow, OF is true        11

      */
      comp = registers[instr.second_register] - registers[instr.first_register];
      compOF = (long)registers[instr.second_register] - (long)registers[instr.first_register];
    
      //OF 
      if(compOF < INT32_MIN || compOF > INT32_MAX)
	condition_code = condition_code + 0x800;
      //SF
      if(((comp >> 31) & 0x1) == 1 )
	condition_code = condition_code + 0x80;

      //ZF
      if(comp == 0)
	condition_code = condition_code + 0x40;

      //CF
      if(comp > (unsigned int)registers[instr.second_register])
	condition_code = condition_code + 0x1;

      registers[16] = condition_code;
      break;
    case je:
      //ZF
      condition_code = registers[16];
      ZF = ((condition_code  >> 6 )& 0x1);
      if(ZF)
	program_counter = program_counter + instr.immediate;
      break;
    case jl:
      //SF xor OF
      condition_code = registers[16];
      OF = ((condition_code >> 11 )& 0x1);
      SF = ((condition_code >> 7) & 0x1);    
      if(SF ^ OF)
	program_counter = program_counter + instr.immediate;          
      break;
    case jle:
      //(SF xor OF) or ZF
      condition_code = registers[16];
      OF = ((condition_code >> 11) & 0x1);
      SF = ((condition_code >> 7) & 0x1);
      ZF = ((condition_code >> 6) & 0x1);
      if((SF ^ OF) || ZF)
	program_counter = program_counter + instr.immediate;
      break;
    case jge:
      //not (SF xor OF)
      condition_code = registers[16];
      OF = ((condition_code >> 11) & 0x1);
      SF = ((condition_code >> 7) & 0x1);
      if(!(SF ^ OF))
	program_counter = program_counter + instr.immediate;
      break;
    case jbe:
      //CF or ZF
      condition_code = registers[16];
      CF = (condition_code & 0x1);
      ZF = ((condition_code >> 6) & 0x1);
      if(CF || ZF)
	program_counter = program_counter + instr.immediate;
      break;
    case jmp:
      program_counter = program_counter + instr.immediate;
      break;
    case call:
      /*
	%esp = %esp - 4

	memory[%esp] = program_counter + 4

	jump to target
      */
      registers[6] = registers[6] - 4;
      (*(int *) & memory[registers[6]]) = program_counter + 4;
      program_counter = program_counter + instr.immediate;
      break;
    case ret:
      /*
	if %esp == 1024, exit simulation
	else
	program_counter = memory[%esp]
	%esp = %esp + 4
      */
      if(registers[6] == 1024)
	exit(0);
      else{
	program_counter = (*((int *)&memory[registers[6]]));
	registers[6] = registers[6] + 4;
	return program_counter;
      }
      break;
    case pushl:
      /*
	%esp = %esp - 4
	memory[%esp] = reg1
       */
      registers[6] = registers[6] - 4;
      *((int*)&memory[registers[6]]) = registers[instr.first_register];
      break;
    case popl:
      /*
	reg1 = memory[%esp]
	%esp = %esp + 4
      */
      registers[instr.first_register] = *((int*)&memory[registers[6]]);
      registers[6] = registers[6] + 4;
      break;
    case printr:
      printf("%d (0x%x)\n", registers[instr.first_register], registers[instr.first_register]);
      break;
    case readr:
      scanf("%d", &(registers[instr.first_register]));
      break;


  // TODO: Implement remaining instructions

  }

  // TODO: Do not always return program_counter + 4
  //       Some instructions jump elsewhere

  // program_counter + 4 represents the subsequent instruction
  return program_counter + 4;
}


/*********************************************/
/****  DO NOT MODIFY THE FUNCTIONS BELOW  ****/
/*********************************************/

/*
 * Returns the file size in bytes of the file referred to by the given descriptor
*/
unsigned int get_file_size(int file_descriptor)
{
  struct stat file_stat;
  fstat(file_descriptor, &file_stat);
  return file_stat.st_size;
}

/*
 * Loads the raw bytes of a file into an array of 4-byte units
*/
unsigned int* load_file(int file_descriptor, unsigned int size)
{
  unsigned int* raw_instruction_bytes = (unsigned int*)malloc(size);
  if(raw_instruction_bytes == NULL)
    error_exit("unable to allocate memory for instruction bytes (something went really wrong)");

  int num_read = read(file_descriptor, raw_instruction_bytes, size);

  if(num_read != size)
    error_exit("unable to read file (something went really wrong)");

  return raw_instruction_bytes;
}

/*
 * Prints the opcode, register IDs, and immediate of every instruction, 
 * assuming they have been decoded into the instructions array
*/
void print_instructions(instruction_t* instructions, unsigned int num_instructions)
{
  printf("instructions: \n");
  unsigned int i;
  for(i = 0; i < num_instructions; i++)
  {
    printf("op: %d, reg1: %d, reg2: %d, imm: %d\n", 
	   instructions[i].opcode,
	   instructions[i].first_register,
	   instructions[i].second_register,
	   instructions[i].immediate);
  }
  printf("--------------\n");
}

/*
 * Prints an error and then exits the program with status 1
*/
void error_exit(const char* message)
{
  printf("Error: %s\n", message);
  exit(1);
}
