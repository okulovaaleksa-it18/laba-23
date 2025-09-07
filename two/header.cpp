#include "header.h"
#include <queue>
#include <algorithm> 

// Удаление дерева
void GeneralBinaryTree::clearTree(TreeNode* node)
{
    if (!node) return;

    queue<TreeNode*> nodesToDelete;
    nodesToDelete.push(node);

    while (!nodesToDelete.empty())
    {
        TreeNode* current = nodesToDelete.front();
        nodesToDelete.pop();

        if (current->left)
        {
            nodesToDelete.push(current->left);
        }
        if (current->right)
        {
            nodesToDelete.push(current->right);
        }

        delete current;
    }
}

// Вставка
TreeNode* GeneralBinaryTree::insert(TreeNode* node, int value)
{
    TreeNode* newNode = new TreeNode(value);
    if (!node)
    {
        return newNode;
    }

    TreeNode* current = node;
    TreeNode* parent = nullptr;

    while (current)
    {
        parent = current;
        if (value < current->value)
        {
            current = current->left;
        }
        else if (value > current->value)
        {
            current = current->right;
        }
        else
        {
            // Если значение уже существует
            delete newNode;
            return node;
        }
    }

    if (value < parent->value)
    {
        parent->left = newNode;
    }
    else
    {
        parent->right = newNode;
    }

    return node;
}

// Вспомогательная функция для разворота правой ветви
void GeneralBinaryTree::printReverse(TreeNode* node)
{
    vector<int> values;
    while (node)
    {
        values.push_back(node->value);
        node = node->right;
    }
    reverse(values.begin(), values.end());
    for (int val : values)
    {
        cout << val << " ";
    }
}

// Пост-порядковый обход (лево-право-корень) 
void GeneralBinaryTree::morrisPostOrder()
{
    TreeNode dummy(0);
    dummy.left = root;
    TreeNode* current = &dummy;

    while (current)
    {
        if (!current->left)
        {
            current = current->right;
        }
        else
        {
            TreeNode* predecessor = current->left;
            while (predecessor->right && predecessor->right != current)
            {
                predecessor = predecessor->right;
            }

            if (!predecessor->right)
            {
                predecessor->right = current;
                current = current->left;
            }
            else
            {
                predecessor->right = nullptr;
                printReverse(current->left);
                current = current->right;
            }
        }
    }
}


//Графический вывод дерева
void GeneralBinaryTree::dump4(TreeNode* node, bool high, vector<string> const& lpref, vector<string> const& cpref, vector<string> const& rpref, bool is_root, bool is_left, shared_ptr<vector<vector<string>>> lines)
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

    string sval = to_string(node->value);
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
        vector<string> out;
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
// Реализация итератора (post-order обход)
GeneralBinaryTree::Iterator::Iterator(TreeNode* root)
{
    current = root;
    // Инициализируем стек для эмуляции post-order обхода
    while (current)
    {
        nodes.push_back(current);
        visited.push_back(false);
        current = current->left;
    }
}

// Проверка наличия следующего элемента
bool GeneralBinaryTree::Iterator::hasNext()
{
    return !nodes.empty();
}

// Получение следующего элемента
int GeneralBinaryTree::Iterator::next()
{
    while (!nodes.empty())
    {
        current = nodes.back();
        bool is_visited = visited.back();

        if (!is_visited)
        {
            // Переходим к правому поддереву
            visited.back() = true;
            TreeNode* temp = current->right;
            while (temp)
            {
                nodes.push_back(temp);
                visited.push_back(false);
                temp = temp->left;
            }
        }
        else
        {
            // Возвращаем значение узла
            nodes.pop_back();
            visited.pop_back();
            return current->value;
        }
    }
    throw runtime_error("Много элементов");
}


int getIntInput(const string& prompt)
{
    string input;
    int value;
    while (true)
    {
        cout << prompt;
        getline(cin >> ws, input);

        input.erase(0, input.find_first_not_of(" \t"));
        input.erase(input.find_last_not_of(" \t") + 1);
        bool is_valid = true;
        for (char c : input) {
            if (!isdigit(c) && c != '-' && c != '+')
            {
                is_valid = false;
                break;
            }
        }

        if (is_valid && input.size() > 1)
        {
            for (size_t i = 1; i < input.size(); i++)
            {
                if (!isdigit(input[i]))
                {
                    is_valid = false;
                    break;
                }
            }
        }

        if (!is_valid)
        {
            cout << "Ошибка ввода. Введите целое число без посторонних символов.\n";
            continue;
        }

        try
        {
            value = stoi(input);
            return value;
        }
        catch (...)
        {
            cout << "Ошибка ввода. Число слишком большое или некорректное.\n";
        }
    }
}

void consoleInput(GeneralBinaryTree& tree)
{
    int n = getIntInput("Введите количество элементов: ");
    for (int i = 0; i < n; ++i)
    {
        int value = getIntInput("Введите элемент " + to_string(i + 1) + ": ");
        tree.insert(value);
    }
}

void fileInput(GeneralBinaryTree& tree)
{
    ifstream file("laba23.2.txt");
    if (!file.is_open())
    {
        throw runtime_error("Не удалось открыть файл");
    }
    int value;
    while (file >> value)
    {
        tree.insert(value);
    }
    file.close();
}

void randomInput(GeneralBinaryTree& tree)
{
    srand(time(0));
    int n = getIntInput("Введите количество элементов: ");
    for (int i = 0; i < n; ++i) {
        tree.insert(rand() % 1001);
    }
}
