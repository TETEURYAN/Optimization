#include <ilcplex/ilocplex.h>
#include <iostream>

using namespace std;

int main(){
    IloEnv env;

    IloModel model(env);

    //componente i na tinta j; i = {solA, solB, sec, cor}; j = {sr, sn}
    IloArray <IloIntVarArray> comp(env, 2);
    for(unsigned i = 0; i < 2; ++i){
        comp[i] = IloIntVarArray(env, 4, 0, 0x7fffffff);
    }

    string componentes[] = {"Sol. A", "Sol. B", "SEC", "COR"};
    string tintas[] = {"Secagem Rápida", "Secagem normal"};

    //volume
    model.add(comp[0][0] + comp[0][1] + comp[0][2] + comp[0][3] == 1000);
    model.add(comp[1][0] + comp[1][1] + comp[1][2] + comp[1][3] == 250);

    //qnt min SEC
    model.add(0.3 * comp[0][0] + 0.6 * comp[0][1] + 1 * comp[0][2] >= 250);
    model.add(0.3 * comp[1][0] + 0.6 * comp[1][1] + 1 * comp[1][2] >= 50);

    //qnt min cor
    model.add(0.7 * comp[0][0] + 0.4 * comp[0][1] + 1 * comp[0][3] >= 500);
    model.add(0.7 * comp[1][0] + 0.4 * comp[1][1] + 1 * comp[1][3] >= 125);

    model.add(IloMinimize(env, 1.5 * (comp[0][0] + comp[1][0]) + 1 * (comp[0][1] + comp[1][1]) + 4 * (comp[0][2] + comp[1][2]) + 6 * (comp[0][3] + comp[1][3])));

    IloCplex cplex(model);
    cplex.solve();

    cout << "Valor ótimo: "<<cplex.getObjValue()<<endl;
    for(int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 4; j++){
           cout<< "Quantidade de " << componentes[j] <<" na tinta de " << tintas[i]<<": "<<cplex.getValue(comp[i][j])<<endl;
        }
    }

    env.end();
    return 0;
}