//
// Created by stasiek on 23.01.2021.
//

#include "ooasm.h"

int64_t Num::get_value(Memory *memory) const {return x;}

int64_t Lea::get_value(Memory *memory) const {
    return memory->var_addr.at(id);
}

int64_t Mem::get_value(Memory *memory) const  {
    return memory->mem_vector.at(rvalue->get_value(memory));
}

void Mem::set_value(Memory *memory, value_t new_val) const {
    memory->mem_vector.at(rvalue->get_value(memory)) = new_val;
}


void Data::declare(Memory *memory) const {
    memory->mem_vector.at(memory->next_index) = n->get_value(memory);
    memory->var_addr.insert(std::make_pair(id, n->get_value(memory)));
}

void Mov::execute(Memory *memory, Flags *flags) const {
    memory->mem_vector.at(memory->next_index) = rvalue->get_value(memory);
}


void Add::execute(Memory *memory, Flags *flags) const {
    value_t result = lvalue->get_value(memory) + rvalue->get_value(memory);
    lvalue->set_value(memory, result);
    flags->set(result);
}
void Sub::execute(Memory *memory, Flags *flags) const {
    value_t result = lvalue->get_value(memory) - rvalue->get_value(memory);
    lvalue->set_value(memory, result);
    flags->set(result);
}

void Inc::execute(Memory *memory, Flags *flags) const {
    value_t result = lvalue->get_value(memory) + 1;
    lvalue->set_value(memory, result);
    flags->set(result);
}

void Dec::execute(Memory *memory, Flags *flags) const {
    value_t result = lvalue->get_value(memory) - 1;
    lvalue->set_value(memory, result);
    flags->set(result);
}


void One::execute(Memory *memory, Flags *flags) const {
    lvalue->set_value(memory, 1);
}
void Onez::execute(Memory *memory, Flags *flags) const {
    if (flags->is_zero())
        lvalue->set_value(memory, 1);
}
void Ones::execute(Memory *memory, Flags *flags) const {
    if (flags->is_signed())
        lvalue->set_value(memory, 1);
}