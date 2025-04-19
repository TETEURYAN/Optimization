#include <ilcplex/ilocplex.h>
#include <bits/stdc++.h>

ILOSTLBEGIN

// Problema do Fluxo Máximo:
// Dado um grafo direcionado com capacidades nas arestas, e dois vértices (fonte e sumidouro),
// o objetivo é determinar a maior quantidade de "fluxo" que pode ser enviada da fonte ao sumidouro sem exceder as capacidades.
// Aparece em redes de transporte, logística, e até na detecção de cortes mínimos em grafos.
// O algoritmo clássico para resolver isso é o de Ford-Fulkerson (ou sua versão eficiente: Edmonds-Karp).


int main(){
    IloEnv env;

    const int vertices = 7;
    const int s = 0;  // Fonte
    const int t = 5;  // Sumidouro

    // Matriz de capacidade
    vector<vector<int>> peso(vertices, vector<int>(vertices, 0));

    peso[0][1] = 5;
    peso[0][2] = 4;
    peso[0][3] = 6;

    peso[1][2] = 4;
    peso[1][4] = 6;

    peso[2][4] = 5;
    peso[2][5] = 4;

    peso[3][5] = 6;
    peso[3][6] = 5;

    peso[4][5] = 5;
    peso[4][6] = 3;

    peso[5][6] = 6;

    IloModel model(env);
    vector<vector<IloNumVar>> grafo(vertices, vector<IloNumVar>(vertices));

    // Criar variáveis grafo apenas onde há capacidade
    for (int u = 0; u < vertices; ++u) {
        for (int v = 0; v < vertices; ++v) {
            if (peso[u][v] > 0) {
               grafo[u][v] = IloNumVar(env, 0, peso[u][v], ILOFLOAT);
                model.add(grafo[u][v]);
            }
        }
    }

    // Restrição de conservação de fluxo
    for (int v = 0; v < vertices; ++v) {
        if (v == s || v == t) continue;

        IloExpr entrada(env), saida(env);
        for (int u = 0; u < vertices; ++u) {
            if (peso[u][v] > 0) entrada += grafo[u][v];
            if (peso[v][u] > 0) saida += grafo[v][u];
        }
        model.add(entrada == saida);
        entrada.end();
        saida.end();
    }

    // Objetivo: maximizar o fluxo que sai da fonte
    IloExpr fluxoFonte(env);
    for (int v = 0; v < vertices; ++v) {
        if (peso[s][v] > 0)
            fluxoFonte += grafo[s][v];
    }
    model.add(IloMaximize(env, fluxoFonte));
    fluxoFonte.end();

    IloCplex cplex(model);
    cplex.solve();

    // Resultados
    cout << "Fluxo máximo: " << cplex.getObjValue() << "\n";
    cout << "Fluxo nas arestas:\n";
    for (int u = 0; u < vertices; ++u) {
        for (int v = 0; v < vertices; ++v) {
            if (peso[u][v] > 0) {
                double val = cplex.getValue(grafo[u][v]);
                if (val > 1e-6) {
                    cout << "grafo[" << u << " -> " << v << "] = " << val << "\n";
                }
            }
        }
    }
    return 0;

}