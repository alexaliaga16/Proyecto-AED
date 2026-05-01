#include "Node.h"
#include <string>

const int MAX_ROWS = 100;
const int MAX_COLS = 26;

class SparseMatrix
{
public:
    Node* headerRow[MAX_ROWS];
    Node* headerCol[MAX_COLS];

    SparseMatrix();

    void insert(int row, char col, std::string value);

    void print();

    std::string query(int row, char col);
};