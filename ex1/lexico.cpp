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
    TOKEN_MULT,
    TOKEN_DIV,
    TOKEN_MENOR,
    TOKEN_MAIOR,
    TOKEN_ELSEIF,
    TOKEN_OUTRO
};

typedef pair<TokenType, string> Token;
typedef vector<Token> Tokens;

bool isKeyword(const string &s);
bool isIdentifier(const string &s);
bool isNumber(const string &s);
bool isOperator(const string &s);
bool isSeparator(const string &s);
bool isCharLiteral(const string &s);
Tokens tokenize(const string &line, int lineNumber);

int main(int argc, char *argv[])

{
    if (argc != 2)
    {
        cerr << "Uso: " << argv[0] << " arquivo.c" << endl;
        return 1;
    }

    ifstream file(argv[1]);
    if (!file.is_open())
    {
        cerr << "Erro ao abrir o arquivo " << argv[1] << endl;
        return 1;
    }

    Tokens tokens;
    string line;
    int lineNumber = 1;

    while (getline(file, line))
    {
        Tokens lineTokens = tokenize(line, lineNumber);
        if (!lineTokens.empty() && lineTokens.back().first == TOKEN_OUTRO)
        {
            return 1;
        }
        tokens.insert(tokens.end(), lineTokens.begin(), lineTokens.end());
        lineNumber++;
    }

    for (size_t i = 0; i + 1 < tokens.size(); i++)
    {
        if (tokens[i].first == TOKEN_ELSE && tokens[i + 1].first == TOKEN_IF)
        {
            tokens[i] = {TOKEN_ELSEIF, ""};
            tokens.erase(tokens.begin() + i + 1);
        }
    }

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
        case TOKEN_MULT:
            cout << "<MULT> ";
            break;
        case TOKEN_DIV:
            cout << "<DIV> ";
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

bool isKeyword(const string &s)
{
    static const set<string> keywords = {"int", "char", "for", "if", "else", "return"};
    return keywords.find(s) != keywords.end();
}

bool isIdentifier(const string &s)
{
    if (s.empty() || (!isalpha(s[0]) && s[0] != '_'))
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
    size_t start = (s[0] == '-') ? 1 : 0;
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

Tokens tokenize(const string &line, int lineNumber)
{
    Tokens tokens;
    string token;
    bool negative = false;

    for (size_t i = 0; i < line.size(); i++)
    {
        char c = line[i];

        if (isspace(c))
        {
            if (!token.empty())
            {
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
                    if (negative)
                    {
                        tokens.push_back({TOKEN_NUM, "-" + token});
                        negative = false;
                    }
                    else
                    {
                        tokens.push_back({TOKEN_NUM, token});
                    }
                }
                else if (isCharLiteral(token))
                {
                    tokens.push_back({TOKEN_LETRA, token});
                }
                token.clear();
            }
        }
        else if (isSeparator(string(1, c)) || isOperator(string(1, c)))
        {
            if (!token.empty())
            {
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

            string s(1, c);
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
                if (i + 1 < line.size() && line[i + 1] == '=')
                {
                    tokens.push_back({TOKEN_IGUAL, ""});
                    i++;
                }
                else
                {
                    tokens.push_back({TOKEN_ATRIB, ""});
                }
            }
            else if (s == "+")
            {
                if (i + 1 < line.size() && line[i + 1] == '+')
                {
                    tokens.push_back({TOKEN_MAISMAIS, ""});
                    i++;
                }
                else
                {
                    tokens.push_back({TOKEN_MAIS, ""});
                }
            }
            else if (c == '-')
            {
                if (token.empty())
                {
                    negative = true;
                    token += c;
                }
                else if (isNumber(token))
                {
                    if (negative)
                    {
                        tokens.push_back({TOKEN_NUM, "-" + token});
                    }
                    else
                    {
                        tokens.push_back({TOKEN_NUM, token});
                    }
                    token.clear();
                    negative = false;
                    string s(1, c);
                    tokens.push_back({TOKEN_MAIS, ""});
                }
            }

            else if (s == "*")
                tokens.push_back({TOKEN_MULT, ""});
            else if (s == "/")
                tokens.push_back({TOKEN_DIV, ""});
            else if (s == "%")
                tokens.push_back({TOKEN_MOD, ""});
            else if (s == "<")
                tokens.push_back({TOKEN_MENOR, ""});
            else if (s == ">")
                tokens.push_back({TOKEN_MAIOR, ""});
        }
        else if (!isalnum(c) && c != '_' && c != '\'')
        {
            cerr << "Erro Lexico: “" << c << "” nao reconhecido na linha " << lineNumber << "." << endl;
            tokens.push_back({TOKEN_OUTRO, ""});
            return tokens;
        }
        else
        {
            token += c;
        }
    }

    if (!token.empty())
    {
        if (isKeyword(token))
        { // Keyword check last token
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
    }
    return tokens;
}