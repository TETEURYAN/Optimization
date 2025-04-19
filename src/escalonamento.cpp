#include <ilcplex/ilocplex.h>
#include <bits/stdc++.h>

ILOSTLBEGIN

// Problema de Escalonamento:
// Consiste em alocar tarefas a recursos (como máquinas ou tempo) de forma eficiente, respeitando restrições como ordem de execução, tempo e disponibilidade.
// Um exemplo clássico é escalonar enfermeiras em dias possíveis, tal como o problema desse código.
// O objetivo pode ser minimizar o tempo total (makespan), atrasos, ou conflitos, e o problema pode variar muito dependendo das restrições.

int main(){
    IloEnv env;

    IloModel model(env);

    vector<int> demanda = {8, 6, 5, 7, 6, 4, 5};
    
    int dias = 7;
    
    //Quantidade de enfermeiras que começam no dia i
    IloIntVarArray enfermeiras(env, dias, 0, 0x7fffffff);


    for (int i = 0; i < dias; i++) {
        IloExpr soma(env);
        for (int k = 1; k < 4; k++) {
            soma += enfermeiras[(i - k + dias) % dias];  // pega 5 dias antes incluindo o atual
        }
        model.add(soma >= demanda[i]);
        soma.end();
    }

    model.add(IloMinimize(env, IloSum(enfermeiras)));

    IloCplex cplex(model);
    cplex.solve();

    for (int i = 0; i < dias; ++i)
        cout << "Enfermeiras no dia " << (i + 1) << " = " << cplex.getValue(enfermeiras[i]) << endl;

    cout << "Total de enfermeiras: " << cplex.getObjValue() << endl;

 
    env.end();
    return 0;


}