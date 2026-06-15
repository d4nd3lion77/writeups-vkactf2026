#include <random>
#include <algorithm>
#include <iostream>
#include <stack>
#include "windows.h"
#include "string.h"
#include "opcodes.h"

class VirtualMachine;
typedef void (*Handler)(VirtualMachine*);
class VirtualMachine{
    private:
        bool running;
        WORD decode_key;
        WORD ip;
        WORD sp;
        WORD bp;

        WORD ax, bx, cx, dx, si, di;
        Handler handler_table[256];

        static void op_halt(VirtualMachine*);
        static void op_const(VirtualMachine*);
        static void op_push(VirtualMachine*);
        static void op_pop(VirtualMachine*);
        static void op_inc(VirtualMachine*);
        static void op_dec(VirtualMachine*);
        static void op_add(VirtualMachine*);
        static void op_sub(VirtualMachine*);
        static void op_mul(VirtualMachine*);
        static void op_div(VirtualMachine*);
        static void op_and(VirtualMachine*);
        static void op_or(VirtualMachine*);
        static void op_xor(VirtualMachine*);
        static void op_not(VirtualMachine*);
        static void op_shl(VirtualMachine*);
        static void op_shr(VirtualMachine*);
        static void op_jmp(VirtualMachine*);
        static void op_cmpje(VirtualMachine*);
        static void op_cmpjne(VirtualMachine*);
        static void op_cmpjg(VirtualMachine*);
        static void op_cmpjge(VirtualMachine*);
        static void op_cmpjl(VirtualMachine*);
        static void op_cmpjle(VirtualMachine*);
        static void op_dup(VirtualMachine*);
        static void op_call(VirtualMachine*);
        static void op_ret(VirtualMachine*);
        static void op_syscall(VirtualMachine*);
        static void op_mov(VirtualMachine*);
        static void op_mod(VirtualMachine*);
        static void op_loadind(VirtualMachine*);
        static void op_storeind(VirtualMachine*);
        static void op_nop(VirtualMachine*);
        static void op_swap(VirtualMachine*);
        static void op_print(VirtualMachine*);
        static void op_scan(VirtualMachine*);
        static void op_unknown(VirtualMachine*);
        
    public:
        WORD fetch_opcode();
        WORD memory[MAX_MEMORY];
        VirtualMachine();
        bool loadCode(WORD *code, size_t size);
        void runCode();
};


