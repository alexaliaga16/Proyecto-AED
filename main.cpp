#include "SparseMatrix.h"
#include <iostream>

int main() {
    SparseMatrix sm;
    sm.insert(3,'B',"Hola");
    sm.insert(1, 'G', "kanye");
    sm.insert(1,'A', "Mundo");
    sm.insert(1,'F', "wire");
    sm.insert(1,'E', "ghsot");
    sm.insert(1,'V', "town");
    sm.insert(6,'H', "desde matrix");
    sm.print();
    std::cout << "--------------------------------------" << std::endl;
    std::cout << sm.query(1, 'A') << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    sm.modify(3,'B', "C++");
    std::cout << "--------------------------------------" << std::endl;
    sm.print();
    sm.deleteCell(1,'A');
    std::cout << "--------------------------------------" << std::endl;
    sm.deleteCell(6,'D');
    std::cout << "--------------------------------------" << std::endl;
    sm.print();
    std::cout << "--------------------------------------" << std::endl;
    sm.deleteRow(1);
    sm.print();
    return 0;
}