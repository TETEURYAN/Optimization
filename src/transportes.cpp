#include <ilcplex/ilocplex.h>

ILOSTLBEGIN

// Problema dos Transportes:
// Determina o plano de distribuição de mercadorias de vários fornecedores para vários destinos,
// minimizando o custo total de transporte, respeitando a oferta e a demanda.

int main(){
    IloEnv env;

    IloModel model(env);
 
   //Custo de transporte entre uma fábrica I e um depósito J
    IloArray <IloIntVarArray> custo(env, 3);
    for(unsigned i = 0; i < 3; ++i){
        custo[i] = IloIntVarArray(env, 3, 0, 0x7fffffff);
    }
 
    //Oferta da fábrica I
    model.add(custo[0][0] + custo[0][1] + custo[0][2] <= 120);
    model.add(custo[1][0] + custo[1][1] + custo[1][2] <= 80);
    model.add(custo[2][0] + custo[2][1] + custo[2][2] <= 80);
 
    //Demanda para o depósito J
    model.add(custo[0][0] + custo[1][0] + custo[2][0] == 150);
    model.add(custo[0][1] + custo[1][1] + custo[2][1] == 70);
    model.add(custo[0][2] + custo[1][2] + custo[2][2] == 60);
    
    //Função objetivo: minimizar o custo dado as restrições
    model.add(IloMaximize(env, 8*custo[0][0] + 5*custo[0][1] + 6*custo[0][2]
                            + 15*custo[1][0] + 10*custo[1][1] + 12*custo[1][2]
                            +  3*custo[2][0] + 9*custo[2][1] + custo[2][2]));
 
 
    IloCplex cplex(model);
    cplex.solve();
 
 
    cout << "Valor ótimo: "<<cplex.getObjValue()<<endl;
    for(int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++){
            cout<< "Custo operacional da fábrica " << i + 1 <<" para o depósito " << j + 1<<": "<<cplex.getValue(custo[i][j])<<endl;
        }
    }
 
 
    env.end();
    return 0;


}