//
// Created by zuza on 21.01.2021.
//

#ifndef JNP1_6_OOASM_H
#define JNP1_6_OOASM_H

#include <bits/stdc++.h>
#include <iostream>



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


class Memory {
public:
    explicit Memory(size_t mem_size): mem_vector(mem_size, 0), var_addr() {}
    void dump(std::ostream& stream) const {
        for(auto i:mem_vector) {
            stream << i << " ";
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
    virtual inline value_t get_value(Memory *memory) const = 0;
};

class Lvalue {
protected:
    std::unique_ptr<Rvalue> rvalue;
    Lvalue(std::unique_ptr<Rvalue> &&r) : rvalue(std::move(r)) {}

public:
    virtual inline value_t get_value(Memory *memory) const = 0;
    virtual inline void set_value(Memory *memory , value_t new_val) const = 0;
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
    explicit Mem(std::unique_ptr<Rvalue> &&rvalue) : Lvalue(std::move(rvalue)) {}

    value_t inline get_value(Memory *memory) const override;

    void set_value(Memory *memory, value_t new_val) const override;
};

//--------------------------------------------Instructions-------------------------------------------------------

class Instruction {
public:
    virtual void execute(Memory *memory, Flags *flags) const = 0;
    virtual void declare(Memory *memory) const = 0;
};

class Data : Instruction {
private:
    ID id;
    std::unique_ptr<Num> n;
public:
    Data(ID _id, std::unique_ptr<Num> &&_n) : id(_id), n(std::move(_n)) {}

    inline void execute(Memory *memory, Flags *flags) const {}
    inline void declare(Memory *memory) const;
};

class Mov : Instruction {
private:
    std::unique_ptr<Lvalue> lvalue;
    std::unique_ptr<Rvalue> rvalue;
public:
    Mov(std::unique_ptr<Lvalue> &&l, std::unique_ptr<Rvalue> &&r) : lvalue(std::move(l)), rvalue(std::move(r)) {}

    inline void execute(Memory *memory, Flags *flags) const;
    inline void declare(Memory *memory) const {}
};

//--------------------------------------------Arithmetic operations-------------------------------------------------------


class Add : Instruction {
private:
    std::unique_ptr<Lvalue> lvalue;
    std::unique_ptr<Rvalue> rvalue;
public:
    Add(std::unique_ptr<Lvalue> &&l, std::unique_ptr<Rvalue> &&r) : lvalue(std::move(l)), rvalue(std::move(r)) {}

    inline void execute(Memory *memory, Flags *flags) const;
    inline void declare(Memory *memory) const {}
};

class Sub : Instruction {
private:
    std::unique_ptr<Lvalue> lvalue;
    std::unique_ptr<Rvalue> rvalue;
public:
    Sub(std::unique_ptr<Lvalue> &&l, std::unique_ptr<Rvalue> &&r) : lvalue(std::move(l)), rvalue(std::move(r)) {}

    inline void execute(Memory *memory, Flags *flags) const;
    inline void declare(Memory *memory) const {}
};

class Inc : Instruction {
private:
    std::unique_ptr<Lvalue> lvalue;
public:
    Inc(std::unique_ptr<Lvalue> l) : lvalue(std::move(l)) {}

    inline void execute(Memory *memory, Flags *flags) const;
    inline void declare(Memory *memory) const {}
};

class Dec : Instruction {
private:
    std::unique_ptr<Lvalue> lvalue;
public:
    Dec(std::unique_ptr<Lvalue> l) : lvalue(std::move(l)) {}

    inline void execute(Memory *memory, Flags *flags) const;
    inline void declare(Memory *memory) const {}
};

class One : Instruction {
private:
    std::unique_ptr<Lvalue> lvalue;
public:
    One(std::unique_ptr<Lvalue> l) : lvalue(std::move(l)) {}

    inline void execute(Memory *memory, Flags *flags) const;
    inline void declare(Memory *memory) const {}

};

class Onez : Instruction {
private:
    std::unique_ptr<Lvalue> lvalue;
public:
    Onez(std::unique_ptr<Lvalue> l) : lvalue(std::move(l)) {}

    inline void execute(Memory *memory, Flags *flags) const;
    inline void declare(Memory *memory) const {}
};

class Ones : Instruction {
private:
    std::unique_ptr<Lvalue> lvalue;
public:
    Ones(std::unique_ptr<Lvalue> l) : lvalue(std::move(l)) {}

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


std::unique_ptr<Num> num(value_t value) {
    return std::make_unique<Num>(value);
}

std::unique_ptr<Lea> lea(ID id) {
    return std::make_unique<Lea>(id);
}

std::unique_ptr<Mem> mem(std::unique_ptr<Rvalue> &&rvalue) {
    return std::make_unique<Mem>(std::move(rvalue));
}

std::unique_ptr<Data> data(ID id, std::unique_ptr<Num> &&n) {
    return std::make_unique<Data>(id, std::move(n));
}

std::unique_ptr<Mov> mov(std::unique_ptr<Lvalue> &&lvalue, std::unique_ptr<Rvalue> &&rvalue) {
    return std::make_unique<Mov>(std::move(lvalue), std::move(rvalue));
}

std::unique_ptr<Add> add(std::unique_ptr<Lvalue> &&lvalue, std::unique_ptr<Rvalue> &&rvalue) {
    return std::make_unique<Add>(std::move(lvalue), std::move(rvalue));
}

std::unique_ptr<Sub> sub(std::unique_ptr<Lvalue> &&lvalue, std::unique_ptr<Rvalue> &&rvalue) {
    return std::make_unique<Sub>(std::move(lvalue), std::move(rvalue));
}

std::unique_ptr<Inc> inc(std::unique_ptr<Lvalue> &&lvalue) {
    return std::make_unique<Inc>(std::move(lvalue));
}

std::unique_ptr<Dec> dec(std::unique_ptr<Lvalue> &&lvalue) {
    return std::make_unique<Dec>(std::move(lvalue));
}


std::unique_ptr<One> one(std::unique_ptr<Lvalue> &&lvalue) {
    return std::make_unique<One>(std::move(lvalue));
}

std::unique_ptr<Onez> onez(std::unique_ptr<Lvalue> &&lvalue) {
    return std::make_unique<Onez>(std::move(lvalue));
}

std::unique_ptr<Ones> ones(std::unique_ptr<Lvalue> &&lvalue) {
    return std::make_unique<Ones>(std::move(lvalue));
}








#endif //JNP1_6_OOASM_H
