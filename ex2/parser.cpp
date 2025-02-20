#include <iostream>
#include <string>

using namespace std;

enum TokenType
{
    TOKEN_I,
    TOKEN_N,
    TOKEN_IGUAL,
    TOKEN_E,
    TOKEN_O,
    TOKEN_ABRE_PAREN,
    TOKEN_FECHA_PAREN,
    TOKEN_FIM,
    TOKEN_ERRO
};

struct Token
{
    TokenType type;
    char value;
};

string entrada;
size_t pos = 0;
Token tokenAtual;

void A();
void E();
void E_();
void D();
void D_();
void L();
void consumir(TokenType esperado);
Token getNextToken();

Token getNextToken()
{
    Token token;

    if (pos >= entrada.length())
    {
        token.type = TOKEN_FIM;
        token.value = '\0';
        return token;
    }

    char c = entrada[pos];

    while (c == ' ' && pos < entrada.length())
    { // Ignorar espaços
        pos++;
        c = entrada[pos];
    }

    if (c == 'i')
    {
        token.type = TOKEN_I;
    }
    else if (c == 'n')
    {
        token.type = TOKEN_N;
    }
    else if (c == '=')
    {
        token.type = TOKEN_IGUAL;
    }
    else if (c == 'e')
    {
        token.type = TOKEN_E;
    }
    else if (c == 'o')
    {
        token.type = TOKEN_O;
    }
    else if (c == '(')
    {
        token.type = TOKEN_ABRE_PAREN;
    }
    else if (c == ')')
    {
        token.type = TOKEN_FECHA_PAREN;
    }
    else
    {
        token.type = TOKEN_ERRO;
        token.value = c;
        return token;
    }

    token.value = c;
    pos++;
    return token;
}

void consumir(TokenType esperado)
{
    if (tokenAtual.type == esperado)
    {
        tokenAtual = getNextToken();
    }
    else
    {
        cout << "erro sintatico" << endl;
        exit(1);
    }
}

void A()
{
    consumir(TOKEN_I);
    consumir(TOKEN_IGUAL);
    E();
}

void E()
{
    D();
    E_();
}

void E_()
{
    if (tokenAtual.type == TOKEN_E)
    {
        consumir(TOKEN_E);
        D();
        E_();
    }
}

void D()
{
    L();
    D_();
}

void D_()
{
    if (tokenAtual.type == TOKEN_O)
    {
        consumir(TOKEN_O);
        L();
        D_();
    }
}

void L()
{
    if (tokenAtual.type == TOKEN_I)
    {
        consumir(TOKEN_I);
    }
    else if (tokenAtual.type == TOKEN_N)
    {
        consumir(TOKEN_N);
        if (tokenAtual.type == TOKEN_I)
        {
            consumir(TOKEN_I);
        }
        else if (tokenAtual.type == TOKEN_ABRE_PAREN)
        {
            consumir(TOKEN_ABRE_PAREN);
            E();
            consumir(TOKEN_FECHA_PAREN);
        }
        else
        {
            cout << "erro sintatico" << endl;
            exit(1);
        }
    }
    else if (tokenAtual.type == TOKEN_ABRE_PAREN)
    {
        consumir(TOKEN_ABRE_PAREN);
        E();
        consumir(TOKEN_FECHA_PAREN);
    }
    else
    {
        cout << "erro sintatico" << endl;
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Uso: ./parser \"<expressao>\"" << endl;
        return 1;
    }

    entrada = argv[1];
    pos = 0;
    tokenAtual = getNextToken();

    A();

    if (tokenAtual.type == TOKEN_FIM)
    {
        cout << "entrada valida" << endl;
    }
    else
    {
        cout << "erro sintatico" << endl;
    }

    return 0;
}
