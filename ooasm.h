//
// Created by stasiek on 19.01.2021.
//

#ifndef ZADANIE6_OOASM_H
#define ZADANIE6_OOASM_H

#include <bits/stdc++.h>
#include <iostream>


class Rvalue;  //Nadklasa p-wartości
class Lvalue;  //Nadklasa l-wartości
class num;
class lea;
class mem;

class Instruction;
class data;
class mov;

class ArithmeticOperation;
class add;
class sub;
class inc;
class dec;

class one;
class onez;
class ones;

class Processor {
private:
    size_t N, abused_memory = 0;
    int64_t* tab;
    char **labels;

    bool SF = 0;
    bool ZF = 0;

    friend mem;
    friend lea;
    friend data;
    friend ArithmeticOperation;
    friend one;
    friend onez;
    friend ones;

public:
    Processor(size_t _N) : N(_N), tab(new int64_t[_N]), labels(new char*[_N]) {}
};

//--------------------------------------------elements of OOAsm language-----------------------------------------------

//Nadklasa p-wartości
class Rvalue {
public:
    virtual int64_t get_value(Processor *proc);
};

class Lvalue {
protected:
    Rvalue *r;
    Lvalue(Rvalue *_r) : r(_r) {}

public:
    virtual void set_value(Processor *proc, int64_t new_val);
};

class num : public Rvalue {
private:
    int64_t x;
public:
    num(int64_t _x) {x = _x;}

    int64_t get_value(Processor *proc) override;
};

class lea : public Rvalue {
private:
    char *id;
public:
    lea(char *_id) : id(_id) {}

    int64_t get_value(Processor *proc) override;
};

class mem : public Rvalue, public Lvalue {
public:
    //@TODO: Te konstruktory są strasznie brzydkie (ładniej byłoby jakbyśmy mogli wczytać po prostu Rvalue),
    //@TODO: ale nie wiem jak zrobić referencję kiedy argumentem jest obiekt
    mem(num n) : Lvalue(&n) {}
    mem(lea l) : Lvalue(&l) {}
    mem(mem const &m) : Lvalue(m) {}

    int64_t get_value(Processor *proc);

    void set_value(Processor *proc, int64_t new_val);
};


int64_t mem::get_value(Processor *proc) {
    size_t address = r->get_value(proc);

    if (address < proc->N) {
        return proc->tab[address];
    }
    else {
        //@TODO: Wyrzucamy jakiś wyjątek OutOfBoundsException
    }
}

void mem::set_value(Processor *proc, int64_t new_val) {
    size_t address = r->get_value(proc);
    if (address < proc->abused_memory) {
        proc->tab[address] = new_val;
    }
    else {
        //@TODO: Zgłaszamy jakiś OutOfBoundsException
    }
}

//--------------------------------------------Instructions-------------------------------------------------------

class Instruction {
    virtual void execute(Processor *proc);
};

class data : Instruction {
private:
    char *id;
    num n;
public:
    data(char *_id, num _n) : id(_id), n(_n) {}

    void execute(Processor *proc) override;
};

class mov : Instruction {
private:
    Lvalue *l;
    Rvalue *r;
public:
    mov(mem m, mem me) : l(&m), r(&me) {}
    mov(mem m, num nu) : l(&m), r(&nu) {}
    mov(mem m, lea le) : l(&m), r(&le) {}

    void execute(Processor *proc) override;
};

//--------------------------------------------Arithmetic operations-------------------------------------------------------

class ArithmeticOperation : Instruction {
protected:
    void check_flags(Processor *proc, int64_t x) {
        proc->ZF = x == 0;
        proc->SF = x < 0;
    }
};

class add : ArithmeticOperation {
private:
    mem *l;
    Rvalue *r;
public:
    add(mem m, mem me) : l(&m), r(&me) {}
    add(mem m, num nu) : l(&m), r(&nu) {}
    add(mem m, lea le) : l(&m), r(&le) {}

    void execute(Processor *proc) override;
};

class sub : ArithmeticOperation {
private:
    mem *l;
    Rvalue *r;
public:
    sub(mem m, mem me) : l(&m), r(&me) {}
    sub(mem m, num nu) : l(&m), r(&nu) {}
    sub(mem m, lea le) : l(&m), r(&le) {}

    void execute(Processor *proc) override;
};

class inc : ArithmeticOperation {
private:
    mem *l;
public:
    inc(mem m) : l(&m) {}

    void execute(Processor *proc) override;
};

class dec : ArithmeticOperation {
private:
    mem *l;
public:
    dec(mem m) : l(&m) {}

    void execute(Processor *proc) override;
};

//--------------------------------------------ones-------------------------------------------------------

class one : Instruction {
private:
    mem *l;
public:
    one(mem m) : l(&m) {}

    void execute(Processor *proc) override;

};

class onez : Instruction {
private:
    mem *l;
public:
    onez(mem m) : l(&m) {}

    void execute(Processor *proc) override;

};

class ones : Instruction {
private:
    mem *l;
public:
    ones(mem m) : l(&m) {}

    void execute(Processor *proc) override;

};




int64_t Rvalue::get_value(Processor *proc) {return 0;}

void Lvalue::set_value(Processor *proc, int64_t new_val) {}

int64_t num::get_value(Processor *proc) {return x;}

int64_t lea::get_value(Processor *proc) {
    for (size_t i = 0; i < proc->N; ++i) {
        if (*id == *proc->labels[i])
            return i;
    }

    //@TODO: Wyrzucamy jakiś wyjątek IDnotFound

    return proc->N;
}


void Instruction::execute(Processor *proc) {}

void data::execute(Processor *proc) {
    if (proc->abused_memory < proc->N) {
        proc->tab[proc->abused_memory] = n.get_value(proc);
        proc->labels[proc->abused_memory] = id;
        proc->abused_memory++;
    }
    else {
        //@TODO: Zgłaszamy jakiś OutOfBoundsException
    }
}

void mov::execute(Processor *proc) {l->set_value(proc, r->get_value(proc));}

void add::execute(Processor *proc) {
    int64_t x = l->get_value(proc) + r->get_value(proc);
    l->set_value(proc, x);
    check_flags(proc, x);
}

void sub::execute(Processor *proc) {
    int64_t x = l->get_value(proc) - r->get_value(proc);
    l->set_value(proc, x);
    check_flags(proc, x);
}

void inc::execute(Processor *proc) {
    int64_t x = l->get_value(proc) + 1;
    l->set_value(proc, x);
    check_flags(proc, x);
}

void dec::execute(Processor *proc) {
    int64_t x = l->get_value(proc) - 1;
    l->set_value(proc, x);
    check_flags(proc, x);
}

void one::execute(Processor *proc) {
    l->set_value(proc, 1);
}

void onez::execute(Processor *proc) {
    if (proc->ZF == 1)
        l->set_value(proc, 1);
}

void ones::execute(Processor *proc) {
    if (proc->SF == 1)
        l->set_value(proc, 1);
}

#endif //ZADANIE6_OOASM_H
