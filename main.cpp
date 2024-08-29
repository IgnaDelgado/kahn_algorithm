#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Estructura de datos para almacenar una arista de un grafo
struct Edge {
    int src, dest;
};

// Una clase para representar un objeto de grafo
class Graph {
public:
    // un vector de vectores para representar una lista de adyacencia
    vector<vector<int>> adjList;

    // almacena el grado de entrada de un vértice
    vector<int> indegree;

    // Constructor del grafo
    Graph(vector<Edge> const& edges, int n) {
        // redimensiona el vector para contener `n` elementos de tipo `vector<int>`
        adjList.resize(n);

        // inicializa el grado de entrada
        vector<int> temp(n, 0);
        indegree = temp;

        // agrega aristas al grafo dirigido
        for (auto& edge : edges) {
            // agrega una arista desde la fuente hasta el destino
            adjList[edge.src].push_back(edge.dest);

            // incrementa el grado de entrada del vértice destino en 1
            indegree[edge.dest]++;
        }
    }
};

// Función para realizar un ordenamiento topológico en un DAG dado
vector<int> doTopologicalSort(Graph const& graph) {
    vector<int> L;

    // obtiene el número total de nodos en el grafo
    int n = graph.adjList.size();

    vector<int> indegree = graph.indegree;

    // Conjunto de todos los nodos sin aristas entrantes
    vector<int> S;
    for (int i = 0; i < n; i++) {
        if (!indegree[i]) {
            S.push_back(i);
        }
    }

    while (!S.empty()) {
        // elimina el nodo `n` de `S`
        int n = S.back();
        S.pop_back();

        // agrega `n` al final de `L`
        L.push_back(n);

        for (int m : graph.adjList[n]) {
            // elimina una arista de `n` a `m` del grafo
            indegree[m] -= 1;

            // si `m` no tiene otras aristas entrantes, inserta `m` en `S`
            if (!indegree[m]) {
                S.push_back(m);
            }
        }
    }

    // si un grafo tiene aristas, entonces el grafo tiene al menos un ciclo
    for (int i = 0; i < n; i++) {
        if (indegree[i]) {
            return {};
        }
    }

    return L;
}

// Función para agregar una nueva asignatura
void addSubject(vector<Edge>& subjects, string subjectName, vector<string>& subjectNames, int& totalSubjects) {
    int numDependencies;

    cout << "Ingrese el número de dependencias para la asignatura " << subjectName << ": ";
    cin >> numDependencies;

    // Agrega la nueva asignatura
    subjectNames.push_back(subjectName);
    totalSubjects++;

    // Agrega dependencias
    for (int j = 0; j < numDependencies; j++) {
        cout << "Ingrese la dependencia " << j + 1 << " para la asignatura " << subjectName << ": ";
        string dependency;
        cin >> dependency;
        // Encuentra el índice de la asignatura
        int k = 0;
        while (subjectNames[k] != dependency) {
            k++;
        }
        Edge edge = { k, totalSubjects - 1 };
        subjects.push_back(edge);
    }

    // Crea un nuevo grafo con las asignaturas actualizadas
    Graph graph(subjects, totalSubjects);
    vector<int> L = doTopologicalSort(graph);

    // Imprime el orden topológico
    cout << "El orden de las asignaturas es el siguiente: " << endl;
    if (L.size()) {
        for (int i : L) {
            cout << subjectNames[i] << " --> ";
        }
        cout << "Fin" << endl;
    }
    else {
        cout << "El orden topológico no es posible" << endl;
    }
}

int main() {
    // Inicialización de variables
    vector<Edge> subjects;
    int totalSubjects;

    cout << "Ingrese el número de asignaturas: ";
    cin >> totalSubjects;

    // Array dinámico para asignaturas
    vector<string> subjectNames(totalSubjects);

    for (int i = 0; i < totalSubjects; i++) {
        cout << "Ingrese el nombre de la asignatura " << i + 1 << ": ";
        cin >> subjectNames[i];
    }

    int numDependencies;

    // Inicializa las dependencias de las asignaturas
    for (int i = 0; i < totalSubjects; i++) {
        cout << "Ingrese el número de dependencias para la asignatura " << subjectNames[i] << ": ";
        cin >> numDependencies;
        for (int j = 0; j < numDependencies; j++) {
            cout << "Ingrese la dependencia " << j + 1 << " para la asignatura " << subjectNames[i] << ": ";
            string dependency;
            cin >> dependency;
            // Encuentra el índice de la asignatura
            int k = 0;
            while (subjectNames[k] != dependency) {
                k++;
            }
            Edge edge = { k, i };
            subjects.push_back(edge);
        }
    }

    // Inicializa el grafo
    Graph graph(subjects, totalSubjects);

    // Realiza el ordenamiento topológico
    vector<int> L = doTopologicalSort(graph);

    // Imprime el orden topológico
    cout << "El orden de las asignaturas es el siguiente: " << endl;
    if (L.size()) {
        for (int i : L) {
            cout << subjectNames[i] << " --> ";
        }
        cout << "Fin" << endl;
    }
    else {
        cout << "El orden topológico no es posible" << endl;
    }

    // Opción para agregar y eliminar asignaturas
    int option;

    while (true) {
        cout << "Ingrese 1 para agregar una asignatura, 2 para eliminar una asignatura (0 para salir): ";
        cin >> option;

        if (option == 1) {
            string subjectName;
            cout << "Ingrese el nombre de la asignatura: ";
            cin >> subjectName;
            addSubject(subjects, subjectName, subjectNames, totalSubjects);
        }
        else if (option == 2) {
            string subjectName;
            cout << "Ingrese el nombre de la asignatura para eliminar: ";
            cin >> subjectName;
            // Encuentra el índice de la asignatura
            int k = 0;
            while (subjectNames[k] != subjectName) {
                k++;
            }
            subjectNames.erase(subjectNames.begin() + k);
            totalSubjects--;
            // Reconstruye el grafo después de eliminar la asignatura
            vector<Edge> updatedSubjects;
            for (Edge e : subjects) {
                if (e.src != k && e.dest != k) {
                    // Ajusta los índices para la lista de asignaturas actualizada
                    if (e.src > k) e.src--;
                    if (e.dest > k) e.dest--;
                    updatedSubjects.push_back(e);
                }
            }
            subjects = updatedSubjects;
            Graph graph(subjects, totalSubjects);
            L = doTopologicalSort(graph);
            // Imprime el orden topológico
            cout << "El orden de las asignaturas después de eliminar es el siguiente: " << endl;
            if (L.size()) {
                for (int i : L) {
                    cout << subjectNames[i] << " --> ";
                }
                cout << "Fin" << endl;
            }
            else {
                cout << "El orden topológico no es posible" << endl;
            }
        }
        else if (option == 0) {
            break;
        }
    }

    return 0;
}
