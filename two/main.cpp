#include "header.h"

int main()
{
    setlocale(LC_ALL, "Russian");
    GeneralBinaryTree tree;
    int choice;

    while (true)
    {
        cout << "\nМеню:\n";
        cout << "1. Ввод с консоли\n";
        cout << "2. Ввод из файла\n";
        cout << "3. Случайная генерация\n";

        choice = getIntInput("Выберите действие (0 для выхода): ");

        if (choice < 0 || choice > 3)
        {
            cout << "Неверный выбор. Пожалуйста, введите число от 0 до 3.\n";
            continue;
        }

        try
        {
            switch (choice)
            {
            case 1:
                consoleInput(tree);
                break;
            case 2:
                fileInput(tree);
                break;
            case 3:
                randomInput(tree);
                break;
            case 0:
                return 0;
            }

            cout << "\nДерево:\n";
            tree.dump();

            cout << "\nОбход дерева (лево-право-корень): ";
            auto it = tree.getIterator();
            while (it.hasNext())
            {
                cout << it.next() << " ";
            }
            cout << endl;

        }
        catch (const exception& e)
        {
            cerr << "Ошибка: " << e.what() << endl;
        }
    }

    return 0;
}
