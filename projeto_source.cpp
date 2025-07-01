#include <bits/stdc++.h>
#define pii pair<int, int>
#define f first
#define s second
using namespace std;

class No {
private:
    bool livre;
    int num;

public:
    No(bool l, int n) : livre(l), num(n) {}
    bool get_livre() const { return livre; }
    int get_num() const { return num; }
};

class Grafo {
private:
    vector<vector<int>> matriz_adjacencia;
    vector<bool> visitado;
    vector<No> nos;
    int tam;

public:
    Grafo(int t) {
        tam = t;
        matriz_adjacencia = vector<vector<int>>(t, vector<int>(t));
        visitado = vector<bool>(t);
    }
    int get_tam() { return tam; }
    void add_no(No n) { nos.push_back(n); }
    void add_aresta(int o, int d) {
        matriz_adjacencia[o][d] = 1;
        matriz_adjacencia[d][o] = 1;
    }
    bool checar_aresta(int o, int d) { return (matriz_adjacencia[o][d] == 1); }
    void print() {
        for (int i = 0; i < tam; i++) {
            for (int j = 0; j < tam; j++) {
                cout << matriz_adjacencia[i][j] << ' ';
            }
            cout << '\n';
        }
        cout << '\n';
    }
    void print_caminho() {
        int csz = sqrt(tam);
        vector<vector<char>> c(csz, vector<char>(csz, '-'));
        for (int i = 0; i < tam; i++) {
            if (visitado[i]) {
                for (int j = 0; j < csz; j++) {
                    for (int k = 0; k < csz; k++) {
                        if (k + j * c.size() == i) {
                            c[j][k] = '*';
                        }
                    }
                }
            }
        }
        for (int i = 0; i < csz; i++) {
            for (int j = 0; j < csz; j++) {
                cout << c[i][j] << ' ';
            }
            cout << '\n';
        }
        cout << '\n';
    }
    void dfs_helper(int o, int d) {
        if (visitado[o])
            return;
        else
            visitado[o] = true;
        for (int i = 0; i < tam; i++) {
            if (matriz_adjacencia[o][i]) {
                if (i == d) {
                    visitado[i] = true;
                    return;
                }
                dfs_helper(i, d);
            }
        }
    }
    bool dfs(int o, int d) {
        dfs_helper(o, d);
        return visitado[d];
    }
    bool bfs(int o, int d) {
        queue<int> q;
        visitado = vector<bool>(tam);
        q.push(o);
        visitado[o] = true;
        while (!q.empty()) {
            o = q.front();
            q.pop();
            for (int i = 0; i < tam; i++) {
                if (matriz_adjacencia[o][i] == 1 && !visitado[i]) {
                    q.push(i);
                    visitado[i] = true;
                    if (i == d) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    void gerar_dot(string filename, int source, int destination) {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Erro ao abrir o arquivo " << filename << endl;
            return;
        }
        int n = sqrt(tam);
        file << "graph G {\n";
        file << "    layout=neato;\n";
        file << "    splines=false;\n";
        file << "    node [shape=square, width=0.5, height=0.5, "
                "fixedsize=true, label=\"\"];\n";
        file << "    edge [penwidth=2.0];\n";
        file << "    overlap=false;\n";
        file << "    start=1;\n";

        for (int i = 0; i < tam; i++) {
            string color;
            if (i == source) {
                color = "green";
            } else if (i == destination) {
                color = "red";
            } else if (visitado[i]) {
                color = "blue";
            } else {
                color = nos[i].get_livre() ? "white" : "gray";
            }

            int row = i / n;
            int col = i % n;

            int y = n - 1 - row;

            file << "    " << i << " [pos=\"" << col << "," << y << "!\""
                 << ", style=filled, fillcolor=\"" << color << "\"];\n";
        }

        for (int i = 0; i < tam; i++) {
            for (int j = i + 1; j < tam; j++) {
                if (matriz_adjacencia[i][j] == 1) {
                    file << "    " << i << " -- " << j << ";\n";
                }
            }
        }

        file << "}\n";
        file.close();
    }
};

void gerar_labirinto(vector<vector<int>>& lab) {
    srand(time(0));
    int tam = lab.size();
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            int r = rand() % 4;
            if (r == 3)
                lab[i][j] = 1;
        }
    }
}

void printar_labirinto(vector<vector<int>> lab) {
    int tam = lab.size();
    cout << "  ";
    for (int i = 0; i < tam; i++) {
        cout << i << " \n"[i == tam - 1];
    }
    for (int i = 0; i < tam; i++) {
        cout << i << ' ';
        for (int j = 0; j < tam; j++) {
            cout << lab[i][j] << ' ';
        }
        cout << '\n';
    }
    cout << '\n';
}

void gerar_grafo(Grafo& g, vector<vector<int>> lab) {
    int lab_tam = lab.size();
    auto Get = [&](int i, int j) { return j + i * lab_tam; };
    for (int i = 0; i < lab_tam; i++) {
        for (int j = 0; j < lab_tam; j++) {
            bool l = (lab[i][j] == 1);
            int n = Get(i, j);
            g.add_no(No(l, n));
        }
    }
    for (int i = 0; i < lab_tam; i++) {
        for (int j = 0; j < lab_tam; j++) {
            if (i + 1 < lab_tam && lab[i][j] == lab[i + 1][j]) {
                g.add_aresta(Get(i, j), Get(i + 1, j));
            }
            if (j + 1 < lab_tam && lab[i][j] == lab[i][j + 1]) {
                g.add_aresta(Get(i, j), Get(i, j + 1));
            }
        }
    }
}

void solve(int lab_tam) {
    auto Get = [&](int i, int j) { return j + i * lab_tam; };
    vector<vector<int>> l(lab_tam, vector<int>(lab_tam));
    gerar_labirinto(l);
    printar_labirinto(l);
    Grafo g(lab_tam * lab_tam);
    gerar_grafo(g, l);
    bool repetir = true;
    bool ok = false;
    while (repetir) {
        pii x, y = {-1, -1};
        cout << "Escolha a primeira celula\n";
        cin >> x.f >> x.s;
        cout << "Escolha a segunda celula\n";
        cin >> y.f >> y.s;
        if (x.f >= 0 && x.s < lab_tam && y.f >= 0 && y.s < lab_tam &&
            l[x.f][x.s] != 1 && l[y.f][y.s] != 1 && x != y) {
            repetir = false;
        } else
            continue;
        ok = (g.bfs(Get(x.f, x.s), Get(y.f, y.s)) == 1 &&
              g.dfs(Get(x.f, x.s), Get(y.f, y.s)) == 1);
        g.gerar_dot("grafo.dot", Get(x.f, x.s), Get(y.f, y.s));
    }
    cout << '\n';
    cout << (ok ? "POSSIVEL" : "NAO POSSIVEL") << "\n\n";
    g.print_caminho();
}

int main() {
    cout << "Digite o tamanho do grid: ";
    int t;
    cin >> t;
    solve(t);
    system("start cmd /K \"cd && dot -Tpng grafo.dot -o grafo.png && start "
           "grafo.png\"");
}
