#ifndef MACHINE_H
#define MACHINE_H

#include "Memory.h"
#include "RegisterFile.h"
#include <string>

class Machine {
private:
    Memory mem;
    RegisterFile regFile;
    unsigned char screen; // value stored at address FF
    bool halted;

    bool isValidInstruction(unsigned short instruction) const;

public:
    Machine();
    bool loadProgramFromFile(const std::string& filename, int startAddress);
    void step();
    void runFull();
    void displayState() const;
    void editMemory(int address, unsigned char val);
    void editRegister(int regIndex, unsigned char val);
    void reset();
    bool isHalted() const { return halted; }
};

#endif
