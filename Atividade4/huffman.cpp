// Disciplina: Sistemas Multimídia - UEPB
// Parte 3 - Huffman Coding
// Referência: https://www.geeksforgeeks.org/dsa/huffman-coding-greedy-algo-3/

#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <map>
using namespace std;

// Classe que representa um nó da Árvore de Huffman (baseada no GeeksforGeeks)
class Node {
public:
    int data;   // Frequência
    int index;  // Menor índice original na subárvore (para desempate determinístico)
    Node *left, *right;

    // Nó folha
    Node(int d, int i) {
        data = d;
        index = i;
        left = right = nullptr;
    }

    // Nó interno
    Node(Node* l, Node* r) {
        data = l->data + r->data;
        index = min(l->index, r->index);
        left = l;
        right = r;
    }
};

// Comparador para a fila de prioridade (Min-Heap)
class Compare {
public:
    bool operator() (Node* a, Node* b) {
        if (a->data != b->data)
            return a->data > b->data; // Menor frequência primeiro
        return a->index > b->index;   // Desempate pelo índice original
    }
};

// Percorre a árvore em pré-ordem e associa o código diretamente ao índice original do caractere
void preOrder(Node* root, vector<string> &ans, string curr) {
    if (root == nullptr) return;

    if (root->left == nullptr && root->right == nullptr) {
        if (curr == "") curr = "0"; // Caso de caractere único
        ans[root->index] = curr;
        return;
    }

    preOrder(root->left, ans, curr + '0');
    preOrder(root->right, ans, curr + '1');
}

// Libera a memória alocada dinamicamente para a árvore de Huffman
void liberarArvore(Node* root) {
    if (root == nullptr) return;
    liberarArvore(root->left);
    liberarArvore(root->right);
    delete root;
}

// Gera os códigos de Huffman para os símbolos e suas frequências
vector<string> huffmanCodes(string &s, vector<int> &freq) {
    int n = s.length();
    priority_queue<Node*, vector<Node*>, Compare> pq;

    for (int i = 0; i < n; i++) {
        Node* tmp = new Node(freq[i], i);
        pq.push(tmp);
    }

    if (n == 1) {
        liberarArvore(pq.top());
        return {"0"};
    }

    while (pq.size() >= 2) {
        Node* l = pq.top();
        pq.pop();

        Node* r = pq.top();
        pq.pop();

        Node* newNode = new Node(l, r);
        pq.push(newNode);
    }

    Node* root = pq.top();
    vector<string> ans(n);
    preOrder(root, ans, "");
    liberarArvore(root);
    return ans;
}

// Função solicitada: realiza a compressão de uma mensagem utilizando o algoritmo de Huffman
string huffman(const string &texto) {
    if (texto.empty()) {
        cout << "Aviso: Nao foi informada nenhuma string para comprimir.\n";
        return "Nenhuma string foi informada.";
    }

    // Conta a frequência de cada caractere único na mensagem
    map<char, int> mapa_freq;
    for (char c : texto) {
        mapa_freq[c]++;
    }

    string s = "";
    vector<int> freq;
    for (auto par : mapa_freq) {
        s += par.first;
        freq.push_back(par.second);
    }

    vector<string> codigos = huffmanCodes(s, freq);

    // Cria tabela de busca rápida para codificar a mensagem
    map<char, string> dicionario;
    for (size_t i = 0; i < s.length(); i++) {
        dicionario[s[i]] = codigos[i];
    }

    // Codifica o texto completo
    string mensagemComprimida = "";
    for (char c : texto) {
        mensagemComprimida += dicionario[c];
    }

    // Exibe a tabela e estatísticas da compressão
    cout << "Mensagem original : \"" << texto << "\"\n";
    cout << "\tCaractere\tFrequencia\tCodigo\n";
    for (size_t i = 0; i < s.length(); i++) {
        cout << "\t'" << s[i] << "'\t\t" << freq[i] << "\t\t" << codigos[i] << "\n";
    }
    int bitsOriginais = texto.length() * 8;
    int bitsComprimidos = mensagemComprimida.length();
    cout << "Binario comprimido: " << mensagemComprimida << "\n";
    cout << "Tamanho original  : " << bitsOriginais << " bits (ASCII 8-bit)\n";
    cout << "Tamanho comprimido: " << bitsComprimidos << " bits\n";
    cout << "------------------------------------------------------------\n";

    return mensagemComprimida;
}

int main() {
    cout << "=== ALGORITMO DE HUFFMAN ===\n";
    cout << "Digite uma string para comprimir com Huffman: ";
    
    string entradaUsuario;
    getline(cin, entradaUsuario);

    cout << "\n";
    huffman(entradaUsuario);

    return 0;
}
