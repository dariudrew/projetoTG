#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>
#include <string>
using namespace std;

const int INFINITO = numeric_limits<int>::max();

struct Aresta {
    int destino, peso;
    Aresta(int destino, int peso) : destino(destino), peso(peso) {}
};

using Grafo = vector<vector<Aresta>>;

void Prim(const Grafo& G, int v0) {
    int numVertices = G.size();
    vector<int> custo(numVertices, INFINITO);
    vector<int> prev(numVertices, -1);
    vector<bool> visitado(numVertices, false);

    custo[v0] = 0;

 
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pq.push({0, v0});

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        if (visitado[u]) continue;
        visitado[u] = true;

        for (const Aresta& aresta : G[u]) {
            int v = aresta.destino;
            int peso = aresta.peso;

            if (!visitado[v] && custo[v] > peso) {
                custo[v] = peso;
                prev[v] = u;
                pq.push({custo[v], v});
            }
        }
    }

    int custoTotal = 0;
    vector<pair<int, int>> arestas;

    for (int v = 0; v < numVertices; ++v) {
        if (prev[v] != -1) {
            arestas.emplace_back(min(prev[v] + 1, v + 1), max(prev[v] + 1, v + 1));
            custoTotal += custo[v];
        }
    }

    sort(arestas.begin(), arestas.end());

    cout << custoTotal << endl;
}

Grafo lerGrafo(const string& nomeArquivo, int& numVertices, int& verticeInicial) {
    ifstream arquivo(nomeArquivo);
    if (!arquivo) {
        cerr << "Erro ao abrir o arquivo de entrada.\n";
        exit(1);
    }

    int numArestas;
    arquivo >> numVertices >> numArestas;

    Grafo G(numVertices);

    int u, v, peso;
    for (int i = 0; i < numArestas; ++i) {
        arquivo >> u >> v >> peso;
        u--;
        v--;
        G[u].emplace_back(v, peso);
        G[v].emplace_back(u, peso);
    }

    arquivo.close();
    return G;
}

void menu() {
    cout << "============= MENU ===============\n";
    cout << "-h: Mostra os parametros de ajuda\n";
    cout << "-f: Indica o arquivo de entrada\n";
    cout << "-o: Redireciona a saida para um arquivo\n";
    cout << "-i: Indica o vertice inicial\n";
    cout << "-s: Mostra a solução no terminal (formato simplificado)\n";
    cout << "==================================\n";
}

int main(int argc, char* argv[]) {
    string nomeArquivo, arquivoSaida;
    int verticeInicial = -1;
    int numVertices = 0;
    bool exibirAjuda = false;
    bool mostrarSolucao = false;

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-h") {
            exibirAjuda = true;
        } else if (arg == "-f" && i + 1 < argc) {
            nomeArquivo = argv[++i];
        } else if (arg == "-o" && i + 1 < argc) {
            arquivoSaida = argv[++i];
        } else if (arg == "-i" && i + 1 < argc) {
            verticeInicial = stoi(argv[++i]) - 1;
        } else if (arg == "-s") {
            mostrarSolucao = true;
        }
    }

    if (exibirAjuda || nomeArquivo.empty()) {
        menu();
        return 0;
    }

    Grafo G = lerGrafo(nomeArquivo, numVertices, verticeInicial);

    ostringstream buffer;
    Prim(G, verticeInicial);

    if (!arquivoSaida.empty()) {
        ofstream saida(arquivoSaida);
        if (!saida) {
            cerr << "Erro ao abrir o arquivo de saída.\n";
            return 1;
        }
        saida << buffer.str();
    }

    if (mostrarSolucao || arquivoSaida.empty()) {
        cout << buffer.str();
    }

    return 0;
}
