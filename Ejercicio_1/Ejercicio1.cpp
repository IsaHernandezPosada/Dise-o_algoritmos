#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <cmath>
#include <unordered_map>
#include <iomanip>
#include <numeric>
#include <climits>
#include <algorithm>

using namespace std;

vector<int> cambio_greedy(int monto, vector<int> denominaciones);
vector<int> cambio_bruteforce(int monto, vector<int> denominaciones);       // optimizada (memo)
vector<int> cambio_bruteforce_puro(int monto, vector<int> denominaciones); // fuerza bruta 

// Estructura para almacenar resultados de comparación
struct ResultadoExperimento {
    int monto;
    vector<int> combinacion_greedy;
    vector<int> combinacion_bruteforce;
    int num_monedas_greedy;
    int num_monedas_bruteforce;
    bool coinciden;
};

// Utilidad: convertir vector<int> a string para imprimir bonito
string vectorToString(const vector<int>& v) {
    if (v.empty()) return "{}";
    string s = "{";
    for (size_t i = 0; i < v.size(); i++) {
        s += to_string(v[i]);
        if (i + 1 < v.size()) s += ",";
    }
    s += "}";
    return s;
}

// Función para imprimir tabla de resultados (alineada)
void imprimirTabla(const vector<ResultadoExperimento>& resultados, const string& titulo) {
    cout << "\n" << titulo << "\n\n";
    
    // Encabezados
    cout << setw(4) << "M" 
         << setw(22) << "Greedy" 
         << setw(22) << "Bruteforce"
         << setw(12) << "# Greedy"
         << setw(12) << "# Optimo"
         << setw(12) << "Coinciden" << "\n";
    cout << string(90, '-') << "\n";
    
    // Datos
    for(const auto& resultado : resultados) {
        cout << setw(4) << resultado.monto;
        cout << setw(22) << vectorToString(resultado.combinacion_greedy);
        cout << setw(22) << vectorToString(resultado.combinacion_bruteforce);
        cout << setw(12) << resultado.num_monedas_greedy;
        cout << setw(12) << resultado.num_monedas_bruteforce;
        cout << setw(12) << (resultado.coinciden ? "Si" : "No") << "\n";
    }
    cout << "\n";
}

int main() {

    vector<int> canonico = {1, 2, 5, 10, 20, 50};
    vector<ResultadoExperimento> resultados_canonico;
    
    // Crear montos del 1 al 30
    vector<int> montos(30);
    iota(montos.begin(), montos.end(), 1);
    
    bool todos_coinciden_canonico = true;
    
    for(int M : montos) {
        ResultadoExperimento resultado;
        resultado.monto = M;
        
        // Ejecutar ambos algoritmos
        resultado.combinacion_greedy = cambio_greedy(M, canonico);
        resultado.combinacion_bruteforce = cambio_bruteforce(M, canonico); // optimizada 
        
        resultado.num_monedas_greedy = resultado.combinacion_greedy.size();
        resultado.num_monedas_bruteforce = resultado.combinacion_bruteforce.size();
        
        resultado.coinciden = (resultado.num_monedas_greedy == resultado.num_monedas_bruteforce);
        
        if(!resultado.coinciden) {
            todos_coinciden_canonico = false;
        }
        
        resultados_canonico.push_back(resultado);
    }
    
    // Imprimir tabla del sistema canónico
    imprimirTabla(resultados_canonico, "Sistema canonico D = {1, 2, 5, 10, 20, 50}");
    
    // Conclusión del experimento A
    cout << "Experimento 1:\n";
    if(todos_coinciden_canonico) {
        cout << "El sistema canonico {1, 2, 5, 10, 20, 50} es optimo\n\n";
    } else {
        cout << "Se encontraron diferencias en el sistema canonico\n\n";
    }
      
    vector<int> no_canonico = {1, 4, 6};
    vector<ResultadoExperimento> resultados_no_canonico;
    
    int primer_M_diferente = -1;
    
    for(int M : montos) {
        ResultadoExperimento resultado;
        resultado.monto = M;
        
        // Ejecutar ambos algoritmos
        resultado.combinacion_greedy = cambio_greedy(M, no_canonico);
        resultado.combinacion_bruteforce = cambio_bruteforce_puro(M, no_canonico); // fuerza bruta pura aquí
        
        resultado.num_monedas_greedy = resultado.combinacion_greedy.size();
        resultado.num_monedas_bruteforce = resultado.combinacion_bruteforce.size();
        
        resultado.coinciden = (resultado.num_monedas_greedy == resultado.num_monedas_bruteforce);
        
        // Encontrar el primer M donde difieren
        if(!resultado.coinciden && primer_M_diferente == -1) {
            primer_M_diferente = M;
        }
        
        resultados_no_canonico.push_back(resultado);
    }
    
    // Imprimir tabla del sistema no canónico
    imprimirTabla(resultados_no_canonico, "Sistema no canonico D = {1, 4, 6}");
    
    // Conclusión del experimento B
    cout << "Experimento 2:\n";
    if(primer_M_diferente != -1) {
        cout << "El menor M donde Greedy y Fuerza Bruta difieren es M = " << primer_M_diferente << "\n";
        
        // Mostrar detalles del primer caso donde difieren
        for(const auto& resultado : resultados_no_canonico) {
            if(resultado.monto == primer_M_diferente) {
                cout << "\nDetalle del primer caso donde fallan:\n";
                cout << "M = " << primer_M_diferente << "\n";
                cout << "Greedy:     " << vectorToString(resultado.combinacion_greedy)
                     << " (" << resultado.num_monedas_greedy << " monedas)\n";
                cout << "Optimo:     " << vectorToString(resultado.combinacion_bruteforce)
                     << " (" << resultado.num_monedas_bruteforce << " monedas)\n";
                cout << "Diferencia: " << (resultado.num_monedas_greedy - resultado.num_monedas_bruteforce) << " monedas\n";
                break;
            }
        }
    } else {
        cout << " No se encontro diferencias en el rango M=1 a 30\n";
        cout << "Puede que el primer M donde difieren sea mayor a 30\n";
    }
    
    cout << "Conclusion de los  de los dos Experimentos probados\n";
 
    // Contar coincidencias
    int coincidencias_canonico = 0;
    int coincidencias_no_canonico = 0;
    
    for(const auto& resultado : resultados_canonico) {
        if(resultado.coinciden) coincidencias_canonico++;
    }
    
    for(const auto& resultado : resultados_no_canonico) {
        if(resultado.coinciden) coincidencias_no_canonico++;
    }
    
    cout << "Sistema Canonico {1, 2, 5, 10, 20, 50}:\n";
    cout << "Coincidencias: " << coincidencias_canonico << "/30 casos\n";
    cout << "Porcentaje: " << (coincidencias_canonico * 100.0 / 30) << "%\n\n";
    
    cout << "Sistema No Canonico {1, 4, 6}:\n";
    cout << "Coincidencias: " << coincidencias_no_canonico << "/30 casos\n";
    cout << "Porcentaje: " << (coincidencias_no_canonico * 100.0 / 30) << "%\n";
    cout << "Primer M donde falla: " << (primer_M_diferente != -1 ? to_string(primer_M_diferente) : "No encontrado en rango 1-30") << "\n\n";
    
    return 0;
}

vector<int> cambio_greedy(int monto, vector<int> denominaciones) {
    sort(denominaciones.rbegin(), denominaciones.rend()); // ordenar de mayor a menor 
    
    vector<int> combinacion; 
    int i = 0;
    
    while (monto != 0 && i < (int)denominaciones.size()) {
        if(denominaciones[i] <= monto) {
            combinacion.push_back(denominaciones[i]);
            monto -= denominaciones[i];
        } else {
            i++; 
        }
    }
    
    return combinacion; 
}
unordered_map<int, int> memo;

int bruteforce_min_monedas(int monto, const vector<int>& denominaciones) {
    if (monto == 0) return 0;
    if (monto < 0) return INT_MAX;
    if (memo.find(monto) != memo.end()) return memo[monto];
    
    int min_monedas = INT_MAX;
    for (int d : denominaciones) {
        if (d <= monto) {
            int res = bruteforce_min_monedas(monto - d, denominaciones);
            if (res != INT_MAX) min_monedas = min(min_monedas, 1 + res);
        }
    }
    return memo[monto] = min_monedas;
}

vector<int> cambio_bruteforce(int monto, vector<int> denominaciones) {
    memo.clear();
    int min_monedas = bruteforce_min_monedas(monto, denominaciones);
    if (min_monedas == INT_MAX) return {};
    
    vector<int> combinacion;
    int monto_actual = monto;
    while (monto_actual > 0) {
        for (int d : denominaciones) {
            if (d <= monto_actual) {
                int resto = bruteforce_min_monedas(monto_actual - d, denominaciones);
                if (resto != INT_MAX && 1 + resto == bruteforce_min_monedas(monto_actual, denominaciones)) {
                    combinacion.push_back(d);
                    monto_actual -= d;
                    break;
                }
            }
        }
    }
    return combinacion;
}

void fuerzaBrutaAux(int monto, const vector<int>& denominaciones, int index,
                    vector<int>& actual, vector<int>& mejor) {
    if (monto == 0) {
        if (mejor.empty() || actual.size() < mejor.size()) mejor = actual;
        return;
    }
    if (monto < 0 || index >= (int)denominaciones.size()) return;

    // Tomar moneda actual (puedo usarla varias veces)
    actual.push_back(denominaciones[index]);
    fuerzaBrutaAux(monto - denominaciones[index], denominaciones, index, actual, mejor);
    actual.pop_back();

    // Pasar a la siguiente moneda (no la tomo más)
    fuerzaBrutaAux(monto, denominaciones, index + 1, actual, mejor);
}

vector<int> cambio_bruteforce_puro(int monto, vector<int> denominaciones) {
    sort(denominaciones.rbegin(), denominaciones.rend());
    vector<int> mejor, actual;
    fuerzaBrutaAux(monto, denominaciones, 0, actual, mejor);
    return mejor;
}
