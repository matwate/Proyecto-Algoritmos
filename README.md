# Proyecto de Algoritmos

Visualizador de algoritmos de ordenamiento

## Instalacion

### Windows

Este proyecto usa [raylib](https://www.raylib.com/) para dibujar en pantalla, asi que asegurese de tenerlo instalado.
Despues clone el repositorio

```sh
git clone https://github.com/matwate/Proyecto-Algoritmos.git
```

Despues de clonarlo encontrara un archivo `main.code-workspace`, abralo en vscode y oprima el boton `open workspace`

Una vez eso ya puede ejecutar el programa con `F5`

### Linux/MacOS

1. Clona el repositorio y navega al directorio principal:
    ```bash
    git clone <url-del-repositorio>
    cd visualizador-algoritmos
    ```

2. Compila el proyecto utilizando **Makefile**:
    ```bash
    make
    ```

3. Ejecuta la aplicación:
    ```bash
    make run
    ```

---

## Caracteristicas

- **Visualización algoritmos de ordenamiento**:

  - Insertion Sort
  - Selection Sort
  - Bubble Sort
  - Quick Sort
  - Merge Sort
  - Heap Sort
  - Radix LSD Sort
  - Bogo Sort.

  Y se puede cambiar el tamaño del arreglo sobre el cual Bogo Sort trabaja.

- **Visualización de árboles**: Inserciones, recorridos (inorden, preorden, posorden), y balanceo.
- **Exploración de grafos**: BFS, DFS, Dijkstra, y generación de grafos aleatorios.

---

## Demos

### Algoritmos de Ordenamiento

#### Bubble Sort
![Bubble Sort](demos/sort/bubble.gif)

#### Heap Sort
![Heap Sort](demos/sort/heap.gif)

#### Insertion Sort
![Insertion Sort](demos/sort/insertion.gif)

#### Quick Sort
![Quick Sort](demos/sort/quick.gif)

#### Radix Sort (LSD)
![Radix Sort](demos/sort/radix.gif)

#### Selection Sort
![Selection Sort](demos/sort/selection.gif)

---

### Visualización de Árboles

#### Inserción de Nodo Aleatorio
![Inserción de Nodo Aleatorio](demos/trees/random_node_tree.gif)

#### Recorrido Inorden
![Recorrido Inorden](demos/trees/inorder.gif)

#### Recorrido Preorden
![Recorrido Preorden](demos/trees/preorder.gif)

#### Recorrido Posorden
![Recorrido Posorden](demos/trees/posorder.gif)

#### Balanceo del Árbol
![Balanceo del Árbol](demos/trees/balance_tree.gif)

---

### Visualización de Grafos

#### Búsqueda BFS
![Búsqueda BFS](demos/trees/bfs.gif)

#### Búsqueda DFS
![Búsqueda DFS](demos/trees/dfs.gif)

#### Algoritmo de Dijkstra
![Algoritmo de Dijkstra](demos/trees/djikstra.gif)

#### Generación de Grafo Aleatorio
![Generación de Grafo Aleatorio](demos/trees/random_graph.gif)

#### Creación de Grafo Personalizado
![Creación de Grafo Personalizado](demos/trees/custom_graph.gif)

---

## Contributing

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License

[MIT](https://choosealicense.com/licenses/mit/)
