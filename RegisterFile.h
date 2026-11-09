#ifndef REGISTER_FILE_H
#define REGISTER_FILE_H

class RegisterFile {
private:
    unsigned char regs[16]; // 16 registers: 0 to F
    int pc;                 // Program Counter
    unsigned short ir;      // Instruction Register (4 hex digits)

public:
    RegisterFile();
    void reset();

    void setRegister(int index, unsigned char val);
    unsigned char getRegister(int index) const;

    void setPC(int val);
    int getPC() const;

    void setIR(unsigned short val);
    unsigned short getIR() const;

    void display() const;
};

#endif
