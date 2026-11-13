// Disciplina: Sistemas Multimídia - UEPB
// Parte 3 - Huffman Coding
// Referência: https://www.geeksforgeeks.org/dsa/huffman-coding-greedy-algo-3/

// Bibliotecas utilizadas
#include <iostream>   // Entrada e saída de dados (cin e cout)
#include <vector>     // Utilização de vetores
#include <queue>      // Utilização da fila de prioridade
#include <string>     // Manipulação de strings
#include <algorithm>  // Função min() para comparação de valores
#include <map>        // Armazenamento de caracteres e suas frequências
using namespace std;

// Classe que representa um nó da Árvore de Huffman
class Node {
public:
    int data;   // Armazena a frequência 
    int index;  // Armazena o menor índice 
    Node *left, *right; // Ponteiros para os filhos esquerdo e direito

    // Construtor utilizado para criar os nós folha
    Node(int d, int i) {
        data = d;       // Define a frequência
        index = i;      // Define o índice 
        left = right = nullptr; // Inicialmente, o nó não possui filhos
    }

    // Construtor de nós internos
    Node(Node* l, Node* r) {
        data = l->data + r->data; // Soma as frequências dos dois nós filhos
        index = min(l->index, r->index); // Mantém o menor índice para desempate
        left = l;   // Define o primeiro nó como filho esquerdo
        right = r;  // Define o segundo nó como filho direito
    }
};

// Compara dois nós para determinar qual deve ter maior prioridade
class Compare {
public:
    bool operator() (Node* a, Node* b) {

        // Prioriza o nó que possui a menor frequência
        if (a->data != b->data)
            return a->data > b->data;

        // Em caso de frequências iguais, prioriza o menor índice original
        return a->index > b->index;
    }
};

// Percorre a árvore em pré-ordem e gera os códigos binários dos caracteres
void preOrder(Node* root, vector<string> &ans, string curr) {

    // Verifica se o nó atual é nulo; se for, encerra essa chamada
    if (root == nullptr) return;

    // Verifica se o nó atual é uma folha, ou seja, não possui filhos
    if (root->left == nullptr && root->right == nullptr) {

        // Caso exista apenas um caractere na mensagem, atribui o código "0"
        if (curr == "") curr = "0";

        // Armazena o código gerado na posição correspondente ao índice do caractere
        ans[root->index] = curr;
        return;
    }

    // Percorre o filho esquerdo, adicionando 0 ao código atual
    preOrder(root->left, ans, curr + '0');

    // Percorre o filho direito, adicionando 1 ao código atual
    preOrder(root->right, ans, curr + '1');
}

// Percorre a árvore e libera a memória alocada dinamicamente para seus nós
void liberarArvore(Node* root) {

    // Se o nó for nulo, não existe memória para liberar
    if (root == nullptr) return;

    // Libera primeiro a subárvore esquerda
    liberarArvore(root->left);

    // Libera depois a subárvore direita
    liberarArvore(root->right);

    // Libera a memória do nó atual após seus filhos
    delete root;
}

// Constrói a Árvore de Huffman e retorna os códigos binários dos caracteres
vector<string> huffmanCodes(string &s, vector<int> &freq) {

    // Obtém a quantidade de caracteres únicos
    int n = s.length();

    // Cria uma fila de prioridade de nós, organizada pelo comparador Min-Heap
    priority_queue<Node*, vector<Node*>, Compare> pq;

    // Cria um nó folha para cada caractere e insere na fila de prioridade
    for (int i = 0; i < n; i++) {
        Node* tmp = new Node(freq[i], i); // Cria um nó com frequência e índice
        pq.push(tmp); // Insere o nó na fila de prioridade
    }

    // Trata o caso em que existe apenas um caractere diferente na mensagem
    if (n == 1) {
        liberarArvore(pq.top()); // Libera o único nó criado
        return {"0"}; // Retorna o código binário 0
    }

    // Repete o processo enquanto existirem pelo menos dois nós na fila
    while (pq.size() >= 2) {

        // Obtém e remove o nó com a menor frequência
        Node* l = pq.top();
        pq.pop();

        // Obtém e remove o segundo nó com a menor frequência
        Node* r = pq.top();
        pq.pop();

        // Cria um novo nó interno somando as frequências dos dois nós retirados
        Node* newNode = new Node(l, r);

        // Insere o novo nó na fila para participar das próximas combinações
        pq.push(newNode);
    }

    // O único nó restante na fila representa a raiz da árvore de Huffman
    Node* root = pq.top();

    // Cria um vetor para armazenar os códigos de cada caractere
    vector<string> ans(n);

    // Percorre a árvore e gera os códigos: esquerda = 0 e direita = 1
    preOrder(root, ans, "");

    // Libera a memória utilizada pela árvore após gerar os códigos
    liberarArvore(root);

    // Retorna o vetor contendo os códigos de Huffman
    return ans;
}

// Recebe uma mensagem e realiza sua compressão utilizando o algoritmo de Huffman
string huffman(const string &texto) {

    // Verifica se a mensagem informada está vazia
    if (texto.empty()) {
        cout << "Aviso: Nao foi informada nenhuma string para comprimir.\n";
        return "Nenhuma string foi informada.";
    }

    // Cria um mapa para armazenar cada caractere e sua respectiva frequência
    map<char, int> mapa_freq;

    // Percorre a mensagem e contabiliza quantas vezes cada caractere aparece
    for (char c : texto) {
        mapa_freq[c]++;
    }

    // Cria a string dos caracteres únicos e o vetor de suas frequências
    string s = "";
    vector<int> freq;

    // Separa os caracteres e suas frequências em estruturas correspondentes
    for (auto par : mapa_freq) {
        s += par.first;             // Adiciona o caractere à string
        freq.push_back(par.second); // Adiciona sua frequência ao vetor
    }

    // Constrói a árvore de Huffman e obtém os códigos binários
    vector<string> codigos = huffmanCodes(s, freq);

    // Cria um dicionário que relaciona cada caractere ao seu código binário
    map<char, string> dicionario;

    // Associa cada caractere ao código correspondente
    for (size_t i = 0; i < s.length(); i++) {
        dicionario[s[i]] = codigos[i];
    }

    // Cria uma string vazia para armazenar a mensagem codificada
    string mensagemComprimida = "";

    // Percorre a mensagem original e substitui cada caractere pelo seu código
    for (char c : texto) {
        mensagemComprimida += dicionario[c];
    }

    // Exibe a mensagem original
    cout << "Mensagem original : \"" << texto << "\"\n";

    // Exibe o cabeçalho da tabela de caracteres, frequências e códigos
    cout << "\tCaractere\tFrequencia\tCodigo\n";

    // Exibe os dados de cada caractere e seu respectivo código de Huffman
    for (size_t i = 0; i < s.length(); i++) {
        cout << "\t'" << s[i] << "'\t\t"
             << freq[i] << "\t\t"
             << codigos[i] << "\n";
    }

    // Calcula o tamanho original considerando 8 bits por caractere
    int bitsOriginais = texto.length() * 8;

    // Calcula a quantidade de bits da sequência codificada
    int bitsComprimidos = mensagemComprimida.length();

    // Exibe a sequência binária gerada
    cout << "Binario comprimido: " << mensagemComprimida << "\n";

    // Exibe o tamanho estimado da mensagem original
    cout << "Tamanho original  : " << bitsOriginais << " bits (ASCII 8-bit)\n";

    // Exibe o tamanho da sequência codificada
    cout << "Tamanho comprimido: " << bitsComprimidos << " bits\n";

    // Exibe uma linha separadora
    cout << "------------------------------------------------------------\n";

    // Retorna a mensagem codificada
    return mensagemComprimida;
}

// Função principal: inicia a execução do programa
int main() {

    // Exibe o título do algoritmo
    cout << "=== ALGORITMO DE HUFFMAN ===\n";

    // Solicita ao usuário uma mensagem para realizar a compressão
    cout << "Digite uma string para comprimir com Huffman: ";

    // Declara uma variável para armazenar a mensagem digitada
    string entradaUsuario;

    // Lê a mensagem completa, incluindo espaços
    getline(cin, entradaUsuario);

    // Pula uma linha para organizar a saída
    cout << "\n";

    // Chama a função que realiza a compressão da mensagem
    huffman(entradaUsuario);

    // Finaliza o programa indicando execução bem-sucedida
    return 0;
}
