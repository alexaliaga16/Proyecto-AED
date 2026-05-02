#include "MainWindow.h"
#include <QApplication>
#include <QMessageBox>
#include <iostream>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Hoja de Cálculo - Sparse Matrix");
    resize(900, 650);

    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    // Tabla de 20 filas x 10 columnas (A-J)
    table = new QTableWidget(20, 10, this);
    QStringList headers;
    for (int i = 0; i < 10; i++)
        headers << QString(QChar('A' + i));
    table->setHorizontalHeaderLabels(headers);
    mainLayout->addWidget(table);

    // Inputs de fila, columna, rango y valor
    QHBoxLayout* inputLayout = new QHBoxLayout();
    inputRow = new QLineEdit(); inputRow->setPlaceholderText("Fila");
    inputCol = new QLineEdit(); inputCol->setPlaceholderText("Col");
    inputRowEnd = new QLineEdit(); inputRowEnd->setPlaceholderText("Fila fin (rango)");
    inputColEnd = new QLineEdit(); inputColEnd->setPlaceholderText("Col fin (rango)");
    inputValue = new QLineEdit(); inputValue->setPlaceholderText("Valor");
    inputLayout->addWidget(inputRow);
    inputLayout->addWidget(inputCol);
    inputLayout->addWidget(inputRowEnd);
    inputLayout->addWidget(inputColEnd);
    inputLayout->addWidget(inputValue);
    mainLayout->addLayout(inputLayout);

    // Botones fila 1
    QHBoxLayout* btnLayout1 = new QHBoxLayout();
    QPushButton* btnInsert = new QPushButton("Insertar");
    QPushButton* btnQuery = new QPushButton("Consultar");
    QPushButton* btnDelete = new QPushButton("Eliminar Celda");
    QPushButton* btnDeleteRow = new QPushButton("Eliminar Fila");
    QPushButton* btnDeleteCol = new QPushButton("Eliminar Columna");
    btnLayout1->addWidget(btnInsert);
    btnLayout1->addWidget(btnQuery);
    btnLayout1->addWidget(btnDelete);
    btnLayout1->addWidget(btnDeleteRow);
    btnLayout1->addWidget(btnDeleteCol);
    mainLayout->addLayout(btnLayout1);

    // Botones fila 2
    QHBoxLayout* btnLayout2 = new QHBoxLayout();
    QPushButton* btnDeleteRange = new QPushButton("Eliminar Rango");
    QPushButton* btnSum = new QPushButton("Suma Rango");
    QPushButton* btnAvg = new QPushButton("Promedio");
    QPushButton* btnMax = new QPushButton("Máximo");
    QPushButton* btnMin = new QPushButton("Mínimo");
    btnLayout2->addWidget(btnDeleteRange);
    btnLayout2->addWidget(btnSum);
    btnLayout2->addWidget(btnAvg);
    btnLayout2->addWidget(btnMax);
    btnLayout2->addWidget(btnMin);
    mainLayout->addLayout(btnLayout2);

    connect(btnInsert, &QPushButton::clicked, this, &MainWindow::onInsert, Qt::UniqueConnection);
    connect(btnQuery, &QPushButton::clicked, this, &MainWindow::onQuery, Qt::UniqueConnection);
    connect(btnDelete, &QPushButton::clicked, this, &MainWindow::onDelete, Qt::UniqueConnection);
    connect(btnDeleteRow, &QPushButton::clicked, this, &MainWindow::onDeleteRow, Qt::UniqueConnection);
    connect(btnDeleteCol, &QPushButton::clicked, this, &MainWindow::onDeleteCol, Qt::UniqueConnection);
    connect(btnDeleteRange, &QPushButton::clicked, this, &MainWindow::onDeleteRange, Qt::UniqueConnection);
    connect(btnSum, &QPushButton::clicked, this, &MainWindow::onSum, Qt::UniqueConnection);
    connect(btnAvg, &QPushButton::clicked, this, &MainWindow::onAverage, Qt::UniqueConnection);
    connect(btnMax, &QPushButton::clicked, this, &MainWindow::onMax, Qt::UniqueConnection);
    connect(btnMin, &QPushButton::clicked, this, &MainWindow::onMin, Qt::UniqueConnection);
    connect(table, &QTableWidget::cellChanged, this, &MainWindow::onCellChanged, Qt::UniqueConnection);
}

// Sincroniza la tabla visual completa con el estado actual de la matriz dispersa
void MainWindow::refreshTable() {
    table->blockSignals(true); // evita disparar onCellChanged durante el redibujado
    table->clearContents();
    for (int i = 0; i < 20; i++) {
        Node* curr = sm.headerRow[i];
        while (curr != NULL) {
            int col = curr->col - 'A';
            table->setItem(i - 1, col, new QTableWidgetItem(QString::fromStdString(curr->value)));
            curr = curr->nextInRow;
        }
    }
    table->blockSignals(false);
}

// Actualiza la matriz cuando el usuario edita una celda directamente en la tabla
void MainWindow::onCellChanged(int row, int col) {
    QTableWidgetItem* item = table->item(row, col);
    if (item && !item->text().isEmpty()) {
        char colChar = 'A' + col;
        sm.insert(row + 1, colChar, item->text().toStdString()); // fila 1-based
    }
}

// Valida los campos y llama a sm.insert() con los valores del formulario
void MainWindow::onInsert() {
    if (inputRow->text().isEmpty() || inputCol->text().isEmpty() || inputValue->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Fila, columna y valor son obligatorios");
        return;
    }
    int row = inputRow->text().toInt();
    if (row < 1 || row > 20) {
        QMessageBox::warning(this, "Error", "Fila debe estar entre 1 y 20");
        return;
    }
    char col = inputCol->text().toUpper()[0].toLatin1();
    if (col < 'A' || col > 'J') {
        QMessageBox::warning(this, "Error", "Columna debe estar entre A y J");
        return;
    }
    std::string val = inputValue->text().toStdString();
    sm.insert(row, col, val);
    refreshTable();
    QMessageBox::information(this, "Éxito", "Celda insertada correctamente");
}

// Consulta el valor de la celda indicada y lo muestra en un cuadro de diálogo
void MainWindow::onQuery() {
    if (inputRow->text().isEmpty() || inputCol->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Ingresa fila y columna");
        return;
    }
    int row = inputRow->text().toInt();
    char col = inputCol->text().toUpper()[0].toLatin1();
    std::string result = sm.query(row, col);
    QMessageBox::information(this, "Consulta", QString::fromStdString(result));
}

// Elimina la celda indicada si existe; muestra error si está vacía
void MainWindow::onDelete() {
    if (inputRow->text().isEmpty() || inputCol->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Ingresa fila y columna");
        return;
    }
    int row = inputRow->text().toInt();
    char col = inputCol->text().toUpper()[0].toLatin1();
    std::string result = sm.query(row, col);
    if (result == "Celda Vacia") {
        QMessageBox::warning(this, "Error", "La celda no existe");
        return;
    }
    sm.deleteCell(row, col);
    refreshTable();
    QMessageBox::information(this, "Éxito", "Celda eliminada correctamente");
}

// Elimina la fila indicada si tiene al menos un nodo
void MainWindow::onDeleteRow() {
    if (inputRow->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Ingresa una fila");
        return;
    }
    int row = inputRow->text().toInt();
    if (sm.headerRow[row] == NULL) {
        QMessageBox::warning(this, "Error", "La fila está vacía");
        return;
    }
    sm.deleteRow(row);
    refreshTable();
    QMessageBox::information(this, "Éxito", "Fila eliminada correctamente");
}

// Elimina la columna indicada si tiene al menos un nodo
void MainWindow::onDeleteCol() {
    if (inputCol->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Ingresa una columna");
        return;
    }
    char col = inputCol->text().toUpper()[0].toLatin1();
    if (sm.headerCol[col - 'A'] == NULL) {
        QMessageBox::warning(this, "Error", "La columna está vacía");
        return;
    }
    sm.deleteCol(col);
    refreshTable();
    QMessageBox::information(this, "Éxito", "Columna eliminada correctamente");
}

// Elimina todas las celdas en el rango rectangular definido por los cuatro campos de entrada
void MainWindow::onDeleteRange() {
    if (inputRow->text().isEmpty() || inputCol->text().isEmpty() ||
        inputRowEnd->text().isEmpty() || inputColEnd->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Ingresa fila inicio, col inicio, fila fin y col fin");
        return;
    }
    int rowStart = inputRow->text().toInt();
    int rowEnd = inputRowEnd->text().toInt();
    char colStart = inputCol->text().toUpper()[0].toLatin1();
    char colEnd = inputColEnd->text().toUpper()[0].toLatin1();
    sm.deleteRange(rowStart, rowEnd, colStart, colEnd);
    refreshTable();
    QMessageBox::information(this, "Éxito", "Rango eliminado correctamente");
}

// Calcula y muestra la suma del rango; si fila/col fin están vacíos, usa solo la celda de inicio
void MainWindow::onSum() {
    if (inputRow->text().isEmpty() || inputCol->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Ingresa al menos fila y columna inicio");
        return;
    }
    int rowStart = inputRow->text().toInt();
    int rowEnd = inputRowEnd->text().isEmpty() ? rowStart : inputRowEnd->text().toInt();
    char colStart = inputCol->text().toUpper()[0].toLatin1();
    char colEnd = inputColEnd->text().isEmpty() ? colStart : inputColEnd->text().toUpper()[0].toLatin1();
    double result = sm.sum(rowStart, rowEnd, colStart, colEnd);
    if (result == 0)
        QMessageBox::warning(this, "Suma", "No hay valores numéricos en el rango o la suma es 0");
    else
        QMessageBox::information(this, "Suma", "Resultado: " + QString::number(result));
}

// Calcula y muestra el promedio del rango; si fila/col fin están vacíos, usa solo la celda de inicio
void MainWindow::onAverage() {
    if (inputRow->text().isEmpty() || inputCol->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Ingresa al menos fila y columna inicio");
        return;
    }
    int rowStart = inputRow->text().toInt();
    int rowEnd = inputRowEnd->text().isEmpty() ? rowStart : inputRowEnd->text().toInt();
    char colStart = inputCol->text().toUpper()[0].toLatin1();
    char colEnd = inputColEnd->text().isEmpty() ? colStart : inputColEnd->text().toUpper()[0].toLatin1();
    double result = sm.average(rowStart, rowEnd, colStart, colEnd);
    if (result == 0)
        QMessageBox::warning(this, "Promedio", "No hay valores numéricos en el rango");
    else
        QMessageBox::information(this, "Promedio", "Resultado: " + QString::number(result));
}

// Calcula y muestra el valor máximo del rango
void MainWindow::onMax() {
    if (inputRow->text().isEmpty() || inputCol->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Ingresa al menos fila y columna inicio");
        return;
    }
    int rowStart = inputRow->text().toInt();
    int rowEnd = inputRowEnd->text().isEmpty() ? rowStart : inputRowEnd->text().toInt();
    char colStart = inputCol->text().toUpper()[0].toLatin1();
    char colEnd = inputColEnd->text().isEmpty() ? colStart : inputColEnd->text().toUpper()[0].toLatin1();
    double result = sm.maxVal(rowStart, rowEnd, colStart, colEnd);
    QMessageBox::information(this, "Máximo", "Resultado: " + QString::number(result));
}

// Calcula y muestra el valor mínimo del rango
void MainWindow::onMin() {
    if (inputRow->text().isEmpty() || inputCol->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Ingresa al menos fila y columna inicio");
        return;
    }
    int rowStart = inputRow->text().toInt();
    int rowEnd = inputRowEnd->text().isEmpty() ? rowStart : inputRowEnd->text().toInt();
    char colStart = inputCol->text().toUpper()[0].toLatin1();
    char colEnd = inputColEnd->text().isEmpty() ? colStart : inputColEnd->text().toUpper()[0].toLatin1();
    double result = sm.minVal(rowStart, rowEnd, colStart, colEnd);
    QMessageBox::information(this, "Mínimo", "Resultado: " + QString::number(result));
}
