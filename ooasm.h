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

using value_t = int64_t;
using ID = const char*;

class Processor {
private:
    size_t N, abused_memory = 0;
    value_t* tab;
    ID *labels;

    bool SF = 0;
    bool ZF = 0;

    friend Lea;
    friend Mem;
public:
    explicit Processor(size_t _N) : N(_N), tab(new value_t[_N]), labels(new ID[_N]) {}
};


class Memory {
public:
    explicit Memory(size_t mem_size): mem_vector(mem_size, 0), var_addr() {}
    void dump(std::ostream stream) {
        for(auto i:mem_vector) {
            stream << i << " ";
        }
    }

private:
    std::unordered_map<const char*, size_t> var_addr;
    size_t next_index = 0;
    std::vector<value_t> mem_vector;
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
    void set(int64_t res) {
        ZF = res == 0;
        SF = res < 0;
    }
};

//--------------------------------------------elements of OOAsm language-----------------------------------------------

//Nadklasa p-wartości
class Rvalue {
public:
    virtual inline value_t get_value(Processor *proc) const = 0;
};

class Lvalue {
protected:
    std::unique_ptr<Rvalue> rvalue;
    Lvalue(std::unique_ptr<Rvalue> &&r) : rvalue(std::move(r)) {}

public:
    virtual inline void set_value(Processor *proc, int64_t new_val) const = 0;
};

class Num : public Rvalue {
private:
    value_t x;
public:
    Num(value_t _x) : x(_x) {}

    value_t inline get_value(Processor *proc) const override;
};

class Lea : public Rvalue {
private:
    ID id;
public:
    Lea(ID _id) : id(_id) {}

    value_t inline get_value(Processor *proc) const override;
};

class Mem : public Rvalue, public Lvalue {
public:
    //@TODO: Te konstruktory są strasznie brzydkie (ładniej byłoby jakbyśmy mogli wczytać po prostu Rvalue),
    //@TODO: ale nie wiem jak zrobić referencję kiedy argumentem jest obiekt
    //moze coś takiego?
    // mem(RValue& rval) : m_rvalue(rval) {}
    explicit Mem(std::unique_ptr<Rvalue> &&rvalue) : Lvalue(std::move(rvalue)) {}

    value_t inline get_value(Processor *proc) const override;

    void set_value(Processor *proc, value_t new_val) const override;
};

//--------------------------------------------Instructions-------------------------------------------------------

class Instruction {
    virtual inline void execute(Processor *proc) const = 0;
public:
    virtual void evaluate(Memory&, Flags&) const = 0;
    virtual void pre_evaluate(Memory&) const {}
};

class Data : Instruction {
private:
    ID id;
    std::unique_ptr<Num> n;
public:
    Data(ID _id, std::unique_ptr<Num> &&_n) : id(_id), n(std::move(_n)) {}

    inline void execute(Processor *proc) const override;
    inline void evaluate(Memory&, Flags&) const;
    inline void pre_evaluate(Memory&) const;
};

class Mov : Instruction {
private:
    std::unique_ptr<Lvalue> lvalue;
    std::unique_ptr<Rvalue> rvalue;
public:
    Mov(std::unique_ptr<Lvalue> &&l, std::unique_ptr<Rvalue> &&r) : lvalue(std::move(l)), rvalue(std::move(r)) {}

    inline void execute(Processor *proc) const override;
    inline void evaluate(Memory&, Flags&) const;
    inline void pre_evaluate(Memory&) const;
};

//--------------------------------------------Arithmetic operations-------------------------------------------------------

class Add : Instruction {
private:
    std::unique_ptr<Lvalue> lvalue;
    std::unique_ptr<Rvalue> rvalue;
public:
    Add(std::unique_ptr<Lvalue> &&l, std::unique_ptr<Rvalue> &&r) : lvalue(std::move(l)), rvalue(std::move(r)) {}

    inline void execute(Processor *proc) const override;
    inline void evaluate(Memory&, Flags&) const;
    inline void pre_evaluate(Memory&) const;
};

class Sub : Instruction {
private:
    std::unique_ptr<Lvalue> lvalue;
    std::unique_ptr<Rvalue> rvalue;
public:
    Sub(std::unique_ptr<Lvalue> &&l, std::unique_ptr<Rvalue> &&r) : lvalue(std::move(l)), rvalue(std::move(r)) {}

    inline void execute(Processor *proc) const override;
    inline void evaluate(Memory&, Flags&) const;
    inline void pre_evaluate(Memory&) const;
};

class Inc : Instruction {
private:
    std::unique_ptr<Lvalue> lvalue;
public:
    Inc(std::unique_ptr<Lvalue> l) : lvalue(std::move(l)) {}

    inline void execute(Processor *proc) const override;
    inline void evaluate(Memory&, Flags&) const;
    inline void pre_evaluate(Memory&) const;
};

class Dec : Instruction {
private:
    std::unique_ptr<Lvalue> lvalue;
public:
    Dec(std::unique_ptr<Lvalue> l) : lvalue(std::move(l)) {}

    inline void execute(Processor *proc) const override;
    inline void evaluate(Memory&, Flags&) const;
    inline void pre_evaluate(Memory&) const;
};

class One : Instruction {
private:
    std::unique_ptr<Lvalue> lvalue;
public:
    One(std::unique_ptr<Lvalue> l) : lvalue(std::move(l)) {}

    inline void execute(Processor *proc) const override;
    inline void evaluate(Memory&, Flags&) const;
    inline void pre_evaluate(Memory&) const;

};

class Onez : Instruction {
private:
    std::unique_ptr<Lvalue> lvalue;
public:
    Onez(std::unique_ptr<Lvalue> l) : lvalue(std::move(l)) {}

    inline void execute(Processor *proc) const override;
    inline void evaluate(Memory&, Flags&) const;
    inline void pre_evaluate(Memory&) const;
};

class Ones : Instruction {
private:
    std::unique_ptr<Lvalue> lvalue;
public:
    Ones(std::unique_ptr<Lvalue> l) : lvalue(std::move(l)) {}

    inline void execute(Processor *proc) const override;
    inline void evaluate(Memory&, Flags&) const;
    inline void pre_evaluate(Memory&) const;
};


class end_exception : public std:: exception {
public:
    const char* what() const noexcept override {
        return "end of instruction";
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
