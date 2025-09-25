#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <set>
#include <algorithm>
#include <cstdlib>
#include <ctime>
using namespace std;

// Estructura para Kruskal
struct UnionFind {
    vector<int> parent, size;
    UnionFind(int numNodos) {
        parent.resize(numNodos);
        size.assign(numNodos, 1);
        for (int i = 0; i < numNodos; i++) parent[i] = i;
    }
    int find(int x) {
        if (parent[x] == x) return x;
        return parent[x] = find(parent[x]); 
    }
    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (size[a] < size[b]) swap(a, b);
        parent[b] = a;
        size[a] += size[b];
        return true;
    }
};

// estructura de arista
struct Edge {
    int u, v, w;
};

// kruskal
int kruskal(int numNodos, vector<Edge> &edges) {
    sort(edges.begin(), edges.end(), [](Edge A, Edge B) {
        return A.w < B.w;
    });

    UnionFind uf(numNodos);
    int total = 0;
    int taken = 0;

    for (auto &e : edges) {
        if (uf.unite(e.u, e.v)) {
            total += e.w;
            taken++;
            if (taken == numNodos - 1) break;
        }
    }
    return total;
}

// Prim 
int prim(int numNodos, vector<vector<pair<int,int>>> &visitado) {
    vector<bool> marcado(numNodos, false);
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> cola;
    cola.push(make_pair(0, 0));

    int total = 0, usados = 0;

    while (!cola.empty() && usados < numNodos) {
        pair<int,int> minimo = cola.top();
        cola.pop();
        int peso = minimo.first;
        int nodo = minimo.second;

        if (marcado[nodo]) continue;
        marcado[nodo] = true;
        total += peso;
        usados++;

        for (auto &p : visitado[nodo]) {
            if (!marcado[p.first]) {
                cola.push(make_pair(p.second, p.first));
            }
        }
    }
    return total;
}

// leer grafo desde el archivo
bool leerGrafo(string file, int &V, vector<Edge> &edges,
               vector<vector<pair<int,int>>> &visitado) {
    ifstream in(file.c_str());
    if (!in) {
        cout << "No se pudo abrir el archivo" << file << endl;
        return false;
    }
    int a, b, w;
    int maxNode = -1;
    edges.clear();
    while (in >> a >> b >> w) {
        edges.push_back({a, b, w});
        maxNode = max(maxNode, max(a, b));
    }
    V = maxNode + 1;
    visitado.assign(V, {});
    for (auto &e : edges) {
        visitado[e.u].push_back({e.v, e.w});
        visitado[e.v].push_back({e.u, e.w});
    }
    return true;
}

// funcion para generar grafo 
void generarGrafo(int V, int E, vector<Edge> &edges,
                  vector<vector<pair<int,int>>> &visitado) {
    edges.clear();
    visitado.assign(V, {});

    int maxE = V * (V - 1) / 2;
    if (E > maxE) E = maxE;

    set<pair<int,int>> usados;
    srand(time(0));

    while ((int)edges.size() < E) {
        int u = rand() % V;
        int v = rand() % V;
        if (u == v) continue;
        if (usados.count({u, v}) || usados.count({v, u})) continue;

        int w = 1 + rand() % 20; 
        edges.push_back({u, v, w});
        visitado[u].push_back({v, w});
        visitado[v].push_back({u, w});
        usados.insert({u, v});
    }
}

int main() {
    int opcion;
    cout << "Opciones:\n";
    cout << "1. Leer grafo desde archivo\n";
    cout << "2. Generar grafo disperso (3V)\n";
    cout << "3. Generar grafo denso (V(V-1)/4)\n";
    cout << "Opcion: ";
    cin >> opcion;

    int V;
    vector<Edge> edges;
    vector<vector<pair<int,int>>> visitado;
    string archivo = "grafo_*.txt"; 

    if (opcion == 1) {
        if (!leerGrafo(archivo, V, edges, visitado)) return 0;
        cout << "Archivo cargado con " << V << " nodos y "
             << edges.size() << " aristas.\n";
    } else {
        cout << "Numero de nodos: ";
        cin >> V;
        int E = (opcion == 2) ? 3 * V : (V * (V - 1)) / 4;
        generarGrafo(V, E, edges, visitado);
        cout << "Grafo generado con " << V << " nodos, y " << edges.size() << " aristas.\n";
    }

    // Medir tiempo de kruskal 
    clock_t t1 = clock();
    int cK = kruskal(V, edges);
    clock_t t2 = clock();
    double tiempoK = double(t2 - t1)*1000.0 / CLOCKS_PER_SEC;

    // Medir tiempo de prim
    t1 = clock();
    int cP = prim(V, visitado);
    t2 = clock();
    double tiempoP = double(t2 - t1)*1000.0 / CLOCKS_PER_SEC;
    
    cout << "Resultados:\n";
    cout << "Costo Kruskal=" << cK << " tiempo=" << tiempoK << "ms\n";
    cout << "Costo Prim=" << cP << " tiempo=" << tiempoP << "ms\n";

    if (cK == cP) cout << "Los costos coinciden\n";
    else cout << "Los costos son diferentes\n";

    return 0;
}