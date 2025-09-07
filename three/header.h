#pragma once
#include <set>
#include <iomanip>
#include <iostream>
#include <vector>
#include <string>
#include <memory>

using namespace std;

const string ch_hor = "-";
const string ch_ver = "|";
const string ch_ddia = "/";
const string ch_rddia = "\\";

// Состояния узла
enum State { NOT_COVERED, COVERED, CAMERA };

struct TreeNode
{
    int value;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int val) : value(val), left(nullptr), right(nullptr) {}
};

class BinaryTree
{
private:
    TreeNode* root;
    int cameras;

    TreeNode* addNode(TreeNode* node, int value);
    State dfs(TreeNode* node); // Рекурсивная функция для установки камер
    void installCameras(TreeNode* node, vector<bool>& covered, set<int>& cameraNodes);
    void printTree(TreeNode* node, bool high,
        vector<string> const& lpref,
        vector<string> const& cpref,
        vector<string> const& rpref,
        bool is_root, bool is_left,
        shared_ptr<vector<vector<string>>> lines);

public:
    BinaryTree();
    void add(int value);
    void coverTree();
    int minCameraCover(); // Функция для подсчёта камер
    void printPretty();
    TreeNode* getRoot();
};
int getIntInput(const string& prompt); // Новая функция для ввода чисел
bool korrec(const string& a);
void userInput(BinaryTree& tree);
void randomInput(BinaryTree& tree);
void fileInput(BinaryTree& tree);
