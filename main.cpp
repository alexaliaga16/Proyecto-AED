#include "SparseMatrix.h"
#include <iostream>

int main() {
    SparseMatrix sm;
    sm.insert(3,'B',"Hola");
    sm.insert(1,'A', "Mundo");
    sm.insert(6,'D', "desde matrix");
    sm.print();
    std::cout << sm.query(1, 'A') << std::endl;
    return 0;
}