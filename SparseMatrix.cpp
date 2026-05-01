#include "SparseMatrix.h"
#include <iostream>
#include <string>

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

void SparseMatrix::insert(int row, char col, std::string value)
{
    Node *newNode = new Node(row, col, value);
    Node *curr = headerRow[row];
    Node *prev = NULL;
    //* Para filas
    if (headerRow[row] == NULL)
    {
        headerRow[row] = newNode;
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
            newNode->nextInRow = curr;
            headerRow[row] = newNode;
        }
        else
        {
            prev->nextInRow = newNode;
            newNode->nextInRow = curr;
        }
    }
    //*Para columnas
    curr = headerCol[col - 'A'];
    prev = NULL;
    if (headerCol[col - 'A'] == NULL)
    {
        headerCol[col - 'A'] = newNode;
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
            newNode->nextInCol = curr;
            headerCol[col - 'A'] = newNode;
        }
        else
        {
            prev->nextInCol = newNode;
            newNode->nextInCol = curr;
        }
    }
}

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

void SparseMatrix::deleteCell(int row, char col)
{
    Node *curr = headerRow[row];
    Node *prev = NULL;

    while (curr != NULL)
    {
        if (curr->col == col)
        {
            Node *toDelete = curr;

            // desconecta de fila
            if (prev == NULL)
                headerRow[row] = curr->nextInRow;
            else
                prev->nextInRow = curr->nextInRow;

            // desconecta de columna
            Node *curr2 = headerCol[col - 'A'];
            Node *prev2 = NULL;
            while (curr2 != NULL)
            {
                if (curr2->row == row)
                {
                    if (prev2 == NULL)
                        headerCol[col - 'A'] = curr2->nextInCol;
                    else
                        prev2->nextInCol = curr2->nextInCol;
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

        Node *next = curr->nextInRow;
        deleteCell(curr->row, curr->col);
        curr = next;
    }
}

