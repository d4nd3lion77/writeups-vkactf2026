#include "handlers.h"

VirtualMachine::VirtualMachine(){
    std::mt19937 rng;
    rng.seed(0xB00B5);
    ip = ax = bx = cx = dx = si = di = 0;
    decode_key = 0xB00B;
    sp = MAX_MEMORY -1;

    for(int i=0;i<256;i++)
    
    handler_table[i] = op_unknown;

    handler_table[HALT] = op_halt;
    handler_table[CONST_]  = op_const;
    handler_table[POP]   = op_pop;
    handler_table[PUSH] = op_push;
    handler_table[INC]   = op_inc;
    handler_table[DEC]   = op_dec;
    handler_table[ADD]   = op_add;
    handler_table[SUB]   = op_sub;
    handler_table[MUL]  = op_mul;
    handler_table[DIV] = op_div;
    handler_table[AND]  = op_and;
    handler_table[OR]   = op_or;
    handler_table[XOR]   = op_xor;
    handler_table[NOT]   = op_not;
    handler_table[SHL]   = op_shl;
    handler_table[SHR]   = op_shr;
    handler_table[JMP]  = op_jmp;
    handler_table[CMPJE] = op_cmpje;
    handler_table[CMPJNE]  = op_cmpjne;
    handler_table[CMPJG]   = op_cmpjg;
    handler_table[CMPJGE]   = op_cmpjge;
    handler_table[CMPJL]   = op_cmpjl;
    handler_table[CMPJLE]   = op_cmpjle;
    handler_table[DUP]   = op_dup;
    handler_table[CALL]  = op_call;
    handler_table[RET] = op_ret;
    handler_table[SYSCALL]  = op_syscall;
    handler_table[MOV]   = op_mov;
    handler_table[MOD]   = op_mod;
    handler_table[LOADIND]   = op_loadind;
    handler_table[STOREIND]   = op_storeind;
    handler_table[NOP]   = op_nop;
    handler_table[SWAP]  = op_swap;
    handler_table[PRINT]  = op_print;
    handler_table[SCAN]  = op_scan;
    memset(memory, 0, sizeof(memory));
}

bool VirtualMachine::loadCode(WORD *code, size_t size) {
    if (size > MAX_MEMORY) {
        std::cout << "[ERROR] Not enough memory\n";
        return false;
    }

    memcpy(memory, code, size);
    
    std::cout << std::endl;
    return true;
}

void VirtualMachine::runCode() { 
    running = true;
    ip = 0; 
    sp = MAX_MEMORY - 1; 
    WORD opcode = fetch_opcode();
    handler_table[opcode](this); 
}

WORD VirtualMachine::fetch_opcode() {
    WORD enc = memory[ip];
    WORD key = 0xB00B ^ ip;
    WORD opcode = enc ^ key;
    ip++;
    return opcode;
}