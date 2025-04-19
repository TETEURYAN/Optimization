#include <ilcplex/ilocplex.h>
#include <iostream>

using namespace std;

int main(){
    IloEnv env;

    IloModel model(env);

    //area de uma fazenda i na cultura j; i = {1, 2, 3}; j={milho, arroz, feijão}
    IloArray <IloIntVarArray> area(env, 3);
    for(unsigned i = 0; i < 3; ++i){
        area[i] = IloIntVarArray(env, 3, 0, 0x7fffffff);
    }

    string culturas[] = {"milho", "arroz", "feijão"};

    //area max fazenda
    model.add(area[0][0] + area[0][1] + area[0][2] <= 400);
    model.add(area[1][0] + area[1][1] + area[1][2] <= 650);
    model.add(area[2][0] + area[2][1] + area[2][2] <= 350);

    //area max cultura
    model.add(area[0][0] + area[1][0] + area[2][0] <= 660);
    model.add(area[0][1] + area[1][1] + area[2][1] <= 880);
    model.add(area[0][2] + area[1][2] + area[2][2] <= 660);

    //água
    model.add( 5.5 * area[0][0] + 4 * area[0][1] + 3.5 * area[0][2] <= 1800);
    model.add( 5.5 * area[1][0] + 4 * area[1][1] + 3.5 * area[1][2] <= 2200);
    model.add( 5.5 * area[2][0] + 4 * area[2][1] + 3.5 * area[2][2] <= 950);

    //proporção
    model.add((area[0][0] + area[0][1] + area[0][2])/400 == (area[1][0] + area[1][1] + area[1][2])/650 && (area[0][0] + area[0][1] + area[0][2])/400 == (area[2][0] + area[2][1] + area[2][2])/350);

    model.add(IloMaximize(env, 5000 * (area[0][0] + area[1][0] + area[2][0]) + 4000 * (area[0][1] + area[1][1] + area[2][1]) + 1800 * (area[0][2] + area[1][2] + area[2][2])));

    IloCplex cplex(model);
    cplex.solve();

    cout << "Valor ótimo: "<<cplex.getObjValue()<<endl;
    for(int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++){
            cout<< "Área da fazenda " << i + 1 <<" na Cultura de " << culturas[j]<<": "<<cplex.getValue(area[i][j])<<endl; 
        }
    }

    env.end();
    return 0;
}