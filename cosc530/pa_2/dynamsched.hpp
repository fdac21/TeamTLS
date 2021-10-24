#include <fstream>
#include <sstream>
#include <string>
#include <vector>

enum FLAGS {
    lw = 0x001,
    flw = 0x002,
    sw = 0x004,
    fsw = 0x008,
    DT = lw | flw | sw | fsw,
    add = 0x010,
    sub = 0x020,
    ARITH = add | sub,
    beq = 0x040,
    bne = 0x080,
    CTRL = beq | bne,
    fadd_s = 0x100,
    fsub_s = 0x200,
    fmul_s = 0x400,
    fdiv_s = 0x800,
    FP = fadd_s | fsub_s | fmul_s | fdiv_s,
};

class ResStation {
    int instruction;

   public:
    ResStation();
};

class DynamSched {
   private:
    // Reservation Stations
    std::vector<ResStation> effAddr;
    std::vector<ResStation> fpAdds;
    std::vector<ResStation> fpMuls;
    std::vector<ResStation> Ints;
    std::vector<ResStation> reorder;

   public:
    DynamSched(std::string configFile);
    void allocateBuffer(std::fstream& conf, std::vector<ResStation>& buffer);
};

class Latency {
   public:
    int fp_add;
    int fp_sub;
    int fp_mul;
    int fp_div;

    Latency(int fp_add, int fp_sub, int fp_mul, int fp_div);
};
