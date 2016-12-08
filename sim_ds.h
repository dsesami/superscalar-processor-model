#ifndef __SUPERSCALAR_PIPELINE__
#define __SUPERSCALAR_PIPELINE__

typedef unsigned int u_int;
typedef unsigned long u_ll;

extern u_int ROBSIZE, IQ_SIZE, WIDTH, CACHE_SIZE, P, dyn_instr_count, cycles;
extern char* TRACE_FILE;
void retire();
void writeback();
void execute();
void issue();
void dispatch();
void regRead();
void rename();
void decode();
u_int fetch();
bool advance_cycle();
bool isrobfull();
bool isiqfull();
bool isROBempty();
bool isIQempty();
bool ready(u_int index);
void status();
void robstatus();
void execute_status();
bool execute_needed();

typedef struct bundle {
    u_ll address;
    int op_type;
    int dest;
    int rn_dest;
    int src_1;
    int rn_src_1;
    int src_2;
    int rn_src_2;
    int   fetch_first, fetch_time,
            decode_first, decode_time,
            rename_first, rename_time,
            regRead_first, regRead_time,
            dispatch_first, dispatch_time,
            issue_first, issue_time,
            execute_first, execute_time,
            writeback_first, writeback_time,
            retire_first, retire_time;
    bool valid;
    u_int dyn;
} bundle;

typedef struct rob_entry {
    rob_entry() : valid(false) {}
    bool valid;
    u_int dyn;
    bool ready;
    u_ll address;
    int dest;
    bundle instruction;
} rob_entry;

typedef struct rmt_entry {
    rmt_entry() : valid(false), rob_tag(0) {}
    bool valid;
    int rob_tag;
} rmt_entry;

typedef struct iq_entry {
    iq_entry() : valid(false) {}
    bool valid;
    bundle instruction;
} iq_entry;

typedef struct ex_entry {
    bundle instruction;
    int age;
} ex_entry;

#endif
