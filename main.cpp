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
class Rvalues : public virtual element_OOAsm {
public:
    virtual int64_t get_value() {return x;}
};

/*
//@TODO: To też powinna być klasa abstrakcyjna
//Nadklasa l-wartości
class Lvalues {
public:
    virtual int64_t get_address();
};
 */

class num : public Rvalues {
public:
    num(int64_t _x) {x = _x;}

    //int64_t get_number() /*override*/ {return x;}
};

class mem : public Rvalues {
public:
    mem(Rvalues r) {x = r.get_value(); }

    int64_t get_value() override {return tab[x];}

    //@TODO: To trzeba poprawić
    int64_t get_address() {return 2137;}
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

    num p = num(1);
    lea q = lea("WTC");
    mem m = mem(q);

    pomocnicza(q);
}
