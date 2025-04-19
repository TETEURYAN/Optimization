#include <ilcplex/ilocplex.h>
#include <iostream>
#include <vector>

using namespace std;

int main(){
    IloEnv env;

    IloModel model(env);

    int vertices, arestas;
    cout<<"Quantidade de bairros: ";
    cin>> vertices;
    cout<<"Quantidade de arestas: ";
    cin>> arestas;

    vector<vector<int>> adjacencias (vertices);
    for (int i = 0; i < vertices; i++) {
        adjacencias[i].push_back(i);  
    }

    cout<<"Insira as arestas: src tgt"<<endl;
    for(int i = 0; i < arestas; i++){
        int src, tgt;
        cin >> src >> tgt;
        adjacencias[src].push_back(tgt);
        adjacencias[tgt].push_back(src); 
    }
    
    IloBoolVarArray bairros (env, vertices + 1);

    for (int i = 0; i < vertices; i++) {
        IloExpr cobertura(env);
        for (int j : adjacencias[i]) {
            cobertura += bairros[j];
        }
        model.add(cobertura >= 1);
        cobertura.end();
    }

    IloExpr total_escolas(env);
        for (int j = 0; j < vertices; j++) {
            total_escolas += bairros[j];
        }
        model.add(IloMinimize(env, total_escolas));
        total_escolas.end();

    IloCplex cplex(model);
    cplex.solve();

    cout << "Valor ótimo: "<<cplex.getObjValue()<<endl;
    for(int i = 0; i < vertices; i++)
    {
        if(cplex.getValue(bairros[i] == 1)){
            cout<<"Bairro "<<i<<endl;
        }
    }

    env.end();
    return 0;
}