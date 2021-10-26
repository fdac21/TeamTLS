#include <deque>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

enum INST {
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
  INT = beq | bne | add | sub | lw | sw,
  FP = fadd_s | fsub_s | fmul_s | fdiv_s | flw | fsw,
};

struct Latency {
  int fp_add;
  int fp_sub;
  int fp_mul;
  int fp_div;
};

struct Instruction {
  uint32_t inst;
};

struct Entry {
  INST type;
  int reg;
  bool ready;
};

class ResStation {
  Instruction *instruction;
  int val1, val2;
  int immediate;
  bool busy;
  ResStation *RS1, *RS2;

 public:
  ResStation();
};

// TODO: Finish out register struct
struct Register {
  ResStation *RS = NULL;
};

class ReorderBuffer {
  int size;
  std::deque<Entry> buffer;

 public:
  ReorderBuffer(int size);
  void insertEntry(Entry e);
  void commitEntry();
}

class DynamSched {
 private:
  Latency latencies;
  // Reservation Stations
  std::vector<ResStation> effAddr;
  std::vector<ResStation> fpAdds;
  std::vector<ResStation> fpMuls;
  std::vector<ResStation> arithmetics;
  std::deque<Entry> reorder;

  std::vector<ResStation *> CommonDataBus;

  std::vector<Register> fps;
  std::vector<Register> ints;

 public:
  DynamSched(std::string configFile);
  void allocateBuffer(std::fstream &conf, std::vector<ResStation> &buffer);
  void allocateReorderBuffer(std::fstream &conf);
  void setLatencies(std::fstream &conf);
  void allocateRegisters(std::vector<Register> &registers);
  void process();
};
