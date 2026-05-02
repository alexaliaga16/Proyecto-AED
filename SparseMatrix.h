#include "Node.h"
#include <string>

const int MAX_ROWS = 100;
const int MAX_COLS = 26;

class SparseMatrix
{
public:
    Node *headerRow[MAX_ROWS]; // Cabecera de lista enlazada para cada fila
    Node *headerCol[MAX_COLS]; // Cabecera de lista enlazada para cada columna (índice = col - 'A')

    SparseMatrix();

    // Inserta o actualiza el valor en la celda (row, col)
    void insert(int row, char col, std::string value);

    // Imprime todas las filas no vacías con sus celdas
    void print();

    // Retorna el valor de la celda (row, col) o indica que está vacía
    std::string query(int row, char col);

    // Modifica el valor en (row, col); si no existe, inserta la celda
    void modify(int row, char col, std::string newValue);

    // Elimina la celda en (row, col)
    void deleteCell(int row, char col);

    // Elimina todas las celdas de una fila
    void deleteRow(int row);

    // Elimina todas las celdas de una columna
    void deleteCol(char col);

    // Elimina todas las celdas dentro del rango rectangular dado
    void deleteRange(int rowStart, int rowEnd, char colStart, char colEnd);

    // Suma los valores numéricos en el rango; ignora celdas no numéricas
    double sum(int rowStart, int rowEnd, char colStart, char colEnd);

    // Calcula el promedio de valores numéricos en el rango
    double average(int rowStart, int rowEnd, char colStart, char colEnd);

    // Retorna el valor máximo numérico en el rango
    double maxVal(int rowStart, int rowEnd, char colStart, char colEnd);

    // Retorna el valor mínimo numérico en el rango
    double minVal(int rowStart, int rowEnd, char colStart, char colEnd);

    // Imprime los nodos de una fila en orden de columna
    void printRow(int row);

    // Imprime los nodos de una columna en orden de fila
    void printCol(char col);
};
