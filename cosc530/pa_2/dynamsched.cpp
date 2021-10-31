#include "dynamsched.hpp"

#include <deque>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int main() {
  DynamSched sched = DynamSched("source/config.txt");
  sched.process();

  return 0;
}

/* Util Functions */

int getValue(fstream& f) {
  string line;
  stringstream ss;
  int val;
  getline(f, line, ':');
  getline(f, line);
  ss << line;
  ss >> val;

  return val;
}

INST strToInst(string inst) {
  if (inst == "lw")
    return LW;
  else if (inst == "flw")
    return FLW;
  else if (inst == "sw")
    return SW;
  else if (inst == "fsw")
    return FSW;
  else if (inst == "add")
    return ADD;
  else if (inst == "sub")
    return SUB;
  else if (inst == "beq")
    return BEQ;
  else if (inst == "bne")
    return BNE;
  else if (inst == "fadd.s")
    return FADD_S;
  else if (inst == "fsub.s")
    return FSUB_S;
  else if (inst == "fmul.s")
    return FMUL_S;
  else if (inst == "fdiv.s")
    return FDIV_S;
  perror("Inst was not valid");
  exit(-1);
}

void parseRegisters(Instruction& inst, string regs) {
  int a = -1, b = -1, c = -1;
  if (inst.opcode & DT) {
    sscanf(regs.c_str(), "%*c%d,%*d(%*c%d):%*d", &a, &b);
  } else if (inst.opcode & CTRL) {
    sscanf(regs.c_str(), "%*c%d,%*c%d", &a, &b);
  } else {
    sscanf(regs.c_str(), "%*c%d,%*c%d,%*c%d", &a, &b, &c);
  }

  if (a != -1) inst.regs.push_back(a);
  if (b != -1) inst.regs.push_back(b);
  if (c != -1) inst.regs.push_back(c);
}

template <typename T>
HAZARD checkReorderBuffer(deque<T>& buffer, int size) {
  return (buffer.size() >= size) ? STRUCT : NONE;
}

template <typename T>
HAZARD checkRSBuffer(deque<T>& buffer, int size) {
  return (size >= buffer.size()) ? STRUCT : NONE;
}

/* End Util Functions */

DynamSched::DynamSched(string configFile) {
  string line;
  fstream conf;

  conf.open(configFile);

  if (!conf.is_open()) {
    perror("Error opening config file.\n");
    exit(-1);
  }

  while (getline(conf, line) && line != "buffers")
    ;

  // Skip empty line
  getline(conf, line);

  // Allocate Effective Address Buffer
  this->allocateBuffer(conf, this->effAddr);

  // Allocate Floating-Point Adds Buffer
  this->allocateBuffer(conf, this->fpAdds);

  // Allocate Floating-Point Muls Buffer
  this->allocateBuffer(conf, this->fpMuls);

  // Allocate Int Buffer
  this->allocateBuffer(conf, this->arithmetics);

  // Allocate Reorder Buffer
  this->allocateReorderBuffer(conf);

  // Set Latencies
  this->setLatencies(conf);

  // Allocate Floating-Point Registers
  this->allocateRegisters(this->fps);

  // Allocate Integer Registers
  this->allocateRegisters(this->ints);
}

void DynamSched::allocateBuffer(fstream& conf, ResStationBuffer& buffer) {
  int size = 0;

  size = getValue(conf);

  buffer = ResStationBuffer(size);
}

void DynamSched::allocateReorderBuffer(fstream& conf) {
  int size = 0;

  size = getValue(conf);
  this->reorder = ReorderBuffer(size);
}

void DynamSched::setLatencies(fstream& conf) {
  int cycles;

  cycles = getValue(conf);
  this->latencies.fp_add = cycles;

  cycles = getValue(conf);
  this->latencies.fp_sub = cycles;

  cycles = getValue(conf);
  this->latencies.fp_mul = cycles;

  cycles = getValue(conf);
  this->latencies.fp_div = cycles;
}

void DynamSched::allocateRegisters(vector<Register>& registers) {
  registers.resize(16);
  for (int i = 0; i < 16; i++) {
    registers.at(i) = Register();
  }
}

void DynamSched::process() {
  string op, regs;
  HAZARD h = NONE;
  bool issued = true;

  while (issued && cin >> op >> regs) {
    issued = this->issue(op, regs);
    this->execute();
  }
}

bool DynamSched::issue(string op, string regs) {
  // Parse instruction
  Instruction inst;
  inst.opcode = strToInst(op);
  parseRegisters(inst, regs);

  // Check Reorder Buffer
  if ((h = checkReorderBuffer(this->reorder.buffer, this->reorder.size)) ==
      STURCT) {
    // Stall
  }

  // Integer Ops and Branch Ops
  if (inst.opcode & ARITH || inst.opcode & CTRL) {
    if ((h = checkRSBuffer(this->arithmetics.buffer, this->arithmetics.size)) ==
        STURCT) {
      // Stall
    }

    // Insert into reorder buffer and reservation station
    this->reorder.insert(inst, this->arithmetics);
  }
  // Data Transfer Ops
  else if (inst.opcode & DT) {
    if ((h = checkRSBuffer(this->effAddr.buffer, this->effAddr.size)) ==
        STURCT) {
      // Stall
    }

    // Insert into reorder buffer and reservation station
    this->reorder.insert(inst, this->effAddr);
  }
  // Floating Point Add/Sub Ops
  else if (inst.opcode & FPA) {
    if ((h = checkRSBuffer(this->fpAdds.buffer, this->fpAdds.size)) == STURCT) {
      // Stall
    }

    // Insert into reorder buffer and reservation station
    this->reorder.insert(inst, this->fpAdds);
  }
  // Floating Point Mul/Div Ops
  else if (inst.opcode & FPM) {
    if ((h = checkRSBuffer(this->fpMuls.buffer, this->fpMuls.size)) == STURCT) {
      // Stall
    }

    // Insert into reorder buffer and reservation station
    this->reorder.insert(inst, this->fpMuls);
  }

  // If instruction was successfully issued, read next instruction on following
  // cycle
  return true;
}

void DynamSched::execute() {
  // Check every RS buffer against the CDB
  this->checkCDB(this->effAddr);
  this->checkCDB(this->fpAdds);
  this->checkCDB(this->fpMuls);
  this->checkCDB(this->arithmetics);
}

void DynamSched::checkCDB(ResStationBuffer& buffer) {
  ResStation rs;
  for (int i = 0; i < buffer.buffer.size(); i++) {
    if (rs.rs1 == CommonDataBus->rs) {
      rs.rs1 = NULL;
      rs.val1 = 1;
    } else if (rs.rs2 == CommonDataBus->rs) {
      rs.rs2 = NULL;
      rs.val2 = 1;
    }
  }
}

/* Reorder Buffer */

ReorderBuffer::ReorderBuffer(int size) { this->size = size; }

ReorderBuffer::insert(Instruction inst, ResStationBuffer& rs_buffer) {
  ResStation rs = createRS(inst);
  Entry e;
  ResStation* temp;

  // Loop through reorder buffer
  for (int i = this->buffer.size() - 1; i >= 0; i++) {
    if (rs.rs1 == NULL && rs.rs2 == NULL) break;
    temp = this->buffer.at(i).rs;

    // Loop through operand registers of the new RS
    for (int j = 1; j < rs.inst.regs.size(); j++) {
      // Check if the current operand is the result of an op in the buffer
      if (re.busy && re.inst.regs.at(j) == temp->inst.regs.at(0)) {
        if (rs.rs1 == NULL) rs.rs1 = temp;
        break;
        else if (rs.rs2 == NULL) rs.rs2 = temp;
        break;
      }
    }
  }

  // TODO: Make sure that I don't need to pass in register vector instead of
  // assuming that the register is open if it isn't in another RS
  if (rs.rs1 == NULL) {
    rs.val1 = 1
  }
  if (rs.rs2 == NULL) {
    rs.val2 = 1
  }

  // Insert RS into RSBuffer
  rs_buffer.insert(rs);

  // Create Entry
  e = createEntry(inst, rs);
  e.ready = false;
  this->buffer.push_back(e);
}

void ReorderBuffer::commitEntry() {
  // Remove entry at head of buffer and commit the results
  // Possibly return Entry to caller and take care of scheduler interaction
  // outside of buffer
}

/* End Reorder Buffer */

/* ResStationBuffer */

ResStationBuffer::ResStationBuffer(int size) {
  this->buffer.resize(size);
  this->size = 0;
}

void ResStationBuffer::insert(ResStation rs) {
  for (int i = 0; i < this->buffer.size(); i++) {
    if (!this->buffer.at(i).busy) {
      this->buffer.at(i) = rs;
    }
  }
}

ResStation createRS(Instruction inst) {
  ResStation rs;
  rs.inst = inst;
}

/* End ResStationBuffer */

/* Entry Functions */

Entry createEntry(Instruction inst, ResStation& rs) {
  Entry e;
  e.ready = false;
  e.INST = inst.opcode;
  e.rs = &rs;
}

/* End Entry Functions */