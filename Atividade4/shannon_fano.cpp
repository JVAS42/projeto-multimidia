// Disciplina: Sistemas Multimídia - UEPB
// Parte 2 - Shannon-Fano Algorithm
// Referência: https://www.geeksforgeeks.org/dsa/shannon-fano-algorithm-for-data-compression/

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <cmath>
using namespace std;

// Estrutura de nó baseada no GeeksforGeeks (expandida para suportar toda a tabela ASCII)
struct node {
    string sym;
    float pro;
    int arr[256];
    int top;
};

// Função recursiva do algoritmo de Shannon-Fano
void shannon(int l, int h, node p[])
{
    float pack1 = 0, pack2 = 0, diff1 = 0, diff2 = 0;
    int i, k = l, j;

    if ((l + 1) == h || l == h || l > h) {
        if (l == h || l > h)
            return;
        p[h].arr[++(p[h].top)] = 0;
        p[l].arr[++(p[l].top)] = 1;
        return;
    }
    else {
        for (i = l; i <= h - 1; i++)
            pack1 = pack1 + p[i].pro;
        pack2 = pack2 + p[h].pro;
        diff1 = abs(pack1 - pack2);

        j = 2;
        bool acionouBreak = false;
        while (j != h - l + 1) {
            k = h - j;
            pack1 = pack2 = 0;
            for (i = l; i <= k; i++)
                pack1 = pack1 + p[i].pro;
            for (i = h; i > k; i--)
                pack2 = pack2 + p[i].pro;
            diff2 = abs(pack1 - pack2);

            if (diff2 >= diff1) {
                acionouBreak = true;
                break;
            }
            diff1 = diff2;
            j++;
        }

        // Correção do bug original do GfG: só incrementa k se o laço parou via break
        // ou se o laço nem chegou a executar (quando h - l + 1 == 2)
        if (acionouBreak || j == 2) {
            k++;
        }

        for (i = l; i <= k; i++)
            p[i].arr[++(p[i].top)] = 1;
        for (i = k + 1; i <= h; i++)
            p[i].arr[++(p[i].top)] = 0;

        shannon(l, k, p);
        shannon(k + 1, h, p);
    }
}

// Ordena os símbolos em ordem crescente de probabilidade (lógica original do GfG)
void sortByProbability(int n, node p[])
{
    node temp;
    for (int j = 1; j <= n - 1; j++) {
        for (int i = 0; i < n - 1; i++) {
            if ((p[i].pro) > (p[i + 1].pro)) {
                temp = p[i];
                p[i] = p[i + 1];
                p[i + 1] = temp;
            }
        }
    }
}

// Exibe a tabela de símbolos, probabilidades e códigos gerados
void display(int n, node p[])
{
    cout << "\tCaractere\tProbabilidade\tCodigo\n";
    for (int i = n - 1; i >= 0; i--) {
        cout << "\t'" << p[i].sym << "'\t\t" << fixed << setprecision(4) << p[i].pro << "\t\t";
        for (int j = 0; j <= p[i].top; j++)
            cout << p[i].arr[j];
        cout << "\n";
    }
}

// Função solicitada: realiza a compressão de uma mensagem com Shannon-Fano
string shannon_fano(const string &texto)
{
    if (texto.empty()) {
        cout << "Aviso: Nao foi informada nenhuma string para comprimir.\n";
        return "Nenhuma string foi informada.";
    }

    node p[256];
    map<char, int> freq;
    for (char c : texto) {
        freq[c]++;
    }

    int n = 0;
    float total_chars = texto.length();

    for (auto par : freq) {
        p[n].sym = string(1, par.first);
        p[n].pro = par.second / total_chars;
        p[n].top = -1;
        n++;
    }

    sortByProbability(n, p);

    // Tratamento para mensagens com apenas 1 caractere distinto (ex: "aaaaa")
    if (n == 1) {
        p[0].arr[++(p[0].top)] = 0;
    } else {
        shannon(0, n - 1, p);
    }

    // Monta o dicionário de códigos para comprimir a mensagem de entrada
    map<char, string> dicionario;
    for (int i = 0; i < n; i++) {
        string codigo = "";
        for (int j = 0; j <= p[i].top; j++) {
            codigo += to_string(p[i].arr[j]);
        }
        dicionario[p[i].sym[0]] = codigo;
    }

    // Codifica a mensagem completa
    string mensagemComprimida = "";
    for (char c : texto) {
        mensagemComprimida += dicionario[c];
    }

    // Exibe os resultados detalhados
    cout << "Mensagem original : \"" << texto << "\"\n";
    display(n, p);
    int bitsOriginais = texto.length() * 8;
    int bitsComprimidos = mensagemComprimida.length();
    cout << "Binario comprimido: " << mensagemComprimida << "\n";
    cout << "Tamanho original  : " << bitsOriginais << " bits (ASCII 8-bit)\n";
    cout << "Tamanho comprimido: " << bitsComprimidos << " bits\n";
    cout << "------------------------------------------------------------\n";

    return mensagemComprimida;
}

int main()
{
    cout << "=== ALGORITMO DE SHANNON-FANO ===\n";
    cout << "Digite uma string para comprimir com Shannon-Fano: ";
    
    string entradaUsuario;
    getline(cin, entradaUsuario);

    cout << "\n";
    shannon_fano(entradaUsuario);

    return 0;
}
