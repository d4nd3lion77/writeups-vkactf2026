#include "vm.h"
#include <iomanip>

void VirtualMachine::op_unknown(VirtualMachine* vm){
    // std::cout << "Error at sp: " << vm->sp << " ip: " << vm->ip-1 << std::endl;
    // std::cout << "Unknown instruction: " << vm->memory[vm->ip-1] << std::endl;
    vm->running = false;
}

void VirtualMachine::op_halt(VirtualMachine* vm){
    // std::cout << "[DEBUG] HALT: Stopping VM" << std::endl;
    // vm->running = false;
    return;
}

void VirtualMachine::op_const(VirtualMachine* vm){
    WORD value = vm->memory[vm->ip++];
    vm->memory[--vm->sp] = value;
    // std::cout << "[DEBUG] CONST: push " << value << " to stack[sp=" << vm->sp << "]" << std::endl;

    WORD next = vm->fetch_opcode();
    vm->handler_table[next](vm);
}

void VirtualMachine::op_push(VirtualMachine* vm){
    WORD addr = vm->memory[vm->ip++];
    WORD value = vm->memory[addr];
    vm->memory[--vm->sp] = value;
    // std::cout << "[DEBUG] PUSH: push mem[" << addr << "]=" << value << " to stack[sp=" << vm->sp << "]" << std::endl;

    WORD next = vm->fetch_opcode();
    vm->handler_table[next](vm);
}

void VirtualMachine::op_pop(VirtualMachine* vm){
    WORD addr = vm->memory[vm->ip++];
    WORD value = vm->memory[vm->sp++];
    vm->memory[addr] = value;
    // std::cout << "[DEBUG] POP: pop " << value << " from stack[sp=" << vm->sp-1 << "] to mem[" << addr << "]" << std::endl;
    
    WORD next = vm->fetch_opcode();
    vm->handler_table[next](vm);
}

void VirtualMachine::op_inc(VirtualMachine* vm){
    WORD old_val = vm->memory[vm->sp];
    WORD a = vm->memory[vm->sp];
    WORD aux0 = a * old_val;
    a = 23547 + 15994 * ((~((a ^ a) & (~aux0 | a ^ a) & ~(aux0 &
		a | aux0 ^ aux0) ^ a) | ~(aux0 & (a & aux0 ^ ~aux0) & a & a
		& a & ~(aux0 & a) & (~(~aux0 & aux0 & aux0) ^ aux0 & ((aux0
		| aux0) ^ aux0)))) & (~((aux0 & a | aux0) ^ ~a ^ a | a |
		aux0 | ~a | a & ~a) & (aux0 ^ a) | (~~(a ^ a) | (aux0 ^ a) &
		a & a ^ a ^ a ^ a ^ a) & aux0 ^ ~((aux0 | aux0 & aux0 &
		(aux0 ^ aux0)) & (aux0 | (a | aux0) ^ aux0 & a)))) + 13573 *
		(~aux0 ^ ~(((a | a | a) & ~~a ^ ~(aux0 & aux0) & aux0) &
		(aux0 & aux0 & ~a ^ ~aux0 ^ aux0 ^ aux0 ^ (aux0 & a ^ aux0 |
		a)) & (aux0 ^ a & ((a | aux0) ^ a ^ a | a)))) + 22753 *
		~aux0 + 23335 * ~(((aux0 & aux0 & a | a & aux0 | aux0 & aux0
		| aux0 | aux0 | a & aux0 & (a ^ aux0)) ^ aux0 ^ a ^ (a & a ^
		~a) & (aux0 & aux0 ^ (a | aux0))) & ~~(~~a & (aux0 & a |
		aux0 | a)) ^ aux0) + 46814 * aux0 + 54516 * ~~((aux0 & aux0
		& aux0 & ~a & ~~a | ~a & (a ^ aux0) ^ ~a | ~(a ^ (aux0 |
		aux0))) ^ (~(aux0 | ~aux0) ^ (aux0 | aux0 | ~a) & (aux0 &
		aux0 | a | aux0) | ~(~(a | a) & (a | aux0 | ~aux0)))) +
		44709 * ((a | aux0) ^ ~((aux0 ^ a) & (~~a ^ aux0 & a ^ a) &
		(aux0 | a | ~~a)) | aux0) + 19646 * ((((a ^ aux0) & (aux0 |
		a) & (aux0 | aux0 & a) ^ ~aux0 ^ (a | a | aux0)) & aux0 ^
		((a | a | aux0 | a) & (~aux0 ^ ~aux0 | a) | ~(aux0 & (a ^
		a)) & (a & a & a ^ ~aux0 & a & aux0))) & (aux0 & (~(aux0 |
		a) | ~a & ~a | aux0 ^ aux0 | a | a & (aux0 | aux0)) ^ ((a ^
		aux0) & (aux0 | a) & (a ^ aux0) & a ^ ~aux0 ^ (a | aux0) ^
		aux0 & a ^ ~a | aux0)) ^ aux0 & ~(~(~aux0 & (aux0 & a ^
		(aux0 | a))) ^ ((aux0 | a) ^ a ^ a ^ a & ~aux0 | aux0))) +
		51347 * ((~(~((aux0 | a) ^ (a | a)) | ~aux0 | a & a & ~a) |
		aux0) ^ (aux0 & a | ~aux0) | aux0) + 29130 * (~~((a | aux0 |
		aux0 | a) & aux0 & aux0 & (a ^ aux0) & ((a ^ aux0) & aux0 &
		aux0 ^ ~(aux0 ^ a))) | a ^ aux0 | ~~((aux0 & (aux0 | a &
		aux0) | aux0 | a ^ aux0 ^ a ^ aux0) & (aux0 | ~a ^ a & aux0
		& aux0))) + 6434 * (~(aux0 & (a | aux0) | ~~a) & aux0 ^ a ^
		aux0 ^ aux0 ^ ~aux0 ^ (a | aux0) & a ^ (aux0 | aux0 | a |
		aux0 | aux0) ^ ~(a & (aux0 ^ aux0)) ^ a ^ aux0 ^ aux0 ^
		~(~aux0 & (aux0 | aux0)) ^ aux0 ^ a ^ ~(((aux0 | aux0) ^ a ^
		aux0) & a ^ ~(aux0 | a)) ^ a & aux0 & a & (aux0 | a | aux0)
		& ~((aux0 | a | aux0 & a) ^ ~(aux0 & a)) & a) + 55503 * (a &
		a) + 34079 * (~aux0 ^ a) + 16145 * (~aux0 ^ ~((aux0 |
		(~(aux0 | a) | a & a ^ ~a) & (~(aux0 ^ a) | ~(aux0 & aux0)))
		^ ~(~~(aux0 & a) & (~~a ^ a)))) + 40354 * (a | (~(~(aux0 ^
		aux0) ^ a) | a ^ a | aux0 ^ (a | aux0) | ~~a) ^ aux0 &
		~~((aux0 | aux0) & ~aux0) | ~((aux0 | ~aux0) ^ aux0 & a ^
		a)) + 53883 * (a ^ ((aux0 & ~(a ^ aux0) | a | aux0) & aux0 |
		~~~a & a ^ ((~a | ~a) & (~a ^ ~a) | ~a & aux0 & aux0 & a &
		~a)) & (~~(a ^ a) | aux0 & (aux0 | a) ^ aux0 | ~(aux0 &
		~(aux0 | aux0)) | (aux0 | ~aux0) ^ aux0)) + 64587 * (((~aux0
		^ a ^ ((aux0 | a) & aux0 & a | a | aux0 ^ aux0) ^ (~a & a &
		a | (aux0 | aux0) & a)) & ~(~(a | a | aux0 | aux0) | ~aux0)
		| aux0 | ~(~(aux0 & a ^ aux0 & a) & ~(a | a) & (a ^ a &
		aux0))) & (aux0 | ~((a ^ a | a ^ aux0) & (a ^ a) & ~aux0) |
		(a & aux0 ^ aux0 ^ a) & (aux0 | a ^ aux0) & (a & ~a ^ a &
		aux0 ^ aux0 ^ a) | aux0)) + 32041 * (~(~((aux0 ^ a) & (a ^
		aux0)) | aux0 | a | ~(~(a | ~aux0) ^ ~(aux0 ^ a | aux0 ^
		a))) & (~~(a ^ aux0) & ((aux0 | aux0) & ~aux0 | aux0 ^ ~a) |
		~a & (aux0 ^ ~a & (a | aux0)) | ~((~~aux0 | ~a | aux0) & a &
		aux0 & a)) & (aux0 | aux0 ^ a | a | ~aux0 & ~a | ~(a | a) ^
		aux0 ^ aux0 ^ ~aux0 | ~(a ^ aux0) ^ (aux0 ^ a | a | aux0)))
		+ 33768 * a + 52250 * ~((a & (a & aux0 ^ ~aux0) & aux0 |
		((aux0 ^ a) & (a ^ a) | a & aux0 | aux0 | aux0) & ~(~aux0 |
		~aux0)) & (aux0 & a & (a ^ a) | aux0 & a ^ aux0 ^ a) & ((a |
		aux0) & (aux0 | aux0) | ~~a) & ~(~(aux0 | aux0) & aux0) ^
		((aux0 & aux0 ^ (a | a) ^ a ^ a ^ aux0 | ~~a & ~a & (a | a))
		^ ~(a | (aux0 ^ a) & aux0 & aux0) | ~~~~(aux0 & aux0))) +
		2452 * (aux0 & aux0) + 53615 * ~(~(aux0 | a) | ~(aux0 & a &
		~a ^ a & ~aux0) & (~aux0 | ~(aux0 & aux0 | a & a)) | ~aux0 &
		(a & a | a & a) ^ a & aux0 & aux0 & (aux0 | a) ^ (~aux0 |
		aux0 | aux0 | ~(aux0 | a)) & aux0) + 15514 * (a & aux0 &
		aux0) + 42508 * (a ^ (~~(~aux0 & a) | aux0 ^ (~(a ^ a) | a ^
		a ^ aux0 | ~~~a) ^ aux0 & (~(a | a) ^ aux0 ^ aux0 ^ aux0 ^ a
		| (aux0 & a | a ^ a) ^ aux0))) + 21663 * ~~a + 28379 * (((a
		| a | a ^ aux0) ^ a ^ ~aux0 ^ a ^ aux0 ^ a ^ (~aux0 | a |
		aux0)) & ((~a | aux0 ^ a) ^ (aux0 | a) ^ aux0 & a ^ ((a |
		aux0) ^ aux0) & (aux0 | aux0)) & (~(a | a) | ~a ^ a ^ a | a
		& ~(aux0 ^ aux0) | aux0) ^ ~(aux0 & (a ^ a | aux0 ^ aux0) &
		(~aux0 | a) | ~~(a & aux0) & aux0) ^ ~~((a ^ (~a & (aux0 ^
		aux0) | ~(aux0 & aux0))) & ((a & aux0 ^ (a | aux0)) & (aux0
		^ aux0 ^ aux0 ^ a) | ~aux0 ^ aux0 ^ ~(aux0 & a)))) + 20594 *
		((((~a | ~a) & (~aux0 ^ (a | a)) & ~(aux0 & a & ~a) | aux0)
		^ a | a) & (~~aux0 ^ a ^ a & a ^ a & a ^ ~a ^ (a ^ ~aux0) &
		(aux0 & aux0 ^ (a | aux0)) & (~aux0 | a | a) | ~(aux0 & a ^
		~a) | ~~a | a) & ~(~~(a & a) & ~~a & ~(aux0 ^ a) & (aux0 &
		~a ^ ~((aux0 ^ aux0) & a)))) + 6663 * ((~((a ^ ~a) & a ^ (a
		| a | ~a) ^ a) ^ ~aux0) & ((~(aux0 & a & a & a) | aux0 | ~(a
		^ a ^ ~a)) ^ ((a & a | ~a) & ~a ^ ~a ^ (aux0 | aux0 | aux0 |
		a)) & (~(a | aux0 | aux0) ^ ~(~aux0 ^ a ^ a))) & aux0) +
		24560 * (~(~~((a ^ aux0) & (aux0 | aux0)) ^ a ^ ~((aux0 |
		aux0) ^ aux0 & aux0) | aux0) & ((~a | (a ^ a) & (a ^ a) ^ a
		& aux0 ^ (aux0 | a) | ~a) ^ ~((~aux0 ^ (aux0 | a) | a ^
		~aux0) ^ (aux0 & a | aux0 ^ a) ^ a)) & (~~aux0 | (aux0 ^
		aux0) & (aux0 | a) & (aux0 | a) & a) & (a | a | a | aux0 | a
		& a) & a) + 11308 * ((~(~((a | aux0) & ~a) | ~~~a) ^ (aux0 |
		((aux0 | a) ^ (a | a) | ~aux0 | ~a) ^ (aux0 ^ (a | a) | a &
		a | a & aux0))) & ~~(aux0 | ~(aux0 ^ aux0) & (aux0 | aux0))
		& ~(((~(aux0 & a & (aux0 | aux0)) | ~(a & aux0 ^ aux0 ^ a))
		^ (~(~aux0 & a & aux0) | a | a)) & ~(aux0 | aux0) & (~(a ^
		aux0) ^ ~aux0 ^ ~a ^ (~a | a ^ aux0)))) + 56688 * (((aux0 ^
		~aux0 ^ aux0 & aux0 ^ ((aux0 | a) ^ ~aux0) & ~a & (a ^ a)) &
		aux0 | (aux0 | aux0) ^ ((~a | aux0 ^ a) ^ a) & a) & (a ^ a)
		& (aux0 & aux0 ^ ~aux0 ^ a | ~(a | aux0)) & ~(a | (aux0 &
		aux0 ^ a ^ a) & aux0) & (aux0 ^ ~~a & a)) + 59492 * (aux0 &
		(~aux0 ^ ~((aux0 & aux0 ^ a ^ a | ~(aux0 & aux0)) ^ aux0)) &
		(~((a | aux0 ^ a) & ~(aux0 | aux0)) & ((a | a) & (a | a) ^
		aux0 ^ ~aux0 | aux0 & aux0 | aux0 | a | ~aux0) | ~(aux0 &
		aux0 ^ a))) + 6614 * (~((~(a | aux0 | a) ^ a) & aux0 & (a ^
		a | aux0 | aux0) & (~aux0 ^ aux0) & ~(aux0 | a) & ((a | aux0
		| a | aux0 | aux0) ^ (~(aux0 | aux0) | (a | aux0) & aux0) ^
		(~a | (a | a | aux0 | a) & (~a ^ (a | aux0))))) | ~~~(~(aux0
		| aux0 | aux0) | a ^ a ^ ~a | a | ~a)) + 63762 * (~a ^ (aux0
		& ~(a ^ (a | a)) & a ^ ~a & aux0) & ~(~a & ~aux0 & (~aux0 |
		aux0 | a) & a) & (aux0 & (a & a ^ aux0 ^ aux0) & ~aux0 & (a
		^ aux0) | aux0 | ~(aux0 | aux0) & aux0)) + 57032 *
		((~~~~aux0 | a) ^ aux0) + 53026 * ((a & (a ^ aux0 ^ aux0 &
		aux0 ^ ~~aux0 ^ (a & a | a) & a | ~~(aux0 ^ aux0 ^ ~a)) ^
		~(aux0 | a | ~aux0)) & ((~aux0 | ~(~a | aux0)) & aux0 ^
		((aux0 & aux0 ^ ~aux0) & (~a | aux0 & aux0) ^ (a | aux0 ^
		aux0 ^ a) ^ (~aux0 | a | a & a & aux0) ^ (aux0 & a & aux0 |
		aux0 | aux0) | aux0))) + 32956 * ~(aux0 & a & ((aux0 | aux0)
		^ (aux0 | a)) & (aux0 ^ a | ~aux0 | (aux0 | a) ^ aux0) & ((a
		& aux0 & (a ^ a) | aux0) ^ ~~aux0 & ((a | aux0) ^ a ^ aux0)
		| ~a & ~a & (aux0 ^ aux0 ^ ~a) | (~aux0 | a) & aux0) ^ (~((a
		| aux0) & (~a ^ (a | aux0)) | aux0 | ~a ^ a ^ a) | a)) +
		29607 * ~(~~~aux0 | (a ^ (aux0 & a ^ a) & (~a | aux0 & a)) &
		(~aux0 | ~(aux0 & a | a & aux0)) & ((a | a) ^ (aux0 ^ a) &
		(a | a) ^ (a & aux0 | a | a) ^ (~(a ^ a) | a & aux0) & (~a ^
		a ^ ~a))) + 14764 * (~(a & aux0 & aux0 & (~((aux0 & aux0 | a
		& aux0) ^ (~a | aux0 | a)) ^ a & aux0 ^ (aux0 & aux0 ^ aux0
		& a | ~(aux0 & a)))) & (a | a | a & ~(a & aux0 ^ aux0 ^ aux0
		^ aux0 & (a ^ aux0) ^ aux0))) + 17095 * ((a | aux0 & a ^ a)
		& a ^ a) + 51684 * ((a | aux0 | ~~(aux0 ^ a) | a & (aux0 ^
		a) ^ a ^ ~(a ^ aux0) & ~a | aux0) & ((aux0 & a | ~aux0) ^ (a
		| a) & aux0 & aux0 ^ (a | a | aux0 | aux0) ^ a & a & aux0 ^
		a ^ ~~aux0 ^ ~(a & a) & aux0 & aux0 ^ a | ~a)) + 277 *
		~((~~(a & aux0) ^ aux0) & a | ~aux0 | aux0) + 14685 * ((aux0
		| ~((aux0 ^ a ^ (aux0 | a) | (aux0 ^ aux0) & a & aux0) & ~(a
		^ (aux0 | a)))) & a & (a | a & aux0 ^ a & aux0) & ~(aux0 & a
		& (aux0 | aux0)) & aux0 | aux0) + 49627 * (a ^ ~(aux0 ^ aux0
		& aux0 & aux0 & (~aux0 | aux0 & a) ^ ~aux0) & (a ^ ~aux0)) +
		36163 * ((~(~(~aux0 | aux0 | aux0) | (aux0 ^ a | ~a) & aux0)
		| aux0 | a) ^ aux0) + 49284 * ((aux0 & ~(a ^ aux0) & ((aux0
		| aux0) ^ a) & ~(aux0 | aux0) & a & aux0 | a & a) & aux0 &
		aux0 | a & (~aux0 ^ aux0 & a ^ aux0 ^ a ^ a ^ a) & (aux0 |
		~aux0 ^ ~aux0) ^ a ^ ~(a & aux0 & a & aux0 & (a & ~aux0 |
		~aux0 ^ ~a)) & (aux0 | (aux0 | aux0) ^ aux0 ^ (~aux0 |
		aux0)) & ~(~(a | aux0) & (aux0 ^ a ^ ~a))) + 34187 *
		(~(((~aux0 | a) ^ aux0) & (aux0 & aux0 & aux0 | aux0 & aux0
		| a) & aux0) & ~~~(~aux0 & ~aux0 & (a & aux0 ^ aux0 & aux0))
		| (aux0 ^ (a ^ aux0) & aux0) & ~~(a | a) & ~(~a & aux0) &
		(~a ^ aux0 ^ aux0 | ~(aux0 | a)) ^ aux0 ^ ~(~~~aux0 & a ^ a
		& (a | aux0) ^ a ^ a ^ ~a ^ ~(~a | a | a))) + 13679 * (((a ^
		a ^ a | ~aux0 ^ (aux0 | a)) ^ aux0 ^ a ^ ~a ^ a ^ ~(aux0 &
		(a | a) | a & aux0 ^ aux0 | aux0 & aux0 & (a ^ aux0 ^
		aux0))) & aux0 & (~(aux0 & ~a & (~(a & aux0 ^ ~aux0) ^ (aux0
		| aux0 | aux0 & aux0 | (aux0 | aux0) & a))) ^ (a | (a & a |
		a | aux0 | ~~aux0) ^ a & a & aux0 & aux0 ^ aux0 & a & (a |
		aux0) ^ aux0))) + 46660 * (((~a | ~(~aux0 | aux0 & aux0) ^
		aux0) ^ ~(aux0 ^ a ^ aux0 ^ a ^ ~aux0) & (aux0 ^ (~a | a ^
		a) | (aux0 & aux0 ^ a) & (aux0 | aux0 | aux0 & aux0))) & a |
		~(~(a & aux0) & ~(a & aux0) ^ aux0) & (((aux0 | a) ^ (aux0 |
		aux0)) & ((aux0 | aux0) ^ ~a) & aux0 ^ aux0) | aux0) + 24096
		* (aux0 | aux0) + 41565 * (~a & a ^ ((a | (aux0 | aux0) & (a
		^ a) & ~(a ^ a) | aux0) & aux0 | (aux0 | a) & ((aux0 ^ a ^
		aux0 | aux0 & a & (aux0 ^ aux0)) ^ ~(a | aux0) & (a & a ^
		aux0 & aux0)) | ~((a & aux0 | a & aux0) ^ a ^ aux0))) +
		25975 * ~(aux0 & ~(~(a ^ aux0) & (a & aux0 | aux0) & a ^ (a
		| aux0 | a | aux0) & a & (a ^ (aux0 | aux0) ^ (~a | a)))) +
		34134 * ~(a & (a ^ a ^ a ^ (a | a) ^ aux0 & aux0 ^ aux0 ^ a
		^ aux0 ^ a & a ^ ~aux0 ^ a ^ a & aux0 & ~a ^ (a & a ^ ~aux0)
		& (a ^ a | ~aux0))) + 55708 * (~(a & (a | ~(aux0 & a) ^ ~a &
		(aux0 ^ aux0)) & ((a | ~a) ^ (aux0 | aux0 | aux0)) & ~(aux0
		^ a ^ (aux0 | a))) | aux0 & (a ^ a | a ^ a | ~a | a) & ~(a &
		aux0) & ~~a & (aux0 & a & (aux0 ^ a) ^ (a ^ aux0) & a & a) &
		(a ^ aux0 ^ aux0 & a & (aux0 ^ aux0)) & ((~~a & aux0 | ~~a ^
		~aux0 ^ aux0 ^ a | aux0 & ~~aux0) ^ ~((a ^ aux0 ^ aux0 |
		aux0) & (aux0 | ~a ^ aux0)))) + 3834 * ((aux0 | ~(a & ~a & a
		& aux0)) & ~((aux0 | ~(aux0 & a)) ^ (aux0 | (aux0 ^ aux0) &
		~a)) & ~~(a & ~(aux0 ^ aux0) | ~(a & aux0) | a & a ^ a) ^ (a
		| ~(aux0 ^ ~(aux0 | aux0 | a)) | ~a)) + 41635 * ((a & ~aux0
		& a ^ (aux0 ^ ~a) & ~(a | aux0) | aux0 | ~(aux0 ^ aux0) ^ a
		| (~aux0 | aux0) ^ ~~a ^ ~~~aux0 | (~(aux0 | a) | aux0 | ~a)
		^ (~aux0 | a | a) & (aux0 ^ a ^ (a | aux0))) ^ ((a | aux0) ^
		(a | a) ^ a ^ ~aux0) & (aux0 ^ ~aux0 & ~a) & (a ^ aux0 & a)
		& ~(aux0 | aux0) & ~(aux0 | aux0 ^ a) ^ ~(a & ~(aux0 ^ aux0)
		& a) ^ ~~a) + 3367 * (((aux0 | ~a) & ~a | (~aux0 | aux0 ^ a)
		& ~a & ((aux0 | aux0) & a & aux0 ^ ~(aux0 | a)) & (aux0 |
		aux0) & (aux0 & a ^ (~(a ^ aux0 ^ aux0) | a))) ^ aux0 ^ a) +
		48661 * ((aux0 & ~(aux0 ^ a) | ~a) ^ ~(aux0 | ~(a | a | aux0
		| a)) & ((~(a | a) ^ (aux0 | aux0)) & (aux0 & a ^ (a | a) ^
		~(a ^ a)) | a) ^ (~~(a | a) ^ (~(a | aux0) | a & aux0 | ~a)
		| (~(aux0 ^ aux0) | aux0 | aux0 | aux0 ^ aux0) & (~(a | a) |
		aux0 ^ a | ~aux0)) ^ a) + 42049 * (~(aux0 | a) ^ aux0 ^ ~a ^
		((a | aux0) ^ (aux0 | a) | aux0 | a ^ aux0) ^ (aux0 | ~(a &
		aux0 | a | aux0)) | ~a | aux0 & (aux0 ^ (a ^ ~aux0) & (a & a
		| a & aux0) & (a ^ (aux0 ^ a) & a & a)) | ~(~~(aux0 | aux0 ^
		aux0) & (a | aux0) ^ (~a | (aux0 | aux0 | ~(aux0 | aux0)) ^
		((a | a) ^ a ^ a | ~aux0 ^ aux0 ^ aux0)))) + 47079 * ((~(~a
		^ ~a & a & (aux0 ^ a | aux0 | aux0) | a & (a | aux0) & a & a
		& (~(a | a) | aux0 | a & aux0)) ^ ~aux0 ^ (aux0 | aux0)) &
		~(~(~(a | aux0) | (a | a) ^ aux0 & a) & (aux0 & aux0 ^ (a |
		aux0) | ~a | ~aux0 & aux0 | a ^ a ^ aux0 & a)) & aux0 & (~~a
		& aux0 & a & aux0 ^ ~(a & aux0 & (a | aux0)) ^ (~(a & aux0)
		| a | ~(a & a | ~a)))) + 25943 * ((~(~a & a & a | (a ^ aux0)
		& aux0 | (a ^ aux0 | ~aux0) ^ ~(aux0 & a)) ^ (~a ^ a | a)) &
		(aux0 ^ (~aux0 | (aux0 ^ a) & aux0) & aux0 ^ aux0) & (a |
		(~((a ^ aux0) & ~aux0) | aux0) ^ ((a | a) ^ a) & ~aux0 &
		(aux0 ^ a) & aux0 ^ (a | ~((aux0 | a) ^ (a | aux0) ^
		aux0)))) + 40552 * ~((aux0 ^ ~(a ^ aux0 ^ ~aux0 | aux0 |
		aux0 | ~aux0) ^ ~aux0 & ~aux0 ^ a ^ (a | a) ^ a ^ (a & a |
		aux0 ^ aux0)) & aux0) + 60514 * ~(~(((a ^ a) & a & aux0 & a
		& (aux0 ^ a) | ~~(aux0 & a)) ^ (aux0 ^ a & aux0 | ~aux0 &
		aux0 | a)) & (a ^ ~~((a & a | a | aux0) & a))) + 61970 *
		(~((a | a) & (aux0 | aux0) | aux0 & aux0 ^ aux0) & (a ^ ~a)
		& ((a | ~aux0) ^ a) & ~(((a ^ a | a ^ aux0) ^ (a | a) ^
		~aux0) & (a | a & a & a) & (~((a | a) ^ ~aux0) ^ (aux0 &
		aux0 | ~a) ^ (~aux0 | aux0 ^ aux0))) | ~~(~(a & a) ^ aux0 |
		(aux0 & aux0 | ~aux0) & (a & a ^ a)) | ~a) + 15493 *
		(((~(~(a ^ aux0) ^ ~a ^ a ^ aux0) | aux0 ^ (a | a) ^ (~a | a
		^ a) ^ a & a & ~aux0 ^ (~a | a | a)) & (~aux0 & a ^ (aux0 ^
		a | a & aux0) | (a | aux0) ^ (a | a) ^ (a | aux0 & aux0) |
		(~aux0 | ~aux0) & ~(a & a) & a) ^ (a & aux0 ^ ~(~a & (a |
		aux0)) ^ aux0 | ~((~(aux0 & a) | aux0) ^ ~(a & aux0 & aux0 &
		aux0)))) & ((a | a | aux0 | aux0) ^ aux0 ^ a & a ^ ~aux0 ^
		~aux0 & a & ~(aux0 ^ aux0) & ~(aux0 ^ a) | ~((aux0 ^ a |
		~aux0 | aux0 ^ aux0 ^ ~a) ^ ~(a ^ a ^ a))) & a) + 30617 *
		((~((aux0 | aux0) & (aux0 | a) & (aux0 ^ a) & a & a) |
		((aux0 ^ aux0) & (a ^ aux0) | aux0) & aux0 & (a & a ^ (a |
		aux0))) & (aux0 | aux0 & ~(aux0 ^ a)) & ((a ^ ~aux0 | a) ^ a
		& (a | a)) & (~aux0 | aux0 | ~((a ^ aux0) & a & a) | aux0 |
		aux0 | ~a | aux0) | a ^ (~((aux0 | a & a) & ((a | a) ^ a) |
		a ^ (aux0 | a) | aux0) | (aux0 ^ a ^ aux0 ^ a ^ (aux0 ^ a) &
		~aux0 | ~aux0) & ((a | aux0 | ~aux0) ^ aux0 & aux0 ^ ~aux0 |
		aux0))) + 59604 * ((~((aux0 | a) ^ a) | a | aux0 | a | a ^
		aux0) & ((~~aux0 | ~(aux0 ^ aux0)) ^ ~(aux0 | aux0 | aux0))
		^ (~~~(aux0 ^ aux0) | ~~a | ~(aux0 | a) | aux0 ^ aux0 ^
		aux0) ^ aux0 ^ a ^ ~~aux0 | ~((a & (a ^ aux0 ^ a) ^ ~(aux0 &
		a) | (~aux0 | aux0 ^ aux0 ^ ~aux0) ^ (aux0 ^ a | aux0 & a) ^
		aux0) & aux0)) + 50397 * (aux0 ^ ((a | (a ^ a ^ ~a) & (aux0
		& a ^ (aux0 | aux0)) ^ a) ^ ((a & a | aux0 & aux0) ^ (a |
		aux0 | a | aux0) ^ aux0 ^ aux0 ^ aux0 ^ a ^ (a | aux0) |
		~(aux0 & ~a & aux0 & (a ^ a))) | ~(aux0 ^ a | a ^ aux0 |
		aux0 ^ a | a | a | (a ^ a) & (aux0 ^ aux0) ^ a & a & a) &
		aux0)) + 25521 * ~(aux0 & ~aux0) + 48079 * (~a & (((a | a) &
		(aux0 | aux0) | ~(aux0 & aux0)) ^ aux0 | aux0 | ~a) & ~((a |
		aux0) ^ aux0 ^ ~(a ^ aux0) | aux0) & (a | a)) + 6669 *
		(((~(aux0 & a) | a & aux0 | ~a) & ~(aux0 | aux0 | a) ^ a &
		(~aux0 & a ^ a)) & ~((aux0 ^ a | a | a) & ~(a ^ aux0) ^ ~(a
		| a) ^ a & aux0 & a) ^ aux0 ^ a) + 7011 * (aux0 ^ (a | aux0
		| aux0) ^ (aux0 | ~aux0) ^ ((((aux0 | a) ^ (a | a)) & ~(a &
		aux0) ^ (~a | a & aux0) ^ a & a & ~aux0) & a | ((aux0 ^
		aux0) & a | aux0) & ((aux0 | aux0 | aux0 ^ a) ^ ~(aux0 ^ a))
		& (a & a ^ aux0 ^ a ^ ~~a | ~~aux0 & ((aux0 | a) ^ aux0 &
		a)) | (aux0 & ~a ^ ~~a) & aux0 & a & aux0 ^ ((a | a) ^ a &
		aux0) & (a ^ a) & ~a & a | aux0 | a ^ (~(aux0 & a) | ~(aux0
		^ aux0)))) + 34673 * (aux0 | (~aux0 | aux0 ^ (aux0 & a | a |
		aux0) | a & (aux0 ^ aux0 ^ ~aux0) ^ ~aux0 & a | a) ^ (~a ^
		~(~aux0 | aux0 ^ a) | ~~(a & aux0) & ((a | aux0) ^ ~a |
		(aux0 | a) & aux0 & aux0)) ^ a) + 33307 * ~a + 15501 *
		(~~(~((aux0 ^ aux0 ^ ~a) & aux0) | ~(aux0 & a) ^ (aux0 |
		aux0 | ~a) ^ aux0) | (~~aux0 & (a ^ aux0) | a ^ (a & aux0 |
		a & a) | a) & ~((a | a | a | ~aux0 ^ a ^ a) ^ aux0) &
		((~aux0 & (a ^ aux0) | aux0 | (~a ^ aux0 ^ a) & aux0 & a &
		a) ^ (a ^ aux0) & ~(a | a) & (a | ~aux0 | aux0 | ~a) ^
		aux0)) + 2950 * ~~((~~~a ^ ~(~a & a) ^ a) & ~a) + 41787 *
		((aux0 & ~(aux0 | ~(a | a)) | ~~(~a | a | (aux0 | aux0) ^
		~a) | ~a & a & (~(a | a | ~aux0) ^ (aux0 & aux0 | ~aux0 | a
		| ~a)) | aux0 ^ ~(aux0 & aux0 ^ a & aux0 ^ a ^ aux0 & a)) ^
		aux0) + 21700 * ~~(aux0 & (~(a & a ^ aux0) ^ ~a ^ (aux0 ^
		aux0 | aux0) & a & a & aux0 ^ (aux0 | aux0) & a)) + 25459 *
		~(~~(aux0 & aux0 | ~aux0) ^ ~(~(aux0 & aux0) & a & a & a) ^
		aux0 | ~((aux0 ^ aux0) & (a ^ a) & aux0 & aux0 & (aux0 ^
		aux0) & (aux0 ^ aux0 ^ ~a) & ((a | a | a ^ aux0) & ~(a | a)
		| aux0 & a))) + 61042 * ~(((a | a) & aux0 ^ (aux0 | a) ^
		~aux0 ^ (~(aux0 ^ aux0) | aux0 & aux0 & (a | a)) | ~(a ^
		aux0) ^ ~aux0 | ~((aux0 | a) & (aux0 ^ aux0))) & ((aux0 |
		aux0 | a | ~a) & a | ~(a ^ aux0) | aux0 | (a ^ aux0 | a ^ a)
		^ (~aux0 | a | aux0)) | a) + 58326 * (~(a ^ aux0) | ~(aux0 ^
		a & a & ~a ^ ~(a ^ a) ^ (~aux0 | ~aux0) & (aux0 ^ a ^ aux0)
		& ~(aux0 | ~aux0)) | ~~~(~aux0 & ~a) & ((~~aux0 | ((aux0 |
		a) ^ aux0) & ((a | aux0) ^ a)) ^ aux0) | ((aux0 & aux0 | a)
		& aux0 ^ aux0 | aux0) ^ (a ^ ~(aux0 ^ aux0 ^ a) | ~aux0 ^
		(~a ^ (aux0 | aux0)) & ((aux0 | aux0) ^ a & aux0))) + 9097 *
		(((a | (a ^ aux0) & ~aux0) & ~(aux0 | aux0 ^ a) ^ aux0) &
		aux0 & (a & ~a ^ a & aux0 ^ aux0 ^ aux0) & aux0 & ~(((aux0 |
		aux0) ^ a ^ aux0 ^ ~(aux0 | a) ^ ~(aux0 ^ aux0) ^ a) &
		~aux0) ^ (a ^ aux0 & a | a | ~~aux0 ^ aux0) ^ aux0) + 12904
		* ~(~(aux0 | (a | ~aux0 | ~(a ^ aux0)) ^ ~(aux0 | aux0 | a ^
		a)) | ~a) + 9697 * (~aux0 & (a ^ ~a & ~(~a | ~aux0)) & (a ^
		a) | a & (~~a ^ (a & a | aux0) | (a | aux0) & aux0 & a |
		(aux0 ^ aux0) & a & a | a) & (~~(~aux0 | a ^ a | (aux0 | a)
		& ~aux0) | ~(aux0 | (aux0 ^ a) & (aux0 | a) ^ (aux0 & aux0 |
		a)))) + 44966 * ((aux0 ^ (aux0 ^ aux0 ^ ~aux0) & (a | aux0)
		& (a | aux0) ^ (~aux0 ^ (a ^ aux0 | ~a)) & aux0 ^ aux0 ^ a)
		& (aux0 | a) ^ ~a ^ (a | (a | (aux0 ^ a) & a) ^ ~(aux0 ^
		~aux0) | (a ^ a ^ aux0) & (aux0 ^ a ^ (a | a)) ^ (~aux0 ^
		~aux0) & a)) + 44330 * ((aux0 | ~~~(aux0 | aux0) ^ (~(~aux0
		& (aux0 ^ aux0)) | a & aux0 & aux0 & aux0) & ~((aux0 | aux0
		& aux0) & (aux0 & aux0 | a & a))) ^ a) + 59170 * ~~(a &
		(aux0 | ~aux0 | aux0 | ~(a | a)) | a) + 51199 * ((aux0 ^ ~(a
		| ~(aux0 | a | aux0) & a)) & ~((~(a ^ (~a | a | aux0)) |
		~~(aux0 & a) | aux0) ^ a)) + 38450 * (~(a & (a & aux0 & (a ^
		a) & (a ^ a | aux0 & a) & (~(aux0 | aux0) ^ ~a) ^ (~(~a ^
		aux0 & a) | ~(a | aux0) & ~aux0 & (aux0 | aux0)))) & ((~(a &
		aux0 ^ a & a) ^ a ^ ((aux0 | a) & (aux0 | aux0) ^ ~aux0) &
		~aux0) & ((aux0 | aux0 | aux0 & aux0 | ~aux0 | a) & (aux0 ^
		a ^ ~aux0 | ~a) | ~(aux0 ^ a) & ~aux0 & ~aux0 & a) ^ aux0 ^
		a)) + 40103 * (~(~~(~aux0 & aux0 & a) ^ aux0 ^ a) ^ a) +
		58430 * (~a ^ a) + 59637 * (aux0 & aux0 ^ (aux0 | a) ^ (a ^
		aux0) & (aux0 ^ aux0) ^ (~aux0 & (a ^ aux0) | ~a | aux0) ^
		~(~aux0 ^ (a | aux0)) & (~(a | aux0) | aux0 & a ^ aux0 ^ a)
		^ ((aux0 | a) & aux0 | ~(a ^ aux0)) & aux0 & (~(aux0 ^ aux0
		& aux0) ^ (~aux0 ^ (aux0 | a)) & (a ^ aux0 & a)) | (aux0 |
		~((a ^ a) & (~aux0 ^ ~aux0))) & (~a | (a ^ aux0 | ~a) & (~a
		^ (aux0 | a))) & a | aux0) + 45247 * (a | a ^ (aux0 ^
		~(~(aux0 & aux0) | aux0)) & aux0 & (~~a ^ a) & ((aux0 ^ a ^
		a) & (aux0 ^ a ^ ~a) ^ ~(aux0 ^ ~a))) + 46849 * (~(aux0 |
		(aux0 ^ a | a & a) & a) & (aux0 ^ a) & (aux0 ^ ~(aux0 ^ (a |
		aux0))) ^ ~(a | aux0) & ((~a | a & aux0) ^ a & a & ~aux0) &
		a & aux0 & aux0 & (aux0 ^ aux0) ^ ~~(aux0 | a | aux0 | aux0
		^ a) ^ ~((aux0 | (a | a) ^ a ^ (aux0 | aux0) ^ (a | a)) ^ (a
		| ~~a ^ a) | a & aux0 & (a ^ aux0) & (aux0 | a) & (a | a |
		aux0 | aux0 | a ^ aux0 | aux0 | a))) + 23200 * (~(aux0 ^ (a
		^ a ^ a & a) & a) & ~(aux0 ^ aux0) & ~(a ^ a) & a & (aux0 |
		a & (~a | a)) | ~~(aux0 ^ a ^ a | aux0 & aux0 & (a ^ aux0))
		| ~(~(a ^ aux0 ^ (aux0 | a)) | (aux0 & a | ~a) ^ a) | ~(aux0
		^ (~a | ~a) | aux0 ^ a | (a | a) ^ aux0 | a) & (~(aux0 & a &
		a) & ((aux0 & aux0 | aux0 & a) ^ ~(aux0 & a)) ^ (aux0 ^ aux0
		| aux0) ^ ~aux0 & (a | a) ^ (aux0 | aux0) ^ a ^ a | ~(~a ^
		(aux0 ^ a ^ (a | a)) & aux0))) + 5577 * (a ^ aux0 ^ (~(a | a
		^ aux0 & a) & (a ^ a ^ a & ~a) | ~(a | a)) ^ ~(~(aux0 & a) |
		aux0 & aux0 & aux0 & (aux0 ^ a | a | aux0) & ~aux0 ^ (a | (a
		| a) ^ ~aux0) ^ (aux0 & a | a & aux0) ^ aux0 & a)) + 54936 *
		(aux0 & (~(~aux0 ^ ~a ^ aux0 & (aux0 | a)) ^ (aux0 ^ aux0 ^
		a) & a & (aux0 | ~(a & aux0))) ^ ~(~a & a) ^ (a | ~((a | a)
		& aux0 | aux0)) | a) + 16671 * (aux0 ^ a & ((aux0 & aux0 & a
		| ~aux0 ^ (a | a) | aux0) ^ ~~(~a | a | a)) & ~(aux0 & (a |
		aux0) & (a ^ a) ^ aux0 & aux0 & aux0 & a & a & ~aux0)) +
		1140 * ~(~(aux0 & a & aux0 | aux0 | a | aux0 | a | aux0 ^
		aux0 & (a ^ a) | ~((a & aux0 | a ^ a) ^ ~~aux0)) | aux0 &
		((aux0 | ~(a ^ a ^ a ^ a)) ^ (~((a ^ aux0) & ~a) | ~(~a ^ a
		^ aux0)))) + 9397 * (aux0 ^ aux0) + 62983 * (~~~~a &
		~(~(~aux0 & aux0 & a) ^ (aux0 ^ a ^ a & aux0) & ~aux0 |
		aux0) ^ (a | ~(~(aux0 ^ aux0 ^ aux0) ^ (aux0 | aux0) ^ aux0
		& a & a & a ^ aux0))) + 62827 * (~(a ^ a & ~~a & ((a ^ a ^ a
		^ a) & (aux0 & aux0 ^ a ^ a) ^ aux0)) | ~(~(aux0 & ((a |
		aux0) ^ (aux0 | aux0) ^ ~a)) ^ (((a ^ aux0) & ~a | ~(a ^ a))
		^ (a & a | a) & (a ^ (aux0 | aux0)) | ~(~a & (aux0 | aux0) |
		~~a)))) + 7774 * (((aux0 ^ aux0) & ~aux0 ^ aux0 ^ (~(a |
		aux0) | ~a ^ a ^ a)) & (~a | a) | aux0 & (aux0 | aux0 | a |
		aux0) & ~(aux0 ^ a ^ ~a) ^ (~a ^ (aux0 | aux0) | ~a | a &
		aux0 | aux0) | (~(aux0 & aux0) & (a ^ ~a) | ~(~a | a ^ aux0)
		| (aux0 | a) & aux0 & a & (aux0 | aux0) & aux0 ^ a ^ ~aux0 &
		aux0 & aux0) ^ ~a | aux0) + 37034 * ((a | ~(a & (a ^ aux0))
		^ ~(aux0 & aux0) ^ a) & ~((~(a | aux0) | aux0 | aux0 ^ aux0)
		^ ~(a & aux0 & (aux0 ^ aux0))) ^ ~a ^ a) + 17436 * ((a | a)
		& a & (~aux0 | aux0 ^ (aux0 | a & aux0 & (aux0 ^ a)) ^ ~((a
		| aux0) ^ (aux0 | aux0)) & (aux0 | a | aux0) & a)) + 43555 *
		(a & ((aux0 & ((aux0 ^ aux0) & (a ^ aux0) | ~~a) & (a |
		~(aux0 & a)) | ~(aux0 & a & (aux0 | a) & ~a ^ (~(a & aux0) |
		aux0 ^ aux0 | aux0 & a))) ^ (~((~(a & aux0) | ~aux0) & (aux0
		^ a) & a & a & a) | ~(aux0 ^ (aux0 ^ a ^ aux0 & a) &
		aux0)))) + 27905 * ~((~(~(aux0 & aux0) ^ (aux0 & a | aux0 |
		a)) & aux0 ^ ~aux0) & ((~(aux0 & a) & (a & aux0 | aux0) ^
		(aux0 & aux0 & ~aux0 | aux0 & a | a | a) | ~(~aux0 & ~aux0)
		^ (a | a | aux0 ^ aux0) ^ (a | aux0) & (aux0 ^ aux0)) ^
		~~a)) + 19339 * (aux0 | ~(~(a & (a | a)) & ~~a | ~a | a |
		~a)) + 20134 * ((~((a & a | ~a | aux0 & a ^ a) ^ ~a ^ ~a &
		~a ^ (a ^ a & (aux0 | aux0) | a)) ^ ~(a | aux0 ^ a & a ^ a |
		aux0)) & (~(aux0 & aux0 & a & (aux0 | a | a ^ a) | ~(aux0 &
		aux0) & ~(a | a) | aux0 | a & aux0 | aux0 | aux0) | (a ^
		aux0) & ((~aux0 | ~a) & (a ^ aux0 | a) ^ (aux0 ^ aux0) &
		~aux0 & ~a & a & aux0) & (a | ~a ^ (aux0 | a)) & ~(a | a) &
		(aux0 ^ a) & ~a)) + 20735 * (~((a | ~a | ~a) ^ (aux0 | a | a
		| a) ^ a ^ aux0 | ~(~a ^ a | a & aux0 ^ (a | aux0))) | ~(((a
		| a & aux0 & a) ^ (a ^ aux0 ^ a ^ aux0 | aux0 & aux0)) & a)
		| ((~aux0 ^ a ^ aux0) & ~a & ~a & ((a | aux0 | a & a) ^ a ^
		~aux0) ^ (aux0 & a & ~a | aux0 | ~((aux0 | a) & (a | a))) |
		~~(~a | aux0) & (~a & (a ^ aux0) | a & aux0 | aux0)) &
		~(aux0 | ~aux0 | ~(~a & (aux0 | a)))) + 2739 * ~(aux0 & aux0
		& (~aux0 ^ aux0 & a | ~aux0 | a) & aux0 | (~((a | aux0) &
		(aux0 | a)) | ~~~a) ^ aux0 ^ (aux0 | aux0) & a) + 22226 * (a
		^ aux0 ^ aux0 ^ (aux0 | a | ~aux0) ^ aux0 ^ (aux0 | aux0 |
		a) & a ^ aux0 & a ^ ~(~((a ^ aux0 ^ a) & ~(a ^ aux0) & a) ^
		((aux0 | aux0) ^ a ^ ~(aux0 & a)) & ~~(aux0 & a) & ~~(a &
		aux0 | aux0 | a))) + 17647 * ((aux0 & ~(a & aux0 ^ a ^ aux0
		^ ~aux0 ^ a & aux0) & ((aux0 ^ a | a | a) & ~(aux0 | a) ^
		~(aux0 | a) ^ ~~aux0) | (~~(~aux0 ^ aux0) | (a | a | aux0 |
		aux0) ^ (a ^ a | a ^ aux0) ^ (a ^ a) & (aux0 ^ a)) & (~~a |
		aux0 ^ a ^ (aux0 | a) | ~~a & ~aux0) & ~(aux0 ^ (aux0 | aux0
		| a & a))) & aux0) + 60072 * (((~aux0 & ~a ^ a | aux0 ^ (a |
		aux0) & aux0 & a) & (a | (a ^ aux0) & ~a | a) ^ ~(~aux0 ^ a
		^ a | a) ^ ~~(a | a | aux0 ^ a)) & (~aux0 | ~a & ~aux0) ^ a)
		+ 51451 * (((a | a) ^ (aux0 | aux0 | aux0 | a) ^ ~(a & aux0
		^ ~aux0) | a) & (~aux0 | a) ^ ~a ^ (a | aux0) ^ (aux0 | a) ^
		(a | a) & (aux0 ^ a) ^ aux0 ^ ~(aux0 & aux0 ^ a & aux0 | (a
		| a) ^ a & a | ~(aux0 & a & aux0)) ^ (aux0 | ~aux0 & (aux0 ^
		a) & (a ^ a) ^ a) ^ ~(a ^ (aux0 | aux0 | a ^ aux0 | (aux0 |
		aux0) ^ (a | aux0)))) + 16110 * (~(a & ~(a & aux0) ^ aux0 ^
		aux0) | aux0 | ~(~aux0 & a & a & aux0) & aux0 & ((aux0 ^ a)
		& (a | aux0) ^ a ^ a ^ ~aux0) | ((a ^ (a | a) ^ a) & (a ^
		aux0 ^ aux0 & aux0 ^ (a & a | ~a)) | ~(~(a | a) ^ ~aux0 & a
		& a) | a | ~(aux0 ^ a ^ ~aux0) | aux0 ^ a) & ~(~(a & aux0 &
		aux0 & aux0) ^ a & a | aux0 ^ ~(aux0 | aux0) ^ a)) + 49998 *
		(aux0 & (~aux0 ^ ~(~~aux0 & ~~a & a) ^ ~(a & aux0) ^ (~a &
		(aux0 | aux0) & aux0 & aux0 & a | a ^ aux0 ^ a | ~aux0 | a ^
		a | (a | aux0 | ~a) ^ ~aux0 & (a ^ a)))) + 64353 * (~(aux0 &
		aux0 & a) ^ (aux0 ^ a | ~~(a | a) & (~aux0 & a | a & aux0 |
		~aux0)) ^ (a ^ aux0) & aux0 & (a ^ a ^ ~aux0) ^ (~((aux0 |
		aux0) ^ a) | aux0) | ~(~(~(a ^ a) & (aux0 | aux0 | aux0 |
		aux0)) & (a & aux0 & a & a ^ (aux0 | a & a | aux0))) &
		(~~(aux0 & a) & (~(aux0 | aux0) | ~(aux0 ^ a)) ^ ~(~a &
		(aux0 | a) & ~(aux0 | aux0)) ^ (~(aux0 | aux0) ^ ~a ^ a & a
		| ~(a | aux0) | a | a))) + 13659 * (~(~(aux0 | (aux0 | a &
		a) ^ a) | aux0) & a) + 45823 * (~(a | (aux0 | aux0) & (a |
		a) & (a & a | aux0) | ~~(aux0 ^ (a | aux0)) | ((aux0 | aux0)
		& (a ^ a) ^ ~a ^ ~a | a ^ (a | aux0) | a ^ a ^ a) ^ ~~(aux0
		& (a | a))) ^ (~~(aux0 | ~(a & a)) ^ aux0) & ((~(aux0 ^ a) |
		~aux0 | aux0) ^ ~(a | aux0) & (a ^ aux0 ^ aux0 & aux0) | a)
		& (aux0 ^ ((aux0 | a) ^ a & a) & ~aux0 & aux0 & aux0) &
		~(aux0 & aux0 ^ (a | a) | ~(a ^ a))) + 58306 * ((a & a ^ a)
		& (((aux0 | aux0) & ~a & ((aux0 | a) ^ a & aux0) ^ ~(aux0 ^
		~aux0) | ~(aux0 | aux0) & (aux0 & aux0 ^ a ^ aux0) & ~a) ^
		(a ^ ~aux0 | ~(aux0 & (a ^ aux0))) ^ aux0) ^ (~~((aux0 ^ a)
		& a) ^ ~a) & a & ((a ^ aux0 ^ a ^ aux0 | aux0 ^ ~aux0) ^ ~(a
		& aux0) ^ a ^ ~(aux0 ^ a ^ (a | aux0)) ^ (~aux0 ^ (a | a) |
		aux0 | a) | ~(~(aux0 & aux0 ^ a) | ((aux0 | a) ^ ~a) & (a ^
		~aux0)))) + 24981 * ((~~~(aux0 ^ aux0) & (~~(a & a) ^ aux0 &
		(a ^ a ^ ~a)) | ~a & a | aux0 & (aux0 & aux0 | aux0 & aux0)
		| ~((a | aux0 | aux0 ^ aux0) & ~~a)) ^ (a & (a ^ ~a & (a |
		a)) | ((a ^ a | aux0) ^ aux0 ^ aux0 ^ a) & (a | ~aux0 | aux0
		^ a)) ^ ~((a | aux0 | aux0 ^ aux0) & ~aux0 & ~((aux0 | a) ^
		(aux0 | aux0))) ^ ~~~a) + 14528 * (~(~(~(aux0 | aux0 | ~a) |
		aux0) | ~(a | (aux0 ^ aux0) & (aux0 | aux0)) ^ (aux0 | ~a |
		aux0 | a | ~(a | a))) & ~(aux0 & aux0 & (aux0 ^ (a | aux0 |
		aux0 ^ aux0) ^ (aux0 | a) & (a ^ a) ^ a ^ a | ~~~(a ^
		aux0)))) + 59427 * (aux0 & ((aux0 | a) ^ (~(aux0 ^ aux0 | a
		& aux0) | a | a | aux0 | a | a) ^ (aux0 ^ a ^ aux0 & a) &
		~~a ^ aux0) | ~~aux0 ^ aux0 & ((~a & (aux0 | a) | (aux0 ^
		aux0) & (a | a)) ^ a | ~((a | aux0) & (aux0 ^ aux0) & (a ^
		a) & ~a))) + 14616 * (aux0 & ~(aux0 & ~(aux0 & a & a)) | a)
		+ 33877 * ~((aux0 | ~a | ~(a & ~a & ~aux0 | (a | a) & (a |
		aux0) & (a | a | aux0))) & (~~a ^ (~aux0 ^ a | ~~~aux0)) &
		(~((a & a ^ aux0 ^ aux0) & (aux0 & a ^ ~a)) ^ ~aux0 &
		(~(aux0 ^ a) | ~(a | a)))) + 21122 * (~a & (~aux0 ^ ((~aux0
		| aux0) & aux0 | a) & ((aux0 | aux0 & aux0) ^ aux0 & a ^
		aux0 & a) & (~(a | aux0) | a & ~a) | (~a | aux0 | aux0) &
		((a | aux0 | a ^ aux0 | ~a | a) ^ ~(aux0 | a) & ~(a & a)) ^
		~~(aux0 | aux0) & aux0 & ((aux0 ^ a | a & a) & ~(aux0 ^ a) ^
		~(aux0 | a)))) + 17397 * ((~((a ^ ~aux0) & ~(aux0 & aux0)) &
		(aux0 ^ aux0 & aux0) & (aux0 | aux0) & aux0 & ((aux0 ^ aux0)
		& a & aux0 ^ aux0 ^ a & aux0) ^ ~~(~(a | aux0) | a)) &
		~~~(aux0 ^ a | a | a) & ~(~(a | aux0 | aux0 ^ aux0) ^ a) |
		((a | a) & (aux0 | aux0) | a & a & a & aux0) & aux0 & aux0 &
		(~a ^ ~(a | a) & (a ^ a ^ (a | a))) | aux0 ^ ~(aux0 | aux0 &
		aux0) ^ ~(~aux0 ^ aux0 | a) | a ^ ~a ^ ~a ^ a ^ aux0 ^ ~(~a
		& aux0) ^ (aux0 | aux0) ^ ~(a & aux0 | aux0 | a) ^ (a & (a ^
		aux0) & (a | a) ^ (~a | a | aux0) & ~a) & (a ^ (a ^ aux0 &
		a) & a)) + 54232 * (~(aux0 & (~(aux0 ^ aux0) | a ^ ~aux0) ^
		(~((aux0 | aux0) ^ (a | a)) | (a | ~a) ^ aux0)) & a & (~a |
		aux0 | aux0 | aux0 & aux0 & ~a) & ~((~a | a & aux0) & aux0)
		& a & (~aux0 | (a | aux0) ^ (~a | ~a) | (aux0 ^ aux0 & aux0
		| a & aux0 ^ (aux0 | aux0)) ^ ((aux0 | a) ^ a ^ a) & ~~aux0
		| aux0 ^ ~aux0 | a)) + 57642 * (~(~((aux0 | a | aux0 ^ a) &
		aux0) & aux0) & (aux0 | ~a | (~(aux0 ^ aux0) ^ (aux0 | aux0
		| aux0 | aux0) | (aux0 | a) & (a | a) | a | ~a) & aux0) |
		~(aux0 | a | aux0) | ((a ^ aux0 ^ a) & (a | aux0 | aux0 ^ a)
		| ~(a ^ a) ^ a | (aux0 ^ a | aux0 ^ a) ^ ~aux0 ^ a) & ((a &
		a | ~aux0) ^ ~a | a ^ aux0 & a ^ (aux0 | a)) & (~a | a) &
		(~a ^ a & aux0) & ~((a ^ a) & (aux0 ^ aux0))) + 20221 * (((a
		& aux0 | ~aux0 | ~~aux0) & (aux0 | aux0) & ~aux0 & ~(aux0 &
		a) ^ (~a ^ ~a | ~aux0 | aux0 | aux0) ^ (aux0 | aux0 & (aux0
		^ a)) ^ aux0) & ((~(~aux0 | a & a) | ~~(a | aux0)) ^ (~~(a &
		a) | a) ^ ~~((aux0 | aux0 | a | aux0) & (~aux0 | aux0))) |
		(aux0 | (aux0 & aux0 ^ aux0 ^ a) & (a & a ^ a) | ~a) & (a &
		aux0 | (aux0 ^ aux0) & (aux0 | a) ^ aux0 ^ (aux0 | aux0) ^
		((a | a) & a | ~(aux0 ^ aux0))) ^ ~aux0) + 19155 * ((~~(aux0
		| aux0) | ~(a | a | a) | aux0 & (a ^ a) & (aux0 | aux0) & ~a
		^ (a | ~~a)) ^ a & ~a & (((a ^ a) & (aux0 ^ aux0) | a & a |
		a | a) ^ ~(aux0 & a | a | aux0)) ^ (~(a | aux0) | (aux0 |
		aux0) ^ a & aux0 | aux0) ^ (a ^ ~aux0 & (a | a)) & aux0 ^
		(~a | a) & a | ~~(~((a | aux0) & aux0 & aux0 ^ a) | aux0 |
		aux0)) + 48438 * (a ^ ~(a ^ ~(aux0 & aux0 & ~a ^ ~(aux0 |
		a)) ^ ~a & (aux0 | a) & ~a & (a | a | aux0 | aux0 | ~(aux0 |
		aux0)))) + 54521 * (~((aux0 ^ aux0 ^ ~aux0 | aux0 & (aux0 ^
		a) | (a & aux0 | aux0) & a | ~(~a ^ a & aux0) ^ (~a ^ (a |
		a) | ~aux0 & aux0)) & (aux0 ^ ~(a ^ aux0 ^ a) & ~(aux0 |
		~a))) | (~((~aux0 ^ a & aux0) & (a ^ aux0 & a) ^ (aux0 ^
		aux0 ^ a | aux0 & aux0 | aux0 & aux0)) | a) ^ ~aux0) + 52440
		* (a & (~(a ^ aux0 ^ aux0 | (a | a) & (aux0 ^ aux0)) | a |
		a) ^ (a & ~a | ~~aux0 | (a ^ a ^ a ^ a ^ aux0) & aux0 |
		(aux0 | a | ~a | ~(aux0 | a)) & ~~~a) & ~aux0) + 429 * (a &
		~(aux0 ^ (aux0 | aux0) ^ ~a | ~(aux0 & a)) & (aux0 | aux0 |
		aux0 ^ a ^ aux0 & a | a & aux0 & a | a ^ (aux0 & aux0 & aux0
		& ~a | ~(~aux0 ^ aux0))) & ~a) + 26978 * (((aux0 ^ ((aux0 |
		a) & ~aux0 | a & a ^ (aux0 | aux0)) & ~~(a ^ a)) & ~((a |
		aux0 | a | a) ^ ~~(a & aux0)) ^ (~(aux0 ^ (aux0 | a & aux0))
		^ (aux0 | (a ^ aux0) & aux0 & aux0 & (a | aux0 | aux0 ^
		aux0))) & a) & aux0 & a) + 7622 * ~(~(a & ~a ^ ~~(aux0 & a &
		(aux0 | aux0))) & (aux0 | aux0 | aux0 | aux0 | ~a ^ aux0 ^
		aux0 | ((aux0 ^ aux0) & a ^ ~aux0 ^ a) & ~(a | a | aux0))) +
		16737 * (aux0 ^ (aux0 ^ aux0 | ~(aux0 | aux0 ^ a | aux0 ^
		aux0 | a & a | a & aux0 & aux0 & a | ~aux0 & a) | ~(((aux0 |
		aux0) & ~a | a | aux0 | a & aux0) & (aux0 ^ (aux0 | a) |
		(aux0 ^ aux0) & aux0 & a)))) + 56511 * ((~a & (a ^ a) & ~a &
		a ^ a & aux0 & ~aux0 & (aux0 & a | a) ^ (a ^ a) & (aux0 ^
		aux0) & a & (aux0 | aux0) | ~~(a & a & aux0 ^ a)) & ~(aux0 |
		a & (aux0 | aux0) & ~(a | a) | aux0 ^ aux0 ^ ~aux0 ^ aux0 ^
		aux0 ^ ~aux0 ^ (a & a | aux0 ^ a)) ^ ~(~((aux0 | aux0 | ~a)
		& aux0 ^ ~((a | a) ^ a)) ^ aux0 & aux0 & ~aux0 & (aux0 ^ a &
		a & aux0))) + 8285 * ~((~(~~aux0 ^ ~aux0 ^ (a | a) ^ ~~(a ^
		aux0)) ^ ~(aux0 & aux0 ^ a & a | a & aux0 ^ (a | aux0)) ^
		(~a & aux0 & a & ~~a | ~~a)) & ~((~a | aux0 & (a ^ a) | aux0
		^ a ^ ~a) ^ a ^ a ^ (a ^ a) & (aux0 ^ aux0))) + 37642 *
		(aux0 ^ ~aux0 & ~~(aux0 & aux0 & (aux0 | aux0)) ^ ~aux0 ^
		aux0 & (a | aux0) | ~~(~~aux0 ^ ~a & aux0 | aux0)) + 37575 *
		((((aux0 | a) & aux0 & aux0 | a ^ (a | aux0) ^ a | aux0) ^
		(~~~a & a & (aux0 | a | a | aux0) | a)) & (a ^ aux0 & ~(aux0
		^ a) & ~(~aux0 & (a | a)) & (~a ^ ~a) & (a | aux0 & a) &
		~aux0) | ~aux0 | aux0) + 12161 * ~(~((~(a & a) | (aux0 |
		aux0) & (a ^ aux0) | (a | aux0) & ~aux0 ^ aux0 & aux0 & (a |
		a)) ^ ~(~a | a & aux0 | ~a | a & aux0)) ^ a & (~aux0 & (~(a
		& a) ^ a ^ a & aux0) | ~((~a | a ^ aux0) ^ aux0))) + 5475 *
		(~~(a | (aux0 | aux0 ^ a) & a | ~(~a ^ aux0 & aux0)) &
		(~~((aux0 | aux0 | aux0) & ~aux0 & (aux0 | aux0)) ^ aux0 ^
		~~~((aux0 | aux0 | ~a) ^ a & a & (a | a)))) + 25954 * (aux0
		| ~((~(a & aux0) ^ aux0) & aux0 & (~aux0 & (~aux0 ^ ~a) |
		aux0 & (aux0 | a) & (aux0 | aux0)))) + 5378 * (~~(a & aux0 ^
		aux0 & a ^ aux0 & ~aux0) | ~(a & (aux0 ^ a) | a) & (~(aux0 |
		aux0 | ~a) | aux0) | ~a & ((~aux0 & ~aux0 | aux0 & a ^
		~aux0) ^ (a | ~aux0 ^ a & a) ^ ~~aux0) | a ^ a) + 13025 *
		((aux0 | a) ^ ~aux0) + 28263 * (~(a | ~(~((a | aux0) ^ ~a) |
		~(a & aux0) & ~a)) | a) + 18759 * (a ^ (~((aux0 | a) ^ a ^
		(aux0 & a | aux0 | a | aux0)) | ~~aux0 & aux0 & aux0 & aux0
		& a & (~a & (a | a) | aux0 & aux0 ^ a ^ a) ^ (aux0 | aux0 |
		a | aux0) & a) | ~(aux0 ^ aux0) & (~((a ^ a) & a & a) & ((a
		| a) ^ a ^ aux0 ^ (a ^ a) & (a | aux0)) ^ (aux0 | aux0 ^ a)
		& (a & a ^ aux0) & (aux0 | ~(a ^ aux0))) & ~((~~a ^ aux0) &
		a)) + 57422 * ((~a & ~a ^ (~aux0 | a & a) ^ ~(a ^ a) ^ (aux0
		^ aux0 | aux0 ^ a) ^ aux0 | a) & ~((a | a | aux0 | (a ^ a) &
		aux0) & ~((a ^ aux0) & a)) & ~a & ~aux0 & (a | aux0) & (a |
		a | aux0) & (a | a) & ~a & (a & (aux0 ^ aux0) & ~a | aux0) |
		~(((a | a) & ~aux0 | a ^ a ^ a ^ aux0) ^ (~aux0 & a | aux0 ^
		aux0 ^ a) ^ (~a | ~aux0)) | aux0) + 31491 * ((~aux0 ^ (~aux0
		^ ~(a ^ aux0) ^ (aux0 | aux0)) & ~~aux0 ^ ~(~~~a | ~~(a |
		a))) & (aux0 ^ a ^ aux0 ^ (a | ~(aux0 | aux0)) ^ (aux0 |
		aux0 ^ aux0 | ~a) & (~a ^ ~aux0 ^ a) ^ a & a | ~(~(aux0 ^
		aux0) & a & a & (a | a) ^ ~a & ~aux0 & ~aux0 & (a | aux0) |
		~(a & a | aux0 | aux0)))) + 58384 * ((a ^ ~((aux0 | aux0 |
		a) & (~a ^ a & aux0)) ^ a) & (~(aux0 & ~(aux0 & a) ^ (~(aux0
		& aux0) | (aux0 | aux0) ^ a & aux0)) ^ aux0) ^ a) + 35797 *
		~(a ^ ~(~a | a ^ ~a) & (~(a | a) ^ ~~a ^ (~(aux0 | aux0) |
		(aux0 ^ aux0) & a & aux0)) ^ ~aux0) + 46981 * ((a ^ (~a ^ (a
		| a)) & (aux0 & a ^ aux0) ^ a ^ ~a ^ ~a) & aux0 & ~a & aux0
		& a & (~~(aux0 & aux0) | (~a | a) & ((a | aux0) ^ a ^ a) | a
		& aux0) & ~(aux0 & ((a | a | a) ^ a & (a | aux0)))) + 3807 *
		(a | ~(~(aux0 | ~(aux0 | aux0)) ^ (~(a | a) & (aux0 | a) &
		(a ^ aux0) | ~(a & aux0) | aux0 ^ a & a)) | aux0 | aux0 |
		aux0) + 64461 * ~(~a | ~~a) + 711 * (aux0 | ~((aux0 | a) &
		(~a & a & ~a ^ (~(a ^ aux0) | ~(a | aux0)) | a & aux0 & a &
		((a | a) ^ (a | aux0)) ^ ~a & a & (a ^ a ^ aux0)))) + 33849
		* ~(aux0 & ~((~(a | a) | (aux0 | a) & (aux0 | a) | aux0) ^
		~~(~aux0 & ~a))) + 39774 * (aux0 ^ (~aux0 | aux0)) + 55295 *
		~(~~(~~(a | a) ^ ~(aux0 & a & a)) | ~aux0 | ~~(aux0 & ~(aux0
		^ aux0))) + 21265 * (aux0 | ~~(~~aux0 & (a | a) & ~aux0 |
		((a | aux0) ^ ~a) & (a ^ a ^ a)) | ~((~aux0 & (a ^ a) ^
		aux0) & ~(a ^ aux0) & (a ^ a | a & aux0)) & aux0) + 28296 *
		(~a ^ (~(a ^ ((a | aux0) ^ (a | aux0)) & aux0) | ~(~a ^ a) |
		(~(aux0 | a) | aux0) & (~(aux0 | a) | (aux0 | a) ^ a & aux0)
		^ ~a | ~a ^ (aux0 | a))) + 46250 * (~~(aux0 ^ aux0 ^ ~aux0 &
		a ^ (a & aux0 | aux0 & a) & (~aux0 ^ aux0 & a)) & a) + 57179
		* ~(aux0 ^ aux0) + 64192 * (a & aux0 & (aux0 & ~(a | aux0 |
		aux0 | a) ^ ~~(a ^ (aux0 | aux0)) ^ a) ^ ~aux0) + 9101 *
		(((~(a & ~a) | ~(a | a | aux0)) ^ (~a | ~(aux0 ^ a | a ^
		aux0)) | ~(~aux0 | aux0 ^ ~aux0) & (a & ~a ^ aux0 ^ aux0) |
		aux0 & ((a ^ aux0 ^ (aux0 | a)) & (aux0 ^ aux0) & a | ~(aux0
		& aux0) & a & a & ~a) & ~((aux0 | aux0 | a) ^ aux0 & (aux0 ^
		a))) ^ (~~(a | aux0 | a & a) & ~~(a ^ a ^ ~a) | ~(((a | a) ^
		a) & ~(aux0 & aux0) | ~((a ^ aux0) & a)) | ~~(aux0 | a |
		aux0) | aux0 | aux0 & aux0 | aux0 & a | (a ^ a) & (a ^ a) |
		a)) + 29 * ~((~aux0 & aux0 ^ ~(aux0 ^ ~(a ^ a)) & a) & ((a ^
		aux0 ^ (aux0 | a) | a) ^ aux0 ^ a ^ aux0 ^ (aux0 | aux0) ^
		aux0 & a ^ a) & (aux0 ^ ((a | a) ^ (aux0 | a) | ~aux0 & a &
		aux0) ^ ~a)) + 60681 * (((~(~(~a & (a ^ a)) | ~(aux0 | a)) |
		~a | (a ^ aux0 ^ ~a) & a ^ ~(~a ^ ~aux0)) ^ a) & aux0 & ((~a
		^ a ^ aux0) & (aux0 | a) ^ (~aux0 ^ (a | a)) & (a ^ a) &
		aux0 ^ ~~(aux0 & a & (aux0 | a))) & ((~(aux0 ^ (aux0 | a)) |
		aux0) ^ aux0 & (aux0 ^ a) & ~a ^ ((a | a) ^ aux0) & (a &
		aux0 | a) | ~a)) + 35227 * ((~(((aux0 ^ a) & a ^ (aux0 &
		aux0 | aux0)) & (a & a & (aux0 ^ a) ^ a ^ ~aux0) & a) | a) &
		(((a ^ a | a & aux0) ^ aux0 & (a ^ a) ^ ~(a | aux0) |
		~(~aux0 & (a ^ ~aux0))) & a ^ ~(((a | a) ^ aux0) & ~(aux0 ^
		a) ^ ((aux0 ^ aux0) & aux0 | aux0 | a | aux0) ^ (a ^ aux0 ^
		(a | aux0)) & ~(aux0 | aux0) ^ ~~(aux0 & a)))) + 18476 *
		(~((a ^ (a | aux0)) & (a ^ a) & (aux0 ^ a) & (a ^ aux0 ^ (a
		| a) ^ aux0 ^ a ^ (a | a)) ^ (~(aux0 & aux0) | ~aux0 | aux0
		^ aux0) & ~(~aux0 ^ ~aux0)) & a & aux0 ^ a) + 47737 * (a ^
		(~~(aux0 ^ aux0) ^ (a ^ aux0 ^ a & aux0) & aux0 ^ (~~a | ~a
		| a | aux0) & ~(a ^ aux0 ^ aux0 ^ aux0) | (~(aux0 | aux0 |
		a) ^ ~aux0) & aux0) ^ (~((~a & (a ^ a) | aux0 ^ a ^ aux0) &
		a) | ((~a | a | aux0 | aux0 ^ a & a) ^ ~(aux0 & aux0) & (~a
		| ~aux0)) & (~a & (aux0 ^ a) ^ ~a & ~aux0 ^ a))) + 61457 *
		~(aux0 & ~aux0 | ~~(aux0 | a) ^ a ^ a ^ (a | a | aux0) &
		(aux0 | aux0 ^ a));
        vm->memory[vm->sp]=a;
    // std::cout << "[DEBUG] INC: stack[" << vm->sp << "] " << old_val << " -> " << vm->memory[vm->sp] << std::endl;
    
    WORD next = vm->fetch_opcode();
    vm->handler_table[next](vm);
}

void VirtualMachine::op_dec(VirtualMachine* vm){
    WORD old_val = vm->memory[vm->sp];
    WORD a = vm->memory[vm->sp];
    WORD aux0 = old_val*a;
    WORD aux1 = old_val*old_val - 4 * a * 41234;
    a = 24247 + 60101 * ((~(((aux0 | aux1) & ~aux0 | ~aux1 | aux0
		| a | aux0 | aux0) ^ aux0 & ~(a | aux0) & (aux1 & aux0 |
		aux0 & aux1) & ((aux0 | aux0) ^ aux0 & aux0)) ^ ~~(aux1 & a
		| aux1 & a) & ((aux0 | a) ^ (aux0 | a) | (aux1 ^ aux0) &
		(aux0 ^ a) | a) ^ ~~(~a & aux0)) & (a | ((aux0 ^ aux0 ^ aux0
		^ aux0) & (aux1 ^ aux1) & aux1 & aux1 & (a ^ a ^ aux1) &
		aux1 ^ ((aux1 | aux0 ^ a) ^ aux0 ^ aux1) & ~(~aux0 | aux1 |
		a)) & (~(aux0 | aux1) ^ a & ~aux1 | ~aux0 ^ ~a | a & a |
		aux1 & a) & a)) + 9124 * ((~~((~aux0 | ~aux0 | aux1 ^ ~aux1)
		^ a ^ aux0 ^ a ^ aux1) | a & (a ^ aux0 ^ aux1 ^ ~a |
		~~~aux1) | ~((~aux0 ^ aux1) & ~(aux0 | aux0)) ^ (~aux0 &
		(aux1 | aux0) & (a | a) | ~(a ^ aux0 ^ ~a))) ^ ~(aux1 ^ a &
		a & ((~a | aux0 & a) ^ aux1 ^ aux1) & a)) + 23183 * (~(~(a ^
		(aux1 | aux0) ^ aux0 | a) & (a & ~(aux0 ^ aux1 | aux0 &
		aux0) | aux1)) | aux1) + 43109 * (~~aux1 & aux1 & (aux0 |
		(~a | aux1 | a) & a) & (aux1 ^ (aux1 ^ ~(a ^ a) | ~aux1) ^
		aux0) | ~aux1 | aux0 | ~(aux1 & (aux0 ^ a)) ^ aux1 | ~(~(a &
		aux1) | aux0 ^ aux0 ^ aux1 ^ a)) + 41793 * (~((~aux0 | aux0
		& aux1 | ~aux1 | (a | a | a) ^ ~(aux1 | aux0) | aux1) ^
		~~(aux1 | aux1) & a & ~(aux0 & aux0) & (a ^ a)) ^ (aux1 |
		~((aux1 & aux1 & aux0 | (aux1 ^ aux0) & aux0) & (aux0 & a &
		aux1 & a ^ aux0) ^ aux0 ^ (~(aux1 | a) | ~~aux0)))) + 62065
		* (~((((aux1 | a) & aux0 & a | aux0) ^ (aux1 | aux1 | aux1 ^
		a) ^ ~(aux0 ^ aux0) ^ ~((aux0 | a) & ~a) & a) & (a | ~a)) |
		~(aux1 & (aux1 | aux0 | aux0) ^ ~aux0 & aux1 & (a & a ^ (a |
		a)) & ((aux0 | aux1 & aux0) ^ aux1 ^ a ^ a)) & ~~(~~(aux1 |
		aux1) ^ (~aux0 | aux1 & aux0) ^ ~(aux1 & aux1))) + 27802 *
		((((aux1 ^ (a ^ aux0 | a ^ aux1)) & (aux1 & aux1 & (aux0 ^
		aux1) ^ aux0 ^ aux0 ^ aux1) | aux1) ^ aux1) & ((a & ((aux1 |
		a | aux0 | aux1) ^ aux1 ^ aux0 ^ aux1 & aux0) | aux0 &
		~~(aux1 & a)) ^ ~~~a & ~(~aux1 & ~~aux0)) ^ (aux1 | aux1) &
		~~(aux1 ^ ~((a ^ aux1) & (aux0 | aux0)))) + 49525 * (~~~(~a
		^ a & aux1 | (a | a) ^ aux0 ^ aux1 | ~(a | aux1) & ~(a |
		aux1)) ^ a) + 29811 * a + 10883 * (~((a | a ^ aux1 ^ (aux0 ^
		a) & aux0 | ~aux0) ^ (a ^ ~aux1) & a & (a ^ aux0 ^ aux0 ^
		aux1) & aux0) & (((aux0 & aux0 ^ a & aux0) & (a | aux1) &
		~aux0 & ~(aux0 ^ aux1) & (a ^ aux0) & a & aux1 | aux0) ^ a)
		& ~((~(aux1 & aux0) ^ (a | a) ^ aux1 & aux1 | aux0 ^ aux0 ^
		~a ^ (a | ~aux0)) ^ ~(a & aux1) & aux0 & ~~(aux1 & aux1))) +
		11923 * ((~(a & aux0 & aux1 & a | a) | aux1 | ~a | aux1 ^
		aux1 ^ aux0 | (aux1 ^ a | ~aux1) ^ ~a | aux1 & (~~a ^ (a | a
		| a)) & (aux1 ^ aux1 ^ (aux1 | aux1) ^ (aux0 | aux1 ^
		aux1))) & a | ((aux1 ^ aux0 ^ aux1) & aux1 ^ ~(aux0 ^ ~a) ^
		~aux0) & aux0 | ~(((aux1 | aux0 & aux0) ^ a) & (~(aux1 | a)
		^ ~(aux1 | aux0))) ^ ~(~(~aux0 | ~aux1) ^ ~(aux1 | aux1 | a
		& aux0))) + 60025 * (~(~~(a ^ a) | (aux1 | a) & aux1 & aux1
		^ (a ^ a | aux0 ^ aux1) | aux0 ^ ~~a) & aux1 ^ aux0) + 3306
		* aux1 + 10972 * ((aux1 ^ aux1 | a) ^ ~(~aux1 & (aux0 ^
		aux1) & ((a | aux1) ^ aux0 ^ aux0) & (~(a | aux1) | (aux1 |
		aux1) ^ (aux1 | a))) ^ aux0 & (aux0 ^ ~(aux0 ^ aux1)) &
		~((aux0 ^ aux1 | a ^ aux1) ^ ~a ^ aux0 & aux0) ^ (~(aux1 ^
		aux1 | aux1 & aux1) | ~~a & ((a | aux0) ^ (a | a))) ^ (aux0
		| aux0 | ~aux0 & aux1) | ~a) + 31433 * (~aux1 | a) + 29843 *
		((a ^ (aux0 ^ a | aux1 & aux0) ^ (aux1 | a) & (aux0 | a)) &
		((aux1 | aux0) ^ ~a ^ (aux1 | aux0) & (a | aux1) ^ aux1) &
		~(aux0 ^ ~(aux0 & aux1) ^ aux0) & (~(~~aux1 ^ aux1) ^ aux1 ^
		a ^ aux1 ^ ~aux0 ^ ~~a | aux1 ^ ~(~aux0 & (aux1 | aux0)) ^
		~(aux1 | ~aux1) ^ aux0) ^ ~aux0) + 60303 * (aux1 & ((~(aux1
		& aux1) | ~a | a ^ aux0 | aux0 | a & aux1 | aux1 | aux1) ^
		(a & aux1 & a | (a | a) ^ ~aux1 | ((aux0 | aux0) ^ (aux0 |
		a)) & ~(aux1 & a)) ^ (~((a | aux0) & (a ^ a) | aux0) | (~a |
		aux0 ^ aux0) ^ aux1 & ~aux0 ^ (aux1 ^ aux0 | ~a) & (~a ^
		aux1))) | aux0 | a) + 44033 * (aux0 ^ ~(aux0 & (a ^ ~aux1 |
		aux0 & (aux1 ^ a) | aux0) & (~((aux0 ^ aux1) & ~a & ~a &
		(aux0 ^ aux1)) | ~aux0 & aux1 & aux1 & ~a & (a ^ a) ^
		~~(aux1 & aux1)))) + 52684 * ~(aux1 & ((aux1 | aux0 | aux0 ^
		aux0) & (aux1 ^ aux1 | ~a) | a & a | ~aux0 | aux0 | aux0 &
		a) & a & ((aux1 ^ aux0 | aux0 & a) ^ (~aux0 | ~aux1)) ^
		(aux1 & aux1 | ~~(a & a ^ a) | aux0 | aux1 & a & ~a |
		~((aux0 | a) ^ (aux1 | aux1)))) + 51426 * (((a | ~(aux0 ^ a)
		| ~(aux0 & aux0)) ^ aux0 & ~aux1 & a & aux0 & (aux1 | a |
		aux0)) & ~~aux1 & aux1 ^ (aux0 ^ ~~(~aux0 ^ aux0 & aux0)) &
		(~(aux0 | (aux0 | a) & aux1 & aux0) ^ ~(aux0 & (aux1 ^ a) &
		~a)) ^ ~~(a & a & a & aux0 | ~a & (aux0 | aux0) | aux0) ^
		aux1) + 61080 * (~(~(a ^ (aux0 | ~(aux0 & a))) | ~(~(aux1 &
		a) & ~aux1 ^ ((a | a) ^ ~aux0 | ~(aux0 ^ aux1)))) ^
		~~~~aux1) + 58558 * (a & ((aux1 | ~a | a | aux1) & ~(a ^ a)
		& (aux1 ^ aux0) & (aux1 ^ a) & aux0 | ~(aux0 & (aux1 & aux1
		^ (aux1 | aux1) ^ ~aux0 & (aux0 | a)))) ^ aux0) + 47383 *
		~(~aux0 | aux0) + 36190 * ~((aux0 | a | aux0) ^ ~a & (~(aux1
		& a | a ^ aux0) ^ ~~(aux0 & a)) | ~(aux1 & (aux0 | a ^ aux1)
		| (aux1 ^ aux1) & (aux1 | a) ^ aux1) ^ (~aux0 | a ^ aux1 | a
		^ a ^ aux1) & aux1 ^ (aux0 | ~(a ^ aux1)) ^ (aux1 | aux1) &
		(a ^ aux1) & ~aux1) + 52911 * (a | aux0) + 40833 * (aux0 ^
		aux0 ^ (~aux1 & (a ^ a | aux0 | a | ~aux1 | aux1) | a &
		~(aux1 | aux0) & aux1 & aux0 & aux0) & (aux0 & ~a ^ aux0 &
		(a & aux0 & aux1 & aux0 ^ ~a & (aux0 ^ aux1))) | ~~((~((aux0
		| aux1) ^ ~aux0) | aux0 & aux0 | ~(aux0 & a)) ^ (~(~aux0 | a
		& aux0) | ~a))) + 3346 * (((a ^ a) & ~(a | aux1) ^ (~(aux0 ^
		~aux0) | (~(aux0 ^ a | a | a) | a & a ^ (aux0 | ~a)) & a)) &
		(~(~aux0 & (aux1 ^ a) & aux0 & a & aux0) | ~(~a ^ a & aux0)
		& a & ~(aux1 ^ a)) & ((aux0 | a | (a | a) & ~aux1 ^ aux0) ^
		~~(aux0 & (a ^ a))) & ~(~~(aux0 & a | aux1 | aux0) | aux1))
		+ 45250 * (aux0 & (~a ^ a | ~((aux1 | ~(aux0 ^ aux1)) ^
		(~(aux1 | aux0) | ~(aux1 ^ a))) & a)) + 268 * ((~((aux0 |
		~a) & (a ^ (a ^ a | aux1 ^ aux0 | aux0 & a | aux1))) ^ aux0)
		& (~~(aux0 & ~((a | a) ^ aux0 & aux1)) | ((a ^ aux1 | aux0)
		^ aux0 | a & a ^ (a | a) | a & aux0 & aux1 & aux1) & aux1 |
		a & ~aux1 & aux0 & a & aux1)) + 40255 * ((aux0 ^ ((aux0 |
		aux0) & aux0 & a | a) ^ ~(a | a | aux0 | a)) & ((a & (aux0 ^
		aux0) | a & aux0 | aux1 & a | ~(aux1 ^ aux0 | aux0)) ^ (aux0
		^ ~aux0 & (aux1 | aux1) | ~(~a & (aux1 | aux1)))) | ~(aux0 &
		aux0) | ~~((~(a | aux1) ^ ~aux0) & ~(aux1 & aux0 & a &
		aux1)) | aux0 ^ ~(aux1 ^ a | a ^ a | (aux1 | aux1) & (a |
		aux1)) & (~(aux0 ^ a | ~aux0) ^ aux1)) + 11546 * (~(aux1 ^ a
		^ aux1 | aux0) & ~((a | aux0 | aux0 & aux0) & (a | aux0) &
		(a ^ a)) & ~(aux0 | aux0) | aux1 | a) + 11019 * (~(((aux1 |
		a) & ~a & ~~aux0 | a | aux0 & aux1 | aux0 & aux0 & aux1) ^
		(aux0 & aux1 ^ (a | a) ^ aux1) & ~(~a ^ aux1)) ^ (~(a ^ aux0
		^ aux0 & aux1) | aux0) & ~(~~(aux1 ^ a) & a) | (~aux1 ^ aux1
		| aux1 & ~(aux1 | (aux0 | aux0) & aux0)) ^ (aux0 & (~aux0 ^
		a) ^ (a | (a ^ aux1) & (aux1 | aux1)) ^ ~aux0 | ~(aux0 | a &
		a & (a ^ aux1) & ~(aux0 | aux1)))) + 55245 * ~aux1 + 5240 *
		(aux0 & (~((a | ~~(aux0 & a)) & a & aux1 & aux0 & (aux0 ^
		aux0) & ~aux0 & (~(aux0 ^ aux1) ^ aux0 ^ aux1 ^ a & aux0)) ^
		a ^ ~(a ^ aux1 ^ aux0 & aux1 ^ (aux1 ^ aux1) & ~a))) + 28341
		* ((~((~aux1 & aux1 & aux1 ^ (aux0 | a | aux0 ^ a)) & (~aux1
		| aux0 ^ aux1) & aux1 & aux1 & a ^ ~~aux1 ^ a ^ aux0 ^ ~aux1
		^ ~~aux1 ^ ~~aux1) ^ ~(((aux1 | a) ^ (a | a) ^ aux0 & aux1 &
		aux1 & a) & ~~a & a) & ~(~(aux1 & a ^ a) | (aux1 | aux1 &
		aux1) & (aux0 | aux1 | aux1))) & ~((~(aux1 & aux1 ^ aux0) |
		aux1 ^ aux0 ^ aux0 & aux0 | ~(aux0 ^ (a | a | aux0 ^ aux1)))
		^ (aux0 ^ a ^ ~a | a | a | aux0 ^ aux1 | ~aux0 ^ ~(aux0 ^
		a)) ^ aux0)) + 48132 * aux0 + 20923 * (~(~aux1 ^ (aux1 &
		(aux1 | aux0) | ~aux1) ^ ~((aux0 | a) & (a | a) ^ (aux1 ^
		aux1) & aux1 & a)) & aux0 | ~((~~(aux0 ^ aux1) | (~aux1 |
		aux0) ^ aux1 & aux1) ^ ~(aux0 & a | ~aux0 | a) | aux0 ^
		~((aux1 | aux0) & (a ^ aux1) & aux1))) + 8405 * (a & a &
		~aux0) + 23867 * (((~(aux1 & aux0 & aux1 & (aux0 | aux1)) ^
		aux0) & ((aux0 ^ aux1 ^ a ^ aux1) & a ^ (~(aux0 & aux0) |
		~a) | ~~~aux1 ^ (aux0 & a ^ aux1 ^ aux1) & ~~a) ^ (a ^ ~aux0
		| aux1 | ~aux0 & aux1)) & aux1 & ~a & (aux0 ^ aux1 | ~aux1)
		& (aux1 | ~a) & ~~aux1 & aux1 & aux0 & a) + 6729 * ~a +
		53643 * ~aux0 + 64363 * (a | ((~(aux0 & a) | ~(a | aux1)) ^
		aux0 ^ (~aux1 ^ ~a) & ~aux0 & aux0 & (aux1 ^ (aux0 ^ aux1) &
		(a | aux0))) & a & a | ~((~(aux0 | aux0) | (a | aux1) ^ (a |
		a)) & (aux1 | aux1 ^ aux1 & a) ^ a)) + 29128 * ~((~~(a & a &
		(aux1 ^ aux1)) | a) & (~(aux1 & aux0 & (a ^ aux1) ^ a) |
		~(aux1 | ~aux0) | a) & aux1) + 15227 * (aux1 & aux1 ^ ~(aux0
		^ (aux1 ^ aux1 ^ aux0 ^ ~aux0) & ~aux0 ^ (aux0 | aux0 ^ (a ^
		aux1) & ~aux1 ^ (~aux1 | aux0 ^ a)))) + 19554 * ~~(a | aux1
		& (aux0 ^ aux1) ^ ~a & (aux0 ^ aux0) | aux1 | ~((~aux1 | a |
		aux1) ^ (a | aux1 | aux1) | (a & a | a) ^ aux1)) + 39158 *
		~~(((~(aux1 & a) | aux1) ^ a) & (aux1 | ~(aux0 | a)) & ~aux0
		& (~(a | aux0) & (aux0 & aux1 | a | aux0) | ~a) & (~aux0 |
		aux0)) + 35882 * (aux0 & (~(aux0 & aux1 ^ aux0 ^ aux0 | aux0
		& aux1 & a & aux0 | ~(aux1 ^ aux1 | ~aux0)) & aux1 & ((a &
		aux0 | a & aux1) & a ^ (aux1 | aux1 | aux1 ^ aux0) ^ ~(a |
		aux0)) ^ aux0)) + 25208 * (~(~(aux0 & aux1 & a & a & ~(aux0
		& a)) ^ aux0 | a | ~(~(aux1 | aux1) & (a ^ aux1) & (a |
		aux1))) | aux1 ^ aux1) + 40487 * ((aux0 ^ ~(((aux1 | aux1) ^
		(aux1 | aux1) | aux0 ^ a ^ aux1 ^ aux1) & ((a | aux0) ^ aux0
		^ aux0) & (aux1 & a ^ ~aux1))) & a & a & ~(aux1 | ~aux1 | (a
		| a) ^ aux0 & aux1 | ~(~aux1 | a | aux0 | aux0))) + 40948 *
		(~((a ^ aux1) & ~aux1 & (a ^ aux0 ^ aux0) & (aux0 ^ ~aux0 &
		~(aux1 ^ aux1)) & (aux1 ^ aux0 & aux0 | aux1 | aux0 | (a |
		aux1 & aux0 | aux0 | aux1) ^ ~aux0)) & a) + 43023 * ~(~~aux1
		| ~a) + 47844 * (~aux1 & (aux1 ^ (aux0 | aux0 & (aux1 | a &
		aux1 & ~aux0))) ^ (aux1 ^ ~((a ^ a) & a) & (~(a | a) | a ^
		aux0 | ~aux1) ^ ~((~aux1 ^ aux1) & (~a | aux0 & a | ~aux1 &
		aux0))) & ((~a ^ ~aux1 ^ a | ~((a & aux0 ^ a ^ a) & aux0)) ^
		((aux0 ^ aux1 | a | a) & ~aux1 & aux1 & aux0 | ~(a | aux1) ^
		~aux0 & a & aux0 | ~~aux1 | aux0 | (~a | ~aux0) ^ (aux1 |
		~a)))) + 54599 * ((a | (~aux1 & aux0 | (aux1 ^ a) & aux1 &
		a) ^ (~(a ^ a) | ~~aux0) | ~(aux1 ^ ~(aux0 & a ^ ~aux0))) &
		(~aux1 | a) ^ ~~a & ((aux0 ^ a ^ aux0) & ~aux0 | a | aux0 |
		a | aux1) ^ ~(aux0 ^ a ^ a & a) ^ (~aux1 | aux0 | aux1) &
		((aux1 | a) ^ (aux0 | aux0)) ^ aux1 ^ a) + 46557 * (aux1 &
		((~(aux0 | aux1) ^ ~(a & aux1) | ~~aux1) ^ ~((a & a | aux0 |
		aux0) ^ (~aux0 | aux1))) & aux1 | aux0) + 35952 * (~(~aux1 &
		~((a ^ aux1) & (aux1 ^ a)) & ~((~a ^ aux1) & aux1) & (aux1 |
		~aux1 & (aux1 | aux1) & a & (aux1 ^ a ^ (a | aux1) ^ ~(a |
		aux1)))) ^ aux1) + 10317 * ((aux1 | a) & (~~((a ^ aux0 | a |
		aux1 | aux1 | ~aux0) & a) | ~~((a | aux0) & ((a ^ aux1 | ~a)
		^ ~(a | a))))) + 9127 * ~(~(a & aux1 & (a ^ aux1)) & aux0 &
		(aux1 | (aux1 | a) & a | (~aux0 | aux0) ^ a ^ a ^ a) &
		~~(aux1 ^ aux0 ^ a & aux1 | ~~aux0) | ~~((~a | a) & aux1) &
		((a ^ a ^ aux1 ^ aux0) & (aux0 ^ aux1) & (aux1 | a) ^ (~aux0
		| ~~a) | a)) + 31557 * (((a | a) ^ ~(aux1 & (aux0 ^ aux1 |
		aux1) | aux0) | a) & ~~(~a & (aux0 ^ aux0) ^ ~(aux0 & a) | a
		| aux0 | aux1 | aux1 & a | a | aux0 | a | (aux1 ^ a) & ~a |
		aux1)) + 13653 * ~(aux1 ^ aux1) + 28500 * (aux0 & (a ^ ~a ^
		~~aux1 | a) & a & ~(aux1 ^ aux1) & aux0 & aux0 & (a ^ a) &
		aux0 & ~~aux1 | a ^ ~(~(aux0 | aux1 | ~(aux1 | a)) ^ aux1 ^
		~aux1 ^ ((a | aux1) & aux1 | a))) + 1880 * (aux1 ^ ~~~(a ^
		(a ^ aux1) & ((a | aux0) ^ aux0))) + 22427 * ((a | ~(aux0 &
		((a & a | ~aux0) ^ ~a & aux0 & aux1)) ^ (~aux0 | ~(~(aux0 ^
		aux0) & (aux0 & aux0 | aux0 ^ aux1)))) & aux0) + 35751 *
		~~aux0 + 37078 * ~~(aux1 | ~(~(aux0 | a) ^ aux0 & a ^ ~aux0)
		^ aux0) + 16727 * ((~aux1 & ((a | a) & aux0 ^ aux1 ^ (aux0 ^
		a ^ a ^ aux0) & a & ~a) & (~(~a | ~a) ^ ~~(aux0 ^ a)) | ~(a
		& a & aux0 & aux1) & ((a | aux1) ^ (aux0 | aux0)) & ~~aux0 ^
		(a ^ aux0 | a) ^ (~(a & ~aux1) ^ ~a | (aux0 | aux1 | ~aux0)
		& ~(a | a) ^ (a | a ^ a ^ aux1))) ^ ~((~a & ~a ^ aux0) & a |
		aux1 | ~(aux0 & a | ~aux0) | a)) + 4396 * ~(aux0 & ((aux1 |
		aux1 & (a ^ aux1) ^ aux1 & aux0 ^ ~aux1 | aux0) ^ ((aux0 &
		aux1 ^ ~a) & (a ^ aux0) & a & aux0 & ((aux1 ^ aux0 | aux1 |
		aux1) ^ a) | aux1))) + 65068 * (~(~(a | aux1 & aux1) ^ (aux1
		| (aux1 | aux0) ^ ~a)) & a ^ ~(~~((aux0 | a) & aux0) &
		((~aux1 & aux1 & a | aux1 ^ a ^ (aux1 | aux0)) ^ ~aux0)) ^
		aux0) + 59596 * ~(~~~(aux0 & ~aux0) ^ a | ~((aux0 ^ ~aux1) &
		aux1 ^ (aux0 & aux1 | a | aux1) ^ (~a | aux1)) & ~(aux0 & a
		& (a ^ aux0)) & ~(~a | aux1 & a) & ((a & a ^ a ^ aux1) &
		(aux1 ^ aux1 & a) ^ ~(aux1 ^ a ^ aux0 ^ a))) + 60742 *
		~~~(~~(a | a) | a & aux0 ^ ~a | a ^ aux0 ^ a & aux1 | a) +
		46330 * (~((a | aux1 | aux0 | aux1) & (~aux0 ^ (aux1 | a)) ^
		~~aux1 ^ ~aux0 & ~aux1) & (~(~(a & aux1) & (aux0 & aux0 ^
		aux0 ^ aux0)) ^ ~~(a ^ aux1 ^ a & aux1)) ^ (~aux0 | aux1 |
		aux1 ^ aux0) ^ ((aux0 | ~a & ~a | ~((aux1 | aux1) ^ ~aux1))
		^ aux1 | ((aux1 | a) & ~aux0 | ~(aux1 ^ aux0)) ^ ~~(aux0 &
		aux0) ^ aux0 | aux1 | a)) + 30334 * (~(~a | ~aux1 & (~a | a)
		| ~a ^ aux1 | (~a ^ ~aux0) & ((aux0 | aux0) ^ aux0)) &
		(~(aux0 ^ a ^ a & a | (aux0 | aux1) & (a | aux1) | a & (aux0
		| aux1)) | (~~aux1 ^ ~(aux1 ^ aux0)) & (aux1 ^ aux1 | aux0 ^
		a) & aux0 | (aux0 ^ a & ~a) & a) & ((aux1 & (aux1 ^ aux0) &
		aux1 & (a ^ a) & a & a & aux1 & ~aux1 | (~aux1 & (aux0 ^ a)
		| a ^ aux1 | a | aux1) & ~(aux1 ^ aux1) & (aux0 ^ aux1 |
		aux1)) ^ a)) + 27808 * (~~(~(((aux0 | a) ^ aux1) & (a ^ aux0
		| aux0 ^ aux0)) & ~~(a & aux0 | aux0 ^ a)) ^ ~a) + 52999 *
		(aux1 & ((aux1 | (aux0 ^ aux0) & aux0 | aux1) ^ ~(aux1 &
		aux0 ^ aux0 | (aux0 ^ aux0) & (aux1 ^ a))) | a | aux0) +
		63745 * (a ^ (((a | aux1) & a | ~aux0 & aux0) & (aux0 | aux0
		| aux1 | ~~a) | ~(a | aux0 | a ^ a | ~(aux0 ^ a))) | ~(aux0
		^ aux1) ^ (aux1 | a | aux0 | aux1) ^ aux1 ^ aux1 | ~aux1 &
		aux1 | ~((aux0 | aux0) & aux0) ^ ((a | aux0) ^ aux0 ^ a) &
		(~a | aux1 & a) | ((a | (aux1 ^ aux0) & (aux0 | aux1)) & ((a
		| aux0) & (aux0 ^ aux1) ^ ~(aux0 & a)) ^ a) & (a | a & aux1)
		| aux1 | ((~aux1 | aux0) ^ a) & ~~aux0 & (aux0 | aux0) & ~a
		& a) + 13552 * (~(((a ^ aux1 ^ (aux0 | a) | ~a | a & aux1 &
		aux0 & aux1 & a & (aux1 | aux1)) ^ (((a | a) ^ a & aux1) &
		(~a ^ a ^ a) | ~~(aux0 & a))) & ~(aux0 & (aux0 | aux1))) &
		(a ^ ~(a & (~aux1 | ~aux1)) & ~(aux1 ^ aux0 ^ aux1 & a ^
		aux0) ^ aux0)) + 38297 * ((a ^ aux1 ^ (~(aux1 ^ a) | aux0 &
		(a | aux1)) ^ (aux0 & a ^ ~a) & ((aux0 | aux0) ^ aux1) & a |
		(a | ~aux1 | a & aux1) & aux0 & (a | aux1) & aux0 ^ (a & (a
		| a) | ~~a) ^ (a & aux0 | aux0 | aux1 & aux0 ^ ~aux0)) ^ a |
		a & aux0) + 43826 * ((~(~(~aux1 & aux0 & aux0) & aux1) ^ a)
		& aux1 ^ aux0 ^ (aux0 ^ ~~(aux0 & aux0) | (aux0 ^ aux0 |
		aux0) & aux0 & aux1 | aux1 & ~aux1 & ~(a & aux0) | a)) +
		1337 * ((((aux1 ^ aux0 | a ^ aux1 | ~aux0 | aux0 ^ aux1) ^
		aux1) & ~~(aux0 & aux0) | ~~~aux1 | a | ~~aux1 ^ aux1 ^ aux1
		^ aux1 & a ^ (a | aux0 ^ aux0)) & (~(aux1 & aux1 ^ (aux0 |
		aux1)) | (aux1 | aux1) ^ (aux0 | a) | (a | aux0) ^ (a |
		aux0) | ~(aux0 ^ a & aux1) | aux0) & aux1) + 3520 * (aux0 &
		aux1) + 34408 * ((~~(aux1 | aux1) | ~(((a | aux0) ^ (aux1 |
		aux1)) & aux0 & ~a & ((aux0 ^ aux0) & a & a ^ (aux0 | aux0)
		^ a ^ aux1) & aux1 & aux1 & aux0 & a & aux0 & aux1)) & a) +
		38100 * ~(~((aux1 | aux0) & (aux0 | aux1) & ~a ^ ~(aux1 & a
		| ~aux0)) | aux1 & aux1 & ~~(aux1 ^ aux0) | ~a) + 54163 *
		(~(aux0 & aux1 & aux1 & aux1 | aux0 | aux0 | aux0 & a ^ a ^
		a | ~aux0 ^ a ^ aux0 | ~((~a | aux0) & ~~aux0)) & ~(aux0 &
		~(aux1 & aux1)) ^ aux0) + 45963 * (((~aux0 & ~(aux0 & aux1)
		& ~aux1 ^ ~(~aux0 & (aux0 ^ aux0)) ^ ~aux1) & ~aux1 ^ a &
		~(a & aux1) & (~aux1 ^ aux1) & ((aux0 ^ a) & ~aux1 & ((a |
		a) ^ ~aux1) | ~(~a ^ aux1 ^ aux1)) & aux1) & ((~((aux1 ^
		aux1) & (aux1 ^ a) & aux0) | (aux1 | aux1 | aux1 ^ a) &
		(aux1 | a | a | aux1) | aux1 & (aux0 | a & aux0)) ^ (a & (a
		| aux1) ^ aux0 & aux1 & aux0 & (aux0 | a)) & (aux0 ^ ~~(a &
		a))) & (a & (aux1 | aux0 | aux1 ^ aux0) & (aux1 | a & a) |
		~aux0 | ~(~a & (aux0 | aux1) & (aux0 ^ aux0 | a)) & (aux0 &
		a | aux0 ^ aux1 | ~(aux0 ^ aux0)) & (a ^ aux0 ^ aux1 & aux0
		| aux1))) + 51046 * (a | a) + 6467 * ((~((aux0 ^ a & aux0 |
		~~aux1) & (a ^ ~aux0 & a & aux1)) | ~aux1 | aux0) ^ ~(~aux0
		& ((~aux1 | a) ^ ~a & (a ^ aux0) ^ ~a) | (aux0 & a ^ (aux0 |
		aux0) | aux0 | aux1 & a) & (a | aux1 & aux1 & a & a) | aux0
		^ (aux0 ^ a) & aux1 | a & aux1 ^ aux1 | aux0 ^ aux1 ^ (a |
		aux0))) + 27995 * (aux1 & ~((~((a | aux1) ^ ~a | ~(aux0 |
		a)) ^ ~a) & (a & aux0 | a & aux1 ^ aux0 ^ aux1 ^ a ^ a & a)
		& ~(a | aux0 | aux0 | aux1) & ~aux0)) + 64428 * (~(((~(a |
		a) | aux0 ^ aux0 | aux0 ^ a) & a & ~aux1 & (aux1 ^ aux0 | a
		^ a) | aux0 & (~(aux1 | a) | a ^ aux0 | aux1 | a)) & ((aux0
		| a) & (aux1 ^ aux0) | ~~aux1) & (~(aux0 ^ aux1) ^ aux1) &
		aux0 & (aux1 ^ aux0 ^ ~aux1 ^ aux0)) | aux0 & a & ~(aux1 ^
		aux0 ^ ~(a & aux0) ^ aux1 & (a | aux0))) + 10155 * (((aux1 |
		aux1 | (aux0 | aux0) ^ (aux0 | aux0)) & (aux0 ^ aux0 & aux0)
		^ aux0 | a) & (aux1 ^ ~a) | ~((a ^ (a ^ aux1 | aux1 & aux1 |
		~aux0) & (a ^ (~aux1 | ~aux0))) & (a | (a | a | ~aux1) ^ ~(a
		& aux0) ^ aux0))) + 56296 * ~(aux0 | ~(~((aux0 | aux1 | aux0
		^ a) ^ aux0 & (a | aux0)) | aux0 & aux0 ^ aux0 | ~(aux1 |
		aux1) | ~(~aux1 ^ aux0 & a))) + 6746 * (a | ((~~((a ^ aux1)
		& (aux1 ^ a)) | ~~(aux1 & (a | a))) ^ a & ~(~aux0 & (aux1 |
		aux1) ^ aux0)) & (~aux1 | aux1)) + 40462 * (aux1 | aux1) +
		20206 * ~(aux1 ^ ~~((aux0 ^ (aux0 | aux0) ^ aux0 ^ aux0) &
		aux0)) + 25381 * (aux1 ^ aux0) + 10722 * (aux1 | (~~(aux0 &
		a ^ a & a) | ~(~aux0 & ~aux0 | a & a | aux1 | aux1)) ^ aux1
		| ~(a ^ aux0)) + 48487 * ~((~(~aux1 ^ (aux1 | aux0) & (aux0
		^ aux0)) ^ a ^ ~a & aux0 & a ^ aux1 ^ aux1 ^ aux0 ^ aux0 ^
		aux1) & ~((a & aux1 ^ a & aux0 | ~~aux0) & ~(aux1 | aux1 |
		~aux1)) & ~((a ^ (aux0 | aux0 | aux1 | a)) & ~aux1) & a) +
		19540 * (a & a) + 9984 * (((aux1 | a) ^ a ^ aux0 ^ aux0) &
		~(a & a & (aux1 | aux0)) & (~aux1 & (aux1 | aux1) & ~~aux1 ^
		aux1 & (~aux0 ^ aux0 ^ a)) & (~a | ~((aux1 ^ a | aux0) &
		aux0)) & (a | aux0 ^ (aux1 ^ a | a) ^ a ^ ((aux0 ^ a |
		~aux1) & aux1 | a)) & a & (~~a & aux1 & (a & a ^ aux0) ^ a)
		& (aux0 & (~aux0 ^ ~~a) | (aux1 | aux0 | aux0 & a) & a ^
		~aux0)) + 58364 * ~(~a & aux0 ^ ~((aux0 | a | a ^ aux1) & (a
		| a) & ~a ^ ~~~aux1) | aux0) + 33489 * ((a ^ ((aux0 ^ a |
		~aux0) ^ aux0 ^ aux0 ^ aux0 ^ a) & ~(aux1 | a | ~aux0) &
		aux0 | (aux1 & ~aux1 | ~aux0 ^ aux1) & (~(~aux0 ^ (a | a) |
		a) | (a ^ aux1) & (a ^ aux1) | aux1 ^ a | aux0 | aux0 | a |
		a | a & aux1 | (a | aux1) & (aux1 ^ aux0))) & (aux0 ^
		(~(aux0 | a | a) & ~~~aux1 | a ^ aux0 | (~aux0 | aux1 | a) ^
		~(aux1 | aux0))) & (((a & aux1 | ~aux0) ^ ~(aux1 | a) |
		(aux1 ^ aux1) & aux0 & a | aux1 ^ a ^ (aux0 | a) | (aux1 | a
		^ aux1) ^ (aux1 ^ a | a) ^ aux1) ^ ~(aux0 | ~((aux1 | a) &
		(aux0 ^ a))))) + 27995 * ~~a + 34449 * ((~((~(aux1 ^ aux0) |
		a) ^ ~~~aux1) | ((aux0 ^ a | aux0) ^ ~~a) & aux0 & ~~(aux0 &
		a | a & aux0)) ^ ((aux0 & aux0 & ~aux1 | ~~aux0 | ~aux1 &
		aux0 ^ ~(aux1 & a)) & ~a | aux1 & aux1 & (aux0 ^ a & a & a &
		aux1)) ^ aux0) + 4847 * ((~aux1 | ~(a ^ aux1 | a) | ~(~aux1
		| aux0 & a) | ~(aux1 & ~(a ^ aux1))) & aux1 & a ^ ~((a |
		aux0) & aux1 & aux0 & (~a | aux1 & a) & ~(aux0 ^ aux0) & ((a
		| aux1) ^ a & aux1) & ~(aux1 & ~(a ^ aux0)) | ~aux1 ^
		(~~(aux1 ^ aux0) | ~aux1 ^ ~a ^ aux0 ^ aux1))) + 11919 *
		(aux1 ^ (aux1 | a & a) ^ (a | (a & (a ^ a) ^ a & aux0 ^ (a |
		aux0) ^ (aux1 & a | aux0)) & (aux0 ^ (aux0 | aux0 | ~aux0) ^
		a) | ~(~~(a ^ aux0) | a & aux1))) + 60974 * (aux1 ^ ~~(~aux1
		& aux0) ^ aux0 ^ a) + 47408 * ((aux1 | (aux0 | ~aux1 | ~a) &
		~(~aux1 & ~aux0)) & ~(a & aux1 & (aux0 | aux0) & (a ^ a) &
		aux1 & aux1 & a) & ~(aux1 & (aux1 | aux0) & aux1 & aux0 &
		aux0 & (~aux0 ^ aux1 & aux0)) & (~~(a & a | a & aux1) ^
		aux0) & ~(~aux1 & (~(a | a) & ((aux0 | aux0) ^ (aux1 |
		aux1)) ^ aux1 & ~a & (aux0 | aux1 | aux1)) & ~~~~a)) + 24733
		* ~~~aux0 + 15131 * ~(((aux1 ^ (aux0 ^ a) & ~aux1) & aux0 &
		(~aux0 ^ aux1 ^ aux0) | aux1 | aux1 | a) ^ ~((aux1 | a | a)
		& aux0 & aux0 & a & aux1 & (~aux1 ^ ~aux0)) ^ ~((~~aux1 |
		~aux1 | aux1 & aux1) & (~aux0 & (aux1 | aux0) | ~(a & aux1))
		^ (~aux0 & (a ^ aux0) ^ ~~a) & ~(aux1 & a & aux1))) + 15276
		* ((~(~(aux0 ^ aux0) | a & aux1 ^ aux1 ^ aux1 | ~~(aux0 ^
		aux1)) | aux0) & aux0 & aux0 & ((aux1 & a | aux0 | aux0) ^
		~(a | aux1)) & (~a & (a | aux0) & ~a | ~aux1) ^ (~aux0 ^
		~((aux1 | aux0) & (a | a)) ^ ~aux0 ^ (aux0 ^ a) & ~aux0 ^
		(aux1 | aux0) & aux0 & a) & ~(~(a | a | aux0 & a) ^ ~~(aux0
		^ a)) & aux0) + 26814 * ~(((~(aux1 ^ aux0) | a) ^ (a | ~~a)
		| ~((aux0 | aux1) & a | (a | aux0) ^ aux1 & aux0)) &
		(~~(~aux1 ^ aux1 & aux1) ^ (aux0 ^ aux1 ^ (aux1 | a) ^ aux1)
		& aux0 & ~aux0 & aux0 & aux0) | a) + 42831 * ((a & ~(aux0 |
		aux0) ^ ~(~~(a | aux0) ^ a & a & ((aux0 | aux1) ^ aux1 ^
		aux1)) ^ ((aux1 & a & (a | a) | (aux1 | a) ^ aux1) & (aux1 &
		aux1 | ~aux1) & a & a & aux1 | a)) & aux1) + 42508 * ((aux1
		| ~(~(aux0 | aux0 | ~aux0) | ~(aux1 & aux1 | aux1 | a))) ^
		~aux0 & ~aux1 & ~~(aux1 ^ aux0 ^ aux1) ^ ~aux1) + 56059 *
		(aux0 | a) + 56565 * (aux0 ^ (~~a & (a ^ aux0) & (aux1 ^ a)
		& ~(a & aux1 | aux0) | ~(~a & ~aux1 | ~aux0 & aux0 & a) |
		aux1) & aux1) + 20039 * ~((aux1 | aux1) & (~(a & a ^ aux1 &
		~a) | aux1 & aux1)) + 52844 * ~(~a ^ ~((~a ^ a ^ a) & ~a |
		~~(aux0 ^ a)) & (a & ~((aux0 ^ a) & (aux1 | aux0)) ^ aux0 &
		(aux1 ^ aux1 ^ aux0 ^ aux1) & (a & a | aux0 & a))) + 56304 *
		((aux1 | aux1 | (~~(a ^ a) ^ aux1 ^ (aux0 ^ a | ~aux1) |
		aux0) & aux1) & (~(~aux1 & aux0 | ~aux1 | (aux0 | a) &
		~aux1) & a & ((aux1 ^ aux0 ^ a & a) & (a ^ a ^ aux1 ^ a) |
		(a ^ aux0 | aux1) ^ (~a | aux1 & aux1)) ^ (aux0 & (aux0 & a
		^ aux1 | aux1) & (a & (a ^ a) ^ aux1 ^ aux1 ^ a) | aux1))) +
		52024 * ((((aux0 & (a ^ aux0) & aux1 & a & (aux0 | a) |
		~(~aux0 ^ aux1)) & ~(aux0 | (a ^ aux0) & a & a) | ~~((~aux0
		| aux0 & aux0) ^ aux1)) ^ ~(a ^ a ^ (a | aux0 ^ aux0 &
		aux0))) & (~a | aux0 | ~(a | aux0) & (aux0 ^ ~(aux1 | aux1)
		^ aux1 & aux0))) + 10322 * (aux0 & (~(aux0 & aux1) & (a ^ a
		| ~aux0) & ~(aux1 ^ aux1 | a ^ a) ^ (a & aux0 & (a ^ a) | ~a
		| aux0) ^ (aux1 ^ a ^ (aux1 | aux1)) & aux1 & aux0 & (aux0 |
		aux1) ^ ~((aux1 & a ^ a | ~(a ^ aux0)) & ~(aux1 & a))) ^ a ^
		(a ^ (a ^ aux0) & (aux0 | aux0) | (a | aux0) ^ aux1 | aux1)
		& ~aux0 & aux1) + 4427 * ~~~(~aux0 | aux1 | ~aux0 | a ^ a |
		aux0 & aux1 | ~(aux0 & aux1 ^ aux0 & a) | ~aux0) + 38212 *
		((~((aux0 ^ aux0) & (a ^ a) ^ (aux1 | a | aux1)) ^ ((~aux0 |
		aux1 | aux1) ^ ~(a | aux1) | ~(a ^ aux0 & aux1)) ^ aux1 ^
		aux0) & (a & (aux0 & aux0 ^ a & aux0) ^ (aux1 & aux1 & a & a
		| a & aux0 & ~aux0) ^ aux0 & a & a ^ aux1 ^ (a & aux1 ^
		~aux1) & a) & (aux0 ^ ~(aux0 ^ aux1) & (aux0 & a ^ (a |
		aux0))) & aux1 | (aux0 | ~a ^ (~aux0 | a ^ aux0) ^ aux1 ^
		~aux0) & (~(a ^ ~a) ^ (~a | a ^ aux0 | ~aux0 ^ aux0 & aux0)
		& ~(aux0 ^ aux0) & (~a ^ a & aux0 & aux1 & a ^ ((a | aux1) ^
		aux1 & aux0) & a))) + 25383 * (~aux1 ^ (aux1 ^ ~(~a & (aux0
		^ aux1))) & aux1 & ((a ^ ~aux1 | aux1 & aux1 & (aux0 |
		aux1)) & (~~a ^ aux0 & aux0 ^ ~aux1) ^ (~aux0 | ~aux1 | a) &
		~((aux0 | a) & ~aux1)) | ~(aux0 & a & (~aux1 | aux1 ^ a) &
		(aux0 ^ aux0) ^ ((a | aux1) & aux0 | a & a | aux0 ^ aux0) ^
		(~(a | aux1) | a & aux1 ^ aux0)) & ~aux0) + 57274 * (aux0 ^
		a) + 38781 * (aux0 & (~aux0 | ~(a | a) | ~~~~~a) ^ aux1 ^
		~aux1) + 2324 * (~(~~a & ((a | aux0) ^ aux1 ^ a) & ((a | a)
		^ a | ~(aux1 | aux0)) | (aux0 | aux0 & aux0 & (aux0 | a)) ^
		~(~a & (a | aux1))) & aux0 & ~~~(a & aux0) | ~aux1 & ~(aux1
		| ~aux1 | aux1 | aux1 | aux0 & aux0 | aux0 | aux1) & a &
		~aux0) + 15705 * ~(((~aux0 ^ (a | a)) & (aux1 ^ aux0) & ~a &
		((~a | aux0 & aux1) ^ aux1 ^ aux0 ^ a) ^ (~a & aux0 & (~aux1
		| a | a) | ~(a | a | aux0 & aux0)) | aux1) & (((a & a |
		~aux1) ^ (aux0 ^ a) & (aux1 ^ aux1)) & (aux0 ^ (aux1 & aux1
		| a ^ aux1)) & aux0 ^ aux0 & aux1 & a & a ^ ~a & ~~aux1 ^ a
		^ aux0 ^ aux0 ^ (a | aux1) & (aux1 | aux0))) + 46898 * ~((a
		& aux1 & (aux1 | a) & (a ^ a) | ~(aux0 | ~a & aux1 & aux0))
		^ ((aux0 | ~a & (aux0 ^ aux0)) ^ ~(a ^ aux1) | a) | ~(~(a &
		(a | a)) ^ aux0 | ~(aux1 | (aux1 | aux1) ^ a))) + 58539 * (a
		^ aux1 & a & (aux0 ^ ~aux0)) + 44071 * (((~~~(aux1 | aux0) |
		(aux1 | ~aux0 | ~aux1 ^ a & aux0) ^ ~(a & (aux0 | aux0))) &
		((~~a & ~aux1 & (aux1 ^ aux1) | ~(a & aux1 ^ aux0 & aux1)) ^
		((a | aux0) & (aux1 ^ a) | ~(a & aux1)) ^ aux1) | aux1 ^ ~(a
		^ ~(a | aux1)) ^ ~((a ^ a | a | aux1) & (aux0 ^ aux1 &
		aux1))) & ((a ^ ~(~aux1 & a & aux0) ^ ~aux0 | ~((a | aux0) ^
		(a | aux0) | ~(aux1 & a)) & ~aux1) ^ ~~(~aux0 | aux0 & aux0
		| (aux0 | a) ^ ~aux1 | ~~a | aux0))) + 36361 * (aux1 | ~((a
		& a & ~aux1 | (aux1 | a) & aux0) ^ ~~(a ^ aux1)) | (a & a |
		~(a | aux0) | a) & (aux0 ^ aux0 & aux1 & aux0 ^ ~~aux1) | a
		& (aux0 ^ (~aux0 | aux1) | ~((a | aux0) ^ a | a | (aux1 & a
		| aux1) & aux1))) + 4093 * ((aux0 & aux1 & a & aux1 & ~aux1
		& (a ^ aux1) & (~aux0 ^ ~a & (a ^ aux1) ^ ~(a & a ^ aux0)) ^
		a & ~~a & (aux0 ^ a) & (aux0 ^ aux0) ^ aux1 & aux1 & (a ^ a)
		& aux0 ^ ~~(a | aux1) ^ aux0) & (aux1 ^ ((aux0 & aux1 | aux1
		^ aux1) & aux1 & aux0 & (a ^ aux1) | aux0 | ~(aux0 | a)) ^
		(a | ~~aux0 | aux0) & (~~aux0 & (aux1 & aux0 ^ ~aux1) ^
		(aux1 ^ aux0 | aux0 ^ a) ^ a ^ a & a) | aux0)) + 42052 *
		(~aux1 & (~(aux1 & (~aux1 | a ^ a) & (aux0 | ~a)) & (aux0 ^
		a) & a & ~(aux1 & (a | aux0 | aux0)) ^ (a | aux1 ^ ~aux0 &
		~aux0 & (a ^ a) & a & a | a ^ aux0 ^ (~(a ^ aux1) | aux0))))
		+ 44920 * ((~~aux0 ^ ~(aux1 & ~aux1) | (a | aux0 | ~((a |
		aux0) ^ (a | aux0))) ^ aux1 | aux1 & ~~(a & aux1) | ~(a & a
		^ ~a) & (~aux1 ^ ~aux1) & (aux0 & aux0 ^ aux1 & a)) & aux0)
		+ 24805 * (~~(aux0 ^ (~a & a & a ^ (aux0 ^ aux0) & (a ^ a) |
		~aux1)) & aux1) + 63510 * ((aux1 | a | aux1 & aux1) ^ (aux0
		| aux0 | ~~a)) + 35367 * ((a ^ ((~(aux0 & aux1) ^ (aux0 ^
		aux0) & (aux1 ^ aux0)) & (aux0 ^ (aux1 & aux1 | a & aux1)) |
		aux0) | (~aux0 ^ ~(a ^ a) & a ^ ~(aux1 | aux0) & a & a) &
		~~aux1) & ((~(~aux0 ^ a ^ aux0) ^ aux1) & ((~(aux1 | a) |
		aux1) ^ ~((a ^ aux0) & ~a)) & (~~(aux0 ^ aux0) | aux1 & a &
		(aux1 ^ aux0)) & aux0 | ~(((~aux1 | aux0) & (aux0 ^ a) &
		~aux1 ^ ~(aux0 | a & aux1)) & (((aux0 | a) ^ ~aux1 | aux1) ^
		(~a & aux0 | aux1 | aux0 | ~a))))) + 15823 * (~~~(aux0 &
		(aux0 ^ (a | a)) ^ ~aux1 & (aux1 ^ aux1) ^ aux1) & ~(a ^
		(aux1 & (aux1 ^ aux0) & (aux1 ^ aux1) | ~~(a | aux1) | ~~a ^
		a ^ aux0 & aux0 ^ (aux1 | aux0) & (aux0 ^ aux0)))) + 24301 *
		((a | (~~(a & (a ^ aux0)) ^ ~((a ^ a ^ a & aux1) & (a ^ aux1
		^ ~aux1))) & a & ~(~(aux0 & aux1) & (a & aux1 | ~aux1))) &
		aux1) + 12748 * ~~aux1 + 64017 * (((aux1 | a & a) & aux1 |
		~~aux1 | a ^ aux1 & a | aux1 | ~(~~(a | aux1) & (a | aux1 |
		aux0 | aux0) & ~(aux1 | a))) & (a & aux0 & (aux1 ^ ~(aux1 &
		aux0)) | ~~~(aux0 | aux0)) | (~(a ^ a ^ a ^ aux0) & ((aux1 ^
		aux0) & (a ^ a) | aux1) & (a ^ aux1 & aux0 ^ aux0 | ~(aux0 &
		a ^ aux0)) ^ a) & ~((aux1 | a) & ~~(~aux1 ^ aux1 ^ aux1))) +
		13045 * (a | ~(aux1 ^ ~a & (a | a) & aux1 ^ ~((aux0 | aux0)
		^ aux1)) | ((aux0 | a | aux0) & (aux1 | aux1 & a) & (aux0 |
		aux0 | a | a) & ~~a & (~(a | a) ^ ~aux1 & aux1 & aux1) &
		(aux1 | aux1 | ~aux1 | aux1) ^ a) & (~((aux0 ^ a | aux1) &
		~a) & a ^ ~aux1)) + 60825 * ~(aux0 & a & (~(~~aux0 & (aux0 ^
		a ^ a ^ a)) ^ aux1 ^ aux0 ^ aux0 ^ ~aux0 ^ aux0 ^ (aux0 ^ a)
		& ~aux1 ^ ~(aux1 | aux1))) + 24430 * (aux0 | ~~(a | ~(~~aux0
		| aux0 ^ (a | aux0)))) + 9693 * (aux0 ^ a & aux0 & ((aux0 |
		aux1 & aux1 | aux1 | a ^ (~aux0 | aux1 | aux1)) & ~~(~a ^
		~aux1) ^ aux0)) + 43420 * ((~(~~((aux0 | aux0) & a) | ~((a &
		aux1 ^ aux0) & (aux1 | a))) ^ (a | aux1 ^ a & aux0 & (a |
		aux1) | a | aux1 | aux1 ^ ~(aux0 ^ a) ^ ~aux0)) & (a | ((a ^
		aux0 | aux0 | aux1) & (a & aux1 ^ (aux1 | aux0)) & a & a &
		aux0 | ~(aux0 ^ (a | a) & aux1 & a)) ^ ((a | aux1) ^ aux0 &
		aux0 | aux0 | (aux0 ^ aux0) & (a | aux1) | aux1 & aux1 ^
		~aux1) ^ ((aux0 ^ aux0) & (a | aux0) ^ ~~aux0) & (~aux1 &
		aux1 | ~aux0 ^ aux0 & a))) + 49359 * (~(~a & a | ((~aux0 |
		aux0 | aux1) & (a ^ ~aux1) | ~(aux1 | aux1 | aux0)) ^ aux0)
		^ ((~a & (a & (aux0 ^ aux0) | aux1 ^ aux0 ^ aux0) | (~aux0 &
		(a | aux1) ^ ~aux1 ^ aux1) & (aux1 & aux0 | aux0 ^ aux0 | a
		& aux0 ^ (aux1 | aux1))) & ~aux1 | ~aux1 | ~(aux0 & ~a &
		(aux1 ^ aux0) & ~aux0 ^ ~a ^ (a ^ aux0 | a)))) + 8378 *
		(aux0 ^ ~(((aux0 & aux1 | aux1 | a) & a ^ ~(aux0 & a ^ aux0
		^ a)) & (a ^ (~a | aux0 ^ aux0) ^ (aux1 ^ aux0) & (aux0 ^
		aux0)) ^ ~(aux1 & aux0) & ~(aux1 | aux0) & ~(aux1 & a | aux0
		& aux0) & ~~~~aux1)) + 54171 * ~((~a & ~a ^ aux0 ^ aux1 ^ (a
		| aux0) | aux1 & a ^ (aux1 | aux0) | a | (aux0 | a) & a &
		aux1 & ~(aux0 | a) ^ a) ^ ~aux0 ^ ~~((aux0 & aux0 ^ a & aux1
		& (a | aux0)) & ~(aux1 & a & aux0 & aux0))) + 8933 * (aux1 |
		a) + 12639 * (aux1 ^ ~((aux1 | aux0 | a ^ a) ^ ~aux0 ^ ~aux0
		^ (a | aux0 ^ a)) ^ aux1 | aux0 ^ (aux0 | aux1 & ~((a | a) &
		(aux0 | aux1)) & ((a | aux1) & a & aux0 ^ ~a ^ (aux1 | aux0)
		| a ^ ~~aux0))) + 59490 * (((a & (aux1 | aux0) ^ (a | a |
		~aux0)) & aux0 ^ (~(aux0 & (a ^ aux1)) | ~~(a & aux1)) ^
		(~(aux0 | aux0 ^ aux1 | aux0 & aux1 ^ aux1) | aux1) ^ (~(a &
		aux0) & ((a | aux1) ^ aux0 & aux0) | (aux1 | aux1) ^ a | ~(a
		^ aux1) | ~(a | a) | ~(~~~aux0 ^ ~(a ^ aux0 ^ (aux0 |
		aux1))))) & ~(~aux1 & ~(aux1 & ~(aux0 | a) ^ ~(aux0 & a |
		a)))) + 38086 * (~(~~((aux1 | a) & aux0 & aux0) & ((a |
		aux0) & (aux0 ^ aux0) & (aux0 ^ a | a ^ aux1) | a) | aux0 |
		aux0) & aux0) + 4528 * ((a ^ ~aux1 & a & (~aux0 | aux0 & a)
		^ ~~~a ^ a ^ aux1) & ~(aux0 ^ ~a ^ ~aux1 ^ (aux0 & aux0 | a
		| ~aux0) | ~~aux0) ^ ~aux1) + 16351 * ((aux0 ^ (aux0 ^ a ^
		aux0) & (aux1 | a | aux1 ^ aux1)) & (aux1 ^ (aux1 | aux0 |
		~aux0)) & (aux0 & aux0 ^ ~a ^ (aux1 | a) & ~a) | (aux1 &
		aux1 ^ ~aux0 | aux1 & aux1 | a) & ((aux0 & aux1 ^ a & aux0)
		& a ^ ~((aux1 ^ aux1) & (a | aux0))) | ~((~aux1 & aux0 &
		aux0 ^ (a | aux1) ^ a ^ aux1) & (~(aux0 | a) ^ ~(a | a))) &
		~(a | ~(aux1 ^ (aux0 | aux0))) | (aux1 | aux0 ^ ~(a ^ ~aux1)
		^ ~(aux0 ^ aux1 | aux1 & aux1) ^ aux0 ^ ~~aux0) & ((a | aux1
		| aux0 | a | ~a) & ((a ^ aux1) & aux0 & aux1 ^ aux1) | ~~(a
		& aux0 ^ aux1)) & (aux1 & aux1 & (aux0 | aux1) & a & ~((a |
		aux0) & (a | aux1)) ^ ~~(aux1 ^ a | ~aux1))) + 51774 *
		(((~(aux1 | a) | ~(aux0 | aux1) | aux0) & (~(aux0 | aux1 ^
		a) ^ (aux0 & aux1 | ~aux0) & (a ^ aux0) & aux0) ^ a ^ ~aux0
		^ aux1 ^ aux1 ^ ~(aux0 & a) ^ a) & (a | a & aux0 | a | aux1
		| ~(aux1 | aux1) | (a | aux0 | ~a) & aux1 & (~(a ^ a) ^ a) |
		aux1 ^ aux0 | ~aux0 | (a | aux1) ^ (aux1 | aux1) | ~~aux1 |
		~a & (a | aux1) & (aux1 & aux0 | a | aux0) ^ aux0 ^ a) |
		~(aux1 | ~((aux1 | aux1) ^ a | aux0 ^ aux1 ^ aux1)) ^ (aux1
		| aux0 | ~((a ^ aux0 ^ aux0 ^ aux1) & ~(a ^ a)) | ~aux1)) +
		13824 * ((a | (aux1 ^ a & aux0 & a) & (a | aux1 | a | ~(a &
		aux1)) & ~aux0 | aux0) & ((aux1 | aux0 & aux0) & (~(a ^
		aux1) ^ (a | ~aux0 ^ (aux1 | a) ^ ~(aux0 | aux0))) ^ (aux0 ^
		(aux1 | aux1) & aux0 & aux0 | (aux1 & aux1 | aux1 & aux1) &
		(aux0 ^ a | a ^ aux1) | ~(a | a | aux0 & a) & ~(aux1 ^ a) &
		(aux1 ^ aux0 ^ ~aux0) | (~(aux1 ^ aux1 ^ (aux0 | aux0)) | a)
		& (aux0 ^ a ^ (a | aux1)) & (aux1 | aux0 | a ^ aux0) & aux0
		& aux1 & (a | a) & (aux1 | aux0) & aux0 & aux1))) + 12320 *
		(aux1 & ~~(aux0 | aux0) ^ (~(a & aux1 ^ (a | a) | ~(aux0 &
		aux0)) | aux0 ^ a ^ aux1 ^ aux1 ^ a ^ (a | aux1 & aux0) &
		(~a ^ (aux0 | a))) ^ (~(~aux1 & aux1 & aux0 | ~~a) | ~~(aux0
		& aux0) & (~(aux1 | a) | ~~aux0)) ^ aux1) + 63830 *
		((~(~(aux0 & aux1) ^ (~a | aux0 & a)) & ~(a | ~(aux0 ^ a)) |
		(~aux1 & aux1 | ~~~aux0) & ((~aux0 ^ aux0 ^ aux1 | aux0) ^
		(aux0 ^ a | aux0 ^ aux0))) & a ^ aux1) + 8706 * ((aux1 ^ a)
		& (a ^ ~(((aux1 | aux1) ^ ~aux1) & ~(a ^ aux0)) & aux0) & (a
		^ ~(((a | aux1) ^ aux0) & aux1 & aux0 & (a ^ a) & (aux0 & a
		& (aux1 ^ aux1) | aux0 ^ aux1 | aux0) | aux0 ^ (aux0 | a))))
		+ 11024 * ~(~(~(((aux0 | aux1) ^ aux0 & aux1) & (a & aux0 |
		aux0 | a)) & (~~aux1 | aux1) & aux0 & (a & a ^ aux1 ^ aux1))
		^ (aux1 & (~~(a | aux1) ^ (aux0 & aux0 ^ aux0 & aux1 | a)) |
		(a ^ ~(a & a | ~aux0)) & aux0)) + 16673 * ~((~(aux1 & a ^
		~aux1 ^ (aux1 | a | aux1 ^ aux1) ^ ~(aux0 & a) ^ ~(aux0 ^
		aux0)) | aux0 | (a | aux1) & (aux1 | aux1) & ~~a & ~(a ^ a |
		a | aux1)) ^ ~(aux0 | ~(a & a) ^ ~aux0 | ~(~(a & aux1) |
		~(aux0 | aux0)))) + 31170 * (~(~(aux0 ^ aux1 ^ ~aux0 & aux0)
		& (aux0 & aux1 & ~aux0 ^ aux0 & a & aux1 & aux0 | a)) & ~~(a
		& a) & ~~a) + 11593 * ~(~(a & aux0 & aux0 | aux1 ^ a ^ a | a
		| a & a | aux0 & (aux1 | a)) & aux1 & (~a & ~(aux0 & a) ^
		((aux0 ^ aux1) & aux1 & aux1 | aux1)) & aux0) + 4986 *
		(~~~(aux1 & ~aux0 ^ aux1 | ~(~aux0 & aux0 & aux1)) & (a |
		a)) + 39129 * (~(~((a ^ a | a & a) & (a ^ (aux1 | a))) ^ ~a)
		& (aux1 | aux0 & (a | aux1) & aux0 | aux1 & aux1 ^ aux0 &
		aux0 ^ a & (aux1 | a) | ~(~~a ^ (aux0 | aux0 | aux0 ^ aux0))
		| ~(aux1 & aux1 & aux1 & ~aux1 & ~aux0)) ^ aux1) + 49546 *
		(~(aux0 | aux0 & ~~aux1 | (aux0 ^ a) & (aux1 ^ a) ^ ~aux1 ^
		aux0 ^ a | ~(~a & (aux1 | a)) ^ ~a) & (~~(aux0 & (aux0 | a)
		^ (a ^ aux0 | a | aux1)) | ~~aux1 ^ aux1 ^ aux1 | aux0 &
		~aux0 & (aux0 ^ ~aux0 ^ ~a)) & ~~((~a & aux0 & aux1 | ~aux1
		| ~aux0) & ~(a ^ a ^ a))) + 22402 * (((~aux1 & a ^ (aux0 |
		aux0) & (a | a) | a) & ~((a | aux0 & a) & (a | a) & aux1) |
		((a | aux1 ^ a | ~a | ~a) ^ a) & (~(aux1 & a | aux0 ^ aux0)
		^ (~a ^ ~aux0 | aux0 & ~aux0))) & a ^ a) + 9998 * ~(aux0 ^
		(aux1 ^ ~(a ^ aux1) | (a ^ a | ~aux0) & (aux1 | aux1 | ~a))
		^ ~a ^ (~(~(aux0 | a) & (aux0 & a | aux0 | aux0)) ^ (aux1 &
		aux1 ^ aux0 & a) & aux0 & ~((aux1 | aux1) & (aux0 | aux0)))
		& (((aux0 | aux1 | aux1 & aux0) & a | ~~(aux1 | aux0)) ^ (~a
		^ ~aux0 | aux1 | a & aux1) ^ a ^ ~(aux1 & aux0))) + 36025 *
		(~(~(~(~a | aux1 & a) | aux0 & aux0 & a & (aux1 | aux0)) |
		aux0) | (a | aux0 | a ^ (aux1 | ~aux0) | ~(a & aux1 & aux1 &
		aux0)) & ~~a) + 23542 * (~(~(aux1 & (~aux0 ^ ~(aux1 ^
		aux0))) & ~aux0) & aux0) + 39781 * (~(~~(a & a | aux1 &
		aux1) | a & (aux0 | aux0) & (a | a) ^ aux1 | aux0) ^ aux1);
        vm->memory[vm->sp] = a;
    // std::cout << "[DEBUG] DEC: stack[" << vm->sp << "] " << old_val << " -> " << vm->memory[vm->sp] << std::endl;

    WORD next = vm->fetch_opcode();
    vm->handler_table[next](vm);
}

void VirtualMachine::op_add(VirtualMachine* vm){
    WORD b = vm->memory[vm->sp++];
    WORD a = vm->memory[vm->sp++];
    WORD aux0 = b<<a;
    WORD result = 24459 + 10030 * (aux0 ^ aux0 ^ ~aux0 ^ a) + 63616 *
		(((~(b ^ b ^ (a | a) | aux0 & ~aux0) | aux0 & (a | aux0) &
		((aux0 | a) ^ aux0) & b) & aux0 & ~((a | a | b) & ~(aux0 ^
		b)) ^ ((b ^ aux0 | aux0 & a) & ~~b & ~b | aux0) & (~aux0 |
		(b ^ b) & ~b ^ aux0 & (b | aux0) & a)) & ~~(~((aux0 | aux0)
		& aux0) ^ b | ~(aux0 & b & (b | b)) ^ (~a | aux0 & aux0) &
		~~a)) + 45454 * b + 19546 * (aux0 | (b | aux0 | ~(a ^ a)) &
		b) + 8793 * ~(b ^ aux0 & (~aux0 & ((a | b) ^ ~aux0) | ~a & b
		| a)) + 48217 * (aux0 & ~(~~(b | b) | ~(b & a | ~b) & (aux0
		| b)) & aux0) + 18682 * (~(~a | ~((b | ~aux0 | (aux0 ^ a) &
		b & aux0) & ~((aux0 | b) & (b ^ a)))) ^ b) + 42497 * (((a ^
		b) & (((~a ^ (a | a)) & (b | b) & (aux0 | b) | a) ^ ((a | b)
		& (a ^ a) | ~aux0) & (a | b) & b & ~aux0) | (aux0 ^ aux0) &
		(aux0 ^ aux0 & b) & (a | ~aux0) & ~(~aux0 & ~b) | (a ^ aux0
		| b ^ b | ~b) & a ^ ~a ^ b) ^ a ^ b ^ (~~~~b ^ (a ^ b ^
		(aux0 | aux0) | ~(b | a)) ^ (a | aux0) | ((aux0 | b | ~b) ^
		~b & (aux0 ^ b)) & ~a & aux0 & b & (a ^ a) & a)) + 19835 *
		~aux0 + 858 * aux0 + 27438 * (aux0 & ((~b ^ a & aux0 | (a ^
		a) & aux0 & aux0) & ~(~aux0 & aux0 & aux0) ^ ~(b ^ b) &
		(aux0 ^ b | b ^ aux0) ^ ~((a ^ b) & (b | b))) ^ (b ^ (b | b)
		& (aux0 | ~b)) & b ^ ~(b & a | b | aux0 | aux0 | aux0 | a) ^
		~(~b & (a ^ aux0) | aux0 ^ a ^ ~b) ^ ~(a & ~~a) ^ (aux0 ^
		~(aux0 | a) | aux0 ^ (aux0 | a) ^ b ^ aux0 ^ a) ^ (~~(a &
		aux0) | ~(aux0 | b)) ^ aux0) + 44352 * (b | ~(((a | b) ^ b ^
		b) & (aux0 | a | ~b) & (a | b & aux0 ^ (b | a))) | ~~(a ^ b)
		^ ~(~b ^ a) | (a ^ ~(aux0 ^ a)) & b | (aux0 | b | b ^ b |
		~(aux0 & aux0)) ^ ~~a & a ^ a & aux0 | (~b | b | aux0) & b ^
		~~(b ^ a) | a ^ (a & a ^ a | (aux0 | aux0) & a)) + 49863 *
		((((aux0 | a | ~a | ~b | b | b) & ((aux0 | a) ^ b | ~aux0 |
		b ^ a) | ((b | b) & (b | b) ^ aux0 & (b ^ a)) & aux0) ^
		~~((aux0 | b) & ~a) ^ ~a & ((a | aux0 | a) ^ ~aux0) | (aux0
		^ b ^ aux0 & b | ~aux0 | ~b | ~a & a | aux0) & ((a ^ a | b)
		^ ~(~aux0 & (a | aux0) & ~(aux0 & b)))) ^ a) + 27138 * (~b &
		(~(~a | aux0 & a) ^ ~(a & aux0) ^ a ^ (b | a) | aux0) &
		(~(~b | b ^ aux0 | a | aux0 | ~a) | a) | ((~~b ^ aux0 & a &
		b & a) & ~a & (~b ^ ~(a ^ a | aux0 & aux0)) ^ (a & (a ^
		aux0) | ~b & (aux0 | a) ^ ~a ^ aux0 & aux0 | ~((aux0 | a &
		b) ^ (b | b) & ~a))) & b & b) + 2442 * (~~(b & ~(a & (b | b
		& aux0))) & b & ~(a ^ ((a | aux0 | b) ^ (aux0 ^ aux0) & b) &
		~(b | b & b))) + 9152 * ~(((~~~b | b) & (~a | ~a | (aux0 |
		aux0) & aux0 & a) & a | ~(a | a) ^ ~a ^ aux0 & ~(b & b) ^
		(~(b ^ aux0 | ~b) | b ^ (b & aux0 | b & aux0))) ^ b ^ (a |
		~(b & b) ^ (b | a) & ~b | a ^ a ^ b ^ a ^ a & b | b)) + 4640
		* (~(~~aux0 | ~(aux0 ^ a ^ aux0 ^ aux0) & (aux0 | a & aux0 |
		(b | a) & (aux0 ^ b))) ^ ~(b | ~aux0 & (aux0 ^ aux0) | ~~b |
		~(a & a & a & aux0 ^ a & aux0 ^ ~aux0)) ^ ~(aux0 | ((a |
		aux0) & (a | a) | ~aux0) ^ aux0 | a ^ ~~(a & b & a))) + 5346
		* (~a & b & (aux0 ^ b ^ aux0 ^ (~a | (aux0 | b & (a ^ a)) ^
		(b ^ b ^ aux0) & ~a & b))) + 23632 * ~~~~(b | aux0 & ~~a) +
		59467 * a + 22748 * ~(((~(aux0 ^ b) ^ (aux0 | b) ^ aux0 ^
		aux0 ^ ~b) & (~(a ^ b) ^ aux0 & (a ^ aux0) ^ ~(b & a | a &
		aux0)) | ~(b ^ ~aux0) ^ ~(b | a) & (~aux0 | aux0 & aux0 | b
		& a)) ^ (b | (~(~b & b & a) | b & a & a & b ^ ~(aux0 & b)) ^
		~(~(b ^ a) & (~b ^ a ^ a)))) + 59498 * (~aux0 ^ (~a ^ (aux0
		& b | b ^ a) | ~~(b & aux0) | ~(~b ^ (b | a)) | ~(a ^ b |
		b)) ^ b ^ b & (b & (aux0 | a) & (b ^ a) | ~~(aux0 & a)) & b
		& a & aux0) + 9659 * (~a ^ aux0 & (b & (a ^ a ^ ~a) ^ aux0)
		& ((aux0 | a ^ ~a) & a ^ (~a & a & ~aux0 | (b | aux0) ^ b &
		(b | a))) | (b ^ ~((a | b) & a & aux0) | ~((a | aux0) ^ (b |
		a)) ^ (a & aux0 | aux0 | ~(a | aux0)) | aux0 & (aux0 & a ^
		aux0 ^ aux0 ^ a ^ (a | aux0 & b) & (a | a) & aux0 & aux0)) &
		((~a | a) ^ aux0 | b)) + 62172 * ~a + 56402 * ((~a | b | a)
		^ (~(aux0 | ~~~aux0 ^ ((aux0 | a) & aux0 & b | (a | aux0) ^
		b ^ a)) | ~(aux0 ^ (aux0 | a | aux0 & b | ~~a) | ~(b & a))))
		+ 13786 * (b | (aux0 | ~(b & a)) ^ ~(aux0 & b) ^ b & aux0 ^
		(b | b) ^ ~aux0 & (b ^ aux0 ^ a | ~aux0 ^ (b | a)) | b &
		~aux0 | ~(b ^ aux0) | aux0 | aux0 & ((a & b | a & b) ^ a ^ a
		^ a) | ~~aux0) + 20591 * (~a ^ b ^ a & ~(~a ^ a & b ^ b ^ a
		& a) ^ ~b) + 35190 * ~((~b ^ (~b | b | aux0) ^ a ^ aux0 ^
		~b) & a) + 7823 * (a & ((b | ~(a & aux0)) & a ^ aux0 ^ aux0)
		& (a | ~aux0) & ~aux0) + 49564 * (((~~~(~b ^ b ^ aux0) | b |
		b | aux0) ^ b ^ (b ^ ~~(a ^ aux0)) & aux0) & a) + 46638 *
		(~(~~(a & aux0) & a & (b | a | b | a) ^ ~(aux0 ^ (b | aux0)
		& (b | aux0)) | aux0 & (~aux0 & (aux0 | b) ^ (b & b | a &
		a)) & ~(~b & b & a)) & b) + 32933 * ~((~b | a | a | ~aux0 &
		(aux0 | b)) & ~(a & a) & b | ~(b & aux0 ^ aux0 | ~b ^ a & b)
		| a | ~(a | aux0 & aux0 | a) & ~(b ^ aux0) & (aux0 | aux0 ^
		aux0) & ~~a ^ ~(aux0 & (~aux0 | aux0 ^ a)) & ((aux0 & b | b
		^ a) ^ a & ~a | aux0 & a & (aux0 ^ b) & b)) + 9299 * ((aux0
		^ aux0 ^ ((aux0 | (~a | ~aux0) ^ ~a & (aux0 ^ aux0)) & aux0
		| ((a ^ aux0 | aux0) ^ a | (~aux0 | a ^ a) ^ (aux0 ^ b | a &
		aux0)) & a)) & (~(~aux0 ^ (a | b)) & (~b ^ aux0 & b ^ (a ^
		b) & (b | a)) & a | ~(aux0 & (a | ~(a | a))) | ~~(~(aux0 | b
		& b) | ~a ^ (b | b) ^ b))) + 13673 * (b & ~~(aux0 & b | b &
		b) & (~((b | b) & b | aux0 & b | aux0 ^ aux0) ^ a) ^ aux0 |
		aux0) + 2175 * ~(~~((b & (aux0 | a) ^ ~a & b & aux0) & ~(a ^
		a | a & b)) & b & ~a & ~b) + 20493 * ((aux0 ^ aux0 ^ a) &
		~(aux0 | a) & ((a ^ aux0) & (aux0 ^ b) ^ (a ^ b | aux0)) &
		(a | b | b | a & aux0) & (a | a) & (a ^ a) & a & aux0 ^ b &
		(~aux0 & ~b & (~b | aux0 & aux0) | ~(b ^ b | b & b) | (a & a
		^ a ^ a) & ~(a ^ aux0) ^ a) | ~(a ^ aux0 ^ b ^ aux0 | (a |
		a) & b & b) & (aux0 & a | b) & a & ((aux0 | aux0 | aux0 |
		(aux0 ^ aux0) & ~a | a ^ (a ^ b) & (b ^ aux0)) & a ^ (b ^ b)
		& b & ~a & ~a)) + 15781 * ((~(a & (a ^ a | aux0 | b)) ^
		~~~a) & (~(a | ~aux0 | aux0) ^ ~(a & a & (a ^ aux0) | a ^ b
		& a)) | (a & (a | b & a) ^ a & (a | a) & (~aux0 ^ ~a)) & a &
		(~aux0 | ~aux0) & ((aux0 ^ aux0) & ~b ^ a & b) ^ a | a ^ (a
		^ (~b | aux0 ^ b) ^ (b | b) & a ^ (b | b) ^ (a | aux0)) &
		~(b | b | a | b) ^ ((~aux0 ^ a) & (b & a | aux0 | aux0) ^ b)
		& (~b | (b ^ a | b & a) & (b ^ aux0) & (a ^ a))) + 32306 *
		(a & (b | a | b & aux0 & (a | b) | a) ^ (aux0 & b ^ a & b ^
		~a ^ aux0 | ~(a & aux0 | a | a)) ^ ~(aux0 & b & ~aux0) &
		(aux0 ^ ~b ^ a) ^ b | b | (a | ~b) ^ (((a | a | ~b) ^ ~(aux0
		^ b)) & (a | a) & (b ^ aux0) & ~a | ~(~a & (aux0 | aux0)) &
		~b)) + 12224 * (~aux0 & a) + 8045 * (~a ^ ~~(~((b | b) ^ a &
		b) | ~aux0 | ~b | aux0)) + 41873 * ~(~(b | (aux0 ^ aux0 |
		aux0 | a) ^ (b | a)) & (b ^ (a | ~((b ^ aux0) & (aux0 ^
		a)))) | (b | b) & ((a | aux0) & aux0 ^ ~aux0 | ~b) ^ ~~(a ^
		~aux0 ^ aux0)) + 6966 * ~((aux0 | aux0) & (b | ~(aux0 | a ^
		(b ^ b | aux0 | aux0)))) + 41020 * (a | b & (~(((aux0 | b) ^
		a & b) & (a & b | b | b)) ^ ~~(b & aux0) & b & a) ^ ~~(((a |
		aux0) ^ a | aux0) ^ a ^ (a ^ aux0 | b & b))) + 26595 * (((a
		| b | b ^ aux0) & ((aux0 | b) ^ b) ^ ~(b ^ aux0) ^ aux0 &
		aux0 ^ aux0 ^ aux0 ^ ~~~(b ^ aux0) | (~~b & ~a | ~(a ^ a) ^
		~b & (b | b)) ^ ~((b ^ aux0 | aux0 ^ b) & (a ^ b ^ aux0 &
		aux0))) ^ (b ^ (~b | aux0 ^ aux0 ^ (a | a)) | a | (a ^ ~(a ^
		a)) & ((aux0 | b) & (aux0 | a) ^ (aux0 | b | a | aux0)) |
		(~(a ^ aux0) ^ (~a | a)) & aux0) | a | ~((b & aux0 ^ (aux0 |
		a) | ~(b ^ aux0)) ^ (b & aux0 & (a | a) | a)) & aux0) +
		33083 * ~~b + 47459 * ((a & ~aux0 | ~aux0) & ~b & (~~b ^
		(~(a & b) & ((a | aux0) ^ ~a) ^ aux0) & a & (aux0 ^ (~a |
		~aux0))) & (b | ~(a & aux0) & ~(a & a) & (a ^ a) & ~(a &
		aux0) | ~((~a | a & a) & ~~aux0))) + 2794 * (~((a ^ b ^
		~aux0 ^ (b ^ a | ~aux0) | ~a & b ^ ~(a | a)) & ~~((a | b) ^
		a & a) & b & ((~aux0 ^ ~b) & ~(a & aux0) | (a ^ aux0 | aux0)
		^ a & ~b)) ^ (a & ~((a | b) & (a | b) & ~aux0) | ~(~a & ~(b
		| b)) | b) ^ a) + 30548 * ~(a | aux0) + 35664 * ~((((b ^ a |
		~b) ^ b & a & aux0 & a) & ((~b | ~b) ^ a) & (~~b ^ ~(aux0 |
		a) ^ (~a | aux0 & b | ~(aux0 | b))) ^ (~aux0 | ~a | a ^ a) ^
		~(a ^ aux0) & a & aux0 & b ^ aux0) & ~(~(aux0 ^ aux0 ^ a) |
		~(~aux0 & (a ^ a)) | a | a | aux0 | aux0 | ~a)) + 22030 *
		~(b & (b ^ ~~~aux0) & (~((a | aux0) ^ ~aux0) ^ a ^ aux0 ^ (a
		^ b | aux0 ^ a) ^ aux0) ^ a) + 40492 * (~~~~(b & a & (a |
		b)) ^ (aux0 | ~~((aux0 | a) & (a ^ b) | (a | b) ^ (b | b)))
		^ (~(~(b | aux0 | aux0) & aux0) | b & (~(b ^ aux0) ^ ~a) ^ a
		& a & ((a | aux0) & (aux0 ^ b) ^ a & a & (b ^ aux0)) | a)) +
		56517 * (b & aux0) + 27234 * ((~~b | ~((b ^ b ^ aux0 ^ aux0)
		& ~a & (a ^ a) ^ ~aux0)) & aux0 | a & aux0 & aux0 & ~b | b)
		+ 35657 * (~((a | a) & ((aux0 & a | b & a) ^ ~~aux0 ^ b) ^
		b) | ~~(a & ~(a | a & b)) & (b & (a ^ aux0 | ~aux0 | aux0 |
		b) | aux0 | ((~a | b | a) ^ ~~b) & (~(b ^ b) ^ aux0 & a ^
		~a) | ~((b | a) & ~a) | (a ^ aux0) & (b | a) | b ^ a ^ b)) +
		40581 * (aux0 & (~(~(a | aux0 ^ aux0) & ~a & aux0 & ~b) &
		(aux0 ^ ~a ^ (aux0 | aux0) ^ (b & a & b | aux0 & a & aux0) ^
		(b | ~(b ^ b) | b & (aux0 ^ a) ^ (b ^ a) & a & aux0)) | aux0
		| (aux0 | b) ^ (b | aux0) | ~~b | aux0 ^ a | aux0)) + 18240
		* (a ^ (~(~~(b | b) & ~b) | ~(aux0 & aux0 & (aux0 | aux0) &
		~a & (aux0 ^ b) | aux0) | ~~~(a & a))) + 39284 * (~(b &
		aux0) | (~(~(a & aux0 & ~aux0) | ~~(b ^ b)) ^ aux0 ^ a & ~(a
		^ b ^ a & a)) & ((b | b ^ b ^ (a | a) | ~(a | aux0) | aux0 |
		(b ^ a) & a & a) ^ ~~(a ^ aux0) & a & (aux0 & aux0 ^ b & b |
		~a | a ^ a) & ~(aux0 | b | b))) + 52850 * (~(a | b) ^ (aux0
		& b & a & aux0 | b | aux0 | b) ^ (aux0 | b) | ~b) + 58963 *
		((a & aux0 & b & a & b | ~(a | a) ^ ~a | ~(a ^ b) | ~b &
		aux0 & b) & aux0 & b | ~(b | ~((~(b | b) ^ ~a & (a ^ a)) &
		(~aux0 & ~b ^ ~(a & b))))) + 33495 * ~~(b ^ aux0 ^ ~~(~b ^
		~b)) + 2061 * ((b & ((b & b | ~b) ^ aux0 ^ b ^ (aux0 | b)) |
		b) ^ a ^ ((b ^ b | aux0) & (b ^ aux0 ^ a & b) & b ^ (b |
		~aux0 | aux0 ^ b) & ~a & (a | b) & (a & b ^ aux0 & a) | a) ^
		b) + 23752 * (~~~((b ^ b) & (aux0 ^ a) ^ (~aux0 | aux0 ^ a))
		^ aux0 | a | ~(~(aux0 ^ aux0) | ~~aux0) & b & b) + 28223 *
		((((aux0 | aux0 & b) ^ ~a ^ b) & aux0 ^ (b ^ ~~aux0) & (aux0
		^ a ^ b & b ^ a) & ~aux0 ^ ~(((b | a | a ^ a) ^ ~aux0 & (b ^
		aux0)) & ~b) ^ aux0 & b) & (a ^ ~~(aux0 & ((a | a | b) ^
		~(aux0 & b))))) + 28724 * ((~~a ^ b | ~~aux0 | b) & ~((b | a
		| aux0 | a) ^ (a | a) ^ (aux0 | a) ^ (~b | aux0) & aux0 ^
		~~~aux0 ^ a ^ a ^ aux0 ^ a ^ (aux0 ^ aux0) & a) & (aux0 &
		((~(~a ^ ~a) | (aux0 | b | b) & b & a) ^ ~(b ^ (aux0 & aux0
		| a ^ b))) ^ ((a | a | b ^ b | b & (b ^ b)) & ~aux0 ^ a) &
		aux0)) + 42774 * (((b | aux0) ^ (~(b & aux0) | b | b & aux0)
		^ (aux0 & a | aux0 | b | (aux0 ^ aux0) & (b ^ aux0) | ~a ^
		~b ^ a ^ aux0 ^ b ^ aux0) | b) & (~(aux0 | aux0) ^ ~b & aux0
		^ (b ^ b) & b & (~b | a ^ aux0) | a) & (a ^ (aux0 | aux0)) |
		(b | b | ~(~aux0 | aux0 & b) & b & ~aux0 & ~b & (~~~a ^ ~~a
		^ a)) ^ a) + 52004 * (~(~(a & b ^ ~aux0 ^ ~(a & a)) | a &
		(aux0 | a) & ~~aux0 ^ (b | aux0) & (a | b) & ~~b | aux0 & a)
		^ ~((a & (aux0 ^ a) & ~(a & aux0) ^ ~~aux0) & (~(a | b) & (a
		^ b ^ ~b) ^ (a | b | a | a) ^ ~(aux0 ^ a)) & a & ~(a ^ aux0
		^ ~aux0))) + 15796 * (~((aux0 & a & b & b | aux0 ^ (aux0 |
		aux0) ^ aux0 & aux0) ^ b ^ (b | b) & (aux0 ^ b) & aux0 &
		~(~a | aux0) & ~~(aux0 & ~aux0)) | a) + 18229 * ~(a & ~((~(a
		& b) | (b | a) & (aux0 ^ a) | ~~a & aux0) ^ ~(a ^ (b | a)) &
		((a | b) ^ b & aux0) & (a & b | b ^ b))) + 3404 * (b & (~(b
		^ ~~a) ^ ~((a | aux0) & b) ^ ~(b ^ (a | a))) & ((~(~a | b |
		aux0) | ~~a ^ (a ^ aux0 | a ^ aux0) | ~(~(a ^ b) ^ ~(b ^
		a))) ^ ~aux0 & ((a ^ aux0) & a | (aux0 | a) ^ b) & (a & a ^
		~aux0 ^ ~(b ^ aux0)) & (b | b & b & a & aux0)) ^ ~((b & aux0
		^ (aux0 ^ a | aux0 & a) | ~(a ^ a ^ b & a)) & a) & ~((~b |
		aux0 ^ aux0) ^ ~(a & a) | ~a | (b & aux0 | aux0 & b) & aux0
		| ~(a ^ (a | b)))) + 10023 * (b | aux0 & a & ((b | b) ^ ~~a
		& (b & b ^ ~a)) & ~(~(aux0 & a ^ a ^ a) & b) | b) + 1864 *
		(((aux0 & b | aux0 & a) ^ (b ^ aux0) & a | ~~b & (a & a ^
		aux0 & aux0)) ^ ((b | a) ^ b ^ a ^ b | ((aux0 | a) ^ b) & (b
		& a | b ^ b)) ^ (b | b & ~~a | aux0) ^ ~((~aux0 ^ aux0) &
		(~b | ~aux0 ^ a) ^ (aux0 | aux0) ^ a ^ (a & b ^ b ^ b | a &
		aux0 ^ b ^ aux0)) ^ (a | ~(aux0 ^ ~a | b | a | aux0 | a) |
		aux0) ^ ~b ^ b ^ (~~aux0 ^ ~b ^ aux0) & b) + 1706 * ~~~b +
		34197 * ((b ^ ~~(b & aux0 | a | a) ^ b ^ (a | a) ^ ~a ^ aux0
		^ b ^ b ^ ~(b ^ b)) & aux0 & b & (a & a & (a ^ b) | ~~a) &
		~~(b ^ aux0) & b | a ^ ~((a ^ b ^ (aux0 | aux0)) & (a ^ a ^
		b) | aux0 & b ^ b ^ aux0) ^ ((~(b | aux0) ^ aux0 & (a | a))
		& aux0 | aux0)) + 49512 * ~(b | a & (b | (aux0 & a ^ ~b) &
		aux0) ^ ~((aux0 | a) ^ (a | a) ^ (a & aux0 | a & aux0)) &
		~(~aux0 ^ aux0 ^ b)) + 9111 * (aux0 | (~~(b ^ aux0) ^ b ^
		(aux0 | (a & aux0 ^ a ^ b) & ~a & ~a)) & ~aux0 | a) + 51776
		* ((a & (~(b ^ a) | ~a & (b ^ b) | aux0) | ~((aux0 | aux0) &
		~b ^ ~(aux0 ^ aux0)) & (b | ~a) | ~b) & ~(a ^ aux0)) + 18889
		* (~(b & (~(aux0 & aux0) ^ (aux0 ^ a) & (a ^ aux0) ^ a ^
		aux0 ^ b ^ (~a | aux0 & b))) ^ (a | (aux0 ^ (aux0 ^ b) &
		aux0 & a | ~(~b & (aux0 ^ aux0))) & (b | ~(a | a) & ~(b |
		b))) ^ ~(~b & b & b)) + 32722 * ~(aux0 ^ ~(b | ~aux0 | a) |
		aux0 ^ (aux0 | b & (a ^ b) & ~a) | b & ~(b | aux0 ^ a ^ b))
		+ 12902 * ~(b ^ (b | ~~aux0) ^ ~((b ^ a) & (a | a) & a & ~(a
		& b) ^ a)) + 64000 * ((~(((b | a) ^ (a | b)) & (b & aux0 | b
		| aux0)) & (aux0 | (b | aux0) ^ aux0 & aux0 ^ ~(a ^ b)) |
		((a | aux0) ^ (a ^ aux0 | b & aux0)) & aux0 | ~(aux0 | aux0)
		& a & aux0) & ~((aux0 & (a | b) & b ^ (~b | a ^ a | ~a ^ b &
		aux0)) & b) ^ ~(b & (~(b ^ aux0) ^ ~(a & (aux0 ^ aux0)))) ^
		~~(aux0 ^ aux0 | ~aux0) & (~(b & a) ^ b) & (~(aux0 ^ aux0 |
		aux0 & a ^ b & aux0) ^ aux0 ^ (~aux0 & ~b | ~(aux0 |
		aux0)))) + 34999 * ((~a ^ ~~(a & (a | a ^ a) ^ (~(a ^ b) | a
		& a))) & (a & (aux0 ^ aux0 ^ aux0) ^ ~(aux0 | ~(a ^ (b |
		aux0) | a)))) + 48090 * (a & (~~(aux0 | a) & ~a & ((b | b) &
		(aux0 ^ aux0) & (b | a & a) | b) ^ b ^ aux0) | ~(b ^ (aux0 &
		a & a & a & b | aux0 ^ a & ~b)) | a & a & (~(a & b) ^ aux0 ^
		(a | a)) | ~((a & aux0 | ~a) ^ ~(a & a)) | ~((aux0 | aux0 ^
		b) & (aux0 | a)) ^ ~b) + 28743 * ~(~~~((a | a) ^ ~aux0 | b &
		a ^ (a | a)) | b ^ b | ~b | a & b | aux0 | aux0 | aux0) +
		64115 * ((~((a ^ aux0 ^ a) & (a & b ^ aux0 ^ a) ^ ~(a | aux0
		| b)) | ~aux0 | aux0 & b & ~(~~b & (~b | b ^ a))) & aux0) +
		18256 * (aux0 & (aux0 ^ ~~~((aux0 | a) ^ (aux0 | aux0)) | b
		^ (b | ~(b & ~b) | (a ^ aux0 ^ ~aux0 | ~~b) ^ ~b & b & a &
		b))) + 8547 * (~(~aux0 | ~((~a | a | aux0) & (b | aux0 | a))
		^ b) & aux0 & ~(aux0 ^ (a | b | b) & (~b | b & aux0) ^ b ^
		(a | b) & (b ^ a))) + 20530 * ((~~aux0 | a) ^ aux0) + 48615
		* (a ^ (((b & a ^ a) & (aux0 | ~aux0) ^ aux0 & aux0 & b ^
		aux0 & a & (a ^ a)) & a & aux0 | ~((a ^ aux0 | aux0 & aux0)
		^ b | a) | ~aux0)) + 30400 * (~(~~aux0 | (b & aux0 & (aux0 ^
		b) | a & aux0 ^ (b | a)) ^ ~(aux0 | aux0 | a | aux0) | (aux0
		^ aux0 | a | aux0 | ~b) & (b ^ a) ^ b) ^ aux0) + 51735 *
		(~~(b ^ a ^ ~~a ^ (aux0 ^ b) & ~a & aux0) | a | a) + 65139 *
		(((aux0 & aux0 | a | aux0) ^ aux0 | (a | a) ^ ~a | ~b) & a &
		(aux0 | aux0) & b & ((aux0 | aux0) ^ aux0 ^ b) | aux0 | ~(b
		& (b ^ a) ^ b ^ (b | aux0 & aux0 & b & b)) | aux0 | ~((((a |
		a) ^ a & aux0) & (~aux0 | aux0 & b) | ~(a ^ a ^ b ^ aux0)) ^
		(aux0 ^ a) & b ^ (a ^ b) & (b | a) ^ (~(a | b) | ~a & a &
		aux0) ^ ~b)) + 7544 * (~(~(a | b | ~b | ~(a ^ b)) | aux0) &
		(~(~(aux0 & aux0) | ~(a ^ b) | aux0) | ~((aux0 ^ b) & a & b
		| b ^ a | b | b) ^ aux0) & (~~a ^ ~((aux0 | ~b) ^ a) & ~a))
		+ 57319 * ((((b & a ^ aux0 | b ^ b) ^ (a | b | b | a)) & (~b
		^ (b ^ (a | b) | ~~a)) | ~(aux0 | a & a | ~b) ^ (a & a ^ a |
		a & a ^ ~a) ^ (b ^ a) & ~a ^ (b | a | aux0 | b) | ~((a ^ b |
		aux0 ^ a) & aux0 ^ aux0) | aux0 & a) & ((~(~a | (b | aux0 |
		aux0) ^ b & a ^ aux0 & aux0) | a) ^ (~b ^ ~a ^ ~aux0 & a ^ b
		^ a ^ (a ^ a) & (aux0 | aux0) ^ ~aux0) & aux0 & (~~(a & a) ^
		(a | aux0)))) + 63130 * ((~~(aux0 & aux0 | a) & ~(a ^ aux0 ^
		b | a | a | ~a) ^ ~a) & (b ^ ~(~a | a ^ a) | b | aux0 ^
		(~~aux0 ^ aux0 | b ^ ~b ^ ~aux0)) | ~(b & (b | aux0 ^ a ^ b
		^ aux0)) | b & (a & aux0 ^ (aux0 | b) | b & b & (aux0 | a))
		| b ^ a | (a ^ ~~a & (~b | aux0 ^ b) | ~(~(aux0 ^ aux0) &
		(~a ^ (b | aux0)))) ^ aux0) + 24374 * (~(((~((aux0 | a) ^ b
		& aux0) | a | a & (aux0 ^ a)) ^ ~~(aux0 & aux0 | aux0 ^ a))
		& (~~b & (~a ^ b) & ((a | a) ^ aux0) & a | b)) ^ (~aux0 |
		b)) + 47348 * (~(((b | b) ^ a ^ a & (b | a) | b | aux0 | b ^
		aux0 ^ aux0 | ~aux0) & ((b | a) & ((~b | aux0 ^ aux0) ^
		~~aux0) | b | ~(b & b & (b ^ b)))) | ~~(a ^ ~~(~aux0 ^ aux0
		^ a))) + 49756 * (b | ~~(a & a & a & a & (aux0 ^ ~b) & (~b |
		~b | ~aux0 & ~a) ^ (a | a & (~b ^ (aux0 | aux0))))) + 60119
		* (a ^ (aux0 | ~(b & aux0 ^ (b | a)) & b) ^ ~(~b ^ b) | ((a
		| a) ^ a ^ (a | b | a | b)) & (~b ^ a & aux0 & b) & (b &
		(aux0 ^ a) & aux0 & aux0 | ~((aux0 | b) ^ ~b)) ^ b ^ ~(~((a
		& b | a | aux0) ^ aux0 & (a | b)) | a)) + 32935 * (((a &
		aux0 ^ (b | a)) & (aux0 | a) & a & b & ((b | b) ^ a & a) &
		~b ^ ~~b ^ (~aux0 | aux0) ^ (aux0 ^ aux0 ^ aux0 ^ a | (a |
		aux0) & (aux0 | a)) | aux0) & ((~((a | aux0) ^ a) | ~~(b &
		aux0)) & (~(b & a) | b & a ^ b ^ b | ((a | b) ^ b & aux0) &
		~(aux0 | b)) ^ ~b) | ((a ^ (aux0 | b | b | aux0) | ~(a ^
		aux0) & (b | a) & a & b ^ a) ^ (~(a & a) & (aux0 ^ ~aux0) |
		(aux0 ^ a ^ a) & b & aux0 | a)) & (~~((b | a) ^ (aux0 | b))
		& (~aux0 & (b | b) | aux0 | (~a ^ (b | b)) & (a | a) & ~a) |
		b | ~~a ^ ~~a ^ (~~aux0 | aux0 & (b | b)))) + 19877 * (~a |
		~((aux0 ^ ~aux0 ^ b) & (~b | aux0 | ~~~aux0))) + 17022 *
		(aux0 & ~(aux0 & aux0 & (aux0 | b)) & ~a | a | b | b ^ ~~~a
		& (aux0 ^ a & aux0 & b) ^ a ^ b ^ (b | b) ^ aux0 & aux0 ^ (a
		| b) ^ ~(~aux0 & b & a) ^ (b | a) ^ b ^ (aux0 ^ aux0 | aux0
		| a) ^ (aux0 ^ a ^ (aux0 | aux0)) & b ^ a) + 41677 * (a |
		aux0 | a ^ ~~(a & aux0 ^ ~aux0) | ~(a & (~(aux0 | aux0) ^
		a))) + 41387 * (a ^ ~a ^ aux0) + 58112 * (((~(a ^ b | b & a)
		^ ~((a | aux0) & a & b)) & aux0 | a) ^ ~~~(a ^ aux0 ^ a ^
		aux0 & a ^ aux0 & aux0) ^ (~a | ~~b) & ~~~aux0 & (b | (aux0
		^ a) & b) & (~(b ^ aux0) ^ (a | b | aux0 ^ a)) & a) + 50722
		* ~b + 1894 * ~((((~b | a ^ a) ^ ~~aux0) & (~b | aux0 | ~b)
		^ (aux0 ^ (b | b) | aux0) ^ (a & a & a | ~~aux0) ^ a) & a) +
		47952 * ~~(~(~(~a ^ a) & ((a & b | aux0) ^ (b ^ a | aux0 |
		a))) & ~(aux0 & a & ~b & aux0 | ~a ^ ~(a ^ aux0))) + 31298 *
		(~~((aux0 ^ a ^ aux0) & (b & a ^ aux0 & b) | aux0 & ~aux0) |
		aux0 | aux0 ^ ((~(b & a) | a) ^ aux0 ^ aux0 ^ a & a ^ a |
		(~b ^ a & a ^ b ^ ~b) & (~b | a & b | (a | b) ^ aux0 &
		aux0)) | ~((b ^ b | a) ^ ~b & a | ~(~aux0 ^ a)) ^ ~~~~(aux0
		| b)) + 48820 * ((a | (a ^ ((aux0 | aux0) ^ a | aux0 | ~b))
		& ~b & ~(b & a ^ a)) ^ ((b & a & a ^ (a ^ b) & (aux0 |
		aux0)) & (aux0 ^ b & b | b ^ b ^ ~aux0) ^ (aux0 | ~(aux0 &
		b)) ^ b | (b | (~aux0 | ~aux0) & ~(aux0 & a)) & b) | b |
		~~(~~~b & (b & a | a | aux0 | a & ~a))) + 63718 * ((((b & a
		| b & aux0) & ~~b | b | ~(b & a | a | aux0) & ((aux0 | aux0
		& b) ^ ~~b)) ^ (aux0 | (b | aux0) & (a ^ b) ^ (aux0 ^ aux0 |
		a ^ b) | b & a ^ b | b) | a) ^ (a | aux0 & (b & b | b & b |
		~aux0 ^ a | ~(~b | a & a))) ^ ~~((aux0 | a | aux0) ^ ~~a | a
		^ aux0 ^ (aux0 | b) | b)) + 6988 * ~(aux0 ^ a) + 30423 *
		~~(b & b | b) + 17322 * (b & ~((~(b ^ a ^ b) | ~(a ^ b |
		~b)) & (~(~a ^ a) | ~~(aux0 & aux0)) & ~(b ^ b))) + 34349 *
		(a | ~b) + 24293 * ((a & (~aux0 ^ a) | ~b | ~~a & (a | a | b
		^ aux0) | ~(a | aux0 | ~aux0) | a ^ aux0 ^ a | a | b) &
		((~~a ^ ((a | a) & a | aux0)) & ~~b | a & aux0 | (b & a | a
		& a) ^ ~(a ^ aux0) ^ ~~(aux0 | b)) & ~(b & ~a & b)) + 19127
		* (~(~(~aux0 & (~aux0 | a)) & (b ^ b ^ b ^ (a | a)) & aux0 |
		~(((a | aux0) ^ a & aux0) & ((a | aux0) ^ ~a)) ^ ~(~b ^ (b |
		b)) ^ (a | a & (a ^ aux0))) ^ a & (b | ~~a & (~~aux0 | (b ^
		a) & aux0 & a)) ^ ~(aux0 | b | ~(b ^ b) | b)) + 56587 *
		~(~(~(~b | ~a) & aux0 | ((a | aux0) & (a ^ b) | (b | b) &
		(aux0 | a)) & b) ^ (b ^ ~(a | b) ^ ~a ^ b ^ a | b & ((aux0 ^
		aux0) & a | aux0 & b & (aux0 ^ a)) | ((b | a | aux0) ^ a &
		aux0 & (aux0 ^ b)) & aux0)) + 24113 * ((a | ((aux0 | aux0) ^
		(b | a) | aux0 & aux0 ^ aux0 & b) ^ (a ^ b) & a & b ^ ~b &
		~b) ^ (a | ~~(a | aux0) | ~a) | (~~b | ~b) ^ (~(aux0 & aux0)
		^ (a & a | ~b | a)) & b | b & (aux0 ^ ~(a ^ b ^ (b | a) ^ (b
		^ b) & a ^ b))) + 28413 * ~(~(~(a ^ a | b | aux0) ^ b) | ~a
		| b | ~~(~b ^ b & a) | ~~a | (b | aux0) & a & aux0 | ~b |
		~aux0) + 29468 * (~(~a ^ b ^ (a | a ^ a ^ aux0) & ~(a ^ ~b))
		^ ~(~aux0 | a & a | a) & a & ~(b ^ b | a & aux0) ^ (aux0 | a
		& b ^ ~aux0 | ~aux0) ^ ~(a | aux0 | ~aux0 | (aux0 ^ b) & a)
		^ (((aux0 | b) & (b | aux0) | aux0) ^ ~~aux0) & (b | aux0 |
		b | b & a ^ aux0 ^ aux0 | (a | a) & (a | aux0) & aux0) &
		(~(a & aux0 | ~b) ^ a ^ ((aux0 ^ aux0) & b | b ^ ~aux0) ^
		((a | aux0) ^ aux0 ^ b | aux0))) + 35223 * (((aux0 ^ a & a ^
		aux0 ^ b ^ (b & aux0 | aux0 | aux0) & (aux0 | a | b | aux0)
		| ~((b | ~a) ^ b ^ b)) ^ b | ~~((aux0 & b ^ (a | a) ^ ~aux0
		^ b) & (b ^ aux0 | b ^ aux0) & ~(a & aux0))) ^ (~~(a | aux0
		^ aux0 ^ aux0 | a | a | aux0 & aux0) | (~~(~a ^ a & b) | a ^
		(a | aux0 ^ b | ~(aux0 ^ aux0))) ^ (~((aux0 ^ a) & (aux0 |
		b)) ^ ~b & (aux0 | aux0) ^ aux0 | ~((~b | b) ^ (~aux0 |
		~a))))) + 59545 * (aux0 ^ (~((aux0 | aux0) ^ ~a ^ (a | b) ^
		(aux0 | b)) ^ b | (~(b ^ a) ^ (b | a)) & (aux0 & aux0 ^ ~b |
		~(a & b)) ^ aux0) | ~~((a | b | aux0 | aux0) ^ aux0) | ~a &
		((a | a) ^ aux0 & a) ^ ~~b ^ (~(b | aux0) | a) & ((b | a | b
		^ aux0) ^ ~a & b & b) | (~(aux0 & aux0 & (b | b)) | b | (a |
		a | a ^ a) & ~(a ^ b) | ~~(aux0 & aux0)) & ~((a & (a | b) ^
		b) & (aux0 | aux0 | a | a | a & a | a | a))) + 26809 * ~(a &
		(b | ~(a & b | aux0 | ~~aux0) & aux0)) + 5359 * (b & (aux0 |
		(~(aux0 ^ b) ^ aux0) & ((aux0 | b | aux0) ^ (aux0 | b | aux0
		& aux0))) & ~(a | a | ~aux0 | aux0 & (aux0 | b) | ~~aux0) &
		aux0 & (b | a & (a & a | a | b)) & (aux0 ^ aux0 | ~a |
		~(aux0 | (a ^ a) & (aux0 | aux0) | (a ^ a | ~a) & (aux0 | b
		| ~b)) | b)) + 39441 * (~a ^ ((aux0 | (b | aux0 & aux0) &
		aux0 & ~a) & ((b ^ b | ~a) & ~(b & b) | ~(aux0 & (b | a))) |
		a) ^ ((b & b & (b | a) | ~(aux0 & a) | a) ^ ~(a | a | aux0 &
		b | a ^ aux0 | ~b) | ~aux0)) + 62426 * ~(aux0 & (aux0 ^ ~((b
		^ b | a ^ a) & (aux0 | b ^ b)) & (aux0 ^ (aux0 | b) & aux0 ^
		(a ^ b) & (a | b)))) + 21371 * ~~(~a & aux0 & ~~aux0 & (~(b
		& a) | aux0 | b) & (aux0 | aux0 ^ a | a ^ a) | ~b) + 56098 *
		(b & (b ^ a ^ ~(aux0 & a & a & a & aux0)) ^ ~~(~b ^ (aux0 ^
		a) & (b | b) ^ aux0) ^ a ^ a ^ ~(b ^ (b | b)) ^ ~aux0 ^
		(~~~a | a & (~aux0 | ~a) ^ (~aux0 | a | a))) + 8784 *
		((~(~aux0 & (b & (a | a) & ~b | ~b)) | b) & ~(b ^ aux0)) +
		27076 * (a | ~b ^ a) + 2374 * ~~(~~a & ~(b | a) ^ ~a ^ ~(b ^
		a ^ b & aux0 ^ (a & a | a | a)) | ~a & ~~~a & (~a & (a ^
		aux0) | b | ~(aux0 & a) & (~b ^ ~a))) + 25148 * ((~~~(aux0 &
		b ^ a ^ b) & ((~b & (aux0 | b) | ~aux0 | ~aux0 ^ (a & aux0 |
		~a)) ^ (a | (aux0 | aux0) ^ (b | b)) & ~((b | b) & ~a)) |
		~(~(b & aux0 ^ aux0 & b) & a | (~b ^ aux0 ^ a) & aux0 & aux0
		& b & (a ^ aux0))) ^ b) + 64553 * ~~((a & (~~aux0 ^ aux0 &
		aux0 ^ b ^ b) ^ ~aux0) & (~(aux0 | aux0) ^ ~(aux0 & a & (b ^
		b))) & ~(a ^ ~(a | a))) + 25064 * ~(~~(aux0 | b | aux0 ^
		aux0) & (~(~aux0 & b & a) ^ aux0 & ~b ^ aux0) & (~~((a | b)
		^ b & a) ^ ~~~a & ~aux0) | ((aux0 ^ b | aux0 & aux0) & a |
		~(b & aux0 & aux0) | ~~~(a ^ aux0)) & ~((a | ~b) & (~(aux0 &
		aux0) | a & a ^ a))) + 11054 * (a & (a ^ (a | b | b & a)) &
		aux0 ^ b | ((aux0 | (b | a) & aux0 & b) & aux0 | (~(aux0 ^
		b) | a ^ aux0 | a & aux0) ^ (b ^ a ^ ~aux0 | ~aux0 ^ aux0 ^
		aux0)) & (a & a ^ b ^ a ^ aux0 | b | (b | b) & (b | aux0) &
		b & aux0 & aux0 ^ ~(b ^ a ^ aux0)) | (((aux0 | b | aux0) ^ b
		& aux0 & (a | b)) & (b & aux0 ^ b ^ ~(aux0 & a)) ^ ~~(a ^
		aux0 ^ (a | aux0))) & aux0) + 46369 * (b & (a | (b | b) & (a
		| a) & (aux0 | b | b)) | a ^ (b | a | a) ^ aux0 ^ ~(a &
		aux0) ^ ((b | a) ^ b) & ~(a ^ b ^ a) | ~(b ^ (aux0 | ~aux0)
		^ b & ~a & a & aux0) & ((b | ~(b | aux0)) ^ (b | aux0) ^
		aux0 ^ a ^ (~b | b) | (~(aux0 & aux0) | a ^ aux0 ^ (aux0 |
		a)) & (~(b & a) ^ (a | ~b))) | a) + 18783 * (~aux0 & (a ^
		aux0 ^ a) & (~((a | aux0) & ~a) ^ (a | (a ^ a) & aux0 & b))
		& (a ^ ~(~b ^ b ^ b)) ^ (~(a ^ aux0 & b & (aux0 | a) ^ aux0
		& aux0 ^ ~aux0) ^ aux0 | (a | ((a | b & b) ^ (aux0 | a) ^ b)
		& (a | a)) ^ ~(aux0 & aux0 & a & b | b & a ^ aux0 & a) & (a
		& (~aux0 | ~b) | ~a & a & b & ~~aux0))) + 7655 * (aux0 ^
		(~(a & a & aux0 | b & aux0 ^ b & b) & (a ^ aux0) ^ (~((aux0
		| aux0) ^ ~aux0) | aux0 ^ aux0 ^ aux0 ^ a) & (a ^ ((aux0 |
		b) ^ a ^ aux0 | (a ^ b) & ~b)) | ~(~(a | b & (b ^ a)) ^
		(aux0 | b | ~a | ~(b & b | b | aux0))))) + 190 * (((~(b & a
		| a | aux0) ^ ~a & aux0 ^ (a | b | b | a) | a) ^ b ^ aux0) &
		~~(aux0 ^ a | a | b & aux0 & (aux0 | aux0) | a ^ (~b | a |
		b) | a)) + 49523 * (~~(b & a | ~a) & a & (~~((aux0 | aux0) &
		aux0 & a) ^ ~((a ^ b) & ~aux0 ^ ~~a)) & ~(~~(b | aux0) ^ a))
		+ 18448 * (a & b | ~(aux0 & (((a ^ a) & (aux0 & aux0 | aux0
		| a) | (b & a | b) & b & a & a) ^ (b | b)))) + 10613 * ((~(a
		& aux0 & (~(b & aux0) ^ (aux0 | b) ^ aux0 & aux0)) ^ aux0 &
		((~aux0 | b & aux0 | b | a | ~(b ^ aux0 & aux0)) ^ (a ^ a) &
		a ^ a ^ (a & a | aux0 & b) & ~b & a & b)) & (~a ^ ~(b &
		~~(aux0 ^ a) & ~(a ^ a) & ((aux0 | b) ^ aux0 ^ b)))) + 61766
		* ((~((~aux0 ^ aux0 & aux0) & a | aux0) | (b | ~aux0 | aux0
		| ~aux0) & ((b | aux0) ^ (a | a) ^ (aux0 | aux0 | b) ^ (b ^
		b) & ~aux0 & (~a | ~aux0))) & ~((~(a ^ b ^ b ^ b) | aux0 |
		aux0 ^ aux0 | ~(a ^ a)) ^ aux0) | (b | b) ^ ~~(~(aux0 & a) |
		a ^ b ^ ~b) & aux0 & (aux0 ^ a ^ ~b ^ (aux0 ^ a) & ~a ^ ~((b
		| aux0) & (b | a)))) + 40 * ~~~aux0 + 28575 * ((a | (b ^ b |
		aux0 ^ a) ^ ~(a & b) ^ (~(a ^ a) | a ^ a ^ b) | b ^ b ^ b ^
		(b ^ a) & a ^ b) ^ ~aux0 | ~(~((a | b | aux0 | b) & (a ^ a ^
		a)) | ~(a & (b | a)) ^ (~(aux0 | b) | a | a | b ^ b)) &
		((~(a & b) | (aux0 | b) & b & b) ^ (aux0 ^ b ^ aux0) & (b ^
		a) & (b | a) ^ (aux0 & aux0 | b | b) & (~b ^ (aux0 | b)) &
		(b | b)) & (aux0 & aux0 ^ a ^ a ^ (b | aux0) ^ (aux0 & b |
		aux0) & ~(a & aux0)) & ~a) + 22186 * ((~~(aux0 & a & a ^ (a
		| b) ^ (aux0 | aux0)) | ~((b | aux0) ^ ~a ^ (a & aux0 & (b |
		a) | ~b & (a ^ b)))) ^ a ^ (~a ^ (b | ~aux0)) & ((a ^ b & a
		| b | (b | a) & (aux0 ^ a) | ~a) & (~(a & b) | a & aux0 | a
		^ aux0 | aux0) ^ ~(~~b ^ (aux0 & a ^ ~aux0 | (aux0 | b) ^ b
		& b)))) + 5425 * ~~a + 64312 * ((aux0 | aux0 & ~(a ^ a) | b
		| aux0) & a | a) + 34938 * (((b & (~a ^ a ^ aux0 | ~aux0 |
		aux0 ^ aux0) ^ ~((b ^ a) & aux0 ^ ~a ^ (a | a))) & (aux0 |
		(aux0 ^ a) & (a | aux0) ^ aux0 & ((aux0 | b) ^ b ^ aux0)) ^
		(b | ~(~aux0 ^ b ^ a ^ aux0) & (~aux0 ^ (a ^ b) & ~b ^
		~(aux0 & b | a)))) & ((aux0 ^ ~b) & ~~(aux0 & aux0) & (b &
		aux0 | aux0 | aux0) & b ^ ~((aux0 ^ b ^ aux0 ^ aux0 ^ ~(a &
		a)) & ((aux0 ^ aux0) & ~aux0 | ~aux0 | a ^ b)) ^ ~(aux0 | b
		^ a | aux0 ^ aux0) & (a | a ^ b ^ a ^ (aux0 ^ b) & a) & a))
		+ 39968 * (b & aux0 & (~(((aux0 | b) & ~b | b | aux0) & a) |
		~a | (a ^ aux0) & ((a ^ aux0) & ~a | a ^ aux0 | a ^ a |
		(aux0 ^ b) & ~a))) + 43965 * ((~((a | b) & (~b & (aux0 | a)
		& ~(b ^ a) ^ aux0)) | ((b | aux0) & b | ~aux0 ^ a & b) & ~(b
		^ b ^ b ^ b) & ((~aux0 | ~aux0) ^ ~(b & b) | b) & aux0) ^
		~(b & aux0)) + 16900 * ((((~(a ^ a) | ~(a | a)) ^ (a & a &
		(b ^ b) | ~(a ^ a))) & ((a ^ a | b & a) ^ b & b ^ (a | a) ^
		(aux0 ^ aux0) & a & b & b & (aux0 | aux0)) | ~a | ~(~(b &
		aux0) & ~(aux0 | a))) & b ^ (~~(a ^ a & a ^ (b | aux0) ^ ~(a
		^ aux0)) | (~(a & (b | aux0) & a) | ~(a | ~(a & a))) ^ ~~~a
		^ (~aux0 ^ (b | a)) & ~~a & ~aux0)) + 6113 * (~a & (a |
		aux0)) + 1494 * ~~~((aux0 | b) ^ b ^ a ^ ~b & (aux0 ^ b) ^
		aux0 ^ a ^ b ^ ((b | aux0) ^ b & b | ~aux0 | a | (aux0 | a)
		^ (a | aux0))) + 37782 * ~(b & (~((~(b ^ b) ^ ~(a | a)) &
		(~(a & aux0) ^ ~~b)) ^ ~(a & ~((a | a) ^ (aux0 | b))))) +
		64419 * ((b ^ (~(aux0 | aux0) & a & ((aux0 | aux0) & a & a ^
		aux0 & aux0 ^ ~aux0) ^ ~aux0 & aux0) & (~~a ^ ~aux0 ^ ~b)) &
		(a ^ (~(aux0 & a & (b ^ b)) | a ^ a | ~b | ~aux0) & (aux0 &
		b ^ ~b | aux0 & aux0 | b | b) & (b & aux0 | aux0 ^ a) & ~b)
		& a) + 36772 * ~~(b | b) + 17302 * ~(((~(aux0 | aux0) |
		~aux0 ^ aux0) & ~(b & aux0 | ~aux0) & b | ~(aux0 & a) ^ aux0
		^ ~(a & aux0) ^ ~b) & aux0) + 50818 * (aux0 & (aux0 | b)) +
		27903 * ((((a & aux0 | aux0 & aux0) ^ aux0) & a & ~~(b ^ b ^
		aux0 ^ aux0) | aux0 | ~((b | a) ^ a ^ b) | ~~(aux0 | a) & a
		| a) ^ ~((b | a | a ^ a | (aux0 | b) & b) ^ (~(aux0 ^ b) | a
		& aux0 ^ (aux0 | b))) ^ ~b ^ a ^ b ^ ((aux0 ^ aux0) & (a ^
		aux0) | b & a | b | a) ^ (~(aux0 & b) | a ^ a ^ b) ^ ~~(b |
		b) & (aux0 & b | a) ^ ~~((a ^ a) & ~a & ~~b)) + 14624 * (b &
		a);
    vm->memory[--vm->sp] = result;
    // std::cout << "[DEBUG] ADD: " << a << " + " << b << " = " << result << " [sp=" << vm->sp << "]" << std::endl;

    WORD next = vm->fetch_opcode();
    vm->handler_table[next](vm);
}

void VirtualMachine::op_sub(VirtualMachine* vm){
    WORD b = vm->memory[vm->sp++];
    WORD a = vm->memory[vm->sp++];
    WORD aux0 = b>>a;
    WORD result = 22706 + 23703 * ((a & (~~aux0 ^ ~~aux0) ^ b) & (aux0 ^
		(aux0 | aux0 | ~(aux0 | a))) | ~~b | ~~b | ((aux0 & a | a) ^
		b ^ aux0 ^ a ^ a | b) ^ a ^ ~~aux0) + 9066 * (~(((b | b) &
		~a & ~(aux0 | a) ^ aux0 & (~b | aux0 & b)) & ~(b | b | aux0
		| (a | b) & ~b) & ((aux0 ^ aux0) & (b ^ b) ^ ~a ^ (~a ^ ~a |
		aux0)) & (b | a | b | aux0 & aux0 & b & b | a)) | a | ~(a |
		~aux0 | ~a | ~aux0 ^ aux0) | ~~((b | a | a | ~b ^ aux0) ^
		b)) + 39760 * ((~((a | a | a & b) ^ (a ^ a | a | aux0) ^
		aux0) | ((~aux0 | b & aux0) ^ ~(a & a) ^ a & ~b) & aux0) ^
		(b & a & b & (b | b) | b | b ^ ~((aux0 | a) ^ (a | a))) &
		~((a ^ a | ~a) ^ (aux0 | b | a ^ a) ^ ~(aux0 | aux0 | aux0 |
		b)) ^ aux0) + 39839 * (~((~b & ~b & (aux0 ^ aux0) & a & aux0
		| ~(a & aux0) & (a | a) & (a ^ aux0)) & (~(aux0 & a & ~aux0)
		^ (a ^ b | a & a) ^ (aux0 | a) & (b ^ aux0))) & ((~b | b &
		aux0) ^ ~aux0 ^ ~a ^ aux0 & b ^ b ^ b ^ b & a) & aux0 &
		(~((a ^ a ^ (aux0 | aux0)) & ~~b) ^ ~~(a & (aux0 ^ a))) ^
		~(~((a | b | aux0) & ~(b ^ b)) | ~~a ^ b ^ (a ^ aux0) &
		(aux0 | aux0) ^ ~a | ~(aux0 ^ (~(b & aux0) | aux0 & a ^
		~a)))) + 42625 * (a & ~aux0) + 53402 * a + 32308 * (~b |
		aux0 & (~aux0 ^ ~(b | a))) + 21736 * ~(aux0 ^ a) + 16668 *
		~a + 20737 * ((a & (a | a) & ~(a & b) ^ ~(aux0 & ~b) ^ aux0
		^ b) & (~aux0 & (aux0 | aux0 ^ b) ^ (b & b & aux0 | a ^ b |
		b ^ a) | a | ~(b & aux0 & a)) | aux0 & ((a | (a ^ a | b |
		aux0) ^ (a ^ a) & ~b) ^ ~(a ^ b) & ~(aux0 | aux0) & (~(b |
		a) | (aux0 | a) ^ b & aux0)) | ~~(~((b | aux0) ^ (b | b) |
		~b) ^ ~((~a | b | aux0) ^ (aux0 | a ^ a)))) + 46480 * (aux0
		^ (b | ~~b) & (~(~(b | aux0) ^ b) | ~(aux0 & ~aux0) ^ ~b) ^
		~~a ^ ((a & b | aux0) & (aux0 ^ aux0 ^ ~aux0) | aux0 & (aux0
		^ a) & ((aux0 | aux0) ^ ~aux0)) & ((a | a) ^ b ^ ~~b ^ (a |
		aux0 | ~aux0 ^ (aux0 | b))) ^ ~~(~a & a & aux0 | aux0 | a ^
		a)) + 592 * b + 4077 * ((b & ~(a & ~(aux0 | b)) ^ a) & a &
		~~(a ^ aux0 ^ aux0 ^ ~aux0 & (b ^ aux0)) & (a | (a ^ a) & ~a
		^ (aux0 ^ aux0 ^ b | (aux0 | aux0) & (b | b) ^ ~b & b & b |
		aux0))) + 21658 * (aux0 ^ ((~(a & aux0 ^ ~b ^ ~b) | ~a | a |
		b) & ((a | b | a ^ aux0) & (a | b) & (a ^ a) ^ ((aux0 | a) ^
		aux0 & aux0 | a | a)) & ((a | aux0 | aux0) ^ a) & ~((b |
		aux0) ^ (b | a)) | (~~a ^ aux0 & b & ~aux0) & ~((a | aux0) ^
		aux0 ^ a) ^ (a & aux0 | a | b | ~a ^ a) ^ (aux0 & a ^ aux0 &
		aux0) & (aux0 & b ^ b) ^ a & (~~(b ^ a) | b & b & aux0 & a &
		a))) + 43716 * (~b ^ ~(((b | ~(a ^ b)) ^ ~~(a | aux0)) & (~b
		^ b ^ b) & b & (~b ^ aux0 & b | b) | aux0)) + 43808 * (aux0
		& ((a ^ (~~a | aux0) ^ a & (a ^ b) & (~b | a ^ b) ^ a | (b &
		a & aux0 & a & (b & aux0 | a ^ b) | ~(a & b) | ~b ^ aux0 &
		b) ^ a) ^ ~(aux0 & (b | b | aux0 | b) & (aux0 & aux0 ^ (b |
		a)) & (((aux0 ^ b) & a | aux0 & b) ^ ~((b | a) & a & b)))))
		+ 45915 * aux0 + 52905 * (~(~aux0 & ((a | ~a) ^ ~aux0 | aux0
		& aux0 ^ a & b | aux0 & (b | aux0)) & (b | a ^ ~aux0) & ~(b
		| aux0) & ((a ^ b) & (b ^ b) | ~a & b)) | b) + 42480 * ~~(b
		| aux0 | (aux0 & a & (aux0 | a) ^ a ^ aux0 ^ ~aux0 | ~(~b ^
		~aux0)) & ~aux0) + 17642 * (~(b & b ^ aux0 ^ a & b & aux0) &
		aux0 | b & (~a ^ b & a) & ~(a ^ a) ^ ((~b | aux0 ^ a) ^ (b |
		b & a) | ~(a | aux0) & (a ^ aux0 | aux0)) | (~b | ~(a ^
		~aux0 ^ ~b)) & (aux0 | ~aux0 & b) & ~~~(aux0 | a) | ~(b |
		(~aux0 ^ aux0) & (b | a & b) | ~aux0) & b & (a ^ aux0 ^ ~b)
		& (a ^ a ^ b & b) & aux0 & (~(~aux0 | b) ^ (~aux0 | aux0 & b
		& ~a)) & b) + 41147 * (~(aux0 & (a ^ ~aux0 & (a | a) & a &
		((aux0 | aux0 & b) ^ a & aux0 ^ a))) & ~~~a) + 11813 * (b ^
		a | ~(~(~(~b & (aux0 | b)) ^ (a ^ a | b | aux0 | ~~a)) &
		(~(aux0 | aux0 | ~aux0 | ~~b) | (a | (b | b) & (aux0 | a)) &
		(a ^ a ^ (aux0 | a) ^ b)))) + 44891 * ~((aux0 ^ (~b | aux0 ^
		a) | aux0) & ~~~aux0 & a ^ (~(a | a | ~aux0) ^ (b ^ a ^ a ^
		aux0 | aux0 | aux0 | aux0) | a) | a) + 19818 * (b & (a |
		aux0 & ~a & (a | aux0))) + 45694 * (a & ~~aux0) + 12517 *
		~((aux0 ^ b & (aux0 | a) ^ (aux0 | a)) & (b ^ ~b ^ ~aux0) &
		b & (~a ^ ~(b & (~b | ~a))) & aux0 & (~(a ^ aux0) & (a |
		aux0) & b | ~(a ^ a) & ((aux0 | aux0) ^ b) | ~~(~b | a |
		a))) + 50380 * ~((a & ~b ^ ((a | b) & ~b | (a | b) & (a | a)
		| a | ~b ^ aux0 | b)) & a & ~((a ^ b ^ ~a) & (~b ^ a) | ~a &
		(b ^ a) & (b | aux0 | a))) + 52166 * (a & ((~b ^ a ^ (~b |
		~aux0)) & ~(a ^ b | a) & (~(a & b) ^ ~b & (a ^ aux0) ^ (~~b
		| aux0)) | aux0 | (b & b | aux0 & a | (b | b) ^ ~aux0) &
		((aux0 | b | a | b) ^ (aux0 ^ a | aux0 ^ a))) | b | b |
		~~~(a ^ aux0 ^ aux0 & aux0) & ((~b ^ aux0 & a) & (a & b |
		aux0) & (b | aux0) ^ (b | aux0 | a) ^ (a | b) ^ (a | b) ^
		aux0)) + 51015 * (aux0 | b | (~(a & (aux0 | b | a)) ^ (aux0
		^ (aux0 | b ^ b)) & (~b | ~aux0 | b | aux0)) & ((a | b & b |
		~a) ^ ((a ^ a | b ^ a) ^ b) & a & b & a & a & b)) + 23075 *
		(a ^ (a | aux0) ^ ((a | a | a) ^ a ^ a ^ aux0 ^ (~a & b | ~a
		& (b | a))) & ~(((a | aux0) ^ ~b) & ~(aux0 & b)) & (a ^ a) &
		(b | b | aux0) & (b ^ aux0 | a | aux0) & ~b | (~(~~b & aux0
		& aux0 & ((a | b) & (a | aux0) ^ a ^ a ^ (b | a))) ^ (((b |
		b) ^ b ^ b) & (aux0 | a | b | ~(b | b)) | aux0)) & ~((~(b ^
		b) ^ (aux0 | aux0 | b | aux0)) & ((b ^ aux0 | aux0) ^ (aux0
		^ b) & aux0 & a) & ~~(a ^ aux0) & b)) + 22101 * (~~((a |
		aux0 ^ b) & (b ^ a | aux0) | ~((b | aux0) & aux0) | a) | ((a
		| aux0 | a | a | aux0) ^ ~((a | b) ^ aux0 ^ b) ^ (b | aux0 |
		aux0) | (~b | b | b) ^ b & b & aux0 & b | a | (~a ^ a & b) &
		~b ^ ((b ^ a) & aux0 & b | aux0 & aux0 ^ (b | aux0))) ^
		((~(aux0 & a ^ aux0 ^ b) | ~~b) ^ aux0 | a)) + 19510 *
		(~aux0 ^ (~(aux0 & ((b | a) ^ b ^ a | b & a & (a ^ a)) & b)
		| ~b)) + 62838 * (aux0 ^ ~((~(b & a) | b | b) & (a & aux0 &
		aux0 | ~aux0 ^ ~aux0) & aux0) & ~~(~(b | a | aux0 ^ aux0) |
		aux0 ^ ~a ^ b & b)) + 64504 * (a & aux0 & ((~b & b & (a ^ b
		^ b) | ~b ^ a ^ a ^ ~(b & b)) & b | ~(~(b ^ a | ~a) ^ (aux0
		| b)) | ~((aux0 | aux0) & aux0 & aux0 & ~(aux0 & b) | a & a
		& (aux0 ^ aux0) | aux0 & ~aux0))) + 1852 * ~(a ^ b & ((b & b
		^ a) & a ^ a) ^ (~((b | aux0) ^ a ^ aux0) & (aux0 | aux0) ^
		~((aux0 ^ a) & (b | aux0) & (aux0 & a | a & a)) | aux0 | ~(a
		& aux0 | ~a) | (aux0 | b) & a & (b & aux0 ^ ~a))) + 38663 *
		(~~(~((a ^ aux0 | b & a) ^ ~b & b) & ~(~aux0 | aux0)) ^
		(aux0 ^ (~b ^ a & aux0 | aux0 ^ aux0 | aux0 ^ a) ^ b ^ ~(b |
		b & b) | aux0 | (b | b) & ~a & (a & a | aux0 & a) | aux0) ^
		~(~(b ^ (b | a) | ~~a) | ~~~(b ^ a))) + 60852 * (b | ~(b &
		((b & a | aux0 & aux0) ^ (aux0 ^ aux0) & ~b)) & (~~(aux0 & b
		^ a) | ~(aux0 & aux0 & a & aux0 & b)) ^ ~~(aux0 & b | b)) +
		12828 * (~~a | ~((b ^ ~~b) & ((aux0 | b) ^ ~a & (a ^ b)) &
		(~aux0 & ~a | ~~a | ~(aux0 & aux0) | (b | b) & (a ^ b)) |
		aux0 & ~(aux0 & (b ^ a) & (a | aux0)))) + 2999 * (((~a | a ^
		b | b & aux0) ^ aux0 | aux0) & aux0 & ((a | b) ^ ~a ^ ~a) &
		(~aux0 ^ ~a ^ (b ^ aux0 | ~a)) ^ aux0 ^ (~((~b ^ b) & a & a
		& b & (b & b & ~b | b)) | a) ^ ~(~(aux0 ^ b) | aux0 ^ (a |
		b)) & (~a & aux0 | ~b) & ~(a ^ b ^ ~b) & b & b & ~((a & b ^
		b & aux0) & (a & b ^ (aux0 | b)))) + 11698 * (((aux0 | b &
		(b ^ aux0) ^ (b | a) & a) & b | aux0) ^ ~a & ~(b ^ b ^ b ^ b
		| aux0 | b ^ b | ~a | aux0 ^ ~a) | ~(~(aux0 & a ^ b) & (a ^
		b ^ a & b | aux0)) ^ ((b ^ a) & ~a | a ^ a ^ a) & b ^ aux0 ^
		b ^ (~(aux0 & aux0) | aux0) ^ b) + 41415 * ~((~(aux0 | aux0)
		| aux0 | ~((a | a) & ~b) | (b ^ aux0 ^ (b | b)) & aux0 ^ a)
		^ b ^ a ^ (~a | b ^ a) ^ (aux0 ^ aux0 & (b | aux0)) & (~aux0
		| ~(a ^ b)) ^ b) + 42270 * (~(aux0 & a | ~((~a | a | a) ^
		b)) | ~(a | ~~a) ^ (a ^ aux0 ^ aux0 & a | ~(b ^ aux0) |
		~(aux0 ^ a)) | ~(a & aux0 & a & b & b | ~aux0 | ~a) | ~a |
		~~((b | a) & (aux0 ^ a) | a ^ (b | aux0)) | b) + 34253 *
		~~(aux0 & ((b & aux0 | aux0 & b) & (aux0 ^ a) & ~a ^ ((a |
		aux0) ^ b ^ aux0 | a)) & (~(b ^ aux0 | b ^ a) & (b & a | a ^
		aux0 | aux0 & a ^ aux0 ^ aux0) ^ ((aux0 | aux0 | a ^ aux0) &
		(aux0 & a ^ ~a) | b))) + 33183 * (~(~~(~~aux0 | aux0 ^ b ^ b
		& aux0) & ~~(b ^ a ^ aux0 ^ ~(b | aux0))) | (~(b & (aux0 & b
		| aux0 ^ aux0) ^ aux0 ^ (a | aux0) ^ ~~b) | ~aux0 & (a & a ^
		(a | b)) | b | a | a) & ~~a) + 46662 * (a & ((((a ^ a) &
		(aux0 ^ aux0) | a ^ ~aux0) ^ aux0) & ~b ^ b | b | ~(b ^ aux0
		^ aux0 & a) ^ ((b | b) ^ aux0) & (a | b | ~aux0) | aux0)) +
		47165 * ~~(aux0 ^ ((b | b | aux0) & a & ~(a & aux0 | ~aux0)
		| ~(~b | b ^ a) ^ aux0)) + 63027 * ((~~a & ~((aux0 & (aux0 |
		aux0) ^ a) & ~(b | aux0 | aux0)) | (aux0 | (aux0 & a ^ ~a |
		aux0 & aux0 & b) & (a | b | aux0 ^ a | a | a | a)) & (aux0 &
		b & aux0 ^ b ^ ~~(b & a)) & (a & b ^ b & aux0) & a & (aux0 &
		~aux0 | b ^ b | ~b)) ^ ((a | (b | b) ^ aux0 ^ (~a | ~a)) &
		aux0 & b & (~b | a & a) | ~(aux0 ^ a | b ^ a | b | ~aux0 ^
		(b | a) ^ a ^ b)) & ((aux0 & a | aux0 | b | a & a | a |
		aux0) ^ (~b ^ (a | a) | aux0) ^ aux0 ^ b | ~a & (aux0 ^ (a ^
		b ^ aux0 | ~b & b)))) + 15334 * ~(b | ~a | ((a & a | b |
		~(aux0 ^ b)) ^ a ^ ~(a ^ aux0)) & (~(a ^ aux0 ^ b) ^ ~b)) +
		48333 * ~aux0 + 27494 * ~~(b & a & ((~b | aux0) ^ (b | b | a
		& a)) & aux0 & b) + 46421 * (b & ((~((a | b) & ~a) & (~aux0
		^ a) | ~a | aux0 & (a ^ a) | ~a) ^ ~aux0 & (~a & ~a & (a | a
		| b) | aux0) ^ a)) + 15722 * ~~aux0 + 458 * (((~((a | a) ^
		aux0 ^ a ^ (b & b | ~a)) | ~(~~b ^ ~~a)) ^ (aux0 ^ (a | b) &
		~aux0) & b & (a | b & b ^ b ^ a | aux0 ^ a ^ aux0 & a | (a |
		aux0) ^ a) | ~(((b | a) & ~b ^ (a ^ b | aux0 | aux0)) &
		(aux0 ^ (b | b) & ~aux0)) & b) & (~(~a & b) ^ ~~(a | b) ^
		aux0 & ~aux0 ^ a ^ aux0 & (b ^ aux0) & ~b & a & b) & (a &
		(~(aux0 | a | b) | b ^ (b | aux0) ^ b ^ aux0 & b) | aux0)) +
		58405 * (((b ^ ~(aux0 | b) & (b ^ a ^ b & a) ^ ~(~b ^ (aux0
		| b)) | (~(a | b | b & aux0) ^ a & b ^ ~a ^ aux0) & a) ^ b)
		& ((b | b & (a | a) & aux0 ^ (a & aux0 ^ ~a) & (b ^ a ^ a))
		^ (b & b ^ b & b | aux0 & aux0 ^ aux0 & b | (a ^ a) & (b |
		a) ^ (a | b) & a & b | b)) & (~~a & a ^ a)) + 39253 * (((~b
		| ~~aux0 & ~b | ~(aux0 & aux0 & (a ^ aux0)) | b & aux0 & (a
		| b) & ~(b & a) ^ (aux0 | b ^ b | (b ^ aux0) & (aux0 ^
		aux0))) ^ (b | (a ^ ~a) & ~aux0 & ~a ^ (~a | aux0 ^ a) & (a
		^ (aux0 | b))) & a) & (~(((b | b) ^ a) & a & ~b & ~(b |
		aux0) | b ^ (a & b & (aux0 | a) | aux0 & aux0 & (aux0 | b)))
		^ (~(~b & aux0 & aux0 ^ b) & (~(~aux0 ^ b) ^ ~(aux0 ^ aux0)
		& aux0 & (aux0 | a)) | aux0))) + 60424 * ((~((a | a | (a |
		a) & (b ^ a)) ^ aux0) & ~((aux0 ^ aux0 ^ ~b) & ~(aux0 & b) ^
		aux0) ^ a & (a & a ^ a ^ a) & b & (b ^ aux0 ^ ~~(a | b)) &
		aux0) & ~(a ^ ~((aux0 ^ b) & aux0) ^ a ^ aux0 & a & b ^ ~(a
		^ aux0)) & (b | (~b & ~b ^ (b | b | b & b) ^ aux0) & ((~b |
		~a) ^ ~b ^ a) & (~~aux0 | aux0 ^ a & b))) + 22361 *
		(~(~(~(aux0 ^ a) ^ (~b | a)) | (a ^ b) & aux0 | ~b | ~a) &
		~(~~((aux0 | aux0) & (aux0 ^ a)) & (~~aux0 & aux0 | a)) ^
		~~~aux0 & a ^ aux0) + 49409 * (~(~~~a | ((b | aux0) ^ aux0)
		& ~~a & (a | a) & (a ^ a) & ~b ^ aux0) | a ^ ~~aux0 ^ (aux0
		| (b | a | b) & (aux0 ^ a) & b ^ ~(aux0 | aux0 | aux0 &
		aux0) | b)) + 26668 * (((aux0 | ~(a | a & b ^ aux0 ^ a) ^ (b
		^ ~(b & aux0) | b)) ^ ((aux0 ^ b) & ((b | aux0) ^ (b | a) ^
		b) | aux0 ^ b ^ ~b ^ (b & aux0 | aux0 | a) | ~(~(b | aux0) |
		aux0 ^ aux0 & a) & (~a & a | a & a | a) & ~(~a ^ a ^ a))) &
		((aux0 ^ aux0 | b ^ a ^ aux0 ^ a | ~(aux0 | b)) ^ (b | a) |
		a & b) & ~(a & a & b & (a ^ b) ^ (aux0 ^ b ^ aux0 & a | aux0
		& ~b)) & (aux0 | ~(a ^ a & a) & (~b ^ a ^ aux0 ^ ~aux0))) +
		52210 * ~~b + 49275 * (b | ~((~b | a & ~a & (b | b) ^
		(~~aux0 | aux0 & b)) ^ (a | a ^ b ^ aux0 | b | b ^ b | (b |
		~aux0) ^ a | ~((a ^ b) & a)))) + 26120 * (b | a | ~(a | ~a ^
		~a | ~(b & a) & (a & b | a ^ aux0) | ~(~(b | aux0) | a)) |
		~(~(b ^ a) ^ aux0 ^ b & ~a & ~a ^ (b ^ ~(a ^ aux0) | ~aux0 &
		b & a)) & b) + 58984 * ~(~(~a ^ (b | b | b & aux0 | a ^ aux0
		^ aux0 & aux0) ^ b & aux0 & (aux0 | b) ^ aux0 & b ^ ~a) |
		~(~(~(aux0 ^ a) | a ^ a ^ b & b) | a)) + 4255 * (aux0 ^ b &
		~(aux0 & a & (a ^ b ^ aux0 & a)) & ~~aux0) + 60767 * (~(a &
		(aux0 | a) & ~aux0 | ~~b ^ a) & ((~(aux0 | b) | ~b | b & a)
		^ (a | a) & (aux0 | b) ^ b ^ ~(aux0 & a ^ b) ^ aux0 & a & b
		^ (b ^ b) & a) & a | ~~(~(aux0 & a) | ~(aux0 ^ a) | (a ^ a)
		& b & a ^ ~b & (a | a)) ^ ~aux0) + 56980 * (~(~(b ^ (aux0 |
		aux0 | b)) & ((b ^ aux0) & ~b & (a | aux0 & aux0) | (~aux0 |
		a) & (b & b ^ a)) | b | ~b & (a & aux0 ^ aux0 & a) | ((b ^
		aux0) & a ^ ~~b) & a) ^ a) + 20900 * ~((~(~a ^ b ^ aux0 ^ a
		^ aux0 ^ ~b) | b & ~b ^ (b | b | a) | (a | aux0) ^ aux0 ^ b
		^ ~(a ^ b)) & a ^ b) + 21414 * ~(~(a & ((aux0 ^ a) & (b |
		aux0) | a | aux0 | aux0 ^ a) & ~(a & ~aux0 | b)) & b & ~a) +
		24735 * ((~(b & b ^ a ^ a) | a | ~aux0 & ~~a | aux0 | ~a |
		~((aux0 ^ a | a) & a ^ (aux0 ^ ~a) & a) | (~~(aux0 & aux0) ^
		~(aux0 & a & (a | aux0))) & ((aux0 ^ aux0 ^ b & a) & ~~aux0
		| ~(a & aux0) & aux0)) & (b ^ (~a | ~~(aux0 | b)) | (aux0 |
		aux0) ^ ~a | (b | a) & aux0 | ~(~aux0 | ~aux0) | b) & (a ^
		~(((a | aux0) ^ ~aux0) & aux0 | b))) + 63558 * (~~(~(b ^
		(aux0 | b)) & ~~aux0) ^ aux0 & (a ^ ~a) & ~(aux0 ^ a) & (a ^
		~b ^ ((aux0 & a ^ a ^ a) & b | ~aux0 ^ ~b ^ aux0 ^ aux0 ^ a
		& a)) | (b ^ a) & b) + 65506 * (aux0 & (a | ~((aux0 | a) &
		aux0 | ~(a ^ b)) & (aux0 ^ ~b ^ aux0 & b ^ ((a | aux0) ^ (a
		| b)) & ((a | b) ^ ~aux0))) ^ ~(~(b & (a & (b | a) | a)) ^
		((a ^ aux0 | aux0 ^ b) ^ (~aux0 | aux0 ^ b)) & (~aux0 | b ^
		b) & (aux0 & b | a & b) ^ (~b | aux0 | a ^ b ^ ~aux0 ^
		aux0))) + 63089 * ((~~a | a ^ (b | aux0 ^ ~(b ^ aux0)) &
		(~(aux0 | aux0) & (a ^ aux0) & b & a ^ ~a)) & (~(~~(a ^ b ^
		aux0) & ((b | b) & b | (a | aux0) ^ a ^ a) & ~(~aux0 ^ (aux0
		| aux0))) | b)) + 48945 * ((b | (~(~(aux0 & aux0) | b &
		aux0) | a ^ (b & b | a | b) & (b ^ b) & (a ^ aux0)) &
		(~(aux0 & aux0) & aux0 & aux0 & a & a ^ a ^ a ^ b & b)) ^
		~~(aux0 & ~((a | b) ^ (b | aux0)) & b)) + 32305 * ~(((aux0 ^
		a | ~b | b & aux0 & a) & (b & b ^ b | (a | a) & aux0) | ~(a
		& (a ^ b | b & aux0))) ^ ~~((aux0 & aux0 ^ aux0 ^ aux0) & a)
		| ~~a) + 44401 * ~~~~a + 6304 * ((~(b ^ ~((~aux0 | aux0 |
		aux0) ^ (~b | aux0 | b))) | aux0) ^ a) + 25141 * (~b ^ ((b |
		aux0 | b ^ a | a | aux0 | a ^ a) & (b ^ aux0 ^ b & aux0 ^ (a
		| b) & (b | aux0)) ^ ~((b | aux0) ^ aux0 ^ a) ^ (b | a | a |
		aux0) ^ (b | b ^ (a | aux0) & aux0 & aux0 & b)) & (~(b ^ a)
		^ (aux0 & aux0 | aux0 & aux0) | aux0 ^ (b | aux0 | aux0 ^
		aux0) | ~~a ^ ~b | aux0 | b ^ b & a) & (b ^ ~((a | b) ^
		aux0) ^ (b & a ^ ~b | ~(aux0 & aux0 ^ a ^ b)))) + 23586 *
		~(b ^ aux0) + 64303 * (aux0 ^ (~~((~b | a & b) ^ aux0 & b &
		(a | aux0) ^ ~(aux0 & a) & a & ~a) | ~~~((aux0 | b) & a) &
		~(~(a ^ b) ^ b & b & ~a) & (b & a ^ ~(b & aux0)) & ((a ^ b |
		~aux0) ^ ~(a ^ b)))) + 26941 * (~~(~b ^ (aux0 | b) | aux0 &
		b ^ (a | b) | b | aux0) ^ ~((~b & (b ^ aux0) | ~(b | aux0) |
		~(~aux0 & a)) ^ a | aux0 & ~(b & a & a | ~(a | a)))) + 61539
		* ((aux0 | b | ~aux0 | ~aux0 | a) & (b ^ a) & ((b | a) & ~a
		^ ~(aux0 & b)) & ~~aux0 & (b & aux0 | b | a) ^ (a ^ (b ^ a |
		aux0 | b) ^ (aux0 | aux0) & (a ^ a) & ~a & b & aux0 | ~aux0)
		| ~((a | aux0 | b) & a & aux0 ^ (a ^ ~a) & (a | ~b | aux0 &
		b)) | (~((b | a) & (aux0 | b)) | aux0 | b | aux0 | a & b) &
		b & aux0 & a & b & (~(~b | b ^ b) | (aux0 | aux0 ^ a) ^ aux0
		& (b | b)) & ~~(b & ~b) & (~~a ^ (a ^ a ^ aux0) & (b & aux0
		^ ~aux0) ^ b)) + 19978 * ~(~((aux0 & b ^ (aux0 | aux0) | a ^
		~aux0) ^ (aux0 ^ a ^ ~a | ~aux0 ^ b)) & aux0 & aux0) + 2395
		* (~aux0 & (~b & b | (~((b ^ aux0) & aux0 & a | (aux0 |
		aux0) & (a | a)) | ~((b ^ aux0) & (aux0 | a) | b ^ b | b &
		b)) & b)) + 14589 * (aux0 | ~b | a) + 21385 * ~(aux0 & (~b &
		~(a ^ (aux0 | aux0)) ^ b) ^ b ^ (a | a | ~(aux0 ^ b) ^ ~aux0
		| aux0)) + 36397 * (~((~aux0 | a & b | ~(b & b)) ^ aux0) & b
		& ~~((a ^ aux0 ^ b) & ~(a ^ aux0)) & ~a | ~((b & a | b ^
		aux0 | ~b ^ a ^ aux0) & (~(b | a) | ~(aux0 | b))) & aux0 ^
		~(b & (a ^ ~aux0 & aux0 ^ ~(a ^ b) ^ (aux0 ^ a) & b))) +
		16628 * ((b ^ (~(b ^ aux0) ^ a) & (a ^ aux0) & (b | b) & (b
		^ a & aux0) ^ aux0 ^ (a ^ aux0) & (~(aux0 & b) | a ^ (aux0 |
		aux0)) & (~(aux0 & b) | aux0) ^ (~((a | b) & (~aux0 ^ (aux0
		| aux0))) | ~(a & a ^ b & aux0) | a | b ^ a)) & (~((aux0 ^
		(b | b)) & b) | ((~aux0 | a | aux0) ^ ~aux0) & (~aux0 | b &
		~b)) & ~a & a) + 24148 * (~(b & (b | b | ~b) & (a & aux0 |
		a) & b & b & (aux0 ^ b) & (a ^ aux0) & b & a) & b |
		~((~~aux0 | ~(b & a) & a & (a ^ b) & ((aux0 | b) & a & a ^
		a)) & (~b & a & b & ~(aux0 ^ aux0) | b & aux0 & b & ~aux0) &
		a)) + 57481 * (~~aux0 & ((aux0 | b & b & b ^ ~aux0 & b & b |
		a) ^ ((b ^ a | b ^ a) & (aux0 ^ b ^ b) ^ aux0 & ~(aux0 |
		aux0) | b & (a & a ^ b & a ^ aux0))) & a & (aux0 | aux0) &
		b) + 41201 * (((aux0 | b ^ aux0 & b | ~aux0 | aux0) & ~~a |
		b | ~b & b & b & (b ^ aux0) & b ^ ~(~b ^ (aux0 | a))) &
		((~(aux0 | b & aux0) ^ aux0) & ((~~a | b & a ^ ~b) ^ ~((aux0
		| a) ^ a ^ a)) | ~~~aux0 & (~a | ~aux0)) | ~((a & a ^ b ^ b
		& aux0 & (a ^ aux0)) & ~((b ^ b) & ~aux0 ^ (aux0 | b | a)) |
		((a | b | b & b) & a ^ ((a | a) ^ (b | a)) & (b ^ ~b)) & ~b
		& (~(b ^ a) | ~aux0 | ~a))) + 51735 * ~(~aux0 & aux0 ^ a &
		(aux0 | b) & b & (b | aux0 ^ aux0) & (~b & (a ^ a) | ~(aux0
		^ a) | aux0 | aux0 | a & aux0 | ~(b ^ b)) ^ ~(b ^ a)) +
		19899 * (b & b) + 28358 * (b | ~((aux0 & a | aux0 | b) ^ (b
		& aux0 | aux0 & a)) ^ a ^ aux0 ^ (~~a ^ ~(a & a) | ~(a |
		aux0) | ~b | aux0 & a | b) | ~~((aux0 | a) ^ a & aux0) ^
		~((aux0 & aux0 ^ a & a) & a) | a) + 35941 * (~(aux0 | ~((~b
		^ (aux0 | b)) & ~~aux0 & (~(a | a) ^ a ^ aux0 ^ b & b))) &
		~((aux0 | aux0 | a & a) & (~aux0 ^ a | a & a ^ ~b ^ (~b | a
		& a) | ~a & aux0 & ~~a & (aux0 | a | b)))) + 59029 * (b ^
		~((aux0 | aux0) ^ b | b ^ aux0 ^ (b | aux0)) ^ b & (aux0 & b
		| aux0 ^ b) ^ (a ^ b ^ aux0 & aux0) & (~a ^ b & aux0) | a ^
		~a ^ ~(b | ~((b | a) ^ aux0 ^ b)) | (~(~aux0 ^ b & b) ^ aux0
		^ (aux0 ^ b | a | ~(b & b)) ^ aux0 ^ ~(~((aux0 | aux0) ^
		aux0 ^ a) & aux0)) & aux0 & (b & (b ^ (aux0 | b | b)) ^ (~a
		& (b | b) ^ b | ~(a & ~b)))) + 32628 * (aux0 & ((~(a & aux0
		| ~a | (aux0 | a) ^ ~aux0 | (aux0 | a) & (b ^ a ^ (a | b)))
		| ~a) ^ b & b ^ aux0 ^ ~aux0)) + 53269 * ~~(a ^ ((b ^ b |
		aux0 | a) ^ a ^ aux0 ^ b | ~aux0) | aux0 & ~b | ~(a ^ aux0))
		+ 46952 * (~(~b ^ aux0 & aux0 | (a | aux0) & (a | a) | ~(b &
		aux0 ^ ~a)) | (a & ~a ^ ~(aux0 & b)) & (aux0 ^ ~a) & ~(aux0
		^ b) | ~(b ^ (a | a) & b & a & (~aux0 ^ ~b) & (aux0 & a & b
		| ~~a)) | ~~~((a | a) ^ a & b ^ a & a | (~aux0 | a & a) &
		aux0)) + 13428 * (a | aux0 ^ (aux0 | aux0 & a & (b | a) ^
		~(aux0 & a) ^ (~aux0 | aux0) & ((a | aux0) ^ ~aux0)) & (a ^
		~b ^ ~~(b & b) ^ ~aux0 & b ^ ~(aux0 | a | b))) + 38869 * ((b
		^ ~aux0 & ~b ^ ~(a | aux0)) & (a ^ b) & b & (~((a & aux0 ^
		~b ^ a & b & a) & b) ^ b ^ (b ^ ~~b | aux0))) + 7759 * ((b |
		(~~(aux0 ^ a) & (aux0 | aux0) & ~b & ((aux0 | b) ^ b ^ aux0)
		^ aux0) & a) & a & (b | a | (aux0 ^ a & aux0 & a & b) & ~a))
		+ 57081 * (~~((~(a ^ aux0) | b | aux0 | aux0) & (a | ~(a |
		aux0))) & (~b | aux0 & a & aux0 & ~aux0 & (a & ~aux0 ^ ~b &
		(aux0 | b)) ^ (aux0 | a & aux0 ^ b ^ a | (aux0 | aux0) &
		~b)) | ~b & (b | b)) + 5110 * (((~aux0 ^ ~b | ~(b ^ aux0) |
		b | a) & ~(b ^ aux0) & a & (b ^ a) & (a ^ ~(aux0 & b)) | ~~a
		& ~a) & (~(~(~b | a & b) & b) ^ (aux0 & b & (aux0 | aux0) ^
		(a | b) & a & a ^ b ^ a | ~(~aux0 ^ (a | a)) ^ a)) ^ b ^
		(~((~aux0 ^ ~(b & a)) & ~b) | ~(~(b ^ a) & (a ^ a | b)) | (a
		| ~aux0 | a) & (~(a ^ b) ^ ~(aux0 | aux0)))) + 8812 * ((b ^
		~(b ^ a) ^ ~b ^ (~aux0 ^ (aux0 | a) | ~(a ^ aux0)) | (aux0 ^
		(b | aux0) | ~~b) & (aux0 ^ aux0 ^ aux0 & aux0) & ~(b | a) |
		b) ^ (b | aux0 | ~~(aux0 & b) & (aux0 ^ ~(aux0 | a))) ^ b) +
		8922 * (aux0 | ~(~b & ~(aux0 & b) & b) ^ (b | ~(aux0 | aux0
		& b | aux0)) | ((aux0 ^ a) & ~b ^ (a | aux0 | ~aux0) ^ b) &
		(~b & a & aux0 ^ ~aux0 ^ (aux0 | b) ^ aux0) ^ (b | ~(~a ^ a
		^ b) | ~(aux0 ^ aux0))) + 23363 * (~(~((b | a) ^ b) & ~(aux0
		& aux0) & ((aux0 | a) ^ ~a)) & (a ^ a ^ b | ~(~(b ^ b) ^ b &
		a ^ b ^ b)) & ((~(b ^ a | aux0 & a) | a) ^ (~(b ^ a) | aux0
		| a | aux0) & ~~(aux0 ^ aux0)) & aux0 ^ ~~(~(b & a | aux0) ^
		aux0 ^ aux0 | (~a ^ (aux0 | a)) & (~a | aux0 & a) & (a &
		aux0 ^ b & a) & a)) + 48285 * (~(~(a | aux0 | b) ^ (b | aux0
		| ~aux0) ^ b ^ b ^ b | a) & (aux0 ^ a | (b | ~~aux0) & (aux0
		| ~(aux0 | a)) ^ (aux0 | a | b & b | ~b) ^ b) | ~b) + 19382
		* (b ^ b & aux0 ^ ~(a ^ aux0) ^ (b ^ a) & a ^ ~(b | a | a |
		~aux0 | b & a & ~b) ^ (~(b ^ b ^ ~aux0) & (a & ~b ^ ~~aux0)
		& (~(a ^ aux0) & (a | aux0 | a ^ a) | a) | ~~(a & b) & (aux0
		^ b) & (((a | b) ^ aux0) & a ^ (aux0 & aux0 | aux0 & aux0 |
		aux0))) | a) + 52431 * (b | a & (~(a & a & ~aux0) & (~(a |
		aux0) | ~aux0) ^ aux0 ^ ~~~(aux0 & aux0 ^ aux0 ^ a))) +
		54490 * (~~(~(a | a) | aux0 & aux0 | aux0 | aux0 & (a | b) |
		aux0 ^ a & a) | ~(b | ~(~a ^ ~b) | (aux0 | a) & aux0 & aux0
		& (aux0 ^ aux0 | ~a)) | (b | (b & a | a & b) & ~(aux0 ^ a) ^
		~(b ^ aux0) ^ (aux0 ^ a) & (b ^ a)) & ((~~a | a) & a |
		~(aux0 ^ aux0) ^ b | (~a | aux0 & a) & (aux0 ^ aux0 & aux0))
		^ (~~((aux0 | b) & a & aux0) & aux0 | ~a & (~b & (a | aux0)
		| b ^ aux0 ^ a) ^ a)) + 25557 * (~(a ^ (~a | ~a | aux0 ^
		aux0 | b) | ~b | ~(b & aux0 & b & b ^ ~(b & b) ^ b)) | b) +
		4842 * (~((a | (~(b | aux0) | aux0 & b | ~a) ^ (~b & b & a |
		b)) & ~aux0) ^ ((aux0 & ~((b ^ aux0) & (a | a)) | (aux0 ^ a)
		& (b | aux0) ^ aux0 | aux0) & ~(~((aux0 | aux0) ^ a) ^ (b &
		a | a & a | ~aux0 | a ^ aux0)) | aux0)) + 32867 * ((~(~(a |
		b) & (a & b ^ a ^ a)) | a ^ ((aux0 | a) ^ (a | aux0) | a | a
		| ~b) | ~~(a & a) & aux0 & (aux0 ^ b | aux0 & aux0) ^ (a & a
		& (aux0 | b) & a | (b | a) & (aux0 | b) & (a & b | ~b)) | b)
		^ ~(a ^ (b ^ a | (b | b) & ~a) & (b ^ a & a | aux0) ^ b)) +
		40778 * ~(~b & ~((b & (aux0 | a) ^ (b & aux0 | a & aux0)) &
		aux0) ^ aux0) + 62982 * ~(~a ^ a ^ ~(~(~b | a ^ aux0) ^ ~a)
		^ (~b | ~(~(a | aux0) & aux0) & ~(b | b) & a & b)) + 10080 *
		((((aux0 | aux0 | aux0) ^ (a ^ a) & (aux0 ^ a) | ~aux0 ^ ~b)
		^ ((b | b ^ aux0) & aux0 & aux0 | a) ^ (a ^ (aux0 | b) |
		~(~~b & ((aux0 | b) ^ aux0)))) & (~((a | a) & a | aux0) |
		(aux0 | aux0 & a | a & a) ^ ~~(aux0 | a) | b) & ((b ^ b |
		aux0 & aux0 | ((b | b & aux0) ^ a) & (~b & ~a ^ (b | a ^
		a))) ^ (~(aux0 | b) & (b | aux0 | a & a) & ~(~aux0 ^ a & a)
		& (~~(b & b) ^ b) | (~aux0 | a | b) ^ (b | a) & b ^ (b & a &
		a | b ^ b | a & a) ^ (a & a | b | b) & (a ^ a | b | a) &
		(aux0 ^ ~a | ~(a ^ aux0))))) + 21444 * ((aux0 | ~((aux0 ^ a
		| b ^ b | aux0) ^ (a | a) ^ a & aux0 ^ b & (a ^ aux0))) &
		((a | b) & a & a ^ (aux0 | a | a) ^ ~~(b & a) | (aux0 & a ^
		(aux0 | a) | b) ^ a | ~~~aux0) | b | ~(a & ~~(~a | aux0))) +
		22057 * ~~(((~(~b | a | b) | a) ^ (b & a ^ a | ~(b & a)) ^
		(b ^ b ^ a & a) & (aux0 | b | b & b)) & (aux0 ^ (a & aux0 |
		aux0 ^ aux0) ^ ~aux0 ^ a ^ (a | aux0 | aux0 | aux0) ^ ~aux0
		^ ~a)) + 21922 * (~(((~~b | ~~a) ^ a | ~(b & a) ^ aux0 & b &
		b & ~(aux0 | aux0)) & ~(b ^ a & (b ^ b) & (a | b))) &
		~(~(~aux0 ^ ~a) & b & b | ~((a & a & (a | b) | (aux0 ^ b) &
		aux0 & a) ^ (a ^ a | b ^ b) ^ (b | a) ^ b ^ aux0))) + 60387
		* ((~~(aux0 & aux0 & (b ^ a) | a) ^ a) & ((b ^ a ^ a ^ a |
		aux0 ^ a | aux0) ^ ~~~b ^ a | (a | a) & (aux0 & b ^ aux0) |
		~~(a & a) | (b & (b | aux0) | b) & (aux0 ^ aux0) & aux0 &
		~~a) ^ (~(~b | ~(a ^ aux0)) | a ^ a | aux0 | b & b | b ^ b |
		~((b | a) & (b | aux0) & (aux0 ^ b ^ a)) ^ ~(~(aux0 | a) & b
		& (b | b))) & (~(aux0 & aux0) | ~(aux0 & b ^ aux0) ^ (a | a
		& a | ~(b & aux0)) | a)) + 5860 * (aux0 ^ ~a ^ (b | a &
		~(aux0 & b & aux0 & aux0 | b))) + 56789 * ~(a & (~~aux0 ^ b
		& ~a) & b & aux0) + 16018 * (~((a ^ aux0 ^ ~aux0 ^ ~(b |
		aux0) | ~(a ^ aux0 ^ aux0)) & ((b & a | a & aux0) & (b & a |
		b ^ b) | a) ^ (a | ~b)) | aux0 ^ ~(b | aux0 | aux0)) + 61553
		* (((aux0 & aux0 ^ b ^ a | ~(aux0 & a) | ~(~b & aux0 & b)) &
		(b | ~a ^ b) | ~(~(a & b) | aux0 ^ b ^ a ^ a) & (aux0 |
		~~aux0 ^ b)) & (aux0 ^ b) | a ^ ~(b ^ a ^ a ^ b ^ aux0 &
		aux0 & a | ~~(a ^ b)) | ~((~(a | aux0) | b) & ~(aux0 | a |
		b)) ^ (~a ^ (~aux0 | b | b) ^ (a & aux0 & ~b | a & ~b)) & (a
		^ a ^ (b | aux0) | aux0 | a)) + 47781 * (a & (~~(b | aux0 |
		a & a) ^ a & (~b ^ aux0 ^ b) ^ (aux0 & aux0 & aux0 | aux0 &
		~b) | ~~aux0 & ~(a ^ b) & (aux0 ^ b ^ b) ^ ~((a ^ a | b |
		aux0) ^ (aux0 ^ b | ~b))) & (~b | b ^ ~b ^ (~b | (b | a) &
		(aux0 | b) ^ (b | aux0 & a)))) + 10374 * (((aux0 & ~b | aux0
		| (~b ^ (b | aux0)) & (b ^ aux0)) ^ aux0 ^ a | (~(a ^ aux0)
		^ (aux0 ^ b | a)) & (b | (a | b) & (b | a)) ^ (b ^ aux0) &
		(a | a) | (~~a ^ ~b & ~b | b) ^ a | b) & a & ~(~(aux0 & (b |
		aux0) | (a | b) ^ ~a) & (b & (aux0 ^ aux0) & (aux0 | aux0) |
		(aux0 | a | aux0 | a) & ~aux0 & (aux0 ^ a)))) + 32612 * (a ^
		~~~(~(~a & aux0) | ~(~a | b & a))) + 47138 * ~((~(~aux0 |
		~(a ^ aux0) & (b & a | b ^ a)) ^ ((~a ^ ~a ^ (aux0 | a) & (b
		| aux0)) & b | ~b)) & (b ^ (a | ~b) ^ (~~(b & b) | a & aux0
		^ ~b | aux0 | aux0 ^ aux0) | ~(a ^ a & a ^ ~a ^ (a ^ a ^ b)
		& aux0))) + 21934 * (~(aux0 & (~(a & a) | a | ~aux0) ^
		~(aux0 & b ^ b & b ^ (b | a | ~b))) & a | aux0) + 21854 *
		~((a & aux0 ^ aux0 & b | aux0 & a | aux0 ^ aux0) & (~b | a)
		^ a ^ (a ^ b | aux0 ^ a) ^ aux0 ^ b ^ b | (a & ~a ^ aux0) &
		~(aux0 ^ b | aux0 | aux0) & (aux0 ^ aux0 & b) | ~(a &
		((~aux0 | ~a) ^ b)) ^ aux0 ^ aux0 ^ ~b ^ ~b ^ (aux0 | aux0 ^
		b) & (a ^ b ^ (a | aux0))) + 16223 * (~~~(a & a & (aux0 | b)
		& (aux0 ^ ~b) & (b & a & b & aux0 | b)) ^ (~~(a ^ aux0 & b |
		a & b & aux0) ^ (aux0 & a ^ a | ~a ^ ~a) ^ a ^ (b | aux0) ^
		~(b ^ a) ^ a ^ a | a)) + 38546 * ~(aux0 ^ (a | ~((b & b ^
		~a) & b) ^ b)) + 39971 * (~((~b ^ ~(aux0 & a) | b | b) & b)
		^ (~aux0 & b | a) ^ a & aux0 & ~(b | aux0 | b | b) ^ a | ~a
		^ ~(~(b & aux0 & (b | b)) | b) & b & (a & b & (b & b |
		~aux0) ^ (~b ^ a & b) & b & b & ~aux0)) + 39875 * ((a |
		~(~~b ^ b | ~(a ^ b ^ b ^ b)) ^ aux0 & (~aux0 | (aux0 ^ a |
		a | a) & (~aux0 ^ aux0 ^ aux0))) ^ ~(b | a | b | aux0 | b |
		~(b & aux0 & ~a) & ((a ^ a | b) ^ a) | ~(b | a | b | aux0) |
		~(b & a | ~b) ^ (b ^ aux0) & a & b ^ aux0 & b & ~b)) + 61098
		* ((aux0 | b & (~(~aux0 & (aux0 | aux0)) ^ ((aux0 | a) ^
		~aux0) & (b | aux0 & aux0)) & aux0) & (~a | ~a | a | ~(a &
		aux0) | ~(b ^ a | ~aux0) | a & (~a ^ aux0 & a | a))) + 47416
		* ~(((b & b ^ ~~aux0) & ((a ^ a | b) ^ ~(aux0 | b)) | (aux0
		| aux0 | ~aux0) & (b & b ^ ~aux0) ^ b) & (~~(b & a) ^ ~(aux0
		& b) & (aux0 ^ a | aux0 | aux0) | (b | (aux0 ^ b) & ~aux0) &
		(~a & a & aux0 ^ b & b ^ b & b)) | ~aux0 & ((aux0 | b) & a &
		(b ^ b ^ (aux0 | b)) & (b ^ b | aux0 & b | b) | aux0)) +
		15865 * ~((~((a ^ b ^ (b | aux0)) & b) & a & (a & aux0 ^
		aux0 ^ a) & (a ^ a & (aux0 | b)) | a | aux0 | b | ~a & (aux0
		| aux0 & a | aux0 | a)) & aux0) + 51617 * ~((a ^ ~aux0 &
		~aux0) & (aux0 ^ a ^ aux0 & a ^ aux0) & (~(b ^ aux0) ^ a & a
		^ ~aux0 ^ ~a & (b ^ b) ^ a) & ((aux0 | ~(a | a) ^ (b ^ a) &
		aux0) ^ aux0 & b & (b & aux0 | b) & ((aux0 & a | a) ^ ~a)) &
		((aux0 & aux0 ^ (aux0 | b) | a ^ a | ~aux0 | (a | a) & ~b &
		~~aux0) ^ ~(aux0 & a ^ aux0) & (~a ^ aux0 ^ b ^ b & b ^ aux0
		^ aux0) | aux0)) + 37385 * ~~~((b ^ a) & (a | a | ~a | ~b |
		~a) & (aux0 | aux0 & a ^ b & b)) + 14713 * (b ^ b) + 30482 *
		(a | ~~(b ^ (aux0 | ~a ^ a ^ (b | aux0)))) + 34866 * (~(b ^
		b & b & aux0) | b & aux0) + 30303 * ~(a & aux0 ^ (b |
		~((aux0 | b) & a & a) | a) ^ a ^ ((aux0 ^ ~aux0) & aux0 & a
		| aux0 ^ a | aux0 & b | ~aux0 ^ (aux0 | a)) ^ ~a) + 65034 *
		~(a | ~b ^ (a & aux0 | b | ~aux0 & b & ((b | b) ^ a & aux0))
		^ b) + 520 * (~a | a) + 61393 * (~(a | a) & aux0 & (b | b) &
		(aux0 ^ ~(a & a)) & aux0 ^ b & (b & b & a | ~(aux0 & a)) &
		aux0 | ~~(~a & a & a ^ (b ^ a) & (b | a) ^ ~(aux0 | aux0) &
		(b ^ b) & a) | ~(~((aux0 ^ aux0) & b & (b | b) & (a | aux0)
		^ ((b | a) ^ (b | b)) & b) & ~(aux0 ^ aux0))) + 28239 * ~~a
		+ 61213 * (a & ~(~((~a ^ (aux0 | b)) & b) & (~aux0 | b | ~(a
		& b))) | a) + 61990 * (b & aux0 | aux0) + 50297 * (a | a |
		((~b & ~a | b) ^ (a | aux0) ^ b & b ^ aux0 ^ a) & ((a | a) &
		~~b ^ ~b & (b | a) & (a | b | b)) & ~b | a & (~~(a ^ b ^ (a
		| b)) | ~(a ^ b) & (a & aux0 ^ ~b) | b | ~(a & b) | b)) +
		50766 * ((a | (a & (aux0 ^ aux0 & a) | (b ^ b ^ (a | a)) & a
		& aux0 & b & aux0) ^ b) & ~(b & (~a | aux0 | b | ~(b | b)) &
		~a & (aux0 | ~b)) | b | ~(((a | b) ^ (aux0 | aux0)) & aux0)
		& (b ^ ((a ^ aux0) & (b ^ b) | ~aux0)) | b ^ aux0 & aux0 &
		aux0 & a ^ aux0 & a ^ b | ~((a ^ aux0 | aux0 & b) & aux0 & a
		& ~aux0)) + 43962 * (aux0 | (b ^ ((aux0 ^ aux0) & (a | a) ^
		~(aux0 ^ a)) & ~(aux0 & ~aux0) | aux0) ^ ((a | b & aux0 ^
		~b) ^ ~~(b ^ b) | (aux0 | b | aux0 ^ aux0) ^ b & b ^ (aux0 &
		a ^ (aux0 | a) | b | a | a & a)) & aux0) + 36332 *
		~((((~aux0 ^ (aux0 | a) | (a ^ b) & aux0) ^ a) & ~(b | a &
		a) & aux0 | (~(aux0 ^ b) | ~aux0 | b) & aux0 & a) & ~(~(a &
		a) & aux0 & ((b | a) ^ ~b | a | aux0 | a) & (a | a | aux0 |
		~b | b ^ (b | aux0) & aux0 & a))) + 47210 * (a & (aux0 ^ a ^
		(~b ^ (b | a) | b & a ^ ~b | ((aux0 | b) ^ aux0 ^ aux0) &
		b)) & ~((a ^ ~~a | aux0 & (b ^ a | b)) ^ ~(aux0 & b ^ aux0 ^
		a) ^ a & (b | b) & ~a)) + 19085 * (b & aux0 & aux0 & b &
		aux0 ^ ((~(a & aux0) ^ ~a ^ (aux0 | aux0)) & aux0 ^ (a ^
		(aux0 & aux0 | a | a) | a ^ (aux0 & b | aux0 & a)) | ~(a & a
		| b ^ b) & aux0 & ((~a | b & aux0) & (b & aux0 ^ a ^ aux0) |
		~~(b ^ aux0))) ^ (a & (~a ^ (b & b | a ^ aux0) ^ aux0 ^ ~(b
		| aux0 ^ b) & ~b & b & b & (aux0 ^ a | b & b)) | ~(~(aux0 &
		b) | aux0 & b & (a | a)) ^ aux0 & ((aux0 | b) ^ aux0 ^ b) &
		(a ^ aux0) | ~~(~(b & b) & ~(b | b)))) + 46466 * ((a ^ (~(b
		^ aux0) | a ^ a | ~a) | b) & ~(b & aux0 & b & b ^ (a | aux0)
		& ~a) & ((aux0 ^ ~aux0) & (~b ^ a & aux0) ^ (aux0 | a | aux0
		& a) ^ ~~aux0) & (~(~a & ~a) | ~~a & (b ^ b ^ a ^ aux0) |
		(aux0 ^ a ^ (a | b) | a) ^ a | (~~a | aux0 & (a | aux0) | a
		| ~(aux0 & b)) & ((aux0 | a) & b ^ (a | ~b)) & ((aux0 | aux0
		& a) ^ ~~aux0)) ^ ~(~b & ((aux0 | b | ~aux0) ^ (aux0 ^ b |
		~aux0) ^ b & b) ^ aux0)) + 14331 * (~(b | ~((aux0 & aux0 ^ a
		& b) & a) & ~(a ^ ~b)) ^ ~(a | b ^ (aux0 | a) | ~(aux0 & b))
		^ (~(aux0 ^ aux0) ^ (b | aux0) ^ (b ^ b | ~a | b | a | a) |
		(~a | b & b) & aux0 ^ a) ^ (~((~a | a | aux0) & ~aux0) | b &
		aux0 ^ aux0) & (a | (b ^ (aux0 & b | aux0 ^ aux0)) & b)) +
		64825 * ~~((b | a | aux0 | b | aux0) ^ b & ~b | ((~aux0 & a
		| a ^ b ^ ~aux0) ^ (a & b | aux0 & aux0) & (~b ^ a)) & (~(b
		& b) ^ (b | aux0 | aux0 ^ aux0) ^ ~((b | aux0) & (aux0 ^
		b)))) + 39432 * (aux0 | b) + 39562 * (((a | a | b | b & b) ^
		(b ^ a) & b & a ^ (b & b | ~aux0) ^ (aux0 | ~~a) ^ ~~~b) &
		aux0 ^ (a | a | ~(b ^ ~~a)) | ((~a ^ a) & ~a & (~a | aux0 &
		a) & ~(b ^ aux0) | b & (b ^ aux0) & ~aux0 & aux0) & (a & (~a
		| b | b | b | aux0) ^ (a ^ (a | ~a) | a | aux0 | a | a)) ^
		aux0) + 6571 * ~(b ^ b) + 63940 * (~~((b ^ (aux0 | b) & (a &
		a | ~a)) & (~(b | b) ^ a) & (~b ^ a & b ^ ~(a | b))) | ((~b
		^ (aux0 | b) | a) ^ a | ((a | b) ^ a & a | (aux0 ^ b) &
		(aux0 | b)) & (a | ~aux0 | a & a & ~aux0)) ^ (aux0 & (a ^
		aux0 | ~a) ^ (b | aux0 | aux0) ^ (b | ~aux0)) & (a | a | ~b
		& (aux0 | a)) | ~((~aux0 & ~aux0 | a & aux0 ^ aux0) ^ (aux0
		^ b | a) ^ b ^ a ^ a ^ aux0) ^ (~a ^ ~~a | aux0 & b & (a ^
		b) & (aux0 ^ (aux0 | a)) | ~(~a & a | b & aux0 | aux0))) +
		38175 * (a ^ ~aux0) + 44768 * (aux0 | aux0 | b | ~~a) +
		20068 * (~((a & a & (b ^ aux0) ^ ~(b | b)) & aux0) &
		(~~(aux0 ^ aux0) ^ aux0 & a & (a | aux0) & a & ~aux0 | aux0
		& aux0 ^ aux0 ^ b & aux0 ^ a & aux0 | a) | b | aux0) + 50808
		* (a ^ b) + 12255 * ~((~(~(a & b & a) & (~b ^ b ^ a ^ b & b
		& (aux0 | aux0))) | a) ^ aux0 ^ aux0 ^ a) + 4153 * ((~(~b ^
		aux0 & aux0 & aux0 & a) ^ ~(~b & ~aux0 | a | a) ^ (aux0 | (b
		& a ^ ~aux0) & (aux0 & aux0 ^ ~b)) ^ (~~a | aux0 | ~~a & a))
		& ~aux0 | a ^ (~(a & b) & a | b ^ aux0 | aux0 | ~aux0 & b) &
		~~a & (~(aux0 ^ aux0) ^ a ^ a) ^ (~((~b | aux0) ^ ~(aux0 &
		aux0)) | ~((a | b) & (b ^ a) | b))) + 17199 * ((~(~((~a | a
		| aux0) ^ (aux0 ^ aux0) & (aux0 | a)) & ((a & aux0 | a & b)
		& ~b ^ ~(a ^ a) ^ ~(aux0 | aux0))) | ~a | a & a) & a) +
		38086 * ((~(aux0 & ~~b | ~(a | ~b)) | b & (aux0 ^ aux0 | ~b)
		& b ^ ~aux0 & (b ^ (b | a) ^ aux0)) & a & b) + 19603 * ((b ^
		(~((a & b | b & aux0) ^ a & aux0 & (a ^ a)) | aux0 ^ ~aux0 |
		a ^ b ^ (a | aux0) | a ^ a) & ((aux0 & aux0 | a ^ a) ^ b &
		(a | a) ^ aux0 ^ b & a ^ ~~a | a | ~a | aux0 & b | aux0 &
		a)) & (b ^ ~~~(~(b | aux0) & (aux0 ^ aux0) & (a | a)))) +
		37705 * ~~(aux0 ^ ~(a ^ b & a) ^ (a | a & a) & ~(aux0 |
		aux0) | aux0) + 44167 * (~(((aux0 | b) & ~b | ~~aux0) ^ ~a &
		(aux0 ^ a) ^ (aux0 ^ aux0) & ~b | a ^ (aux0 | a | a & aux0)
		^ (aux0 | b | b & aux0 | ~aux0)) & ~((a ^ aux0) & b & ~b &
		~a ^ ~(aux0 | aux0) & (~b ^ ~aux0)) & (aux0 | b ^ (b ^ aux0
		| b ^ a | ~(a & aux0))) ^ ~(~(b ^ a) & ((a | aux0) ^ (b |
		a)) ^ ~~(b & a)) & ((b | ~(aux0 | a) | ~~b) ^ ~((a ^ aux0) &
		~b ^ b)) ^ ((aux0 | ~a) ^ (aux0 & aux0 | a | b)) & ((a | b)
		^ b & b ^ a ^ a ^ ~a) & ~(~(a & aux0) & b & aux0) & (~~(aux0
		^ a | aux0 | aux0) ^ b)) + 43983 * (b & ~~(~~(aux0 ^ a) | b
		| ~a | aux0 | b | a | b) ^ ~~(~~(a & aux0 | aux0 & aux0) &
		~(~aux0 ^ a | aux0 & aux0 & (b | a))));
    vm->memory[--vm->sp] = result;
    // std::cout << "[DEBUG] SUB: " << a << " - " << b << " = " << result << " [sp=" << vm->sp << "]" << std::endl;

    WORD next = vm->fetch_opcode();
    vm->handler_table[next](vm);
}

void VirtualMachine::op_mul(VirtualMachine* vm){
    WORD b = vm->memory[vm->sp++];
    WORD a = vm->memory[vm->sp++];
    WORD aux0 = b^a;
    WORD result = (24489 + 64109 * aux0 + 32219 * ((~aux0 ^ ~b & (a | a &
		b) & ((a | a) ^ b)) & aux0 & ~((~(a ^ aux0 ^ a & b) ^ a ^
		(aux0 ^ aux0) & (aux0 ^ aux0)) & (b ^ a ^ aux0 ^ (~aux0 |
		~a)) & (aux0 | aux0 ^ ~aux0)) ^ ~a) + 14373 * (~~(a ^ a ^ a
		| a | b & (aux0 | aux0) & ~a) ^ a ^ b) + 38890 * b + 40190 *
		((b ^ ~a ^ aux0 ^ (aux0 ^ aux0 | aux0 | ~(aux0 | aux0)) ^ (b
		| aux0 | a ^ aux0 | b & b & ~a) | (~b | ~aux0) & (a | (b ^
		b) & (b | aux0) & (~a | b & aux0) | ~b ^ (a | a) ^ aux0 ^
		aux0 ^ aux0 | b)) & (a ^ ((a | ~aux0) ^ a ^ b ^ b ^ (a | ~a)
		^ (a | b) & (b ^ a)) & a & aux0)) + 47207 * (a & ~b | ~~(~~b
		| (b | a) ^ aux0 & a | ~~b & (~b ^ aux0 ^ aux0)) & (aux0 ^
		~(~(aux0 | b) | b | b | b ^ aux0 | aux0 & (a | a | ~b)))) +
		50363 * (a | ((a | (b | b) ^ aux0 ^ b) ^ aux0 ^ ~(a & b) |
		a) ^ aux0 | a | ~b) + 28099 * (aux0 & ((~(a & a) | aux0) ^
		(aux0 ^ a ^ a ^ aux0) & aux0 & (b ^ aux0)) ^ (~aux0 | ~(aux0
		| aux0) | aux0 | aux0 | aux0 & b) & ~~~a ^ ~aux0 ^ (a | ~b ^
		b) & ((b | aux0 | b | a) ^ b ^ a ^ aux0) ^ b ^ ~(a ^ a |
		aux0 | aux0 | ~aux0 & ~b) ^ ~((a & aux0 | a ^ b | b) & aux0
		& ~(aux0 & b) ^ (aux0 ^ aux0) & b & aux0 & (~a | b ^ aux0) ^
		aux0 & (aux0 ^ b) & b) & (~~((aux0 & aux0 | a & b) & b & a &
		b & b) ^ (a ^ b ^ aux0 ^ ~(a & b)) & ~((b | a) ^ aux0) & (a
		^ a))) + 40482 * a + 49735 * (a | aux0) + 4278 * (aux0 &
		~~((b ^ a) & (b ^ b) & (aux0 ^ aux0)) & ~((aux0 & b | aux0 |
		~a ^ b & a | b & a | aux0 | b ^ b) ^ b) & (~b ^ a ^ ~b ^ a))
		+ 49713 * (~(((a & a | ~aux0 | b) & (b & aux0 ^ (b | aux0) ^
		aux0 & aux0 & ~aux0) | aux0) ^ (~a & (aux0 ^ b) ^ (aux0 | a
		| b ^ aux0)) & (aux0 ^ b & b | b & (a | b)) ^ (aux0 | a)) ^
		b) + 10658 * (~((aux0 & (aux0 ^ b) | a ^ aux0 ^ aux0 ^ a |
		~aux0) & ~((aux0 | a | a) & (aux0 | ~aux0)) ^ (aux0 ^ b ^ a
		^ a | (a | aux0) & ~b | ~(b | b) | b | ~aux0) & aux0 & (aux0
		| a)) ^ (((b ^ b) & (a | b) | ~b | aux0 & aux0) & ~b ^ b) &
		(~~(b & b | aux0 & aux0) | ~aux0 ^ aux0 | ~(a & a ^ a ^ a))
		^ (b | ~((aux0 & b ^ b) & (b ^ b ^ ~aux0) | aux0))) + 27250
		* (~(aux0 | (~a ^ a ^ aux0 ^ a ^ aux0) & (aux0 ^ ~~a) & a) &
		(b ^ a ^ ((b | b) & aux0 & b | b)) & b & (~~((a ^ a) & (b |
		b)) & ((aux0 ^ b) & (aux0 ^ a) & ~(a ^ aux0) | ~aux0) | a))
		+ 43319 * ~((aux0 | ~b) & b) + 43718 * ((~((a | b) ^ (aux0 |
		a)) ^ aux0 ^ ~(aux0 & a) ^ aux0 & (a | ~a ^ ~a) | ~(aux0 | b
		^ a) ^ (aux0 ^ a) & a & b & (b ^ aux0) | a & b) & (~((~b | a
		^ b) ^ a ^ aux0 ^ b ^ a) & (aux0 ^ (aux0 | b) ^ (b ^ aux0) &
		~b) & ~~(aux0 ^ b) | a) & (b | aux0)) + 6056 * (b | (aux0 |
		a | a ^ b ^ a ^ b & aux0 ^ (aux0 | b)) ^ ~((aux0 & a | aux0)
		& a) ^ aux0 & (b ^ b & b | b) | a) + 42895 * (aux0 & b) +
		34290 * ((~((aux0 ^ a) & a & aux0 ^ (b | b | aux0 ^ b) & b &
		a & ~a) | (~((b | aux0) & a) | a | a & a | b ^ (b | b)) & b
		& aux0) & b ^ ~(~~(aux0 | a) & a | (a ^ aux0) & ~b & (a | b
		| a ^ aux0) | aux0) ^ a) + 58263 * (~a ^ (~~((aux0 | aux0) &
		(aux0 | b) | ~aux0 & (aux0 ^ b)) ^ ~~~~(b | b) | (~aux0 ^
		~aux0) & ~(a ^ b) & (b ^ aux0 & (b | a ^ a | b)))) + 23692 *
		(a & (~(a ^ ~b & a & aux0) | a) ^ ~(~aux0 ^ ~b ^ ~(a &
		aux0)) & ~~((aux0 | a) & (aux0 | a)) & (~aux0 | b) & (~~aux0
		^ aux0 ^ ~aux0) | ~(b | aux0)) + 54567 * (((b | (aux0 & a &
		aux0 ^ b) & (~(b | b) ^ (b & a | aux0 | a))) ^ a) & (~(b ^
		aux0 & b & a | b & aux0 & (b | aux0) | a ^ a ^ ~aux0) ^
		aux0) ^ b) + 53140 * (aux0 & (a ^ ~(a & a ^ aux0 ^ b) &
		~(aux0 & b & (aux0 ^ b))) & ~((~a ^ a ^ aux0) & (b ^ a | a ^
		a)) & ((b | a) ^ ~a) & (a | b | a | a) & aux0 & b & b) +
		28701 * ~((~b ^ a) & (b | a & b & ~(b & b) | (aux0 ^ a) &
		aux0 & b ^ (a | b | b)) & ~(b | ~~(b & aux0))) + 13906 *
		~~(b | aux0 ^ ~aux0) + 6643 * ~~aux0 + 2347 * (a & a & ~~((a
		^ aux0) & ~aux0 ^ ~(b | b) ^ (aux0 ^ a | b) & (~a ^ ~aux0))
		| b) + 39414 * (b ^ b) + 36999 * (~(~((a | b | b ^ aux0) &
		(b | b) & ~a ^ (~a ^ ~a) & aux0) & (b | ((aux0 | b) ^ b) & a
		^ (~a & aux0 | ~b & aux0 & b))) ^ ~(b | ~(~a & b) ^ ~((aux0
		| a) & ~a)) ^ ~(a ^ b ^ (b ^ aux0 | ~b) | ~((a ^ a ^ ~b) &
		(aux0 ^ b | b)))) + 21597 * (b & b) + 7844 * (b | ~(~((a ^
		(b | a) | a & a ^ aux0 & a) & ~~b) & ((~(aux0 ^ aux0) & (~a
		| aux0) | aux0 & a | a | aux0) ^ aux0))) + 64155 * ~(b & a &
		(~a | ~aux0 | ~(a & aux0)) & b | b) + 23540 * (aux0 | (aux0
		| ~(a & aux0) | ~((a | a) & (a | a))) & aux0 & ((a | b & (b
		^ a ^ (b | a))) ^ b) & b & (a & ~(b ^ b ^ b & b) ^ ~(aux0 |
		b | a) & ~(a ^ b) & (aux0 | aux0 | b))) + 12174 * ~a + 32655
		* (aux0 | ~(~~~~~b ^ b & aux0)) + 44359 * ~(((aux0 & (b | a)
		^ a ^ ~a | a) ^ aux0 | (aux0 ^ aux0 ^ a | a) & (aux0 & b &
		~a ^ ~b & (aux0 ^ aux0)) ^ (aux0 | aux0 & b & ~b ^ aux0)) ^
		a) + 16026 * (~(b ^ ~~((b | aux0) ^ (b | a))) & ~~(a | ~(b |
		a) & (a | aux0 & b)) & (~aux0 & b & aux0 & ~a & ~~a & ~b &
		(b ^ aux0) & ~(a ^ aux0 ^ a ^ (a | a & b)) | ~(~~(a | a) &
		(aux0 ^ a ^ b ^ aux0 | ~(b ^ aux0))) | ~b)) + 64378 * (b ^
		aux0) + 28180 * ~((aux0 | ~~(aux0 & a) ^ (b ^ aux0 ^ aux0 &
		b) & (a ^ b) & (aux0 | aux0) | aux0) & a) + 41434 * ~(~(a &
		b) & (~(a & aux0 & b) | b) | b | ~b) + 5266 * (b ^ (~b & a |
		(((~aux0 | a & aux0) ^ b | b) ^ b) & ~a)) + 13708 * (~(aux0
		& ~b ^ b & aux0 & b) & aux0 & a & ((~(aux0 ^ b ^ aux0 & a) |
		(aux0 ^ b) & (b | b) & ~b & b) ^ (~a | ~a & (aux0 | b) | ~(a
		| a)) | ~(((~aux0 | ~a) ^ (aux0 | a) & aux0) & a)) | aux0) +
		7316 * (~((b ^ (a & aux0 | ~a) | ~(a & b & aux0)) & ~~(b |
		aux0) & (a ^ a ^ b ^ b) & a & aux0 & ~b & aux0) ^ ((aux0 |
		a) & (aux0 ^ a) ^ a ^ b) & ~b & (a ^ a) & ~~(~b & ~(~a &
		a))) + 46966 * ((aux0 ^ (~aux0 ^ (a | a | b & a) & ((aux0 |
		aux0) ^ a & b) ^ ~~a & b ^ (a ^ aux0 ^ a ^ aux0) & ~~b |
		aux0 & (~a & (b ^ a) ^ (a | a | aux0 & b)) | aux0 | (b | a)
		^ a & b | (b | aux0 | b | b) ^ aux0)) & (~(~b & (~b ^ ~(b &
		a)) ^ ~((aux0 ^ b) & (b ^ a)) ^ ~(~a | ~aux0)) ^ (aux0 | (~b
		| ~aux0) ^ (aux0 ^ aux0) & ~a ^ (~b | a | b) & ~(b | aux0) ^
		(a ^ aux0 | a & b) ^ b ^ b ^ a))) + 53414 * ~((~a ^ (~(a ^
		(aux0 | b)) | aux0 & a & b & b) ^ a) & (aux0 | ~~aux0)) +
		13798 * ~aux0 + 13792 * ~(a & (aux0 ^ (b | a | b | b |
		~aux0) ^ b ^ (aux0 & aux0 | aux0 | (b | a) ^ (b | b)))) +
		56845 * (aux0 ^ (aux0 & ((~(b ^ a) | ~(a ^ a)) ^ ((aux0 |
		aux0) ^ aux0 | aux0)) & (~(b & aux0) | b | a) | a)) + 64372
		* (~~aux0 ^ aux0 ^ ~(~~b & ~~b) | a | b & aux0 ^ b & a ^
		(aux0 & a | aux0 & b) | a | ((a | aux0) ^ aux0 ^ a) & (a | a
		& a) & (a | aux0 & aux0) & ~(b ^ a) | a) + 14903 * ~((a ^
		~~a ^ (((a | a) ^ a) & (a ^ (a | a)) | (a | b) ^ a ^ aux0))
		& a & ~~aux0) + 40931 * (aux0 | ~(b ^ (a ^ a | a ^ a)) &
		(~aux0 & aux0 & a & ~b | ~b) | aux0 | ~((aux0 | a) ^ ~~~b &
		((~(b | aux0) | a) ^ (~b | ~b) & (a & a | b | a)))) + 24991
		* (~(b | aux0 | ~~aux0) ^ a) + 61890 * (aux0 ^ ~(~(a &
		~(aux0 ^ aux0)) & a | (~a | ~b) & (a & aux0 | aux0 | b) ^ a
		^ (a | b) ^ ~(aux0 & a) ^ (a & a & aux0 | (b ^ b) & b | a &
		a & ~a | a & (a | a)))) + 18997 * ((a | b | (~(a | a) |
		~aux0) & ~~(aux0 ^ a) & b) & (~(aux0 & (a ^ b | ~b) | a ^
		aux0 ^ ~b | ~(a ^ b)) & ~(b & (b | b)) | ~a & ~a | b)) +
		52456 * ((b & ~(aux0 | b) & (~a | ~b) & ~b | ~aux0 ^ (b |
		~aux0)) & (~(a | a | aux0 | a & a ^ aux0) & aux0 ^ ~(b &
		aux0 & aux0)) | (~(a & b & b & aux0 | b) | ~(aux0 ^ aux0 ^
		aux0 & b) & (a & (b ^ a) ^ (b ^ b) & ~b)) ^ b ^ ~~(aux0 ^
		~~b ^ (a | aux0 | b | (a ^ b) & a & b))) + 52517 * (~a |
		~((~(aux0 ^ aux0) & (~b | ~b) ^ aux0 & (aux0 ^ a) & a & b) &
		(a ^ ~b | b & (b ^ a)) & ~aux0 ^ a)) + 27692 * (a | ~~((~(b
		^ b) | b) & (aux0 ^ a ^ b & a) & ~(aux0 & a)) | ~(a ^
		~((aux0 ^ a) & ~a) | b & b & (a | aux0) ^ b) | ~a) + 51745 *
		(~((~(aux0 & b & (aux0 | a)) & ~((a | a) ^ b) | ~(a & aux0 ^
		(a | aux0)) ^ ~a) ^ ~(~(aux0 & a) | ~a & (aux0 ^ aux0)) &
		(aux0 & a & aux0 & (a ^ b) | a & a | a & b | a & a & aux0 &
		b)) & (aux0 | (((~aux0 | ~a) ^ (aux0 | b) & b & aux0) &
		(~(aux0 ^ b) ^ ~aux0 & ~aux0) | (~(a & b) | ~aux0 & aux0 &
		aux0) & (b ^ a) & aux0 & ~(b & b)) & (~(~(aux0 | b) | aux0 ^
		a ^ (b | a)) ^ (b | aux0)))) + 56465 * ~(a & (aux0 ^ b | ~(a
		^ aux0 ^ a | a)) | b & ((b | b) ^ (b | a & a | ~(b | aux0))
		& ((a | aux0 | a) ^ a ^ b ^ (a | aux0)))) + 52779 * (a &
		(~~(a & ~b & (aux0 ^ b) & aux0) ^ ~(~a | (a | ~a) ^ (a & a |
		~a) ^ (b ^ b | b)))) + 50321 * ((~~~(a & b ^ ~b) | b & (a |
		aux0)) ^ (b ^ (aux0 | b) & (aux0 | b) ^ ~(b & b) & (b ^ b ^
		b & a)) & ~~((a | b) & ~aux0) & ~(~aux0 & (a | b) & (a ^
		aux0 | ~b) | ~(a & b)) ^ aux0 ^ ~~(~~a ^ (aux0 | aux0))) +
		23492 * (a & aux0) + 26134 * (~((a | ~(~a & (aux0 ^ aux0)))
		& b) ^ (~(~aux0 | ~(a | aux0) ^ aux0) | ~aux0 | ~b | ~((b |
		aux0) ^ a & b) & (b & aux0 | aux0) & aux0) ^ ~~(~~~a | ~~(b
		^ b)) & aux0) + 59794 * ~~~~(((a & b | a) ^ ~~a) & ~((a ^
		aux0) & b & b)) + 9194 * (~~(~a & (~aux0 ^ aux0 ^ a) ^ (aux0
		& b | aux0 | a) & (aux0 | a) & (aux0 | a) | aux0) & ((aux0 |
		aux0 | aux0 & aux0 & (a ^ aux0) & (~a ^ (a | b)) | b | aux0)
		^ (~a | ~(aux0 ^ ~~(b ^ a))))) + 10728 * (~((a | (~aux0 ^ a
		& b) & a & a & b) & (b & aux0 | b & b & b ^ ~(b & aux0)) |
		((b | aux0 ^ ~aux0) ^ (a | a | b & aux0) ^ aux0 ^ aux0 ^ (a
		| a)) & ((b | ~(b & a)) ^ (b ^ a ^ b) & (~a ^ ~aux0))) | (a
		| a) & a) + 19823 * ((~~a & ~(~(b & b) & (b ^ ~a) ^ ~(b ^
		~b)) | ~(aux0 ^ (a & b & (aux0 ^ aux0) | ~(aux0 & aux0)) ^
		~(aux0 & a & (b ^ b) | a))) & ~((~b | a) ^ a & ~~(b & aux0 &
		a))) + 40423 * (~(~(aux0 | a | ~a) | (~a ^ (aux0 | a) & (a |
		aux0)) & a) | ~a & b | ~(aux0 | a | ~aux0 ^ aux0) & ((~b & b
		& b | ~b ^ (aux0 | aux0)) ^ ~~(aux0 & a)) & ~aux0 ^ (~~b ^
		(a | a | aux0 & a) | ~a & b & aux0 | a & a ^ a) & (aux0 &
		((b | a) ^ ~aux0) | b) ^ ~b & ((a | b | aux0 | aux0) & b &
		(b | b) | b | aux0 ^ (b | b))) + 9185 * ((~~~~(b | a | b ^
		a) ^ ~~(b & aux0 ^ aux0 & a ^ b ^ b ^ a & b ^ b & (~a | a &
		b))) & (b | aux0 & a & (b | b) | a ^ a ^ b & a ^ ~(a & a) ^
		(a & aux0 & a | ~~aux0) | b) & b & ~a & (aux0 | b) & b & (b
		| b) & ~(aux0 ^ a) & b & ~~(aux0 ^ a ^ aux0 ^ (a ^ a | a)))
		+ 8870 * (~b ^ a | (aux0 | ~~((aux0 ^ aux0 | ~b) & ~(b &
		b))) & b) + 37913 * ((aux0 | aux0 & (a | ~aux0 & (a | aux0)
		^ (aux0 | aux0 & aux0)) ^ aux0 ^ ~(aux0 | ~b ^ ~aux0)) &
		aux0) + 51195 * (~((~~a | ~a | a & a) & (a & (b ^ aux0) ^ (a
		| a) ^ a ^ aux0) & (aux0 & ~(b | a) | ~aux0)) ^ ~(~~(b | b |
		aux0) & a) ^ b) + 54618 * (~(((aux0 ^ aux0 ^ a ^ b) & (aux0
		^ a ^ b & b) ^ aux0) & ~aux0) ^ ~((~~(a | a) | aux0) & aux0)
		| a | ~((~(a & a) | ~~b) & (aux0 & aux0 & b ^ (a | b) ^
		~aux0)) & ~(~(aux0 & b & b & a) | b ^ a | b | b)) + 65459 *
		(b ^ (b & (aux0 & a ^ b) | (a | b | ~a) ^ (b | a) | ~((a ^
		aux0) & b) & (~aux0 | a ^ b & b) | b) ^ aux0) + 35786 * (a ^
		aux0) + 47538 * (~((a ^ b ^ (a | aux0) ^ ~a ^ b ^ (aux0 & b
		^ aux0) & b & ~a) & b ^ ~a ^ (a ^ aux0 ^ aux0 | ~(aux0 | a)
		| b | a | a | aux0 & b & b)) & ((aux0 | b ^ aux0 ^ aux0 | a
		& (b ^ a) | aux0) ^ ~((aux0 & a & ~b | ~aux0 ^ (b | a)) & b
		& b) ^ a)) + 27583 * ((~((aux0 | a ^ aux0 | a & b) ^ (~b |
		~aux0 & (a ^ aux0))) | ~a & (aux0 ^ (aux0 | b | a | aux0 |
		b)) | ~(aux0 | b) & (~b | a) & ~~(a & b) & (b & a | a | aux0
		| ~~b | a & a ^ aux0 & b ^ (a | aux0) & a & a) | b) & (~b |
		aux0)) + 1737 * ((~(((aux0 | b) ^ (a | a) | aux0 & a & ~a) &
		(aux0 & b | aux0 | aux0 | (b ^ b) & ~b)) ^ ~(~(aux0 & b) &
		(b | aux0 | b | a)) ^ ~((aux0 | aux0) ^ aux0) ^ (aux0 | a) ^
		a) & (aux0 | aux0)) + 15410 * (aux0 & ((~(~aux0 & ~aux0 ^
		aux0) | ~aux0) ^ ~~(b & b ^ b & b | ~b | b)) & b) + 37598 *
		(b ^ (b | (aux0 & ~(b & a) | ~((b ^ a) & ~a)) ^ aux0 ^ (~(~b
		| ~a) | ~(aux0 ^ b) & (~b | a & aux0) | a | ~a))) + 19152 *
		((((a ^ a) & (aux0 ^ a) ^ (b | b) ^ aux0 ^ a) & (~aux0 | a |
		a | a) | (a | a | aux0 ^ b | aux0 & b & (a | aux0)) & ~((a ^
		a) & ~b) | a) & (b | ~(b | a) & (aux0 | b & b) | ~aux0) &
		(~((b | b) & (a | b)) ^ ~~a ^ aux0) & aux0 ^ ~(~(~(aux0 &
		aux0) ^ (a & aux0 | b & aux0)) ^ ~~aux0) ^ ~~(a & aux0 & b |
		~aux0 & ~a | a ^ ~a & ~aux0)) + 23264 * (a & (a | ~(aux0 & b
		& a)) & (aux0 ^ (a ^ b & b & (aux0 | b)) & (a ^ a ^ aux0 | b
		^ aux0 ^ aux0 & b)) ^ ~(aux0 & aux0) & (aux0 ^ a | b | aux0)
		& ((aux0 | b) ^ aux0 & b ^ (~aux0 | aux0 ^ a)) & ~~((aux0 |
		a) & ~aux0) & ~((aux0 | a) & aux0 & a ^ aux0 & aux0 & (b |
		a)) & ~~(aux0 & aux0) & (~(aux0 | a) | (a ^ aux0) & a &
		aux0) | (~~((b | b) & (aux0 | a)) | ~b | ~b | ~b | ~(b ^ b))
		& ((b | aux0) & ~aux0 ^ b ^ b ^ ~b & ~a & ~aux0 | ~((a & b |
		aux0 | a) & b)) ^ ~aux0) + 60395 * ~((aux0 & ~(b & b ^ a ^ a
		| ~(a & aux0)) ^ ~(a ^ a | (a & b ^ a ^ a) & (b ^ (aux0 |
		aux0)))) & (~(((b | a) ^ a | ~b) ^ (aux0 | a | a | b) & ~b &
		a) | ~((aux0 | aux0 ^ b) & ~aux0 & aux0 & a | b ^ aux0 ^ ~a
		| (aux0 | a) ^ b & a))) + 47861 * ((~(~~aux0 ^ ~a ^ aux0 & a
		^ aux0 ^ a ^ aux0 ^ (aux0 ^ a | aux0 & a)) ^ a) & ((a ^ b ^
		aux0 | b & a | aux0 ^ a) ^ a) & (b | ~~a ^ ~(aux0 | aux0)) &
		~~(b | b ^ aux0) & (aux0 ^ a | ~(b ^ aux0)) & (a & a | b |
		aux0 | b & (aux0 | a)) & a) + 20729 * (~(a ^ ~a) & ~((~b |
		~(aux0 & b) | ~aux0 ^ (b | a)) & ((a | aux0 ^ aux0 | (b | b)
		^ ~b) ^ b)) & (aux0 | aux0 ^ b & b ^ aux0 ^ a ^ aux0 ^ aux0
		^ aux0)) + 43464 * (aux0 | (~(b & aux0 ^ (a | aux0)) ^ b) &
		a | b & b & (aux0 & a ^ ~b ^ (b | b) ^ aux0 & aux0 ^ a ^
		~~aux0 & (aux0 | a | b) & b) | b & a & aux0 & a) + 6058 *
		(~~((~(a ^ aux0) ^ (a | a) ^ ~b) & (aux0 ^ aux0 | b | b) &
		(b | b ^ aux0) & (~(a ^ a) ^ ~(aux0 & aux0)) & ~~(a ^ a)) &
		((~(aux0 ^ aux0) & aux0 & ~aux0 | a | ~(aux0 | a) | (~b & ~a
		^ aux0 ^ b & b) & ~b) ^ ((aux0 ^ b) & ~b | aux0 ^ b | a |
		aux0 & ~a ^ (aux0 ^ aux0 | b | a)) & ((~(a & aux0) | (b |
		aux0) & a & b) ^ (~~b | ~(aux0 & b)))) & (b ^ ~aux0) & (aux0
		| ~(aux0 & b) ^ aux0 ^ a ^ aux0 | ~(~(a ^ a) | ~a))) + 7348
		* (((((~aux0 | a ^ b) ^ (a ^ b | a ^ a)) & (~a | ~aux0 & a &
		aux0) | ~(~b | a) ^ b) & ~aux0 ^ ~~(b | ~(a ^ aux0)) ^ ((~~b
		^ a ^ a ^ b & a) & ~a & aux0 & a | a)) & ~(~~a & (aux0 & ((a
		^ aux0 | b ^ aux0) ^ aux0 & a ^ b ^ a) ^ ~a))) + 59195 * (b
		| a ^ ~~(aux0 & (b ^ aux0)) | ~a ^ ~(aux0 | a) ^ ~~b ^ (~a |
		aux0 ^ a | ~(b & a)) | b) + 61067 * (b & ~(aux0 & ~(a ^ b ^
		b ^ a ^ b & a)) ^ ~(((a | aux0) & (aux0 ^ a) | a | a) ^ ~~~b
		& (b & aux0 | aux0 | b | aux0 & aux0 ^ a) ^ b & (a ^ (aux0 |
		b) ^ (aux0 | a) ^ a | ~(a | aux0) ^ a ^ a ^ a & a))) + 64008
		* (a | (((aux0 ^ a) & (a | b) ^ (b ^ a | b) ^ ~~a ^ b ^ aux0
		^ a) & ((~b ^ b) & (~a ^ aux0) ^ a & aux0 ^ (a ^ aux0 |
		~aux0)) ^ (~~b | a | ~aux0 | aux0) & b) & b) + 60611 * (b |
		~~~(((b | aux0) ^ ~b | aux0 & b & (b | aux0)) & (a ^ ~~b)))
		+ 54107 * (a ^ ~((~~b | a) & ~(b | b | aux0) & a) ^ (~((aux0
		^ ~b | a ^ aux0 | a ^ b) ^ aux0 ^ b) | a) ^ ~(aux0 & ((b | a
		| b) & aux0 ^ (~b | a ^ aux0) ^ a & aux0 & b & aux0))) +
		33995 * (b & ~(aux0 | a ^ aux0 | aux0 | ~~aux0 & aux0 | ~(b
		| b) ^ (aux0 ^ aux0) & aux0 & aux0) & (~(~~(a & b) ^ ~aux0 ^
		b ^ b ^ ~(b ^ aux0)) ^ aux0 | b ^ ~~(a | aux0 ^ a) | ~b)) +
		45565 * (~~(~((aux0 | a) ^ b) | b & b ^ a ^ a | ~b & (b ^ a)
		| (a ^ b | aux0) & (aux0 ^ a | aux0 & aux0) ^ ~(aux0 & aux0
		| ~aux0)) & ((~(b ^ b) | (aux0 ^ (~a | aux0 & a)) & (aux0 ^
		b | aux0 & b ^ aux0)) ^ ~(~~(b | b) & (~aux0 & b & a ^ ~(b &
		a)))) & (~~(aux0 ^ b) ^ (~aux0 ^ a ^ a | (b | b) ^ ~aux0) ^
		aux0 ^ b & (~b | aux0 ^ aux0) | ~(~(b ^ a) & aux0) ^ a)) +
		17932 * (a ^ ~(((a ^ aux0 | b | b) ^ ~(b & a)) & (~(aux0 ^
		b) | a) & ~a) | ~(~((b | a) & (a | aux0)) | ~a ^ a & aux0 &
		(aux0 | b)) ^ (~~(b & aux0) | (a & aux0 | aux0) ^ b) ^ (a |
		~b | aux0 & aux0 & aux0) & (aux0 | a ^ b) & b | ~((b ^ b ^ b
		| aux0 & ~aux0) & ~~~b | ~((b | b) & aux0 & a) ^ a & aux0))
		+ 1328 * ((~(b ^ a & b ^ (b ^ a | a | aux0) ^ (b | aux0 | a
		^ aux0) ^ a) | aux0) ^ aux0 & (a | ~(~(a ^ a) & b & ~b ^
		a))) + 36904 * (~(b & (~(a & a & aux0 & aux0) ^ aux0 ^ a)) ^
		(aux0 & (aux0 & b ^ b | ~aux0) | aux0 ^ a) & ~a & ~(~a & b &
		aux0 & (b & b | b & a)) & (~(b | a ^ b) & ~(~aux0 & a) ^
		~(aux0 ^ a) | (a ^ aux0) & ((a ^ b | aux0 ^ a) ^ aux0) ^ a &
		(a | b | b) ^ aux0)) + 3946 * ~~(((a | b | a | b) ^ (~a |
		aux0 & aux0) | ~(~b & a) | ~b) & ~(~((aux0 | aux0) ^ ~aux0)
		| a ^ ~aux0 ^ (a | a))) + 9968 * ((a ^ ((~aux0 | a & aux0) ^
		b ^ (aux0 & b | aux0 & b) & a & a & aux0 & b) & ~(aux0 & a &
		aux0 & aux0)) & ~a | a) + 36832 * ((b | ~(a | b) ^ (b | a) &
		(b | aux0)) ^ b ^ (~(b ^ a) & (b | aux0 | aux0 ^ aux0) ^
		aux0) & b | (~~~aux0 | (~(a | b) | b & aux0 & (a ^ b)) ^
		~((aux0 ^ a) & b)) & aux0 & (a ^ ((a | b) ^ ~b | b ^ b ^
		(aux0 | b))) | ~(aux0 | ~(a | a) & ~~a) & (a ^ b | a | ~~b)
		& (b & aux0 & (b ^ aux0) | (a | aux0) ^ ~a) & (a ^ aux0 ^
		~aux0 ^ (b ^ (b | b) | (aux0 | b) ^ a)) ^ ~~(((a | a) ^ a ^
		b | b) & (b ^ ~(aux0 | aux0)))) + 35734 * (b ^ aux0 ^ ~(b &
		aux0)) + 56158 * (aux0 | a ^ ~(b ^ (~(aux0 ^ b) | b)) | ~~b)
		+ 48529 * ((b ^ ~(~~((a | b) & b & aux0) | ~(~(a | a) & b &
		b & ~b))) & aux0) + 14770 * ~(~((a | ~(b & a) ^ ~a ^ (b |
		b)) ^ a) | ~(~b ^ aux0 ^ (a ^ a | ~aux0) ^ a) & ((aux0 |
		aux0 | aux0) ^ (~b | a ^ aux0) | ~b | aux0 ^ a | aux0 & a) &
		(~(aux0 ^ a) | ~(aux0 & a & ~b))) + 12444 * ((aux0 & (a & b
		| b & aux0) & (~b | aux0 & aux0) & ((aux0 ^ aux0) & aux0 & a
		| ~(aux0 ^ aux0)) | a | aux0) ^ (~(b ^ a) & a | (b ^ a) & ~b
		^ aux0 ^ (aux0 | aux0)) ^ ~~(b & a | b & b) ^ ~(a & a | aux0
		| aux0) ^ (b & b ^ aux0 ^ a) & ((b | b) ^ ~b) ^ a ^ b) +
		20145 * ~~(~b ^ aux0 & aux0 | ~b | a & a | ~a & (aux0 | a) |
		~(aux0 ^ a) | a | (a & aux0 | ~(a ^ b)) ^ a & b ^ ~b ^ ~aux0
		^ (b | aux0 & a)) + 16457 * ~((((~b ^ a) & (a ^ aux0 ^ aux0
		^ aux0) & a | a ^ (aux0 | ~aux0 ^ aux0 ^ b)) ^ aux0) &
		(~((~b ^ (aux0 | aux0)) & (b ^ aux0 ^ b ^ b) & ((aux0 & b |
		a) ^ (b | a | aux0))) ^ b ^ ~~~a ^ (~~aux0 | (b | a) & b &
		a))) + 13518 * (a & b ^ ((b ^ a & a) & (a ^ a | ~b) | (b |
		aux0) & aux0 | ~a) ^ (aux0 | ~(aux0 & aux0 | a ^ aux0) ^
		(aux0 | a) & (b ^ b) ^ a & (aux0 ^ aux0)) ^ b ^ aux0 ^ b ^
		a) + 50264 * (aux0 ^ ~~a ^ b ^ (~(aux0 & (b ^ a) | ~~b) |
		((a | a) ^ b | aux0 & a & aux0 & aux0) & a) ^ (~(a | ~aux0 |
		b | ~((aux0 | a) ^ aux0)) | ~((~aux0 ^ ~a) & (b | aux0 | a |
		a)) ^ ~(b | ~a | aux0))) + 44036 * ~(a & (~(b & (aux0 ^ a))
		^ (aux0 & a | aux0 & b) & (a ^ a) & ~a) & aux0 | ((~~b | a &
		b | aux0) & ((aux0 | aux0) ^ a | a ^ aux0 ^ (aux0 | aux0)) |
		b ^ (~aux0 | a & a) ^ b & aux0 & b & (a | aux0) & b) & aux0)
		+ 1249 * (aux0 & (~((~((b | aux0) ^ aux0 ^ b) ^ aux0 ^ aux0
		& a & aux0) & b & b) ^ (a ^ ~b | ~(aux0 & b & a & aux0) &
		(~(b | b) ^ a) | (~b ^ (aux0 | b) ^ a & (aux0 | b)) &
		(~~aux0 ^ ~(b ^ b)) | ~(b & b & (aux0 ^ b)) & b & a))) +
		60255 * (((a & aux0 | b) ^ ~((aux0 | b & aux0 & b) & ((a ^
		a) & ~aux0 ^ (aux0 | a) & b)) ^ b) & b) + 1122 * (aux0 | ~~b
		^ aux0 & ~~(aux0 & b ^ (a | a) ^ b & a ^ a ^ aux0)) + 45918
		* (~(~b ^ a & aux0) & (aux0 | aux0) & (((aux0 ^ aux0 | aux0)
		^ b ^ aux0 & a) & aux0 & a | a)) + 33919 * ~~~b + 37051 *
		((aux0 ^ b ^ a ^ (b | b)) & b & (b & b | (~aux0 | a & aux0)
		^ (a | aux0 | aux0 ^ aux0)) | a | (~(aux0 ^ a | b & b) |
		aux0) & ((a | b & b) & (b ^ b ^ ~a) ^ a) & ((aux0 & a & (a ^
		b) | aux0 & (b ^ aux0)) & ((b | b) ^ a ^ a | aux0) ^ ~(a &
		a) ^ (a & b | b) ^ b ^ b ^ a ^ aux0) | (((a | b) & (a |
		aux0) | aux0 | b | ~b) ^ (aux0 ^ aux0 & a | aux0 ^ ~a) ^
		(aux0 & a | aux0 | b | ~a ^ ~a | b)) & ~(~a ^ a ^ b | b & b
		^ aux0 ^ aux0) & a | ~~((b ^ b) & ~b ^ ~~b ^ ~~aux0)) +
		33211 * ((~aux0 | a) ^ (a & (aux0 ^ aux0) & (b & aux0 | ~b)
		^ (b ^ a | b & (aux0 | b)) ^ aux0) & b ^ ~~(aux0 ^ b ^ (a |
		b) ^ a ^ ~(b ^ aux0) ^ (a ^ aux0) & ~aux0)) + 8250 * ~~((~b
		| ~(a | ~aux0 & aux0)) ^ aux0) + 9221 * ((b ^ ~~((a | b) & a
		& a)) & a ^ b ^ (aux0 | aux0)) + 35924 * (~((a & a | aux0 &
		aux0) & aux0 | aux0 ^ (b | aux0) & b) ^ ~~a ^ b | b) + 35267
		* ((a & ~((aux0 | aux0) ^ ~aux0 ^ ~(a ^ b)) | (~(a | b) | ~b
		^ ~b) & ((b | a | b & b) ^ a & aux0 ^ b ^ a) | ~(aux0 & ~(a
		^ a)) | (a ^ ~~b & ((a | aux0) ^ b & aux0)) & ~~~~b ^
		(~(aux0 & aux0 ^ aux0 & b | b ^ aux0 ^ a ^ aux0) | aux0)) ^
		(b ^ ~(a & a) ^ ~~aux0 ^ ~aux0 | ~(b & a ^ ~a) & (~b & (b |
		a) ^ b) & (~a & (~b | b & b) | b | a | a | ~(a | aux0))) &
		aux0) + 38989 * ~(((aux0 | b) & (~((aux0 | b) & (a | aux0))
		| (b | a) ^ b ^ aux0 ^ (a | aux0) ^ a & a) ^ a) & ~aux0) +
		63148 * (a & ~~(aux0 & a | a | (b | b) & aux0 & b | (~aux0 |
		b) & ((aux0 | aux0) ^ a) | a)) + 5347 * (b & (aux0 & a ^ a ^
		~((~aux0 | b & a) ^ ~a & (a ^ aux0) | ~~~a)) & b) + 37254 *
		~(~(~(a | a) ^ ~(aux0 ^ b) & ~(b | b) ^ ~(b | a) & (a ^ (b |
		aux0)) & ~(a ^ aux0) & a) ^ (~aux0 & (a ^ b) ^ (b & b | a |
		aux0) ^ ~(b & b) ^ a) & b ^ (a ^ a) & ~b & (a ^ b & b) ^
		(~(a & a) | b & a | b) ^ a) + 4769 * (aux0 | b) + 56812 *
		(((a & (~(b ^ (a | aux0)) | a | ~~b) | ~(a | a & aux0 | b |
		a)) ^ ((a & b & b & a | ~(a ^ a) | ~(aux0 & aux0) | ~a & a)
		^ (a ^ aux0 | b ^ aux0 | ~(aux0 | aux0)) ^ ~(a ^ ~b)) &
		(aux0 ^ (~a ^ (a | b)) & ~(a ^ aux0) ^ (~(a | a) | b | b &
		a))) & ~aux0) + 18819 * (~aux0 | a & (a | ~(aux0 | aux0) & a
		& a & aux0 & a | ~aux0 | a)) + 9698 * ((((~~b ^ a) & ~b |
		~(a & aux0)) ^ b | aux0 | a) & ((~~b ^ ~(aux0 ^ aux0 |
		aux0)) & (~(aux0 ^ aux0 ^ aux0) ^ ~~~a) ^ b & (a ^ b & b |
		aux0 ^ (a | aux0)) & aux0 | b ^ ~b ^ a ^ (~(b ^ b) | b) | (a
		^ (~a & (b ^ a) | (aux0 ^ aux0) & (b ^ a))) & ((~aux0 | aux0
		& aux0) ^ ~(aux0 ^ b) | ~(a ^ b)))) + 18723 * (b | b | a |
		~b & aux0 & (b & (aux0 ^ a) & aux0 & aux0 & b & aux0 ^ (a &
		aux0 & aux0 & a | ~(b | b))) & (a & (~a ^ (aux0 | b)) | aux0
		& b & a & (~b ^ a ^ aux0)) | (~(a & b & b ^ a ^ aux0 ^ b ^
		aux0) & b & (a ^ b) & (a | b) & (~aux0 | b & aux0) & a |
		~(~(aux0 ^ aux0) & ~(a | aux0) ^ aux0 & aux0 & b ^ b ^ a)) &
		(b ^ aux0 ^ b & ~(~aux0 | a))) + 45866 * (~b ^ ((a | b | a ^
		b | b) & ((a | a) ^ (b | a)) & (~a ^ ~b) | b & (aux0 & aux0
		^ ~a) ^ b | (~~(a ^ aux0) | b & aux0 & aux0 & (a ^ aux0 ^ a
		^ b)) & (a | ~~a | aux0)) ^ ~(b ^ aux0 & a | b)) + 11671 *
		~(b & ~~(~b ^ (a | a)) & ~~(aux0 | a) & ~a & b) + 50989 *
		(~~(b ^ ~((aux0 | b | a | aux0) & ~(a & a))) | (a ^ b) &
		(~(~((aux0 | b) & (b ^ a)) & (a ^ (aux0 ^ aux0 | aux0 ^ a)))
		| a)) + 16970 * ~(aux0 | ((a | aux0) & b & b | aux0 | (a |
		aux0) & a & b & b) ^ (aux0 | aux0 | b | b & ~aux0) & b ^
		((aux0 & a | ~a) ^ (a | b) & (a ^ b)) & (aux0 ^ b ^ aux0 ^ a
		^ b) ^ (a & b | a | b) & a ^ b) + 55873 * (b ^ ~((b & aux0 &
		aux0 | (aux0 | aux0) ^ ~aux0) & (a ^ ~b) ^ aux0) ^ ~~(~(~~a
		^ b & aux0 & aux0 & b) | ~(b | b) ^ (aux0 ^ b | ~b) ^ aux0 ^
		b)) + 36268 * (((b & aux0 & aux0 | ~b ^ b & aux0 | b) & aux0
		^ (~~b | aux0 ^ aux0 | aux0 | a) & (aux0 | a | aux0) & b &
		(b ^ a) ^ (b & a ^ aux0 ^ ~aux0 & a | a) | ~((b ^ a ^ b ^
		~(b ^ a)) & b & ~(aux0 | (b | b) ^ aux0 ^ a))) ^ ~~~((aux0 |
		~aux0) ^ ~b ^ aux0 & aux0 ^ ~(aux0 & a | aux0))) + 1875 *
		~(a & ~(((a | b) ^ (a | b)) & b & aux0 & b & aux0 ^ (b & (a
		^ aux0) | aux0 & b)) ^ aux0) + 4250 * (~b & ~((~(aux0 ^ aux0
		^ aux0) | ~~aux0 ^ b) ^ (~~aux0 | a) ^ ~((b | aux0) & b &
		aux0)) & ((aux0 ^ aux0 ^ b ^ a ^ b) & ~~(a ^ a) & aux0 &
		~~(~aux0 ^ b ^ (a & b | aux0 & b)) ^ b)) + 6782 * ((~aux0 ^
		~aux0 ^ aux0 | (~a | b ^ b) ^ b | (a & aux0 ^ b ^ b | ~aux0
		& b) & (aux0 & a | a | a) & b) & ~~(aux0 ^ aux0) | b ^ (a ^
		a | ~(~(aux0 | a) & aux0)) | (aux0 | b & a) ^ (~b ^ (~a ^ b
		| ~(a | b) & (aux0 ^ aux0) & (b | b)) | aux0)) + 32247 *
		((aux0 ^ ~aux0 | (~aux0 & aux0 & (~a | a ^ a) | (b | b) ^ (b
		| b) | b ^ aux0 ^ a | ~~(a & (a ^ aux0))) & ((~(a ^ aux0) ^
		~(a & b)) & (a ^ ~aux0 ^ (aux0 | aux0)) | ~a)) ^ ~(b ^ (a &
		(b | aux0) ^ (b ^ b | ~aux0) | aux0) & (((b | a) ^ a & b) &
		(b ^ a ^ b) ^ (b | aux0 | a) & (b ^ a & b)))) + 38519 * ~~a
		+ 17087 * ~((~aux0 ^ b | aux0) & ~((a ^ b & aux0 ^ ~aux0 |
		~(aux0 ^ a ^ (aux0 | b))) ^ (~aux0 & (aux0 ^ aux0) ^ b & a)
		& (a ^ (aux0 | aux0) & (b | a)))) + 42481 * (((~aux0 |
		~((aux0 | b) ^ a) ^ b) & b ^ b ^ ~(a & ((b | b) ^ aux0 & b))
		& a & ~~a & (a | aux0 ^ aux0)) & (a ^ ~(b | b))) + 17378 *
		(~~((b & b ^ a & a ^ ~(aux0 & aux0)) & (b ^ aux0 ^ aux0 & a)
		& (b | ~b) ^ ~(~(aux0 | aux0) ^ aux0)) ^ ~((b & b ^ a & aux0
		| ~a ^ a ^ aux0) ^ b | a | a | ~~aux0)) + 61488 * (~((b ^
		aux0 | b | aux0 | a | b) & ~a) & (aux0 | ~((aux0 | a) ^
		(aux0 | aux0)) | aux0 ^ b | (~aux0 | aux0 ^ aux0) ^ (b ^ a)
		& ~aux0 | b) & ~a) + 8358 * (~a | a | ~~(b & b) | b | a |
		aux0 ^ (~(aux0 & aux0 & (a | a)) ^ (~b | a & a) & b ^ (b | a
		| b & aux0) & (b | b | aux0) & (aux0 & b | b) & b | ~(b & a
		& a) | ~a | ~~(b & a ^ a))) + 18814 * (a | ~~(aux0 & ~a & a
		^ aux0) ^ (a ^ ~(aux0 | (b ^ aux0) & (b ^ a)) | ~b)) + 32248
		* ~(~(b ^ aux0 & b ^ (a | b) ^ b & (b ^ a) & (b ^ b)) |
		~((~b | b) & ~(a ^ b) ^ aux0) | (a & a | ~aux0) & aux0 & (a
		| b) ^ (a & a | aux0 & aux0 | a) ^ (a | ~(~aux0 & (a ^ b)))
		^ ~aux0 & (~(aux0 ^ b) ^ ~~a | a ^ a & aux0 ^ (b & aux0 |
		aux0))) + 54666 * (~a & ((~(a & b ^ ~aux0) | ~a & (a & a |
		aux0 & a)) ^ ~((b ^ b ^ aux0 & b) & (aux0 & a ^ ~b)) | (~b ^
		b ^ ~aux0 | b) & ~((aux0 ^ aux0) & (aux0 ^ a)) & ((b ^ b) &
		(aux0 ^ a) ^ b)) & (aux0 | aux0)) + 39131 * ~((~((a | ~aux0)
		^ a ^ (b ^ aux0 | a)) | b) & ~~a) + 37512 * (~(((~(a & aux0)
		| aux0) ^ ~(a & (a ^ b)) | ~(b & a | ~aux0 | aux0)) ^ ~b) &
		(~(a | ~aux0 | ~~aux0) & a | (b & b | aux0) & b ^ aux0 |
		~~~aux0 | ~aux0) & ~b) + 25903 * ((a | b & (~(a | a) ^ b ^ b
		& aux0 ^ a & (~a | ~aux0)) ^ (a | b)) ^ ~(b & aux0)) + 43888
		* (a & ~~((aux0 | b | aux0 | a) & (a | aux0 | b) & b) ^
		(~(~(~aux0 | b & a | ~aux0 & a & a) | ~aux0) | aux0)) +
		18086 * (~~((aux0 | ~(aux0 | b)) & (~aux0 | a | a) & ~(a &
		a) | ~aux0) & aux0) + 31454 * ~~(~(~a & (b ^ aux0)) & (a ^ a
		^ ~aux0 ^ b) | (aux0 ^ b) & b & b | b & (a ^ aux0) ^ ~(aux0
		& aux0) | b & ((b | b) ^ (b | a)) & (aux0 | aux0 | ~aux0) &
		~((b ^ b) & (b | a))) + 55255 * (~(aux0 ^ aux0 ^ (a | b) &
		~aux0 ^ a ^ (aux0 & aux0 ^ ~aux0 | ~a ^ b & a)) ^ a ^ b) +
		44299 * ~((aux0 | (b ^ ~(~aux0 | aux0 ^ b)) & ~aux0) ^
		(((~(b ^ aux0) | aux0 ^ a ^ ~a) ^ ~(aux0 ^ a) & (a ^ a & b))
		& (b & a & ~b ^ a ^ ~~~b) | ~(aux0 ^ a ^ b ^ b | aux0) | b))
		+ 36675 * ((~a | ~aux0 & ~aux0 & ~a & ~(a & b | aux0 | aux0)
		^ (aux0 ^ b & aux0 & a) & ~aux0 | ~aux0 & (b | a) ^ ~a ^ (b
		| a) | ~(aux0 ^ aux0) | aux0 ^ aux0 | aux0 | b) ^ a &
		(~(aux0 | b | b & b) ^ aux0 ^ b) & (~aux0 ^ aux0 ^ (~aux0 |
		aux0 ^ a) & ~(a & a) | ~aux0 ^ a & (~b ^ (a | a)) ^ ~(a | b)
		& ((a | b) ^ (aux0 | b)))) + 17656 * (~b & b & ~(~(aux0 & a)
		& a | b) & (~~(b & a) | ~(b & a) & (b & b ^ b)) & b) + 36419
		* ~(~(aux0 & aux0) & ~aux0 ^ ~~~~(b | b) | aux0) + 25637 *
		~(aux0 ^ ~aux0 | ~(aux0 & b | a | aux0 | a) | ~(~(a | aux0)
		^ a ^ a ^ b & a) | ~a) + 47056 * ~(a ^ aux0) + 52619 *
		(~(~aux0 & b ^ ~(b ^ aux0) ^ ~~(b ^ aux0) ^ ~~(~a | ~a) |
		~(~b & (~aux0 | aux0 | ~(a | b)))) ^ aux0 ^ b) + 53554 *
		(aux0 ^ ~((~~aux0 | aux0) ^ ~((a ^ a) & aux0 ^ (b | a) ^ b &
		aux0) | (a ^ b ^ ~b) & ((a | b) ^ ~aux0) & (~(b | a) ^ (a |
		aux0) & ~a) | (a | aux0 | a ^ aux0) ^ (a ^ b | a & a) |
		~(aux0 | a) ^ (aux0 ^ aux0 | b & a))) + 8318 * ~(b | (b |
		~(aux0 | aux0) | b) ^ (a & aux0 ^ aux0 ^ a) & ~(a | aux0) ^
		~(~b & ~b) | (a ^ ((a | aux0) & aux0 & a | aux0) | ~(aux0 &
		b) & aux0 & ~a) & (aux0 | b & b | ~b | (b ^ aux0) & aux0 &
		aux0 ^ ~(b & a) | a ^ aux0 & a & aux0 & (aux0 | a | ~a))) +
		50785 * (b | ~~(b & b & a & (aux0 ^ b ^ ~b)) & ~b ^ ~(aux0 ^
		~(a ^ aux0) & ~(b ^ aux0) ^ a ^ ~~aux0)) + 35357 * (~~~aux0
		& (b & ~((~(b | a) ^ a ^ aux0 ^ b) & (a ^ b ^ b | ~a)) |
		~aux0)) + 23986 * (~~(~(b ^ ~~aux0) ^ b) & ~~(~~~(b ^ a) ^
		((a ^ a | b ^ b) ^ a & aux0) & (aux0 | aux0 & aux0 | ~b))) +
		40651 * (~(~(((aux0 & b | aux0 & a) ^ ~b) & (b & aux0 ^ a ^
		b)) & ~(aux0 | a) & b) & (aux0 & a & aux0 ^ ~~a ^ aux0 ^ b ^
		~a ^ aux0) & (((aux0 | b) & b | (b | aux0) ^ aux0 ^ a) ^
		(aux0 ^ a | aux0 ^ b) & aux0 & a & b & b | ~aux0 | a))) *
		(19561 + 9115 * ((b | (~aux0 | ~~a ^ (a ^ aux0 | b ^ aux0) ^
		(aux0 ^ aux0 ^ a | (a | aux0) & ~a)) & (~a ^ (b ^ a ^ ~b |
		~b)) & ~(aux0 ^ aux0 ^ aux0 ^ (b | aux0 & aux0))) ^ ~~aux0 ^
		((~(aux0 & aux0) ^ b & aux0) & (a ^ (b | b) & a & a) & ~(~b
		^ aux0 ^ aux0 & a) | b)) + 12262 * ((~(aux0 & aux0 | ~a) |
		(aux0 | aux0) & b & ~(b & b) | ~a) & a & ~((b | a) ^ (aux0 |
		a) | aux0) & ~((b & aux0 & ~aux0 ^ (a ^ b) & (b ^ b) | (aux0
		| a | b ^ aux0) & a) ^ (b ^ b | b ^ b | b) & (a ^ ~a)) & ~a)
		+ 43771 * ~(a ^ ~~(~(a ^ a) ^ b) & b) + 9139 * ((b ^ ~~((b ^
		b | b ^ aux0) & a) ^ ~(a | a)) & ~b) + 565 * (b & ((aux0 & b
		| aux0 ^ a | aux0 ^ ~b) & ((b ^ b | b | aux0) ^ aux0) | ~(~a
		| aux0 & a) & ~(~b ^ ~a)) & (a | b ^ (a | aux0)) ^ (~b |
		~aux0 & ~a & ~~aux0) ^ aux0 ^ (~~a | ~b | ~a) ^ (a ^ b |
		aux0 & b) & a & ~aux0 ^ a ^ ~~a) + 45181 * (~((aux0 ^ a &
		aux0) & (b | aux0 | a & b) & (b ^ ~aux0) & a) & ~((b | a) ^
		b & a ^ (aux0 | aux0) ^ a ^ aux0 ^ ~(b & aux0 ^ a & b)) &
		~(b & ~~~(b ^ b)) | b) + 41916 * (~aux0 ^ ((b | ~(aux0 & a)
		& (b | a | aux0) | (b | b) & (aux0 | aux0) & (b ^ a ^ ~aux0)
		| ~(b | aux0) | b) ^ (aux0 & ~((aux0 | a) ^ ~aux0) | (~b |
		~aux0 | a | aux0) ^ (~(b ^ a) | (aux0 ^ a) & (b | b))) |
		~(~(~b & (a | b)) ^ aux0 ^ a | (b & b & b | ~b ^ (b | b)) &
		(~b ^ aux0 & b ^ (b & aux0 | aux0 | a))))) + 54731 * (~(aux0
		^ (~(aux0 ^ b) ^ (b | a) & aux0) & b ^ ~(a ^ (a ^ aux0) & (b
		| aux0) | a & b & (b | b) ^ a)) ^ (~b | ~~(b ^ a | b ^ b) &
		a & (~(a | b & b) ^ (~(a | b) | ~~aux0) ^ (b & b ^ a ^ aux0
		| ~~a | ~(a & ~aux0))))) + 17540 * (aux0 ^ ~a | b & b | ~~b
		^ (b | (aux0 | b) & (b | aux0) | ~aux0 | (~a ^ a ^ b |
		~(aux0 ^ a)) & (a ^ b & b | (a | a) ^ aux0 ^ aux0))) + 38856
		* (a & ~~((b ^ aux0) & a & a | ~aux0 | ~a) | ~b & (a | b) &
		~aux0 | ~~b) + 55496 * (~(b ^ (~~aux0 | ~(aux0 ^ aux0) &
		aux0) & (a | (aux0 | a) ^ (b | b)) & (a ^ aux0 | b & b | a))
		& b) + 62137 * (a | (aux0 & a & (b | b) & ~aux0 & (a & a | a
		& aux0) & b | (~b | aux0) ^ a ^ ~a) & (a ^ aux0 ^ b ^ aux0 |
		a | ~(b ^ a ^ ~a) & (b | ~~a)) | a | a ^ (~b ^ (a | aux0) ^
		~a & a & a | ~~~aux0)) + 48668 * a + 53854 * ~~a + 60090 *
		(~~a | aux0 & aux0 | a) + 26724 * ((b | ~~(~b ^ b ^ (a |
		a))) & ~(~~((aux0 | aux0) ^ (aux0 | a)) & ~a) | b) + 60173 *
		((a | ~((a | ~b) & ~aux0 & (aux0 | a)) & aux0 & ~~(aux0 &
		a)) ^ ~(~~(a & a) & a & (b | aux0) & (b | a | ~aux0 | ~aux0
		| ~a)) ^ (~(~~aux0 & ~a ^ (a ^ b | a | aux0) ^ (~aux0 | b ^
		aux0)) | ((aux0 ^ a | b & b) ^ a) & (b ^ aux0 ^ ~aux0 ^
		~~aux0) & b) ^ ~~a) + 1646 * (b ^ aux0) + 9868 * ((~~aux0 |
		b) & ((b | ~~a ^ (a | aux0 & aux0)) ^ (a & (aux0 ^ (a | b))
		| b | b & b | aux0 ^ b ^ (aux0 | aux0)) | b) | ~aux0 | ~((a
		| b) ^ (a | b) ^ a ^ b ^ a) & (~~aux0 ^ a) & (a ^ a ^ (b |
		aux0)) & (aux0 | a | aux0 | a) | (~(aux0 & b & a) ^ (~a ^ a
		| ~aux0)) & aux0 & (~(aux0 ^ b) | ~b ^ ~b)) + 24100 *
		(~(~((aux0 ^ a) & (aux0 | b)) | ~~(aux0 & aux0)) | ~((aux0 &
		b & (a | b) | b & ~aux0) & aux0) | (~(aux0 | aux0) ^ (aux0 |
		~b)) & b & (~(aux0 ^ b) ^ ~b & (b | aux0) ^ a & ~(b ^ aux0))
		^ a & a | aux0) + 19374 * (aux0 | (aux0 & a ^ b & b ^ aux0 |
		(~a ^ b & aux0) & (b ^ ~b)) ^ ~(~a & (a | aux0) | ~~b) | ~b
		| ~~(b & (a | b) & (aux0 | aux0 | aux0 | b) | (~b | aux0 &
		a) & (b & aux0 ^ a & a)) | b & ~(a ^ b ^ aux0) | aux0 | b |
		~(aux0 | ~aux0 | ~b ^ b & aux0)) + 17555 * ~((~(aux0 & b |
		aux0 & b | (a | a) ^ b ^ aux0 | b & (a | aux0) | b | b |
		aux0) ^ ((~a ^ ~b | b ^ b | a | aux0) & (a & (b ^ b) | ~(b ^
		aux0)) | (aux0 ^ ~a | ~b | ~a) & (a ^ (a | b ^ aux0)))) &
		((b | a) ^ aux0 & ((b | aux0) & aux0 & (a | b) | ~((a | b) ^
		b & aux0)))) + 62721 * (~a | aux0) + 17132 * ~(~b & (~(a &
		aux0 & b) ^ ~b & a ^ (a ^ b) & (b ^ a) ^ a & (b | b) & (aux0
		^ aux0) & (~b | a)) ^ a ^ ~~b) + 33592 * b + 10604 * ((aux0
		^ a & ~(a & aux0) & ((a | aux0) ^ a ^ a ^ aux0)) & ~(((b |
		~a) & (b | a & a) | ~(a ^ (aux0 | a))) & aux0) ^ ((a ^ a | a
		| b ^ a ^ (b | b)) & (b | b) | (~(aux0 | aux0) ^ (a ^ a | a
		^ b)) & ~b & a & (a ^ aux0)) & (~(b ^ aux0) ^ b & a ^ a ^ a)
		& (~aux0 | ~b | b) & ~((b | a ^ aux0) & ~(b ^ aux0)) & aux0
		& ~((b ^ (aux0 | b)) & (~aux0 ^ aux0)) & (~(~(a & a) &
		(~aux0 ^ b)) | ~(a & ~(b ^ b)))) + 34813 * ~~aux0 + 5026 *
		~~~b + 7367 * (~(aux0 ^ a) ^ ~(a & (~(~a & a & aux0 & b) |
		(~a | aux0 | a | b | ~a) ^ b))) + 27072 * ((aux0 & aux0 &
		aux0 & ((a | aux0) ^ b ^ aux0) & a & a & ~b ^ a | (aux0 &
		aux0 | a | a) & (aux0 ^ ~b) & ~(a ^ b | b | a) & (~(b ^ b |
		aux0) ^ (~(b | aux0) | b)) & (~(~a & aux0) | (a | aux0 |
		~aux0) & (a & aux0 ^ a & aux0) | aux0)) & b) + 14666 * (a &
		(~~aux0 & (a ^ a) & (b | a) ^ (b ^ b ^ ~aux0 | aux0 & (b |
		a)) ^ ((aux0 ^ b) & (aux0 | b) | b | a | b ^ b) ^ ~~a ^
		(~aux0 | a) | ~((aux0 ^ a ^ a & b | aux0 ^ aux0 | a | b) &
		aux0 & ~(aux0 ^ a) | (aux0 ^ a | aux0 & a) & (b ^ aux0 ^ b)
		| (aux0 | a) & (aux0 ^ b) ^ b))) + 51664 * (~((aux0 & a & b
		& aux0 ^ (a | b) & (aux0 | a)) & ~(b & aux0) & (~a | aux0) ^
		(~(b ^ aux0 ^ b) | ~b | ~a | ~~aux0)) ^ a | ~a) + 50530 *
		~((((~(b & aux0) | ~aux0 | b | a) ^ (aux0 | ~~aux0)) & ~(a ^
		a) & (a ^ b | a & a) & (b ^ a & a ^ a) ^ aux0) & b) + 48114
		* ~a + 46542 * ((((b | b) ^ b ^ b | a & aux0 ^ aux0 & aux0)
		^ (b ^ b ^ a | ~aux0 & aux0)) & aux0 & aux0 & aux0 & aux0 |
		~b | (aux0 | a) ^ a ^ b ^ b ^ aux0 & ~b ^ ~(~aux0 & aux0 & a
		^ aux0 & b & ~aux0) | aux0 | ((a ^ (a ^ a) & (b ^ a)) & (b |
		~aux0) ^ a ^ (~~a | b | a ^ (a | b) | b)) & (a & ~~(a | b) ^
		~((a ^ ~a) & (b | aux0) & (b | b)) | ~b)) + 26031 * ~~~~(a &
		(aux0 | aux0) & (~b | aux0 & a) | aux0) + 65080 * ~(a ^
		~(aux0 ^ b ^ ~b & ~a & ~aux0)) + 44004 * (a ^ (~~(~(b | aux0
		| aux0) ^ ~~(a | b)) | a & (b & a | b | b | a) & ~aux0 & ~~a
		^ ((aux0 | (aux0 ^ aux0) & (aux0 ^ a)) ^ ~(b | b) ^ b) & ((a
		| b) ^ aux0 ^ b ^ (a ^ b) & b | (aux0 ^ aux0) & ~b | b |
		~a))) + 26448 * (~b & (a & a | a) & (~(~(~~aux0 & a & b & (a
		| a)) | ~(a ^ aux0 ^ b ^ a) & (a ^ aux0 | b & aux0) & (a ^
		a) & (a ^ aux0)) | (aux0 ^ ~~a & ~(aux0 & a)) & ~(aux0 | ~a)
		| ~(~(a ^ b) ^ a & b & aux0 & aux0) ^ ~a)) + 50624 *
		(~(~(~~aux0 ^ ~b & (a | a)) | ~(a ^ a) & a & b & aux0 & a |
		a | a) ^ (~(a | a & (a | aux0 & a) & (b ^ aux0 | aux0 |
		aux0)) | ~(((b ^ b ^ b) & ~b | aux0 & (a ^ a ^ a)) & a))) +
		37963 * (aux0 ^ b | (~~~(a & (a ^ b)) ^ aux0) & a & (a ^ a ^
		b | aux0 | aux0 | b & a | (b | b) ^ b & b | (aux0 | a) ^
		aux0 | aux0)) + 31409 * (aux0 ^ ((aux0 | ~(~aux0 | b ^ b) |
		aux0) ^ b | ~~a)) + 22285 * ((b | ~~(a | b) | (a | b) &
		(aux0 | a) | aux0 | (b & a & a | a | b | b ^ b) ^ b | a ^ b
		& aux0 & (~aux0 ^ a ^ a ^ (aux0 | a) ^ b) ^ a) & ((aux0 |
		aux0 ^ b) & b ^ (~a | b) ^ (aux0 ^ b | ~aux0) | a | a | aux0
		& ~(b ^ a) ^ (a & aux0 ^ aux0 | b) | ~(((~b | b ^ a) ^ a & a
		& (aux0 ^ b)) & ~a & ~~a) ^ a)) + 13139 * ((~(~(a & a) | ~b
		| ~b) ^ b) & (a | a) ^ (aux0 & ~((~b | aux0 ^ a) ^ b) ^ ~(b
		& ~(aux0 ^ b)) ^ (b & a ^ aux0 & b | (b | aux0) & ~aux0 |
		aux0)) & (~~(a & a ^ a ^ b) ^ ~(b ^ a | b) ^ (b ^ a | a | a)
		^ (b ^ a) & aux0 & b) & ~~((b | b) & a & b) & ~b & ((aux0 ^
		b | b) ^ ~(a & a))) + 30434 * (~((b ^ a ^ (aux0 | a)) & (a |
		~b) & (b | aux0 | b & b) & (a | aux0) & aux0 & aux0 & b &
		aux0) & (~~(~~aux0 | b ^ (aux0 | b)) ^ b) & aux0) + 19877 *
		(aux0 ^ (((aux0 | b | a ^ a) ^ aux0 ^ a) & (b | a | a & aux0
		& (aux0 ^ b)) | a) ^ ((b ^ aux0) & b & aux0 ^ ~a ^ b ^ b |
		(aux0 ^ b ^ ~a ^ (a | a | aux0 ^ b)) & ~(a ^ a ^ b)) ^ b) +
		2816 * (b & a & aux0 & ((~a ^ aux0 & b) & a ^ ~~(aux0 |
		aux0) ^ a ^ (~a | ~aux0 ^ a & a))) + 52590 * (~((a ^ (~(b &
		aux0) ^ a & aux0 & b) & (a ^ (aux0 ^ aux0) & aux0)) & a) |
		((~(aux0 ^ a | b & b) | aux0 & ~a & (aux0 ^ aux0) | (a |
		aux0) ^ ~((b | a) ^ a & b)) ^ (b ^ b & b & (b ^ aux0) | b &
		a & (b | b) | b)) & (a ^ (~(a & a) ^ (b | aux0) & ~aux0 | a)
		^ ~((a | a) & (b ^ a) & ~a))) + 53390 * (a | aux0 | ((~b ^ b
		^ (b | b) & ~b | ~aux0 & ~a ^ ~aux0 & b & a) ^ aux0) &
		~(~(aux0 & b & a) & (~b ^ aux0 & b | b ^ aux0 & a)) | (b ^
		~~aux0 | (~(b | b) | ~b & ~a) & (aux0 & b ^ ~a ^ (a & b | b
		^ aux0))) ^ ~(aux0 ^ ~(aux0 | aux0) ^ ~((b | b) & aux0 &
		aux0))) + 59985 * (~((a | ~b ^ (a | a)) ^ (b | a | a | ~~a)
		^ ~((a & b | a & a) & ((a | b) ^ aux0 ^ a)) | ~a & a) |
		aux0) + 47558 * (a ^ ~((b ^ b | ~aux0 | aux0 & b) & (~b & b
		& aux0 ^ ~(a & aux0))) & (a ^ a ^ a & (aux0 ^ aux0 ^ b & b)
		& a) ^ aux0) + 28746 * (a | b & ~~b ^ a) + 55012 * (~~~aux0
		^ (~(b | aux0) ^ b ^ (a ^ aux0 ^ b) & (a & a ^ aux0 ^ aux0)
		^ (aux0 & aux0 | b | b) ^ (a | a | b | aux0)) & (~~(b ^
		(aux0 | b)) | (aux0 & aux0 ^ ~a | aux0) ^ ~(aux0 ^ aux0 ^
		aux0 & aux0)) & aux0 & (a & a | b ^ a | ~b) & (aux0 & ~a |
		b) & ~(~a & ~b ^ (a & a | ~a))) + 669 * (~(aux0 ^ ((~(aux0 |
		aux0) | aux0) & ~(aux0 & aux0) & ~(b ^ b) | ~b | a)) & ((b &
		~b ^ ~((aux0 ^ a) & a & b) | a ^ ~a | (a | a | aux0 | a) ^
		(a | b) ^ a) ^ a) & ~~((b | b) & (aux0 | a) & (a & b ^ b) &
		(~~aux0 ^ b))) + 7712 * (~((aux0 | b) & b ^ aux0) & (~b ^
		aux0) & b | ~(((b | a | b ^ b) & ~(a & aux0) | aux0 & a |
		~b) & aux0) | a) + 10877 * (((~(a ^ b) | ~aux0 | ~a) & (b ^
		b ^ a) & ~~a ^ ((b | aux0) & ~b | aux0) ^ ~aux0 | b ^ a | b
		& ~~((aux0 | aux0) & a & aux0 | b ^ aux0 | a & a)) ^ (~(a |
		aux0 | a & aux0 | (a | a) & (aux0 | aux0) | (~a ^ aux0) & (a
		| aux0 | a)) | aux0 ^ (a | b & aux0 | ~(aux0 | aux0) & (b ^
		~a)) | ~(~aux0 | b) & (((b ^ b) & aux0 | b) ^ b ^ ~(a ^ a))
		| aux0)) + 65226 * ~(((a ^ a) & b & b ^ a | ~a | (b & a ^
		~a) & (b & aux0 | a) ^ ~(b | a) ^ (b | a ^ b)) ^ ~aux0 |
		(aux0 | (aux0 | aux0 | aux0 & aux0) & b & b & aux0) ^
		~((aux0 | b) ^ ~b | aux0) ^ a) + 2505 * (~((a | ~(~(a |
		aux0) ^ ~a ^ aux0 & aux0)) & a & (~(aux0 ^ aux0) | ~(b | b))
		& (b & aux0 & aux0 ^ ((aux0 | aux0) ^ a & aux0 | a))) ^ (b ^
		a) & (a & (~a ^ (aux0 | aux0)) & (a | aux0) & b & a | (aux0
		| (aux0 | a) ^ a & a) ^ aux0 ^ ~(aux0 | aux0)) & (a ^ b)) +
		43687 * (aux0 & ~~~(a & b & (a | aux0) & (b | aux0) & (b ^
		a) ^ ~(aux0 & a))) + 37447 * ((~((~((a ^ a) & b & a) | ~(b &
		aux0) | ~(a | b)) ^ (a | b) & ~aux0 & (a | b) & a ^ ~((aux0
		| a) ^ b)) | aux0) ^ aux0 & ~(aux0 ^ (b | aux0 & aux0 ^ b |
		b ^ b | a | b))) + 30323 * ((b & ~((aux0 | ~aux0 & a & b) &
		a) | (b | ~(aux0 ^ aux0 ^ a | (a | aux0) ^ aux0 & b)) ^ aux0
		& (~(aux0 ^ aux0 ^ a) ^ (b & aux0 ^ a & aux0 | a | a | b ^
		b))) & (b ^ ~~(a ^ a | b & aux0) & (a | a) ^ ~~(b | ~aux0 ^
		aux0 ^ a) & aux0)) + 48363 * ((b | ~(~aux0 & (~(b ^ aux0) |
		(a ^ aux0) & b)) | ~((aux0 ^ (b ^ aux0) & (a ^ a) ^ (~(aux0
		| a) | aux0 | aux0 | ~aux0)) & (a ^ b))) & ~(aux0 & (~~a ^ a
		^ (a | aux0)) | (~b | ~~a) ^ ~b ^ ~aux0 ^ b & aux0 ^ a & a |
		((aux0 & b | aux0 ^ a) & ~(aux0 & a) | b) & ~((aux0 & aux0 |
		a ^ aux0) ^ (b | b) ^ (aux0 | aux0)))) + 37165 * ((a ^ a) &
		(~(a & aux0 ^ ~aux0) & (aux0 ^ aux0) ^ ~(~a & (b ^ a) | a &
		(aux0 ^ b))) & (~b | (a ^ (a | aux0 | a | ~b)) & aux0) & b)
		+ 25905 * (~a ^ ((~((aux0 ^ aux0) & a) | a & a ^ a & a | b |
		b) ^ (~(aux0 & aux0) | b ^ aux0 ^ a) & ~((a | aux0) ^ b)) &
		aux0 & b ^ ((aux0 | b | ~(a | aux0)) ^ a) & ~((b | aux0 | a
		& aux0) ^ aux0 | ~~(aux0 & aux0))) + 22487 * (aux0 | aux0 |
		a | b | ~((b | aux0) & (aux0 | a) | aux0) | ~(b ^ a) ^ aux0)
		+ 36571 * (~~(a | ~~b) | aux0) + 29204 * ~(~(~(aux0 ^ aux0)
		& (aux0 ^ a) & (aux0 | aux0)) ^ (b | ~(b | a)) ^ ~(b ^ b) ^
		b ^ aux0 ^ ~b ^ ~(b & b | ~aux0 | ~(b & b)) & (aux0 | a &
		(aux0 ^ b)) ^ aux0) + 37675 * (~(~(b & aux0 ^ ~aux0 ^ a ^
		aux0 ^ a & a) ^ b) ^ ~a ^ (a | aux0 & a & aux0 & ~(aux0 ^ b)
		| ~(aux0 ^ a) ^ aux0 & a & aux0 | (a & a & a | aux0) & ~(a &
		aux0 & ~a) | ((a ^ aux0) & (b ^ b) ^ ~(a ^ b) ^ ~aux0) & ((a
		| ~a | aux0 ^ a) ^ (~~a | aux0 & a ^ ~aux0)))) + 20630 *
		(~(((~(aux0 & aux0) | ~a | a & aux0) ^ a & (a ^ aux0) &
		(aux0 & a | b | a)) & a) ^ ~a ^ (~((aux0 & a | b & a) & b &
		(aux0 | b)) & (b ^ b ^ a ^ ~a ^ a & (a ^ aux0) ^ (a ^ a |
		b)) | aux0 ^ (a ^ a ^ (b | aux0) | ~b & a) & (a & b ^ (b |
		aux0) | a | a | ~b) | ~(aux0 & ~a) ^ a ^ ~(~~(b | b) ^ aux0
		^ aux0 & aux0 & ~aux0))) + 42348 * (b & (a | ~~(a & b & (b |
		a) & ~a & ~b) & b)) + 55391 * ~~(a | ((a | aux0) ^ (aux0 |
		aux0)) & ((a | aux0) ^ a & a) | ~(~a ^ b & a) & b & a & (b |
		b | aux0 | a) | a | aux0 | ~a & a & a & a) + 45782 * (~(a &
		b & (aux0 ^ b)) & (aux0 | a ^ aux0) & (~b | aux0 ^ b) |
		~aux0 & ((b | aux0) ^ (a | aux0)) & (a | b ^ a) | aux0 |
		(aux0 | aux0 | aux0 & a | b ^ aux0 | a) ^ a | ~((a ^ b |
		aux0 | b | b & a & (a ^ aux0)) & b) | ~(~a | aux0) & a) +
		56379 * (aux0 ^ ~(b ^ ~(a | b | a ^ a | aux0 | ~a) ^ a)) +
		39380 * ((~a | aux0 ^ aux0 ^ (a ^ aux0 | aux0 & a) & (b ^ a
		^ b & aux0) & (~(b | a) ^ ~(aux0 | aux0)) & ((aux0 ^ a) &
		(aux0 ^ a) & aux0 & a | ~b)) & ((~(b ^ b | a) & (a | aux0 |
		a & b ^ b ^ a) | ~aux0) ^ ~(aux0 ^ b | b ^ aux0) & (a ^ b) &
		b & (~aux0 | a | a) & ~b & (a | ~(a ^ aux0))) & (~(~~(aux0 &
		aux0) ^ aux0) | ~~(aux0 & b))) + 16874 * ~(b & aux0 & (~~(a
		| a) | (b | aux0) ^ a | ~(b | aux0) | (~(aux0 & aux0) ^
		aux0) & ((a | aux0) ^ aux0 | (b ^ aux0) & aux0)) | ~((~a |
		b) ^ ~(~a | aux0 & aux0 | ~(b & a)))) + 30176 * ((~(~(aux0 &
		b) ^ ~~a ^ aux0) | ~~(a & a ^ ~aux0) ^ aux0) ^ ~~((a ^ a) &
		aux0) & ((a | b | ~aux0) ^ (b | aux0) ^ ~a) & (~(b ^ a) ^
		~(a ^ aux0)) ^ (~((aux0 ^ aux0 | aux0) ^ (b & a | b & a)) |
		~(~(a | aux0) ^ b)) ^ a ^ (aux0 | ~a) & (a | (aux0 ^ aux0 ^
		(b | a)) & ~a | ~b)) + 9727 * ((~(~~(aux0 | b) ^ (a & b & ~a
		| aux0 ^ (a | aux0)) | b) | ((~(a | aux0) | ~aux0 | aux0 &
		a) ^ a | b) & ~aux0 & (a ^ aux0 ^ aux0 ^ (aux0 | b) ^ (aux0
		| b)) & ((aux0 ^ b) & a ^ (~a | ~a))) & ((~(a ^ ~aux0 & ~b &
		(aux0 ^ b ^ aux0 & a)) | a & (aux0 ^ b | a ^ b | aux0) |
		~((b ^ a | a & b) ^ (a ^ b) & b & a)) ^ (~((aux0 | aux0) &
		(a ^ aux0) & (aux0 ^ a ^ (b | aux0)) ^ ~(aux0 & b) & (b & b
		^ (b | b))) | aux0 & (a | ~~a | aux0 & a ^ b)))) + 16283 *
		((aux0 & (~a ^ a) ^ aux0) & (aux0 & b ^ a ^ a | b ^ aux0 |
		aux0 & a) & (b ^ b) & b & aux0 & b & ~((b | aux0 ^ a) & a &
		aux0 & ~a ^ (a & a & (a ^ a) | a | aux0 | ~b)) | ~aux0 |
		(~(a ^ aux0) & ~~a | a ^ (b | a) ^ aux0 ^ a | b) ^ a | (b ^
		b ^ (aux0 | b) | ~(aux0 | a) | aux0 & (aux0 & aux0 ^ b)) ^ b
		^ ~(b & aux0) ^ (b ^ a) & (aux0 | b) ^ ~b ^ a ^ a ^ a) +
		63468 * (((a ^ b & b ^ a ^ ((aux0 | aux0) & (a | a) | a ^ a
		| a | aux0)) & aux0 & ((aux0 | a) & (b ^ a) ^ a | a | (a |
		b) & (a ^ a) & b ^ ~(b ^ aux0) & aux0 & aux0 & ~aux0) | ~((b
		^ aux0 | aux0 & a) & b | (aux0 ^ b | b ^ aux0) ^ (b | aux0 |
		a) | aux0)) & ((a ^ a ^ aux0 ^ aux0 ^ a ^ (a | a) ^ aux0) &
		(~(a | b | ~aux0) | ~a & b) | (~aux0 | (a | a) ^ ~aux0 | ~b
		^ (aux0 | a)) ^ ~(~b & (a ^ b) | a & aux0 & (a ^ a))) & (((b
		| aux0) ^ (aux0 | aux0) ^ b) & b & ((b | a ^ b | (a ^ a) &
		(aux0 | aux0)) ^ b) | ~(a & b) ^ aux0 ^ ~(b & b | ~aux0) ^
		(~a ^ aux0 | ((b | b) ^ (aux0 | aux0)) & (b ^ aux0 & a)))) +
		17659 * (b & (~(b & (b & a & ~aux0 & (a ^ b) & ~aux0 | ~a))
		| ~(~b & (aux0 ^ aux0)) & (a & b | b ^ aux0 | ~(aux0 & a)) |
		aux0 & (~b ^ ~aux0) ^ (aux0 ^ a) & aux0 & b & (a ^ a) & ~a |
		(~b | aux0 | ~a & (aux0 ^ a)) & aux0 | b)) + 64057 * (~(a &
		~aux0) & (((b | a | a ^ a) & aux0 | a) & (b ^ b) | b) | b ^
		(~(~(a | aux0) ^ (~aux0 | b ^ b)) ^ (aux0 | aux0 | aux0 ^ a
		| aux0 & ~a & ~~a) | ~(a & aux0) & ~(a & a | b) & (a & aux0
		^ a ^ aux0 | ~aux0))) + 50376 * (((b | a & aux0 & (aux0 ^ b)
		| ~b ^ a & b | a | aux0) & (~(aux0 & b ^ a | (aux0 | a) ^ (b
		| aux0)) ^ aux0) ^ b) & ((~~(b | a) | b) & (~(a | b) | ~~a |
		b) & ~(~((aux0 ^ aux0) & (a | aux0)) & ~(aux0 ^ b | a ^
		aux0)) ^ b)) + 57596 * (aux0 | a & (~(a | aux0) & (a ^ a |
		aux0 ^ b) | (b ^ aux0 ^ b) & (~aux0 | a ^ b)) & ~a & ~~(b &
		(aux0 & a ^ ~b | a | a | b | aux0))) + 59650 * (aux0 &
		~((aux0 & b | ~b | a) ^ aux0) & ((~(a | a) | a) ^ aux0 |
		aux0) & (aux0 | a ^ (~a ^ aux0 ^ (~a | b & a) ^ (a | b) |
		~~(b & b) | ~~aux0))) + 54018 * (aux0 & aux0 ^ a) + 61633 *
		(a & ~(((b | aux0) & (b | b) | (aux0 | a) & (aux0 ^ aux0)) ^
		~~(a & b) | ~~(aux0 | aux0 ^ a)) & (b | ~(~a | a ^ a) & ((b
		| aux0 | a & a) ^ a ^ aux0 ^ (b | aux0)) | ~a)) + 18877 * (b
		& ~(~(aux0 & a ^ b) & aux0 & (~a | ~b & ~a & (~aux0 ^
		aux0))) | aux0 & ~(b ^ b ^ a ^ b | b | ~(b | b) ^ a |
		~~((aux0 ^ b) & b & a))) + 135 * (((aux0 | ~a & b & a | aux0
		& aux0 ^ ~aux0 | (b ^ a ^ ~a) & ~(b ^ a)) ^ aux0 ^ b | b) &
		~b) + 24438 * ~(a & ~((a ^ b | b | aux0) & ~(a & a) & (b ^
		aux0 | a ^ b) & ~(a ^ b)) & ((aux0 & aux0 ^ b & a ^ aux0 & a
		^ b & a | b) ^ aux0) & (b ^ (aux0 & aux0 ^ ~a) & ~(b | aux0)
		& (aux0 | a | b) & (a | b | b | aux0))) + 58774 * (aux0 |
		~~(~(~aux0 | aux0 | a) & (~~aux0 ^ aux0 ^ a) ^ b & (~a ^
		(aux0 & aux0 | b)))) + 7862 * aux0 + 1644 * (a ^ ~((a | b) ^
		b)) + 18692 * (((a | ~(b | a ^ aux0)) & ((aux0 | b) ^ (b |
		b) ^ b | aux0 & a & (b ^ aux0) & (b & a | b)) | ~a) & ((~b ^
		(a ^ b | aux0 ^ b) | a | a & a | ~(aux0 ^ a)) ^ (b ^ aux0 |
		a) & aux0 ^ ~(b | aux0 | a) | ~(b | ~(b | aux0 | a))) |
		~~(~(b ^ aux0) ^ b & a & (b ^ aux0) ^ (a & a | ~a) & (a ^
		aux0) & ~aux0 | aux0)) + 49303 * ~(b ^ ~b ^ b) + 33655 *
		(~((b | ~b & a & ~(a | a) & (aux0 | (a | b) ^ ~b)) & ~b) | b
		& b) + 21868 * ((~aux0 ^ (a | b) ^ ~(b ^ aux0)) & (~(b & ~(b
		^ a | ~a)) ^ a) | (a ^ ~((a ^ b | ~b) ^ ~(aux0 ^ aux0)) ^
		~((b ^ b ^ aux0 | aux0) ^ (b & aux0 | a ^ b) & (b | a |
		aux0))) & ~(~(aux0 & ~b) & b) & (a ^ ~(b & aux0 & ~b)) & b)
		+ 55564 * (aux0 | ~(((aux0 | a) ^ ~b) & (~b ^ ~a) & b &
		(~(aux0 ^ b | aux0) ^ ~b) & (a ^ a & (aux0 ^ aux0) & a & a &
		a))) + 46864 * (a ^ ~(b & ((b | ~b & aux0) ^ a ^ (aux0 ^ b ^
		b ^ b ^ ~(a ^ a) | ~~(b ^ aux0))))) + 17517 * (~~aux0 | a |
		(~aux0 & ~a | ~a) ^ aux0 & b & b ^ aux0 | a & b | (~~(aux0 &
		b) ^ ~(aux0 | aux0) ^ ~a ^ ~aux0) & ((~aux0 | aux0 ^ a) &
		(aux0 ^ b ^ a ^ b) ^ ~a) ^ (~(b | b | a) | a | aux0 | aux0 &
		a | a) ^ ~(a ^ b) ^ (aux0 & a | a ^ b) ^ (aux0 | a) | ~(a &
		(~aux0 | b & aux0 & a) | a ^ (b ^ a | aux0 | aux0) & (a & b
		^ aux0 ^ b))) + 52666 * ((a | b | ~aux0 | (aux0 ^ b) & a &
		aux0 & (b ^ b | a)) & ~~(aux0 ^ aux0 ^ ~b) & a ^ (aux0 |
		~(~aux0 & a & b) | ~(aux0 & a & aux0 & a)) & a ^ ~(~(b ^ b &
		b ^ aux0) & (~a ^ (a | aux0) | b | aux0 & b & a & aux0 | (b
		| a) ^ ~aux0) & b)) + 65407 * (a ^ (~(~b | ~aux0 | aux0 ^ a)
		^ ~~(b | a) ^ ~(aux0 & aux0 & a)) & (~(a ^ (aux0 ^ a) &
		~aux0) ^ ~a) | ~a & (aux0 & a & (b | b) & ~(b ^ a) ^ aux0) &
		(b ^ ~(a & b) ^ ~aux0 & (a ^ (b | b)) ^ (aux0 | b | ~a | ~b)
		^ aux0) ^ (aux0 & (a ^ aux0 & aux0) & (aux0 & a | aux0 & b)
		^ ~a ^ b | (aux0 ^ a ^ aux0 ^ aux0) & b & aux0 & (b ^ a) &
		(a | aux0) | (a & b | a & aux0) ^ ~aux0)) + 43559 * ~((a ^
		aux0 ^ b | b | b) & ((b | a) & a & aux0 | aux0 | ~(b | aux0
		| aux0 ^ b) | (a ^ aux0) & ~(b ^ a) & ~aux0) ^ (a | aux0 |
		aux0)) + 54776 * ((b | b & ~~b & (~aux0 | a & a) & ~~aux0 ^
		~(a & aux0) & (a | (b | a) ^ a & a | b & b)) & (a | aux0) &
		(b ^ (~b | a ^ aux0) ^ b | ~(a & ~aux0) | ~((aux0 | a) ^ (b
		| b))) & ~(~a & ((a & aux0 ^ ~aux0) & ((a | aux0) ^ (aux0 |
		b)) ^ a & a & a & (b | aux0 | a ^ aux0)))) + 35203 * (~(~~(b
		^ aux0 ^ (b | aux0)) & ~(b | ~aux0 ^ aux0) & (~(((aux0 |
		aux0) ^ a & aux0) & b) ^ ((a ^ a) & (aux0 ^ a) ^ a & a ^ b ^
		a) & (b & a ^ b & b ^ a))) | ((aux0 ^ (a | aux0) & ~aux0 |
		(~aux0 | ~b) & b & b) & b ^ (a | ((aux0 | a) ^ a ^ aux0) &
		~aux0)) & ~(~((b ^ b) & (a ^ b)) ^ ~(aux0 | aux0) ^ ~aux0 &
		a & aux0) & (~a | b)) + 4731 * (~(aux0 ^ (~a | a & b | ~b |
		aux0 | a | a) ^ (b | ~a)) | (b & a & a & b & ((b ^ b) &
		(aux0 | aux0) ^ aux0) | ~(aux0 | a | b ^ aux0 | ~(b & a))) &
		(b ^ a & (~a ^ (b | b) | a ^ a | aux0 & b)) & (~b ^ a ^
		(aux0 & a ^ b ^ b) & ~aux0 & (a | b) ^ (a | aux0 ^ a | b &
		aux0) ^ ~(b ^ b ^ ~aux0)) & ~~aux0) + 57631 * ~(aux0 ^
		(~aux0 | b & aux0 & (aux0 | a)) & b | aux0 & a & (b & aux0 &
		~aux0 | (a | aux0) ^ b & aux0 | a) | aux0) + 43607 *
		~(~~~(~(b | a) & (b ^ b)) | aux0) + 8478 * (a & ~(b | a) &
		~((b | ~~(b | a | b)) & b)) + 52430 * (~~a & (a | b | aux0 &
		aux0) & ~a ^ ~aux0 ^ (aux0 ^ (aux0 | b) & a & aux0 & aux0 &
		aux0 & b) & (b & (a ^ b ^ a ^ aux0) | aux0 & aux0 & aux0 |
		a) | ~(b ^ (a | aux0) | ~(aux0 ^ a)) & a & ((b | ~a & (b ^
		b)) ^ ~aux0 | aux0) | (~b | a) ^ (b ^ (b ^ b & aux0) & (aux0
		& aux0 ^ a ^ aux0) & ~(a ^ a) & b | (~(aux0 ^ b ^ (aux0 |
		b)) | (~a | aux0 ^ b) & ((a | a) ^ (b | aux0))) ^ aux0 & (a
		^ ~(a & aux0)))) + 20334 * ~((a ^ ((b ^ b) & ~aux0 ^ b &
		aux0 & (aux0 | aux0) | (a ^ a ^ ~a) & ~aux0 & ~a)) & (~((a ^
		b ^ ~aux0) & ((b | a) ^ aux0)) | (~(b & b) | a & (aux0 | a))
		& (aux0 & b & ~aux0 | ~b | b | a)) & ~~b) + 24985 * (~b ^ (b
		^ a) & ~a & (~(~aux0 ^ b ^ (~b | aux0 ^ a ^ a & aux0)) ^
		~~b)) + 25584 * ~(a & ~~~(aux0 ^ aux0) & (aux0 ^ a & a & ~b)
		& (b ^ a) | a & (b & aux0 ^ aux0 ^ ~aux0 & a | a & aux0 ^ (b
		| a) ^ ~(aux0 | aux0)) & a) + 49181 * (~(~((b ^ aux0) & b &
		a) | aux0 & (b ^ b) | aux0 ^ aux0 & aux0) & ~((~aux0 | aux0
		^ aux0 | ~a) & (b ^ b ^ (b | b)) & ((a | a) ^ ~a)) & aux0 &
		(a ^ ~~aux0)) + 61702 * (((~~((b | b) ^ a) | (~(b | aux0) ^
		a ^ a) & (a | b ^ b) & (aux0 ^ ~b)) ^ ~~((b ^ b) & ~aux0 ^ a
		& a & a)) & (b | ~(aux0 & ~(b & aux0)) | ~aux0 ^ (~b ^ (aux0
		| b) | (aux0 ^ a) & (a | a)) | b) | ~~((a ^ a | a | a & aux0
		^ ~a) ^ b & a & ~aux0 ^ ~(aux0 | a)) & a) + 2433 *
		(~~~((aux0 | aux0 & a & ~aux0) & (aux0 | b) & aux0 & ~(aux0
		| a)) ^ aux0) + 19984 * ((aux0 | aux0 ^ ~(a & aux0) | ~~(~a
		| aux0 & a) & ~~(a ^ a ^ aux0 ^ b) & aux0) ^ b ^ ~~(~(a &
		aux0 ^ aux0) | a & ~b)) + 10674 * ((aux0 ^ ~a & aux0) & aux0
		| ~(a ^ ~((b & b | a) & (aux0 & a | aux0 & a) | aux0 ^
		~~a))) + 18250 * ((b | aux0) ^ a) + 41157 * ~(aux0 & ~aux0 ^
		b & (~(b ^ ~a) & aux0 ^ (~(b | aux0 | a & aux0) | ~(b &
		aux0) | a))) + 5518 * (~(((b ^ aux0) & ~a | ~b ^ b ^ aux0) &
		(~b ^ a ^ a) & (b | a | b | aux0)) & ~b & (~(aux0 ^ ~a & b &
		a & (aux0 | a)) ^ a & ~aux0) & (~(a & ~~(~aux0 & a & b)) ^
		(a | ~~aux0))) + 34239 * (((b | ~~~~a) ^ ((a | a) & ~a |
		aux0 | b | b | b | ~(aux0 & a | a ^ b | ~b & (a ^ aux0))) |
		~(((~(b & b) | aux0 & a ^ aux0 ^ aux0) ^ (a & (aux0 ^ aux0)
		| aux0)) & (b ^ (a ^ b) & (a | a) | (~a ^ b ^ a) & (aux0 ^
		a) & (b | b)))) & ~((~~a | b) ^ (aux0 ^ b) & a & (a | a) ^ b
		| aux0)) + 48112 * ((~((aux0 | b) & b & aux0 & ~a ^ aux0) ^
		(~(b & ~aux0 & aux0) | ~~(b & b & (b ^ aux0))) | aux0) &
		(~((aux0 | a) ^ (b ^ ~a ^ ~(a | aux0)) & (b ^ ~a)) | (~~aux0
		^ (a | b | aux0 | ~a & aux0 & aux0) ^ ~~(a | a | aux0)) &
		((~aux0 | b) & a & ~(aux0 & a & ~aux0) | b & (~~b ^ b ^ a ^
		b)))) + 25586 * (((aux0 | (b | a) & ~a & (b | b) | ~b) & b &
		~b ^ ~((~a & ~aux0 | aux0 ^ a | aux0 | a) ^ (b | ~a ^ ~a)) ^
		(~((a & aux0 | b | aux0) & (aux0 ^ a) & aux0 & b) | ~~(aux0
		| b) ^ ~a)) & ~((~aux0 ^ (b ^ aux0 | aux0 ^ aux0) ^ a | (a ^
		(b | aux0 | aux0 & a)) & ~(b & aux0 | ~a)) ^ (a | ~aux0))) +
		31262 * ~b + 34371 * ((~b ^ (b & ~(b ^ aux0 ^ ~aux0 | aux0 ^
		a ^ a) | b)) & ~((a & b & (a | b) | b & b & a) ^ b ^ ~(~b &
		b) & ~(aux0 | aux0 ^ aux0)) & ~(aux0 & b & ~~~(b ^ a))) +
		53379 * (~(~(~(a & a) | a ^ a ^ ~a) ^ aux0 & ~(aux0 | b) ^
		b) & a & (b | ((b | ~b) ^ ~(b ^ b) ^ b) & a ^ ~~(aux0 | a |
		b) ^ b ^ a ^ (~aux0 | a) ^ (a ^ aux0) & b & b)) + 7561 * (b
		& ~(aux0 & a & ~b)) + 4653 * (a ^ ~(~((a ^ a | a) & (a ^
		aux0 | aux0 ^ b)) & (~(aux0 ^ aux0) ^ (~aux0 | aux0 | b)) &
		a ^ (a ^ (a & aux0 ^ aux0 ^ a | a & (b ^ a)) | ~~(b & b | a
		| a)))) + 21433 * (~aux0 ^ ~~((a | a) & a & a ^ aux0) ^ a ^
		b | b & ~((~(b | aux0) | aux0 & a ^ b & b | b | ~(a | b)) &
		~(b & a))) + 8230 * ~~(b | (a ^ a ^ aux0 ^ aux0 ^ aux0 & b &
		aux0) & aux0 | ~a) + 24819 * ((aux0 | ~b | aux0 & a) & (~a |
		aux0 | aux0) & b & aux0 & (a ^ b) & ((~(aux0 ^ b) | a ^ b) ^
		a) & (~(b ^ a) | a) & (b | (~(b ^ aux0) | ~a & b & b) ^ (b |
		a) & ~a & aux0 | ~a ^ a ^ a ^ ~b | aux0 ^ b & aux0 & a) ^ a
		^ aux0 ^ ((b ^ b ^ (a | aux0)) & (a & aux0 ^ (a | aux0)) &
		~(b | aux0 | a) | (b | a) & (a | aux0) ^ a ^ b & b) ^ a) +
		59888 * ~((aux0 ^ ~aux0) & (~((aux0 ^ aux0) & ~a & b) ^ b) |
		~(~(~b & ~b & (~a | ~a)) & ~b & (b & aux0 | b | aux0 ^ a &
		b))) + 12828 * (a & ~(aux0 & a)) + 42169 * (~a & (b | (b & b
		^ (a | b) ^ b | b) & b & b)) + 13912 * ~(~(~aux0 ^ ~a ^ (~b
		| ~b) ^ aux0 | b & b ^ ~a | ~a & aux0 | b) | b ^ ~(b | aux0
		| a ^ b | ~a & (aux0 | a)) & aux0) + 56085 * ~(a ^ ~b) +
		50910 * ~((~b & (b | ~a) | ~(b | b) ^ aux0 & b ^ (b | a)) &
		~((b ^ b) & aux0 & a & ~(b ^ a)) | ~(~((a | b) ^ b ^ b) ^
		~~~a) | b | ~b) + 48556 * ~~b + 23808 * ((a ^ ~(b ^ (a | b)
		& ~a & (aux0 ^ a ^ b))) & ~(~(~b & a ^ ~aux0 & a) ^ ~b) |
		(~(a & aux0) & ~~aux0 ^ b ^ a | (~a | b | b) ^ (b ^ a | b ^
		a) | b ^ (a | aux0) | b & aux0 ^ aux0 ^ aux0) ^ (aux0 | ~(a
		& a | aux0 ^ b)) & (~b ^ aux0 & a) & aux0 & (a & b | a | b)
		& (b ^ aux0 | a) | ~~(~b & (aux0 | a | aux0) & (b & b | aux0
		& aux0))) + 8760 * (aux0 | (b & aux0 ^ aux0 | ~aux0) ^ aux0)
		+ 26436 * (~b & (~((~aux0 | b) ^ b) ^ a) & a) + 31524 *
		(~aux0 & ((~a | aux0) & (b ^ aux0 & b) ^ ~aux0 ^ a & b ^
		(~(b & a | b ^ aux0 | ~aux0 | b) | ~(b ^ ~aux0) | ~~(b |
		b))) | (a | ~b | ((a | a | a | a | a) ^ (~aux0 ^ ~b | (b |
		b) ^ a)) & ~(~(aux0 & b) | a & b ^ b)) & aux0) + 29759 *
		(aux0 & (~(b | a | a | a) | ~a) & a & a | (~(b ^ (b | a)) &
		a & (a ^ a ^ ~b) | a | ((b | aux0) ^ b ^ aux0) & a) & (~a |
		a | ~(a | a | b)) ^ b) + 53022 * (aux0 | ~(a | b & aux0 & (b
		^ aux0) | a | aux0) | b) + 25523 * (a & b) + 57494 * (b & b)
		+ 40462 * (~(~~a & ~b | (~aux0 | ~a) & (aux0 ^ aux0 & b) &
		~~aux0 & (~aux0 | ~a) | a & a & (b & b | aux0 ^ a)) | aux0)
		+ 54743 * (((b | aux0 & a) ^ (~~aux0 ^ ~~(a | a) | b ^ ~~a ^
		b & b ^ ~aux0) & ~(((b & a | aux0 | aux0) ^ ~(a | aux0)) &
		~aux0)) & (~(~a & (aux0 | a) & (a | aux0) & aux0 & b) ^ a |
		((aux0 | b) ^ aux0 & aux0 | b ^ aux0) ^ aux0 & (~aux0 ^
		(aux0 | aux0)) ^ aux0 ^ a ^ (b | a) ^ a & a & (a ^ b) ^ ~b &
		b | a | ((~a | a | aux0) & a | (aux0 | aux0 ^ a) & ((b | a)
		^ aux0 & a)) & b)) + 63780 * ((~(~~aux0 ^ (aux0 ^ b |
		~aux0)) ^ ((b | b) & ~a | aux0) ^ (b ^ aux0) & ~b ^ aux0 ^ b
		| ~aux0) & ((b ^ aux0 | a) ^ ((a | a | a ^ b) & aux0 | aux0
		| ~(aux0 & b & aux0 & b ^ b)) & b)) + 56335 * (~(~a | (a ^
		aux0) & ~aux0 | a ^ b | ~b | ~(a & b) ^ ~(b | b)) ^ aux0 &
		~(~(aux0 ^ aux0 | ~b) & aux0) | ~((b & (a ^ b | a) | ~~a &
		(aux0 ^ ~a)) & ~~aux0) & ~(a | ~(aux0 & ~(a | a)))) + 60312
		* ~(~~((aux0 | ~~b) & b) ^ (aux0 | ~(((aux0 | aux0) ^ a & a)
		& (a | b | b & aux0)) ^ aux0)) + 23941 * (a | ~~~~((aux0 ^
		aux0) & (b | aux0)) | aux0 & aux0) + 16268 * ((b ^ ~(b ^ (b
		& b ^ (a | a)) & b) ^ (~(a | aux0) | a)) & (~(a & b ^ b ^ (b
		| b) ^ ~~aux0) ^ a ^ ~a)) + 27462 * (~(aux0 & ~b | ~(((a |
		aux0) ^ ~(aux0 & b)) & b & ~(a & aux0))) ^ aux0 ^ aux0) +
		62707 * (~aux0 & (a ^ ~(aux0 & aux0 ^ a ^ (~a | ~a)) ^
		(~~~(b | a) ^ (b & (a ^ b) ^ a & ~b | (b ^ aux0) & (a |
		aux0) & a & a & (aux0 ^ aux0))) & (b ^ (aux0 ^ (aux0 | b)) &
		(aux0 & a ^ a ^ a) & b))) + 19851 * ~~((~(aux0 | a) ^ ~a) &
		(a | b) & ~a & (aux0 ^ a | b | a) & aux0 & a & aux0) + 20849
		* (((a ^ aux0 ^ b ^ ~a | ~b | a) ^ (a & ~aux0 & b & aux0 ^
		~(b | a ^ b)) & a) & a & (a | ((~(b | a) ^ ~(aux0 ^ a)) &
		(aux0 ^ b) & (aux0 | a) & (b | b | a) | (b | a & (a | a)) &
		a & (aux0 | aux0) & (b ^ aux0)) ^ ~a)) + 60617 * (b & aux0 &
		~~(~((b | aux0) & (b ^ a)) & ((aux0 ^ b | aux0) ^ a))) +
		13501 * (~b | ~((aux0 ^ b) & b) & (aux0 | b) & ((~b | a) &
		~(a | aux0) | ~b & (a | b) ^ a & b & ~b) & (aux0 ^ ~(a & a)
		| aux0 & (~b ^ aux0 ^ aux0) | ~a ^ (aux0 ^ aux0) & (a | a) ^
		((a | a) ^ aux0 & a | ~b ^ ~a)) | ~~(b | ~(a ^ aux0 | a))) +
		54403 * (((aux0 | ~~(b | b) ^ ((b | b) & b & a | ~b | aux0 ^
		a)) ^ a ^ b) & ~(a ^ (~((b ^ a) & ~a) ^ aux0 ^ a | ~aux0 &
		~~(a & a)))) + 47607 * ((~(~b & ~~b) | ((a ^ aux0 ^ aux0 ^
		aux0 | aux0 ^ a ^ (b | a)) ^ (a | b & aux0 | a | aux0)) &
		((a | a) & b | b ^ b ^ a) & ~(~a & ~aux0) ^ (aux0 | ~(~aux0
		^ ~b) ^ b)) & a) + 5330 * (~(b & (a & (~a ^ a ^ aux0) &
		(aux0 ^ b & aux0) | ~~((b | b) ^ a ^ a))) & aux0) + 41473 *
		(aux0 | (~(~a & a & b | ~~aux0) ^ (a ^ b & (b ^ a)) & ~(~b ^
		a & aux0) ^ ~(~(b & b) ^ aux0 | b)) & (((a ^ a) & ~b & (a |
		a) & b & b | a & ~a | ~b | aux0 ^ b) ^ (~b | a & a) & ~(a |
		aux0) & b | a)) + 34478 * ~~(aux0 ^ ~b) + 7713 * (~~((b ^ a)
		& ~b & b & a & (aux0 | aux0) ^ (a | a) & a & aux0 & (~b |
		aux0 ^ b) | aux0 & ~(aux0 | a) & aux0) ^ aux0) + 31381 * ((b
		| ~(a | aux0) | a ^ b ^ ~aux0 ^ aux0 ^ (b & aux0 ^ a & a) &
		~(aux0 & aux0)) & aux0 ^ ~a) + 44499 * ((~~(b & ~~a ^ a ^
		aux0) | b) & a) + 25673 * ~((aux0 | ~a) & b ^ (b | ~b) ^ b)
		+ 14522 * ((b ^ (~((aux0 | b) ^ a & b ^ b) | b | aux0 | aux0
		| ~(a & aux0))) & ~(~(aux0 ^ ~~(a | aux0)) & ((~(b & a ^ ~b)
		| ~~a | ~~b) ^ (b & a ^ ~aux0 ^ (aux0 & aux0 | a & aux0) | a
		& aux0 | aux0 ^ aux0 | aux0 & a ^ a & aux0)))) + 41862 * (~a
		^ b ^ b ^ ~~(b ^ a ^ aux0 ^ (b & a | aux0)) ^ ~(~aux0 & (b ^
		aux0)) & (aux0 & aux0 ^ a & b) & ~~b ^ ~b) + 26674 * ~~(aux0
		& (a & b | aux0 ^ b | (a | a) & a & b | b) | aux0 & a) +
		41984 * ~aux0 + 64247 * ~~(~(~aux0 ^ (a | aux0 | b ^ a)) ^
		(~(b ^ b) ^ ~a ^ aux0 ^ aux0) & ((a | a | b) ^ b ^ aux0) ^
		(a ^ b ^ b ^ ~aux0 & (a ^ b) | b & b ^ (aux0 | aux0) | a &
		(b ^ b) | (a ^ (b ^ aux0) & aux0) & ~~(b | aux0))) + 17665 *
		((~((b & a | aux0 ^ a) & ~a & ~(~b & a) & (~(b ^ a) ^ ~(b &
		a) ^ ~(aux0 & b ^ a ^ b))) | b | a & (b | a | b & a) & ~aux0
		& (aux0 ^ a ^ (~(a ^ a) | a ^ a ^ ~b))) ^ (b ^ a) & (~(aux0
		& ((b | aux0) ^ aux0)) | ~b) & ~~(~aux0 ^ a)) + 8215 *
		((~(~~(a & a ^ (a | a)) ^ ((aux0 | aux0) ^ a ^ aux0 ^ b |
		aux0 & ~~aux0)) | b) ^ ((a ^ ~(aux0 & b) | aux0) & b | aux0)
		^ (aux0 ^ (~(b & b) | a ^ aux0 | a)) & ((aux0 | aux0) & (b |
		aux0) ^ aux0 | ~(aux0 & a) ^ (aux0 ^ b) & ~aux0) & a) +
		34107 * ((~(~((a ^ a) & (aux0 | aux0)) ^ a ^ (~a | aux0 &
		b)) ^ ~(~(b ^ a) ^ (a | aux0 ^ b)) ^ (aux0 ^ aux0 | b ^ aux0
		| b | b) & ~b) & a & ((~~b | aux0 ^ a ^ b | ~~aux0) ^ aux0 ^
		b) & ~aux0) + 12830 * ~(~a & ((b & ~~b ^ ~(aux0 & b) & ~~a)
		& b ^ ~(a | ~(a ^ aux0 & a)))) + 61358 * (~~~aux0 & ((~((~a
		^ b ^ ~(aux0 ^ b)) & (b | aux0) & aux0 & b & (~b ^ a & b)) |
		aux0) ^ (~a ^ ~b) & (aux0 & b ^ (b | aux0 | ~b) | a) &
		~(aux0 | b | a | ~b | aux0))) + 28947 * (~~a & ~(~(b ^ b ^
		aux0) & ((aux0 | b) & aux0 | aux0 & a ^ aux0) ^ ~~(b ^ b) &
		(b & a ^ a & aux0 | a ^ aux0 ^ aux0)) | ~~(a ^ ~a ^ a ^ aux0
		^ ~a) | ~((a | aux0 | b ^ b | ~b) ^ ((a | b) ^ (b | aux0) |
		(b ^ a) & ~aux0)) ^ ((aux0 & b & b | ~(a | a)) ^ aux0) &
		((~b | a & aux0) & b | ~b ^ (a | a) ^ aux0 & b)) + 52277 *
		(b & a) + 10139 * ((aux0 ^ a & b & a & b ^ a & (a | aux0) ^
		~(~aux0 & a & a)) & a & b & ~a & ~(b & aux0) & ~~(a ^ a) &
		~(~~a & (a & aux0 ^ b & b)) | a | (a | ~aux0) & ~a | ~~((a |
		aux0 ^ ~aux0) ^ b ^ a ^ a & aux0 ^ ~(b | a))) + 6075 * ~~(a
		& ~(b & (b ^ b ^ aux0 ^ a) & (a | a) & (a ^ b))) + 28830 *
		(((aux0 ^ aux0 & a ^ ~a & (b | b) | ~~b ^ ~(aux0 ^ a)) ^ ~(b
		& a & ~b | a) | (a | (aux0 ^ a | b ^ a) ^ aux0) & (a | aux0
		^ (aux0 ^ a | b ^ b))) & ((~b | a & aux0) & ~(aux0 & aux0) &
		((aux0 | aux0) ^ aux0 | a | a | aux0 | b) | (aux0 ^ ~b) & b
		& (aux0 ^ b & b) & (~b | aux0 ^ aux0) | ~((a | aux0 ^ b |
		aux0 ^ a) & b)) ^ ~(b | ~(~(aux0 ^ b) & a ^ b ^ (~a |
		~b)))));
    vm->memory[--vm->sp] = result;
    // std::cout << "[DEBUG] MUL: " << a << " * " << b << " = " << result << " [sp=" << vm->sp << "]" << std::endl;

    WORD next = vm->fetch_opcode();
    vm->handler_table[next](vm);
}

void VirtualMachine::op_div(VirtualMachine* vm){
    WORD b = vm->memory[vm->sp++];
    WORD a = vm->memory[vm->sp++];
    if (b == 0) {
        // std::cout << "[ERROR] DIV: Division by zero at ip=" << vm->ip-1 << std::endl;
        vm->running = false;
        return;
    }
    WORD result = a / b;
    vm->memory[--vm->sp] = result;
    // std::cout << "[DEBUG] DIV: " << a << " / " << b << " = " << result << " [sp=" << vm->sp << "]" << std::endl;

    WORD next = vm->fetch_opcode();
    vm->handler_table[next](vm);
}

void VirtualMachine::op_and(VirtualMachine* vm){
    WORD b = vm->memory[vm->sp++];
    WORD a = vm->memory[vm->sp++];
    WORD result = 59310 + 60167 * (~(a ^ ~~b) ^ (~b | a) | a) + 13480 *
		((~~(a | b | b ^ a) | a) & (b & (a ^ b ^ ~b ^ a & a & a & a)
		| ~b & ~(a ^ b) & a) & ((b & b & b & (a | b) ^ b ^ (b ^ a |
		a & a)) & ~a ^ ~b & (b & (b | b | a) ^ a)) & (a & ~(a ^ b) &
		(~b ^ ~a ^ a) & (((b ^ b) & (a | a) | ~(a | b)) ^ (~(b & b)
		| ~b & (b ^ a))) ^ ~(b & ~((a | a) ^ (b | a))) | b ^ a ^ a))
		+ 33342 * a + 57139 * ((a & ((a & b & (a | b) | (a | b) ^ a
		^ b) & b | b | ~a | a & a & (b ^ a)) ^ (a & b & ~(b ^ b) | b
		^ (~b | b & b | a ^ b ^ (b | b))) & ~((a & a ^ a & b) & (a |
		b) & (a | b)) & ~(~(a | b) ^ b ^ a ^ a)) & (~~((a | b) ^ (b
		| b) ^ (b | b) & (a | a)) | (~~a ^ (a | b) & b & b) & (a & b
		& (b ^ b) | ~(a & b)) & (a | ~(a ^ b ^ b & a)) | a)) + 13221
		* ((~~~~b & ((~a | a | (a ^ b) & ~a) ^ b ^ ~b ^ (a | b)) ^
		(b & a | (a | a) & ~a & (a | b) & b ^ (a | ~~a))) & ~(a ^ ~b
		^ b ^ b ^ a & b ^ ~b) ^ ~b) + 26459 * (((~a | a & b & a |
		~(b | b) | (b ^ ~b) & (a | b & b) & (a | a ^ (b | a))) ^ ((a
		& (b | b) ^ ~a) & (a ^ a ^ b | b & a | a | a) | a & ~(a ^ (a
		| b))) | b & (~(a & a ^ ~(a | b)) | ~(a & b | ~b | ~a ^ b)))
		& b) + 56875 * ~(b & ((a & b | a ^ a | (b | a) & a & b | b &
		(a ^ b) | a & b ^ b ^ a) ^ (b & a & (b | b) | (a | a) & (b ^
		b)) ^ b) | (a ^ a) & (a & a ^ a & a | ~b ^ a & a) & (~(b |
		b) ^ (b | b | a) ^ (a | b) ^ a ^ a ^ (b | a | ~a)) & (b & a
		| b | (b | a) & b) & (a | b | a) & (a & a ^ ~b) & a & ~(b &
		a & (a ^ a))) + 53402 * ((~(((b ^ a | b & b) ^ b) & a &
		~(~~b ^ a)) | b) & ((b | ~~(a | b) ^ a) ^ (b | ~~a) | (~~~(a
		| a) ^ b) & b)) + 39339 * (~b | b & ((a & (a ^ a) ^ b | ~~b
		& ((b | b) ^ a & b)) & ~(~b ^ (b ^ b | b & b)) ^ ((b | ~(a |
		b)) ^ ~(a ^ b) ^ (a | b) & ~a) & (a ^ b | b) & (b ^ a ^ a ^
		b) & ~((b | a) ^ ~b))) + 20165 * ((b | b | (~a ^ b & a) & (a
		& a | ~b) ^ (~a | a)) & ~(b ^ a) & (~~b | a)) + 59730 * ~b +
		37160 * ~(b | ((~a | a) ^ (a & a | a | a)) & b & ~(a | ~(a ^
		a)) | ~((~b ^ ~a) & (b & a | b ^ b)) & ~a) + 60418 * (~b |
		~a) + 62491 * b + 19273 * (~((b & b & a ^ b ^ b ^ b | a | b
		| ~(b | b) | b) & b) & ~((~(~a ^ (b | a)) | b | (b ^ b) & a)
		& (a & a ^ a ^ (a | b | b) ^ ~(b ^ a ^ a)) ^ b & ~(b | b) &
		~b & (b | b | b | ~(a | b) ^ a))) + 34669 * (~((a & b & a &
		a & ~(b ^ a) | ~~~a) & ((a ^ b) & a ^ (b ^ a | ~b) & a & a &
		a) & ~(a & ~(~b | a | b))) & (a | a)) + 60654 * ~((~a | (a |
		a) & (a | b) | ~(b | b) | ~a | (a | a | b) & ~(b | b) | ~(b
		| a) ^ b & b) ^ ~(~((~a | ~a) & ~b & (a ^ a)) ^ ~(b ^ a ^ b
		& a | b ^ a | ~a))) + 33743 * ((a | ~(~b & b & b ^ b)) ^ (a
		| ~b ^ (b | b) | b) & b ^ (a | ~b ^ (b | a & a | a) | ~((a &
		b | b ^ a) ^ b & ~a)) ^ ~((b | a | a & b) & ~~b & ~~(b | b)
		| ~(~(b | a) | a & b ^ a ^ b)) & b) + 7758 * (~(b & a ^ ~(a
		& a)) & b & ((a | ~b ^ b) ^ ~~(a | a)) & (a | (b ^ a) & a &
		b & (b | a)) ^ (a ^ b ^ b & a | a ^ a & a) & a & ~(b & (a ^
		a) ^ ~~a) ^ ~(b | a | b & b | b | b | ~a) ^ ~~(b | a & a) |
		~((a ^ b & b ^ b ^ b) & ~((b ^ a) & b) | b ^ a & b) & ~((~(b
		^ a) | ~a & a | ~b ^ (b | a) ^ ~(a | a)) ^ (a | (a ^ b) & (b
		| a) & ~(b & a)))) + 48812 * (~~(~~~~b | ~(a ^ ~b | b & a ^
		b ^ b)) & (~b | ~(~(a | ~a) & a) | (a | b | b | b & a ^ a ^
		b) & ~(~b & (a | a)) & ~(~(a ^ b) | a) ^ (~~(a & ~b) | ~(a ^
		~b ^ (a | a) ^ a & b)))) + 52216 * (b & (b & a & b & (a & b
		^ b & a) & (a & b | b ^ a | ~a | ~b) ^ (~~(a | b) | (b ^ b |
		~a) ^ (a ^ b) & a) | a) ^ a) + 9094 * (b & (~((b | b | a |
		b) & b) ^ (~(b & a) | a) ^ ((b ^ ~b | b) ^ (b | b ^ b | ~b &
		~b)) & a & ~b ^ ((~((a | a) ^ a & b) ^ b) & ((a | b | a) ^
		(b ^ a) & (a ^ a) ^ (~~b | (a | a) & b)) | a))) + 35909 *
		(~b | b) + 32819 * ~(~(~(a & b & ~b) & (b | ~(b ^ a)) & (a ^
		a | ~b | (a | b) ^ a & b) & (~a | a | a | a ^ a)) | ~((b ^
		~(b & a)) & (b & b ^ b | ~a & ~b)) & (a | ~(a | a) & ~(b &
		b) & (~b | b ^ a | a & a ^ (b | b)))) + 14032 * (b & (~(~(a
		& (b ^ a)) ^ a) | a) & ~(a ^ ((~a | b & b) & b | ~a & (a ^
		b) & ~(a | a) | a))) + 43465 * ~(~(a ^ (a & b | ~a) & ~b ^
		~~a ^ (b ^ a) & (b ^ b)) ^ ~(~b & ~(b & a) | a & ~(b | a)) ^
		(a ^ b ^ a ^ a ^ ~b ^ ~b) & ~(a ^ b ^ a) ^ b) + 60350 * (b |
		(~(~(b & a) | b) ^ (b & (b ^ b) | ~~b) ^ b) & (~b & a ^ a ^
		(b | a) ^ (a | b ^ b | b ^ b | b)) & (~~~b & (~a & b & b ^
		(b | a) & ~b) ^ ~(b & b ^ b | b & b)) & a) + 29111 * (~(b ^
		(~((a | a) ^ (a | a) | ~~b) | (b | b | a ^ b) ^ b & b & ~a ^
		(~a | ~a | b ^ ~b))) | ~((a | ~a) & ~(a | a) ^ (b | a) | a |
		b & (b ^ (a | a)) & ~a & ~~(a ^ b | a))) + 37151 * ~(b &
		~~((~a ^ a) & (b | a) & b & a & ~(~a | ~b))) + 33746 * (~(b
		^ ~(~b & (a | a) ^ a)) & ((~b | ~a) ^ ~(b & a ^ b & b) ^ (a
		^ (b & a | ~a) | ~((b | b) & (b | a))) ^ ~(~(b & a) & ~a &
		~b | a)) & b) + 24291 * (b & b) + 46481 * ~(((b | b | a |
		~(a & a)) ^ ~((a ^ a) & ~b)) & (b & b | b) & a & b ^ ~(~(b &
		a | ~b) ^ ~(a & b) ^ ~b) | ~((~~a | a ^ b ^ a ^ a) ^ ~(b ^
		b) ^ a | (b & a | b ^ a) & ~(b | b) ^ a)) + 10356 * ((~b &
		(~(b ^ a) ^ (b | b | b & b)) | ~((b ^ a) & b & (b | a) & a &
		b)) ^ (~(b ^ ~a ^ a) | a) | (a | b | b | b ^ b | a) & (~(b |
		a & a) ^ a) & (a | (a | b) ^ b | ~(a & a) ^ b | ~(b & a) ^
		(b | b) & b ^ (a | ~b | a & a)) | (b | ~a & ~~b) ^ ((a | b)
		& (b ^ b) & b & (a & b | b ^ a | (b | b) ^ b) | ~a & a & (a
		| b) & (b | a)) | a & ~(((b | a) ^ b & a) & (b ^ b | a & b))
		| b & b & b & (a & b ^ b & b ^ (a | b | a ^ b) ^ (b | a) & a
		^ (a ^ b | b ^ a))) + 7715 * (~~(b & b & ((b ^ a) & ~b & (a
		^ a ^ a & a) | (a ^ a) & ~a | ~(a ^ a))) | ~a ^ (~(a ^ (b |
		b)) & (~(b ^ b) | ~(b | b)) ^ a | ~(~a ^ ~b ^ b ^ b ^ (a & b
		| b ^ a)))) + 39185 * ~(b ^ b ^ b & (~a | b)) + 28919 * ~(b
		& a) + 9886 * ((~b | b ^ a | a | a | ~(a & a) | a & ((a | b)
		^ a) | ~b & ~b ^ ~b ^ (b ^ b) & ~b ^ (b | a | ~a) | a) &
		~((a & a ^ ~(~(b ^ b) & (a ^ a) & ~b)) & ~(~~a & ~(b & a)) &
		a)) + 58786 * ((~~(b | ~(a | b) & (b | a) & (a ^ a)) | ~(b &
		(b ^ (~b | a ^ a)) & ~b)) ^ (~(a | ~(a | a | b & a)) | ~((a
		^ a ^ ~b) & (~a ^ b & b)) ^ b) ^ (b | (b | a ^ (b | b) | b)
		^ ~~(a & a) & (a & b & b ^ b & b & ~a))) + 33739 * ((a & (b
		^ a ^ (a | b)) ^ ((b ^ b) & (a ^ b) | a) ^ a ^ ~b | ~((b & b
		^ ~b) & b ^ (a | a) ^ a ^ (a | a) & (b | a))) & ((~(b ^ b) &
		~~b ^ ~(b ^ b ^ b ^ a)) & (~(a | a & a) | ~(b | b) | b & ~b)
		| b) & (a ^ b & b ^ (a | (b | ~a | a | b) ^ ~(a & b) | (b ^
		a ^ b & a ^ ~(a | a) | (b & b | a | b) & a) & ~~b))) + 3040
		* (b & (a ^ a)) + 19798 * ((~a ^ ~~(~b & ~b) & (a | (~a ^
		~b) & a)) & ((~b ^ (a | b)) & a ^ ~~a & (~b | b) ^ (b | a ^
		a) & (b ^ a) & ~b) & ~(~a | b & b | a ^ b ^ (a | a) | (a | b
		| a ^ b) & ~a & (a | a)) ^ b) + 34528 * ((a ^ a) & ~~b ^ ((a
		| a | b | b & a & a | a | a) ^ (b ^ b) & ~a) & (~((a & b | b
		| b) ^ b ^ (b | a)) ^ a) | ~(a & ~a & b & ~a | (a ^ a | a |
		b) & ~(a ^ b) | (b ^ b) & (b ^ b) | ~b | a) & b) + 31174 *
		((~~(~a & b & a) & a ^ b & ~(b & b) & ~(a & a & a) & b & ~(a
		& b) & (a ^ a ^ b & b)) & ((a ^ a ^ b | b | ~b) & ~(~a | ~a)
		| ~(b ^ b | ~b) | a) & (~~b | ~~(a ^ a) | ~(a ^ b)) ^ b ^
		((b ^ a ^ b | ~b ^ (a | a)) & (a ^ b ^ a | a | b | b) | b) &
		(~b | ~~~(b ^ a))) + 32744 * ((~a ^ b | ~b ^ b | a) ^ (~~b |
		a) ^ a ^ ~((~a | b | b) ^ b) & ((b | a ^ a) & a | (a | b ^
		a) & b) ^ ~~((b & b | a | b) & (a & b ^ b)) | ~(a & a & b &
		a & b & ~(a ^ b | b & b) & b | (a ^ (a | a) | ~b ^ ~a | ~b)
		^ ~a ^ ~a ^ (b ^ b) & b & b ^ b)) + 37500 * ((~b & ((a | a)
		^ a ^ b) & (~a ^ (a | a)) & ((b | b) ^ ~a | a & a ^ (b | b))
		& ~(a & (a | a)) ^ ~(a | a | a | a) & ~(a | b) & ~(a & a) &
		b | ~~a & a & (b ^ (b | b) ^ (b | b)) ^ ~(~b | b & a | ~(b ^
		a)) | ~~(~(b | a) & ~(b ^ b))) ^ a ^ a) + 38068 * (a & a & b
		| a | b) + 8387 * (~~(~~(b | b) ^ ~a) ^ (a & (~(a | a) | ~(b
		| a) | (b | a | a) & ~(a & a)) | b) | ~a) + 36405 * ((b & b
		| (~(a | b) & ~(a | a) ^ a & b & ~b & ((b | a) ^ b ^ b)) &
		(a ^ (a ^ (b | a) | ~a ^ ~b)) | a) ^ a) + 9683 * ~(~(a & b &
		(b ^ a) & (~a ^ (b | a)) ^ (~(b & b) | a)) ^ (~(b & b) ^ b &
		b ^ a & a | ~b | b ^ (~a ^ b ^ a | ~a)) ^ (a | ((a ^ a ^ ~b)
		& ~(a ^ a) | (~b ^ (a | b)) & (a | ~b)) ^ a)) + 55757 *
		~~(~~b | (~b | ~a) & (a & a ^ a & b) ^ ~a ^ b | (a & b ^ b &
		a) & a & (a & b & a & a | a)) + 37645 * (b & ~~((a | b | b |
		b) & (a | b | a) ^ b ^ b ^ b & a ^ ~(b & b)) | ~a ^ (b |
		~((~b ^ (a | b)) & (a & b ^ a & a)) | ~(b ^ a) & (a | a | b)
		^ (~b & (b ^ b) | a & b | b & b))) + 13286 * ((b | ~(a & (a
		^ a) & a & a & (b & a & b & a ^ (b | a) & ~a))) ^ a | ~((a &
		~(b & b) & (b ^ b ^ ~b) | (b | b) & (a ^ a) & ~(b ^ a) & (a
		| b)) ^ a ^ ~(~a | a | b) & b)) + 30343 * (((~~(b & b ^ (b |
		b)) ^ ~(a ^ b ^ (b | a) | b ^ a & a)) & ((b ^ a ^ a ^ b) &
		~a ^ ~~(~b | b ^ a)) | a) & ~~(a & (~(a ^ a) ^ ~(b | b) | (a
		^ b & b) & ~b))) + 42504 * ~(~~b | ~(~~a ^ b) | a | a) +
		44197 * ~((b ^ b & (~a ^ ~(~b & b & a))) & ~(a | a | ~~~b))
		+ 21335 * ((a & a & ~(b & a) & (b | b ^ b) | ~((a ^ b) & (a
		^ b) & ~a & (a ^ b)) & a) ^ a ^ (~(b ^ a) | (b ^ b) & ~a |
		b) & ~a | b ^ b) + 56921 * (b ^ (b ^ a | b & a & b ^ (b & a
		| a ^ a) ^ a ^ (b | a) ^ (a | b)) ^ a ^ ~(b | (a ^ a) & (a ^
		a) & (b ^ a | b)) & ((b | a | a & b) & ~~b | (a | ~a) & a &
		a) & a & (b ^ b ^ b ^ a & b) & (a & ((a & a | ~b) ^ ~a ^ (a
		| b)) & (a & b & (a ^ a) | a & b | b | a) & ~(~a & (a | b))
		| (b ^ b ^ ~b ^ a & b ^ a ^ a) & ~a & a)) + 47970 * (b |
		(((a ^ a) & a | a) & (a & a | b & b | a & (a | b)) & b | a)
		^ a) + 55288 * ((a & b & b & ~(b | b) & ((~a | b ^ b) ^ (b |
		b | b ^ a)) ^ a & (a ^ b) ^ (a | a) & a & b ^ b ^ b ^ ~~~(b
		^ a ^ (a | a))) & ((b ^ a) & (~a ^ ~((b | b) & (a | b))) ^
		a) | ~(~(b | b & ~(a & b)) | ~(a & a ^ a ^ b | b & b | ~b) ^
		~~~(a ^ a))) + 744 * ~(~(a ^ (~a ^ b & b ^ (a | b | a | b))
		& (b | b ^ b & b)) | (~a | a | b | b) & (~b ^ ~(b & a) & a)
		^ a ^ a & ~a) + 43749 * (~~(b ^ a ^ ~(a ^ a ^ a & b)) & (b |
		((a | a) & b & b | ~(a ^ a)) & ~(~b ^ b & a)) & (b | (a ^ b
		^ ~a | ~a) ^ ~a & (a ^ b) ^ (b & a | b & b)) & ~a) + 25199 *
		~((~(~b | b | a) | ~~(b ^ a)) ^ ((a | a | a & a) & (~b | a)
		| a | ~a | a | b) ^ (a & b & a & b | ~(a | a)) & ~(~a & (a ^
		b)) ^ ~(b ^ b | a ^ a | a | a) | ~a & b) + 36086 * ~~~(a &
		(b ^ b) & (a ^ b) ^ a) + 32678 * (b & ~((b ^ b) & (b ^ b) &
		a) & ~~b ^ (((b ^ a) & b & a | a ^ a ^ b | ~a) & a | ~(b |
		~b) & ~(~b ^ b & a) | (~a & b | a & b | b ^ a) ^ ~(a & b &
		b)) ^ ~(b & a | ~(a | ~b) | ~(b & b & (b ^ b))) & ((~~(b &
		b) | a & a & (a | b) & ~b) & (b ^ b ^ a & b | ~a ^ b | b) ^
		b ^ ~(b & ~(b ^ a)))) + 23153 * (a & ~(b | b & ~b | a ^ b ^
		(a | b) ^ (a | b) & a & a | (a ^ a) & (a ^ a) & ((a | a) ^ b
		^ b) & (a | b) & (~b ^ b & b) | b)) + 47921 * (((~(b & ~(a &
		b)) ^ ~b & a & (b | a) & b & ~(~b ^ b ^ b)) & a ^ ~(~b ^ (b
		| b) ^ ~~b ^ b & b ^ (a | a) ^ ~b & (a ^ a)) ^ a) & ((b ^ b
		| a & (a ^ a) & b & ((a | a) ^ (a | b) | a & a | a & b) | b
		| b & (a & a & a ^ (b | a | b ^ b))) ^ ~~(~(a | a) | ~~a) &
		(~~a ^ ~b & b & a | a & a & (a ^ a) ^ (a & b | a)) & ~((b ^
		a) & (a | a) & ~a))) + 2517 * ((((b | b | b & b | b | a | b
		^ a) ^ a) & ~(a & (b | b) & ~b) & (~a ^ (a & b | a | b | a &
		b) ^ (a ^ b | a ^ b) ^ ~a ^ ~a) | ~(b ^ (b ^ b ^ b) & a ^
		b)) ^ ~b) + 27890 * (a & b & (~b | (a | a | ~a) & (a ^ a) &
		b ^ (a | b)) & ~(a ^ a) & (~((b | b) & a & b & ~(b ^ a) ^
		~(b & (b | a))) | b ^ b) & a) + 12757 * ((b | (b | ~~((b |
		a) ^ (a | a))) ^ b & (b ^ ~(a & b | a & b))) & ~(b & (~(b ^
		b ^ b ^ a) | (b | b | b & b) ^ a & a & a) | ~(a | a))) +
		14617 * (a & ~((~(b | a) & a | ~b | (~a ^ a & a) & (b | ~b)
		| ~b) & (b | b & b & b & b ^ (a | b) & b & a | a))) + 7066 *
		(~b | (b & a ^ a ^ b ^ a & a) & (a | a | (b | a) & (b | b))
		| b | ~((a | a) ^ b & b) | ~a | b | ((a | b) ^ a) & (~a | b
		| b | a | b | a | ~(b ^ a & a)) ^ b) + 10635 * ((~((a & b |
		a ^ b) & (~b ^ b & a) | ~(b ^ a) | (a | a) & ~a) ^ a) & ~b &
		b) + 52962 * (b & (~a & ((a | a) ^ a ^ a ^ ~(a | b)) | ~(a ^
		(b | b) ^ ~a)) & (b | b) ^ (b ^ b) & ((b | b ^ a | a) & (a &
		b | b ^ b) & a & a & ~a | ~(~b ^ ~b | ~b ^ b & b)) & ~~(~a ^
		~b) & ~(~~a | a)) + 24284 * ~(~(~((~b | ~b) & a) | ~(b ^ a ^
		(a | b) ^ ~(a ^ a))) ^ a) + 35260 * (~(~~(b & b) ^ b & b ^
		~a ^ ~a) & (a & (b | a) & ~b & ((a | b) ^ a & a) & (b & a |
		a | a) ^ b) | ~(~b | ~((a ^ b | b ^ a) & (a & b | a))) |
		~~~(a & b) & (b ^ (a | a | b | a | a | a)) & ((~(b ^ a) | b
		| ~b | a ^ ~(a | a)) ^ a) & (a & (b ^ b | ~(a & (a | a))) |
		~~~(b | a & b))) + 3426 * (a ^ ~(~(b ^ a ^ b & b) & (~~a ^ a
		^ a) & (b | b | b ^ b | ~(b ^ b) | (b ^ a | a) ^ ~(a | b)))
		| b) + 31706 * ((a ^ ~((b | a ^ (b | a)) ^ (a ^ a & a) & (a
		& a | b | a)) | b) & (b | (a | b | a ^ b | (a | a) ^ b ^ b)
		^ (a & a & b | b ^ b ^ a) ^ ~~b ^ b)) + 4848 * ~~(~((a | ~(b
		^ a)) & (~~b ^ a & a ^ ~a)) & a) + 63149 * ~(a | ~((~(a ^ b)
		| ~b & (b | a)) & ((b ^ a | b | b) ^ a)) | ~~a & (b | a | b
		| a) & a) + 41497 * (b ^ ~b) + 42041 * (~((b | ~(b | a)) ^
		(~b | a ^ b) ^ b ^ ~((a | b & b) & (b ^ b ^ ~a)) | ~b) & ~a)
		+ 65528 * (a ^ a) + 64170 * (~(a & (~a ^ ~~~b) & (b & (a ^ a
		& b | ~b | b ^ a) | (b ^ (a | b)) & (a ^ b | a) | ~(~a &
		b))) | (~b ^ (b | b | a)) & b | ~~~~(b & a) ^ (b | (b | b) &
		b | a & a | a | ~b & ~b & (b ^ a) | (a & a | a) ^ ~~a)) +
		28187 * (b | ~((((a ^ a) & a | ~(b | a)) & (a ^ b) & b & a &
		a & a | ~~(a ^ a) & (b & a ^ b | b)) ^ b)) + 11552 * (~~b &
		b) + 20843 * (b | b | ~(~(a ^ (a | a) ^ ~~b) | ~((b | a) ^
		~a) ^ a & b) ^ ((a ^ a & a ^ (~b | a ^ a)) & (b & (a | b) ^
		~(b ^ a)) | ~(b | b)) ^ ~((b | ~a ^ a ^ b) ^ b ^ a ^ a ^ b ^
		b)) + 45594 * (((a & a ^ a ^ b & b & a & a | b ^ b ^ ~b ^ (a
		& a | ~a)) & (~(~a | b | a) | (a ^ (a | b)) & ~~b) & b | ~b
		& (b | a & b | ~~~b)) & ~(~((a | b) ^ ~b) & b ^ b & b & b &
		a ^ ~(a & b) ^ b) & ~((a ^ ~a ^ b ^ b ^ b & b) & ~(b ^ b) |
		b | ~(b ^ a) | (b ^ a) & (a ^ b ^ ~a))) + 42455 * ~~((b & b
		| a & (a ^ b) ^ ((a | a) ^ ~b) & (a ^ a) & (a ^ a)) ^ (~b ^
		(a | b) | b) & b ^ (~(a & b) & (~a | b & a) | a | a)) +
		34829 * ~((b & ~(a & (a | b)) | (~a | b & b) & (a | b) ^ (b
		| ~a & a) | ~(b | b) & ~(a | b) & ~~(a & b) | ~(b & a ^ b) ^
		~(b & (a ^ a))) ^ a) + 53258 * (~~((a ^ ~(~b & (a | b))) &
		((b & b | a | a) ^ ~(b | a) ^ a)) ^ a) + 21139 * (a | a) +
		52819 * ((b & ~(~((a | b) & (a ^ a)) | ~(a & a) & a) | (b ^
		(a | a & b) ^ ~((b | b) ^ a)) & a | a ^ b) & b) + 44984 * (b
		^ b & ((b | a) ^ ~b | b | a | b & a) & ((a | b) & (a | b) ^
		b) ^ ~((a ^ a | a) ^ ~a ^ a ^ a ^ b) ^ a ^ ~(b ^ a ^ a | (b
		| b) & ~a | ~b)) + 843 * (~(~b | b | a & b ^ b & b) | (~((b
		^ a) & ~(a | a)) ^ b ^ b ^ (b ^ b) & (a ^ b) ^ ~(~b & a &
		b)) & b & b & ((~a & ~a | b | b & b) ^ ~~(b ^ a)) | (a | ~(a
		| a) & b | b) ^ (~a | a | a & a & (a | a)) ^ ~(b & a) & (a |
		b) & a & a & (~a & (b ^ a) ^ ~b ^ a ^ b)) + 22049 * (b | b |
		b) + 46736 * (~b & (b ^ b ^ b & b | a & b) & (~(b ^ b) ^ b)
		& ((a | a | b | b ^ a | ~b | a & b) ^ (b ^ b ^ (a | b) ^ a &
		(b | a)) & (b ^ a | b ^ a | ~a | b & a)) ^ ~(b & (b | b | a)
		& ~(b | a) & (a & b ^ ~b)) & (~~(b & a) ^ ~(b & a ^ (b | b))
		| a) ^ a & (~((~b ^ ~b) & (a ^ a) & a) | ~(~b | ~a) ^ (a |
		(a | a) & a)) ^ b ^ ((a ^ b | a & a) & (a | a | ~a) & a |
		~((~a ^ b) & ~(a ^ a)))) + 10017 * ~(~(~(b ^ b) & (a & a |
		b) | (b | b | ~a) & (a | b | b)) | ~~(~a ^ (a | a)) | b & a
		& b & b ^ (a ^ a) & (a ^ a) ^ (a | a & a ^ ~b) | b) + 40036
		* (~(a | (b | (b ^ a ^ ~a) & (b | b) & (a | a)) ^ ~~~a & (b
		^ b ^ ~b | a ^ b | ~b)) & a) + 36163 * ((~(a ^ (a | ~~b)) ^
		a ^ b) & ((~~(a ^ b) | ~~b) ^ b | ~a | ~((~b | ~(b & (a ^
		a))) ^ (a & b & ~a | ~b | b & b) & (~b & ~a ^ b & b)))) +
		5927 * (~(~((b | b | b ^ b) ^ ~(b ^ a)) & b & ((b | b) ^ b ^
		a) & (a ^ b ^ a)) & b & (b & ~(~(b | b) | b) ^ b | a ^ ~(~b
		& (a | b) | b & b & b & a | ~~(b ^ a)))) + 51992 * ((a | ~b
		^ b) & ((~(b ^ b ^ b) ^ a) & (~(a & b) | a ^ b ^ a | a ^ b ^
		~a | a ^ b ^ ~b) | b | (b | a & b ^ b) ^ a ^ b & ~a ^ ~~a &
		~a ^ b | ((a | b) & a ^ (b ^ b) & a | ~~(a & b)) & a)) +
		47732 * (b | ~a & ((b | b ^ a) ^ (a | a | a ^ b) ^ ~~(b ^ a)
		| a & b | ~a ^ (a ^ b & b | ~~b) | b | ~a ^ (a | b) ^ a)) +
		56862 * (b ^ a) + 11525 * (~((~~~a & (~(b | a) | ~a & (a ^
		a)) ^ ~(b & b & (a ^ a)) & a) & ~((~a | a | a) ^ ~(b ^ a) |
		(b | ~b) & ~a & ~b)) | ~(~~(a ^ b) ^ (a & a & (a | a) | (b |
		b) & (a | b))) ^ a & (b | ~(b | b | a & b)) | ~((a ^ b | b)
		^ (b | ~b) | ~(b | a) | b & b & b) & a) + 12021 * ((~~a ^
		~(b & (b ^ a ^ b)) | b | ~(b | b | b | b | a ^ ~a)) &
		(~~(~(b | a) & (b ^ a | b & a)) ^ ~(b | b | a | ~a & a) ^
		~((b | b) ^ b & a | a | b ^ b)) | ~(~(~(b | a) ^ b & b & a)
		^ a) ^ (a | a)) + 25443 * ~((~~(b | a) | (b ^ a ^ ~a) & (b ^
		b) & a | b | (a | a | a | a | a | (b ^ a) & (b ^ b)) & (~b ^
		(b | b) ^ a ^ b ^ ~(b | a))) ^ ~((a ^ a | a | a) & a & ~(a &
		a & (b | b)) | ~(a & b) ^ a & b ^ b & b | a ^ ~(b & b))) +
		44661 * ~(b & b) + 61800 * ~((~~~b | ~(~~(a ^ b) | a)) ^ a &
		(a | b ^ b | ~a) & ((b | b) & (a | b) | (b | b) & ~b) & a ^
		~(~(b & b) | (a | a) & a & a & ((b | a) ^ b))) + 28127 *
		(~(a ^ ~(~a & (~a | a | a) & b)) ^ ~(~(a ^ a & a & (a ^ b))
		^ ~(b ^ a ^ ~b) & ~(a ^ b ^ ~a)) ^ (~(b | ~(b ^ a | b & b))
		| ~(((a | b) ^ b & b | b & a & (b | a)) ^ (b | b & a & (a ^
		a))))) + 8145 * ((~((b & a & b & a | b) & b) | a ^ ~(~b & (a
		^ b)) ^ a) & (~((~a ^ (b | a)) & (a & a ^ b)) ^ ~~~b | (~b &
		~b ^ ~~a | a) ^ b) & (a & ~(a | b | ~b) & b | ~~(b & a & (a
		| b)) & (a & a & (a | b) | b) | ~(~b | ~(b | b) & (a & b | b
		| a)) ^ b)) + 14186 * (((((a | a) ^ a & a) & b | ~((b | a) &
		(a | b))) ^ (~~b ^ (b | a | ~b)) & (b | a | b | b) & (a ^ b)
		& (b ^ b) ^ ~(((a ^ a) & (a | b) | a) & ~~a & ~(b & a))) &
		~(b | a & ((a | a) ^ a | ~~b)) | ~((((b & b ^ ~a) & (b | b |
		b) | ~((a | b) ^ b)) ^ ~((b ^ b) & (a ^ a)) ^ ~b) & ((~b ^ b
		& a) & a ^ ~(a & a ^ (a | a)) | ~~a | ~((a | a) ^ a)))) +
		12295 * (~(~b ^ ((b | b) ^ (a | a) ^ ~a ^ a ^ a ^ (a & a | a
		^ b) ^ b | b & (b | a) | a & b)) ^ (~a | a | ~~((a | a) & (b
		| a)) | (a & a & b ^ ~b & a ^ (b | b | a & b | b ^ b | a |
		b)) & (~(b & b | a | b) | (a ^ b) & (a | b) | a) & ~(b & a ^
		a))) + 8544 * ~a + 46070 * ((b | ((b | a) ^ b ^ (a | a)) &
		~~(b & b) ^ b | (((a ^ b | a ^ b) ^ ~(a ^ a)) & (b ^ b ^ a)
		& (a | b | b ^ a) | a) & (~(b ^ b | ~b) ^ (b ^ b) & (b ^ b)
		& b | ~a)) ^ b & a & ((a | a & ~a & (a ^ ~a)) ^ (~b & (a ^
		a) | b) & (~a | b) & b)) + 7070 * ~(b & ((a | (b | a) & a |
		~~b & ~(a | a)) & ~~~a | b)) + 1352 * ((b & (b ^ b) | ~((a ^
		a) & a) ^ ~(~b | b | a)) ^ ~(a & b | a) | ~b & (~b | a) |
		~(b ^ ~(~a & (a | b) & a) ^ ((~a ^ (a | b) | a) ^ a) & ~((b
		| b) & (b | a) & b))) + 52086 * ((~a ^ ~~(b & b) & ~~(a ^ a)
		& b & (a ^ b) & (a | b) & (b | b | b | b | (b | a) & (b ^
		b)) ^ a & ~~((a ^ b) & ~a) ^ (~(a & b) & b & b & (b ^ a) ^
		(a ^ b ^ a) & b & b & a) & (b | ~a ^ b & a | ~~a)) & ((a &
		(b | b) | (b ^ a) & a & b | b & b ^ a ^ b) ^ ~(a ^ b ^ b ^ a
		& b) ^ b ^ (b | b & a ^ b ^ b) & (a & b | b | b | (b ^ a) &
		(b | a)) & (a & a & b ^ a & b & (b ^ b) ^ ~((b ^ b) & a &
		b)) ^ (~~a | ~~(a | a)) ^ (b & (b ^ a) | (b | a) & a & b) &
		b)) + 63382 * ((~(b | (~b | a & b) & (b ^ b & a)) | (a | a |
		b | a) & (a | b | ~b) ^ (~b & a & a | ~(b & a)) | a & (b ^ b
		^ a ^ a ^ a ^ a ^ b)) ^ ((~(b ^ a) ^ a) & a | ~(a & (b | b)
		& (b | b))) & (~(a & a ^ a ^ a) | ~a | b | ~a | a ^ a | b ^
		b ^ a ^ a ^ ~(b ^ b)) | ~(a & (~~~a ^ (b & (a ^ b) | ~(a ^
		a))) ^ ~~((b | a | a) & (a ^ b ^ ~b)))) + 21505 * (a & (a |
		(b ^ b ^ (~a | ~b | b & a)) & b)) + 46768 * ~(a & (~((~b ^
		~a) & (~a | b | a)) & a | (a | b) ^ ~(a ^ ~a) | ~~((b | a) &
		~a))) + 16594 * ~~(a ^ (~a & (b | b) | ~a) ^ (a | a) & (b ^
		a) ^ a & a ^ (~(b ^ b & b) | ~b & b | a ^ (a | b))) + 165 *
		(~((~b & (b | a) | a | b | a) ^ ~(a ^ a ^ b)) & ~(a ^ a & a
		| b | b | b | a) & (b & ~b | ~(b | a) | (b | a) & (a | b) ^
		~b) ^ ~(a | a | b | a | a | (~b ^ a & a) & (b | b & b) | b)
		| ~(b | b)) + 26260 * ~(~((~(a | b) | ~a & b & b) & (~b ^
		~b) & ~a & (b ^ b) ^ b) & a) + 3944 * (b ^ ~((a & (a | a) |
		b) ^ (b | b | a) ^ (~a | b) ^ ~(b & b | b) & (~b | b | a) &
		~(b ^ a)) ^ ~(b ^ ((b | b) & ~a | b ^ (b | a)) ^ ~a ^ a)) +
		55251 * (~((a ^ ~(a ^ a & a & a)) & b) & (~b & a ^ b ^ b ^ a
		^ ~a | b | a & a & (b | b) | ~(((a | ~b) ^ b) & (a & b & ~b
		^ a)) | ~((b ^ (a & a | ~a)) & (~b | ~~b) & (~~a ^ (b | b) &
		a & a ^ b)))) + 39783 * (a ^ a & a) + 18563 * (~(~((b | b) ^
		a ^ b | b & a & (b ^ b) | ~(b | a) ^ b) ^ a) | ~(~b & ~a &
		(a | a) & (~~a | b ^ a ^ b | ~~(b & a)) & (~a ^ (b | b) ^ b
		| ~(a & b) | b & b ^ (a | a)))) + 61790 * (~(b ^ (a | (b |
		b) ^ ~~a)) & (((~(b ^ a) | ~(a | a)) ^ (a | a)) & a | ~a &
		(b | b | b) & b & (a | a & a | b ^ a | b)) | (a | ~~(b & b |
		a & b) ^ (a & (a ^ a) ^ (b | b) ^ ~a) & ~(a | ~a)) & (~(a ^
		b & b) ^ ((a ^ b) & ~a | ~(b | a)) ^ a & a & a ^ ((~b ^ b) &
		(a ^ a | a ^ b) ^ (a & b | a & b | a | b | ~a)) & (~~a | a &
		a ^ ~b | ~b & (b ^ b) ^ (b | a) & a))) + 24839 * ~(~((~b &
		~a ^ ~(b | a)) & ~(a ^ a | a)) & (b ^ a | a | a ^ b ^ ~b) &
		a & a & a & a & (~(~a | b ^ b) ^ ~~b ^ (a & a | a)) | ~a) +
		3271 * ~(b & b ^ b & ~~b ^ ~(a | a & a) ^ ~(a | a & a ^ b &
		b) | b) + 18466 * (~a & ~((a ^ b ^ b ^ b ^ a) & (~(b | a) |
		~b | ~a) | b)) + 43329 * (a | ~(~((a ^ b ^ b ^ (a | a)) &
		~a) & a)) + 4006 * ((~(~(b | b) | ~(a ^ a)) ^ (b ^ a ^ ~a ^
		b ^ b ^ a | ~(a ^ a) ^ (b ^ a) & ~a) | (~~b ^ b) & ~(a ^ a ^
		~a) ^ (a ^ b ^ b ^ a | a) & b) & b ^ (b | a & (~~a | b |
		~a))) + 34021 * (~b ^ a & b ^ (b | ~(~(~b | a) & (b | ~a) &
		~~b) | b)) + 39513 * ((~(a & b) | (b | b) & (b ^ b) | b ^ a
		^ b ^ a | ~((a & b | a & a) ^ (~a | a))) & ~~(a & b) & b ^
		(((b & a ^ a) & (a | b) & (b | a) ^ ~~(a ^ b)) & ~(a ^ ~a ^
		(b | b) ^ b & b) | (~a ^ a ^ (a | ~b) | b) ^ ((b ^ b | a ^
		b) ^ a) & b) ^ a ^ b) + 22234 * (~~(a & (b ^ a & a)) & (~~b
		& ~(b & b) ^ (a | a & a | ~b ^ b ^ b)) & b ^ (~(a ^ b ^ b |
		a) | ~((b | a) ^ (a | a) ^ (a | a ^ a))) & (~(~a ^ a | b) ^
		~(a | b & b)) ^ ~(~(a & a & a & b | ~(b ^ b)) & a & (a |
		a))) + 61238 * (((b & ((a | b) ^ b & b | b & b ^ a & b) | (b
		| (a | b) ^ a & a) & ~(b ^ b) & (b ^ (b | a))) ^ a ^ (~b ^
		(a | b) ^ (a ^ a) & a & a) & (~(a ^ a) | ~(b | a))) & ~b |
		(~((a ^ b ^ b ^ a ^ ~(b & a)) & b) ^ ~~(a & b)) & (a | ~~(a
		^ a) | a | b & a | ~a ^ a ^ b | b)) + 37553 * ~(~~(~(~b ^ b
		& b) ^ (b & (b ^ a) | ~(a ^ b))) & ~(~b & b ^ (~a | a | b) ^
		a & a ^ ~b | a & (a & ~a | b))) + 64123 * ((~~b | ~~((a | a)
		& (b | a)) | (b ^ b) & ~a & a ^ b | ~(b ^ a & a & b & ~~b |
		a)) & ~((~~~(a & b) ^ (~~(a | a) | a)) & ~~(a & b & (b | b)
		& (a & a ^ b & a)))) + 53063 * (~(~(a ^ b | b ^ b | b | b |
		b) | ~a) ^ (~a | a | a) ^ b) + 2576 * (~(~(~a ^ a & (b ^ a))
		& (~b & (a ^ b) | a & (b ^ b) | b & (b | a) | a & a ^ (b |
		a)) ^ ~~(~(b & b) ^ a & b & (b | b))) | b ^ (~~b & ~b & (b &
		a ^ a | b) | ~(~b ^ b | ~(b & b)) | a)) + 30312 * (b ^ a ^
		((a ^ a) & a & b ^ a) & a ^ (b & b & b | (a | b) ^ ~a) & (a
		^ b | b ^ a) & b ^ (~(b & b) | ~b) ^ ~(b & a) ^ b & b & (b |
		a) ^ (a | b | a & a) & a & (~(a | b) | a & b | b | b) ^ (b |
		a)) + 52013 * ~~~(~~(~b ^ (a | b)) | ~((a | b | a) ^ b ^
		~b)) + 2796 * (((~a ^ ~b ^ ~b & (a ^ a) | a & ((a | b) ^ a ^
		a)) ^ ~b ^ (a & b & b ^ a & b & b | a ^ b | ~b ^ ~a ^ ~~a ^
		(~(b & b) | a))) & ~~a | a) + 61217 * ((a | ~(a & ~(a ^ b |
		b & b) ^ a)) ^ (a & (b | (a & (a | b) ^ ~~a) & ~~b) | ~(b &
		b))) + 20926 * ~(a ^ (a | a | ~a & b & a & a) ^ (~~((a ^ a)
		& (b | b)) | ~((a | a | b & b) ^ ~a ^ b ^ a)) | a | ~~b) +
		65405 * ((~~b | ((~a & ~a | b) & b | b & a & (b | a) ^ (a |
		a) ^ (b | a) ^ ~((b | b) & a)) & b) & (~~((a | b) & (a | a)
		| (a | b) & (a | b)) ^ (b | b)) & a) + 28099 * ((~(a | ~(a ^
		b)) | ((a | b | ~a) ^ b) & ~(b | b) & (a | a) & ~a) ^ (~((a
		| a) & a & b) ^ b) & (b & ~a | b) & b ^ ((a & a ^ b & a) &
		(~a ^ ~a) | ~(a | b | b)) ^ ~(b ^ (b | a)) ^ ~(b ^ a & a) ^
		(b | a ^ a & (b ^ a)) & (~(b & b) & (a ^ b ^ b ^ a) ^ ((a |
		b) & (a | a) | (b ^ a) & a & a)) ^ (a ^ a | ~(a & b & b &
		~(a & b) & ~((b | b) & (a | a)))) ^ b) + 21101 * ((b | ~b ^
		b & ~~a ^ (a | a) | ~((~a ^ a & b) & a | ~~(b & b))) ^ (~((b
		& b | a | a) ^ b & b & b | b ^ ~a & b) | b ^ ((b | b) & (b ^
		b) ^ (b | a) & ~a) & a) ^ ~a & ((a ^ a & a | ~(b | b) | (a |
		a) & (b ^ b) & ~(b ^ b)) ^ b & (a ^ b) & a & (a | a))) +
		54250 * ~(a & ~(b ^ ~(b & a & (b ^ b)) ^ b)) + 37918 * ((~~a
		^ b ^ a | (a & ~((b ^ a) & (b ^ b)) ^ ~((b | b) & ~a) ^ ~(b
		& b) & ((a | a) ^ a)) & (a & a & b & b & ~~b ^ (b & (b ^ a)
		| b) ^ ~b)) ^ ~~b) + 62172 * ((((b | a | a | b | b) ^ ~~~a |
		~(a & a) & a ^ ~(a ^ b | b)) ^ b | a & a & b) ^ a) + 37982 *
		~((a & a & a ^ b) & (b | b) & ~(a ^ a ^ b ^ b & b ^ ~b) &
		(~(b & a) & (b ^ b) ^ ~(~b | b | b) ^ ~(b | a ^ b) & (a | a
		| b | b | b | a)) ^ ~((a & b & a ^ b) & ~(a ^ b) & b) ^ ((a
		^ a | b & a) ^ (a | b | a | a) ^ ~((a ^ b) & ~a) | (~(b | b)
		^ a & b ^ a ^ a) & (~(b ^ b) ^ a))) + 64067 * ((a ^ ~(b &
		~(b & a) | a) | ((a | ~((b | b) ^ a ^ a)) ^ b ^ ~~b & (b ^ b
		^ b)) & (~(a & a) ^ (b ^ b | a & b) | ~((a | b) & b)) & b &
		~(a | a | b & b)) ^ (a ^ (a | ~(b | a) | ~(b | a)) ^ (b | a
		^ a ^ (a ^ b | b | a & b ^ ~a)) | ~~(a & ~a & ~b))) + 38626
		* (b ^ a & (((b | b) & a | ~b | ~b) ^ ((a | b) & b & a | a &
		a & ~b) | b ^ ~a) & ((a ^ b | ~a) & (a & a ^ b) & ((a | b) &
		(b | a) ^ b) | b)) + 10675 * (~~((b ^ b) & (b & b & ~a ^ b)
		& ~(~(a | b) & b)) & (((~b | a ^ a) & b | a ^ (b ^ a) & a |
		b & ~(a & b) | ~b ^ b & a | ~a ^ b) & a | a));
    vm->memory[--vm->sp] = result;
    // std::cout << "[DEBUG] AND: 0x" << std::hex << a << " & 0x" << b << " = 0x" << result << std::dec << " [sp=" << vm->sp << "]" << std::endl;

    WORD next = vm->fetch_opcode();
    vm->handler_table[next](vm);
}

void VirtualMachine::op_or(VirtualMachine* vm){
    WORD b = vm->memory[vm->sp++];
    WORD a = vm->memory[vm->sp++];
    WORD aux0 = 64676;
    WORD result =19997 + 51099 * ~(((a ^ b | aux0 ^ b) & ~(b & aux0) ^
		aux0 & a & (b | b) & (a ^ a) & aux0 | ~(~a ^ (aux0 | aux0) ^
		~(b & b))) ^ a | ~(a | (b & a ^ b) & aux0) ^ ~aux0) + 35696
		* ~aux0 + 24226 * ~a + 5245 * ((~aux0 | ~b) & (~((b ^ aux0 |
		b & aux0) & (b ^ aux0 | aux0 | b)) | ~(~a & ~b & (b & b |
		~aux0))) ^ ~~((a & a ^ b & b | ~a) ^ aux0) | b & ~a) + 23409
		* (~((~(~aux0 & (b | aux0) & (a & b ^ aux0 ^ a)) | b & (a |
		a | aux0 | aux0 & aux0)) & (~~b & (b | a) & a ^ (~~b | b | b
		& aux0) ^ ~b)) ^ (~(b & ((b | a) & aux0 | ~a & (b ^ a))) |
		~((b | b) & (b | b) ^ aux0) ^ a) & (~aux0 ^ b)) + 2048 *
		(~((~a | b & aux0) & (a ^ (a | aux0)) ^ (~aux0 ^ aux0 | b |
		~b) | ~((b | aux0 ^ a) & ((a | aux0) ^ b & b))) | ~(a | ~(a
		& aux0 | ~a) & ~~(aux0 | a)) | (a | aux0 | ~((b & a | aux0 |
		a) ^ b)) ^ ~~~~(b & b)) + 51310 * (aux0 ^ (b | a) | (b &
		(~~b ^ ~(a ^ aux0)) ^ ~b | ~(a ^ a | b) & (~b | a | a) & ~(b
		& aux0) ^ (~(a ^ aux0) ^ (a ^ a | b ^ a)) & ~(a ^ aux0) & a)
		& aux0) + 22116 * aux0 + 7234 * (a & ((a | a & aux0 & (b ^
		b)) & ~(a ^ aux0 | b | b) & ((aux0 | aux0) ^ ~b | ~b & ~a |
		~b & b & b & a & aux0 & aux0) & ~(~~aux0 ^ (a | a) ^ aux0 ^
		(b & aux0 | aux0 | b) ^ (a | b) ^ aux0) | ((b ^ aux0) & (a |
		b) & (aux0 & aux0 ^ ~aux0) | b | b ^ aux0 ^ b | ~b & (aux0 &
		b | b) & (b & b ^ a ^ aux0)) & a)) + 15074 * (aux0 ^ aux0 ^
		aux0 ^ ~a ^ ((aux0 & aux0 & a & aux0 & (aux0 | a | aux0) &
		aux0 | b | ~(b | b | a | aux0)) ^ (b & b & (a ^ aux0) ^
		(aux0 & b | a | a)) & aux0 & ~~(aux0 ^ aux0) & aux0 | ~a)) +
		1940 * ((((aux0 | a) ^ (a | a) | b & b & ~b | aux0 & a | a |
		b | ~~b) ^ aux0 | ~(~~(b & aux0) | ~~b)) & ((~(a ^ aux0) ^ a
		| ~((aux0 | aux0) ^ b)) ^ ~~(~b ^ b)) & (a | ~(a ^ a) & ~a |
		a & (~(aux0 ^ a) | a)) & (~((b | ~(aux0 ^ b)) ^ a ^ ~a & ((a
		^ aux0) & ~b ^ a)) | a & b & b & ~b & a & b & aux0 & (a &
		aux0 & (aux0 ^ b) ^ ~(a ^ aux0)) & (~~aux0 & (b | b | a | a)
		| b | a | a | a | aux0))) + 40315 * ~(aux0 & (~((b ^ aux0) &
		(b ^ b)) & (aux0 & aux0 & (b ^ aux0) | aux0 & b ^ b) ^ ~(~~a
		& (aux0 ^ aux0 & aux0))) ^ a) + 24335 * ~~((aux0 & b & b &
		aux0 ^ (a & aux0 | a ^ aux0) | ~(~a ^ b & aux0) | ~~a ^ b ^
		a ^ aux0) & ((aux0 & aux0 & b | ~a | a ^ a) ^ ~aux0 & (aux0
		& b ^ b & b) ^ a)) + 13112 * (aux0 ^ ~(~((a | aux0) & aux0)
		& b & aux0 & (a & b ^ (a | b) | ~b & (aux0 | aux0)) ^ (~(b |
		b) ^ (a | a | a) ^ ~(aux0 | a) ^ a & b ^ (aux0 | b) | aux0 &
		(b | aux0 | aux0) ^ a))) + 30330 * b + 17825 * a + 54612 *
		(b | ~((~(b | (aux0 | b) ^ aux0 ^ b) | ~aux0 | b | ~(aux0 ^
		b)) ^ ~(~(~b & ~b) ^ ~a))) + 46714 * (~(a & ((~a | a ^ a) ^
		~~aux0) & aux0 & (a | a) & b & a & b | ~(aux0 & (a ^ aux0))
		& ~aux0 & (~(aux0 ^ b | b ^ a) | ~((b ^ b) & ~aux0))) &
		~(aux0 ^ (b | b ^ a) & (~a ^ ~a & (a | b) ^ (aux0 ^ a) & (a
		^ b) & aux0))) + 8117 * (aux0 ^ ~aux0 | ((aux0 ^ ~(b &
		aux0)) & ~(~b ^ (aux0 | b)) ^ ~(a ^ a ^ b ^ (aux0 & a | a &
		aux0)) | a & ~b | aux0) ^ ((a | b | a & a) & (~aux0 | a &
		aux0) | aux0 | ~~(~b ^ b)) ^ (aux0 | ~(a & b)) & b ^ (aux0 |
		~~(aux0 | a))) + 9389 * ~~~(~~((a | a) & b & aux0) ^ ~b ^ ~b
		^ aux0 ^ ~(b & aux0) & (a ^ aux0) & b) + 15671 * (~(b ^ ~(b
		| a) & a ^ ~a ^ a) | (~((a | b) & aux0 & ~(b ^ aux0) & ((a ^
		aux0 | aux0 ^ aux0) ^ (a ^ a | b & b))) | ~aux0 | ~(b & a))
		& aux0) + 51378 * (~(a ^ (~(b & aux0 | aux0 | ~a) | ~~~b &
		((a | a | aux0) ^ (a ^ aux0 | ~b)))) & (~(b & (~aux0 | aux0
		& b | aux0 & (aux0 ^ aux0) | (b ^ aux0) & aux0 & ((aux0 |
		aux0) ^ aux0 ^ b))) ^ ((~a | (a ^ a) & a & a & b) & aux0 |
		(b ^ (b | aux0) ^ ~(aux0 ^ aux0)) & (b ^ b) & (aux0 ^ aux0)
		& ~aux0 & (aux0 ^ ~(aux0 & a) ^ ~(~aux0 ^ b))))) + 51911 *
		~(~(b ^ ~a ^ ~a) | b & (aux0 | ~(b ^ b | b ^ aux0) ^ ~(aux0
		& aux0 ^ b ^ a))) + 29067 * ((~a | ~b ^ (b | b)) & ~(a ^ ((a
		^ b) & a & b ^ (a | b | b ^ b) ^ aux0 | a))) + 29917 *
		((~((aux0 ^ aux0 ^ (b | b) ^ (aux0 ^ b | aux0 | a)) & (b | b
		& a | b & b)) & ((a & a | ~a) & (aux0 ^ (b | aux0)) & ~a &
		(b | aux0) & (a | b | aux0) | ~(b | ~~aux0)) ^ aux0) & (aux0
		^ ~(aux0 ^ b) | ~(a & aux0 | ~b) & b ^ (~a & ~(b | a) |
		~aux0 | aux0 ^ aux0 & b) ^ b & ~~aux0)) + 14338 * ~(b | ~~(b
		^ a ^ aux0) & a | b | a) + 23742 * (b & b | (((b ^ aux0) & a
		| ~~b) ^ ~(a & b | b) ^ b) & (a | b & (a ^ a)) & ~(b & b ^
		aux0 ^ aux0 ^ aux0) | ~((b | b) & (aux0 | ~b) & aux0 & b)) +
		27567 * ((a ^ (~(b ^ aux0 ^ a & b) | ~a ^ b | ~a ^ b & aux0)
		| ~((a & a | aux0 & aux0) & (a | a) & a & a & ~(~aux0 | ~a))
		| ~((b ^ ~aux0 & (aux0 ^ aux0 ^ ~aux0)) & b)) & (a | ~b ^
		~(aux0 & (b & b | b | b | a ^ a | aux0)))) + 50458 * ((aux0
		& b & a | ~((~(aux0 & a) ^ a) & ~b) | b) ^ b) + 39803 *
		(~((~~a ^ ~b & a & b) & ~b | (b & b & b | a) ^ (~(b | a) |
		aux0 & aux0 | a & b)) ^ a & aux0 & aux0 ^ ~(aux0 & a & b) &
		(b | a) ^ b) + 58960 * (~a & a & aux0 & ((a & aux0 | b &
		aux0 | aux0 | a | a) ^ b) | b | (b ^ (b ^ a & b & (b ^ b) |
		a) | b) ^ aux0) + 28847 * (~(a ^ (~a & ~aux0 ^ (a | aux0) ^
		(a | a) ^ a | ~(b | aux0 | a ^ b) | (b ^ aux0 ^ ~aux0) &
		(aux0 | a) & (aux0 ^ aux0))) ^ (a ^ ~(aux0 | (b | aux0) &
		aux0) & (~(aux0 ^ aux0) ^ a & a & aux0 & aux0) & ((aux0 | a)
		^ aux0 & aux0 | a) | b | (((b | aux0) & (a | a) | a | aux0 |
		aux0) ^ (b | a | b & a) ^ aux0 ^ a & a) & ~~(aux0 ^ a & b)))
		+ 9822 * (aux0 ^ a & b) + 29256 * ((~~(a ^ a) | (~aux0 ^ (a
		^ b) & (aux0 | b)) & ~a) & a ^ ~a | b) + 56485 * ~b + 53889
		* ((aux0 | ~(aux0 ^ (a | a | a ^ b) ^ b)) ^ (aux0 ^ (a | b ^
		a) ^ ~(aux0 | aux0) | ~(aux0 & a | a | aux0) | ((aux0 | b) ^
		b) & a & b & a & a) & aux0 | ~(~a | a) | (b ^ b ^ ~((b |
		aux0) ^ aux0 & a) ^ aux0 ^ (~aux0 | aux0 & a) & (a | a ^ a))
		& ((aux0 & a ^ b & b | (a ^ aux0) & b | ~a & aux0 | (a | a)
		& (aux0 ^ aux0)) ^ aux0 & ~~(a & aux0))) + 39320 * ((b |
		aux0 | a ^ b & a & (a | a) ^ (~aux0 | aux0) | (aux0 & a ^
		(aux0 | aux0) ^ (b | a | aux0)) & a & ~~(aux0 & (a ^ a))) &
		~b & (a | ~~b) | ~(~(a | ~aux0 & (aux0 | a)) ^ (a | aux0 |
		aux0 | (aux0 | a) & b) ^ ~~~a ^ b & (~~b | aux0 & a ^ a & a)
		& ~~aux0)) + 59007 * ~~~(((aux0 | b) ^ a) & a & ~(aux0 ^ b)
		& ~(aux0 & aux0) & ~~(aux0 & aux0) & (a | aux0 | aux0 | b |
		b)) + 61935 * ~~((~((b & aux0 ^ b & a) & ~(b | a)) ^ (~(b &
		a) | b & b ^ a) ^ (a | (a | a) ^ (a | b))) & b) + 39986 *
		~~((aux0 & ~a & ~(aux0 & a) ^ (a ^ b & a | (a ^ a) & ~b)) &
		aux0 & ((aux0 | aux0 | a ^ aux0 | b & aux0 ^ ~aux0) & (~a ^
		~aux0 | (a | a) & (aux0 | aux0)) | ~((a | aux0 | aux0 | b) &
		b))) + 37206 * ~~(~~~(~aux0 ^ a) & ~((~a ^ a) & (b & a | ~b)
		| b)) + 7832 * (a & (a | (b ^ ~a & b | a) ^ ~((b | b) & (a |
		a)) ^ (~b | b & aux0 | (aux0 | a) ^ aux0 & aux0)) & (~~~(~(a
		| a) ^ ~aux0 & ~aux0) | ~(~(a ^ ~b) ^ aux0) ^ ~(~a & a & a &
		(b ^ b) ^ ~~aux0 ^ ~b & (aux0 ^ aux0)))) + 49260 * ~~(a &
		((a | aux0 | a & a ^ aux0 ^ a) ^ (aux0 ^ aux0 | b) & (b ^ b)
		& ~b) ^ aux0) + 16237 * ((~(b ^ ~~b) | ~b | (aux0 ^ b ^ (b |
		b) | ~(a | aux0)) ^ (a | a | b) ^ (b | aux0) ^ a & b) ^ a ^
		~(b & a & (aux0 | (a ^ aux0 | b ^ b) & (b & a ^ ~a)) & (~b &
		(aux0 & a ^ aux0) ^ ~a))) + 34656 * ~~~b + 48602 * (~((~(a ^
		a) ^ (b | b | b & a)) & ~(a & a) & ~(b ^ b) ^ ~(aux0 ^ b ^ b
		& b ^ (b ^ aux0) & aux0 & aux0)) | ~b | b) + 46296 * (((a &
		b | aux0 ^ a) ^ (b | aux0 | a | aux0)) & (~(aux0 ^ aux0) | b
		| a | ~aux0) & (a | aux0 & a | b | aux0 | ~(b ^ b)) & ~b &
		~a ^ ~~((~b | a ^ aux0) & ~(aux0 & b)) & (b ^ ~(aux0 | aux0)
		^ (~(aux0 & a) | ~aux0 ^ (b | a))) & ((a & (b ^ b) | ~~aux0)
		^ ~(b ^ aux0 | a ^ aux0)) ^ (aux0 | aux0 & ~~a & a | ~(~(~b
		^ a & b) | aux0 ^ a & a ^ b))) + 11003 * (((~~((a | a) ^ a ^
		aux0) | ~(a | aux0 | b) | (b ^ a) & b | ~a) ^ ~(~(a & b ^ a
		^ b) | b) | (b ^ aux0 | (aux0 ^ a) & ~a | aux0 | ~b) & (a ^
		~(aux0 ^ a) | ~aux0 & b | ~(b & b)) & ~(aux0 ^ aux0) & (b &
		b | a & b) & (~a ^ ~a | b) & (aux0 & ~b ^ (a & a | b & a) |
		(~b ^ a) & ~(b ^ a))) & ((aux0 | ~~(aux0 ^ a | a) ^ (b | a |
		b | aux0) ^ ~~a ^ (~a ^ ~aux0) & b) ^ ~~(b & (a | a | ~b) &
		~(a ^ aux0)))) + 39919 * (aux0 ^ b) + 6449 * ~(~((a | ~b) &
		a & a) & ~a) + 38024 * (~~b ^ a | b) + 25135 * (b & aux0) +
		32413 * (~((a ^ ~(aux0 & b ^ (aux0 | b))) & (a | aux0 & a ^
		(b | ~a))) & (a ^ (a ^ b ^ a ^ aux0) & a & ~a ^ (aux0 | b |
		a | a) ^ (aux0 ^ (aux0 | aux0) ^ a ^ a ^ (aux0 | (b ^ b) &
		~aux0) | aux0)) & aux0) + 1247 * (b ^ ~~((b | aux0 | a & b)
		^ (b | a | a ^ b) ^ aux0) | ~(~~(aux0 | b) | (b ^ a) & (b ^
		b) & ~a & ~b | ~((aux0 | b ^ aux0) & a)) ^ ~(b ^ b)) + 8916
		* ((a ^ (b & ~(a & aux0) & ~(aux0 | a) | b ^ b ^ (aux0 | a)
		^ a & a ^ a & b | a) ^ (~(a | b) | ~aux0 ^ a ^ b) ^ (a & b |
		a & b | a & aux0 ^ (a | aux0)) ^ ~~(a | b) & ~(b ^ (aux0 |
		a))) & (((a | a | a & b | ~a | b | a) & (~(b | b) | a & a &
		(a ^ a)) ^ ~(~a | a) & (a | b ^ a | a ^ b)) & aux0 ^ ~(~(b |
		a) & aux0) & a)) + 5476 * (~aux0 & ((~b | (aux0 | aux0 | ~b
		| ~a ^ (b | aux0)) ^ (b | a | b | b | ~b | b)) ^ b ^ ~~(aux0
		| a | b & a)) | b ^ ~(aux0 ^ a & b ^ ~a ^ ~aux0 ^ ~a) &
		(~~(aux0 & b ^ (b | a)) | aux0)) + 15197 * ((aux0 ^ a) & (~b
		& a & b & (a | b ^ aux0) ^ aux0 | a) & ~b & b) + 50853 *
		(~~(~a ^ (aux0 ^ a) & a & (a & b ^ ~a) & ~(b & b)) | b) +
		7994 * (~(b | b ^ a) | aux0 | ~((aux0 | aux0 ^ aux0) ^ b &
		b) & (aux0 ^ b) ^ ((aux0 ^ a | a | (a ^ b) & aux0 & b) & b |
		(~aux0 ^ a) & ~aux0 & (b ^ a) & (b ^ aux0) & (b ^ b) & ~b) |
		a) + 24946 * ((a ^ ((~~a ^ ~b ^ aux0 & b ^ (aux0 | a | ~a) ^
		b) & ~(b ^ b ^ (a | a)) & a | (a & a ^ ~b | b | a & b & (a |
		b) | aux0) ^ aux0)) & ~(aux0 | (~aux0 | b) & (aux0 | aux0) &
		(a & (aux0 ^ a) ^ ~b & (b ^ b)) ^ ~(~(b & a) & (a | b | aux0
		^ aux0)))) + 52845 * ((a ^ a & (b | ~b) & (aux0 & b & ~a |
		aux0 ^ a | b ^ aux0) | (a & (b | b) ^ aux0 ^ ~b | ~b) & a) ^
		(aux0 | b | aux0 | ~(aux0 ^ a) | ~aux0 | (b | aux0 ^ ~aux0 |
		~a) ^ (aux0 & a & b | aux0 | aux0 | b | aux0) ^ (aux0 | aux0
		| a ^ b) ^ b)) + 60713 * (a | b) + 28978 * ((a | b | aux0 ^
		~(a ^ aux0) ^ ~a & ~b) ^ aux0 & ~aux0 & a ^ (a | b | (b ^ a
		| ~a) ^ (a ^ aux0 | ~b) ^ (aux0 | a) ^ aux0 ^ b ^ (a | a)) |
		b | (~((b | aux0) & b ^ a) | a) ^ (aux0 ^ b & a | ((aux0 |
		b) ^ b ^ b) & (~aux0 ^ b)) ^ a) + 24282 * (((aux0 & a & ~b |
		aux0 ^ aux0) ^ (a & a | a) ^ aux0 ^ (~(a & b) ^ (b | a) &
		(aux0 | b)) & b) & (aux0 ^ (~(b ^ aux0 | a | aux0) | ~(aux0
		^ a) ^ (~aux0 | aux0 | a))) ^ b | a) + 24236 * (aux0 |
		((aux0 & aux0 & a & b | a) & ~(aux0 & b ^ ~aux0) | ~(~b &
		~~a)) ^ a ^ a ^ (aux0 | a) ^ b & b ^ (aux0 | ~~a) ^ (a ^
		aux0 | aux0 ^ aux0) & (aux0 ^ aux0 | ~aux0) ^ b ^ ~(~(a ^ a
		| ~a) ^ b & aux0 & aux0 ^ a) & (~a ^ (b | ~((aux0 ^ b) & a &
		a)))) + 42891 * ((b ^ ((a | a) & ~b & (~aux0 | b | aux0) ^
		(~(a ^ b) | b ^ b | b & aux0)) & ~(~b | ~aux0) & (aux0 ^ ~b
		| b ^ a | a ^ a) ^ aux0 & ~(~aux0 ^ (b ^ aux0) & (b | a))) &
		~(((aux0 ^ b) & a & b & b & (b ^ a ^ b ^ (aux0 | a | b |
		aux0)) ^ ~(b & a | aux0 ^ aux0 | b ^ a)) & (b | ~((aux0 |
		aux0) ^ ~b) | ~((b | a) & (a ^ a) & (b | b) & (b ^ b))))) +
		45152 * ~(((aux0 | b | b) ^ a) & aux0 ^ (b | b | b) ^ a &
		~(b ^ aux0) | ~(~(a & a | a ^ aux0) | ~(aux0 & aux0) | b |
		aux0 ^ b) | ~a) + 14513 * ~(((~aux0 | ~a) ^ (b | a) & b) &
		aux0 & ~b | a | (~~(b ^ b) | ~~(a & a) ^ ~~a ^ b) & ~((~b ^
		a ^ b | b & a & a & a) ^ ((a | aux0) ^ ~aux0 | a))) + 757 *
		(aux0 & ~(~((a | aux0) & b & aux0 & ~a | aux0 | a ^ a ^
		~aux0) & ~~(~b ^ a ^ a) & ~(a & b & aux0 | (a ^ b) & (aux0 |
		b)))) + 3242 * ((~(~b | a & aux0) & (~(b | aux0) ^ (a | ~b))
		^ b ^ aux0 & aux0 ^ (b | a) ^ ~(aux0 ^ aux0) ^ aux0 ^ aux0)
		& aux0) + 62957 * ((~(~((~aux0 ^ aux0 & a) & (~a | aux0)) |
		a | a & a | b | ~(a ^ a)) ^ b) & ((a ^ ((aux0 | aux0) ^ (a |
		b)) & ~(aux0 | a)) & b | a ^ ~(aux0 & ~aux0 | aux0 & b | a ^
		aux0) | ~(b & ~(a ^ b ^ (a | aux0) & (aux0 | a))))) + 28590
		* (~(aux0 | ~b) | a & ((~~a & (b | b) & b & aux0 | b ^ aux0
		| a | aux0 ^ aux0 ^ aux0 ^ aux0 | a) ^ ~b ^ b & b & a ^ ~(a
		| aux0) ^ b)) + 35090 * ~(b ^ (~~(a & ~aux0) ^ b | a |
		aux0)) + 57044 * ((b ^ ~((b ^ aux0) & b ^ ~(b & a) | a | a |
		b | b | (a ^ b) & (a | aux0)) & (aux0 | (~a ^ ~a ^ aux0) &
		(~~a ^ aux0))) & ~aux0) + 11680 * (~~((~aux0 | a ^ b) ^
		(aux0 | b | b) ^ (aux0 | b | aux0) & (a ^ b ^ ~aux0)) &
		((~(b ^ ~b ^ ~(b & b)) | (a ^ aux0 ^ a) & ~(b | aux0) | ~b)
		^ (a & ((b | a) ^ ~a) & b | aux0)) ^ a) + 37854 * (((aux0 |
		a) ^ ~(aux0 & b & ~aux0) | b | a) & ~~(b & aux0 & b & b | ~b
		^ ~aux0 | b & b ^ a ^ aux0 ^ (aux0 ^ aux0 | aux0)) ^ (aux0 |
		(a | ((b ^ aux0) & ~aux0 | aux0 & b & ~b) & ~~(a & aux0)) &
		~(((aux0 | b | b) ^ ~a ^ (b | a)) & ~~~aux0))) + 2766 *
		(~aux0 | (~(~(~aux0 & b & a) & (b ^ a ^ b ^ b ^ ~(a |
		aux0))) | b) & aux0) + 18602 * (~(a & (~(a | aux0) & (b | b
		^ aux0) ^ ~(~aux0 | ~b) | b)) ^ ~~(a ^ ~a & (b | b) | aux0 &
		a ^ aux0 & aux0 | (aux0 | a) & ~aux0) & b) + 16893 *
		(~(~~~(b | aux0) & (~a ^ b ^ a ^ (b ^ aux0 | b | b) | ~(b ^
		aux0) ^ ~~a) | ~(aux0 | ~(b & aux0) ^ aux0)) & ~~(((a |
		aux0) & ~b | aux0 ^ ~a | ~(a | a)) & ~(aux0 & a & aux0) & ~a
		& (b | b) & ~~aux0)) + 54418 * ~~(((aux0 ^ b) & ~b ^ (aux0 |
		b | b) | a) & ~(~(aux0 | aux0) | b & a ^ aux0) ^ (b | a)) +
		5782 * (a & ((~aux0 ^ aux0 & b ^ ~(a | a)) & ((b ^ a) &
		(aux0 ^ aux0) ^ ~(a ^ b)) & (a ^ aux0 ^ ~aux0 ^ b ^ ~aux0 ^
		b & a ^ a & a & aux0) ^ ~(~(a & b) & (a ^ b) & b) | (~(a ^
		(aux0 | a) | a ^ aux0 | a & b) ^ (b & b & a | ~aux0 ^ b) &
		~~~b) & ((aux0 | b | aux0 | b & a & aux0) ^ a))) + 25880 *
		~~((~~(~b ^ ~b) | ~(b ^ aux0 | a | a)) ^ a) + 15865 * (b &
		(aux0 | (aux0 ^ aux0) & ~b & (b ^ b)) & a & (b ^ b | b & a)
		& ~(a ^ a) ^ ~a ^ ~((~~(aux0 ^ aux0) | aux0 ^ ~(b & aux0)) ^
		a) & ~(a | (a ^ b | b | a) ^ (a ^ b | a & a) | a ^ aux0 ^ b
		^ (b | a | aux0 ^ a))) + 28474 * (aux0 ^ ~(~aux0 | aux0 ^ b)
		& ~a & (b & a | ~b) ^ (~aux0 | ~a ^ ~aux0) & aux0 ^ aux0 ^ b
		& (~(aux0 | b) & (b ^ b) & (a ^ aux0) | ~aux0 & b & aux0 &
		aux0) & ~(aux0 & (a ^ aux0 | aux0 | a & aux0 | ~a)) & (a |
		(a | b & b & b) & (b | aux0 | ~b) & (b & b | a & aux0) ^ (a
		^ b ^ (a | b) | b ^ a ^ b & b) ^ ~(aux0 & a & (aux0 ^ b))))
		+ 7708 * ~((a ^ (a | a) ^ ~~aux0 ^ b) & (~~b & (b ^ aux0) ^
		(b ^ ~aux0) & aux0) & (a & ~a & ~aux0 ^ aux0 ^ a ^ aux0 & a
		& a ^ (a | aux0) ^ (b | a)) | ((aux0 | aux0) & (a | a) &
		(aux0 & b | aux0 & aux0) ^ (a ^ a ^ a & b) & aux0 | b ^ a &
		aux0 & b & b ^ aux0 & b ^ aux0) ^ (a | ~aux0) ^ ~(aux0 & a &
		(b | aux0)) ^ aux0 & ~(b | b) & (a | ~aux0 | b | a)) + 25617
		* ~((~aux0 ^ ~(aux0 | aux0) | b ^ aux0 ^ ~aux0 | a & a | b)
		& (~~(aux0 | a) | (aux0 & b | aux0 & b) & (a ^ aux0 | a &
		aux0)) ^ a | ((((b | aux0) ^ b) & (aux0 ^ a) & (b | aux0) |
		b | b) ^ ((aux0 | a | a) ^ b | (a | b) & (a ^ aux0 ^ b ^
		aux0))) & ~~(aux0 ^ b) & b & aux0 & a) + 46576 * (a & a &
		~~((aux0 & aux0 | a & aux0) ^ (b | aux0 | aux0 & a)) ^ ~(b |
		a) ^ ~((aux0 & a ^ (aux0 | b) | ~~b) & b ^ ((~a | aux0 & b)
		& a | ~(~aux0 ^ aux0 & a)))) + 54428 * ((~(b | ~(b | b) ^
		(aux0 ^ b | aux0 ^ aux0) ^ ~b) | b ^ b ^ b & (b | aux0 | a |
		aux0) ^ aux0) ^ aux0) + 20199 * (((~(a & a) ^ (b ^ aux0) &
		~aux0) & (a | b ^ b & a) | ~((b | ~a) & (b ^ aux0 ^ (a |
		aux0))) | a) & (b & ~(b & aux0) ^ (a & b & (aux0 ^ aux0) |
		~(b | b)) | ~~~(aux0 & a)) & (b | aux0) | (~b ^ ~a | b & b ^
		aux0 | ~(a & aux0) & (a | a) & aux0 & b) ^ a ^ ((a | aux0) &
		a | (a | aux0) ^ b & b) & (~(b | aux0) | a ^ aux0 | aux0 &
		a) ^ (~b | a ^ b | (b | b) & aux0 & b) ^ a & ~b & a | ~aux0)
		+ 19864 * (a | ~~((a | a) ^ a)) + 14337 * (~(aux0 ^ b) ^ a ^
		a & a | ~(~(b & (b | aux0)) & (~(b | b) ^ b) & (b & aux0 &
		(aux0 ^ b) | ~(aux0 ^ aux0)) & (~(b ^ b) | a | a | a & b)) ^
		~((b ^ a ^ a) & aux0 & b & b & a & (b & b | b | a) & (aux0 ^
		b | b & b)) & (aux0 | (b | aux0) ^ b | ~(a & a)) & ~b) +
		30894 * ~~~(aux0 & (~b | ~(a & a)) ^ a) + 28317 * (aux0 ^ a
		^ aux0 ^ ~b ^ ((a | b ^ a | ~(a | b) | a) ^ ~((b & aux0 |
		aux0) & (b ^ aux0 ^ b)) ^ (~(~(aux0 ^ a) ^ ~~a) | aux0 | (b
		^ a & aux0) & ((aux0 | a) ^ (a | aux0)))) & ((~aux0 & (~(a ^
		aux0) | aux0 ^ b | ~aux0) | (a & a | a & b) ^ a | aux0 | a &
		aux0 | a ^ a) ^ ~aux0)) + 55398 * ((aux0 ^ (b | b ^ aux0 &
		~a ^ (~aux0 | a) ^ (aux0 ^ aux0 | b & aux0)) & b) & (aux0 ^
		((a | b) ^ a ^ aux0 ^ ~(a ^ b) | a & aux0 & (aux0 | aux0) |
		aux0 & aux0 & a) | b | ~b & (a ^ b) & b & a & (((b | aux0) ^
		(a | b) | ~b ^ (a | b)) ^ a) & b)) + 38456 * ~(b | ~(aux0 |
		b) & ~(a & b) ^ (aux0 | aux0 | a | (b ^ b) & (b | a)) | ~b |
		~b ^ ~b) + 25317 * (a ^ ((b ^ b & ~(aux0 ^ b) | b) ^ ((b &
		aux0 | aux0 & aux0) ^ (a ^ b | ~a)) & aux0 & (b & b ^ b & a)
		& aux0 & aux0 & b & ~(b ^ a) & b) & aux0) + 615 * ((~(b |
		~~a & (a ^ aux0) & b & a) ^ aux0) & (a & ~(a ^ (aux0 ^ b) &
		~a) ^ aux0) ^ a) + 10736 * ~((~(a | aux0) & (b ^ a) | (b ^
		aux0) & ~a ^ b & b & a) ^ (a | ~~~a) ^ (b | aux0 | b) & ((b
		| aux0) ^ b ^ b) ^ a ^ ~a & b ^ aux0 & ~(b | b) ^ b ^ aux0 &
		~(b & a ^ ~b ^ b & b ^ aux0 ^ b ^ (~aux0 ^ b ^ b | b & b ^
		~aux0))) + 64914 * ((a ^ (~(~(b ^ a) & (aux0 | aux0 | ~a)) |
		a) ^ (~(a & aux0 & ~b) & aux0 & a & aux0 & (~a ^ (b | aux0))
		| b | b ^ a | aux0 ^ aux0 & a | ~~(b | a))) & ~~((b ^ b |
		~aux0) ^ (b | ~a) ^ (a | b | b | aux0) & ~(b | a) ^ (~b | a
		^ a) & a & (~(a ^ a) | ~aux0 & ~b))) + 26404 * (~(a ^ ~~(b &
		b) | ((~b | b) ^ ~(aux0 & b)) & (aux0 & b ^ aux0 & a ^ b)) &
		((aux0 | aux0 | a | b | (b ^ a) & b & aux0) & (b | aux0 | a
		^ aux0 | (b ^ aux0) & (aux0 ^ aux0)) & ~(a & aux0) & (a ^
		~(b | b)) | ~(aux0 | aux0) & (~a | ~aux0) & ~~(b | a) ^ b ^
		~~a ^ a) | (aux0 ^ a ^ ~(aux0 ^ a & a) | a & (b & a ^ a) ^
		~~aux0 ^ ~(aux0 ^ b) ^ (~aux0 & (aux0 | a) ^ ~a) & ~(b ^
		aux0 & a)) ^ (a | ((b & a | b) & a ^ b & b ^ aux0 ^ (a | a)
		^ ~a) & ((a | ~aux0) ^ a ^ a ^ a ^ b) & ~~(aux0 | b))) +
		34114 * ((b ^ aux0) & ~((b ^ ~(a | b ^ aux0 ^ ~aux0)) &
		(~~~a ^ (b ^ b ^ ~aux0) & (a ^ a & a) | a))) + 43431 * ((a ^
		(~((a ^ a | b & aux0) ^ ~(a | aux0)) ^ (a & (a | b | a & b)
		| a ^ b | a) | ~b)) & b) + 39448 * ~(~a ^ a) + 20077 * (a ^
		~(~~aux0 | (aux0 | b) ^ aux0 & b | a ^ ~~a | a) ^ ~(aux0 &
		b) ^ (aux0 | aux0 & ~(b & aux0) & aux0 & aux0 ^ (b | ~~a & b
		& b & a))) + 51647 * (~~b ^ ~(~aux0 | ~(a & a ^ b & a ^
		aux0) ^ (~(aux0 | b ^ b) | a & b | aux0 & aux0 ^ ~b))) +
		12604 * ((a ^ ~(a & aux0 | a & a) ^ (aux0 ^ aux0 | ~b) ^
		aux0 & b & a ^ (a ^ b | a ^ a | b) | a ^ ~b & ~((aux0 ^ b) &
		(b | b))) & b ^ ((a ^ ~~a & ~~b | ((b | b ^ b) ^ b) & b) ^
		(a | a) & ((aux0 ^ ~b | ~(b & aux0)) ^ a) | a ^ ~(b & b ^
		(aux0 | b) ^ ~a) & ~b & a)) + 19524 * ((b ^ (~(a | ~a) | ~(a
		& ~aux0)) ^ ~(~a & ~aux0 | ~(b ^ a)) ^ b) & (aux0 | ~(aux0 &
		a) | a | a ^ b | a ^ (~a | aux0) | aux0) | ~(~(~~(aux0 |
		aux0) ^ (aux0 | b) ^ b & aux0 ^ ~(b | a)) ^ ~(b | a) ^ a & b
		^ (b | b) ^ b ^ ~~aux0 ^ a ^ (aux0 ^ b) & (b | aux0) & (aux0
		^ b) & (a | a))) + 38251 * ~((~~a | ~(~((aux0 | aux0) ^ aux0
		& b) & a & (aux0 | aux0) & (a ^ aux0 ^ ~aux0))) ^ (~(aux0 ^
		b) | aux0 | ~~(aux0 & a) ^ a | aux0)) + 15665 * (~(~(~(b ^
		a) | ~~a | a & a ^ b ^ b | a) ^ a) | aux0 ^ (aux0 | aux0) ^
		b ^ ((aux0 | aux0 ^ b) ^ (b & b | a | b) | ~(aux0 ^ a ^ aux0
		^ b))) + 62567 * ~~(((b | b & aux0) ^ b ^ ~aux0) & (~(b ^ b
		^ aux0 ^ b) | ~((a | b) ^ a)) & ~(aux0 ^ (aux0 ^ b | b ^
		aux0) ^ (a | aux0) ^ aux0 ^ b ^ (a | a) ^ a ^ aux0)) + 34456
		* ((((aux0 ^ b | aux0 | aux0) & b & aux0 | a) ^ a) & (~b &
		(aux0 ^ b) ^ ~a ^ ~~~a) & (~(aux0 ^ aux0 ^ a ^ aux0) | aux0
		| ~aux0 | b & aux0) & ~(b & b & a) & (a | a) & b & (b | ~(b
		| a) | b | a | (((aux0 ^ a) & (a | b) ^ a ^ (a | aux0)) & ~b
		^ b) & ~~(a | b | aux0 | ~(a | a)))) + 57932 * (aux0 | ~~((b
		^ aux0) & (b ^ a) ^ aux0 | ~aux0 ^ ~b ^ a & b) | ~((b ^ b ^
		b & aux0) & (b & a | aux0 | aux0)) ^ (b ^ b | b) & ~b & ~(~a
		| a & b) | ~(~~b & a)) + 22506 * (aux0 ^ (b ^ aux0) & ~(a &
		aux0 | b) & a & ((a | a | aux0) ^ a) & (b ^ aux0 | ~(aux0 &
		aux0) & (a ^ b) & aux0 & b)) + 1576 * (((~a & ~(aux0 | a) ^
		(~(aux0 ^ aux0) | (a | a) ^ a & aux0) | (aux0 & aux0 ^ a &
		a) & aux0 | aux0 ^ aux0 ^ aux0 ^ aux0 ^ (b | aux0) ^ ~aux0)
		^ ~(aux0 ^ (b ^ aux0) & (b ^ aux0) | a) | ~(~(b | a & (b |
		a)) | (b | a) & (b | a) & (a | a) & (aux0 | a | aux0 | b) &
		(aux0 ^ b | aux0))) ^ (~(((a | a) ^ (a | a | ~aux0 |
		~~aux0)) & ((a | aux0) ^ ~b | b) & (a ^ aux0 ^ aux0) & aux0)
		| a)) + 6699 * (((a | aux0 | a | ~b & (b | aux0)) ^ aux0 ^
		~~b ^ (aux0 ^ a ^ a ^ aux0) & (aux0 | a | ~a) | (b | a ^ (a
		& b & a | aux0 ^ aux0 ^ a ^ a)) & aux0) ^ (a | (b ^ ~((~a |
		~b) ^ ~(b ^ a))) & aux0 & aux0)) + 18491 * (b | ~(b ^ ~(a &
		b) ^ (aux0 | a) ^ a ^ a ^ (~aux0 | a ^ a | aux0 & b | a & b)
		^ ((a | a | b ^ a) ^ aux0 | (a | b | a) & aux0 & b & b))) +
		40122 * (~(aux0 ^ (a ^ a ^ ~b | aux0 ^ b ^ a) ^ aux0 & b &
		~a & a) & a & ((~(b | ~aux0 & b) | (aux0 ^ a) & (aux0 | b) &
		(~b ^ aux0 & a) & a | (aux0 & a ^ aux0 ^ b | ~b) & (aux0 &
		~a ^ (a ^ a) & (aux0 ^ a)) & a) ^ aux0)) + 29349 * (~(aux0 ^
		((b ^ aux0) & aux0 & b ^ a & b & aux0 | (b ^ a | aux0 & a) ^
		b ^ b ^ (b | aux0)) ^ b) ^ ((~a ^ a) & ~~aux0 | ~(~~(aux0 &
		aux0 ^ ~b) ^ (~(aux0 & b) | b & (b ^ a) | ~~aux0)))) + 25543
		* ((aux0 & aux0 ^ (~(b | a | a | aux0 ^ a) | ~a) & ((b &
		aux0 | b) & ((b | a) ^ (aux0 | a)) ^ b ^ a ^ ~aux0 | aux0))
		& aux0) + 55012 * (b ^ ~(~(aux0 ^ aux0) & (a & a ^ aux0) &
		~(~aux0 ^ (b | aux0)) | aux0) ^ a) + 41352 * ~(b | a | ~~(a
		^ aux0) | aux0 | aux0 | aux0 | ~(b ^ b) | b & ~a | ~a ^ b ^
		~(a | a) ^ (a | a | b & b | ~b ^ aux0 ^ a)) + 19461 * (((a |
		a) & (b | b) & aux0 & aux0 & aux0 & b | ~(b ^ b | ~b) | ~b &
		aux0 & ~(b | b) & (aux0 ^ (a | a) ^ ~aux0)) & ((~a | a & b ^
		(aux0 | aux0)) ^ (b ^ aux0 | a | b) ^ (aux0 ^ aux0 | aux0) ^
		~a ^ a ^ a) ^ ((a & (aux0 & a | a ^ a) | ~~(aux0 | aux0)) &
		(aux0 | ~b & (a | b) & b & aux0 & (aux0 | aux0)) | ~(b & b)
		^ aux0 ^ ~~aux0 & ~(aux0 | aux0) ^ (aux0 | ~a & aux0 & b) &
		(aux0 ^ aux0 | ~(b | b))) | ~aux0) + 1316 * (~(~(a & aux0 &
		a) & aux0 | aux0) | ~(~(b & aux0 | b ^ aux0) ^ b & (b ^ aux0
		^ aux0 ^ b)) & (~(a | aux0) ^ aux0 ^ ~(~a ^ aux0) | ~~aux0 ^
		~b & a | (aux0 ^ aux0 | aux0 ^ b) & (aux0 ^ aux0) & (b ^ a))
		| aux0) + 45008 * ~~(aux0 & ((b | aux0 | a) & a ^ ~a & aux0)
		^ ~((~b | ~a) ^ (a | ~b) ^ ~~~aux0)) + 35784 * (aux0 |
		(~~~aux0 | ~(~~a & (a & b | ~b) & (b & b ^ aux0 ^ b))) ^
		~aux0) + 19999 * ~~((~a ^ (~a | b & a) & (~a ^ aux0 ^ aux0)
		& (aux0 | a)) & ~((aux0 | a) & a & b | (b | b) & (aux0 |
		aux0) | b)) + 6269 * ((aux0 & a & aux0 & (aux0 ^ aux0 & a) ^
		~((a | a) ^ ~aux0) | (aux0 & aux0 ^ b ^ b ^ (aux0 | b)) & (b
		| ~b)) & ((~b | ~b) ^ (aux0 & b & (a | a) ^ ~~b) & (~(b &
		aux0) | (aux0 ^ aux0) & (aux0 ^ a))) & b & (aux0 & b & b & b
		| aux0 | a | (~a ^ aux0 & b) & ((b | a) ^ (b | b))) & ~~(b |
		a | a | aux0) & (~(~aux0 & aux0) ^ ~a) ^ (aux0 | ~(~((b ^
		aux0) & ~b) | ~(b ^ a ^ (a | aux0))) | b)) + 61709 * (~b ^
		(((~aux0 | b | a) & (aux0 ^ a ^ a) ^ ~(aux0 & aux0 & ~a)) &
		(~~(aux0 | a) | ~a & (b & aux0 ^ a & aux0)) ^ ~(~b & ~a) &
		aux0 ^ (~~a | a & b | a) ^ a) & aux0) + 25088 * (~~(~((aux0
		^ b ^ a) & b & a & ~a) | ~(b & b ^ ~aux0) & ((a | b) ^
		~aux0) & aux0 & (a | aux0)) | ~(b | ~~(~aux0 & (aux0 ^ b)) &
		(aux0 ^ ~b | a ^ aux0 ^ b & aux0) & (~b ^ (a | a)) & ~a &
		b)) + 26507 * ((~(((a | aux0 | a | b | ~b) ^ aux0) & ~~(b |
		aux0) & ~~(a | a)) | (b | a & a) & b | aux0) & a) + 28433 *
		((~~(a & a) ^ ~(b | aux0 | b) ^ ~(aux0 | b | aux0 & aux0) ^
		~aux0) & (~((~(a & a) | ~aux0 | ~aux0) & b & (b & b ^ aux0 &
		a)) ^ ~((a ^ a) & (aux0 ^ b) & (b | a | b & a)) & aux0) |
		aux0 & b) + 7688 * (~(a & (~(b | aux0) & ~(a ^ a) & a ^ a))
		^ aux0 ^ ~(b & (~(b ^ aux0) | (b | aux0) & ~aux0) ^ a)) +
		18171 * ((a & (~~a ^ (b & b | a ^ b) & ~(a ^ aux0) & (~(b |
		a) | a | ~b)) | b) ^ a) + 29600 * ((~aux0 ^ (~aux0 & ((b |
		b) & a ^ ~(b | aux0)) | (~aux0 ^ (a | b)) & (b | a | a) &
		(aux0 | ~(a ^ b)))) & a & ((a ^ a ^ aux0 & a | aux0 & (b ^
		a)) & ~a | b | aux0 | a) | b) + 31515 * (b & (~~(~b | b) ^ a
		| ((~aux0 | ~aux0 | b | aux0 | ~a) ^ (~b | aux0 | b) & ((b |
		aux0) ^ aux0 ^ aux0)) & (~~(aux0 & a) | ~a)) | ((~a | ~b) ^
		~~~a) & a ^ ~aux0 ^ aux0 ^ (~~(a ^ aux0) | ~(a & aux0) | b &
		b | aux0 & b) ^ b) + 59260 * ~(b | (~b ^ ~~(a ^ aux0 ^ aux0
		& b)) & a) + 10084 * ((~b ^ ~(~~(a ^ aux0) ^ ~(b & a) ^
		aux0)) & ~aux0 ^ ~(aux0 ^ ~a | b ^ aux0 | b | b) ^ a ^ ~((~a
		| aux0 & b | (a | a) ^ b) & ~(a & a ^ (aux0 | aux0)) | (a ^
		b | aux0 | a) ^ ~(a | a) ^ (~a | aux0 ^ aux0) & aux0)) +
		6844 * ~(a | aux0) + 57761 * ((aux0 | aux0 | ~~(aux0 & (b |
		b)) ^ b & (~aux0 | aux0 & a) & (~b ^ (b | aux0 ^ a))) & aux0
		& (a ^ aux0 | ~aux0 | ~(a & b)) & (a ^ aux0 ^ aux0 ^ ~a ^ b
		& aux0) & (~((aux0 | aux0) ^ ~aux0) ^ ~(b ^ b & b)) | b) +
		15763 * (~(b ^ (~(a & b) ^ ~~b | ~(aux0 | b | a & aux0)) |
		(~a | a ^ a | aux0 & b ^ aux0 & a) & (~aux0 & b & a | ~aux0
		| aux0) | b) & ~(~(aux0 | ~b ^ aux0 ^ b) & b & (aux0 & a & b
		& aux0 ^ a ^ aux0 & aux0)) & ((~(b | b | a) | b & ~b | aux0)
		^ a ^ b & ((a & b ^ b & aux0) & b & a & aux0 | ~aux0))) +
		18141 * ((~(a & b ^ aux0) ^ (~~a | a) ^ ~((aux0 ^ aux0) &
		aux0 & a) ^ ~((b | a) & aux0 & a) | a & b ^ ~b ^ (a | a |
		aux0) | (a & aux0 ^ (a | aux0)) & (aux0 & b | ~aux0) |
		~(aux0 | b) & (a ^ b) & (aux0 ^ aux0) ^ ~b ^ (aux0 & b |
		aux0 ^ a) | ((b ^ a) & ~b ^ b) & b | ~((b ^ a) & ~a & ((b |
		aux0) ^ a)) | ~(~~~aux0 | a | ~(a | b))) & ~~~b) + 24040 *
		~(~((aux0 & a | b & aux0) & (a | a | aux0 ^ a) & ((a ^ b) &
		(b ^ b) ^ (a | aux0 | ~aux0))) ^ ~(b ^ ~a | b) ^ ~(b &
		~(aux0 ^ b | a | a) ^ ~a & b)) + 56812 * (~(~(aux0 & aux0 &
		~b & (a & a ^ ~b) & (a | aux0) & a & aux0) ^ (aux0 & (aux0 ^
		a) & a & (a ^ aux0) | a) ^ ~(a & ~a & (aux0 ^ a))) | a ^
		(aux0 | (b | ~aux0) ^ aux0) ^ (~a ^ a ^ (b | b | b & aux0))
		& aux0 ^ a ^ b) + 35553 * (((b | (~a ^ ~a) & (aux0 | a) &
		(aux0 ^ a) ^ a ^ a ^ (a | b) ^ (b & b | a) ^ (aux0 | ~aux0))
		^ a & a) & ~~(~(aux0 | b & aux0 | a | b) & a)) + 47626 *
		~~(a ^ a ^ (a & a ^ a) & (a | b) & ~aux0 | aux0 & ~(a ^ a) &
		(~aux0 ^ b ^ aux0) | ~(((a | a | b) ^ ~(a & aux0)) & b)) +
		61366 * (~aux0 | ~~(~~a & (a ^ aux0 ^ aux0 ^ aux0) | a) &
		~((b & aux0 & aux0 | ~a) & a ^ (aux0 | aux0 & (b ^ a & a))))
		+ 9082 * ~(~b & ~~(b | b)) + 56461 * (a ^ ~(~(b & (~a | b |
		aux0)) & (a & ~b ^ aux0 & (b | a) | a)) & (b & a ^ b & (~b ^
		a & aux0 ^ b ^ aux0))) + 23673 * (aux0 ^ a ^ aux0 & a ^ b) +
		21494 * (b ^ aux0 | ~(~(~~a | a & a ^ b & a) ^ aux0 ^ ~((b ^
		aux0) & b & ~(b | a) | ~~a))) + 37181 * ((a | ~a) & (a |
		aux0)) + 64220 * (((aux0 & ~(b & a) ^ (aux0 | a ^ b) ^ a) &
		(b | b) ^ aux0 ^ ~~((b ^ aux0) & (a ^ a)) ^ (~~(aux0 & a) ^
		aux0 & (a ^ a | b ^ b)) & a ^ b ^ ~(aux0 & a ^ b ^ ~(b ^
		b))) & (a & a ^ aux0 & (a | aux0) ^ aux0 ^ ~b) & (aux0 | a &
		aux0 & aux0 & aux0) & (a ^ (b | aux0)) & aux0 & ((aux0 & a |
		aux0 | aux0) ^ aux0) & (b | ((aux0 | b) ^ (aux0 | a)) & (a ^
		b | ~a) ^ ~(aux0 | b) & aux0 & b ^ ~(b & a | ~b) & (b & aux0
		^ b ^ ~~b))) + 49309 * ((~~(~a | b) ^ ~~(aux0 | ~aux0) ^ ((a
		| b) ^ b & b | a & aux0 ^ aux0 ^ a | aux0 ^ aux0 & (b | a))
		& (b | ~(~aux0 & (a | b))) | ~(a | b | aux0 | aux0) ^ a | b
		& aux0) ^ ((~~(b & aux0) ^ ~(aux0 | b) ^ ~(a ^ a) ^ aux0) &
		~(~a | a ^ aux0 ^ b ^ ~(aux0 ^ aux0)) | ~aux0)) + 4882 *
		~(~b ^ (((a | a) ^ aux0 | b) & (~a & a ^ (aux0 ^ a | ~b)) &
		(a & (a ^ b) ^ (aux0 ^ b) & b) | ((b ^ b) & a & aux0 & a & b
		| (b | aux0) & aux0) & ~((aux0 ^ aux0 | ~a) & ~(b | a)))) +
		39246 * ((~(aux0 ^ ~a) | ~(~aux0 & (b ^ aux0) | aux0) | a) ^
		a & ~a & (b ^ ~~a) & a | b & ((aux0 | (b ^ a | ~a) ^ (~a |
		a) & a) ^ b)) + 36117 * (~(b & a & aux0 & ~(a ^ a) | ~(a ^
		b) ^ ~~aux0) & (aux0 & b & b & (aux0 | b) & a | b) & ((~aux0
		| b) ^ ~a & b & a & a | b & (~(b ^ a) ^ ~(b ^ b)) | ~(~(a |
		aux0) ^ (a | a | b & b) | b)) | a | ~(a ^ b & a ^ ~b ^ a |
		(aux0 & aux0 & aux0 ^ aux0 ^ ~a) & (~b ^ ~a ^ b))) + 46314 *
		((~(~~(a ^ b) & a ^ (b & aux0 | (aux0 ^ aux0 ^ (b | b)) &
		a)) | ~(((a | b) ^ ~b) & (a & aux0 ^ aux0 ^ a) ^ ~b) | b) ^
		~(((b | aux0 | ~b) ^ (a ^ a) & (b ^ aux0)) & (~aux0 | ~aux0)
		& a & aux0 & (b ^ aux0) | ~(aux0 ^ b) & (aux0 ^ aux0) & (b |
		aux0) | a & (a ^ b) | b & a ^ (b | b) | ~(((b | a) & ~aux0 ^
		~aux0 & (a ^ aux0)) & (~b ^ a ^ a) & (aux0 & b | b)))) +
		6315 * ~(((aux0 & aux0 & aux0 & (aux0 & b | aux0 | aux0) |
		(aux0 | aux0) & a & b | b | b) ^ (b & aux0 & aux0 & b ^ b &
		b & b & aux0 ^ (aux0 | b | aux0 | aux0 | ~a ^ b & b) |
		~(~aux0 | ~a) | ~a ^ aux0 ^ b ^ (a | a | ~aux0))) & (~aux0 |
		(~b ^ ~~b) & ((aux0 & b | ~aux0) ^ aux0)) & (b | ~((a ^
		aux0) & (aux0 ^ a)) | b | a | aux0 | b)) + 46104 * (~~(b &
		~(b ^ ~a) | ~b) & ~(~(a & (~b ^ (a | b))) & ~(a & a & ~aux0
		& ~(b & b)) | aux0)) + 31203 * ~~(((~a ^ ~b) & (aux0 ^ a ^
		aux0 & a) ^ (a ^ aux0 ^ (a | aux0) | aux0 | b | ~a) ^ (a ^ b
		| aux0 & ~a) & a) & (~b | b)) + 44027 * (~(a | aux0 ^ ~aux0
		^ ~(~aux0 & b & b)) | a | (~((b | b) ^ b ^ a) ^ aux0 & aux0)
		& (b ^ (a | ~a)) & (b ^ aux0 ^ a) & aux0 | a & aux0 & (~(a ^
		a) ^ ~b)) + 33980 * (~(~~(aux0 & b ^ (aux0 | a)) & (aux0 &
		aux0 | a ^ (b | b) ^ a & b & aux0)) ^ ((~b ^ b & aux0 ^ ~~b)
		& (b & b ^ aux0 ^ b) & (b | aux0 | a | b) | ((aux0 | a) ^
		~aux0) & (b ^ a | aux0 ^ aux0) & b) ^ ~((a | aux0) & (a ^ a)
		^ ~(b ^ a)) & aux0 & (a ^ a ^ (b | b) ^ aux0 & a ^ a & aux0)
		| ((b ^ aux0 ^ b | a) ^ b | ~(aux0 & (aux0 | b))) & (aux0 |
		b | aux0 & ~aux0 | ~b) ^ (~aux0 ^ ~aux0) & (~b | a | a) ^
		~(b & aux0 & a) ^ ~b ^ (a | ~~b & ~~aux0) & (aux0 & b & b &
		(aux0 | ~a) | b & ~(aux0 & a))) + 31327 * (a & (((aux0 ^ a)
		& ~b & a & (aux0 | b) | b & aux0 ^ b ^ b ^ (a | aux0) ^ a &
		aux0) & ((aux0 | a) ^ b ^ b ^ a & aux0 | ~b ^ a ^ aux0 ^
		aux0 ^ b ^ ~a) & (~a | a) & (b ^ (b | a ^ b) ^ ~~b) | b)) +
		16876 * (~aux0 & (a & b & (b | aux0) ^ (b | a) & a & a |
		aux0 ^ a & aux0 | a) & (a ^ a ^ ~(b & b)) & b & (((aux0 & a
		| aux0) ^ a) & ~((a ^ a) & (b ^ a)) & ~a ^ (a & (aux0 | a) &
		(a ^ aux0 | a & a) | aux0) ^ (aux0 & (b | a) ^ (~aux0 | aux0
		^ aux0) | a ^ aux0) | ((~aux0 | aux0) & (aux0 ^ a ^ (a | a)
		^ (aux0 | b | ~a)) ^ ~~~aux0) & ~((aux0 & aux0 | aux0 | b |
		(aux0 ^ a) & (b | a)) & (aux0 | aux0 | a & aux0 & aux0)))) +
		13326 * (((aux0 | ~((a | b) & aux0) | aux0 | a & aux0 | a ^
		b) & ~((b | b | aux0 | ~b) & ~(a ^ b ^ b ^ aux0)) | aux0 |
		(~(b & b) | ~b | aux0 ^ b | ~(a | a | ~b)) & ((~(aux0 ^ a) |
		~(aux0 & a)) ^ ~(a & a) ^ (b | b) & ~a)) & ((a & (b & aux0 ^
		b) ^ (~aux0 ^ ~a | (b | a) & (a | b))) & ~(aux0 | aux0) ^ (a
		| ~(a ^ b) & (aux0 & a | ~b) & (aux0 & b | a | a ^ a ^ aux0
		& aux0)) | ~b ^ (((a | aux0 | b) ^ ~~aux0) & (a | aux0) &
		((b | a) ^ (a | a)) | aux0))) + 60058 * (~((a ^ b) & (a |
		aux0) & (a ^ aux0) & aux0 & aux0 ^ aux0 ^ (~(a | b) | aux0 &
		b ^ aux0 | a)) ^ ~(a ^ ~b ^ a ^ a) & ((a | aux0) ^ b | b ^
		aux0 ^ aux0 | b & aux0 & aux0 & b ^ aux0 & (b ^ b)) ^ (b &
		aux0 & (a ^ aux0) ^ (b & a | a ^ aux0) | ~b | b & b | ~a & b
		& a | aux0 ^ ~aux0 ^ ~(b | a) ^ aux0 & aux0) ^ ((b ^ aux0 ^
		b ^ a) & (~a ^ b ^ aux0) & (~a | (b ^ aux0) & a) & ~(~(b |
		b) & b) | ~((b & a ^ ~b | b | aux0 | aux0 & aux0) ^ (a | a |
		b & aux0) ^ (aux0 ^ aux0) & ~b) | ~((~aux0 ^ (b | b) ^ (b |
		~a)) & (b ^ (aux0 ^ a | a ^ b))) & b)) + 14991 * ((a | a & b
		& a | b ^ b | b | ~a) ^ (a & (aux0 | b) ^ aux0 & a | aux0) ^
		(~(a ^ b) | ~(aux0 ^ aux0)) & (b | ~~b) & (b ^ (b | aux0) ^
		(b | aux0) ^ a ^ aux0) & ~(a ^ a) & a ^ a & (~(aux0 | a) ^
		~(b | aux0)) & (a ^ aux0 ^ (b | a) | a | a) & ~(~aux0 &
		(aux0 | a & b) & (b | ~(b & aux0))) | (aux0 ^ ~(a & a &
		(aux0 | aux0) | (b ^ aux0) & a & a)) & ~((b | a) ^ (~b ^ b &
		b) & (aux0 | b) & (b ^ aux0)) & ~b) + 21138 * ((b & (aux0 ^
		a | ~aux0) & (a & b ^ aux0) & (~aux0 | aux0 & b & b) | a) ^
		(~a | ~b ^ ((a | aux0) & (a | a) | ~b)) ^ a ^ (~~a | aux0 ^
		a & aux0 | ~(aux0 & aux0)) ^ (b | ~a & aux0 | (a | ~(aux0 &
		aux0) | aux0 | ~(a & aux0 & b ^ (aux0 | b) ^ ~aux0)) ^ b)) +
		25377 * (((b & ~b & ~(a ^ a) | a & (aux0 ^ aux0 | ~a) |
		aux0) ^ ~~((aux0 | ~b) ^ ~(b ^ a))) & (~~~(a | aux0) ^ ~a) &
		a | b ^ a) + 37807 * (~aux0 & (~((b | aux0 ^ aux0) ^ a & b &
		a & a ^ aux0) ^ (b ^ a | ((b & b | ~a) ^ (aux0 | b | aux0 ^
		b)) & a)) & a) + 26894 * ~((((a ^ a) & aux0 & a | ~aux0) &
		~(~a & b) | aux0) & ~(~aux0 & ~b | b) & ~b & ~(a | aux0 | b
		| aux0) & (aux0 | aux0 & a | b | a) & (~(a ^ b) & b ^ aux0 ^
		(b | aux0 | b & a | ~(aux0 | a) | ~b))) + 23525 * ~((~b |
		aux0) & (~(~(a | b) ^ b) ^ ~~(b | aux0) ^ aux0) & ((a | ~~b)
		& (b ^ aux0) | ~((b ^ aux0 | ~aux0) ^ (aux0 ^ b | a & a))))
		+ 60928 * (~b ^ a) + 46864 * (~a & (((b | a) ^ ~aux0 | aux0
		& b & (a | b) | ~a & ~a ^ (~aux0 | ~aux0)) ^ ((aux0 ^ a) &
		~b | (aux0 ^ a) & (aux0 ^ a)) & a ^ ((aux0 | a | ~b) ^ ~(b &
		aux0) | a) & ~~(a ^ aux0) & (a ^ b ^ (aux0 | a) ^ a)) |
		~(~((~b | a) ^ ~~~aux0) ^ b));
    vm->memory[--vm->sp] = result;
    // std::cout << "[DEBUG] OR: 0x" << std::hex << a << " | 0x" << b << " = 0x" << result << std::dec << " [sp=" << vm->sp << "]" << std::endl;
    
    WORD next = vm->fetch_opcode();
    vm->handler_table[next](vm);
}

void VirtualMachine::op_xor(VirtualMachine* vm){
    WORD b = vm->memory[vm->sp++];
    WORD a = vm->memory[vm->sp++];
    WORD aux1 = 32856249;
    WORD aux0 = rand();
    WORD result = 56209 + 15246 * (aux1 | (aux1 & (~b | a ^ aux0) & ~~aux1
		| ~(aux0 ^ (b | b ^ a))) & ((aux0 | a & aux1) & a ^ aux0 &
		~a & b & b ^ aux0) ^ ~~(aux1 & aux0)) + 16543 * (~(~((a ^
		aux0) & aux0 & b | ~(aux1 & aux0)) & ((a ^ b) & a & (aux1 ^
		~b) | (~aux1 ^ ~aux0) & b) | ~(((aux1 ^ b | ~aux1) ^ (aux0 |
		a) ^ (aux1 | aux0)) & ~(aux0 ^ aux0 & aux0))) ^ b) + 11772 *
		(b | ~((b | aux1) & ~b ^ a & ~(aux0 ^ aux0) | b) | ~(~(aux1
		^ b) ^ (b | b ^ aux0) ^ aux1) ^ ~~aux1 & (a ^ a & a) | ~aux1
		^ ((b | aux1) & (a | b) | aux1 | b) & ~(b ^ aux1 ^ aux0 ^ a
		| ~a | a)) + 11221 * b + 47835 * (~a & ~a ^ (~((a & a | aux1
		| aux1) & a | ~b) ^ (aux1 | ~a ^ aux0 & aux0 & aux0 | b)) &
		a) + 17760 * (((((b | aux1) ^ b & a | ~(aux0 & a)) ^ ~(b ^
		aux1 | ~aux0)) & ((aux0 & aux1 | aux1 ^ aux0) & (aux1 ^ b) &
		(a | aux1) | aux1 ^ aux1 ^ aux0 ^ ~(aux0 ^ a)) | (aux1 ^ a |
		~((a | b) ^ a & b)) ^ ~((b & aux0 | a & aux0) ^ ~(a | b))) ^
		(~~~(a ^ aux0) | b | aux1) ^ (~((a ^ aux1) & a & aux0 | aux1
		^ aux0 | b ^ aux1) ^ (aux1 & b & aux0 | b & aux0 & (b |
		aux1)) & aux0 ^ ~(~(aux1 ^ aux1) ^ b | ~(aux0 & ~a))) & (((a
		| b) & (a ^ aux1) | b ^ a | aux1 | ~((b | b) & (b | aux0) ^
		aux0 ^ a ^ aux0)) ^ (aux1 ^ ~~aux1 & (a & b ^ ~aux1) | ~(a &
		aux0 ^ (aux0 | b)) | aux1))) + 52035 * (~~(aux1 ^ ~(~(a & b)
		| a & a ^ b & b)) ^ ~(~(b & ~(aux0 & aux1) & ~(b ^ a)) ^ a))
		+ 61594 * ~(a | aux0) + 53774 * (aux1 & (~aux1 | ~(aux0 &
		(aux1 ^ a) ^ (b & a | a & a)) | aux0 | aux0) ^ a) + 54752 *
		(~~(~(b | aux0) ^ aux0 ^ b ^ ~~a ^ ~(b ^ aux0 | a ^ b) ^
		aux0) & ~((aux0 & (aux0 | aux0) & (aux0 ^ (aux0 | b)) ^ (b ^
		aux1 ^ (aux1 | aux0) | a | a)) & (aux0 | a | aux0 ^ b ^
		aux1)) & ~((aux1 | ~b & ~aux0) ^ aux1 & ~aux0 & aux0) &
		aux0) + 38797 * ((b | ~(~~b ^ aux0 & aux0 & (aux0 | aux0)) ^
		~aux0 & (a | aux0 | b & aux1) & a & (a | aux0) | ~~(~b &
		(aux1 | a) & b) | b) & ~~b) + 63861 * (~(aux1 & (aux0 ^ aux1
		^ aux1 & aux1 ^ (aux0 | aux1 | a ^ a))) & a | a | ~(~(aux1 ^
		aux0 | aux0 | a | aux1 & aux0 | ~a) & ~(~(a | b) ^ (a ^ aux1
		| ~b)) | (a | b) & ~a & (~b ^ aux1) & ~a & aux1 ^ b)) +
		31408 * aux1 + 21870 * ~b + 1931 * (~(((a | a | b ^ aux0 ^
		aux1 & aux1) & ~(~aux1 & aux1) ^ ~((aux1 ^ ~aux0) & (aux0 ^
		aux1 ^ aux0))) & ~(~~aux0 & ~a & b) & (~(aux0 & b) | ~(a ^
		a) | aux0 & (b ^ aux1) ^ ~aux1 & (b ^ aux0))) & (~b | ~((a &
		a | b ^ b) & (~aux0 ^ (aux0 | b)) ^ ~~aux1 ^ ~(a & aux0))) &
		(~~(~(aux1 | b) | aux1 ^ aux1 ^ a ^ a) | b & (~b | ~((aux1 |
		a) ^ aux1 ^ aux0)))) + 41588 * (a & ~(~~((aux0 ^ b) & (a ^
		aux1)) | ~aux0 | (a ^ aux0 | aux0 | aux0) ^ aux0 & b ^ aux0)
		& (~aux1 & (aux0 & b | aux0 | (aux1 | a) ^ ~b) & (a ^ aux0)
		& aux0 & b & aux0 & ~b ^ ~((a | aux1) ^ b ^ a) & (~a ^ (b &
		aux1 | b | aux0)) & (aux1 | a ^ b | b ^ (aux1 | aux0) |
		aux1))) + 49527 * (~(aux1 & ((a & a ^ aux0 & aux0 ^ ~a ^ a)
		& ~(~aux1 ^ (aux1 | b)) ^ a)) ^ a) + 38540 * (b & a & a &
		(~(aux0 ^ ~b) ^ ~~aux0 ^ a) & ((a | aux1) & (b ^ b) & ((aux1
		| aux0) ^ aux0) ^ ~aux1 & (a & b | a & aux1))) + 33280 *
		((~(~b ^ aux1 & aux1 | b | a | aux1) | (aux1 ^ (aux1 ^ b |
		aux0 & aux0)) & (aux1 & a & ~aux1 ^ a)) ^ ~(~(aux1 | aux0 |
		a) | (aux0 | ~b) ^ aux0) ^ a | a ^ (aux1 & aux1 | (~b | aux1
		& b | (a | a) ^ aux1 ^ a) & (~aux0 | aux0) & a & aux0 &
		(aux1 | a)) | b & (~(aux1 | aux0) | b | b ^ ~a | ~(a ^ b)) ^
		~b ^ ~(a | b) & a ^ a) + 62853 * ~~((~~(aux1 & aux0) & ~(~a
		& ~b) ^ b) & (aux1 & b ^ ((aux1 | aux0) & (aux0 | b) | aux0)
		^ (aux0 | b) ^ ~(aux0 ^ a))) + 45292 * (aux0 ^ aux1 ^
		~((~(aux1 & b) | aux1 & a | aux0 | a) & ~(~a & (a | b)) &
		~((a ^ a) & b & a ^ ~(a | aux0))) ^ ~((a & aux1 | aux1 | a)
		^ (~b | b) | aux0 & aux1 & aux1 & (aux1 ^ (a | a))) & aux0)
		+ 58998 * (((~(a | a) & (aux0 ^ a) & ~aux0 | ~aux1) ^ aux0 &
		b & (aux1 | b ^ a) & (aux1 & aux0 ^ (b | b)) ^ ~((aux0 |
		aux1 ^ aux0 | ~b) ^ ((aux1 ^ b) & (b ^ aux1) | aux0)) ^
		~~~((b | aux1 & aux1) ^ (aux0 | aux1) & aux1)) & ~(((aux0 ^
		aux0 ^ b) & ~~b | (~b | b & aux1) & (a & aux0 | b | aux1)) ^
		(((b | aux0) ^ a ^ aux0) & (a | b) & a | ~b) | aux0 ^ ~(a &
		(a | aux1)) | aux0 | ~aux0 ^ b & aux1 | ~(aux0 ^ a & a))) +
		18841 * (((aux0 | (aux1 ^ b | aux0) & (aux0 ^ (b | b)) &
		aux1) ^ (~(~aux0 ^ aux0 & aux0) | ~(b | a) | a) ^ (~~(b &
		aux1) | aux1 | ~(aux1 & b))) & (a ^ (aux1 | aux0) & ~b ^
		~((aux1 | b) ^ ~aux0)) & ~((aux1 | b) ^ ~b) & ~(aux0 ^ b) &
		aux0 & b & b & (b ^ a ^ (~aux1 ^ a | ~a)) | (a | a) & a |
		~(a & aux0) & a | (aux0 | aux1) ^ (aux0 | aux1 ^ aux1) ^
		(aux0 | aux0) ^ b ^ b | ~(b & (aux0 ^ aux1)) ^ ~(~b & aux1)
		^ (aux1 | aux0 | aux1 & aux1 | a | b | a | a | (aux1 ^ aux1
		^ a & a) & (aux1 & aux1 | b | aux1))) + 22911 * (((~aux0 &
		(b | a) | ~a) & (a & a ^ aux1 ^ aux1) & aux0 & ~b | aux0 ^
		(~a | a ^ aux0 | ~~a) | ~(b & (b ^ a) & (aux0 ^ b) & (aux1 |
		aux1)) | ~(((b | a) ^ a & aux1) & ~(aux1 & b)) | b | aux0 |
		~((aux0 | b | ~b | ~aux1 & (b ^ aux0)) ^ aux1 ^ aux1 ^ (b |
		b))) & ~(~b ^ a ^ aux0 ^ b ^ ~b ^ aux0 ^ aux1) & aux1 &
		~~(~a & (a | aux0) ^ (aux0 & b | aux0) ^ a & (~a | aux0 |
		aux1))) + 37477 * ~((aux1 ^ b & (aux0 ^ b) ^ ~~(b | b | aux0
		& aux0) ^ (~(aux1 | aux0 & (aux1 | aux1)) | ~(aux1 | aux0) ^
		(aux1 | a) ^ b ^ b | ~~(aux0 | aux0))) & (((a & aux0 ^ a &
		aux1) & (aux0 ^ aux0 | aux1 & b) | ~(aux0 | aux1) | ~~b) &
		((aux1 | aux1) ^ aux1 ^ aux0 | ~(aux1 & aux0)) & (~aux1 ^
		aux1 ^ (~aux1 | b)) | aux0)) + 38365 * ((~(~((a | aux1) & (a
		| aux0)) | (aux1 & aux1 ^ a & a) & ~~aux1) & (b & ~aux0 ^
		~~(b ^ b ^ (a | aux0))) | (~b | ((aux1 | aux1) & ~a | b & b
		| aux0 | b) & ((b | aux1) ^ aux0 ^ aux0) & ~(aux1 & a)) ^
		~((~~a ^ b) & ~((a ^ aux0) & (aux1 | aux1)))) ^ aux1) +
		53789 * (~(~((b ^ a) & (aux0 | aux1)) & (a | ~(b ^ b)) & a)
		| ~((b | b) ^ ~aux1) & (aux1 ^ (a | b) | b & a ^ aux0) | ((b
		| a) ^ (aux0 | a) ^ ~aux0) & a & (a | aux1) & ~a | ~(aux0 &
		(~(aux1 | aux1) ^ b & aux1 & (aux0 | aux0))) | (a ^ aux1 & a
		& ~(~b | b) ^ aux1 & a ^ aux1 ^ ~~aux0 ^ a ^ aux1) & a & a)
		+ 58250 * (a & aux1) + 22467 * ~(((aux0 | ~(aux0 | aux0)) &
		aux1 & (a ^ b ^ a & aux0) ^ (~~b | b | aux0 ^ aux0) ^ (b |
		a) ^ aux1) & ~((~(aux1 & aux1 | a & b) ^ ~(aux0 | a) ^
		~(aux0 | b)) & (b | aux0))) + 3748 * (aux0 & (~~(aux1 ^ ~b ^
		(b | aux0)) | aux0) ^ (~~(aux1 | b) & ((aux0 | aux1) ^ (aux1
		| aux1)) & ((aux1 | aux1) ^ ~b) ^ aux1 ^ ~((~b | a ^ aux1 |
		b) ^ b & aux0 ^ (b | b) ^ (aux0 ^ a | aux0 | aux0)) | ~(b |
		a) & ~(~aux0 | a | a | ~aux1 & (aux1 ^ a)) | b | ((a & b |
		aux0 ^ aux1) ^ aux1 & b ^ aux0) & aux0 & aux0 & aux1 & a)) +
		11692 * aux0 + 14710 * ((aux0 ^ b) & b) + 8056 * ((a ^
		(~(aux1 & aux0 ^ aux0 ^ b) | aux0) ^ (~(b | a) ^ ~(b ^ b) |
		~aux0)) & (aux1 | a) & (aux1 ^ ~(aux1 & b)) & (b | (aux1 ^
		aux0) & aux1) & ~(aux1 | aux1 & aux1 | aux1 & aux0 | (aux0 ^
		~b) & (a ^ (aux1 | aux0))) | ~(a | ~aux1 | (aux0 & b | a |
		(a | aux0) ^ aux0 | ~(aux0 ^ aux1) | b) & ((a | aux1 | aux1)
		^ ~(a ^ aux1) ^ (~aux0 | a ^ aux1) ^ ~(a & a)))) + 27240 *
		(~(a ^ ~((aux1 ^ a) & ~~(aux1 & aux1))) ^ (a | aux0)) +
		26792 * ((~b ^ ~aux0) & ~a ^ ~(b & (a | ~b | ((aux1 | b) &
		aux0 & a ^ b) & b))) + 58702 * (~(~(a & a) ^ aux1) ^ (aux0 |
		((aux0 | aux0) ^ aux1 ^ aux0) & ~(aux1 ^ b)) ^ (a & ~(aux0 &
		aux1 & (aux0 | a)) | ~~(aux1 & aux1) & (~aux0 | b ^ aux0 | a
		| b)) ^ ~(~~a ^ (aux0 ^ aux1) & ~aux0) ^ aux1 ^ ~((aux0 ^ b)
		& ~aux0) ^ b ^ (aux1 | aux0 | aux0) ^ (~b ^ a & b ^ aux1 & a
		^ a & aux0 | aux0 ^ a) ^ ((~(aux0 ^ b) | aux1 ^ aux0 ^ aux1)
		^ ~aux0 ^ b | b & (~~aux0 | aux0 | (~a | ~b) & (aux0 | aux0
		| ~aux0)) | (aux1 | a) ^ ~b ^ (aux1 ^ a | aux0) | aux1 | (~b
		& ~aux1 | (aux1 ^ aux1) & (a | a)) & ~~(a | aux0) | aux1)) +
		26069 * ~(aux1 & (~((aux1 & a ^ aux1 & a | aux0 ^ a | ~aux1)
		& (aux1 ^ (a | aux1) ^ a ^ b)) | (~~(aux1 | b) | ~(b & aux0)
		| aux0 & b ^ aux1) ^ ~((aux0 ^ b ^ aux1 & a) & aux0))) +
		8370 * (aux1 | aux0 & aux1 & aux1) + 51815 * a + 576 *
		((~((~a ^ b & aux0) & (b ^ a ^ a & aux1) ^ ~(aux1 ^ a) &
		(aux0 & aux0 ^ a ^ b)) | (a | (aux0 | b) ^ ~aux0) & (~(a &
		aux1) | ~(aux0 | aux1)) | a) & ~((aux0 & a | b & a | aux0 |
		b ^ aux0 | a) ^ (~aux0 ^ (aux0 | aux0) ^ a ^ aux1 ^ ~a) &
		(aux0 & aux1 ^ a) & aux0) & (aux0 | ~(~(b ^ a) & ~(~a |
		~b))) & a) + 36830 * (b & ~(aux0 & a)) + 19368 * (b & aux0)
		+ 854 * (b & ~(aux0 & aux1 ^ ~((a | a | b | b & a) & (aux0 &
		aux0 ^ a ^ b | aux0)))) + 12837 * (b ^ (b ^ ~(aux1 | aux0))
		& a & (aux1 ^ aux0 ^ (aux0 | aux0)) ^ ~aux1 & (a | aux1) &
		((b | a) ^ b & aux0) & (~(b | a) ^ ~(b ^ aux0)) ^ aux0 ^
		(~aux1 | b | (aux0 ^ aux0) & ~b) & ~~(~b | aux0 | aux0) ^ b
		& aux0 ^ (a | a | a & a ^ ~b)) + 37436 * (aux1 & ~((aux1 |
		(aux0 ^ a) & ~a) ^ (a ^ a) & ~aux1 ^ ~(b ^ aux0)) ^ ~(~(~b &
		(aux0 ^ a) | a) & ~(b | ~(aux1 & a))) | a) + 43227 *
		~~~(~(aux1 ^ a & a ^ ~~aux0) | ((aux1 ^ aux0) & (aux0 ^ b) |
		aux1 | aux0 | aux1) ^ (b ^ b | aux0 | aux1 | aux1 & aux0 ^
		~aux1)) + 51082 * ((aux1 & ~aux1 | (~(a | a) | b) & ~(aux0 &
		(aux1 ^ aux1)) ^ b) & ((~(~aux1 ^ aux0 & a) ^ ~aux0 ^ aux0 &
		b ^ a & aux1) & (~(~b ^ a ^ aux0) ^ ~((aux1 | aux0) & (a |
		a))) | (b | b & ~b & (a | b) & aux1 & b) & (aux0 & b ^ b ^ b
		| a | aux0 | ~b)) & (~(((b | aux0) & a & a | ~(aux0 ^ a) ^
		aux1) ^ b & ((b | b) ^ ~aux1) ^ ~(~aux1 | aux1 & aux1)) |
		~a)) + 12193 * (~((aux1 | ~~aux1 | ~b) & (aux0 | ~(a ^ a) |
		~(aux1 ^ aux0)) & ~(aux1 ^ a ^ b ^ a ^ aux0)) & (~~((b | b)
		& (aux1 | b) ^ (aux0 | a | b & b)) ^ (~(aux1 ^ b) | ~aux1 |
		~(aux1 & aux1) ^ (a ^ a) & a | ~~~(b & a)) ^ a & (aux1 ^
		~(aux0 & a | aux0 | aux0) | b))) + 46087 * ((b ^ a & ((aux1
		| aux1 & a) ^ (aux0 ^ a | b) | ~(b ^ a) | aux1 & aux1 | ~b))
		& aux0 | aux0 & (a ^ a ^ a ^ b ^ ~aux0 ^ a ^ b) & (aux0 |
		~(aux0 ^ aux0)) ^ aux0 ^ ~(b & aux1 ^ aux1 ^ a) & aux1 ^
		((~((a ^ b) & (b | b)) | (aux1 | a) & aux0 & (~b ^ a & b)) ^
		~(aux0 & aux0 | ~a | ~(aux1 ^ b))) & b) + 23028 * (aux0 & (a
		| aux1 | (aux0 & aux0 | b ^ aux0) & (aux0 & a | aux1 ^ a) |
		aux1 | ~aux0 & (aux0 ^ aux1) & (a & aux1 ^ aux1 & b) ^ (aux0
		^ aux0 ^ (a | aux0) | ~aux0 ^ aux1) ^ (aux0 | (aux0 ^ aux1)
		& a ^ (aux0 ^ aux1 | a ^ b)))) + 29753 * ((aux0 ^ (aux1 &
		aux1 & b | aux0) & a) & ~(aux0 ^ aux1 & b ^ (b | a) & ~b) ^
		~~(~(a ^ b) ^ (aux0 | aux1) ^ aux0 ^ aux1) & ~b | a ^ aux0 &
		aux1 ^ (((aux1 | b) ^ ~aux1) & (a ^ a | a | aux0) | b |
		~(~(a ^ b) ^ (a | a | a)))) + 29632 * (~((~aux0 & a & aux0 ^
		aux0 & aux0 & aux1 ^ ~(aux1 | aux1)) & ~~(~a ^ ~a)) | (aux0
		| ~(a ^ b & b)) ^ ~(~a & aux1 & a ^ ~(aux0 ^ aux0)) ^ ~~aux0
		| ~b | aux0 | aux1) + 44335 * ~aux0 + 11165 * (~a ^ ((~(aux0
		& (aux0 & a | aux0 ^ aux1)) | aux1 ^ a & ~b & (aux1 | aux1))
		^ ~(aux0 ^ ~(a & aux1) ^ ~aux0 ^ aux0 ^ b) | a)) + 58874 *
		(~((b ^ a) & ~a & (aux1 & a ^ a ^ b) & (aux1 & aux0 & (aux0
		^ b) ^ aux0)) & ((aux0 & b | aux0) ^ (aux1 | aux1) ^ ~b) &
		(~aux0 & b | ~(aux0 ^ a)) & (a ^ a | a & aux0) & ~(a | a) &
		~b & (aux0 ^ aux0) & (a & b | aux0 ^ b) ^ (~(aux1 & b & (a |
		a) & b) & (~~aux0 | ~(a ^ a) | a) | ~~(b & aux0 | aux1) &
		(aux0 & aux0 & aux0 & aux1 & ~a ^ (aux1 | aux1) ^ aux1 ^
		aux1 ^ ~(a ^ aux0))) ^ ((~~~~aux1 | ~aux1) ^ ~a ^ aux1 | ~(a
		& aux1) & (aux1 | aux0 | aux1 ^ ~(a ^ aux0)) & (b ^ b | a |
		aux0 | ~(aux0 ^ aux0) | aux0))) + 50008 * (((aux1 | aux1 &
		~aux1 ^ (aux0 | aux1) & (b ^ aux0) | (aux0 ^ aux1) & aux0 &
		(aux0 ^ b) & b & a) ^ ~(aux1 | b) ^ ~(aux1 | a) ^ ~((b ^ b)
		& ~aux1) ^ a) & ~a ^ ~~aux0 & aux0) + 54740 * (~((aux0 ^
		aux0 ^ ~a) & b & b) & aux0 & aux0 | ~((~(~~aux1 | aux0 |
		aux0 & a) | ~(b | b & b) ^ ~~(b | b)) & (~(aux1 | aux0) &
		((a | b) ^ (a | aux0)) | (~aux0 ^ (aux1 | a)) & ~~aux0 | ~b
		& (a | aux0) & ~~aux0 & (~aux1 & aux0 & aux1 | ~aux1 | aux0
		| aux0)))) + 55602 * (~(~(~~aux0 ^ (a ^ a) & (b ^ a) |
		~(aux0 | aux1) & ~a) ^ (a | aux0 | b) & b ^ (aux1 ^ aux1 ^
		aux1) & ~(a & b) ^ ~(~(aux0 | b) ^ aux1)) & (~(aux1 ^ aux1)
		^ ~((aux1 | aux1 | ~aux1) ^ (b & a | a & aux0)) & (aux0 &
		aux1 & a & (aux0 & aux1 | b) ^ ~(a ^ aux0) ^ aux0 & a) &
		aux1)) + 39499 * (aux1 | a | (~(b & (a | aux1) ^ ~(b | a)) |
		aux0) ^ ~~~~aux1) + 3088 * ((aux1 & (~((a ^ aux1 & aux0) &
		~(b | aux0)) ^ ~~~(b | a)) | a) ^ ((aux1 | (~(a | b) | (aux0
		| aux0) ^ ~aux0) ^ (~(a | b) | ~b | a & aux0)) ^ ~aux1) &
		(~((a ^ aux0) & aux1 & b & (aux1 ^ aux1 ^ aux0 & aux0) ^ b &
		aux0 & (aux1 | aux1) ^ a & aux1) | ~((b ^ aux0) & a) ^ ~aux0
		| ~~a | b)) + 22381 * (a & (~((b & aux0 | aux0 ^ aux0) &
		~~a) & (~~aux1 | aux0) & aux1 & (aux1 ^ b ^ b & a) | a |
		aux1 & (~~aux0 & ((aux1 | aux1 | aux0 ^ b) ^ aux1 & aux0 ^
		(a | a)) ^ ~(aux1 & ~(aux1 & aux0))))) + 444 * ((~(~b ^ b ^
		aux1) & (b | a | aux1 ^ aux1 | ~a) | ~~~(a | aux0)) & (~aux0
		& (a ^ b) ^ a & aux1 ^ (b | a) | ((aux0 | a) ^ aux1 & a) &
		(aux0 | b) & aux0 & a) & (((a | aux0) ^ b ^ a) & (aux0 | b |
		a | a) ^ ~~(a & a)) ^ ~(aux0 ^ (aux1 & b | aux1 & aux0) ^ ~b
		^ ~(b ^ b) & b ^ (aux1 ^ b) & ~aux1 ^ ~(aux1 | a)) ^ b) +
		48388 * (~~((~a ^ b & aux1) & ((b | b) ^ a) | (aux1 | b) &
		(b ^ aux1) ^ (a | b | a ^ aux0)) & ~~a & ((~(a ^ (b | a)) |
		~(aux0 | b & aux0)) ^ aux0 & (b | b) ^ (a | a | b | a) ^
		(aux1 | aux0 | b | (b ^ a) & (aux0 | b))) | b & (~((a ^
		aux1) & a ^ (aux0 | a & aux0) ^ a) | aux0 | ~aux1 | ~(aux1 |
		aux1) | aux1)) + 32465 * ~(b & (~aux0 ^ ~(~aux0 & (a | aux0)
		| aux1) | ~~a | ~~aux1)) + 18551 * ((~b | aux0 | ~b) & ~~a |
		aux0) + 46864 * (~~(((aux0 | a) ^ ~aux1) & a & aux0 & ~aux0
		& (~b | aux0 ^ aux1) & (~b | ~b) ^ aux1) & (aux0 | (a | a |
		~aux1 | b & b ^ (aux1 | aux0) | aux0 & a ^ b ^ a) ^ (~((a |
		aux0) & (aux1 ^ aux1)) | a) & a)) + 42433 * (((~(aux0 & aux1
		& (b | aux1)) | a & a & aux0 ^ ~aux0) ^ aux0) & aux1 & aux0
		| (~((aux1 ^ b | b & aux0) & aux0 & aux0 & aux1 & b) | (aux0
		^ a) & aux1 & b & b ^ (aux1 | (aux1 | aux0) ^ aux1 & b)) &
		~(a ^ ~a) | ((aux0 ^ (b | aux1) | ~(b ^ b) | aux0 & b) & (b
		& (aux1 & a | b & aux0) | (a | aux0) & (aux1 ^ aux0) | ~aux0
		| b | aux0) | (~b ^ (aux1 | b) | aux1 ^ aux1 ^ aux1 ^ aux0 |
		~(~b | ~a)) ^ (aux0 ^ aux1 ^ aux1 | ~a ^ aux1 & a | (b | a)
		^ (b | aux0) | b & ~aux1)) ^ ((~aux0 & (aux1 | a) | ~(b ^
		aux1)) & (b ^ b ^ a ^ a) | b | ((a ^ a) & (aux1 | a) & (aux0
		& aux1 ^ a) ^ (b ^ aux1 | aux1 ^ aux1) & aux0) & a)) + 42146
		* (aux0 & (aux0 | ((~aux1 & ~b | ~(b | a) | ~~b | ~~b) ^
		~(~(aux1 ^ aux0) | ~(aux0 & b))) & (~(aux0 & a & b) & aux0 |
		aux0))) + 62802 * ((aux1 ^ a | ((aux1 | aux1) ^ aux1 & aux0)
		& (~aux0 | ~aux0) ^ a | (b & aux1 & aux1 | (aux0 ^ b) & ~b)
		& (aux0 | aux1 | a & a) & (a ^ aux0 | aux1 | aux1)) & ((b ^
		aux0 | aux1 ^ a) & a & aux0 & (a | a) & ((aux1 | aux0) & b &
		b | ~(aux1 | b)) & ((b ^ b | aux1 | aux0) & (~b | aux0 ^
		aux0) ^ (a | a)) | a ^ ~~(~a ^ ~aux1)) ^ ~(~aux1 & ~(a & b ^
		aux1 & aux0) ^ ~(~b & a & b ^ aux1 ^ ~b)) ^ (a & ~aux0 &
		aux1 & (aux0 | b) & aux0 | a)) + 3274 * ((~((aux1 | a) & ~a
		& b & aux1) & aux0 ^ (~(~a | aux1) | a & (b ^ a) & ~~(a ^
		aux0) & ((aux0 | aux0 ^ aux1 | (b | b) & (b ^ b)) ^ (~aux1 |
		~b | aux0 ^ aux1)))) & aux1) + 42635 * (~(b | (aux1 ^ b & b
		| ~a ^ (a | b)) & b) & (~((aux0 & a ^ ~a) & aux1) | ((aux0 |
		aux1 ^ a) ^ (~b | a ^ b) | ~(aux1 & b) | (aux1 ^ b) & a) & a
		& aux1 & aux0 & ~aux0 & aux0) | (~~~(a & aux1) ^ (~(b | a) &
		(~aux0 ^ a & aux1) | aux0 ^ a | ~a | (aux1 | aux0) ^ ~b) |
		~(~a ^ ~aux0 ^ b & aux0) & aux0) & ~(~(aux1 & aux0 ^ b ^ (b
		^ a | aux0 | aux1)) | (a & aux1 | aux1 | a) ^ (aux0 | b) ^
		(aux0 | aux0) | ~(aux1 ^ b | aux0 | a))) + 4279 * (b | aux0)
		+ 9927 * ((a ^ ~((a ^ aux1) & aux0 & a | ~~a) | ~~(~(a | a)
		^ aux0 ^ b ^ aux1 & aux0) | ((aux0 | aux0) ^ ~aux1 | a | a |
		a | aux1 | b | aux0) ^ (aux0 & a & (aux0 ^ b) | aux1 | aux1)
		^ ~(~(b ^ aux0) | b)) ^ (b | ~~((a ^ aux0) & (a | a) & ~(a &
		a)) | ~a)) + 46132 * ((~(b ^ (aux1 | a | aux1 & aux0)) & (b
		^ aux0 | aux0 ^ aux0 ^ aux0 ^ a) ^ ~~aux1 ^ b | ~aux0 & (b &
		b & a & aux0 & aux0 | aux0)) ^ ((b ^ a | aux1 & aux0 | b) &
		~~(aux1 ^ a) ^ aux1 | a | (~(a ^ a ^ a) ^ (~b | ~aux0) ^
		~aux0) & (aux1 & (aux0 | b) | ~(aux1 | aux0)) & a ^ b)) +
		62308 * ((b ^ ~~((aux0 | a) ^ aux1 ^ a | a ^ b ^ aux0 ^ b) |
		aux0 ^ ~(~a & (aux0 & aux1 | aux0) & aux0)) & ~~b & aux1) +
		4756 * ((~(aux0 | a & (b & aux0 ^ aux0) & ~((b ^ aux1) &
		~a)) | aux0) & (~(a & a & (aux0 ^ a)) ^ (b & a ^ (aux0 |
		aux1) | aux1 ^ ~a) | ~(~b & ~b) ^ aux1 | aux0 | (aux1 &
		(aux1 ^ a ^ a) & a ^ aux1 & b & ~(b & aux1)) & (aux1 ^ (a ^
		aux0 | b) & ~(aux0 ^ b) ^ aux0 ^ aux0))) + 551 * (~(~~aux0 ^
		aux0 ^ b & aux0 & a & ~aux0 ^ (aux1 & (aux1 | aux1) | (b ^
		b) & (b ^ aux0) | a | aux0) ^ ~(aux1 & aux1 ^ (aux1 | b) |
		~(aux0 | a)) & b & a & aux1 & (b ^ aux0) & b) ^ b & b) +
		59202 * (~((aux0 ^ a) & ~~aux1 & ~((aux1 | a) & ~aux0) &
		~~~(b ^ aux0)) & ~aux0 & aux1 & ((aux0 | b | aux1 | ~a ^ ~a
		& ~aux1) & b | (b | (a ^ b) & aux1 & a & ~(aux1 ^ aux0)) &
		~~((b ^ aux0) & (a | aux0)))) + 48733 * (~aux0 ^ ~(aux0 ^
		a)) + 44571 * ~((aux0 ^ (~b & ~a ^ a) & ~(aux1 & aux1 ^ b) &
		(a ^ ~(aux0 & a) | ~(~b ^ aux0 ^ a))) & (~(a | aux1) ^
		~~(aux1 & b | b | ~~b))) + 61107 * (~(a ^ aux0) ^ ~(~aux1 ^
		(aux0 | aux1 & (aux1 & a | aux0 | aux0) & b))) + 34443 * (a
		^ (~((a | aux0) & (b | aux0) & b & b & (aux0 ^ aux1)) & b |
		~((b | b | aux1) & b) | ~b ^ aux1 & b | aux1 | b ^ a | (aux0
		| a) ^ aux1 ^ aux0) ^ b ^ b) + 1848 * ((b & a | aux0) ^ b ^
		~~aux1 ^ b & (a | aux0 ^ aux1 ^ aux0 ^ a) ^ (a | a ^ aux0 &
		(aux0 | aux0) & a) ^ (b ^ aux0 | aux0) ^ aux1) + 13677 *
		(aux0 ^ (aux0 | b) ^ ~(~~(b & (b ^ aux0)) & ~((a ^ aux0 | b)
		^ (aux0 ^ aux0 | ~aux0)) & ~aux1)) + 37909 * (aux0 ^ aux0 |
		~(~a & ((a | aux1) & (a ^ aux0) | ~(aux1 & aux0)) & a |
		(aux0 | b) ^ a | b | ~((b | aux1) & a & aux0) | ~(~aux0 & a
		& aux0) & (aux1 | aux0 | aux0) & ~~aux1)) + 36862 * (aux1 &
		(((~~a & a | aux0 ^ a ^ a | ~aux0 | b | b) ^ (~aux0 | ~aux1
		| aux0 & a | aux0 & a & aux0 & aux0)) & ((b | aux1) & aux0 &
		aux1 & ~(aux1 ^ a) | b | ~aux0 | ~a ^ b) & ~(((a | aux1) ^
		aux0 ^ a) & a & (aux0 ^ aux1)) ^ a)) + 36900 * ((aux0 ^
		(~((a ^ aux0) & (b | aux1) & ~(b ^ b)) | aux0 | ~(aux0 | a ^
		a)) | aux1 | aux0) & a) + 29994 * ~~(aux1 & (a ^ (~b & aux0
		& a | a)) ^ (aux0 | ~aux1 & ((a | a) ^ aux0 ^ a) & ((a ^
		aux0) & b | ~(aux1 & b)))) + 17646 * ((~aux0 ^ ~(b & b &
		~aux1) ^ ~~b & a ^ aux0) & (~~b & (b | b | b & aux1) ^
		(~~aux0 | a & ~b)) & ((~aux0 | a ^ aux0) & (b | b) & b & a ^
		(~(b ^ a) | ~b ^ aux1)) & (aux0 & aux0 ^ (a | aux0) & (b |
		aux1) & ~(a | a) ^ (~(b & aux1) & a | aux0 ^ a ^ (aux1 |
		aux0) | a | aux1 | ~aux0) ^ ~(a & a) & a) ^ aux1 & ~((a |
		aux0 & a) & ~((a | b) ^ b ^ aux1 ^ ~~aux0))) + 63914 * (b |
		aux1 | a | ~aux0 | a) + 9539 * ~((~aux1 & aux0 | ~(a ^ a) &
		(b ^ aux0 ^ b ^ a) & ((a & a | aux1) ^ ~aux1)) & (~~aux1 |
		((~a | a) ^ ~~aux0) & ((aux0 | b | b) ^ (aux1 | aux1 | aux1
		| b))) | ((aux0 ^ (b | b) ^ b ^ aux0 | a) ^ a ^ a) & ~(a ^ b
		& a ^ ~aux0 ^ (a | aux1 ^ b | b & aux1))) + 42147 * (~~aux0
		^ ~~(aux1 & a)) + 18706 * ~(a | ~((b | aux1 | b | aux1 & b ^
		~aux1) & ~(b | b ^ a) & (~aux1 ^ ~(b ^ aux1) & aux0))) +
		11582 * ~aux1 + 30921 * ~((~~~a ^ aux1 | (aux1 | ~(b & b)) &
		aux1) & ~(~~a & (a | aux0 | aux1)) & (b & a ^ ~(b | a) |
		(aux1 | a) & (a | a) ^ (aux0 | aux1 | b | aux1)) | (~(aux1 |
		b) ^ aux1 & b ^ (aux1 | aux0) | b) & a ^ (a | a ^ (aux0 | b)
		^ ~~a) & (~(aux0 & aux0) ^ b & a ^ aux0 ^ aux1) & (~a ^
		(aux0 | aux0) ^ (aux1 | aux1))) + 34351 * ((aux1 | a) &
		((((aux1 | b) ^ (aux0 | b) ^ (aux0 ^ a) & (aux1 | b) | ~~a |
		~(aux0 | a)) & (aux0 & b ^ (a | aux0) ^ ~a & ~a ^ ~((a |
		aux1) ^ a ^ b)) | aux0) ^ aux1)) + 25958 * ~(~aux1 & ~~~b ^
		~(a ^ b) ^ ~(aux1 | aux0) ^ (aux1 ^ b ^ aux0) & ~(aux0 & a)
		^ aux1 & b & (aux1 & a & aux0 & aux1 ^ aux1 ^ aux1) ^ a) +
		63880 * (~((~(aux1 ^ ~aux1) | b) ^ aux0 ^ (aux1 | ~(a & a))
		^ b & (~(aux1 & a) | aux1 & b | ~aux0) & (~(aux1 | b) | (a |
		aux0) & (aux1 | b))) | ~(aux0 & ~(aux0 & aux0 & aux1 | b ^
		~a | a))) + 7593 * ((~~~(b & a) ^ a & a ^ a | b) ^ ~(~~(aux1
		| a) & ~~aux1 & ~~a & ~(a ^ aux1 ^ b ^ b ^ b) | b)) + 37934
		* (aux1 & (b & b | ~a) & ~~~~(b | aux1 | aux0 ^ a)) + 21946
		* (~(aux1 ^ ~~((aux0 | a) & b & aux1 ^ b & aux0 & aux1)) &
		~(aux0 & b & a & aux0 & ~(aux0 ^ aux1) & aux1)) + 9580 * (a
		| b) + 21827 * ((a | ~(((b | a) ^ a ^ a | aux0) & ((~a |
		aux1 | aux0) ^ aux1 ^ aux1 ^ ~b)) ^ (~(a & a) & (aux0 ^ a |
		aux0 | b) | aux1) ^ ~~(b ^ a ^ (a | aux1))) ^ (aux1 & a & a
		^ b) & ~(aux0 & aux1 & b & aux1) & (aux0 ^ ~(b | b ^ b)) ^
		aux1 ^ (((b ^ aux1) & ~a | ~a) ^ (aux0 & aux1 | a) | a) ^ b
		& ~(aux1 ^ ~~a)) + 30559 * ((b & aux1 ^ (~~(aux0 & aux1) |
		~aux0) ^ ~~a) & ~(((aux0 ^ aux0) & aux1 ^ ~(aux0 | a)) &
		(~(a | aux0) ^ aux1) | aux0) | ((~(aux0 & aux1) ^ (b | aux0)
		& aux1 & a | aux0 | (aux0 | aux1) & aux0) ^ b & a ^ ~aux0 ^
		~(aux0 ^ b) ^ a & aux1) & (b & aux0 ^ b & b ^ ~b & (aux1 |
		b) | ~~(b ^ aux0)) & (aux0 | b | a & aux1 | ~aux1) | ~(aux0
		& ~(aux0 | aux1)) & b | aux0) + 19208 * (((aux0 ^ (a | a) ^
		~b | b & (aux1 | aux1 & a) | ~~(aux1 & a) ^ aux1 & (aux0 ^
		aux1) & ~b) & (aux0 | a & (~(a & a) ^ aux1)) ^ (aux1 & aux1
		^ ~(b ^ (aux1 | a) | aux0) | aux1)) & ~aux0 & (b ^ a ^
		~(aux0 | (aux0 | aux0) ^ aux1) & ~((aux1 & b | b) ^ (aux1 |
		b & a)))) + 17411 * (aux0 | (a & (aux0 ^ aux0) | ~(aux0 |
		aux1)) ^ (b ^ a | ~aux1) ^ ~~aux1 ^ (a | a | b) | ~b ^ ~(a &
		a & aux0) & b | a & ~b & a & ((b & aux0 ^ a & aux1) & (aux0
		| aux1) & a | ~~(a | aux1))) + 61014 * ((~a ^ ((aux0 | aux0)
		& b & a | ~aux0 | a | b) & ((a | aux0) ^ (aux1 | b) ^ ~aux1
		& b & aux0) ^ (~(~aux1 ^ b) | a & a & ~a | aux1) & ((aux0 |
		b | a ^ aux0) ^ ~(aux0 & a) ^ a ^ b ^ b) | ~(~b & ~((aux0 |
		b) ^ (a | aux1)) | aux1 & ~(aux0 | aux0) ^ ~(a ^ aux1))) ^
		((a | aux0 ^ aux0) & ~aux0 ^ (a ^ aux0 | aux1 ^ aux1 | aux1
		^ (aux1 | b)) ^ a & a | a & ~(aux0 & aux1) & (a | aux1) & b
		& b & (~aux0 ^ ~(a ^ a) & ~(aux1 ^ b))) & (aux0 | (~(a | b)
		| a & aux0 & a) & ((b | aux0 & aux0) ^ (a ^ aux0) & a) ^
		~((a ^ aux0) & ~b) & aux1)) + 10171 * (a & (~aux1 ^ ~~~aux1
		& b & aux0) & ~(~(a & b ^ a & aux1 | b) ^ ~aux0 ^ ~(b ^
		aux1) & ~~aux0)) + 40722 * (aux0 | aux1 ^ ((b | ~b ^ a) ^
		(aux0 | ~(a ^ aux0)) | a) | ~(aux1 ^ ~~(b ^ aux1)) | (aux1 |
		aux1) & (aux0 ^ a) & ~(aux1 | aux0) & (a | a | aux1 | ~aux1)
		& (b & b | aux1 | (aux1 | a) ^ aux1 | ~(aux0 & b)) | (~(aux0
		& (aux1 | b)) ^ aux1 & ~aux0) & (a ^ ~aux0 | ~b & ~a | (aux1
		| aux0) ^ aux1 & aux1 | ~a & (aux1 ^ a)) | ~((b | a) ^ b) &
		(a ^ aux0 | ~b) & b | ~(a ^ b ^ ~b) & b & a & ~aux0 & (a |
		b) & ~aux1) + 56057 * ~((((~a ^ aux0) & (b ^ aux0 & a) ^
		~(aux1 | a | b ^ aux1) | aux1) ^ (aux1 & (aux0 ^ aux0) &
		(aux1 ^ aux1) ^ b) & aux0) & aux0) + 49503 * ~(~(~(a & aux1
		& aux1) & ~(aux1 | aux1) & b & (~~~b ^ ~(a ^ aux0 | a | b)))
		| b | b) + 15746 * ((b & ~(~(a ^ b) ^ (aux0 | aux0) & (a |
		aux0)) ^ ~~(aux0 & b | aux1 ^ aux1 | aux1 ^ ~aux1) | ~(aux1
		& aux1 | aux0 | a & b | b | aux1) ^ ((aux1 ^ aux0) & aux0 &
		b ^ (b | aux0 | b & a)) & (aux0 & aux0 ^ b ^ a ^ (aux0 |
		aux1) ^ b) ^ (a & a | a ^ (aux0 | aux0) & aux0 & aux0 |
		~aux0 | (aux1 ^ aux0 ^ aux0 & aux0) & (aux1 & aux1 ^
		~aux1))) ^ ~(~(~(aux0 ^ (aux1 | a)) ^ aux0 & ~b) | ~((aux0 |
		aux0) ^ b & aux1 ^ aux0) ^ ~(aux0 & aux0 ^ b ^ aux0) & aux0
		& (b ^ aux0 ^ aux0))) + 6048 * ~~aux0 + 16768 * ((~(~a | ~(a
		| a) | ~a) | aux1 ^ ~aux1 ^ ((aux0 | aux0) ^ ~b | ~aux0 &
		aux1 & aux0) ^ aux1 | ~a) & (~~(aux1 & a ^ a ^ a ^ b |
		~((aux1 | a) ^ aux1 ^ b)) | ~((aux1 ^ a | a & b) & aux0 & (a
		^ (aux0 | a) & aux0)) ^ aux1)) + 12406 * (~(~aux1 & (~~aux1
		^ (aux1 | aux0) ^ ~aux1 | b)) & (aux0 | aux1 | a | a | ~b ^
		a) & aux1 & b | ~((aux0 | aux0) & aux0 & aux0 ^ ~aux1 & (b |
		aux1) ^ ~(b ^ aux0 | ~a)) & (a ^ ~(~a ^ (aux0 | aux0) ^
		~~a)) | (aux0 & a & ~aux0 | ~(aux0 | aux0) | aux1 | ~b |
		aux0 | aux1) ^ ((aux0 | a) ^ ~aux1 ^ a & (aux1 ^ aux1) | ~a
		| aux0 | aux0 | aux1 & aux1) | (aux0 ^ (a & b | aux1 | a)) &
		b & aux1) + 40327 * (~aux0 ^ (aux1 & a & ~~aux0 | aux1 ^
		aux1 ^ b ^ b ^ a ^ aux1 ^ aux0 | aux1 | b | aux0 ^ b | aux1
		^ aux0 | b ^ aux1 | ~~aux1 ^ aux1 ^ b | (~aux1 | b ^ a) &
		(~aux1 | b & b) & (~a | ~(a ^ aux1)) & (aux0 ^ a ^ ~~aux0 ^
		a & b & ~b ^ a))) + 63766 * ((a ^ aux1 & ~(aux0 ^ aux0 | a ^
		b) | ~((~a | aux1 ^ b) & (aux1 | a) & (b | aux0)) ^ aux0 ^
		~(b | ~a)) & ~aux0 & aux1) + 35685 * ~(~(aux0 & aux1) &
		~((~(aux1 | b) ^ (aux0 | ~a) | aux1) ^ ~aux0)) + 5349 * ((~b
		^ ~((~aux0 & ~aux1 ^ ~(aux0 | aux0)) & ~(b & a) & aux0 &
		aux0 & (a ^ aux0)) & ~(~(aux1 ^ ~aux1) ^ ~(aux0 ^ aux1 ^
		b))) & b) + 15516 * ((~(~(a | b) | aux0 | a & a) | ~((aux1 ^
		aux1) & aux0 & b) ^ a | aux0 ^ a ^ aux1 ^ aux0 | a & (b |
		~a) | (a & aux0 | b & aux0 | b | a | aux0 ^ b) & (aux0 & (a
		^ aux0) | ~(a ^ aux1))) & (~(~aux0 | a | aux1) | aux0) |
		aux0) + 64214 * ~(aux1 & ((~(aux1 ^ a) | ~(aux1 ^ aux1)) &
		((aux0 ^ aux0) & b ^ ~a) | a | ~((a | aux0) & ~aux1)) &
		~(~((a ^ b) & (a ^ b) ^ ~b & (b | aux1)) & aux0 & (~(a |
		aux0) ^ (~aux1 | aux1 ^ a)))) + 45722 * (aux1 ^ ~(~(aux0 & b
		& aux1) ^ a & (aux0 | aux1) & b | (aux0 ^ ~aux0 & aux1 & b)
		& b & aux0 & a & b) ^ ((a | ~(b ^ aux0 ^ aux0 ^ b)) ^ aux1)
		& ~~(b & aux0 & (b | aux0) | ~(aux1 | a))) + 54837 * ((~a ^
		aux0 & (aux0 ^ aux1 | aux0 ^ a) & (b | b & aux0) & ~(aux0 |
		a) ^ aux1) & ~~((a | aux0 ^ a) ^ ~(a | aux0) | ~b) | (aux1 ^
		~~aux1 & ((aux0 | aux1) ^ aux0 & aux0) ^ (~aux1 ^ aux0 ^
		aux1 | ~(b ^ aux1))) & aux1 & (a | ~(aux1 ^ b)) & (~~(aux1 &
		aux0) ^ a) & (aux1 ^ ~(aux0 | b | aux0 & aux0 & aux1 |
		~aux1))) + 11298 * (((b ^ a ^ ~aux0) & ~(aux0 ^ aux1) ^ b ^
		b ^ b ^ b | ~a) ^ (~b | (~(a & aux1) ^ (b ^ aux0) & aux0 &
		a) & (b ^ aux1 ^ (aux0 | aux1) | aux0 | aux1)) ^ aux1 ^
		~~(aux0 | aux1 | ~b) & ~~a | aux1) + 45743 * (b ^ (~~(aux1 &
		~a & b & aux1 & a & b) & (((aux0 | b) & (aux1 ^ b) | aux0 &
		aux1 & a & aux1) ^ ~(b ^ aux1) ^ aux0) & (~aux1 & (aux0 |
		~aux1) | aux0 | aux0 & aux0 | aux1 ^ a) | ~~(~((aux1 | b) ^
		(aux1 | b)) & b))) + 40867 * (~a ^ b & (b & a ^ ~aux1) | b ^
		(b & (aux0 | aux0 ^ a) | aux1 | b | aux1 | aux0 | ~b ^ ~a |
		b & ~(aux0 & b | a) | ~(~(a ^ b) | a) | aux1)) + 7361 *
		~(aux1 & ((aux1 | b) ^ aux1 | ~~aux0 | (aux1 & b | aux0 | b)
		& aux1) | (aux0 ^ ~~a ^ b) & (aux1 ^ aux0 ^ b) | ~((~(b | a
		| b ^ aux0) | a) ^ a)) + 18040 * ((~((aux0 | aux0 | b) &
		(aux1 | a) & (b | a) ^ (a & aux1 & (a ^ a) | b & aux0 ^
		~aux1) | ~~b) | b) ^ (~((b ^ b) & ~b) & (~aux0 ^ aux0 & a) &
		aux0 & ((a | a | aux0) & b & a & ~a ^ ~(a & b ^ aux1)) ^
		aux0 | ~~(a ^ ~~~a))) + 2264 * (((a & aux0 | a | b | b) ^
		(~(aux1 | aux1) | ~(aux1 ^ aux0))) & (~~(b ^ aux1) ^ (a |
		~~a)) & ~~(b & ~(a | aux1)) ^ ((~(aux0 ^ a) | aux1 | b &
		aux0 | b | a) ^ ~(aux0 ^ (~aux0 | aux1 & a))) & b ^ aux0) +
		47647 * ((~~((~aux0 | aux1 ^ b) & ~aux0) & ((aux1 & b | aux0
		^ b) & aux1 | ~a & (aux1 | a) ^ ~(aux0 | a) | ~aux0) | (~b ^
		a ^ (aux0 & a | b ^ aux0) & (b | aux1 | ~a)) & (~~~~b ^ a))
		^ (a & aux1 | ~(~((aux1 | aux1 | aux0 ^ aux0) ^ a & (a ^
		aux1)) & b))) + 61131 * ((a ^ (~(aux1 & a & a) ^ ~(aux0 ^
		aux1 & aux0) | ((aux1 | aux0) & a & aux1 | aux0) & ~aux0) |
		~((aux0 ^ aux1 ^ a ^ ~b & aux0) & (aux0 & b ^ a ^ aux1) & a
		& (~(b | a) ^ a ^ a) & (a ^ b) & ~(aux0 & b))) ^ a) + 24611
		* (aux0 & (a ^ ((a | (a | b) & (aux1 | a)) ^ (aux1 & ~a | ~b
		| b ^ aux0) | (b | ~a ^ aux0 ^ a) & (~(a | aux0) ^ ~aux1 &
		(a ^ aux1))) ^ ~~(~(aux1 | b) & ((b | b) ^ aux1 & aux1) ^
		~~aux1))) + 53702 * (~(~aux0 & ~aux1 ^ aux0 & aux1 & (aux0 ^
		a) ^ aux1 & a & aux1 ^ a & (aux0 | a) ^ (b | ~a & ~a)) &
		aux0 ^ ~aux0) + 62382 * ((a ^ (aux1 ^ a ^ aux1 & aux1 ^ aux1
		^ aux1) & ~aux0 ^ ~(aux1 ^ (a | a | b)) ^ aux1) & a) + 3576
		* (((~aux1 ^ (b | a ^ aux0) ^ aux0) & a ^ (aux1 | aux0)) &
		((aux0 ^ aux0) & (b | aux1) & ~aux0 & ~(b & a | aux1 & b) ^
		~a ^ (b & b ^ aux1 | a)) & b | a ^ ~((a ^ (aux0 | aux1)) &
		aux0) & a ^ (a | ~(~b | b | b) ^ aux1 & b & (aux1 | a) ^ (b
		| aux0 | ~a) ^ ~(aux1 & ~(b | aux0)))) + 58647 * ~(~((~a & b
		& aux1 | ~(aux0 & b)) ^ aux1 ^ aux1 | ~(~aux0 | a ^ a |
		aux0)) | ((aux0 & aux1 ^ aux0 & aux1) & (aux0 ^ b) ^ aux1) &
		(b ^ (a | a) | a | ~(b & b ^ (a | aux0))) ^ aux0) + 26441 *
		((~((aux0 & b & aux0 & b | aux1) & (~b | b | aux0 ^ a) &
		aux0) ^ ~((a & aux1 ^ (a | aux1)) & (a | aux0) & aux1 & aux0
		& (~aux1 | ~(aux0 ^ a))) ^ a) & ((~aux1 ^ (aux0 ^ aux0 |
		aux0 | aux1) ^ aux1 | a) ^ ((a | b) & ~aux0 | ~a | a) &
		(aux0 ^ b | b ^ a) & a & aux1 & (a ^ aux1 ^ aux1 ^ a) &
		~((aux0 | aux0) & ~aux1) ^ ~(~(aux0 & a & b) & (a ^ a) |
		aux0 & ~(aux1 & a) | ~~aux0 | aux0))) + 27494 * (b & a & (b
		& aux1 ^ (aux0 & a & (aux0 ^ aux1) | ~aux1 & (aux1 ^ aux0))
		^ ~(a | aux1 | a & aux0) | ~b | b) & ((aux1 ^ aux1 & b | ~a)
		^ aux0 | ~(aux1 & aux0 ^ a ^ aux1) & ~aux0 | ~((b | a) ^
		aux0 & aux1 | aux0 | aux1 | b ^ aux0) ^ (b | b & aux1 ^
		~aux1 | (aux1 ^ b) & (aux1 | a)))) + 18987 * ((((a & aux1 &
		~b ^ ~aux1 & (a ^ b) | ~(aux1 & aux0 & aux0)) ^ ~aux1) &
		(aux1 | aux0 | (b ^ aux1) & ~b) & (b | ~aux0) ^ ~((a ^ a |
		aux0) & ~b & (~(aux1 ^ b) ^ a & aux1 ^ ~aux0))) & ~aux0 &
		((~aux0 | aux1) ^ aux0 ^ b & ~((b | b) & aux0) | (aux0 ^
		aux0) & (aux0 ^ (a | aux0)) & aux1 ^ (~a & (aux0 | a) ^ aux0
		| aux0 ^ a ^ aux0 ^ (a | a | aux0)))) + 42092 * (~(b | (b &
		aux0 ^ b) & (b | a | b ^ aux1) & b) | ~(aux1 & ~((b | aux1)
		^ aux1) | (~b | aux1) ^ aux0 & b & a & aux0 ^ (b | b | ~b |
		aux0)) | a) + 48815 * ~(((aux0 ^ a | b ^ a) & (aux1 ^ a | a)
		& ((aux1 & a | aux0 ^ aux0) ^ b ^ a ^ (aux0 | b)) ^ ((a |
		aux1) & (a | aux0) & b | (b | b) ^ aux1 | (b | a) & ~a) |
		~((a & a | a ^ a) & (aux0 & a | aux0)) ^ ~aux1) ^ ~(a ^ (b |
		a ^ a) ^ ~a ^ ~(aux0 & b ^ ~aux0))) + 37369 * (aux0 & ((aux0
		| ~(~(a ^ a) | b)) ^ aux1) & (((b ^ a | aux1 & aux1) ^ aux1
		& aux0 & ~b) & b ^ b ^ ~aux0 ^ a | b)) + 15205 * (~(aux0 & a
		& ~(~(~b & (b | a)) & (aux1 ^ a | aux0 | ~(a ^ a)))) | (b &
		aux0 & (~(aux0 & a) ^ (aux1 | b) ^ a & aux1) | (a ^ ~a) & a
		& ~a ^ (b | b) ^ (b | aux0) ^ a ^ b ^ aux1 ^ aux1) & (aux1 ^
		b ^ aux0 ^ aux0) & b & b & a) + 4969 * (aux1 & (a ^ ~~(~aux0
		^ (a | aux0 | aux1)) | (aux0 & aux1 ^ aux1 & b | (aux1 | a)
		^ (a | aux1)) ^ b & (aux0 | a | aux0 | b) ^ (aux0 | (aux1 ^
		a) & ~a) & a | ~b)) + 54922 * (a | (b | a | a) ^ ~aux0) +
		23112 * (~(~~aux1 | ~~(aux1 ^ aux1) & ((b | ~a) ^ (a & aux0
		| a ^ b))) & a & ~(aux1 & (aux0 | aux1) ^ aux1 & a ^ ~b) &
		~aux1 ^ (((b & aux1 | a & aux1) & ~~aux1 | ~a & (aux0 |
		aux1) | (aux1 | aux0) ^ a & b) & (~aux0 & (a ^ a) | a & aux0
		| aux0) & b ^ ~(b & a & aux0 ^ ~(b | aux0) | ~aux0)) &
		~(aux0 & ~~(a | a ^ b))) + 23005 * ~(a & ((b | aux0 ^ aux0)
		^ (b | a) & aux1 & aux1) & aux1 & ~((aux1 & aux1 ^ a & a |
		b) & ~(aux0 ^ aux0 ^ ~aux1)) ^ b) + 25986 * ~~((~~((aux1 ^
		a) & a & a) ^ a) & (~~(b & aux1 | b & a) ^ a)) + 36805 *
		~~(aux0 ^ (~~aux0 | b ^ aux0) & ~(aux0 | aux0 | aux0 ^
		aux1)) + 1652 * (~(~(aux1 | aux1 ^ aux1 & b | a & (b ^ aux0)
		^ a) & b) ^ (~((b | aux0 & a) ^ ~b ^ b | b) | ~aux1)) +
		16263 * (~aux1 & (~(~a ^ aux0 ^ a ^ aux1 ^ b ^ a ^ (b ^
		~aux0 | b ^ aux1 | ~a) ^ ~~(aux1 & b | aux0)) ^ a)) + 42336
		* ((a & aux0 ^ (a | aux1) ^ (aux1 & aux1 | b | aux1) ^ ~aux0
		^ ~aux1 ^ (b ^ aux0 | b ^ aux0)) & (aux1 & aux1 & aux0 &
		aux0 ^ ~~a) & ~(aux0 ^ aux0 | aux1) & ~~(aux0 | ~aux1) &
		aux1 ^ ~(((aux1 | aux1) ^ b ^ aux0) & (~aux1 ^ b) & b) &
		((aux1 | ~~aux0) ^ (aux0 & aux1 ^ aux1 ^ aux1 | (aux1 | b) ^
		aux1 | ~~(aux1 & aux0))) ^ aux1) + 36260 * ((aux1 ^ aux1) &
		b) + 58315 * (b ^ ~a & a) + 53171 * (~~((~aux0 | ~a | aux0)
		^ ~b ^ aux0 ^ aux1 ^ ~(aux1 & b)) & b ^ ~(~((aux1 ^ a | aux1
		| aux0) ^ a | b) & aux0)) + 60693 * ~~a + 48981 * ~(a &
		~((~b | ~b | b | (b & aux1 | b ^ aux1) & (~b | aux0 & a)) &
		((b ^ (aux0 | a)) & ~(b ^ aux0) | a & aux1 & (aux0 ^ aux1) |
		(a ^ aux0) & b & b))) + 62103 * (~b | ~~(~aux0 ^ aux0 ^ b ^
		(b | aux0) ^ (aux1 | aux0 & a) | (a & aux1 | aux0 & a) & a |
		~~aux1)) + 35003 * ((a & b ^ ~a ^ (b & b | aux1 ^ b) | aux0
		| ~~a | ~((~(b & aux1) | ~~a) ^ ~a & ~aux0 & (aux0 ^ aux1 ^
		aux1 ^ aux0))) ^ b ^ ~(aux0 | (a & aux0 & ~a | ~(aux0 | a))
		^ b ^ ~aux0 | ~(~aux1 & (aux1 ^ aux1) & aux1 ^ a ^ ~(a |
		aux1)))) + 5774 * (aux1 & aux0 | aux1 & ((aux0 ^ ~((aux0 ^
		a) & (a ^ a))) & aux0 | (~a | aux0 | ~(aux1 ^ b)) ^ ~(a |
		~aux0))) + 7405 * (~~~~b | a | (~aux1 | b | aux1 | aux1) &
		(aux0 | b) | a ^ b ^ aux0 ^ ~(aux1 & a) ^ aux1 ^ (aux0 | b)
		^ b & a ^ ~(aux0 & aux0) | ~(~(aux1 & b) | a & (b | a) |
		aux0 ^ (aux0 | aux1 | b & aux1)) | ~aux0) + 40772 * (((aux1
		^ aux0 | (a ^ aux0 ^ aux0) & aux0 | (aux1 ^ aux1 | aux1 &
		aux1) & (aux1 ^ b | a & aux1) ^ ~((aux0 ^ aux0) & a)) ^
		((aux0 | a) & b & (aux1 | b) & ~(a ^ (aux0 | aux1)) |
		~((aux0 | aux0) ^ (a | aux1) ^ (b ^ aux0) & a))) & aux1 ^
		((aux0 ^ b) & ~aux0 ^ ~~a ^ aux0 | aux1 | aux1 ^ ~aux1) &
		(~aux0 | ~aux1 & aux0 & b) & (a | aux0 & (b ^ b)) & (~(b ^
		aux0) ^ (a | ~(aux1 ^ aux0))) ^ a) + 14766 * ~(((~aux0 | a)
		& b ^ (aux0 ^ ~(aux1 ^ ~a) | (aux0 | b ^ aux0 ^ aux0) & (b |
		~b | aux1 ^ b))) & ~(~a | ~~(b | aux1) | ~((aux0 ^ b) &
		~aux1)));
    vm->memory[--vm->sp] = result;
    // std::cout << "[DEBUG] XOR: 0x" << std::hex << a << " ^ 0x" << b << " = 0x" << result << std::dec << " [sp=" << vm->sp << "]" << std::endl;

    WORD next = vm->fetch_opcode();
    vm->handler_table[next](vm);
}

void VirtualMachine::op_not(VirtualMachine* vm){
    WORD a = vm->memory[vm->sp++];
    WORD aux0 = a * a;
    WORD result = 55701 + 34984 * ~(~(aux0 & ~(a | aux0 | aux0)) & (~a |
		aux0 & aux0 & aux0 & aux0 ^ a ^ a ^ a & aux0 | ~(a & a ^
		(aux0 | a))) & ~(~~aux0 & ((aux0 | aux0) ^ aux0 & a | ~aux0)
		& ~((a ^ a) & a & aux0) & a)) + 25728 * (~(~(a | ~~(aux0 ^
		aux0)) ^ a) ^ ((~~aux0 | ~(a | aux0) | a) & a ^ ~(~~(aux0 |
		a) ^ (a | ~(a ^ a)))) & (~~~(~a & (a ^ aux0)) | a & (~~a |
		aux0 | ~~(aux0 | a)))) + 48234 * aux0 + 33710 * ((~a | (aux0
		^ aux0) & aux0 ^ aux0 ^ (aux0 & (a ^ a) | aux0 & a) ^ ~(aux0
		^ aux0 ^ a) ^ ~(a ^ aux0) ^ (aux0 ^ ~(a ^ aux0) ^ ~(a ^ ~a)
		| ~(~aux0 | a & aux0) ^ ~(aux0 & a | a ^ a))) ^ (aux0 & ((a
		| a ^ aux0 ^ ~a) ^ ~~(a & a)) ^ ~~~((a | aux0) ^ a) |
		~((aux0 & (aux0 ^ a) | (aux0 | a) ^ (aux0 | a)) & (a ^ (aux0
		^ a | ~a)) | a & (a | ~a ^ aux0 ^ aux0)))) + 34357 *
		(((~(aux0 ^ a) | aux0 ^ aux0 | ~a) ^ aux0 & a ^ (~(a ^ aux0
		^ aux0) | aux0 & a & (a | aux0) ^ (a ^ a) & ~a)) & ~((aux0 |
		a | aux0 | aux0 ^ a ^ a) ^ (a | a | a & aux0) & (a ^ aux0 ^
		aux0 & aux0)) & (a ^ ~aux0) ^ ~(a & aux0 ^ (aux0 | aux0 |
		aux0) ^ aux0 & ~~aux0) ^ ~a ^ (aux0 | a)) + 26613 * (~(a | a
		& (~aux0 | ~aux0) | (a ^ a ^ aux0 ^ aux0) & aux0 | aux0 | a
		& aux0 | ~aux0) ^ ~(~aux0 ^ ((a ^ a) & ~aux0 & ~a | aux0 &
		aux0 & (aux0 ^ a) ^ a & a ^ aux0 & aux0)) ^ ~~~~(~a ^
		~aux0)) + 3104 * ~((aux0 | a & aux0 ^ a & aux0 | a ^ aux0 |
		~aux0 | ~~aux0 | (a ^ a | a) & ~a & aux0 | aux0 ^ aux0 ^
		~aux0 ^ ~a & (a ^ aux0)) & (aux0 ^ (~aux0 | ~(a | a & a))) &
		a) + 27036 * ~aux0 + 55715 * ((aux0 ^ a ^ (aux0 | aux0 ^
		aux0 | aux0 | (a | ~a) & (aux0 ^ aux0 & a) | ~aux0)) & aux0
		& (a | ~(((a ^ a | a ^ a) ^ (a | aux0)) & ~(aux0 & a ^ a &
		a)) | ~(aux0 & a))) + 38139 * (~((~(~a & (aux0 ^ a)) | ~(a &
		a) ^ aux0) & (a ^ a ^ ~a | aux0) & aux0 & ~aux0) ^ ~(a &
		((aux0 ^ aux0) & ~aux0 & (a | aux0 | a | aux0) | (a ^ a | a
		& aux0) ^ (a | a | ~aux0) | ~(a | aux0 & aux0) & a))) +
		22020 * (((~a ^ aux0 & ~(a | a) & (a | aux0) & aux0) & a ^
		(aux0 | a)) & a & ((a & ~aux0 & aux0 & aux0 | ~(aux0 ^ aux0
		| aux0)) & (~~(aux0 & a) | (aux0 & a ^ a) & (a & a | aux0 |
		a)) ^ a)) + 43194 * (((aux0 ^ a ^ a & a) & (a ^ aux0 ^ ~a) &
		~(a | a | aux0 ^ aux0) ^ ~(aux0 & aux0 & (aux0 ^ a) | (aux0
		| aux0) & a & aux0) ^ aux0 ^ (aux0 | ~(a & aux0) | (a ^
		aux0) & (aux0 | aux0))) & a ^ (~(a ^ aux0) | ~(~~a & a & a &
		(aux0 ^ aux0)) | a & (~((aux0 | aux0) ^ ~aux0) | a ^ ~a ^
		aux0 & ~aux0)) & a & (((aux0 | aux0 | aux0) ^ ~aux0 | ~aux0
		| ~(a | aux0)) ^ ~(aux0 ^ aux0) & (aux0 ^ (aux0 | a)) &
		(aux0 & a & (aux0 ^ a) ^ aux0 & a & (a ^ a)))) + 43985 * ((a
		& ~(aux0 & a ^ aux0 ^ ~a) | ~((a & a | aux0 ^ a) ^ ~(a & a))
		^ ((~aux0 | a) ^ aux0 & aux0 & (aux0 | aux0) | a ^ ~(aux0 ^
		a))) & (aux0 | a) | ((a | aux0) ^ (~a | ~a) ^ ~(aux0 ^ aux0)
		| ~(~(aux0 | aux0) ^ ~(aux0 & aux0))) ^ a & aux0 & aux0 ^ a)
		+ 55420 * ~((~(aux0 | aux0 & aux0 & aux0 | a ^ (a | a) ^ (a
		| a) ^ (aux0 | aux0)) | a & aux0 | aux0 ^ ~(a | aux0) ^ (~a
		| a) & aux0) & ~~((a | ~(aux0 ^ aux0)) ^ ~aux0 & a & a)) +
		37211 * ((aux0 | ~aux0 ^ aux0) & (~(aux0 & a | aux0 & a) |
		~(aux0 ^ a ^ aux0)) & ~~(a & (a ^ a ^ aux0 & a)) ^ ~(a ^
		((aux0 | aux0) ^ aux0 & a | (a ^ a) & a & a) & (~(aux0 |
		aux0) ^ ~a ^ a ^ a)) | (~((aux0 | a) ^ a) & (aux0 & a ^ aux0
		& aux0) & ~(a ^ a) | a) & aux0 & (aux0 | (aux0 & aux0 | a |
		aux0) ^ (aux0 | ~a) ^ ~aux0 | a)) + 47821 * (((~(a & aux0 &
		(aux0 ^ aux0)) | a) ^ (~(aux0 | aux0) ^ (a | aux0 | aux0)) &
		(a & a & a & aux0 ^ aux0)) & aux0 ^ (~(a & ~a) & (~aux0 &
		~aux0 ^ aux0) | a & aux0 & (a & a | a ^ a) & ~~aux0) & a |
		aux0) + 43527 * (~~a & aux0 | a) + 33987 * (a & (aux0 & a |
		aux0 & ~a ^ (a & aux0 | a | a | a) & ~((a | a) ^ (aux0 |
		aux0)) | ~(aux0 ^ a ^ a ^ aux0 & aux0 | ~(aux0 & a) & (aux0
		^ aux0) & ~aux0))) + 37746 * ((aux0 ^ (aux0 | a) ^ aux0) &
		(((aux0 ^ aux0 ^ a ^ aux0 | ~(a ^ a)) ^ (~a & aux0 | aux0 ^
		a & a)) & aux0 & aux0 & ~aux0 | a & a & (~a & (aux0 ^ aux0)
		| ~aux0) & a)) + 46840 * (a ^ aux0) + 20932 * ~(~~((aux0 |
		a) & ~~(a | aux0)) & ~~((a | a | ~a) & (a | aux0 & aux0) ^
		(a | a) & (aux0 ^ aux0) & (a & a ^ aux0 & a))) + 37912 *
		~~~((~(a ^ a | aux0 | aux0) ^ ~aux0 & a & aux0 & aux0 & a) &
		aux0) + 62202 * (((~((aux0 | aux0) ^ a ^ a ^ ~(aux0 & aux0))
		| aux0 | aux0 | ~((a ^ a ^ a) & a)) ^ a) & ~aux0 & ~((aux0 ^
		a) & ~a ^ (~a | ~a)) & ((~a ^ a ^ aux0) & (~a | a & a) ^ (a
		^ aux0) & (aux0 | aux0) ^ aux0 & a) & (~(a ^ a) ^ ~(aux0 |
		a) | aux0 | aux0 | aux0 | aux0 | ~aux0)) + 14139 * (~a &
		~((aux0 ^ a ^ (aux0 | a) | aux0 & (a ^ aux0)) ^ a & (a | a)
		& a | a) ^ (a & a | ~(~aux0 & a & a) ^ aux0 ^ ~(a & aux0 &
		(a ^ a)) ^ aux0 & ~~a) & (~aux0 ^ (~a | a | aux0) ^ aux0 ^
		~(a | aux0) ^ (a | a | aux0) | aux0 | aux0 | ~((a ^ aux0) &
		(a | a)))) + 55698 * (aux0 ^ a ^ aux0 ^ aux0) + 55415 * ((~a
		| ~(aux0 | ~~(a | a)) | aux0 | ~(~(aux0 & a) ^ (a | a) &
		(aux0 ^ aux0)) | aux0) ^ ~(((aux0 | aux0 | aux0 & a) ^ a &
		aux0 ^ aux0 ^ a | ~(aux0 ^ aux0) ^ (aux0 | a | ~aux0)) &
		~((aux0 & aux0 | aux0 ^ aux0) & ~(aux0 | a)) ^ ~(aux0 ^ a ^
		aux0 & aux0 | ~a & aux0) ^ aux0 & (~(a ^ a) | ~aux0 | ~a)))
		+ 4252 * (a | a) + 20086 * a + 17586 * ~(~((a ^ a) & ~aux0 &
		(a | a) & aux0 & (a | aux0 | aux0 | a) & (aux0 & a ^ aux0))
		& ~((aux0 & aux0 | a ^ aux0) & aux0 & aux0 & aux0 & (aux0 &
		aux0 | aux0 & aux0) & ~(a & a)) | ~(~aux0 & ~~aux0 ^ aux0 &
		(a ^ aux0) ^ aux0) ^ ~(aux0 ^ aux0 ^ ~aux0 | ~(aux0 & aux0)
		^ (aux0 & aux0 | a ^ aux0))) + 51395 * (~(~(~~aux0 | (a ^ a)
		& (aux0 | a)) & ~~a | (aux0 | a | aux0 | a) & a ^ (~~aux0 |
		~(a & aux0)) ^ (a ^ ~(a & a) | aux0 | a)) | a & (~(~~a & (a
		& a ^ a)) ^ ~~((a | a) ^ a & a)) | ~(a & a) ^ (aux0 | a | a)
		& ~(a | aux0) & a | (~(~aux0 | ~a) | ~~a ^ (aux0 & aux0 |
		~aux0)) & ~~(a | aux0 | aux0)) + 36713 * ~~(((~((aux0 | a) &
		aux0 & a) | ~a ^ (a | aux0) ^ (aux0 | aux0) ^ a) ^ (a &
		(aux0 | aux0) ^ aux0 & aux0 ^ ~aux0 | a | a | aux0 | aux0 ^
		aux0 | a & aux0)) & aux0 & a & ((~a | ~a) ^ ~aux0 ^ a & a))
		+ 33791 * ~a + 42133 * (aux0 ^ aux0 ^ aux0 | a) + 50568 *
		(~aux0 | ~(~aux0 & (~(aux0 & (a ^ aux0)) | ~(a & (aux0 ^
		aux0)) | a ^ (~aux0 | ~a) | a & aux0 ^ aux0 ^ aux0 | a ^ a ^
		aux0))) + 58976 * ~(a & (~~(~(a ^ aux0) | aux0 ^ a | a | a)
		| a)) + 4036 * (~a | ~(~aux0 ^ a ^ a & ~~a) & ~(((aux0 &
		aux0 ^ aux0 & aux0) & (~a ^ (a | aux0)) ^ (aux0 | a) ^ aux0
		^ a) & (a ^ a ^ (a & a | aux0 | aux0) ^ aux0))) + 26237 *
		(~aux0 ^ (~(a & aux0 & ((a | aux0) ^ a ^ aux0)) ^ a & ~~~(a
		^ aux0)) & ((aux0 | a | aux0 | aux0 & aux0) ^ aux0 ^ (a |
		~(a ^ aux0 ^ a)) ^ (~a ^ (aux0 ^ aux0) & (aux0 ^ aux0) ^
		((aux0 ^ aux0) & a | a & aux0 | aux0 & a) | ~(aux0 ^ aux0 ^
		(a | a) | aux0 & aux0 | ~aux0)))) + 7685 * ~(a & ((a & a | a
		^ a) ^ a | ~((a | a) & a)) & aux0 & ~(~a | ~(aux0 & aux0)) &
		a) + 35882 * ~~a + 36709 * (~~(~(~a & (aux0 & a ^ (a | a)))
		& (aux0 ^ aux0 ^ aux0 & a) & ~aux0 & (a | a) & ~~aux0) &
		((~(~a ^ ~a) | (a | a) ^ ~aux0 ^ ~~aux0 | ((a | a) & a | a ^
		a ^ a) ^ ~(~a ^ a & aux0)) & (a | ~(aux0 ^ (a | a)) | a & a
		& ~a & (aux0 | a) & ~a) | a & (a ^ aux0) & (a | ~((a ^ aux0)
		& aux0)))) + 48897 * (a ^ ~~(~~aux0 | ~(a | a) | (~a | a) ^
		~(a ^ a)) ^ (aux0 & (~(a & (a | a)) | (a | aux0 ^ aux0) &
		aux0 & a & (a ^ a)) | a & (aux0 ^ (aux0 ^ a | aux0 & aux0))
		^ (aux0 | (aux0 | a) ^ (aux0 | aux0)) & (a | aux0) & ~aux0 &
		~a | ~~aux0)) + 3417 * ((~a & aux0 | a | (aux0 | aux0) ^ a ^
		a | aux0) ^ a | a) + 34014 * (~(~aux0 | (a ^ aux0 | ~a) ^
		~(aux0 ^ aux0) | ~(~aux0 | a & aux0) ^ (aux0 | a & aux0 &
		~a)) & a ^ a) + 57698 * (~a ^ (a | ~(aux0 | aux0) | aux0 |
		~(aux0 & (aux0 ^ aux0) ^ a)) ^ ~(a ^ a) ^ aux0) + 47486 *
		(((~(a & a) & ~a | ~(a & a & (aux0 ^ aux0)) | a & (aux0 ^
		a)) ^ ~~(aux0 ^ aux0 ^ (a | a) | a & aux0 | aux0 ^ aux0) |
		~(~~(a & aux0) ^ (a ^ a) & (aux0 | a) ^ aux0 & a & aux0 &
		aux0) ^ ((aux0 | a | ~aux0) & (~a | a & aux0) | ~(aux0 ^
		aux0 | ~aux0)) & (~aux0 | a & aux0) & (a | aux0) & (aux0 ^ a
		& a & ~a)) ^ ~~(~a | ~~(~aux0 & ~a))) + 62794 * (aux0 &
		aux0) + 18199 * ~((~a ^ ~~a ^ a) & (((~~a | aux0 ^ aux0 ^
		aux0 & aux0) & ~~(a | aux0) | a) ^ ~(~a | aux0))) + 65276 *
		((aux0 | aux0) & a & a ^ (~(a | ~a ^ ~aux0) ^ aux0 ^ (a ^
		aux0 ^ aux0 & aux0) & (aux0 & aux0 ^ ~a) ^ a ^ (~a ^ a & a ^
		a ^ a) & (~a ^ (a | aux0) ^ (aux0 | ~aux0))) & ~(~(~aux0 &
		~aux0 | ~(aux0 & aux0)) | ~((a | aux0) ^ ~a) & ~(aux0 |
		aux0) & ~(a | a))) + 16374 * ~(aux0 | aux0 & ((a | aux0 | a
		& aux0 | a | aux0 | ~(a & (a | aux0))) ^ ~(~aux0 & ((aux0 |
		aux0) ^ ~a)))) + 35206 * ~((a ^ a | aux0 | a & ~(~aux0 |
		~aux0) | ~~a ^ a ^ ~(aux0 | a) & a) ^ (~(aux0 & a & (a ^
		aux0) ^ ~(aux0 & aux0)) | (aux0 ^ (a | a) | (a ^ aux0) & a)
		& a) & ((aux0 ^ aux0) & aux0 & aux0 ^ ~aux0 & aux0 & aux0 ^
		~(a ^ a | aux0 | a)) & ~(a & (aux0 | a) ^ (aux0 ^ aux0) &
		(aux0 ^ aux0))) + 28703 * (aux0 & a ^ ((a ^ (aux0 | a)) &
		~(a & aux0) & (~(aux0 & a) | aux0) ^ ~a & aux0 & (aux0 | a))
		& ((~(aux0 & aux0) & a & a & ~a | ~((aux0 ^ a) & a & a)) ^
		aux0) ^ (a ^ ~a ^ a ^ ~((a ^ a) & a) ^ a | (~aux0 | aux0) &
		aux0)) + 55499 * ((~(a & aux0 & a) | aux0) & (~aux0 | ~(a |
		~(a & a | a ^ a))) | ((~a ^ ~aux0 ^ a & aux0 ^ aux0 & a |
		aux0 ^ a) ^ (~(a & aux0) | ~aux0 | aux0 ^ ~a ^ ~aux0 ^ (a |
		aux0))) & aux0 & a) + 14202 * ((~((aux0 | aux0 | a & aux0) ^
		a) & (~(~aux0 ^ a & a) | ~a & (a ^ a) | aux0) | (aux0 ^ aux0
		^ ~aux0) & aux0 & a & a & a ^ ~~a ^ (aux0 ^ a) & (aux0 |
		aux0) ^ aux0 | aux0) ^ ~aux0) + 26316 * (((a | aux0 | a & a
		| a | a | ~(aux0 | a) | ~((aux0 ^ a | ~a) ^ (a | aux0 | a ^
		a))) ^ ~((aux0 | ~a) & (a | aux0 ^ a)) ^ (aux0 | (a ^ aux0 ^
		a & aux0) & ~aux0 & aux0) ^ (a ^ ~(~a | aux0 & aux0)) & aux0
		^ (aux0 ^ aux0 ^ aux0) & (a ^ aux0 ^ ~a) ^ ((a | aux0) ^ a ^
		aux0 | ~a ^ aux0 ^ a) ^ a) & aux0) + 54210 * (~(~~(a & a) ^
		(~a | a & aux0) ^ a) & ~~(~~aux0 | a) ^ aux0 ^ (a | (~(~(a &
		aux0) & ~(a ^ a)) | a) & (a | aux0 | ~(aux0 & aux0 | aux0 |
		a)))) + 41421 * (aux0 & (~a & aux0 & a & (aux0 & aux0 |
		aux0) & ~(a & a | a | aux0) & a ^ ~((a & aux0 | aux0) & (a |
		a | aux0)) & aux0) & (a ^ (~a | aux0 ^ aux0 | a | a | a ^ a)
		& (a ^ aux0 ^ (a | aux0)) & aux0) & ~(~a & aux0 ^ (~aux0 ^
		~a) & (aux0 | aux0 | aux0))) + 35373 * (aux0 ^ aux0 ^ (a & a
		| a ^ a) ^ ~aux0 ^ a ^ ~((a & aux0 ^ aux0 & aux0 | a & (aux0
		| aux0)) & (~aux0 | a)) ^ aux0 ^ aux0) + 35184 * (~(~a ^
		aux0) ^ (~~(aux0 & (a ^ aux0)) ^ (aux0 & aux0 & aux0 | aux0
		| ~a) & ~((a | aux0) ^ a & aux0) | ~(a | aux0 & aux0 | aux0
		| a ^ a) & ((a ^ a | a ^ aux0) ^ ~(aux0 ^ a)) & aux0) ^
		(~~(a & (aux0 ^ aux0)) | ~a | (aux0 & aux0 ^ (aux0 | a) |
		~aux0 & (a | a)) ^ ~~(a ^ a) ^ (aux0 & a & (aux0 | a) | ~(a
		^ a)) & (~~a | aux0 & a ^ ~aux0)) ^ aux0) + 30715 * (((aux0
		^ ~~(a ^ a)) & ~a | ((a | a | a) ^ (aux0 & a | aux0 ^ a) ^
		~~aux0 & aux0) & ((aux0 | a) & ~aux0 & ~(a & a) ^ ~~(aux0 &
		a)) | aux0) & (a | ~(a & ((aux0 ^ aux0) & a & aux0 & (a &
		aux0 | aux0 ^ a) | a | ~a ^ a & aux0)))) + 10169 * (~(~~aux0
		& ~(a | ~(a ^ aux0) ^ (a | aux0 | a | a))) | a ^ ~(~((aux0 |
		a) ^ ~a) | a ^ a ^ a ^ aux0 | aux0 & a & aux0 & aux0 | a)) +
		17479 * (~(aux0 | aux0 ^ (aux0 | a | ~aux0 | aux0 & a | a) &
		a) | ~(~(~a & (a ^ a & ~a)) ^ ~(aux0 & aux0 & a) & ~aux0 &
		aux0 & (a ^ a ^ aux0 ^ aux0 & (a | aux0) ^ aux0))) + 24421 *
		((a ^ a ^ ((a ^ aux0 | a & a) ^ a | aux0)) & aux0 & (~a ^
		~aux0 ^ aux0 ^ (a ^ aux0) & a) & a | ~(aux0 & ~aux0 & (aux0
		^ a ^ a) & ((a ^ aux0) & a ^ ~(a ^ a)) & ~(aux0 | a) & (~a |
		a & a) & (a & a & aux0 | ~a)) & ~aux0) + 31591 * (a ^ aux0 &
		~((a | aux0) & ~aux0 & a) & aux0 & (~(aux0 & a | a | aux0 |
		(a ^ a) & (a | a)) | aux0 ^ a ^ aux0 ^ a & a & aux0 ^ (a |
		a) & ~aux0 ^ aux0 & a & aux0)) + 50430 * ((~(aux0 & a ^
		~(~~aux0 | aux0 ^ aux0)) ^ (aux0 | ~(a & (aux0 ^ aux0 | a))
		^ ((~a | ~a) ^ aux0) & ~(a ^ a | a))) & (~aux0 ^ (((aux0 &
		aux0 ^ ~aux0) & ~(aux0 | a) | ~a & a ^ aux0) ^ a ^ (aux0 ^
		aux0 ^ a | (aux0 ^ a) & a) | (aux0 | ~a) ^ (aux0 | aux0) &
		(a | aux0) | ~~(aux0 & a) | a))) + 22630 * (aux0 & ((a |
		~(aux0 ^ a)) & ~~aux0 | (~(aux0 | aux0) ^ aux0) & (a & aux0
		& aux0 & aux0 ^ ~~aux0)) & (~(~(aux0 & aux0 & aux0 & aux0) &
		((aux0 | aux0) ^ a ^ ~(aux0 | aux0))) | a | aux0 ^ a ^ a &
		(a ^ a) & (aux0 ^ a ^ aux0)) | (~(aux0 & a) & aux0 & ~a | a
		^ ~aux0 ^ (aux0 & aux0 | a | aux0)) & (~(~aux0 & (a ^ a) |
		~(a ^ a)) ^ (a | aux0) & (aux0 ^ a) ^ a & aux0 & aux0 ^
		aux0) | ~a) + 2419 * ((aux0 & (a | a | ~a | aux0 ^ aux0 | a
		| aux0 & (a & a | ~aux0)) & aux0 ^ (~(~a ^ aux0 ^ a ^ a | a
		| (a | a) ^ ~a) | (~~aux0 | a | a ^ a | (aux0 | aux0) & a &
		~(a & a)) & (a & ~(aux0 & a) | ~(a | aux0) | a & aux0 ^ a &
		a))) & ((((a | aux0) & ~a ^ (a ^ a) & aux0 & aux0) & (~aux0
		^ aux0 & aux0 & a) | ~(a ^ a ^ (aux0 | aux0) ^ ~aux0 ^ a &
		aux0)) & aux0 | ~(~a | aux0 ^ a) | aux0 ^ (a | a) & a & aux0
		| ((aux0 | a) & (aux0 ^ a) | a) & (aux0 ^ ~aux0 ^ (aux0 ^ a)
		& ~aux0) | aux0)) + 3368 * (aux0 & ~aux0) + 49927 * (~(aux0
		& (a ^ a) & ((aux0 | a) ^ (a | aux0)) & ~~(a ^ a)) & ~aux0 &
		a & ~(a & aux0 ^ ~a ^ aux0 | aux0 & a | a | ~(a & a) | (aux0
		| a) & a & (a ^ a) & a & a & (a | a)) & aux0) + 53244 * (a &
		(a ^ (a & ~(a ^ a) & (~aux0 | ~aux0) & a | aux0))) + 13258 *
		(~~(~((a ^ a) & (aux0 | a)) & ~a) & aux0 | ~(a | ~(~(aux0 ^
		a & a) & a))) + 7800 * (aux0 ^ a) + 16179 * ((a ^ ~(~(aux0 &
		aux0) | a | aux0 | a ^ (a ^ aux0) & aux0 | a ^ aux0 ^ aux0 |
		aux0 & aux0 ^ (a | aux0) | aux0 & (a | a) ^ ~aux0 & aux0)) &
		~((a | ~(~aux0 & (aux0 | aux0))) & aux0 ^ (aux0 ^ aux0 |
		aux0) ^ ~a & a & a ^ ~~(a ^ aux0) ^ aux0)) + 34046 * (aux0 &
		a & ~(~(a & ~(aux0 & aux0)) & ~aux0)) + 1488 * ~~((~~a ^ a)
		& (a ^ aux0 ^ ~aux0) & (~a ^ aux0 & a) ^ a ^ aux0 & a & ~a &
		~~aux0 | (a & (aux0 | aux0 | aux0) ^ ~~a) & a) + 50100 * (~a
		| aux0) + 32044 * ~(~a ^ aux0) + 42615 * ((((~aux0 ^ a & a)
		& (aux0 ^ a) & (aux0 | a) ^ (~a | a ^ aux0 | a & a & aux0 &
		a)) & aux0 | ~((aux0 | a) & aux0 & aux0 ^ aux0 & aux0 ^
		aux0) | ~a ^ ~aux0 ^ ~~aux0 | (aux0 | aux0) ^ (aux0 | aux0)
		^ (aux0 | aux0) & aux0 & a) ^ aux0 & (a ^ a ^ a) & (~aux0 ^
		(aux0 & aux0 & aux0 | aux0)) ^ ((~((aux0 | a) ^ (a | a)) |
		aux0 & aux0 & a & aux0 & (aux0 | aux0 | a | a)) ^ aux0 & a ^
		~(a & a | aux0)) & (aux0 | ~(aux0 ^ aux0 ^ aux0) ^ aux0 & ~a
		^ ~~a) ^ (aux0 | (aux0 | ~aux0 ^ (aux0 | aux0)) ^ (a | ~aux0
		& aux0)) ^ ((a | ~(a & aux0)) ^ a) & (a | a ^ aux0 | a ^
		aux0 | a & aux0 & (a ^ a) | (aux0 ^ aux0) & ~aux0)) + 8902 *
		~(aux0 | a ^ (aux0 | aux0 | aux0 ^ aux0) & (aux0 & aux0 ^
		(aux0 | aux0)) ^ ~a | a) + 805 * ((~(((a | a) & ~a ^ aux0) &
		~~~a ^ aux0) | ~(aux0 & aux0 & a) & a & a & aux0 & a & (~a |
		~a) & ~(aux0 | (aux0 | a) & (aux0 | aux0)) | ~(aux0 ^ a ^ a
		^ aux0 ^ a ^ a | aux0 & ~~a)) ^ (~(a & aux0 & aux0 & a &
		aux0 & ~a) | (aux0 | ~~a) ^ ~(a | aux0)) ^ ~(~(a ^ aux0) ^ a
		& a | ~(aux0 & aux0) ^ ~a ^ (aux0 | aux0)) ^ ((~(a & (a ^
		aux0)) | ~a) ^ aux0 | a)) + 18247 * (a & aux0) + 60499 *
		(~~~(~~(aux0 | a) | ~~aux0 | ~a ^ a & a) & (a | ((a ^ a) &
		(a | aux0) | (aux0 | aux0) ^ aux0 ^ aux0 | a & a ^ a ^ aux0
		^ ~(aux0 | aux0)) ^ (a ^ ~aux0 & (aux0 ^ a) | ~(a & aux0)) ^
		a & (a & aux0 ^ (a | a) ^ a ^ ~((a | aux0) ^ aux0 & aux0))))
		+ 35795 * (~(aux0 ^ aux0 ^ a ^ a & aux0 ^ (a | a & aux0 |
		aux0) ^ ~(aux0 & aux0 ^ aux0 ^ a ^ (a | aux0 | a | aux0))) ^
		a | ~(a & ~~(aux0 | aux0) ^ ~(a ^ aux0 | aux0 | a) & (a ^
		(aux0 ^ a | a | aux0)) | (a ^ ~(a & aux0) & (aux0 ^ a ^
		~aux0)) & ~a)) + 38843 * ~(a | a & ((aux0 | ~~~a) ^ aux0 & a
		& a & (aux0 ^ (a | aux0 | aux0 ^ aux0)))) + 1768 * (~(~aux0
		& ~(~aux0 | aux0 | a | a) | ~~(aux0 & a & (aux0 | a) & (aux0
		^ a | ~a))) ^ ~aux0 & (((aux0 | a & a) ^ aux0 | ~(a | aux0)
		& a) ^ ~~aux0 & (a | a) & (a | a) & aux0 & a & a ^ aux0 ^
		aux0 ^ a)) + 53628 * (~a & ~(~a & ((a | a) ^ a | aux0 ^ a |
		aux0)) & (aux0 ^ aux0 ^ aux0 & aux0 ^ ~~a | ~(a ^ a ^ (a |
		aux0)) | (a ^ a) & aux0 & a ^ aux0 | aux0 & aux0 ^ (a |
		aux0) | ~(aux0 & a)) & aux0 & (a & (aux0 & aux0 & aux0 ^
		aux0) & (~(aux0 & a | aux0 & a) ^ (aux0 | a) & aux0 & a & ~a
		& (a | a)) ^ aux0) & a & (~(~a ^ (aux0 | aux0)) ^ (a | aux0)
		& ~a ^ (a | a) | a & (~aux0 | aux0) ^ (aux0 | aux0 | aux0 |
		aux0 & a))) + 8406 * (~a ^ ~((~a & a & aux0 & ~aux0 ^ aux0 &
		a ^ aux0 ^ a ^ ~~aux0) & (~a | ~a | aux0 | aux0)) ^ ~((~a |
		~a ^ a ^ aux0 | (aux0 ^ aux0 ^ aux0) & (aux0 | aux0 | ~a)) &
		a | aux0 | ~aux0 ^ (aux0 | a | a | a) & aux0)) + 27459 *
		((~(~(aux0 & a) & ~(a ^ aux0) ^ aux0 & aux0 & a & a | ~~a &
		~~(a | a)) | a | ((aux0 ^ aux0 | ~aux0) ^ (aux0 ^ aux0) & (a
		^ aux0)) & aux0 ^ ~(~a & (aux0 ^ a)) & a) & a) + 21636 *
		~(~((~aux0 ^ aux0 ^ a) & (aux0 & aux0 ^ aux0 ^ aux0)) &
		~aux0 & (aux0 & (~aux0 & a ^ (aux0 ^ aux0 | aux0 ^ a)) |
		~aux0) ^ ~~~(a | aux0) & (~aux0 & (a ^ a) | ~(a | a)) & a ^
		~aux0) + 21871 * (~~~((a ^ a) & ~(a & a) ^ aux0 ^ aux0 & a &
		~aux0) | (((aux0 | aux0) & (aux0 | a) ^ ~(a ^ a) | ~(aux0 |
		aux0) ^ ~~aux0) ^ ~~aux0 & (~aux0 ^ a & aux0) ^ (aux0 | a ^
		aux0) & (a ^ aux0)) & (a & ~(aux0 ^ aux0) & ((a | a) ^
		~aux0) & aux0 ^ ~(~aux0 ^ a & aux0) & (aux0 ^ ~aux0 ^
		~~aux0)) | ~((aux0 | aux0) & (~aux0 & (aux0 ^ aux0) & (aux0
		| a | aux0 | a) | ~aux0 & ~aux0 ^ aux0))) + 13570 * (~(~(~a
		^ (~(a & aux0) | (a ^ aux0) & (a | a))) ^ ~aux0) ^ aux0 ^ (a
		| ~(a ^ aux0 | aux0 | aux0) ^ aux0 | aux0 ^ ~aux0 ^ ~~a)) +
		40110 * (~~~(~a ^ (aux0 | a)) ^ ~(~~~a ^ aux0) ^ (~(a &
		~(aux0 & (a ^ aux0))) | aux0 & ~aux0 ^ (aux0 | a) & (aux0 ^
		aux0) ^ a ^ aux0) | a) + 2114 * ~~(aux0 & (~aux0 | aux0 ^ (a
		| aux0)) & aux0 & (a | aux0 ^ aux0 | ~aux0 | aux0) & ~(a & a
		& a) & aux0 & (aux0 & a ^ a ^ aux0)) + 27059 * ~((~~aux0 ^
		~aux0 & (aux0 ^ aux0) | a | a | a & aux0 | a ^ a | a ^ aux0)
		^ (aux0 ^ ~aux0 | ~(aux0 & aux0) | aux0 ^ (a ^ aux0) & a) ^
		aux0 ^ aux0 & ~(a ^ ~aux0) ^ aux0) + 31540 * ((aux0 ^ (aux0
		| a) & aux0 ^ a ^ ~(aux0 & a) | ~~a) & a & (~a & (a ^ aux0 |
		aux0 & a) & ~a & (a ^ a) & (aux0 ^ (~a | aux0 | aux0) & (a ^
		a ^ a & a)) ^ ~(aux0 ^ aux0) ^ ~aux0 | a & (aux0 ^ aux0 &
		(aux0 & a | a | a) & ~(a & aux0 | a ^ a)))) + 9335 * (aux0 &
		(((a | aux0) & (a | aux0) | aux0 & aux0 ^ a ^ aux0) & (~~a |
		aux0 & aux0 & (a | a)) ^ (aux0 | a)) & (((~(aux0 | aux0) | a
		& aux0 | aux0) ^ (aux0 | a & aux0) ^ a ^ a | ~a) ^ (aux0 |
		aux0 ^ aux0 ^ a ^ aux0 ^ aux0 | (aux0 & a | a) & aux0)) |
		(~~aux0 ^ (a | a) ^ a & aux0 ^ a ^ aux0 ^ a ^ a & aux0 ^
		aux0 ^ (a ^ aux0) & aux0 ^ aux0 | ~(~(aux0 & a) & (a | aux0
		^ a)) & (aux0 & a & (a | a) ^ a ^ aux0 | (aux0 & aux0 | ~a)
		& aux0)) & aux0) + 26473 * ~(~(aux0 ^ aux0 & a ^ ~aux0) & (a
		| a & aux0 ^ aux0) ^ aux0 | ~((~a | aux0 & aux0) ^ a ^ a ^ a
		^ a) ^ a ^ ((~(aux0 | aux0) ^ a & a & (aux0 | aux0)) &
		((aux0 ^ a) & ~aux0 | a ^ a ^ aux0) | a & aux0)) + 28138 *
		((a | ((~a ^ a & a | aux0) ^ ~(a ^ aux0) ^ ~aux0 ^ (~a ^ a &
		aux0 ^ ~(aux0 & a) | (~a | a) ^ aux0)) & ((~aux0 ^ (aux0 |
		a)) & (a ^ a) & (aux0 ^ aux0) | ~aux0 | ~aux0)) & (aux0 ^
		((aux0 ^ a) & ~a | aux0) ^ (aux0 ^ (aux0 | aux0)) & (a &
		aux0 ^ aux0 & aux0) & ~a & (aux0 ^ aux0) & aux0 | a ^ ~((a |
		aux0) & aux0 ^ (aux0 ^ a | ~aux0))) & (~a & (a ^ aux0 ^ ~a ^
		(a & a | aux0 ^ a)) ^ aux0 ^ ~aux0)) + 65055 * ((~(~((aux0 |
		aux0) & aux0) ^ aux0) ^ (a & (a | aux0) & ~aux0 | aux0 & a)
		^ ~(a | a) & aux0 ^ a) & (aux0 & aux0 | (a ^ aux0) & (a |
		aux0) & ~(aux0 | aux0) ^ ~~~aux0) & (a ^ ~((aux0 | a) &
		(aux0 ^ a) ^ aux0)) & a) + 7061 * (aux0 | ~(~aux0 & ~~~a)) +
		52713 * (aux0 ^ ((((aux0 ^ a) & (a | a) | aux0 ^ aux0 ^ (a |
		a)) ^ (~a | a & a) & a & aux0 & (aux0 | a) ^ (~(~a | aux0 &
		aux0) | ~((aux0 | a) ^ a & aux0))) & (aux0 & a | aux0 & aux0
		^ ~a ^ (aux0 ^ a | a | aux0) ^ aux0) | ~(~~a | ~(aux0 & a &
		(aux0 ^ aux0)) | aux0 | ~~aux0))) + 34648 * (~(~(~(a & aux0)
		^ (aux0 | aux0) ^ a & aux0) ^ (a | ~aux0 | a & aux0 ^ a) ^
		(~a ^ (aux0 | a) | a & aux0 ^ aux0)) & aux0 & ((aux0 | ~a &
		a | (aux0 ^ a ^ a & aux0) & aux0) & ~(a ^ (a | aux0) ^ a ^
		aux0 ^ (aux0 | aux0)) | a | (a | aux0 | ~~aux0) & (aux0 |
		~(aux0 ^ aux0) ^ ~aux0 & (aux0 ^ aux0)) | ~a)) + 17188 *
		(~(~(aux0 | a) & aux0) ^ a & (~((a | aux0) & aux0 | ~a &
		~aux0) & ~a | ~aux0 ^ (a ^ (a | a) | ~(aux0 ^ aux0)) |
		aux0)) + 37715 * (~(aux0 ^ aux0) & (~(a ^ a ^ a | (aux0 |
		aux0) ^ ~((aux0 | aux0) & a & a)) | a)) + 11009 * ~(~(a ^
		(aux0 | aux0 | aux0) ^ (aux0 ^ a | a)) & ~(a & aux0 & (a |
		a) & a & (aux0 & a & a & aux0 ^ ~(a | aux0))) ^ (aux0 | aux0
		| a | ~((aux0 | aux0 ^ aux0) & ~~a | (~a | a | aux0) ^ (a |
		a)))) + 57329 * (~aux0 ^ ~~(aux0 | ~aux0 | aux0 & a) ^
		(~~aux0 & (~aux0 | a | aux0) | aux0 ^ (aux0 & aux0 | ~a) |
		~~(aux0 | a) | (aux0 | aux0 | aux0 ^ a) ^ (~aux0 | a & a)) ^
		(aux0 | (a ^ a) & aux0 ^ aux0 & aux0 ^ (a | aux0)) & a ^
		aux0 ^ (a & ~~aux0 ^ ~(a & a) & (a & a | aux0 | a)) & aux0 &
		(a | ~~aux0 | a ^ a | a | ~~(a | a))) + 24916 * (~aux0 &
		((aux0 ^ a) & (~aux0 ^ ~(a ^ aux0) ^ ~(aux0 ^ a)) & ~~aux0 ^
		~aux0)) + 50883 * (a & ~(a ^ a | a) ^ aux0 ^ a ^ ~((aux0 ^ a
		| aux0 | aux0 | ~aux0 & (aux0 | a)) ^ a & aux0 & (a ^ aux0)
		& (~a | ~aux0)) ^ (((~aux0 | a) ^ ~(a & aux0)) & (~a & ~aux0
		^ aux0) | (~aux0 & a & aux0 | a | aux0 | a | a) & aux0) ^ (a
		& ~a & (aux0 | aux0) | ~(aux0 | aux0) | aux0 ^ aux0 | aux0 ^
		a | ~~(~aux0 & (a | aux0))) & ~((~(a ^ a) | aux0) ^ a ^ a ^
		aux0 ^ ~aux0 ^ (a | a)) ^ aux0) + 10882 * ((~(~(aux0 | a) |
		aux0 ^ a ^ a ^ a) ^ (~a | a & aux0 & a) ^ aux0) & aux0 &
		~(~(~a & (a ^ a)) & (a & a | a ^ a) & ((a | aux0) ^ a ^ a))
		| aux0) + 65401 * (((a | ~(~a & aux0 & aux0 & (aux0 | a ^
		aux0))) ^ a) & ~(aux0 | ~((aux0 | a) ^ a ^ aux0 | aux0 ^
		aux0 ^ aux0) | ((a ^ aux0) & ~aux0 ^ (a & a | a & a)) &
		aux0)) + 42957 * (~(~aux0 ^ (~(a ^ aux0 ^ aux0 & aux0) ^
		aux0 ^ (a | aux0 & a) | ~aux0)) | ~(~(~(aux0 ^ a ^ a) | ~(a
		& aux0) ^ a & aux0) | aux0 | ~(aux0 | aux0) & (a | aux0 | a
		& a) ^ (~a | aux0) ^ (aux0 | aux0) & ~a)) + 42311 * (~aux0 &
		((a & aux0 & aux0 & a | aux0 ^ a ^ (aux0 | a)) ^ ~~(aux0 ^
		aux0) ^ (~aux0 ^ ~(a & aux0)) & ~a | ~(((aux0 ^ aux0) & aux0
		& aux0 ^ aux0) & aux0)) & (a | a | (aux0 ^ aux0 | ~aux0) & a
		& aux0 & a & aux0 ^ ~aux0 & (a ^ a) ^ ~aux0 ^ aux0 ^ aux0 |
		aux0)) + 36015 * (~~(a & a) ^ (~(~(aux0 & aux0 & (aux0 | a))
		^ ~aux0 | a | aux0) | aux0)) + 43945 * ((a | a & ~~a & ~(a &
		a) & ((a | aux0) ^ ~aux0) | ((aux0 | a) ^ a & aux0 & (aux0 ^
		aux0) ^ ((a | a) ^ a ^ aux0 | (aux0 | a) ^ a)) & aux0) ^
		~(~((a ^ a) & aux0 & a) ^ ~a & aux0 & aux0 & (a | a)) ^ (a ^
		a & (a ^ aux0 | ~a)) & a ^ (~(~(a ^ (~a | aux0)) | aux0 &
		aux0 & ~~(a ^ aux0)) | ((~~aux0 ^ ~(aux0 ^ a)) & ~(aux0 & a
		| a | a) | a | ~(aux0 | a) | ~(a ^ aux0) | ~(aux0 | a)) &
		~(a & aux0) & aux0)) + 19591 * ((a | ~~~((a | a) & (a ^ a)))
		^ (aux0 & aux0 & aux0 ^ aux0 ^ ~(aux0 & aux0) ^ a) & (~((a ^
		aux0) & (a ^ a)) ^ (a ^ a ^ aux0) & ((aux0 | aux0) ^ ~a)) &
		~(~(a & aux0) & (a & a ^ ~a) ^ ~~(a ^ a)) | aux0) + 4649 *
		(aux0 & ~(aux0 | aux0 | ~aux0 | ~aux0 & (aux0 ^ a)) & ((aux0
		| aux0) ^ a) | (a ^ aux0) & (a & aux0 & (a | aux0) | ~aux0 ^
		~a | aux0 | ~~(~a | ~aux0)) | ~(~(aux0 ^ a | a) | ~aux0 | a
		| ~(~aux0 & ~a) ^ (a & aux0 | a ^ aux0) ^ a ^ a)) + 50617 *
		~(aux0 | a) + 17474 * (~(~(a & aux0) & ~(a & a) & ~aux0 &
		aux0 | ~a) & ((~(aux0 & (aux0 | aux0) & (a | aux0 | ~aux0))
		| ~((a ^ a) & (a ^ a)) ^ aux0 ^ ~(aux0 | aux0)) ^ ~(aux0 &
		((aux0 | aux0) ^ a)) ^ ~(~a & a) & ~(aux0 | a ^ a)) | aux0)
		+ 60428 * ((a ^ ~~((aux0 | aux0) & (aux0 | a) ^ (aux0 | a) ^
		aux0) ^ (~aux0 ^ (aux0 | a) | a & a & (a | a)) ^ a ^ a) &
		~(aux0 & aux0)) + 56330 * (aux0 & ((a | (aux0 ^ a) & (a ^ a)
		^ aux0 ^ (a | a) | a & (a & aux0 ^ a) & (a & aux0 ^ ~a) &
		(aux0 & a ^ a & a)) ^ ~~~(~aux0 | a)) ^ ((~~a | ~~(aux0 &
		(aux0 | a))) & (a ^ a & ~aux0 & aux0) | a)) + 44684 * (~(a ^
		(~(aux0 & aux0 & (a ^ aux0)) ^ ~(a & a) & (a & a | aux0 &
		aux0) | ~((aux0 ^ a ^ aux0) & ~(aux0 | a)))) & (a ^ aux0 ^ a
		& (~a ^ (a | aux0) ^ (a ^ a) & ~a) ^ aux0 & (~(~a ^ aux0 &
		aux0) | ~((a | aux0) ^ aux0)))) + 11012 * ((a | ~~((aux0 & a
		^ ~aux0) & a) | a) & (aux0 & ~((a ^ a) & (a | a)) & a ^ ~~a
		| ~(~((a & a | aux0 | aux0) ^ ~(aux0 | a)) ^ ((aux0 & aux0 |
		~a) ^ a | ~~~aux0)))) + 20605 * ((~aux0 | ~(a & (~aux0 & (a
		^ aux0) ^ (a ^ a | ~aux0) ^ ~((aux0 ^ aux0) & (a | a))))) &
		~a) + 64672 * (((~(aux0 | a) ^ (aux0 & a ^ (a | a) | ~(aux0
		| a)) ^ (~aux0 ^ a ^ a ^ aux0 & aux0 | ~a & aux0 ^ (~a | a ^
		aux0))) & (aux0 & (~aux0 | a & aux0) ^ (aux0 | a | a & aux0)
		& (aux0 | aux0 & a)) & ~(~(a & a) ^ a) | ~(aux0 ^ ~a | aux0)
		^ ~(aux0 | aux0) ^ a ^ ~a ^ (aux0 | (aux0 | aux0) ^ a ^
		aux0) ^ (a & ((a | a) ^ a) & ~(aux0 & aux0) | a)) ^ (aux0 ^
		~~(~(aux0 | aux0) | ~(a & aux0))) & a) + 22313 * (~(((~(aux0
		| a) | aux0 & a ^ a & aux0) ^ (aux0 ^ aux0 | a & a) ^ a &
		aux0 ^ aux0 ^ a ^ aux0 ^ aux0 & aux0 & a ^ aux0) & (a ^ aux0
		^ (aux0 | ((aux0 | a) ^ (aux0 | a)) & (~aux0 ^ ~aux0)))) ^
		(aux0 ^ ~~((aux0 | a) & (a ^ aux0)) ^ ~(aux0 & (~~aux0 | ~(a
		^ a)))) & (~((a | a) & ~aux0 & (~aux0 ^ (aux0 | aux0))) | (a
		& aux0 ^ (aux0 | aux0) | (aux0 ^ a) & (a | a)) ^ (aux0 ^ a ^
		a) & ((a | a) ^ (aux0 | aux0))) & ~(a & a)) + 31772 *
		~(~((aux0 ^ a) & (aux0 ^ aux0) & (a ^ a) & ~a | ~(a & a) &
		((aux0 | a) ^ ~aux0) | aux0) ^ ~aux0) + 65170 * (aux0 | ~(a
		& (~(a & (aux0 | aux0)) ^ (aux0 | a) ^ a ^ (a | a) ^ aux0 ^
		aux0) ^ ~~(aux0 & a | a & aux0) ^ aux0)) + 53526 * (~~((a &
		aux0 ^ a & a ^ ~a & (aux0 | aux0)) & (aux0 & a & a ^ ~(aux0
		| aux0)) & ~~a) | ~((a | aux0) & (aux0 ^ a) | a | ~(a & a &
		aux0 & aux0) | aux0 | aux0) | a & ~(~aux0 & a & aux0 | a &
		aux0 & ~aux0) | aux0 ^ aux0) + 60556 * ~(a & ~((aux0 ^ (a &
		a | aux0 | a) | a & (a ^ a) & (a | aux0 ^ a)) & (~~aux0 |
		aux0 | aux0 ^ aux0 & a | (aux0 ^ aux0) & ~aux0))) + 34287 *
		~(((a | (a ^ aux0 | a ^ aux0) ^ (aux0 | a)) & ~~(aux0 & a &
		(a ^ aux0)) ^ ~a) & aux0) + 38822 * ~~~(~aux0 ^ a & aux0 ^ a
		^ a ^ ~(a ^ a) | a) + 16099 * ~~~~((a | aux0) & ~a & (a |
		aux0 | a ^ aux0) | (a & a | a | a) & a) + 8696 * (~~(~aux0 |
		aux0) ^ a & ((aux0 ^ aux0) & ~aux0 ^ ~aux0) & ((~aux0 | a ^
		aux0) ^ (a | aux0)) ^ a & (a | a & a | aux0) & (~~aux0 |
		aux0 | aux0) & (a ^ (a ^ aux0) & aux0 & aux0 & (a | aux0) ^
		((aux0 | a) & a & a | a)) ^ (~aux0 ^ (a ^ (a | a) ^ ~aux0) &
		a) & ~aux0 & (~(aux0 ^ aux0 | a & aux0 & a) ^ aux0 & a &
		(aux0 & a ^ aux0 | ~a) | (~aux0 | ~aux0) & (aux0 & a | a) &
		((aux0 | a) & ~a ^ ~aux0 & (a | aux0)) ^ (aux0 & (aux0 ^ a ^
		a) | (aux0 ^ aux0 | a | aux0) ^ (aux0 & aux0 | ~a)))) + 5485
		* ((aux0 ^ a) & aux0 & aux0) + 5373 * (~aux0 | aux0 | (~~a &
		(aux0 | a ^ a ^ a) | aux0) & aux0) + 64490 * (aux0 &
		(~((aux0 | a) & (a ^ a) | aux0 & a & (aux0 ^ aux0) | aux0 ^
		a | aux0 | aux0 | ~aux0) ^ (~aux0 | a & ((aux0 | aux0) ^
		~aux0) & (~a ^ a & aux0))) & aux0) + 14883 * (~(aux0 ^
		((aux0 & a | aux0 ^ a) ^ a & a & a & a) & ((a | aux0) ^ aux0
		^ aux0) ^ aux0 ^ ~(aux0 & aux0) ^ aux0) & (aux0 | aux0 |
		aux0)) + 59689 * (~~a & (~((aux0 ^ aux0) & a | (aux0 | aux0)
		& ~a) | a & aux0 & (a ^ a) & (a ^ a)) ^ (a | a) ^ ~((aux0 ^
		aux0 ^ a & aux0 ^ aux0 | a | aux0) & ((~(aux0 & a) | a | a |
		a) & (~aux0 | aux0 ^ aux0 ^ aux0 ^ a) ^ a))) + 17554 *
		((~(~(a & a | ~a) & aux0) ^ a ^ ~(aux0 & (~(a | aux0) ^ a ^
		(aux0 | aux0)) & ~a)) & (aux0 | aux0 | (aux0 ^ (a | a & a &
		aux0 & aux0 & ~(a | a))) & ((aux0 | aux0 ^ a | ~a) & (aux0 ^
		~aux0 ^ ~a) | aux0 ^ a ^ (~a | ~aux0)))) + 62889 * ((~((a ^
		a | aux0 & a) & (~a ^ aux0)) ^ ~(a | aux0 & aux0 ^ a)) &
		~(~(~aux0 & aux0 & aux0) | aux0) ^ ((~(a | aux0) | a) &
		~~(aux0 ^ a) | ~(a ^ a ^ ~a) ^ aux0 & (aux0 | a | aux0 ^
		aux0)) & (a ^ ((a | aux0) ^ ~aux0) & (aux0 | aux0) & (a |
		aux0) & (~(aux0 ^ aux0) | a & a ^ aux0 ^ aux0)) | a | (a | a
		| aux0 ^ a | ~a & a) ^ (aux0 ^ aux0 | a ^ a) & a ^ (~(aux0 |
		aux0) | ~aux0 & (aux0 ^ a)) ^ (~a ^ a ^ a) & ~(aux0 & aux0)
		| a) + 40228 * (((aux0 ^ a | aux0 ^ a | ~aux0 | a & aux0 |
		aux0 | ~(aux0 & a) & (~aux0 | a ^ a) | (a & a ^ ~aux0) & ~(a
		& a)) ^ aux0 | aux0 ^ a ^ ~aux0 & (a | a & a | aux0) |
		~((aux0 | aux0) ^ aux0 ^ aux0 ^ a) | aux0) & ~a) + 11142 *
		(~~~~(aux0 ^ aux0 & aux0 ^ (aux0 | a)) ^ ~aux0 ^ aux0) +
		18693 * ~~~(aux0 ^ ~a) + 6263 * (a & ((aux0 ^ ~(~(a ^ a) ^
		a)) & aux0 & aux0 & (aux0 | aux0) & (a ^ aux0) & (aux0 |
		aux0) & (aux0 ^ aux0) & (aux0 | a) & (a | aux0) & aux0 &
		aux0 ^ (~(a ^ a ^ a) ^ aux0 ^ a & aux0 & ~aux0 | ~(aux0 ^
		aux0 ^ ~aux0) ^ (~(aux0 ^ a) | (aux0 | a) & a) | (a | a ^ a
		& aux0 | ~~(a & a)) & ~(~(aux0 & aux0) ^ (a ^ a | aux0)))))
		+ 33383 * (aux0 & ~(a | ~~(aux0 | a | aux0)) | ~((a | aux0)
		^ ~a | (aux0 ^ a | a & a) ^ (~a | a) | a) & (~~(~a & (a ^
		aux0)) & a & a & a | ~~(~(aux0 & a) & ~aux0))) + 9936 *
		(~aux0 & (~~(((aux0 | a) ^ aux0) & ~~a) ^ ~(~((aux0 | a) ^
		~a) ^ (aux0 ^ (a | a)) & (aux0 & aux0 ^ a))) & aux0) + 4498
		* (a | ~a) + 19577 * (((~a ^ aux0 ^ a ^ a ^ a ^ (a | aux0))
		& aux0 | a) & (a & a & aux0 & (a | a & a) ^ aux0 | a | aux0
		& (aux0 ^ a) | a) ^ ~((~aux0 | aux0 ^ aux0) & ~(aux0 ^ a) ^
		(~aux0 | aux0 | aux0) ^ (aux0 | a & a)) ^ ~a | aux0 & ((a &
		aux0 | aux0 & a) ^ (a ^ aux0 | a & a) ^ ~~(aux0 & a)) &
		((aux0 | aux0) & (a ^ aux0) | (aux0 | a) & a) & (~(aux0 |
		aux0) | (a | aux0) ^ ~aux0) & (~~a ^ (~~~(aux0 | aux0) | (a
		& aux0 ^ aux0 | ~(a ^ a)) ^ (aux0 & (a | aux0) | ~(a ^
		aux0))))) + 11946 * (~~(~(~a | aux0 ^ a) ^ (aux0 | a) & aux0
		& a ^ (aux0 ^ aux0 | aux0 & aux0)) & (aux0 & ~a & (~(a ^ a)
		^ (a ^ a) & a & a) ^ (~a & (a ^ a | a ^ aux0) | ~(a & a) ^
		aux0 ^ a ^ (a | aux0)) | aux0) | ~aux0) + 64127 * (aux0 ^
		((~~((a | a) & (aux0 ^ aux0)) | a | ~(~a | aux0 ^ a)) & a |
		~aux0)) + 40818 * ((a & ~(a & aux0 & aux0) ^ (~a | aux0 | a
		| aux0 | aux0) & aux0 | aux0) & ((a ^ aux0) & (a ^ a) ^
		(aux0 ^ a | ~a) ^ (a | a ^ aux0 ^ a & a) | a | a & (a & a &
		(aux0 ^ aux0) ^ (aux0 | a | a | aux0)) ^ aux0) & ~~(~(aux0 ^
		(a ^ a) & a) & (~~aux0 ^ ~aux0 ^ (aux0 | aux0)) & ~(a & a) &
		(aux0 & a ^ ~a))) + 36728 * (a & a | aux0 ^ ((~aux0 | aux0 ^
		aux0) & ~a & (~(a ^ aux0) ^ (aux0 | aux0) ^ aux0 ^ a) |
		((aux0 | aux0) ^ aux0 ^ ~a) & (aux0 | ~~a) | a & a | a)) +
		35085 * (((aux0 | (~aux0 ^ ~aux0) & a & ~(a | aux0) & (aux0
		| a & a)) ^ (a | aux0 | aux0 ^ aux0) ^ (a ^ a) & ~a ^ ~~aux0
		^ a & a & (~aux0 | a | a | a | a | a) ^ ~~(~a ^ a ^ ~a) ^
		(~a | ~(aux0 | aux0) ^ (aux0 & aux0 | a)) ^ aux0) &
		(~(~~(aux0 | aux0) ^ ~(a & a & ~a) ^ aux0 & (aux0 & a ^ a ^
		a) ^ a) | ~(~(~aux0 & aux0 | aux0 & (a | aux0)) ^ a ^ (a ^
		a) & ~a ^ (aux0 | ~(a | aux0))))) + 52868 * (((~aux0 ^
		~aux0) & ((aux0 | a) ^ ~aux0) & ((aux0 | a) ^ aux0 | a |
		aux0 | ~a) ^ ~(a | aux0 | ~aux0 | a | aux0 | a) ^ ~(~(aux0 |
		aux0) & aux0 & (aux0 | a) | ~~a ^ ~(aux0 | aux0)) ^ ((aux0 &
		aux0 | aux0 | aux0) ^ aux0) & a & ((a | aux0 | a | aux0 |
		aux0 | a & a) ^ (~a ^ (aux0 | a) | aux0)) ^ ~((~aux0 ^ aux0
		& a ^ a) & (aux0 ^ aux0) & (aux0 | a) & (aux0 & aux0 | a &
		aux0))) & ~a) + 10842 * (a & (a & ~(a | aux0) | (aux0 |
		aux0) ^ ~aux0 | aux0 ^ aux0 | a ^ (a & aux0 | aux0) & (a ^
		aux0 | aux0)) & ~~((a ^ aux0) & aux0 | ~aux0 ^ aux0 & a) &
		aux0) + 48438 * (aux0 & (~a | ~aux0 | ~aux0 & ~aux0) & (a |
		~a) & ~(aux0 | a) & ((a | a) ^ ~aux0 ^ ~(aux0 & a) ^ a) ^
		((a | aux0 | aux0) ^ ~aux0 | a ^ aux0 ^ ~aux0 ^ ~(a & aux0)
		| aux0) & (a | aux0 | aux0 | a | aux0 & aux0 | ~(a ^ aux0))
		| aux0 ^ aux0) + 44286 * ~~(~(~a | (a | aux0) ^ aux0) & (a ^
		(a | a) ^ (a ^ a) & (a | a)) & (a | (aux0 ^ aux0) & ~a) | (a
		& aux0 ^ a ^ aux0 | aux0 ^ (a | a)) & ~(aux0 ^ aux0 ^ aux0)
		| ~~(~a & (aux0 | aux0))) + 19150 * (aux0 | aux0 & (~(aux0 |
		aux0 | aux0) ^ (~(aux0 ^ aux0) | (aux0 | aux0) & (a ^ a)) ^
		a) & ~(a & (a | a) & ~a ^ (~aux0 ^ a ^ a) & a)) + 59472 *
		(aux0 | aux0) + 42152 * (~aux0 & ~((~aux0 | aux0 & ~a & (a |
		aux0)) & (a ^ (a | aux0)) & (aux0 | aux0 | aux0 | a) & (a |
		aux0 ^ (aux0 | a)) | aux0 ^ (a | aux0 | a | aux0) ^ a & ~a ^
		aux0 ^ (a ^ aux0 | aux0))) + 32020 * ((a ^ ~(aux0 & (a &
		aux0 ^ (a | aux0))) | aux0) ^ (aux0 ^ (aux0 | ~a) & aux0 &
		(aux0 ^ a) ^ a) & a ^ a) + 51542 * (aux0 ^ aux0 ^ a & ((~a &
		(a ^ a) | ~aux0) ^ ~~(aux0 ^ a) | a | aux0 ^ ~~a) & aux0) +
		64916 * ((~a | ~a) & (aux0 | ~(a ^ aux0)) & (a | (aux0 |
		~(aux0 | aux0)) ^ a | aux0 & ((a ^ a | a ^ aux0) ^ ~aux0)) ^
		aux0 & ~((~a ^ aux0 & aux0) & (a ^ a ^ ~aux0) | ~(~a | aux0
		& aux0)) & (aux0 ^ a & ~a & aux0 & aux0 ^ a) & aux0) + 13668
		* (a & (~((a & aux0 ^ aux0 ^ a) & (aux0 | a) & ~a ^ (a ^ a ^
		~aux0) & (a ^ a) & (a | aux0)) & (aux0 & aux0 ^ ~aux0 | a &
		(a ^ aux0) | aux0 | (~(a ^ a) ^ aux0 & a & ~aux0) & a & a) |
		~((aux0 | ~a ^ aux0 & a | (~aux0 | ~aux0) & (~aux0 ^ aux0 &
		a)) ^ a ^ aux0))) + 14502 * (a & (~(a & ~(a & aux0)) | ~(a |
		aux0) ^ ~(aux0 ^ a) | aux0) & a ^ aux0) + 47188 * ((~(aux0 &
		((aux0 ^ a) & (aux0 ^ aux0) ^ (a | a | a))) | a & (aux0 ^
		aux0) & (aux0 ^ (a | a & aux0 | a))) ^ (a | ~(a ^ a ^ a) |
		aux0 & aux0 & ~a & (~a | a)) & (aux0 | (~aux0 ^ ~aux0 | aux0
		& (a ^ aux0)) ^ ~~aux0 & (a ^ aux0) & ~a) | a & aux0) +
		28615 * (aux0 & (((a & aux0 ^ a & a) & (aux0 ^ (a | aux0)) ^
		(a ^ a) & ~aux0 ^ ~aux0 & (aux0 | aux0)) & (a ^ ~(a | a) &
		~(aux0 ^ a)) ^ (aux0 | aux0) & (a ^ a) & (aux0 ^ aux0) & (a
		| a) & ~~(a & aux0) ^ aux0) ^ ~~~(~aux0 | ~aux0 ^ aux0 &
		aux0 ^ aux0 & a)) + 49603 * (~((~~(aux0 & a) | a) ^ a) &
		((aux0 | a | ~a) & ((aux0 | aux0) ^ (aux0 | a)) | (aux0 | a)
		& (aux0 ^ a) | a) & (aux0 | ~~~aux0) & (~a & ~a & (aux0 |
		aux0) & aux0 & a ^ (~~a | ~aux0 | aux0 ^ aux0) | ~(aux0 & (a
		& a | ~a))) & ~((aux0 | ~~(a ^ ~a)) ^ ~((a | a) & aux0 & ((a
		| aux0) ^ (a | a))) ^ (aux0 | a & aux0 ^ aux0 ^ aux0) & (~a
		^ ~aux0) & ~a)) + 26263 * ((~~(~(a & a) | ~aux0 | a) | (aux0
		^ a ^ aux0 & a | aux0) & a | ~~a | ~~aux0 | a | ~(aux0 |
		aux0)) & a & (~~a & ~a & a & a & (~aux0 & (aux0 | aux0) &
		(aux0 ^ aux0 ^ a) ^ aux0) | aux0 ^ aux0) & (a ^ ~(aux0 & a ^
		a | ~(aux0 | aux0)) ^ a ^ a)) + 32717 * (a | a & ((~a | a &
		a | ~(a ^ a)) ^ ~a ^ (a | a) ^ ~(aux0 & a)) & ~(~a & ~a & a
		& a & ~aux0 & (~~aux0 | ~(aux0 | a))) ^ (~(~(aux0 ^ a) | a ^
		a | aux0 | a) | (aux0 & a ^ ~(aux0 & aux0)) & aux0 | (aux0 &
		~a & (aux0 ^ a) | ~aux0 & (aux0 | a) & (a | a | a ^ aux0)) ^
		~~a));
    vm->memory[--vm->sp] = result;
    // std::cout << "[DEBUG] NOT: ~0x" << std::hex << a << " = 0x" << result << std::dec << " [sp=" << vm->sp << "]" << std::endl;

    WORD next = vm->fetch_opcode();
    vm->handler_table[next](vm);
}

void VirtualMachine::op_shl(VirtualMachine* vm){
    WORD b = vm->memory[vm->sp++];
    WORD a = vm->memory[vm->sp++];
    WORD result = a << b;
    vm->memory[--vm->sp] = result;
    // std::cout << "[DEBUG] SHL: " << a << " << " << b << " = " << result << " [sp=" << vm->sp << "]" << std::endl;

    WORD next = vm->fetch_opcode();
    vm->handler_table[next](vm);
}

void VirtualMachine::op_shr(VirtualMachine* vm){
    WORD b = vm->memory[vm->sp++];
    WORD a = vm->memory[vm->sp++];
    WORD result = a >> b;
    vm->memory[--vm->sp] = result;
    // std::cout << "[DEBUG] SHR: " << a << " >> " << b << " = " << result << " [sp=" << vm->sp << "]" << std::endl;

    WORD next = vm->fetch_opcode();
    vm->handler_table[next](vm);
}

void VirtualMachine::op_jmp(VirtualMachine* vm){
    WORD target = vm->memory[vm->ip];
    // std::cout << "[DEBUG] JMP: from ip=" << vm->ip << " to " << target << std::endl;
    vm->ip = target;

    WORD next = vm->fetch_opcode();
    vm->handler_table[next](vm);
}

void VirtualMachine::op_cmpje(VirtualMachine* vm){
    WORD b = vm->memory[vm->sp++];
    WORD a = vm->memory[vm->sp++];
    // std::cout << "[DEBUG] CMPJE: comparing " << a << " == " << b << std::endl;
    if (a == b) {
        WORD target = vm->memory[vm->ip];
        // std::cout << "[DEBUG] CMPJE: condition true, jumping to " << target << std::endl;
        vm->ip = target;
    } else {
        // std::cout << "[DEBUG] CMPJE: condition false, continuing to next instruction" << std::endl;
        vm->ip++;
    }

    WORD next = vm->fetch_opcode();
    vm->handler_table[next](vm);
}

void VirtualMachine::op_cmpjne(VirtualMachine* vm){
    WORD b = vm->memory[vm->sp++];
    WORD a = vm->memory[vm->sp++];
    // std::cout << "[DEBUG] CMPJNE: comparing " << a << " != " << b << std::endl;
    if (a != b) {
        WORD target = vm->memory[vm->ip];
        // std::cout << "[DEBUG] CMPJNE: condition true, jumping to " << target << std::endl;
        vm->ip = target;
    } else {
        // std::cout << "[DEBUG] CMPJNE: condition false, continuing to next instruction" << std::endl;
        vm->ip++;
    }

    WORD next = vm->fetch_opcode();
    vm->handler_table[next](vm);
}

void VirtualMachine::op_cmpjg(VirtualMachine* vm){
    WORD b = vm->memory[vm->sp++];
    WORD a = vm->memory[vm->sp++];
    // std::cout << "[DEBUG] CMPJG: comparing " << a << " > " << b << std::endl;
    if (a > b) {
        WORD target = vm->memory[vm->ip];
        // std::cout << "[DEBUG] CMPJG: condition true, jumping to " << target << std::endl;
        vm->ip = target;
    } else {
        // std::cout << "[DEBUG] CMPJG: condition false, continuing to next instruction" << std::endl;
        vm->ip++;
    }

    WORD next = vm->fetch_opcode();
    vm->handler_table[next](vm);
}

void VirtualMachine::op_cmpjge(VirtualMachine* vm){
    WORD b = vm->memory[vm->sp++];
    WORD a = vm->memory[vm->sp++];
    // std::cout << "[DEBUG] CMPJGE: comparing " << a << " >= " << b << std::endl;
    if (a >= b) {
        WORD target = vm->memory[vm->ip];
        // std::cout << "[DEBUG] CMPJGE: condition true, jumping to " << target << std::endl;
        vm->ip = target;
    } else {
        // std::cout << "[DEBUG] CMPJGE: condition false, continuing to next instruction" << std::endl;
        vm->ip++;
    }

    WORD next = vm->fetch_opcode();
    vm->handler_table[next](vm);
}

void VirtualMachine::op_cmpjl(VirtualMachine* vm){
    WORD b = vm->memory[vm->sp++];
    WORD a = vm->memory[vm->sp++];
    // std::cout << "[DEBUG] CMPJL: comparing " << a << " < " << b << std::endl;
    if (a < b) {
        WORD target = vm->memory[vm->ip];
        // std::cout << "[DEBUG] CMPJL: condition true, jumping to " << target << std::endl;
        vm->ip = target;
    } else {
        // std::cout << "[DEBUG] CMPJL: condition false, continuing to next instruction" << std::endl;
        vm->ip++;
    }

    WORD next = vm->fetch_opcode();
    vm->handler_table[next](vm);
}

void VirtualMachine::op_cmpjle(VirtualMachine* vm){
    WORD b = vm->memory[vm->sp++];
    WORD a = vm->memory[vm->sp++];
    // std::cout << "[DEBUG] CMPJLE: comparing " << a << " <= " << b << std::endl;
    if (a <= b) {
        WORD target = vm->memory[vm->ip];
        // std::cout << "[DEBUG] CMPJLE: condition true, jumping to " << target << std::endl;
        vm->ip = target;
    } else {
        // std::cout << "[DEBUG] CMPJLE: condition false, continuing to next instruction" << std::endl;
        vm->ip++;
    }

    WORD next = vm->fetch_opcode();
    vm->handler_table[next](vm);
}

void VirtualMachine::op_dup(VirtualMachine* vm){
    WORD a = vm->memory[vm->sp];
    vm->memory[--vm->sp] = a;
    // std::cout << "[DEBUG] DUP: duplicated " << a << " to stack[sp=" << vm->sp << "]" << std::endl;

    WORD next = vm->fetch_opcode();
    vm->handler_table[next](vm);
}

void VirtualMachine::op_call(VirtualMachine* vm){
    WORD target = vm->memory[vm->ip++];
    vm->memory[--vm->sp] = vm->ip;
    // std::cout << "[DEBUG] CALL: saving return ip=" << vm->ip << " to stack[sp=" << vm->sp << "], jumping to " << target << std::endl;
    vm->ip = target;

    WORD next = vm->fetch_opcode();
    vm->handler_table[next](vm);
}

void VirtualMachine::op_ret(VirtualMachine* vm){
    WORD return_ip = vm->memory[vm->sp++];
    // std::cout << "[DEBUG] RET: returning to ip=" << return_ip << " from stack[sp=" << vm->sp-1 << "]" << std::endl;
    vm->ip = return_ip;

    WORD next = vm->fetch_opcode();
    vm->handler_table[next](vm);
}

void VirtualMachine::op_syscall(VirtualMachine* vm){
    // std::cout << "[DEBUG] SYSCALL: system call requested" << std::endl;
    WORD next = vm->fetch_opcode();
    vm->handler_table[next](vm);
}

const char* reg_name(int reg) {
    static const char* names[] = {"ax", "bx", "cx", "dx", "si", "di", "bp", "sp"};
    if (reg >= 0 && reg <= 7) return names[reg];
    return "unknown";
}

void VirtualMachine::op_mov(VirtualMachine* vm){
    WORD dest_type = vm->memory[vm->ip++];
    WORD src_type = vm->memory[vm->ip++];
            
    WORD dest_addr, src_addr;
    WORD value;
    
    // std::cout << "[DEBUG] MOV: dest_type=" << dest_type << ", src_type=" << src_type << std::endl;
            
    if (src_type == 0) {
        src_addr = vm->memory[vm->ip++];
        value = vm->memory[src_addr];
        // std::cout << "[DEBUG] MOV: src=mem[" << src_addr << "]=" << value << std::endl;
    } else if (src_type == 1) {
        src_addr = vm->memory[vm->ip++];
        switch(src_addr) {
            case 0: value = vm->ax; break;
            case 1: value = vm->bx; break;
            case 2: value = vm->cx; break;
            case 3: value = vm->dx; break;
            case 4: value = vm->si; break;
            case 5: value = vm->di; break;
            case 6: value = vm->bp; break;
            case 7: value = vm->sp; break;
            default: value = 0;
        }
        // std::cout << "[DEBUG] MOV: src=reg[" << reg_name(src_addr) << "]=" << value << std::endl;
    } else {
        value = vm->memory[vm->ip++];
        // std::cout << "[DEBUG] MOV: src=imm=" << value << std::endl;
    }

    if (dest_type == 0) {
        dest_addr = vm->memory[vm->ip++];
        vm->memory[dest_addr] = value;
        // std::cout << "[DEBUG] MOV: dest=mem[" << dest_addr << "]=" << value << std::endl;
    } else {
        dest_addr = vm->memory[vm->ip++];
        switch(dest_addr) {
            case 0: vm->ax = value; break;
            case 1: vm->bx = value; break;
            case 2: vm->cx = value; break;
            case 3: vm->dx = value; break;
            case 4: vm->si = value; break;
            case 5: vm->di = value; break;
            case 6: vm->bp = value; break;
            case 7: vm->sp = value; break;
        }
        // std::cout << "[DEBUG] MOV: dest=reg[" << reg_name(dest_addr) << "]=" << value << std::endl;
    }
    
    WORD next = vm->fetch_opcode();
    vm->handler_table[next](vm);
}

void VirtualMachine::op_mod(VirtualMachine* vm){
    WORD b = vm->memory[vm->sp++];
    WORD a = vm->memory[vm->sp++];
    if (b == 0) {
        // std::cout << "[ERROR] MOD: Modulo by zero at ip=" << vm->ip-1 << std::endl;
        vm->running = false;
        return;
    }
    WORD result = a % b;
    vm->memory[--vm->sp] = result;
    // std::cout << "[DEBUG] MOD: " << a << " % " << b << " = " << result << " [sp=" << vm->sp << "]" << std::endl;
    
    WORD next = vm->fetch_opcode();
    vm->handler_table[next](vm);
}

void VirtualMachine::op_loadind(VirtualMachine* vm){
    WORD addr = vm->memory[vm->sp++];
    WORD value = vm->memory[addr];
    vm->memory[--vm->sp] = value;
    // std::cout << "[DEBUG] LOADIND: load from mem[" << addr << "]=" << value << " to stack[sp=" << vm->sp << "]" << std::endl;

    WORD next = vm->fetch_opcode();
    vm->handler_table[next](vm);
}

void VirtualMachine::op_storeind(VirtualMachine* vm){
    WORD addr = vm->memory[vm->sp++];
    WORD value = vm->memory[vm->sp++];
    vm->memory[addr] = value;
    // std::cout << "[DEBUG] STOREIND: store " << value << " to mem[" << addr << "]" << std::endl;
    
    WORD next = vm->fetch_opcode();
    vm->handler_table[next](vm);
}

void VirtualMachine::op_nop(VirtualMachine* vm){
    // std::cout << "[DEBUG] NOP: no operation" << std::endl;
    WORD next = vm->fetch_opcode();
    vm->handler_table[next](vm);
}

void VirtualMachine::op_swap(VirtualMachine* vm){
    if (vm->sp < MAX_MEMORY - 1) {
        WORD top = vm->memory[vm->sp];
        WORD next_val = vm->memory[vm->sp + 1];
        vm->memory[vm->sp] = next_val;
        vm->memory[vm->sp + 1] = top;
        // std::cout << "[DEBUG] SWAP: swapped stack[" << vm->sp << "]=" << top << " with stack[" << vm->sp+1 << "]=" << next_val << std::endl;
    } else {
        // std::cout << "[ERROR] SWAP: stack underflow!\n";
        vm->running = false;
        return;
    }
    WORD next = vm->fetch_opcode();
    vm->handler_table[next](vm);
}

void VirtualMachine::op_print(VirtualMachine* vm)
{
    WORD mode = vm->memory[vm->ip++];

    if (mode == 0)
    {
        // число
        WORD value = vm->memory[vm->sp++];

        // std::cout << "[DEBUG] PRINT_NUM: " << value << std::endl;
        std::cout << value;
    }
    else if (mode == 1)
    {
        // строка
        WORD addr = vm->memory[vm->sp++];

        // std::cout << "[DEBUG] PRINT_STR: addr=" << addr << std::endl;

        while (true)
        {
            WORD c = vm->memory[addr++];

            if (c == 0)
                break;

            std::cout << (char)(c & 0xFF);
        }
    }

    WORD next = vm->fetch_opcode();
    vm->handler_table[next](vm);
}

void VirtualMachine::op_scan(VirtualMachine* vm)
{
    WORD mode = vm->memory[vm->ip++];

    if (mode == 0)
    {
        // число
        WORD value;
        std::cin >> value;

        vm->memory[--vm->sp] = value;

        // std::cout << "[DEBUG] SCAN_NUM: " << value << std::endl;
    }
    else if (mode == 1)
    {
        // строка
        WORD addr = vm->memory[vm->sp++];

        std::string input;
        std::cin >> input;

        // std::cout << "[DEBUG] SCAN_STR: writing to " << addr << std::endl;

        for (size_t i = 0; i < input.size(); i++)
        {
            vm->memory[addr + i] = (WORD)input[i];
        }

        vm->memory[addr + input.size()] = 0; // null-terminate
    }

    WORD next = vm->fetch_opcode();
    vm->handler_table[next](vm);
}