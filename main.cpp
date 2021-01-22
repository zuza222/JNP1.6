#include <iostream>
#include "ooasm.h"

void pomocnicza(Processor *proc, Rvalue *r) {
    std::cout << r->get_value(proc) << std::endl;
}

int main() {
    Processor proc(100);

    mem m(num(0));
    m.set_value(&proc, 2137);

    mem m2(lea("WTC"));

    mem m3(mem(num(0)))

    pomocnicza(&proc, &m2);

}
