#include <ilcplex/ilocplex.h>
#include <bits/stdc++.h>
using namespace std;

ILOSTLBEGIN

// Problema da Frequência de Torres:
// Cada torre de comunicação precisa operar em uma frequência distinta das torres vizinhas para evitar interferência.
// Isso pode ser representado como um grafo, onde cada vértice é uma torre e arestas conectam torres vizinhas.
// O desafio é atribuir "cores" (frequências) aos vértices de forma que vértices adjacentes não tenham a mesma cor.
// Essa tarefa é conhecida como o problema de coloração de grafos, e sua solução minimiza o número de frequências necessárias.


int main() {
    IloEnv env;

    IloModel model(env);

    const int V = 5; // número de vértices
    const int K = V; // número máximo de torres

    // Grafo: matriz de adjacência
    bool adj[V][V] = {
        {0, 1, 1, 0, 0},
        {1, 0, 1, 1, 0},
        {1, 1, 0, 1, 1},
        {0, 1, 1, 0, 1},
        {0, 0, 1, 1, 0}
    };

    // Variáveis: x[v][k] = 1 se vértice v usa a frequencia k
    vector<vector<IloBoolVar>> x(V, vector<IloBoolVar>(K));
    for (int v = 0; v < V; ++v) {
        for (int k = 0; k < K; ++k) {
        x[v][k] = IloBoolVar(env);
        model.add(x[v][k]);
        }
    }

    // Variável y[k] = 1 se a cor k é usada
    IloBoolVarArray y(env, K);
    for (int k = 0; k < K; ++k)
        model.add(y[k]);

    // Cada vértice recebe exatamente uma frequência 
    for (int v = 0; v < V; ++v) {
        IloExpr sum(env);
        for (int k = 0; k < K; ++k){
            sum += x[v][k];
        }
        model.add(sum == 1);
        sum.end();
    }

    // Vértices adjacentes não podem ter a mesma frequência
    for (int u = 0; u < V; ++u) {
        for (int v = u + 1; v < V; ++v) {
        if (adj[u][v]) {
            for (int k = 0; k < K; ++k) {
            model.add(x[u][k] + x[v][k] <= 1);
            }
        }
        }
    }

    // Ligação entre x[v][k] e y[k]
    for (int v = 0; v < V; ++v) {
        for (int k = 0; k < K; ++k) {
        model.add(x[v][k] <= y[k]);
        }
    }

    // Objetivo: minimizar o número de frequências utilizadas
    IloExpr totalCores(env);
    for (int k = 0; k < K; ++k)
        totalCores += y[k];
    model.add(IloMinimize(env, totalCores));

    IloCplex cplex(model);
    cplex.setOut(env.getNullStream());
    if (cplex.solve()) {
        cout << "Número mínimo de frequências: " << cplex.getObjValue() << "\n";
        for (int v = 0; v < V; ++v) {
            for (int k = 0; k < K; ++k) {
                if (cplex.getValue(x[v][k]) > 0.5) {
                cout << "Torre " << v << " recebe frequencia " << k << "\n";
                break;
                }
            }
        }
    }
    return 0;
}
