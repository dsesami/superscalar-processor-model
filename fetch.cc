#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "sim_ds.h"

#define QUIT 0
#define CONTINUE 1

using namespace std;

extern ifstream infile;
extern u_int CACHE_SIZE;
extern u_int cycles;
extern deque<bundle> DE_register;

u_int fetch() {
    static u_int stall = 0;
    string line;                    // Current line
    string raw_address;             // PC (hexadecimal string)
    u_ll address;                   // PC (unsigned long long)
    int   op_type,                  // Latency: 0=1 cycle, 1=2 cycles, 2=5 cycles
          dest,                     // Destination register: -1=none
          src_1,                    // Source register 1: -1=none
          src_2;                    // Source register 2: -1=none
    for(u_int i=0; i<WIDTH; i++) {
        // If we are assuming a perfect cache...
        if(!CACHE_SIZE) {
            // Stall if the decode register is full.
            if(DE_register.size() == WIDTH) {
                stall++;
                return CONTINUE;
            }
            // Grab an instruction. If there are no more instructions, then quit.
            getline(infile, line);
            istringstream iss(line);
            if(!(iss >> raw_address >> op_type >> dest >> src_1 >> src_2))
                return QUIT;
        }

        // Otherwise process information and send it to the DE_register.
        // sscanf can read hex of format 0xaaaa, so we go ahead and create this...
        raw_address = "0x" + raw_address;
        sscanf(raw_address.c_str(), "%016lx", &address);

        // Add this instruction to the decode register.
        bundle instruction;
        instruction.address = address;
        instruction.op_type = op_type;
        instruction.dest    = dest;
        instruction.rn_dest = -1;
        instruction.src_1   = src_1;
        instruction.rn_src_1 = -1;
        instruction.src_2   = src_2;
        instruction.rn_src_2 = -1;
        instruction.valid = false;
        instruction.fetch_first = cycles;
        instruction.fetch_time = cycles - instruction.fetch_first+1;
        instruction.decode_first = -1;
        instruction.rename_first = -1;
        instruction.regRead_first = -1;
        instruction.dispatch_first = -1;
        instruction.issue_first = -1;
        instruction.execute_first = -1;
        instruction.writeback_first = -1;
        instruction.retire_first = -1;
        instruction.dyn = dyn_instr_count;
        dyn_instr_count++;
        DE_register.push_back(instruction);
    }
    stall=0;
    return CONTINUE;
}
