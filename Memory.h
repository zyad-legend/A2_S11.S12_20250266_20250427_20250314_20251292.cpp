#ifndef MEMORY_H
#define MEMORY_H

class Memory {
private:
    unsigned char cells[256]; // 256 memory cells: 00 to FF

public:
    Memory();
    void clear();
    bool write(int address, unsigned char value);
    unsigned char read(int address) const;
    void display() const;
    int getSize() const;
};

#endif
