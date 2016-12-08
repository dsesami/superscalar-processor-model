#include <vector>
#include <deque>
#include <iostream>
#include "sim_ds.h"

extern std::vector<ex_entry> execute_list;
extern std::deque<bundle> WB_register;
extern std::deque<bundle> RR_register;
extern std::deque<bundle> DI_register;
extern std::vector<iq_entry> IQ;
extern u_int WIDTH;
extern u_int cycles;
using namespace std;

void execute(){

    //execute_status();
    // If there are no instructions, do nothing.
    if(!execute_list.size()) return;

    // Decrease the age of all the instructions in the execute list.
    for(u_int i=0;i < execute_list.size(); i++) {

        // AGE PROCESSING INFORMATION...
        if(execute_list.at(i).instruction.execute_first == -1) {
            execute_list.at(i).instruction.execute_first = cycles;
        }

        // decrease execution counter.
        execute_list.at(i).age--;
    }
    while(execute_needed()){
        for(u_int i=0;i < execute_list.size(); i++) {
            // If it is finishing execution this cycle...
            if(!execute_list.at(i).age) {
                // Wake up IQ by matching ROB values and setting it to ready.
                for(u_int j=0; j<IQ_SIZE; j++) {
                    if((IQ.at(j).instruction.rn_src_1 ==
                                execute_list.at(i).instruction.rn_dest) && (IQ.at(j).instruction.rn_src_1 != -1)) {
                        IQ.at(j).instruction.rn_src_1 = -1;
                    }
                    if((IQ.at(j).instruction.rn_src_2 ==
                                execute_list.at(i).instruction.rn_dest) && (IQ.at(j).instruction.rn_src_2 != -1)) {
                        IQ.at(j).instruction.rn_src_2 = -1;
                    }
                }
                // Wake up DI
                for(deque<bundle>::iterator j=DI_register.begin(); j!=DI_register.end(); j++) {
                    if(!DI_register.size()) break;
                    if(j->rn_src_1 == execute_list.at(i).instruction.rn_dest) {
                        j->rn_src_1 = -1;
                    }
                    if(j->rn_src_2 == execute_list.at(i).instruction.rn_dest) {
                        j->rn_src_2 = -1;
                    }
                }

                // Wake up RR
                for(deque<bundle>::iterator j=DI_register.begin(); j!=DI_register.end(); j++) {
                    if(!RR_register.size()) break;
                    if(j->rn_src_1 == execute_list.at(i).instruction.rn_dest) {
                        j->rn_src_1 = -1;
                    }
                    if(j->rn_src_2 == execute_list.at(i).instruction.rn_dest) {
                        j->rn_src_2 = -1;
                    }
                }

                if(WB_register.size() == WIDTH*5) {
                    return;
                }
                // Add to WB pipeline register.
                execute_list.at(i).instruction.execute_time = cycles - execute_list.at(i).instruction.execute_first+1;
                execute_list.at(i).instruction.valid = true;
                bundle instruction = execute_list.at(i).instruction;
                WB_register.push_back(instruction);

                // Remove from execution list.
                execute_list.erase(execute_list.begin()+i);
            }
        }
    }
    return;
}

bool execute_needed() {
    bool yes = false;
    for(u_int i=0; i<execute_list.size(); i++) {
        if(!execute_list.at(i).age) yes = true;
    }
    return yes;
}

void execute_status() {
    cout << "execute_list:" << endl;
    for(u_int i=0; i<execute_list.size(); i++) {
        cout << i << " | instruction: " << execute_list.at(i).instruction.address << " | time: " << execute_list.at(i).age << endl;
    }
}
