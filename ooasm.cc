//
// Created by stasiek on 23.01.2021.
//

#include "ooasm.h"

int64_t Num::get_value(Processor *proc) const {return x;}

int64_t Lea::get_value(Processor *proc) const {
    for (size_t i = 0; i < proc->N; ++i) {
        if (*id == *proc->labels[i])
            return i;
    }

    //@TODO: Wyrzucamy jakiś wyjątek IDnotFound

    return proc->N;
}


int64_t Mem::get_value(Processor *proc) const  {
    size_t address = rvalue->get_value(proc);

    if (address < proc->N) {
        return proc->tab[address];
    }
    else {
        //@TODO: Wyrzucamy jakiś wyjątek OutOfBoundsException
    }
    //FIXME: return
}

void Mem::set_value(Processor *proc, int64_t new_val) const {
    size_t address = rvalue->get_value(proc);
    if (address < proc->abused_memory) {
        proc->tab[address] = new_val;
    }
    else {
        //@TODO: Zgłaszamy jakiś OutOfBoundsException
    }
}


//@TODO: Coś trzeba tutaj napisać
void Data::execute(Processor *proc) const {

}

void Data::evaluate(Memory&, Flags&) const {

}

void Data::pre_evaluate(Memory&) const {

}


void Mov::execute(Processor *proc) const {

}

void Mov::evaluate(Memory&, Flags&) const {

}

void Mov::pre_evaluate(Memory&) const {

}


void Add::execute(Processor *proc) const {

}

void Add::evaluate(Memory&, Flags&) const {

}

void Add::pre_evaluate(Memory&) const {

}


void Sub::execute(Processor *proc) const {

}

void Sub::evaluate(Memory&, Flags&) const {

}

void Sub::pre_evaluate(Memory&) const {

}


void Inc::execute(Processor *proc) const {

}

void Inc::evaluate(Memory&, Flags&) const {

}

void Inc::pre_evaluate(Memory&) const {

}


void Dec::execute(Processor *proc) const {

}

void Dec::evaluate(Memory&, Flags&) const {

}

void Dec::pre_evaluate(Memory&) const {

}