#include <vector>
#include <iostream>
#include "sim_ds.h"

extern u_int IQ_SIZE;
extern std::vector<iq_entry> IQ;
extern std::vector<ex_entry> execute_list;
extern u_int WIDTH;
extern u_int IS_time;
extern u_int cycles;

using namespace std;
void issue(){

    for(u_int i=0; i<WIDTH; i++) {
        int min=50000;
        int min_index=-1;
        // stall if there isn't enough room to issue something.
        if(execute_list.size() == WIDTH*5) return;

        // Move through the IQ and locate the oldest instruction.
        for(u_int j=0; j<IQ_SIZE; j++) {
            if(!IQ.at(j).valid) continue;
            // skip this if its sources aren't ready.
            if(IQ.at(j).instruction.rn_src_1 != -1) continue;
            if(IQ.at(j).instruction.rn_src_2 != -1) continue;
            if(IQ.at(j).valid && (IQ.at(j).instruction.fetch_first < min) && ready(j)) {
                min = IQ.at(j).instruction.fetch_first;
                min_index = j;
            }
        }

        // oh, but first, age up all of the instructions in the IQ.
        for(u_int j=0; j<IQ_SIZE; j++) {
            if(!IQ.at(j).valid) continue;
            if(IQ.at(j).valid) {
                if(IQ.at(j).instruction.issue_first == -1) {
                    IQ.at(j).instruction.issue_first = cycles;
                }
            }
        }

        // If there wasn't an oldest valid instruction, leave.
        if(min_index == -1) return;

        // add the instruction to the execute list.
        int latency;
        if(IQ.at(min_index).instruction.op_type == 0) latency = 1;
        if(IQ.at(min_index).instruction.op_type == 1) latency = 2;
        if(IQ.at(min_index).instruction.op_type == 2) latency = 5;
        IQ.at(min_index).instruction.issue_time = cycles - IQ.at(min_index).instruction.issue_first+1;
        ex_entry ex_instruction = {IQ.at(min_index).instruction, latency};
        execute_list.push_back(ex_instruction);
        // invalidate that instruction in the IQ.
        IQ.at(min_index).valid=0;
    }
    return;
}

bool ready(u_int index) {
    bool status = true;
    if(IQ.at(index).instruction.rn_src_1 != -1) status = false;
    if(IQ.at(index).instruction.rn_src_2 != -1) status = false;
    return status;
}
