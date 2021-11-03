#include <deque>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

enum INST {
    LW = 0x001,
    FLW = 0x002,
    SW = 0x004,
    FSW = 0x008,
    DT = LW | FLW | SW | FSW,
    ADD = 0x010,
    SUB = 0x020,
    ARITH = ADD | SUB,
    BEQ = 0x040,
    BNE = 0x080,
    CTRL = BEQ | BNE,
    FADD_S = 0x100,
    FSUB_S = 0x200,
    FMUL_S = 0x400,
    FDIV_S = 0x800,
    INT = BEQ | BNE | ADD | SUB | LW | SW,
    FPA = FADD_S | FSUB_S,
    FPM = FMUL_S | FDIV_S,
    FP = FADD_S | FSUB_S | FMUL_S | FDIV_S | FLW | FSW,
};

enum HAZARD {
    STRUCT,
    DATA,
    NONE,
};

struct Latency {
    int fp_add;
    int fp_sub;
    int fp_mul;
    int fp_div;
};

struct Instruction {
    INST opcode;
    int requiredCycles;
    int cycles;
    int startEx;
    int endEx;
    int written;
    int commited;
    std::vector<int> regs;
};

struct ResStation {
    Instruction inst;
    int val1 = -1, val2 = -1;
    int immediate = -1;
    uint64_t address = 0;
    bool busy = false;
    ResStation *rs1 = NULL, *rs2 = NULL;
};

ResStation createRS(Instruction inst);

struct Entry {
    INST inst;
    // int reg;
    ResStation *rs = NULL;
    bool ready = false;
};

Entry createEntry(Instruction inst, ResStation &rs);

struct ResStationBuffer {
    std::deque<ResStation> buffer;
    int size;
    // TODO: Check immediate value parsing and handling
    HAZARD checkBuffer();
    void freeRS();
    void insert(ResStation rs);
};

// TODO: Finish out register struct
struct Register {
    ResStation *RS = NULL;
};

struct ReorderBuffer {
    int size;
    std::deque<Entry> buffer;

    void insert(Instruction inst, ResStationBuffer &buffer);
    void commitEntry();
};

class DynamSched {
   private:
    Latency latencies;
    // Reservation Stations
    ResStationBuffer effAddr;
    ResStationBuffer fpAdds;
    ResStationBuffer fpMuls;
    ResStationBuffer arithmetics;
    ReorderBuffer reorder;

    Entry *CommonDataBus;

    std::vector<Register> fps;
    std::vector<Register> ints;

   public:
    DynamSched(std::string configFile);
    void allocateBuffer(std::fstream &conf, ResStationBuffer &buffer);
    void allocateReorderBuffer(std::fstream &conf);
    void setLatencies(std::fstream &conf);
    void allocateRegisters(std::vector<Register> &registers);
    void process();
    bool issue(std::string op, std::string regs);
    void execute();
    void checkCDB(ResStationBuffer &buffer);
    void write();
    void commit();
};

INST strToInst(std::string inst);

void parseRegisters(INST inst, std::string regs);

template <typename T>
HAZARD checkRSBuffer(std::deque<T> &buffer, int size);

template <typename T>
HAZARD checkReorderBuffer(std::deque<T> &buffer, int size);