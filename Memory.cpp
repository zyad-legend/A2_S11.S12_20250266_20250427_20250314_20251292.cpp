#include "Memory.h"
#include <iostream>
#include <iomanip>

Memory::Memory() {
    clear();
}

void Memory::clear() {
    for (int i = 0; i < 256; ++i) {
        cells[i] = 0;
    }
}

bool Memory::write(int address, unsigned char value) {
    if (address < 0 || address > 255) return false;
    cells[address] = value;
    return true;
}

unsigned char Memory::read(int address) const {
    if (address < 0 || address > 255) return 0;
    return cells[address];
}

void Memory::display() const {
    std::cout << "\n--- Memory State (Hex) ---\n    ";
    for (int i = 0; i < 16; ++i)
        std::cout << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << i << " ";
    std::cout << "\n";

    for (int i = 0; i < 256; i += 16) {
        std::cout << std::setw(2) << std::setfill('0') << i << " ";
        for (int j = 0; j < 16; ++j) {
            std::cout << std::setw(2) << std::setfill('0')
                      << static_cast<int>(cells[i + j]) << " ";
        }
        std::cout << "\n";
    }
    std::cout << std::dec << std::setfill(' ');
}

int Memory::getSize() const {
    return 256;
}
