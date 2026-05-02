#include "SparseMatrix.h"
#include <iostream>
#include <string>

// Inicializa todos los encabezados de filas y columnas en NULL
SparseMatrix::SparseMatrix()
{
    for (int i = 0; i < MAX_ROWS; i++)
    {
        headerRow[i] = NULL;
    }
    for (int i = 0; i < MAX_COLS; i++)
    {
        headerCol[i] = NULL;
    }
}

// Inserta un nodo en (row, col) manteniendo orden ascendente; si ya existe, actualiza su valor
void SparseMatrix::insert(int row, char col, std::string value)
{
    // Si la celda ya existe en la fila, solo actualiza el valor y sale
    Node* curr = headerRow[row];
    while (curr != NULL) {
        if (curr->col == col) {
            curr->value = value;
            return;
        }
        curr = curr->nextInRow;
    }

    Node *newNode = new Node(row, col, value);
    curr = headerRow[row];
    Node *prev = NULL;

    // Inserción ordenada por columna en la lista de la fila
    if (headerRow[row] == NULL)
    {
        headerRow[row] = newNode; // primer nodo en la fila
    }
    else
    {
        while (curr != NULL && curr->col < col)
        {
            prev = curr;
            curr = curr->nextInRow;
        }
        if (prev == NULL)
        {
            // nuevo nodo va al inicio de la fila
            newNode->nextInRow = curr;
            headerRow[row] = newNode;
        }
        else
        {
            // inserción entre prev y curr
            prev->nextInRow = newNode;
            newNode->nextInRow = curr;
        }
    }

    // Inserción ordenada por fila en la lista de la columna
    curr = headerCol[col - 'A'];
    prev = NULL;
    if (headerCol[col - 'A'] == NULL)
    {
        headerCol[col - 'A'] = newNode; // primer nodo en la columna
    }
    else
    {
        while (curr != NULL && curr->row < row)
        {
            prev = curr;
            curr = curr->nextInCol;
        }
        if (prev == NULL)
        {
            // nuevo nodo va al inicio de la columna
            newNode->nextInCol = curr;
            headerCol[col - 'A'] = newNode;
        }
        else
        {
            // inserción entre prev y curr
            prev->nextInCol = newNode;
            newNode->nextInCol = curr;
        }
    }
}

// Imprime todas las filas con al menos un nodo, con su columna y valor
void SparseMatrix::print()
{
    for (int i = 0; i < MAX_ROWS; i++)
    {
        if (headerRow[i] != NULL)
        {
            std::cout << "Fila " << i << ": ";
            Node *curr = headerRow[i];
            while (curr != NULL)
            {
                std::cout << "(" << curr->col << ", " << curr->value << ") ";
                curr = curr->nextInRow;
            }
            std::cout << std::endl;
        }
    }
}

// Busca el valor en (row, col) recorriendo la lista de la fila
std::string SparseMatrix::query(int row, char col)
{
    Node *curr = headerRow[row];
    while (curr != NULL)
    {
        if (curr->col == col)
        {
            return "Valor de celda: " + curr->value;
        }
        else
        {
            curr = curr->nextInRow;
        }
    }
    return "Celda Vacia";
}

// Modifica el valor en (row, col); si la celda no existe, la inserta
void SparseMatrix::modify(int row, char col, std::string newValue)
{
    Node *curr = headerRow[row];
    while (curr != NULL)
    {
        if (curr->row == row && curr->col == col)
        {
            curr->value = newValue;
            std::cout << "Valor cambiado exitosamente en (" << row << ", " << col << ")" << std::endl;
            return;
        }
        else
        {
            curr = curr->nextInRow;
        }
    }
    std::cout << "Celda vacía, insertando valor..." << std::endl;
    insert(row, col, newValue);
}

// Elimina el nodo en (row, col) desconectándolo de la lista de su fila y de su columna
void SparseMatrix::deleteCell(int row, char col)
{
    Node *curr = headerRow[row];
    Node *prev = NULL;

    while (curr != NULL)
    {
        if (curr->col == col)
        {
            Node *toDelete = curr;

            // Desconecta el nodo de la lista de su fila
            if (prev == NULL)
                headerRow[row] = curr->nextInRow; // era el primer nodo de la fila
            else
                prev->nextInRow = curr->nextInRow; // bypass del nodo a eliminar

            // Desconecta el nodo de la lista de su columna
            Node *curr2 = headerCol[col - 'A'];
            Node *prev2 = NULL;
            while (curr2 != NULL)
            {
                if (curr2->row == row)
                {
                    if (prev2 == NULL)
                        headerCol[col - 'A'] = curr2->nextInCol; // era el primer nodo de la columna
                    else
                        prev2->nextInCol = curr2->nextInCol; // bypass del nodo a eliminar
                    break;
                }
                prev2 = curr2;
                curr2 = curr2->nextInCol;
            }

            delete toDelete;
            return;
        }
        else
        {
            prev = curr;
            curr = curr->nextInRow;
        }
    }
    std::cout << "ERROR: Celda no encontrada" << std::endl;
}

// Elimina todos los nodos de una fila iterando sobre su lista enlazada
void SparseMatrix::deleteRow(int row)
{
    if (headerRow[row] == NULL)
    {
        std::cout << "ERROR: Fila vacia" << std::endl;
        return;
    }
    Node *curr = headerRow[row];
    while (curr != NULL)
    {
        Node *next = curr->nextInRow; // guarda el siguiente antes de eliminar
        deleteCell(curr->row, curr->col);
        curr = next;
    }
}

// Elimina todos los nodos de una columna iterando sobre su lista enlazada
void SparseMatrix::deleteCol(char col)
{
    if (headerCol[col - 'A'] == NULL)
    {
        std::cout << "ERROR: Columna Vacia" << std::endl;
        return;
    }
    Node* curr = headerCol[col-'A'];
    while (curr != NULL)
    {
        Node *next = curr->nextInCol; // guarda el siguiente antes de eliminar
        deleteCell(curr->row, curr->col);
        curr = next;
    }
}

// Elimina todas las celdas dentro del rango rectangular [rowStart..rowEnd, colStart..colEnd]
void SparseMatrix::deleteRange(int rowStart, int rowEnd, char colStart, char colEnd)
{
    for (int i = rowStart; i <= rowEnd; i++)
        for (char c = colStart; c <= colEnd; c++)
            deleteCell(i, c);
}

// Suma los valores numéricos del rango; las celdas con texto no numérico se ignoran
double SparseMatrix::sum(int rowStart, int rowEnd, char colStart, char colEnd)
{
    double total = 0;
    for (int i = rowStart; i <= rowEnd; i++)
    {
        Node* curr = headerRow[i];
        while (curr != NULL)
        {
            if (curr->col >= colStart && curr->col <= colEnd)
            {
                try { total += std::stod(curr->value); }
                catch (...) {} // ignora celdas con valor no numérico
            }
            curr = curr->nextInRow;
        }
    }
    return total;
}

// Calcula el promedio de valores numéricos en el rango; retorna 0 si no hay ninguno
double SparseMatrix::average(int rowStart, int rowEnd, char colStart, char colEnd)
{
    double total = 0;
    int count = 0;
    for (int i = rowStart; i <= rowEnd; i++)
    {
        Node* curr = headerRow[i];
        while (curr != NULL)
        {
            if (curr->col >= colStart && curr->col <= colEnd)
            {
                try { total += std::stod(curr->value); count++; }
                catch (...) {} // ignora celdas con valor no numérico
            }
            curr = curr->nextInRow;
        }
    }
    if (count == 0) { std::cout << "ERROR: No hay valores numéricos" << std::endl; return 0; }
    return total / count;
}

// Retorna el máximo valor numérico en el rango; retorna 0 si no hay valores numéricos
double SparseMatrix::maxVal(int rowStart, int rowEnd, char colStart, char colEnd)
{
    double result = -1e9;
    bool found = false;
    for (int i = rowStart; i <= rowEnd; i++)
    {
        Node* curr = headerRow[i];
        while (curr != NULL)
        {
            if (curr->col >= colStart && curr->col <= colEnd)
            {
                try {
                    double val = std::stod(curr->value);
                    if (val > result) { result = val; found = true; }
                }
                catch (...) {} // ignora celdas no numéricas
            }
            curr = curr->nextInRow;
        }
    }
    if (!found) { std::cout << "ERROR: No hay valores numéricos" << std::endl; return 0; }
    return result;
}

// Retorna el mínimo valor numérico en el rango; retorna 0 si no hay valores numéricos
double SparseMatrix::minVal(int rowStart, int rowEnd, char colStart, char colEnd)
{
    double result = 1e9;
    bool found = false;
    for (int i = rowStart; i <= rowEnd; i++)
    {
        Node* curr = headerRow[i];
        while (curr != NULL)
        {
            if (curr->col >= colStart && curr->col <= colEnd)
            {
                try {
                    double val = std::stod(curr->value);
                    if (val < result) { result = val; found = true; }
                }
                catch (...) {} // ignora celdas no numéricas
            }
            curr = curr->nextInRow;
        }
    }
    if (!found) { std::cout << "ERROR: No hay valores numéricos" << std::endl; return 0; }
    return result;
}

// Imprime los nodos de una fila en orden ascendente de columna
void SparseMatrix::printRow(int row)
{
    if (headerRow[row] == NULL) { std::cout << "Fila vacía" << std::endl; return; }
    Node* curr = headerRow[row];
    while (curr != NULL)
    {
        std::cout << "(" << curr->col << ", " << curr->value << ") ";
        curr = curr->nextInRow;
    }
    std::cout << std::endl;
}

// Imprime los nodos de una columna en orden ascendente de fila
void SparseMatrix::printCol(char col)
{
    if (headerCol[col - 'A'] == NULL) { std::cout << "Columna vacía" << std::endl; return; }
    Node* curr = headerCol[col - 'A'];
    while (curr != NULL)
    {
        std::cout << "(" << curr->row << ", " << curr->value << ") ";
        curr = curr->nextInCol;
    }
    std::cout << std::endl;
}
