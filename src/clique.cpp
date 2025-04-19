#include <ilcplex/ilocplex.h>
#include <bits/stdc++.h>
using namespace std;

ILOSTLBEGIN

// Problema da Clique:
// Dado um grafo, o objetivo é encontrar o maior subconjunto de vértices onde todos estão conectados entre si — ou seja, formam uma clique.
// Esse problema aparece em contextos como análise de redes sociais (grupos de pessoas que se conhecem mutuamente) ou bioinformática.
// A identificação de cliques máximas é computacionalmente difícil (NP-difícil), e com frequência usamos algoritmos aproximados.

int main() {
    IloEnv env;
    IloModel model(env);

    const int n = 5; // Número de vértices
    bool grafo[n][n] = {}; // Matriz de grafoacência

    // Arestas
    vector<pair<int, int>> edges = {
        {0,1}, {0,2}, {0,3}, {1,3}, {2,5}, {2,3}, {5,6}, {5,7},{6,3}, {6,9}, {1,4}, {3,4}, {9,7}, {8,9}, {6,7}, {3,6}
    };

    // Preencher matriz de grafoacência
    for (auto [u, v] : edges) {
        grafo[u][v] = grafo[v][u] = true;
    }

    // Variáveis x[i] binárias
    IloBoolVarArray x(env, n);
    for (int i = 0; i < n; ++i) {
        model.add(x[i]);
    }

    // Restrições: para todo par não conectado, x_i + x_j <= 1
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (!grafo[i][j]) {
                model.add(x[i] + x[j] <= 1);
            }
        }
    }

    // Função objetivo: maximizar a soma de x[i]
    IloExpr obj(env);
    for (int i = 0; i < n; ++i) {
        obj += x[i];
    }
    model.add(IloMaximize(env, obj));
    obj.end();

    // Resolver
    IloCplex cplex(model);
    cplex.solve();

    // Saída
    cout << "Tamanho da clique máxima: " << cplex.getObjValue() << "\n";
    cout << "Vértices na clique: ";
    for (int i = 0; i < n; ++i) {
        if (cplex.getValue(x[i]) > 0.5)
            cout << i + 1 << " ";
    }
    cout << "\n";

    return 0;
}
