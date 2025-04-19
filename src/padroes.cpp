#include <ilcplex/ilocplex.h>
#include <iostream>
#include <vector>

using namespace std;

int main(){
    IloEnv env;

    IloModel model(env);

    IloIntVarArray formas (env, 4, 0, 0x7fffffff);

    model.add(formas[0] + formas[2] + formas[3] <= 200);
    model.add(formas[1] <= 90);

    IloExpr total_corpos(env);
    total_corpos += 1 * formas[0] + 2 * formas[1] + 0 * formas[2] + 4 * formas[3];

    IloExpr total_tampas (env);
    total_tampas += 7 * formas[0] + 3 * formas[1] + 9 * formas[2] + 4 * formas[3];

    IloIntVar latinhas(env, 0, 0x7fffffff);
    model.add(latinhas <= total_corpos);
    model.add(latinhas <= total_tampas/2);

    IloExpr lucro(env);
    lucro += 50 * latinhas - 50 * (total_corpos - latinhas) - 3 * (total_tampas - latinhas * 2);
    model.add(IloMaximize(env, lucro));

    IloCplex cplex(model);
    cplex.solve();

    cout << "Valor ótimo: "<<cplex.getObjValue()<<endl;
    for(int i = 0; i < 4; i++){
        cout<<"Quantidade de impressões do padrão "<< i+1<<": "<<cplex.getValue(formas[i])<<endl;
    }
    cout<<"Latinhas produzidas: "<<cplex.getValue(latinhas)<<endl;
    cout<<"Corpos não utilizados: "<<cplex.getValue(total_corpos) - cplex.getValue(latinhas)<<endl;
    cout<<"Tampas não utilizadas: "<<cplex.getValue(total_tampas) - cplex.getValue(latinhas) * 2 <<endl;

    total_corpos.end();
    total_tampas.end();
    env.end();
    return 0;
}