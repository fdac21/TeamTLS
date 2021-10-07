#include <string>

class Buffer {
   public:
    int effAddr;
    int fpAdds;
    int fpMuls;
    int Ints;
    int reorder;

    Buffer(int effAddr, int fpAdds, int fpMuls, int Ints, int reorder);
}

class Latency {
   public:
    int fp_add;
    int fp_sub;
    int fp_mul;
    int fp_div;

    Latency(int fp_add, int fp_sub, int fp_mul, int fp_div);
}

void
initScheduler(Buffer *buf, Latency lat *);