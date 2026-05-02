#include <string>

struct Node
{
    int row;           // Número de fila de la celda (1-based)
    char col;          // Letra de columna ('A'-'Z')
    std::string value; // Contenido de la celda como texto

    Node *nextInRow;   // Siguiente nodo en la misma fila, ordenado por columna
    Node *nextInCol;   // Siguiente nodo en la misma columna, ordenado por fila

    // Inicializa el nodo en posición (1, 'A') con valor vacío
    Node()
    {
        row = 1;
        col = 'A';
        value = "";
        nextInRow = NULL;
        nextInCol = NULL;
    }

    // Inicializa el nodo con la posición y valor dados
    Node(int r, char c, std::string v)
    {
        row = r;
        col = c;
        value = v;
        nextInRow = NULL;
        nextInCol = NULL;
    }
};
