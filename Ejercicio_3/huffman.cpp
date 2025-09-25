#include <iostream>
#include <fstream> 
#include <string>
#include <vector>
#include <queue>
#include <cmath>
#include <unordered_map>
#include <iomanip>

using namespace std;

// Nodo del árbol de Huffman
struct Node {
    int frequency;
    char character;
    Node* left;
    Node* right;

    Node(int f, char c = 0) : frequency(f), character(c), left(nullptr), right(nullptr) {}
};

// Comparador para la cola de prioridad (menor frecuencia primero)
struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->frequency > b->frequency;
    }
};

// Cargar el archivo corpus.txt en un string
string loadCorpus(const string& filename) {
    ifstream file(filename);
    return string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
}

// Generar códigos Huffman recorriendo el árbol
void generateCodes(Node* root, string path, unordered_map<char, string>& codeMap) {
    if (!root) return;
    if (!root->left && !root->right) {
        codeMap[root->character] = path; // hoja -> asignar código
    }
    generateCodes(root->left, path + "0", codeMap);
    generateCodes(root->right, path + "1", codeMap);
}

// Verificar la propiedad de prefijo de los códigos Huffman
bool verifyPrefixProperty(const unordered_map<char, string>& codeMap) {
    vector<string> codes;
    for (const auto& pair : codeMap) codes.push_back(pair.second);
    for (int i = 0; i < codes.size(); i++) {
        for (int j = 0; j < codes.size(); j++) {
            if (i != j && codes[j].find(codes[i]) == 0) return false;
        }
    }
    return true;
}

int main() {
    string data = loadCorpus("corpus.txt");

    // Contar frecuencias de cada símbolo
    unordered_map<char, int> freqMap;
    for (char c : data) freqMap[c]++;

    // Construir cola de prioridad con los nodos iniciales
    priority_queue<Node*, vector<Node*>, Compare> heap;
    for (const auto& pair : freqMap) {
        heap.push(new Node(pair.second, pair.first));
    }

    // Construcción del árbol de Huffman
    while (heap.size() > 1) {
        Node* lo = heap.top(); heap.pop();
        Node* hi = heap.top(); heap.pop();
        Node* merged = new Node(lo->frequency + hi->frequency);
        merged->left = lo;
        merged->right = hi;
        heap.push(merged);
    }

    Node* root = heap.empty() ? nullptr : heap.top();
    unordered_map<char, string> codeMap;
    generateCodes(root, "", codeMap);

    // Calcular longitud media y comparación con longitud fija
    double totalBits = 0;
    int totalFreq = 0;
    for (const auto& pair : freqMap) {
        totalBits += pair.second * codeMap[pair.first].length();
        totalFreq += pair.second;
    }
    double avgLength = totalBits / totalFreq;
    int fixedLength = ceil(log2(freqMap.size()));

    // Mostrar tabla de resultados
    cout << "\nTabla de codigos Huffman:\n";
    cout << "--------------------------------------------------\n";
    cout << left << setw(10) << "Simbolo"
         << setw(12) << "Frecuencia"
         << setw(20) << "Codigo"
         << setw(10) << "Longitud" << endl;
    
    for (const auto& pair : freqMap) {
        cout << left << setw(10) << ("'" + string(1, pair.first) + "'")
             << setw(12) << pair.second
             << setw(20) << codeMap[pair.first]
             << setw(10) << codeMap[pair.first].length() << endl;
    }

    cout << "--------------------------------------------------\n";
    cout << "\nLongitud media Huffman: " << avgLength << " bits/simbolos" << endl;
    cout << "Longitud fija: " << fixedLength << " bits/simbolo" << endl;
    cout << "Propiedad de prefijo: " << (verifyPrefixProperty(codeMap) ? "Cumple" : "No cumple") << endl;
    cout << "longitud fija: " << (totalFreq * fixedLength) << " bits\n";
    cout << "Huffman: " << totalBits << " bits\n";

    return 0;
}
