# Hoja de Cálculo — Matriz Dispersa con Listas Enlazadas Cruzadas

Aplicación de escritorio en C++ con interfaz Qt6 que simula una hoja de cálculo. Internamente almacena las celdas usando una **matriz dispersa** con listas enlazadas cruzadas, evitando reservar memoria para celdas vacías.

---

## Compilación

**Requisitos:** CMake ≥ 3.16, Qt6, compilador C++17.

```bash
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/ruta/a/Qt6
cmake --build .
```

## Ejecución

```bash
./build/main
```

---

## Estructura de archivos

```
├── main.cpp           # Punto de entrada Qt
├── MainWindow.h/cpp   # Interfaz gráfica (slots, tabla visual)
├── SparseMatrix.h/cpp # Lógica de la matriz dispersa
├── Node.h             # Nodo de la lista enlazada cruzada
└── CMakeLists.txt
```

---

## Justificación de la estructura de datos

Una **matriz densa** de 100×26 reservaría 2 600 celdas en memoria aunque la hoja esté casi vacía. La lista enlazada cruzada solo crea nodos para celdas con valor, lo que la hace eficiente en memoria cuando la ocupación es baja (caso típico en hojas de cálculo).

Cada nodo almacena punteros `nextInRow` y `nextInCol`, manteniendo dos listas ordenadas independientes. Esto permite recorrer una fila o una columna completa sin iterar sobre toda la estructura, lo que es clave para las operaciones de rango (`sum`, `deleteRow`, etc.).

---

## Complejidad temporal

| Operación     | Complejidad | Justificación |
|---------------|-------------|---------------|
| `insert`      | O(R + C)    | Búsqueda de posición en lista de fila (R nodos) y de columna (C nodos) |
| `query`       | O(R)        | Recorre la lista de la fila hasta encontrar la columna |
| `deleteCell`  | O(R + C)    | Desconecta de la lista de fila y luego de la lista de columna |
| `deleteRow`   | O(R · C)    | Llama a `deleteCell` por cada nodo de la fila |
| `sum`         | O(F · R)    | Recorre todas las filas del rango nodo a nodo |
| `average`     | O(F · R)    | Igual que `sum` más división final |
| `max` / `min` | O(F · R)    | Recorrido completo del rango para comparar valores |

> R = nodos en la fila, C = nodos en la columna, F = filas del rango.
