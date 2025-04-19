#include <bits/stdc++.h>
#include <chrono>

#define capacidade_bin 1.0

using namespace std;

typedef struct Solucao {
    vector<int> alocacao; // alocacao[i] = bin onde o item i foi colocado
    int num_bins; //qnt total de bins sendo utilizados

    vector<double> calcula_uso_bins(const vector<double>& tam_itens) const {
        vector<double> uso(num_bins, 0.0);
        for (int i = 0; i < alocacao.size(); ++i) {
            uso[alocacao[i]] += tam_itens[i];
        }
        return uso;
    }

    void adiciona_bin() {
        num_bins = *max_element(alocacao.begin(), alocacao.end()) + 1;
    }

    bool operator==(const Solucao& outra) const {
        return alocacao == outra.alocacao;
    }
} Solucao;

//Método para avaliar se um determinado movimento está na Tabu Search
bool esta_tabu(const vector<Solucao>& lista_tabu, const Solucao& s) {
    return find(lista_tabu.begin(), lista_tabu.end(), s) != lista_tabu.end();
}

//Método para avaliar o número de bins das soluções
int avaliar(const Solucao& s) {
    return s.num_bins;
}

Solucao solucao_inicial_greedy(int n, vector<double>& tam_itens) {
    Solucao inicial;
    inicial.alocacao.resize(n);
    vector<double> uso_bins;

    for (int i = 0; i < n; i++) {
        bool colocado = false;
        for (int b = 0; b < uso_bins.size(); b++) {
            if (uso_bins[b] + tam_itens[i] <= capacidade_bin) {
                uso_bins[b] += tam_itens[i];
                inicial.alocacao[i] = b;
                colocado = true;
                break;
            }
        }
        if (!colocado) {
            uso_bins.push_back(tam_itens[i]);
            inicial.alocacao[i] = uso_bins.size() - 1;
        }
    }
    inicial.num_bins = uso_bins.size();
    return inicial;
}

vector<Solucao> gerar_vizinhos(Solucao atual, vector<double>& tam_itens) {
    vector<Solucao> vizinhos;
    vector<double> uso = atual.calcula_uso_bins(tam_itens);
    int n = tam_itens.size();

    for (int i = 0; i < n; i++) {
        int bin_atual = atual.alocacao[i];
        for (int b = 0; b < atual.num_bins; b++) {
            if (b == bin_atual) continue;

            if (uso[b] + tam_itens[i] <= capacidade_bin) {
                Solucao viz = atual;
                viz.alocacao[i] = b;
                viz.adiciona_bin();
                vizinhos.push_back(viz);
            }
        }
    }
    return vizinhos;
}

Solucao tabu_search(int n, vector<double>& tam_itens, int max_iter, int n_vizinhos, int tempo) {
    using namespace std::chrono;
    // n ← número de vizinhos a serem gerados
    // S ← solução candidata inicial qualquer

    Solucao S = solucao_inicial_greedy(n, tam_itens);
    Solucao Melhor = S;
    vector<Solucao> L; // Lista tabu
    L.push_back(S);

    auto inicio = steady_clock::now();
    auto duracao_max = seconds(tempo);

    while (steady_clock::now() - inicio < duracao_max){
        vector<Solucao> vizinhos = gerar_vizinhos(S, tam_itens);
        if (vizinhos.empty()) continue;
        Solucao R = vizinhos[0];
        // se W não está em L e (Qualidade(W) > Qualidade(R) ou R está em L) então
        // R ← W
        for (int k = 1; k < n_vizinhos && k < vizinhos.size(); k++) {
            Solucao W = vizinhos[k];
            if (!esta_tabu(L, W) && (avaliar(W) < avaliar(R) || esta_tabu(L, R))) {
                R = W;
            }
        }

        //se R não está em L e Qualidade(R) > Qualidade(S) então
        // S ← R
        //insira R em L
        if (!esta_tabu(L, R) && avaliar(R) < avaliar(S)) {
            S = R;
        }
        L.push_back(R);

        //se Qualidade(S) > Qualidade(Melhor) então
        // Melhor ← S
        if (avaliar(S) < avaliar(Melhor)) {
            Melhor = S;
        }
    }
    return Melhor;
}

int main(int argc, char* argv[]){
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " Tempo limite (s)" << endl;
        cerr << "Exemplo: " << argv[0] << " 30" << endl;
        return 1;
    }

    int n_itens;
    cout << "Quantidade de itens: ";
    cin >> n_itens;
    vector<double> tamanho_cada_item(n_itens);
    for (int i = 0; i < n_itens; i++) {
        cout << "Tamanho do item " << i + 1 << ": ";
        cin >> tamanho_cada_item[i];
    }

    int iteracoes = 1000;
    int n_vizinhos = 20;
    int tempo = atoi(argv[1]);

    Solucao solucao = tabu_search(n_itens, tamanho_cada_item, iteracoes, n_vizinhos, tempo);

    cout << "\nMelhor solução encontrada:" << endl;
    cout << "Número de bins usados: " << solucao.num_bins << endl;
    cout << "Alocação de cada item:" << endl;
    for (int i = 0; i < n_itens; i++) {
        cout << "Item " << i + 1 << " -> Bin " << solucao.alocacao[i] << endl;
    }
    return 0;
}
