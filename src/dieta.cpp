#include <ilcplex/ilocplex.h>
#include <iostream>

using namespace std;

int main(){
    IloEnv env;

    IloModel model(env);

    //criando as variáveis de decisão (essa criação já deixa implícita a restrição de não negatividades)
    IloIntVar ing1 (env, 0, 0x7fffffff);
    IloIntVar ing2 (env, 0, 0x7fffffff);
    IloIntVar ing3 (env, 0, 0x7fffffff);
    IloIntVar ing4 (env, 0, 0x7fffffff);
    IloIntVar ing5 (env, 0, 0x7fffffff);
    IloIntVar ing6 (env, 0, 0x7fffffff);

    //restrições
    model.add(1 * ing1 + 0 * ing2 + 2 * ing3 + 2 * ing4 + 1 * ing5 + 2 * ing6 >= 9); //vitA
    model.add(0 * ing1 + 1 * ing2 + 3 * ing3 + 1 * ing4 + 3 * ing5 + 2 * ing6 >= 19); //vitC

    model.add(IloMinimize(env, 35 * ing1 + 30 * ing2 + 60 * ing3 + 50 * ing4 + 27 * ing5 + 22 * ing6));

    IloCplex cplex(model);
    cplex.solve();

    cout << "Valor ótimo: "<< cplex.getObjValue()<<endl;
    cout<<"Ingrediente 1: "<<cplex.getValue(ing1)<<endl;
    cout<<"Ingrediente 2: "<<cplex.getValue(ing2)<<endl;
    cout<<"Ingrediente 3: "<<cplex.getValue(ing3)<<endl;
    cout<<"Ingrediente 4: "<<cplex.getValue(ing4)<<endl;
    cout<<"Ingrediente 5: "<<cplex.getValue(ing5)<<endl;
    cout<<"Ingrediente 6: "<<cplex.getValue(ing6)<<endl;


    env.end();
    return 0;
}