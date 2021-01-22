//
// Created by zuza on 21.01.2021.
//

#ifndef JNP1_6_COMPUTER_H
#define JNP1_6_COMPUTER_H

#include "ooasm.h"
#include <bits/stdc++.h>

class  Computer {
public:
    explicit  Computer(size_t memory_size): memory(memory_size), flags() {}
    void boot(const Program &p) {
        while(p.next_instruction()) {
            const Instruction& instruction = p.next_instruction();
            instruction.pre_evaluate(memory);
        }
        p.to_first_instruction();

        while(p.next_instruction()) {
            const Instruction& instruction = p.get_instruction();
            instruction.evaluate(memory, flags);
        }
    }
    void memory_dump(std::ostream& stream) {
        memory.dump(stream);
    }

private:
    Memory memory;
    Flags flags;

};









#endif //JNP1_6_COMPUTER_H
