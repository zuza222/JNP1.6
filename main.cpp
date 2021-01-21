//
// Created by zuza on 21.01.2021.
//

#include <iostream>

class Rvalue;  //Nadklasa p-wartości
class Lvalue;  //Nadklasa l-wartości
class num;
class lea;
class mem;

class Processor {
private:
    size_t N;
    int64_t* tab;
    char **labels;

    friend mem;
    friend lea;

public:
    Processor(size_t _N) : N(_N), tab(new int64_t[_N]), labels(new char*[_N]) {
        //@TODO: Tych przypisań ma nie być w docelowym kodzie (służyły to testowania :-P)
        labels[0] = "Papaj II";
        labels[1] = "WTC";

        tab[1] = 911;
    }
};

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
    mem(mem &m) : Lvalue(m) {}

    int64_t get_value(Processor *proc);

    void set_value(Processor *proc, int64_t new_val);
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
    proc->tab[address] = new_val;
}


void pomocnicza(Processor *proc, Rvalue *r) {
    std::cout << r->get_value(proc) << std::endl;
}

int main() {
    Processor proc(100);

    mem m(num(0));
    m.set_value(&proc, 2137);

    mem m2(lea("WTC"));

    pomocnicza(&proc, &m2);

}