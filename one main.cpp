#include "header.h"

int main()
{
    setlocale(LC_ALL, "Russian");
    cout << "Выберите способ ввода выражения:\n";
    cout << "1 - С клавиатуры\n";
    cout << "2 - Из файла\n";
    cout << "Ваш выбор: ";

    int choice = InputUtils::getInt("");

    Tree* root = nullptr;

    switch (choice)
    {
    case 1:
        root = Tree::inputFromConsole();
        break;
    case 2:
        root = Tree::inputFromFile();
        break;
    default:
        cout << "Неверный выбор!\n";
        return 1;
    }

    if (!root) return 1;

    cout << "\nИсходное дерево выражения:" << endl;
    Tree::dump4(root, true, {}, {}, {}, true, true, nullptr);

    Tree::replaceSubtraction(root);

    cout << "\nДерево после замены вычитаний:" << endl;
    Tree::dump4(root, true, {}, {}, {}, true, true, nullptr);

    Tree::deleteTree(root);
    return 0;
}
