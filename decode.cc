#include <iostream>
#include <deque>
#include "sim_ds.h"

extern std::deque<bundle> DE_register;
extern std::deque<bundle> RN_register;
extern u_int WIDTH;
extern u_int DE_time;
extern u_int cycles;
using namespace std;
void decode(){
    for(deque<bundle>::iterator j=DE_register.begin(); j!=DE_register.end(); j++) {
                // set first cycle and duration of this instruction.
        if(DE_register.size() && (j->decode_first == -1)) {
            j->decode_first = cycles;
        }
    }
    for(u_int i=0; i<WIDTH; i++) {



        // If the rename register is full, do nothing.
        if(RN_register.size() == WIDTH) {
            return;
        }
        // Otherwise go ahead and move a bundle to the rename register.
        else if(DE_register.size()) {
            DE_register.front().decode_time = cycles - DE_register.front().decode_first+1;
            RN_register.push_back(DE_register.front());
            DE_register.pop_front();
        }
    }
    return;
}
