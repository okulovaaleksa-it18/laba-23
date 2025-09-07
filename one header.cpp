#include "header.h"

bool InputUtils::validateInt(const string& input)
{
    if (input.empty()) return false;
    size_t start = 0;
    if (input[0] == '-')
    {
        if (input.length() == 1) return false;
        start = 1;
    }
    return all_of(input.begin() + start, input.end(), ::isdigit);
}

int InputUtils::getInt(const string& prompt)
{
    string input;
    while (true)
    {
        if (!prompt.empty()) cout << prompt;
        getline(cin, input);

        input.erase(0, input.find_first_not_of(" \t"));
        input.erase(input.find_last_not_of(" \t") + 1);

        if (validateInt(input))
        {
            try
            {
                return stoi(input);
            }
            catch (...)
            {
                cout << "Число слишком большое. Пожалуйста, введите другое число: ";
            }
        }
        else
        {
            cout << "Некорректный ввод. Пожалуйста, введите целое число: ";
        }
    }
}

Tree::Tree(int val) : key(val), left(nullptr), right(nullptr) {}

Tree::~Tree()
{
    delete left;
    delete right;
}

Tree* Tree::createNode(int val)
{
    return new Tree(val);
}

Tree* Tree::buildExpressionTree(const vector<int>& tokens)
{
    stack<Tree*> st; //стек для построения дерева выражения  
    for (int token : tokens)
    {
        if (token >= 0)
        {
            st.push(createNode(token));
        }
        else
        {
            if (st.size() < 2)
            {
                cerr << "Ошибка: недостаточно операндов для операции" << endl;
                return nullptr;
            }
            Tree* node = createNode(token);
            node->right = st.top(); st.pop();
            node->left = st.top(); st.pop();
            st.push(node);
        }
    }

    if (st.size() != 1)
    {
        cerr << "Ошибка: некорректное выражение" << endl;
        return nullptr;
    }

    return st.top();
}

//Вычисление значения поддерева
int Tree::evaluateSubtree(Tree* node)
{
    if (!node) return 0;
    if (node->key >= 0) return node->key;

    int leftVal = evaluateSubtree(node->left);
    int rightVal = evaluateSubtree(node->right);

    switch (node->key)
    {
    case -1: return leftVal + rightVal;
    case -2: return leftVal - rightVal;
    case -3: return leftVal * rightVal;
    default: return 0;
    }
}

//Замена вычитания на результат 
void Tree::replaceSubtraction(Tree* node)
{
    if (!node) return;

    if (node->key == -2)
    {
        int result = evaluateSubtree(node);
        node->key = result;
        delete node->left;
        delete node->right;
        node->left = nullptr;
        node->right = nullptr;
        return;
    }

    replaceSubtraction(node->left);
    replaceSubtraction(node->right);
}

bool Tree::validateToken(const string& token)
{
    if (token.empty()) return false;
    if (token == "+" || token == "-" || token == "*") return true;
    return InputUtils::validateInt(token);
}

//разбор строки в опз в вектор 
vector<int> Tree::parseRPN(const string& input)
{
    vector<int> tokens;
    stringstream ss(input);
    string token;

    while (ss >> token)
    {
        if (!validateToken(token))
        {
            cerr << "Ошибка: некорректный токен '" << token << "'" << endl;
            return {};
        }

        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1])))
        {
            tokens.push_back(stoi(token));
        }
        else if (token == "+") tokens.push_back(-1);
        else if (token == "-") tokens.push_back(-2);
        else if (token == "*") tokens.push_back(-3);
    }

    return tokens;
}

void Tree::dump4(const Tree* node, bool high,
    const vector<string>& lpref,
    const vector<string>& cpref,
    const vector<string>& rpref,
    bool is_root, bool is_left,
    shared_ptr<vector<vector<string>>> lines)
{
    if (!node) return;

    using VS = vector<string>;
    auto VSCat = [](const VS& a, const VS& b)
        {
            VS r = a;
            r.insert(r.end(), b.begin(), b.end());
            return r;
        };

    if (is_root)
        lines = make_shared<vector<VS>>();

    if (node->left)
        dump4(node->left, high,
            VSCat(lpref, high ? VS{ " ", " " } : VS{ " " }),
            VSCat(lpref, high ? VS{ ch_ddia, ch_ver } : VS{ ch_ddia }),
            VSCat(lpref, high ? VS{ ch_hor, " " } : VS{ ch_hor }),
            false, true, lines);

    string sval;
    if (node->key >= 0) sval = to_string(node->key);
    else if (node->key == -1) sval = "+";
    else if (node->key == -2) sval = "-";
    else if (node->key == -3) sval = "*";

    size_t sm = is_left || sval.empty() ? sval.size() / 2 : ((sval.size() + 1) / 2 - 1);
    for (size_t i = 0; i < sval.size(); ++i)
        lines->push_back(VSCat(i < sm ? lpref : i == sm ? cpref : rpref, { string(1, sval[i]) }));

    if (node->right)
        dump4(node->right, high,
            VSCat(rpref, high ? VS{ ch_hor, " " } : VS{ ch_hor }),
            VSCat(rpref, high ? VS{ ch_rddia, ch_ver } : VS{ ch_rddia }),
            VSCat(rpref, high ? VS{ " ", " " } : VS{ " " }),
            false, false, lines);

    if (is_root)
    {
        VS out;
        for (size_t l = 0;; ++l)
        {
            bool last = true;
            string line;
            for (const auto& vec : *lines)
                if (l < vec.size())
                {
                    line += vec[l];
                    last = false;
                }
                else line += " ";
            if (last) break;
            out.push_back(line);
        }
        for (const auto& s : out)
            cout << s << endl;
    }
}

Tree* Tree::inputFromFile(string filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Ошибка открытия файла: " << filename << endl;
        return nullptr;
    }

    string input;
    getline(file, input);
    file.close();

    vector<int> tokens = parseRPN(input);
    if (tokens.empty())
    {
        cerr << "Не удалось разобрать выражение из файла." << endl;
        return nullptr;
    }
    return buildExpressionTree(tokens);
}

Tree* Tree::inputFromConsole()
{
    cout << "Введите выражение в обратной польской записи (например: '5 3 - 2 *'): ";
    string input;
    getline(cin, input);

    vector<int> tokens = parseRPN(input);
    if (tokens.empty())
    {
        cerr << "Не удалось разобрать введенное выражение." << endl;
        return nullptr;
    }
    return buildExpressionTree(tokens);
}

void Tree::deleteTree(Tree* root)
{
    delete root;
}
