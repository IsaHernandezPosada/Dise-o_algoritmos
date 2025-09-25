#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <cctype>
using namespace std;

// Estructura para guardar cada actividad
struct Actividad {
    int inicio;
    int fin;
};

// Ordenar por hora de fin
bool ordenarPorFin(Actividad a, Actividad b) {
    return a.fin < b.fin;
}

// Algoritmo de Selección por Fin Más Temprano (EFT)
vector<Actividad> seleccion_eft(vector<Actividad> actividades) {
    vector<Actividad> seleccionadas;

    // Ordenar por la hora en la que terminan
    sort(actividades.begin(), actividades.end(), ordenarPorFin);

    int finAnterior = -1; // fin de la última actividad escogida
    for (int i = 0; i < actividades.size(); i++) {
        if (actividades[i].inicio >= finAnterior) {
            seleccionadas.push_back(actividades[i]);
            finAnterior = actividades[i].fin;
        }
    }
    return seleccionadas;
}

// Verificar si un conjunto de actividades es compatible
bool esCompatible(vector<Actividad> subconjunto) {
    for (int i = 0; i < subconjunto.size(); i++) {
        for (int j = i + 1; j < subconjunto.size(); j++) {
            // no son compatibles si se cruzan
            if (!(subconjunto[i].fin <= subconjunto[j].inicio ||
                  subconjunto[j].fin <= subconjunto[i].inicio)) {
                return false;
            }
        }
    }
    return true;
}

// Fuerza Bruta
vector<Actividad> seleccion_fuerza_bruta(vector<Actividad> actividades) {
    int n = actividades.size();
    vector<Actividad> optimo;
    int mayorNum = 0;

    // se prueban todos los subconjuntos posibles
    for (int mask = 0; mask < (1 << n); mask++) {
        vector<Actividad> opcion;
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                opcion.push_back(actividades[i]);
            }
        }
        if (esCompatible(opcion) && opcion.size() > mayorNum) {
            optimo = opcion;
            mayorNum = opcion.size();
        }
    }
    return optimo;
}

// Generador de instancias aleatorias con límites
vector<Actividad> generar(int N, int maxInicio = 10000, int maxDuracion = 100) {
    vector<Actividad> lista;
    for (int i = 0; i < N; i++) {
        int inicio = rand() % maxInicio;
        int fin = inicio + 1 + rand() % maxDuracion; // fin > inicio
        Actividad a;
        a.inicio = inicio;
        a.fin = fin;
        lista.push_back(a);
    }
    return lista;
}

// Guardar actividades en archivo .json
void guardarJSON(string nombreArchivo, vector<Actividad> actividades) {
    ofstream out(nombreArchivo);
    out << "[\n";
    for (int i = 0; i < actividades.size(); i++) {
        out << "  [" << actividades[i].inicio << ", " << actividades[i].fin << "]";
        if (i != actividades.size() - 1) out << ",";
        out << "\n";
    }
    out << "]";
    out.close();
}

// Leer actividades desde un archivo .json
vector<Actividad> leerJSON(string nombreArchivo) {
    vector<Actividad> actividades;
    ifstream in(nombreArchivo);
    char c;
    int num;
    vector<int> numeros;

    // leer caracter por caracter y solo guarda los números
    while (in >> c) {
        if (isdigit(c)) {
            in.putback(c);
            in >> num;
            numeros.push_back(num);
        }
    }

    // cada par de números es una actividad
    for (int i = 0; i + 1 < numeros.size(); i += 2) {
        Actividad a;
        a.inicio = numeros[i];
        a.fin = numeros[i + 1];
        actividades.push_back(a);
    }
    return actividades;
}

int main() {
    srand(time(0));

    // pruebas pequeñas
    cout << "Pruebas pequenas" << endl;

    int nSmall = 18;
    vector<Actividad> actividadesSmall = generar(nSmall, 20, 10);
    guardarJSON("actividades_small.json", actividadesSmall);

    vector<Actividad> actividadesSmallLeidas = leerJSON("actividades_small.json");

    vector<Actividad> eftSmall = seleccion_eft(actividadesSmallLeidas);
    vector<Actividad> fbSmall = seleccion_fuerza_bruta(actividadesSmallLeidas);

    cout << "EFT=" << eftSmall.size()
         << " | FuerzaBruta=" << fbSmall.size();
    if (eftSmall.size() == fbSmall.size()) cout << " -> Bien" << endl;
    else cout << " -> Error" << endl;

    // pruebas grandes
    cout << "\nPruebas grandes" << endl;

    int nLarge = 10000;
    vector<Actividad> actividadesGrandes = generar(nLarge, 10000, 100);
    guardarJSON("actividades_large.json", actividadesGrandes);

    vector<Actividad> actividadesLarge = leerJSON("actividades_large.json");

    clock_t inicio = clock();
    vector<Actividad> eftLarge = seleccion_eft(actividadesLarge);
    clock_t fin = clock();

    double tiempo = double(fin - inicio) / CLOCKS_PER_SEC;

    cout << "Seleccionadas=" << eftLarge.size()
         << " | Tiempo=" << tiempo << "s" << endl;

    return 0;
}