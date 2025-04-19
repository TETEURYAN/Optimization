#include <ilcplex/ilocplex.h>
#include <bits/stdc++.h>
using namespace std;

ILOSTLBEGIN

// Problema das Facilidades:
// Decide onde instalar centros de distribuição ou serviços, de forma a minimizar custos de operação
// e/ou distâncias até os clientes, respeitando restrições de capacidade e demanda.

int main() {
    IloEnv env;
    IloModel model(env);

    const int N = 3; // depósitos
    const int M = 4; // clientes

    // Custo fixo de instalação
    int f[N] = {100, 150, 120};

    // Custo de atendimento c[i][j]
    int c[N][M] = {
        {20, 24, 11, 25},
        {28, 27, 82, 83},
        {74, 97, 71, 96}
    };

    // Variáveis de decisão
    IloBoolVarArray y(env, N); // y[i] = 1 se depósito i é instalado
    vector<vector<IloBoolVar>> x(N, vector<IloBoolVar>(M));

    // Criar e adicionar variáveis ao modelo
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
        x[i][j] = IloBoolVar(env);
        model.add(x[i][j]);
        }
        model.add(y[i]);
    }

    // Cada cliente deve ser atendido por um único depósito
    for (int j = 0; j < M; ++j) {
        IloExpr sum(env);
        for (int i = 0; i < N; ++i) {
        sum += x[i][j];
        }
        model.add(sum == 1);
        sum.end();
    }

    // Um cliente só pode ser atendido se o depósito estiver instalado
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
        model.add(x[i][j] <= y[i]);
        }
    }

    // Função objetivo
    IloExpr obj(env);
    for (int i = 0; i < N; ++i) {
        obj += f[i] * y[i];
        for (int j = 0; j < M; ++j) {
            obj += c[i][j] * x[i][j];
        }
    }
    
    model.add(IloMinimize(env, obj));
    obj.end();

    IloCplex cplex(model);
    cplex.setOut(env.getNullStream()); 
    
    if (cplex.solve()) {
        cout << "Custo mínimo total: " << cplex.getObjValue() << "\n";
        for (int i = 0; i < N; ++i) {
        if (cplex.getValue(y[i]) > 0.5) {
            cout << "Depósito " << i << " será instalado.\n";
            for (int j = 0; j < M; ++j) {
            if (cplex.getValue(x[i][j]) > 0.5) {
                cout << "  -> Atende cliente " << j << "\n";
            }
            }
        }
        }
    } 

    env.end();
    return 0;
}
