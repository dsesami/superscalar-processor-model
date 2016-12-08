#include <deque>
#include <vector>
#include <iostream>
#include "sim_ds.h"

extern std::deque<bundle> RR_register;
extern std::deque<bundle> DI_register;
extern std::vector<rob_entry> ROB;
extern u_int head, tail;
extern std::vector<rmt_entry> RMT;
extern u_int WIDTH;
extern u_int RR_time;
extern u_int cycles;
using namespace std;

void regRead(){
    for(u_int i=0; i<WIDTH; i++) {

        // set first cycle and duration of this instruction.
        for(deque<bundle>::iterator it = RR_register.begin(); it != RR_register.end(); it++) {
            if(RR_register.size() && it->regRead_first == -1)
                it->regRead_first = cycles;
        }

        // If the dispatch register is full, do nothing.
        if(DI_register.size() == WIDTH) {
            return;
        }
        // Otherwise check if values in the rob are ready, and if so, and move things forward.
        else if(RR_register.size()) {
            // Wake appropriate dependents! Since we do not have actual register values,
            // simply indicate that we have "ready" sources by indicating that they are not renamed.
            // Wake source 1:
            if(RR_register.front().rn_src_1 != -1) {
                if(ROB.at(RR_register.front().rn_src_1).ready) {
                    RR_register.front().rn_src_1 = -1;
                }
            }
            // Wake source 2:
            if(RR_register.front().rn_src_2 != -1) {
                if(ROB.at(RR_register.front().rn_src_2).ready) {
                    RR_register.front().rn_src_2 = -1;
                }
            }
        for(deque<bundle>::iterator it = RR_register.begin(); it != RR_register.end(); it++) {
            if(RR_register.size() && it->regRead_first == -1)
                it->regRead_first = cycles;
        }

            // Finally, move this bundle to the DI register.
            RR_register.front().regRead_time = cycles - RR_register.front().regRead_first+1;
            DI_register.push_back(RR_register.front());
            RR_register.pop_front();
        }
    }
    return;
}
