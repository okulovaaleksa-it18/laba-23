#pragma once

#include <iostream>
#include <fstream>
#include <stack>
#include <vector>
#include <string>
#include <memory>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

const string ch_hor = "-";
const string ch_ver = "|";
const string ch_ddia = "/";
const string ch_rddia = "\\";

namespace InputUtils
{
    bool validateInt(const string& input);
    int getInt(const string& prompt);
}

class Tree
{
public:
    int key;
    Tree* left;
    Tree* right;

    Tree(int val);
    ~Tree();

    static Tree* createNode(int val);
    static Tree* buildExpressionTree(const vector<int>& tokens);
    static int evaluateSubtree(Tree* node);
    static void replaceSubtraction(Tree* node);
    static bool validateToken(const string& token);
    static vector<int> parseRPN(const string& input);
    static void dump4(const Tree* node, bool high,
        const vector<string>& lpref,
        const vector<string>& cpref,
        const vector<string>& rpref,
        bool is_root, bool is_left,
        shared_ptr<vector<vector<string>>> lines);
    static Tree* inputFromFile(string filename = "expression.txt");
    static Tree* inputFromConsole();
    static void deleteTree(Tree* root);
};
