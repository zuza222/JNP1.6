#include <iostream>

int N = 100;
int64_t tab[100];
char *labels[100];

//@TODO: To powinna być klasa abstrakcyjna
//Nadklasa p-wartości
class element_OOAsm {
protected:
    int64_t x;
};

//Nadklasa p-wartości
class Rvalues : public virtual element_OOAsm {
public:
    int64_t get_value() {return x;}
};


//@TODO: To też powinna być klasa abstrakcyjna
//Nadklasa l-wartości
class Lvalues : public virtual element_OOAsm {
protected:
    size_t address;
    Lvalues(size_t addr) : address(addr) {x = tab[address];}
public:
    void set_address(size_t new_addr) {
        address = new_addr;
        x = tab[new_addr];
    }
};

class num : public Rvalues {
public:
    num(int64_t _x) {x = _x;}
};

class mem : public Rvalues, public Lvalues {
public:
    mem(Rvalues r) : Lvalues(r.get_value()) {}
};

class lea : public Rvalues {
public:
    lea(char *id) {
        for (size_t i = 0; i < N; ++i)
            if (*labels[i] == *id) {
                x = i;
                break;
            }
    }
};

void pomocnicza(Rvalues r) {
    std::cout << r.get_value() << std::endl;
}

int main() {
    tab[0] = 2137;
    tab[1] = 911;

    labels[0] = "Papaj II";
    labels[1] = "WTC";

    lea q = lea("WTC");
    mem m = mem(q);

    pomocnicza(m);
}
