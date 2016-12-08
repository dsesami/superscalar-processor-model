#include <vector>
#include "sim_ds.h"
#include <iostream>

extern std::vector<rob_entry> ROB;
extern std::vector<rmt_entry> RMT;
extern u_int cycles;
extern u_int dyn_instr_count;

extern u_int head, tail;

using namespace std;

void retire(){

    if(isROBempty()) return; // Nothing within ROB.

   for(u_int i=0; i<ROB.size(); i++) {
        // set first cycle and duration of this instruction.
        if(ROB.at(i).ready && (ROB.at(i).instruction.retire_first == -1)) {
            ROB.at(i).instruction.retire_first = cycles;
        }
   }
for(u_int i=0; i<WIDTH; i++) {
   if(ROB.at(head).ready && ROB.at(head).valid) {
       // Print information

       ROB.at(head).instruction.retire_time = cycles - ROB.at(head).instruction.retire_first+1;
        cout << ROB.at(head).instruction.dyn << " "
             << "fu{" << ROB.at(head).instruction.op_type << "} "
             << "src{" << ROB.at(head).instruction.src_1 << ","
             << ROB.at(head).instruction.src_2 << "} "
             << "dst{" << ROB.at(head).instruction.dest << "} "
             << "FE{" << ROB.at(head).instruction.fetch_first << ","
             << ROB.at(head).instruction.fetch_time << "} "
             << "DE{" << ROB.at(head).instruction.decode_first << ","
             << ROB.at(head).instruction.decode_time << "} "
             << "RN{" << ROB.at(head).instruction.rename_first << ","
             << ROB.at(head).instruction.rename_time << "} "
             << "RR{" << ROB.at(head).instruction.regRead_first << ","
             << ROB.at(head).instruction.regRead_time << "} "
             << "DI{" << ROB.at(head).instruction.dispatch_first << ","
             << ROB.at(head).instruction.dispatch_time << "} "
             << "IS{" << ROB.at(head).instruction.issue_first << ","
             << ROB.at(head).instruction.issue_time << "} "
             << "EX{" << ROB.at(head).instruction.execute_first << ","
             << ROB.at(head).instruction.execute_time << "} "
             << "WB{" << ROB.at(head).instruction.writeback_first << ","
             << ROB.at(head).instruction.writeback_time << "} "
             << "RT{" << ROB.at(head).instruction.retire_first << ","
             << ROB.at(head).instruction.retire_time << "}  "
             << endl;

        //dyn_instr_count++;
        // invalidate RMT
        if(ROB.at(head).instruction.dest != -1) {
            if(ROB.at(head).instruction.rn_dest == RMT.at(ROB.at(head).instruction.dest).rob_tag) {
                RMT.at(ROB.at(head).instruction.dest).valid = false;
                RMT.at(ROB.at(head).instruction.dest).rob_tag = -1;
            }
        }
        // Move head down and stop.
        //ROB.at(head).ready = 0;
        ROB.at(head).valid = 0;
        ROB.at(head).instruction.address = 0;
        //ROB.at(head).address = 0;
        //ROB.at(head).dest = -1;

        head++;
        if(head == ROBSIZE) head = 0;
        //dyn_instr_count++;
   }
}
    return;
}
