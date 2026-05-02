#pragma once
#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "SparseMatrix.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

private slots:
    void onInsert();                      // Inserta o actualiza una celda con el valor ingresado
    void onQuery();                       // Consulta y muestra el valor de una celda
    void onDelete();                      // Elimina una celda específica
    void onDeleteRow();                   // Elimina todos los nodos de una fila
    void onDeleteCol();                   // Elimina todos los nodos de una columna
    void onDeleteRange();                 // Elimina todas las celdas en el rango dado
    void onSum();                         // Muestra la suma numérica del rango
    void onAverage();                     // Muestra el promedio numérico del rango
    void onMax();                         // Muestra el máximo numérico del rango
    void onMin();                         // Muestra el mínimo numérico del rango
    void onCellChanged(int row, int col); // Sincroniza edición directa en la tabla con la matriz

private:
    SparseMatrix sm;        // Estructura de datos principal (matriz dispersa)
    QTableWidget* table;    // Tabla visual que refleja el estado de la matriz
    QLineEdit* inputRow;    // Campo de entrada para la fila inicio
    QLineEdit* inputRowEnd; // Campo de entrada para la fila fin (operaciones de rango)
    QLineEdit* inputCol;    // Campo de entrada para la columna inicio
    QLineEdit* inputColEnd; // Campo de entrada para la columna fin (operaciones de rango)
    QLineEdit* inputValue;  // Campo de entrada para el valor de la celda
    void refreshTable();    // Redibuja la tabla completa desde el estado de la matriz dispersa
};
