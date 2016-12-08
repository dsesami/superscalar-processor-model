#include <deque>
#include <vector>
#include <iostream>
#include "sim_ds.h"
#include <cstdlib>
extern std::deque<bundle> RN_register;
extern std::deque<bundle> RR_register;
extern std::vector<rob_entry> ROB;
extern u_int head, tail;
extern std::vector<rmt_entry> RMT;
extern u_int WIDTH;
extern u_int RN_time;
extern u_int cycles;
using namespace std;

void rename(){
    for(deque<bundle>::iterator j=RN_register.begin(); j!=RN_register.end(); j++) {

        // set first cycle and duration of this instruction.
        if(RN_register.size() && (j->rename_first == -1)) {
            j->rename_first = cycles;
        }
    }
    if((RR_register.size() == WIDTH) || isrobfull()) {
                return;
            }

    for(u_int i=0; i<WIDTH; i++) {
        if(RN_register.size()) {
            // if the RR register cannot accept things, or the rob does not have enough entries, stall.


            // map the instruction to the ROB.
            ROB.at(tail).valid = true;
            ROB.at(tail).address = RN_register.front().address;
            ROB.at(tail).dyn = RN_register.front().dyn;
            ROB.at(tail).ready = false;
            ROB.at(tail).instruction = RN_register.front();
            // rename the source registers if applicable.
            if(RN_register.front().src_1 != -1) {
                if(RMT.at(RN_register.front().src_1).valid)
                    RN_register.front().rn_src_1 = RMT.at(RN_register.front().src_1).rob_tag;
            }

            // rename source register two similarly.
            if(RN_register.front().src_2 != -1) {
                if(RMT.at(RN_register.front().src_2).valid)
                    RN_register.front().rn_src_2 = RMT.at(RN_register.front().src_2).rob_tag;
            }

            // map the destination register to the ROB and RMT and rename the destination register.
            ROB.at(tail).dest = RN_register.front().dest;
            if(RN_register.front().dest != -1) {
                RMT.at(RN_register.front().dest).valid = true;
                RMT.at(RN_register.front().dest).rob_tag = tail;
            }
            RN_register.front().rn_dest = tail;

            // move the tail down for the next instruction.
            tail++;
            if(tail == ROBSIZE) tail=0;

            // finally, send the instruction on its merry way.
            RN_register.front().rename_time = cycles - RN_register.front().rename_first+1;
            RR_register.push_back(RN_register.front());
            RN_register.pop_front();
        }
    }
    return;
}

bool isrobfull() {
    u_int empty_slots=ROBSIZE;
    for(u_int i=0; i<ROB.size(); i++) {
        if(ROB.at(i).valid) empty_slots--;
    }
    return (empty_slots < WIDTH) ? true : false;
}
