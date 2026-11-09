#include "Machine.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

Machine::Machine() {
    screen = 0;
    halted = false;
}

void Machine::reset() {
    mem.clear();
    regFile.reset();
    screen = 0;
    halted = false;
}

bool Machine::isValidInstruction(unsigned short inst) const {
    int opcode = (inst >> 12) & 0xF;
    // allowed opcodes: 1 to C
    return opcode >= 1 && opcode <= 12;
}

bool Machine::loadProgramFromFile(const std::string& filename, int startAddress) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::string line;
    int currentAddr = startAddress;

    while (std::getline(file, line)) {
        // remove comments
        size_t commentPos = line.find(';');
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }

        std::stringstream ss(line);
        std::string token;
        if (!(ss >> token)) continue; // empty line

        // each instruction must be exactly 4 hex digits
        if (token.length() != 4) { mem.clear(); return false; }

        unsigned int instVal;
        std::stringstream converter;
        converter << std::hex << token;
        if (!(converter >> instVal)) { mem.clear(); return false; }

        if (!isValidInstruction((unsigned short)instVal)) { mem.clear(); return false; }

        // memory overflow check
        if (currentAddr + 1 > 255) { mem.clear(); return false; }

        // store high byte then low byte
        mem.write(currentAddr, (instVal >> 8) & 0xFF);
        mem.write(currentAddr + 1, instVal & 0xFF);

        currentAddr += 2;
    }

    regFile.setPC(startAddress);
    return true;
}

void Machine::step() {
    if (halted) return;

    int pc = regFile.getPC();
    if (pc >= 255) { halted = true; return; }

    // 1. Fetch
    unsigned char b1 = mem.read(pc);
    unsigned char b2 = mem.read(pc + 1);
    unsigned short ir = (b1 << 8) | b2;

    if (!isValidInstruction(ir)) {
        std::cout << "Error: Invalid Instruction encountered!\n";
        halted = true;
        return;
    }

    regFile.setIR(ir);
    regFile.setPC(pc + 2);

    // 2. Decode
    int opcode = (ir >> 12) & 0xF;
    int r  = (ir >> 8) & 0xF;
    int xy = ir & 0xFF;
    int x  = (ir >> 4) & 0xF;
    int y  = ir & 0xF;

    // 3. Execute
    switch (opcode) {
        case 0x1: // LOAD R, [XY]
            regFile.setRegister(r, mem.read(xy));
            break;
        case 0x2: // LOAD R, XY (bit pattern)
            regFile.setRegister(r, xy);
            break;
        case 0x3: // STORE R, [XY]
            if (xy == 0xFF) {
                screen = regFile.getRegister(r);
                std::cout << "\n[Screen Output (FF)] Value: " << std::hex
                          << (int)screen << std::dec << "\n";
            } else {
                mem.write(xy, regFile.getRegister(r));
            }
            break;
        case 0x4: // MOVE: 40RS -> R = S
            regFile.setRegister(x, regFile.getRegister(y));
            break;
        case 0x5: { // ADD (two's complement): 5RST -> R = S + T
            signed char a = (signed char)regFile.getRegister(x);
            signed char b = (signed char)regFile.getRegister(y);
            regFile.setRegister(r, (unsigned char)(a + b));
            break;
        }
        case 0x6: // ADD (floating point) - TODO: needs the float format from the assignment
            break;
        case 0x7: // OR
            regFile.setRegister(r, regFile.getRegister(x) | regFile.getRegister(y));
            break;
        case 0x8: // AND
            regFile.setRegister(r, regFile.getRegister(x) & regFile.getRegister(y));
            break;
        case 0x9: // XOR
            regFile.setRegister(r, regFile.getRegister(x) ^ regFile.getRegister(y));
            break;
        case 0xA: { // ROTATE right: AR0X
            unsigned char v = regFile.getRegister(r);
            int n = y % 8;
            if (n != 0)
                regFile.setRegister(r, (unsigned char)((v >> n) | (v << (8 - n))));
            break;
        }
        case 0xB: // JUMP if R == R0
            if (regFile.getRegister(r) == regFile.getRegister(0))
                regFile.setPC(xy);
            break;
        case 0xC: // HALT
            halted = true;
            break;
    }
}

void Machine::runFull() {
    int count = 0;
    while (!halted && count++ < 100000) step();
    if (!halted) std::cout << "Stopped: too many steps (possible infinite loop).\n";
}

void Machine::displayState() const {
    regFile.display();
    mem.display();
    std::cout << "Screen (Address FF): " << std::hex << (int)screen << std::dec << "\n";
}

void Machine::editMemory(int address, unsigned char val) {
    if (!mem.write(address, val)) std::cout << "Invalid address!\n";
}

void Machine::editRegister(int regIndex, unsigned char val) {
    if (regIndex < 0 || regIndex > 15) {
        std::cout << "Invalid register!\n";
        return;
    }
    regFile.setRegister(regIndex, val);
}
