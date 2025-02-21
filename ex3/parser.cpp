#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum Token
{
    TK_N,
    TK_PLUS
};

Token getToken(char c)
{
    if (c == 'n')
    {
        return TK_N;
    }
    if (c == '+')
    {
        return TK_PLUS;
    }
    return TK_N;
}

vector<Token> tokenize(const string &input)
{
    vector<Token> tokens;
    for (char c : input)
    {
        tokens.push_back(getToken(c));
    }
    return tokens;
}

bool parseE(const vector<Token> &tokens, int l, int r)
{
    if (r - l == 1 && tokens[l] == TK_N)
    {
        return true;
    }
    if (r - l < 2 || tokens[r - 1] != TK_PLUS)
    {
        return false;
    }

    for (int split = l + 1; split < r - 1; ++split)
    {
        if (parseE(tokens, l, split) && parseE(tokens, split, r - 1))
        {
            return true;
        }
    }
    return false;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Uso: " << argv[0] << " <entrada>" << endl;
        return 1;
    }

    string input = argv[1];

    vector<Token> tokens = tokenize(input);

    bool valid = parseE(tokens, 0, tokens.size());
    cout << (valid ? "entrada valida" : "erro sintatico") << endl;

    return 0;
}
