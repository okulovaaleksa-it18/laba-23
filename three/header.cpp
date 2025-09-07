#include "header.h"
#include <iostream>
#include <fstream>
#include <clocale>
#include <limits>
#include <algorithm>
#include <vector>
#include <memory>
#include <sstream>

// Реализация методов класса BinaryTree
BinaryTree::BinaryTree() : root(nullptr) {}

TreeNode* BinaryTree::addNode(TreeNode* node, int value)
{
    if (!node) return new TreeNode(value);
    if (value < node->value)
    {
        node->left = addNode(node->left, value);
    }
    else
    {
        node->right = addNode(node->right, value);
    }
    return node;
}

void BinaryTree::add(int value)
{
    root = addNode(root, value);
}

// Рекурсивная функция для установки камер (динамическое программирование)
State BinaryTree::dfs(TreeNode* node)
{
    if (!node) return COVERED; // Пустые узлы считаем покрытыми

    State left = dfs(node->left);
    State right = dfs(node->right);

    // Если хотя бы один из детей не покрыт, ставим камеру в текущий узел
    if (left == NOT_COVERED || right == NOT_COVERED)
    {
        cameras++;
        return CAMERA;
    }

    // Если хотя бы один из детей имеет камеру, текущий узел покрыт
    if (left == CAMERA || right == CAMERA)
    {
        return COVERED;
    }

    // Иначе текущий узел не покрыт
    return NOT_COVERED;
}

// Основная функция для подсчёта минимального количества камер
int BinaryTree::minCameraCover()
{
    cameras = 0;
    // Если корень не покрыт, ставим камеру в корень
    if (dfs(root) == NOT_COVERED)
    {
        cameras++;
    }
    return cameras;
}

//вывод элементов дерева
void BinaryTree::printTree(TreeNode* node, bool high,
    vector<string> const& lpref,
    vector<string> const& cpref,
    vector<string> const& rpref,
    bool is_root, bool is_left,
    shared_ptr<vector<vector<string>>> lines)
{
    if (!node) return;

    auto VSCat = [](vector<string> const& a, vector<string> const& b) 
        {
        vector<string> r = a;
        r.insert(r.end(), b.begin(), b.end());
        return r;
        };

    if (is_root) lines = make_shared<vector<vector<string>>>();

    if (node->left) 
    {
        printTree(node->left, high,
            VSCat(lpref, high ? vector<string>{" ", " "} : vector<string>{ " " }),
            VSCat(lpref, high ? vector<string>{ch_ddia, ch_ver} : vector<string>{ ch_ddia }),
            VSCat(lpref, high ? vector<string>{ch_hor, " "} : vector<string>{ ch_hor }),
            false, true, lines);
    }

    string sval = to_string(node->value);
    size_t sm = is_left || sval.empty() ? sval.size() / 2 : ((sval.size() + 1) / 2 - 1);

    for (size_t i = 0; i < sval.size(); ++i)
    {
        vector<string> prefix = (i < sm ? lpref : (i == sm ? cpref : rpref));
        lines->push_back(VSCat(prefix, { string(1, sval[i]) }));
    }

    if (node->right) 
    {
        printTree(node->right, high,
            VSCat(rpref, high ? vector<string>{ch_hor, " "} : vector<string>{ ch_hor }),
            VSCat(rpref, high ? vector<string>{ch_rddia, ch_ver} : vector<string>{ ch_rddia }),
            VSCat(rpref, high ? vector<string>{" ", " "} : vector<string>{ " " }),
            false, false, lines);
    }

    if (is_root) 
    {
        vector<string> out;
        for (size_t l = 0;; ++l) 
        {
            bool last = true;
            string line;
            for (const auto& vec : *lines) 
            {
                if (l < vec.size()) 
                {
                    line += vec[l];
                    last = false;
                }
                else 
                {
                    line += " ";
                }
            }
            if (last) break;
            out.push_back(line);
        }
        for (const auto& s : out) cout << s << endl;
    }
}
//графический вывод дерева
void BinaryTree::printPretty()
{
    printTree(root, true, {}, {}, {}, true, false, nullptr);
}

TreeNode* BinaryTree::getRoot()
{
    return root;
}

//Проверка корректности кода
int getIntInput(const string& prompt)
{
    string input;
    int value;
    while (true)
    {
        cout << prompt;
        getline(cin, input);

        // Удаляем пробелы в начале и конце
        input.erase(0, input.find_first_not_of(" \t"));
        input.erase(input.find_last_not_of(" \t") + 1);

        // Проверяем каждый символ
        bool is_valid = !input.empty();
        bool has_sign = false;

        for (size_t i = 0; i < input.size(); i++)
        {
            char c = input[i];
            if (i == 0 && (c == '-' || c == '+'))
            {
                has_sign = true;
                continue;
            }
            if (!isdigit(c)) 
            {
                is_valid = false;
                break;
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
        catch (const out_of_range&) 
        {
            cout << "Ошибка ввода. Число выходит за допустимый диапазон.\n";
        }
        catch (...)
        {
            cout << "Ошибка ввода. Некорректное число.\n";
        }
    }
}

bool korrec(const string& a)
{
    if (a.empty()) return false;
    for (char c : a) if (!isdigit(c)) return false;
    return true;
}

void userInput(BinaryTree& tree)
{
    int n = getIntInput("Введите количество элементов: ");
    for (int i = 0; i < n; i++)
    {
        int value = getIntInput("Введите ключ: ");
        tree.add(value);
    }
}

void randomInput(BinaryTree& tree) 
{
    srand(static_cast<unsigned>(time(0)));
    int n = getIntInput("Введите количество элементов: ");
    for (int i = 0; i < n; i++)
    {
        tree.add(rand() % 100);
    }
}

void fileInput(BinaryTree& tree)
{
    ifstream f("laba23.txt");
    if (!f.is_open()) 
    {
        cout << "Не удалось открыть файл!" << endl;
        return;
    }

    string line;
    while (getline(f, line))
    {
        // Удаляем пробелы в начале и конце строки
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        if (line.empty()) continue; // Пропускаем пустые строки

        istringstream iss(line);
        int value;
        while (iss >> value)
        {
            tree.add(value);

            // Проверяем, не осталось ли мусора после числа
            if (iss.fail()) 
            {
                cout << "Ошибка: обнаружены некорректные данные в строке: " << line << endl;
                break;
            }
        }

        if (!iss.eof() && iss.fail())
        {
            cout << "Ошибка: в строке обнаружены некорректные символы: " << line << endl;
        }
    }
    f.close();
}
