#include <deque>
#include <vector>
#include "sim_ds.h"
#include <iostream>

extern std::deque<bundle> WB_register;
extern std::vector<rob_entry> ROB;
extern u_int ROBSIZE;

using namespace std;

void writeback(){
    while(!WB_register.empty()) {
        // Process the age of the instruction.
        for(deque<bundle>::iterator it = WB_register.begin(); it != WB_register.end(); it++) {
            if(WB_register.size() && WB_register.front().writeback_first == -1)
                WB_register.front().writeback_first = cycles;
        }

        // Mark the instruction as ready within the ROB.
        for(u_int j=0; j < ROBSIZE; j++) {
            if(ROB.at(j).valid) {
                if (ROB.at(j).instruction.dyn == WB_register.front().dyn) {
                    ROB.at(j).ready = true;
                    for(deque<bundle>::iterator it = WB_register.begin();it!=WB_register.end();it++) {
                        if(WB_register.size() && WB_register.front().writeback_first == -1)
                            WB_register.front().writeback_first = cycles;
                    }
                    WB_register.front().writeback_time = cycles - WB_register.front().writeback_first+1;
                    ROB.at(j).instruction = WB_register.front();
                    WB_register.pop_front();
                    if(WB_register.size() == 0) break;
                }
            }
        }
    }
    return;
}
