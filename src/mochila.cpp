#include <ilcplex/ilocplex.h>
#include <iostream>

using namespace std;

int main(){
    IloEnv env;

    IloModel model(env);

    int n, w;
    cout<<"Quantidade de itens: ";
    cin>> n;
    int valores[n], pesos[n];
    
    cout<<endl<<"Capacidade da mochila: ";
    cin>> w;
    
    for(int i = 0; i < n; i++){
        cout<<endl<<"Valor do item "<<i+1<<": ";
        cin>>valores[i];
        cout<<endl<<"Peso do item "<<i+1<<": ";
        cin>>pesos[i];
    }
    cout<<endl;
    
    IloBoolVarArray itens (env, n);

    IloExpr pesoTotal(env);
    for(int i = 0; i < n; i++){
        pesoTotal += pesos[i] * itens[i];
    }
    model.add(pesoTotal <= w);
    pesoTotal.end();

    IloExpr valorTotal(env);
    for (int i = 0; i < n; i++) {
        valorTotal += valores[i] * itens[i];
    }
    model.add(IloMaximize(env, valorTotal));
    valorTotal.end();

    IloCplex cplex(model);
    cplex.solve();

    cout << "Valor ótimo: "<<cplex.getObjValue()<<endl;
    for(int i = 0; i < n; i++)
    {
        if(cplex.getValue(itens[i] == 1)){
            cout<<"Item "<<i+1<<": Valor: "<< valores[i]<<" Peso: "<<pesos[i]<<endl;
        }
    }

    env.end();
    return 0;
}