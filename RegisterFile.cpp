#include "RegisterFile.h"
#include <iostream>
#include <iomanip>

RegisterFile::RegisterFile() {
    reset();
}

void RegisterFile::reset() {
    for (int i = 0; i < 16; ++i) {
        regs[i] = 0;
    }
    pc = 0;
    ir = 0;
}

void RegisterFile::setRegister(int index, unsigned char val) {
    if (index >= 0 && index < 16) {
        regs[index] = val;
    }
}

unsigned char RegisterFile::getRegister(int index) const {
    if (index >= 0 && index < 16) return regs[index];
    return 0;
}

void RegisterFile::setPC(int val) {
    pc = val;
}

int RegisterFile::getPC() const {
    return pc;
}

void RegisterFile::setIR(unsigned short val) {
    ir = val;
}

unsigned short RegisterFile::getIR() const {
    return ir;
}

void RegisterFile::display() const {
    std::cout << "\n--- Registers ---\n";
    for (int i = 0; i < 16; ++i) {
        std::cout << "R" << std::uppercase << std::hex << i << ": "
                  << std::setw(2) << std::setfill('0') << static_cast<int>(regs[i]) << "  ";
        if ((i + 1) % 4 == 0) std::cout << "\n";
    }
    std::cout << "PC: " << std::setw(2) << std::setfill('0') << pc << "  ";
    std::cout << "IR: " << std::setw(4) << std::setfill('0') << ir
              << std::dec << std::setfill(' ') << "\n";
}
