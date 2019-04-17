
#include "Auto.c"

struct Nodo {
    struct Nodo* sig;
    struct Auto* ptrObj;
};

struct Cola {
    struct Nodo* ppio;
};
