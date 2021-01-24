//
// Created by Zuza & Stasiek on 21.01.2021.
//

#ifndef JNP1_6_OOASM_H
#define JNP1_6_OOASM_H

#include <bits/stdc++.h>
#include <iostream>

#define MIN_ID_LEN 1
#define MAX_ID_LEN 10

class Rvalue;  //Nadklasa p-wartości
class Lvalue;  //Nadklasa l-wartości
class Num;
class Lea;
class Mem;

class Instruction;
class Data;
class Mov;

class Add;
class Sub;
class Inc;
class Dec;

class One;
class Onez;
class Ones;

using value_t = int64_t;
using ID = const char*;

bool check_ID_correctness(ID id) {
    std::string _id = id;
    if (_id.length() < MIN_ID_LEN || _id.length() > MAX_ID_LEN)
        throw std::invalid_argument("bad ID length");
    else
        return true;
}

class Memory {
public:
    explicit Memory(size_t mem_size): mem_vector(mem_size, 0), var_addr() {}
    void dump(std::ostream& stream) const {
        for(auto i:mem_vector) {
            stream << i << " ";
        }
    }
    void reset() {
        for(auto& i:mem_vector) {
            i = 0;
        }
    }

private:
    std::unordered_map<ID, size_t> var_addr;
    size_t next_index = 0;
    std::vector<value_t> mem_vector;

    friend Lea;
    friend Mem;
    friend Data;
    friend Mov;
};

class Flags {
    bool ZF = false;
    bool SF = false;

public:
    bool is_signed() const noexcept {
        return SF;
    }
    bool is_zero() const noexcept {
        return ZF;
    }
    void set(value_t res) {
        ZF = res == 0;
        SF = res < 0;
    }

};

//--------------------------------------------elements of OOAsm language-----------------------------------------------

//Nadklasa p-wartości
class Rvalue {
public:
    virtual inline value_t get_value(Memory *memory) const {return 0;};
};

class Lvalue {
protected:
    std::shared_ptr<Rvalue> rvalue;
    Lvalue(std::shared_ptr<Rvalue> &&r) : rvalue(std::move(r)) {}

public:
    virtual inline value_t get_value(Memory *memory) const {return 0;};
    virtual inline void set_value(Memory *memory , value_t new_val) const {};
};

class Num : public Rvalue {
private:
    value_t x;
public:
    Num(value_t _x) : x(_x) {}

    value_t inline get_value(Memory *memory) const override;
};

class Lea : public Rvalue {
private:
    ID id;
public:
    Lea(ID _id) : id(_id) {}

    value_t inline get_value(Memory *memory) const override;
};

class Mem : public Rvalue, public Lvalue {
public:
    explicit Mem(std::shared_ptr<Rvalue> &&rvalue) : Lvalue(std::move(rvalue)) {}

    value_t inline get_value(Memory *memory) const override;

    void set_value(Memory *memory, value_t new_val) const override;
};

//--------------------------------------------Instructions-------------------------------------------------------

class Instruction {
public:
    virtual void execute(Memory *memory, Flags *flags) const = 0;
    virtual void declare(Memory *memory) const = 0;
};

class Data : public Instruction {
private:
    ID id;
    std::shared_ptr<Num> n;
public:
    Data(ID _id, std::shared_ptr<Num> &&_n) : id(_id), n(std::move(_n)) {}

    inline void execute(Memory *memory, Flags *flags) const {}
    inline void declare(Memory *memory) const;
};

class Mov : public Instruction {
private:
    std::shared_ptr<Lvalue> lvalue;
    std::shared_ptr<Rvalue> rvalue;
public:
    Mov(std::shared_ptr<Lvalue> &&l, std::shared_ptr<Rvalue> &&r) : lvalue(std::move(l)), rvalue(std::move(r)) {}

    inline void execute(Memory *memory, Flags *flags) const;
    inline void declare(Memory *memory) const {}
};

//--------------------------------------------Arithmetic operations-------------------------------------------------------


class Add : public Instruction {
private:
    std::shared_ptr<Lvalue> lvalue;
    std::shared_ptr<Rvalue> rvalue;
public:
    Add(std::shared_ptr<Lvalue> &&l, std::shared_ptr<Rvalue> &&r) : lvalue(std::move(l)), rvalue(std::move(r)) {}

    inline void execute(Memory *memory, Flags *flags) const;
    inline void declare(Memory *memory) const {}
};

class Sub : public Instruction {
private:
    std::shared_ptr<Lvalue> lvalue;
    std::shared_ptr<Rvalue> rvalue;
public:
    Sub(std::shared_ptr<Lvalue> &&l, std::shared_ptr<Rvalue> &&r) : lvalue(std::move(l)), rvalue(std::move(r)) {}

    inline void execute(Memory *memory, Flags *flags) const;
    inline void declare(Memory *memory) const {}
};

class Inc : public Instruction {
private:
    std::shared_ptr<Lvalue> lvalue;
public:
    Inc(std::shared_ptr<Lvalue> &&l) : lvalue(std::move(l)) {}

    inline void execute(Memory *memory, Flags *flags) const;
    inline void declare(Memory *memory) const {}
};

class Dec : public Instruction {
private:
    std::shared_ptr<Lvalue> lvalue;
public:
    Dec(std::shared_ptr<Lvalue> &&l) : lvalue(std::move(l)) {}

    inline void execute(Memory *memory, Flags *flags) const;
    inline void declare(Memory *memory) const {}
};

class One : public Instruction {
private:
    std::shared_ptr<Lvalue> lvalue;
public:
    One(std::shared_ptr<Lvalue> &&l) : lvalue(std::move(l)) {}

    inline void execute(Memory *memory, Flags *flags) const;
    inline void declare(Memory *memory) const {}

};

class Onez : public Instruction {
private:
    std::shared_ptr<Lvalue> lvalue;
public:
    Onez(std::shared_ptr<Lvalue> &&l) : lvalue(std::move(l)) {}

    inline void execute(Memory *memory, Flags *flags) const;
    inline void declare(Memory *memory) const {}
};

class Ones : public Instruction {
private:
    std::shared_ptr<Lvalue> lvalue;
public:
    Ones(std::shared_ptr<Lvalue> &&l) : lvalue(std::move(l)) {}

    inline void execute(Memory *memory, Flags *flags) const;
    inline void declare(Memory *memory) const {}
};


class end_exception : public std:: exception {
public:
    const char* what() const noexcept override {
        return "end of instruction";
    }
};


class program {
private:
    std:: vector<std::shared_ptr<Instruction>> instructions;
    mutable size_t next_index = 0;
public:
    program(std::initializer_list<std::shared_ptr<Instruction>> ins) : instructions(ins) {}

    const Instruction& get_instruction() const {
        if (!next_instruction())
            throw end_exception();
        return *instructions[next_index++];
    }
    bool next_instruction() const{
        return next_index < instructions.size();
    }
    void to_first_instruction() const {
        next_index = 0;
    }

};

//---------------------------------------------------unique_pointers----------------------------------------------------


std::shared_ptr<Num> num(value_t value) {
    return std::make_shared<Num>(value);
}

std::shared_ptr<Lea> lea(ID id) {
    return std::make_shared<Lea>(id);
}

std::shared_ptr<Mem> mem(std::shared_ptr<Rvalue> &&rvalue) {
    return std::make_shared<Mem>(std::move(rvalue));
}

std::shared_ptr<Data> data(ID id, std::shared_ptr<Num> &&n) {
    return std::make_shared<Data>(id, std::move(n));
}

std::shared_ptr<Mov> mov(std::shared_ptr<Lvalue> &&lvalue, std::shared_ptr<Rvalue> &&rvalue) {
    return std::make_shared<Mov>(std::move(lvalue), std::move(rvalue));
}

std::shared_ptr<Add> add(std::shared_ptr<Lvalue> &&lvalue, std::shared_ptr<Rvalue> &&rvalue) {
    return std::make_shared<Add>(std::move(lvalue), std::move(rvalue));
}

std::shared_ptr<Sub> sub(std::shared_ptr<Lvalue> &&lvalue, std::shared_ptr<Rvalue> &&rvalue) {
    return std::make_shared<Sub>(std::move(lvalue), std::move(rvalue));
}

std::shared_ptr<Inc> inc(std::shared_ptr<Lvalue> &&lvalue) {
    return std::make_shared<Inc>(std::move(lvalue));
}

std::shared_ptr<Dec> dec(std::shared_ptr<Lvalue> &&lvalue) {
    return std::make_shared<Dec>(std::move(lvalue));
}


std::shared_ptr<One> one(std::shared_ptr<Lvalue> &&lvalue) {
    return std::make_shared<One>(std::move(lvalue));
}

std::shared_ptr<Onez> onez(std::shared_ptr<Lvalue> &&lvalue) {
    return std::make_shared<Onez>(std::move(lvalue));
}

std::shared_ptr<Ones> ones(std::shared_ptr<Lvalue> &&lvalue) {
    return std::make_shared<Ones>(std::move(lvalue));
}


int64_t Num::get_value(Memory *memory) const {return x;}

int64_t Lea::get_value(Memory *memory) const {
    check_ID_correctness(id);
    return memory->var_addr.at(id);
}

int64_t Mem::get_value(Memory *memory) const  {
    return memory->mem_vector.at(rvalue->get_value(memory));
}

void Mem::set_value(Memory *memory, value_t new_val) const {
    memory->mem_vector.at(rvalue->get_value(memory)) = new_val;
}


void Data::declare(Memory *memory) const {
    check_ID_correctness(id);
    memory->mem_vector.at(memory->next_index) = n->get_value(memory);
    memory->var_addr.insert(std::make_pair(id, memory->next_index++));
}

void Mov::execute(Memory *memory, Flags *flags) const {
    //std::cout <<
    //memory->mem_vector.at(lv) = rvalue->get_value(memory);
    lvalue->set_value(memory, rvalue->get_value(memory));
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

#endif //JNP1_6_OOASM_H
