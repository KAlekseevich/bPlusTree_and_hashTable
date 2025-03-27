#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include "HashMap.h"
#include "BPTree.h"

std::wstring StrText = L"Справа Ростов видел первые ряды своих гусар, а еще дальше впереди виднелась ему темная полоса, которую он не мог рассмотреть, но считал неприятелем. Выстрелы были слышны, но в отдалении.";
std::string StrText2 = "Справа Ростов видел первые ряды своих гусар, а еще дальше впереди виднелась ему темная полоса, которую он не мог рассмотреть, но считал неприятелем. Выстрелы были слышны, но в отдалении.";

int main()
{

    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    HashMap hashmap;
    BPTree bptree;

    int choice;

    do {
        std::cout << "Меню:\n";
        std::cout << "1." << '\t' << "Сформировать хэш-таблицу\n";
        std::cout << "2." << '\t' << "Вывести хэш-таблицу\n";
        std::cout << "3." << '\t' << "Найти элемент\n";
        std::cout << "4." << '\t' << "Добавить элемент\n";
        std::cout << "5." << '\t' << "Удалить элемент\n";
        std::cout << "6." << '\t' << "Добавить слова из файла\n";
        std::cout << "7." << '\t' << "Очистить Хэш-таблицу\n";
        std::cout << "8." << '\t' << "Сформировать B+ дерево\n";
        std::cout << "9." << '\t' << "Вывести B+ дерево\n";
        std::cout << "10." << '\t' << "Добавить элемент в дерево\n";
        std::cout << "11." << '\t' << "Удалить элемент из дерева\n";
        std::cout << "12." << '\t' << "Найти элемент в дереве\n";
        std::cout << "13." << '\t' << "Дополнение словаря из файла\n";
        std::cout << "14." << '\t' << "Очистить B+ дерево\n";
        std::cout << "0." << '\t' << "Выход\n";
        std::cout << std::endl;
        std::cout << "Выберите пункт: " << std::endl;
        std::cin >> choice;
        switch (choice) {
        case 1:
        {
            std::cout << std::endl;
            hashmap.make_dict(StrText2);
            std::cout << std::endl;
            break;
        }
        case 2:
        {
            std::cout << std::endl;
            hashmap.show();
            std::cout << std::endl;
            break;
        }
        case 3:
        {
            std::cout << std::endl;
            std::string str;
            std::cout << "Введите ключ для поиска: " << std::endl;
            std::cin >> str;
            std::pair<int, int> INDEXES = hashmap.findIND(str);
            if (INDEXES.first == -1)
                std::cout << "Элемент не найден!" << std::endl;
            else
                std::cout << "Элемент был найден в " << INDEXES.first << " колонке, в " << INDEXES.second << " строке" << std::endl;

            std::cout << std::endl;
            break;
        }
        case 4:
        {
            std::cout << std::endl;
            std::string str;
            std::cout << "Введите строку для добавления: " << std::endl;
            std::cin >> str;
            if (hashmap.empty())
                hashmap.prepare();
            hashmap.insert(str);
            std::cout << std::endl;
            break;
        }
        case 5:
        {
            std::cout << std::endl;
            std::string str;
            std::cout << "Введите строку для удаления: " << std::endl;
            std::cin >> str;
            hashmap.remove(str);
            std::cout << std::endl;
            break;
        }
        case 6:
        {
            std::cout << std::endl;
            if (hashmap.empty())
                hashmap.prepare();
            hashmap.addFromFile();
            std::cout << std::endl;
            break;
        }
        case 7:
        {
            std::cout << std::endl;
            hashmap.clear();
            std::cout << std::endl;
            break;
        }
        case 8:
        {
            std::cout << std::endl;
            bptree.make_bptree(StrText2);
            std::cout << std::endl;
            break;
        }
        case 9:
        {
            std::cout << std::endl;
            if (bptree.empty())
                std::cout << "Словарь пуст!" << std::endl;
            else
                bptree.show(bptree.getRoot());
            std::cout << std::endl;
            break;
        }
        case 10:
        {

            std::cout << std::endl;
            std::string str;
            std::cout << "Введите строку для добавления: " << std::endl;
            std::cin >> str;
            bptree.insert(str);
            std::cout << std::endl;
            break;

        }
        case 11:
        {

            std::cout << std::endl;
            std::string str;
            std::cout << "Введите строку для удаления: " << std::endl;
            std::cin >> str;
            bptree.remove(str);
            std::cout << std::endl;
            break;

        }
        case 12:
        {
            std::cout << std::endl;
            std::string str;
            std::cout << "Введите строку для поиска: " << std::endl;
            std::cin >> str;
            if (bptree.find(str)) std::cout << "Элемент найден" << std::endl;
            else std::cout << "Элемент не найден" << std::endl;
            std::cout << std::endl;
            break;
        }
        case 13:
        {
            std::cout << std::endl;
            bptree.addFromFile();
            std::cout << std::endl;
            break;
        }
        case 14:
        {
            std::cout << std::endl;
            bptree.clear();
            std::cout << std::endl;
            break;
        }
        case 0:
            std::cout << "Выход из меню\n";
            std::cout << std::endl;
            std::cout << std::endl;
            break;
        default:
            std::cout << "Неверный выбор. Попробуйте снова.\n";
            std::cout << std::endl;
            std::cout << std::endl;
            break;
        }
    } while (choice != 0);

    return 0;

}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.


// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
