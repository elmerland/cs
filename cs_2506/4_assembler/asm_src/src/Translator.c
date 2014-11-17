/*
 * Translator.c
 * 	This source file handles the translation of assembly instruction into their
 * 	binary representation.
 *
 *  Created on: Oct 26, 2013
 *      Author: Elmer Landaverde
 */
#include "Translator.h"

/**
 * trans_r_type() This functions takes in a R type instruction and generates the
 * 				corresponding binary code.
 * 	pre:		The instruction structure has being properly initialized and all
 * 				its fields are valid. The 'bin' field is set to NULL and no
 * 				memory has being allocated to it.
 * 				The instruction structure has to be an R type instruction.
 * 	Post:		The 'bin' field inside of the instruction structure has being
 * 				filled out by the binary representation of this instruction.
 * 	Return:		True if the instruction was translated successfully. Otherwise
 * 				it returns false.
 */
static bool trans_r_type(Instruction_Info * const inst);

/**
 * trans_i_type() This functions takes in a I type instruction and generates the
 * 				corresponding binary code.
 * 	pre:		The instruction structure has being properly initialized and all
 * 				its fields are valid. The 'bin' field is set to NULL and no
 * 				memory has being allocated to it.
 * 				The instruction structure has to be an I type instruction.
 * 	Post:		The 'bin' field inside of the instruction structure has being
 * 				filled out by the binary representation of this instruction.
 * 	Return:		True if the instruction was translated successfully. Otherwise
 * 				it returns false.
 */
static bool trans_i_type(Instruction_Info * const inst);

/**
 * trans_j_type() This functions takes in a J type instruction and generates the
 * 				corresponding binary code.
 * 	pre:		The instruction structure has being properly initialized and all
 * 				its fields are valid. The 'bin' field is set to NULL and no
 * 				memory has being allocated to it.
 * 				The instruction structure has to be an J type instruction.
 * 	Post:		The 'bin' field inside of the instruction structure has being
 * 				filled out by the binary representation of this instruction.
 * 	Return:		True if the instruction was translated successfully. Otherwise
 * 				it returns false.
 */
static bool trans_j_type(Instruction_Info * const inst);



/*
 * Arrays that contain the different kinds of R type instructions.
 */
const char * const R_TYPE_1[] = { "add", "sub", "or", "and", "slt" };
const char * const R_TYPE_2[] = { "sll", "srl", "sra" };
const char * const R_TYPE_3[] = { "addi", "ori", "andi", "slti", "addiu" };

/*
 * The size of the R type instruction arrays.
 */
const int R_TYPE_1_SIZE = 5;
const int R_TYPE_2_SIZE = 3;
const int R_TYPE_3_SIZE = 5;

/*
 * Arrays that contains the different kinds of J type instructions.
 */
const char * const J_TYPE_1[] = { "beq", "bne" };
const char * const J_TYPE_2[] = { "bltz", "blez" };
const char * const J_TYPE_3[] = { "j", "jal" };
const char * const J_TYPE_4[] = { "jr" };
const char * const J_TYPE_5[] = { "syscall" };

/*
 * The size of the J type instruction arrays.
 */
const int J_TYPE_1_SIZE = 2;
const int J_TYPE_2_SIZE = 2;
const int J_TYPE_3_SIZE = 2;
const int J_TYPE_4_SIZE = 1;
const int J_TYPE_5_SIZE = 1;

bool translate() {
	if (inst_tbl == NULL) {
		printf("Error: Pointer to instruction structure is null.\n");
		return false;
	}

	for (uint32_t i = 0; i < inst_cnt; i++) {

		// Allocate memory for binary line.
		if (!allocate_bin(&inst_tbl[i])) {
			printf("Error: Could not translate instruction.\n");
			return false;
		}

		// Get opcode index.
		int32_t opc_index = get_index(inst_tbl[i].inst, OPC_TBL, OPC_TBL_SIZE);
		if (opc_index == -1) {
			printf("Error: The instruction name is unknown.\n");
			return false;
		}

		// Write opcode to binary line.
		if (!write_to_bin(OPC_TBL[opc_index].bin, &OPC, inst_tbl[i].binary)) {
			printf("Error: Could not write opcode to binary line.\n");
			return false;
		}

		if (inst_tbl[i].type == R_TYPE) {
			if (!trans_r_type(&inst_tbl[i])) {
				printf("Error: Could not translate R type instruction.\n");
				return false;
			}
		} else if (inst_tbl[i].type == I_TYPE) {
			if (!trans_i_type(&inst_tbl[i])) {
				printf("Error: Could not translate I type instruction.\n");
				return false;
			}
		} else if (inst_tbl[i].type == J_TYPE) {
			if (!trans_j_type(&inst_tbl[i])) {
				printf("Error: Could not translate J type instruction.\n");
				return false;
			}
		} else {
			printf("Error: Instruction has invalid type.\n");
			return false;
		}
	}
	return true;
}

static bool trans_r_type(Instruction_Info * const inst) {
	// Check what type of R instruction is it.
	if (contains(inst->inst, R_TYPE_1, R_TYPE_1_SIZE)) {
		// Get function code index.
		int i = get_index(inst->inst, FUNC_TBL, FUNC_TBL_SIZE);
		if (i == -1) {
			printf("Error: Could not get function code for instruction.\n");
			return false;
		}
		// Write function code.
		if (!write_to_bin(FUNC_TBL[i].bin, &FUNC, inst->binary)) {
			printf("Error: Could not write function code.\n");
			return false;
		}
		// write registers
		if (!write_register(inst->par1, &RD, inst->binary)) {
			printf("Error: Could not write RD register.\n");
			return false;
		}
		if (!write_register(inst->par2, &RS, inst->binary)) {
			printf("Error: Could not write RS register.\n");
			return false;
		}
		if (!write_register(inst->par3, &RT, inst->binary)) {
			printf("Error: Could not write RT register.\n");
			return false;
		}
		// Write SA
		if (!write_to_bin(ZERO_TOKEN, &SA, inst->binary)) {
			printf("Error: Could not write SA register.\n");
			return false;
		}
		return true;
	} else if (contains(inst->inst, R_TYPE_2, R_TYPE_2_SIZE)) {
		// Get function code index.
		int i = get_index(inst->inst, FUNC_TBL, FUNC_TBL_SIZE);
		if (i == -1) {
			printf("Error: Could not get function code for instruction.\n");
			return false;
		}
		// Write function code.
		if (!write_to_bin(FUNC_TBL[i].bin, &FUNC, inst->binary)) {
			printf("Error: Could not write function code.\n");
			return false;
		}
		// write registers
		if (!write_register(inst->par1, &RD, inst->binary)) {
			printf("Error: Could not write RD register.\n");
			return false;
		}
		if (!write_register(ZERO_REG, &RS, inst->binary)) {
			printf("Error: Could not write RS register.\n");
			return false;
		}
		if (!write_register(inst->par2, &RT, inst->binary)) {
			printf("Error: Could not write RT register.\n");
			return false;
		}
		// Get SA number
		uint32_t sa = parse_int_s(inst->par3);
		if (sa >= 32) {
			printf("Error: Invalid shift amount value.\n");
			return false;
		}
		// Write SA
		if (!write_num_to_bin(sa, &SA, inst->binary)) {
			printf("Error: Could not write SA register.\n");
			return false;
		}
		return true;
	} else if (contains(inst->inst, R_TYPE_3, R_TYPE_3_SIZE)) {
		// write registers
		if (!write_register(inst->par2, &RS, inst->binary)) {
			printf("Error: Could not write RS register.\n");
			return false;
		}
		if (!write_register(inst->par1, &RT, inst->binary)) {
			printf("Error: Could not write RT register.\n");
			return false;
		}
		// Determine if immediate is symbol or number.
		int16_t imm = 0;
		if (is_symbol(inst->par3)) {
			imm = get_symbol_addr(inst->par3);
			if (has_error) {
				printf("Error: Symbol does not exist.\n");
				return false;
			}
		} else {
			imm = parse_int_s_16(inst->par3);
		}
		if (!write_num_to_bin_16_s(imm, &IMM, inst->binary)) {
			printf("Error: Could not write immediate value.\n");
			return false;
		}
		return true;
	}
	else {
		printf("Error: Unknown r type instruction.\n");
		return false;
	}
}

static bool trans_i_type(Instruction_Info * const inst) {
	int16_t offset = 0;
	bool has_offset = false;
	// Check if offset is included
	if (inst->par3 != NULL) {
		// Get offset
		offset = parse_int_s_16(inst->par2);
		has_offset = true;
	}
	// Write offset
	if (!write_num_to_bin_16_s(offset, &OFF, inst->binary)) {
		printf("Error: Could not write offset.\n");
		return false;
	}

	// Get second parameter.
	char *rs_par = NULL;
	if (has_offset) {
		rs_par = inst->par3;
	}
	else {
		rs_par = inst->par2;
	}
	// write registers
	if (!write_register(rs_par, &RS, inst->binary)) {
		printf("Error: Could not write RS register.\n");
		return false;
	}
	if (!write_register(inst->par1, &RT, inst->binary)) {
		printf("Error: Could not write RT register.\n");
		return false;
	}
	return true;
}

static bool trans_j_type(Instruction_Info * const inst) {
	if (contains(inst->inst, J_TYPE_1, J_TYPE_1_SIZE)) {
		// write registers
		if (!write_register(inst->par1, &RS, inst->binary)) {
			printf("Error: Could not write RS register.\n");
			return false;
		}
		if (!write_register(inst->par2, &RT, inst->binary)) {
			printf("Error: Could not write RT register.\n");
			return false;
		}
		// Get offset
		uint16_t symbol_addr = get_symbol_addr(inst->par3);
		if (has_error) {
			printf("Error: Symbol does not exist.\n");
			return false;
		}
		int16_t offset = (symbol_addr - (inst->addr + 4)) / 4;
		// Write offset
		if (!write_num_to_bin_16(offset, &OFF, inst->binary)) {
			printf("Error: Could not write offset.\n");
			return false;
		}
		return true;
	} else if (contains(inst->inst, J_TYPE_2, J_TYPE_2_SIZE)) {
		// write registers
		if (!write_register(inst->par1, &RS, inst->binary)) {
			printf("Error: Could not write RS register.\n");
			return false;
		}
		if (!write_register(ZERO_REG, &RT, inst->binary)) {
			printf("Error: Could not write RT register.\n");
			return false;
		}
		// Get offset
		uint16_t symbol_addr = get_symbol_addr(inst->par2);
		if (has_error) {
			printf("Error: Symbol does not exist.\n");
			return false;
		}
		int16_t offset = (symbol_addr - (inst->addr + 4)) / 4;
		// Write offset
		if (!write_num_to_bin_16(offset, &OFF, inst->binary)) {
			printf("Error: Could not write offset.\n");
			return false;
		}
		return true;
	} else if (contains(inst->inst, J_TYPE_3, J_TYPE_3_SIZE)) {
		// Get label address.
		uint16_t symbol_addr = get_symbol_addr(inst->par1) >> 2;
		if (has_error) {
			printf("Error: Symbol does not exist.\n");
			return false;
		}
		// Write address.
		if (!write_num_to_bin_16(symbol_addr, &TARA, inst->binary)) {
			printf("Error: Could not write target address.\n");
			return false;
		}
		// Write empty spaces
		if (!write_to_bin(ZERO_10, &TARZ, inst->binary)) {
			printf("Error: Could not write zero's of target address.\n");
			return false;
		}
		return true;
	} else if (contains(inst->inst, J_TYPE_4, J_TYPE_4_SIZE)) {
		// Get function code index.
		int i = get_index(inst->inst, FUNC_TBL, FUNC_TBL_SIZE);
		if (i == -1) {
			printf("Error: Could not get function code for instruction.\n");
			return false;
		}
		// Write function code.
		if (!write_to_bin(FUNC_TBL[i].bin, &FUNC, inst->binary)) {
			printf("Error: Could not write function code.\n");
			return false;
		}
		// Write register
		if (!write_register(inst->par1, &RS, inst->binary)) {
			printf("Error: Could not write RS register.\n");
		}
		// Write zero's
		if (!write_to_bin(ZERO_15, &JRZ, inst->binary)) {
			printf("Error: Could not write zero's to JR.\n");
			return false;
		}
		return true;
	} else if (contains(inst->inst, J_TYPE_5, J_TYPE_5_SIZE)) {
		// Get function code index.
		int i = get_index(inst->inst, FUNC_TBL, FUNC_TBL_SIZE);
		if (i == -1) {
			printf("Error: Could not get function code for instruction.\n");
			return false;
		}
		// Write function code.
		if (!write_to_bin(FUNC_TBL[i].bin, &FUNC, inst->binary)) {
			printf("Error: Could not write function code.\n");
			return false;
		}
		// Write code.
		if (!write_to_bin(ZERO_CODE, &SYS, inst->binary)) {
			printf("Error: Could not write zero code.\n");
			return false;
		}
		return true;
	}
	return true;
}
