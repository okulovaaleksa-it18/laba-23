#include "header.h"
#include <iostream>
#include <fstream>
#include <clocale>
#include <limits>
#include <algorithm>

using namespace std;

int main()
{
    setlocale(LC_ALL, "rus");
    BinaryTree tree;

    cout << "\nМеню:\n";
    cout << "1. Ввод с консоли\n";
    cout << "2. Ввод из файла\n";
    cout << "3. Случайная генерация\n";
    cout << "4. Выход\n";  // Добавлен пункт выхода

    while (true)  // Добавлен цикл для повторного выбора
    {
        int choice = getIntInput("\nВаш выбор (1-4): ");

        switch (choice)
        {
        case 1:
            userInput(tree);
            break;
        case 2:
            fileInput(tree);
            break;
        case 3:
            randomInput(tree);
            break;
        case 4:  // Выход из программы
            cout << "Завершение программы..." << endl;
            return 0;
        default:
            cout << "Некорректный выбор! Пожалуйста, введите число от 1 до 4." << endl;
            continue;  // Продолжаем цикл при некорректном вводе
        }

        // Вывод дерева и результатов только при успешном вводе
        cout << "\nВывод дерева:\n";
        tree.printPretty();

        // Используем новую функцию minCameraCover() вместо coverTree()
        int cameras = tree.minCameraCover();
        cout << "\nМинимальное количество камер: " << cameras << endl;

        // Сбрасываем дерево для нового ввода
        tree = BinaryTree();
    }

    return 0;
}
