#include <deque>
#include <vector>
#include "sim_ds.h"

extern std::deque<bundle> DI_register;
extern std::vector<rob_entry> ROB;
extern u_int IQ_SIZE;
extern std::vector<iq_entry> IQ;
extern u_int cycles;

extern u_int WIDTH;
extern u_int DI_time;

using namespace std;
void dispatch(){


    // set first cycle and duration of this instruction.
        for(deque<bundle>::iterator it = DI_register.begin(); it != DI_register.end(); it++) {
            if(DI_register.size() && it->dispatch_first == -1)
                it->dispatch_first = cycles;
        }

    // If there isn't enough room in the IQ for the entire bundle set, do nothing.
    if(isiqfull()) {
        return;
    }
    // Otherwise move everything to the IQ.
    else if(DI_register.size()) {
        while(DI_register.size()) {
            for(u_int j=0; j<IQ_SIZE; j++) {
                if (IQ.at(j).valid) continue;

                // wake source 1:
                if(DI_register.front().rn_src_1 != -1) {
                    if(ROB.at(DI_register.front().rn_src_1).ready) {
                        DI_register.front().rn_src_1 = -1;
                    }
                }
                // Wake source 2:
                if(DI_register.front().rn_src_2 != -1) {
                    if(ROB.at(DI_register.front().rn_src_2).ready) {
                        DI_register.front().rn_src_2 = -1;
                    }
                }

        for(deque<bundle>::iterator it = DI_register.begin(); it != DI_register.end(); it++) {
            if(DI_register.size() && it->dispatch_first == -1)
                it->dispatch_first = cycles;
        }

                DI_register.front().dispatch_time = cycles - DI_register.front().dispatch_first+1;
                IQ.at(j).valid = true;
                IQ.at(j).instruction = DI_register.front();
                break;
            }
            if(!DI_register.size()) break;
            else DI_register.pop_front();
        }
    }

    return;
}

bool isiqfull() {
    u_int empty_slots=IQ_SIZE;
    for(u_int i=0; i<IQ_SIZE; i++) {
        if(IQ.at(i).valid) empty_slots--;
    }
    return (empty_slots < WIDTH) ? true : false;
}
