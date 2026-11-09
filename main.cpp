#include <iostream>
#include <string>
#include "Machine.h"

using namespace std;

void showMenu() {
    cout << "\n=== Machine Simulator Menu ===\n";
    cout << "1. Load Program from File (Even Address < FF)\n";
    cout << "2. Run Full Execution\n";
    cout << "3. Step-by-Step Execution\n";
    cout << "4. Edit Register or Memory Cell\n";
    cout << "5. Display Current State\n";
    cout << "6. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    Machine machine;
    int choice;
    bool running = true;

    while (running) {
        showMenu();
        if (!(cin >> choice)) {
            if (cin.eof()) break;
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Try again.\n";
            continue;
        }

        switch (choice) {
            case 1: {
                string filename;
                int startAddr;
                cout << "Enter filename: ";
                cin >> filename;
                cout << "Enter even start address (hex, e.g. 0, 2, 10): ";
                cin >> hex >> startAddr >> dec;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Error: invalid address!\n";
                    break;
                }

                if (startAddr < 0 || startAddr % 2 != 0 || startAddr > 0xFE) {
                    cout << "Error: Address must be even and less than FF!\n";
                } else {
                    machine.reset();
                    if (machine.loadProgramFromFile(filename, startAddr)) {
                        cout << "Program loaded successfully!\n";
                    } else {
                        cout << "Error loading program or invalid instructions found.\n";
                    }
                }
                break;
            }
            case 2:
                machine.runFull();
                cout << "Execution finished.\n";
                break;
            case 3:
                cin.ignore(1000, '\n');
                while (!machine.isHalted()) {
                    machine.step();
                    machine.displayState();
                    if (machine.isHalted()) break;
                    cout << "Press Enter for next step, or 'q' then Enter to stop: ";
                    string input;
                    getline(cin, input);
                    if (input == "q" || input == "Q") break;
                }
                if (machine.isHalted()) cout << "Program halted.\n";
                break;
            case 4: {
                int type;
                cout << "Edit 1. Register or 2. Memory? ";
                cin >> type;
                if (type == 1) {
                    int reg, val;
                    cout << "Enter Register index (0-F in hex): ";
                    cin >> hex >> reg;
                    cout << "Enter new Value (hex): ";
                    cin >> val >> dec;
                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "Invalid input!\n";
                    } else {
                        machine.editRegister(reg, val);
                    }
                } else if (type == 2) {
                    int addr, val;
                    cout << "Enter Memory address (hex): ";
                    cin >> hex >> addr;
                    cout << "Enter new Value (hex): ";
                    cin >> val >> dec;
                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "Invalid input!\n";
                    } else {
                        machine.editMemory(addr, val);
                    }
                }
                break;
            }
            case 5:
                machine.displayState();
                break;
            case 6:
                cout << "Goodbye!\n";
                running = false;
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}
