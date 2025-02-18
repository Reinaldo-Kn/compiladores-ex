#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

// Enumeração para os tokens possíveis
enum TokenType
{
    TOKEN_INT,
    TOKEN_CHAR,
    TOKEN_FOR,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_RETURN,
    TOKEN_ID,
    TOKEN_NUM,
    TOKEN_LETRA,
    TOKEN_ATRIB,
    TOKEN_IGUAL,
    TOKEN_PEV,
    TOKEN_LPAR,
    TOKEN_RPAR,
    TOKEN_LCHAVES,
    TOKEN_RCHAVES,
    TOKEN_MAISMAIS,
    TOKEN_MOD,
    TOKEN_MAIS,
    TOKEN_MENOR,
    TOKEN_MAIOR,
    TOKEN_ELSEIF,
    TOKEN_OUTRO
};

// Definicao de tipos
typedef pair<TokenType, string> Token;
typedef vector<Token> Tokens;

// Protótipos de funções
bool isKeyword(const string &s);
bool isIdentifier(const string &s);
bool isNumber(const string &s);
bool isOperator(const string &s);
bool isSeparator(const string &s);
bool isCharLiteral(const string &s);

// Função principal
int main(int argc, char *argv[])
{
    // Verifica se o arquivo foi passado como argumento
    if (argc != 2)
    {
        cerr << "Uso: " << argv[0] << " arquivo.c" << endl;
        return 1;
    }

    // Abre o arquivo
    ifstream file(argv[1]);
    if (!file.is_open())
    {
        cerr << "Erro ao abrir o arquivo " << argv[1] << endl;
        return 1;
    }

    // Palavras-chave
    set<string> keywords = {"int", "char", "for", "if", "else", "return"};

    // Tokens
    Tokens tokens;

    // Leitura do arquivo
    string line;
    while (getline(file, line))
    {
        // Tokenização
        string token;
        for (size_t i = 0; i < line.size(); i++)
        {
            if (isspace(line[i]))
            {
                if (!token.empty())
                {
                    // Verifica o tipo do token
                    if (isKeyword(token))
                    {
                        if (token == "int")
                            tokens.push_back({TOKEN_INT, ""});
                        else if (token == "char")
                            tokens.push_back({TOKEN_CHAR, ""});
                        else if (token == "for")
                            tokens.push_back({TOKEN_FOR, ""});
                        else if (token == "if")
                            tokens.push_back({TOKEN_IF, ""});
                        else if (token == "else")
                            tokens.push_back({TOKEN_ELSE, ""});
                        else if (token == "return")
                            tokens.push_back({TOKEN_RETURN, ""});
                    }
                    else if (isIdentifier(token))
                    {
                        tokens.push_back({TOKEN_ID, token});
                    }
                    else if (isNumber(token))
                    {
                        tokens.push_back({TOKEN_NUM, token});
                    }
                    else if (isCharLiteral(token))
                    {
                        tokens.push_back({TOKEN_LETRA, token});
                    }
                    token.clear();
                }
            }
            else if (isSeparator(string(1, line[i])) || isOperator(string(1, line[i])))
            {
                if (!token.empty())
                {
                    if (isIdentifier(token))
                        tokens.push_back({TOKEN_ID, token});
                    else if (isNumber(token))
                        tokens.push_back({TOKEN_NUM, token});
                    else if (isCharLiteral(token))
                        tokens.push_back({TOKEN_LETRA, token});
                    token.clear();
                }
                // Adiciona o separador ou operador como token
                string s(1, line[i]);
                if (s == ";")
                    tokens.push_back({TOKEN_PEV, ""});
                else if (s == "(")
                    tokens.push_back({TOKEN_LPAR, ""});
                else if (s == ")")
                    tokens.push_back({TOKEN_RPAR, ""});
                else if (s == "{")
                    tokens.push_back({TOKEN_LCHAVES, ""});
                else if (s == "}")
                    tokens.push_back({TOKEN_RCHAVES, ""});
                else if (s == "=")
                {
                    // Verifica se é "=="
                    if (i + 1 < line.size() && line[i + 1] == '=')
                    {
                        tokens.push_back({TOKEN_IGUAL, ""});
                        i++; // Avança para o próximo caractere
                    }
                    else
                    {
                        tokens.push_back({TOKEN_ATRIB, ""});
                    }
                }
                else if (s == "+")
                {
                    // Verifica se é "++"
                    if (i + 1 < line.size() && line[i + 1] == '+')
                    {
                        tokens.push_back({TOKEN_MAISMAIS, ""});
                        i++; // Avança para o próximo caractere
                    }
                    else
                    {
                        tokens.push_back({TOKEN_MAIS, ""});
                    }
                }
                else if (s == "%")
                    tokens.push_back({TOKEN_MOD, ""});
                else if (s == "<")
                    tokens.push_back({TOKEN_MENOR, ""});
                else if (s == ">")
                    tokens.push_back({TOKEN_MAIOR, ""});
            }
            else
            {
                // Trata números negativos
                if (line[i] == '-' && isdigit(line[i + 1]))
                {
                    if (!token.empty())
                    {
                        if (isIdentifier(token))
                            tokens.push_back({TOKEN_ID, token});
                        else if (isNumber(token))
                            tokens.push_back({TOKEN_NUM, token});
                        else if (isCharLiteral(token))
                            tokens.push_back({TOKEN_LETRA, token});
                        token.clear();
                    }
                    token += line[i]; // Adiciona o sinal de negativo
                }
                else
                {
                    token += line[i];
                }
            }
        }
        if (!token.empty())
        {
            if (isIdentifier(token))
                tokens.push_back({TOKEN_ID, token});
            else if (isNumber(token))
                tokens.push_back({TOKEN_NUM, token});
            else if (isCharLiteral(token))
                tokens.push_back({TOKEN_LETRA, token});
        }
    }

    // Verifica se há "else if" e substitui por <ELSEIF>
    for (size_t i = 0; i + 1 < tokens.size(); i++)
    {
        if (tokens[i].first == TOKEN_ELSE && tokens[i + 1].first == TOKEN_IF)
        {
            tokens[i] = {TOKEN_ELSEIF, ""};
            tokens.erase(tokens.begin() + i + 1); // Remove o token "if"
        }
    }

    // Imprime os tokens
    for (const auto &t : tokens)
    {
        switch (t.first)
        {
        case TOKEN_INT:
            cout << "<INT> ";
            break;
        case TOKEN_CHAR:
            cout << "<CHAR> ";
            break;
        case TOKEN_FOR:
            cout << "<FOR> ";
            break;
        case TOKEN_IF:
            cout << "<IF> ";
            break;
        case TOKEN_ELSE:
            cout << "<ELSE> ";
            break;
        case TOKEN_RETURN:
            cout << "<RETURN> ";
            break;
        case TOKEN_ID:
            cout << "<ID, \"" << t.second << "\"> ";
            break;
        case TOKEN_NUM:
            cout << "<NUM, \"" << t.second << "\"> ";
            break;
        case TOKEN_LETRA:
            cout << "<LETRA, \"" << t.second << "\"> ";
            break;
        case TOKEN_ATRIB:
            cout << "<ATRIB> ";
            break;
        case TOKEN_IGUAL:
            cout << "<IGUAL> ";
            break;
        case TOKEN_PEV:
            cout << "<PEV> ";
            break;
        case TOKEN_LPAR:
            cout << "<LPAR> ";
            break;
        case TOKEN_RPAR:
            cout << "<RPAR> ";
            break;
        case TOKEN_LCHAVES:
            cout << "<LCHAVES> ";
            break;
        case TOKEN_RCHAVES:
            cout << "<RCHAVES> ";
            break;
        case TOKEN_MAISMAIS:
            cout << "<MAISMAIS> ";
            break;
        case TOKEN_MOD:
            cout << "<MOD> ";
            break;
        case TOKEN_MAIS:
            cout << "<MAIS> ";
            break;
        case TOKEN_MENOR:
            cout << "<MENOR> ";
            break;
        case TOKEN_MAIOR:
            cout << "<MAIOR> ";
            break;
        case TOKEN_ELSEIF:
            cout << "<ELSEIF> ";
            break;
        default:
            cout << "<OUTRO> ";
            break;
        }
    }
    cout << endl;

    return 0;
}

// Implementações das funções auxiliares
bool isKeyword(const string &s)
{
    static const set<string> keywords = {"int", "char", "for", "if", "else", "return"};
    return keywords.find(s) != keywords.end();
}

bool isIdentifier(const string &s)
{
    if (s.empty() || !isalpha(s[0]) && s[0] != '_')
        return false;
    for (char ch : s)
    {
        if (!isalnum(ch) && ch != '_')
            return false;
    }
    return true;
}

bool isNumber(const string &s)
{
    if (s.empty())
        return false;
    size_t start = (s[0] == '-') ? 0 : 0; // Considera o sinal de negativo
    for (size_t i = start; i < s.size(); i++)
    {
        if (!isdigit(s[i]))
            return false;
    }
    return true;
}

bool isCharLiteral(const string &s)
{
    return s.size() >= 3 && s.front() == '\'' && s.back() == '\'';
}

bool isSeparator(const string &s)
{
    static const set<string> separators = {";", "(", ")", "{", "}", "[", "]"};
    return separators.find(s) != separators.end();
}

bool isOperator(const string &s)
{
    static const set<string> operators = {"=", "+", "-", "*", "/", "%", "==", "!=", "<", ">", "<=", ">=", "&&", "||", "!", "&", "|", "^", "~", "<<", ">>", "++", "--", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<=", ">>="};
    return operators.find(s) != operators.end();
}