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
    void memory_dump(std::ostream& stream) const {
        memory.dump(stream);
    }

    void boot(std::unique_ptr<program> p) {

        while(p->next_instruction()) {
            p->get_instruction().declare(&memory);
        }
        p->to_first_instruction();

        while(p->next_instruction()) {
            p->get_instruction().execute(&memory, &flags);
        }

    }
    /*
    void boot(const Program& p) { // (const Program& p) ???
    }
     */
private:
    Memory memory;
    Flags flags;
};









#endif //JNP1_6_COMPUTER_H
