#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <queue>
#include <algorithm>
#include <stdexcept>

using namespace std;

const string ch_hor = "-";
const string ch_ver = "|";
const string ch_ddia = "/";
const string ch_rddia = "\\";

struct TreeNode {
    int value;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int val) : value(val), left(nullptr), right(nullptr) {}
};

class GeneralBinaryTree {
private:
    TreeNode* root;

    // Итеративные методы
    void clearTree(TreeNode* node);
    TreeNode* insert(TreeNode* node, int value);
    void printReverse(TreeNode* node);

    // Метод для графического вывода (оставлен с рекурсией)
    void dump4(TreeNode* node, bool high, vector<string> const& lpref,
        vector<string> const& cpref, vector<string> const& rpref,
        bool is_root, bool is_left, shared_ptr<vector<vector<string>>> lines);

public:
    GeneralBinaryTree() : root(nullptr) {}
    ~GeneralBinaryTree() { clearTree(root); }

    // Основные операции
    void insert(int value) { root = insert(root, value); }
    void clear() { clearTree(root); root = nullptr; }
    void dump() { dump4(root, true, {}, {}, {}, true, false, nullptr); }

    // Обходы дерева
    void morrisPostOrder();  // Лево-право-корень без рекурсии и стека

    // Итератор (пост-порядок)
    class Iterator {
    private:
        vector<TreeNode*> nodes;
        vector<bool> visited;
        TreeNode* current;

    public:
        Iterator(TreeNode* root);
        bool hasNext();
        int next();
    };

    Iterator getIterator() { return Iterator(root); }
};

// Вспомогательные функции ввода
int getIntInput(const string& prompt);
void consoleInput(GeneralBinaryTree& tree);
void fileInput(GeneralBinaryTree& tree);
void randomInput(GeneralBinaryTree& tree);
