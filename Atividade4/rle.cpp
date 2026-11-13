// Disciplina: Sistemas Multimídia - UEPB
// Parte 1 - Run-Length Encoding (RLE)
// Referência: https://www.geeksforgeeks.org/dsa/run-length-encoding/

#include <iostream>
#include <string>
using namespace std;

// Função solicitada: recebe uma string texto e retorna sua representação comprimida via RLE
string rle(const string &texto)
{
    if (texto.empty()) {
        return "Nenhuma string foi informada.";
    }

    string result = "";
    int n = texto.length();

    for (int i = 0; i < n; i++) {
        // Conta as ocorrências consecutivas do caractere atual
        int count = 1;
        while (i < n - 1 && texto[i] == texto[i + 1]) {
            count++;
            i++;
        }

        // Concatena o caractere e sua contagem ao resultado
        result += texto[i];
        result += to_string(count);
    }

    return result;
}

void executarTesteRLE(const string &texto)
{
    if (texto.empty()) {
        cout << "Aviso: Nao foi informada nenhuma string para comprimir.\n";
        return;
    }

    string comprimido = rle(texto);
    cout << "Entrada Original : \"" << texto << "\" (" << texto.length() << " caracteres)\n";
    cout << "Saida RLE        : \"" << comprimido << "\" (" << comprimido.length() << " caracteres)\n";
    cout << "------------------------------------------------------------\n";
}

int main()
{
    cout << "=== RUN-LENGTH ENCODING (RLE) ===\n";
    cout << "Digite uma string para comprimir com RLE: ";
    
    string entradaUsuario;
    getline(cin, entradaUsuario);

    cout << "\n";
    executarTesteRLE(entradaUsuario);

    return 0;
}
