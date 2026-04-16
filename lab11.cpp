#include <iostream>
#include <fstream>
#include <list>
#include <Windows.h>
#include <string>
#include <utility>

class JournalEditor {
private:
    std::list<std::string> textBuffer;
    std::list<std::string>::iterator cursor;
    std::string filename;

public:
     explicit JournalEditor(std::string  file) : filename(std::move(file)) {
        load();
        cursor = textBuffer.begin();
    }

    void load() {
        std::ifstream file(filename);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                textBuffer.push_back(line);
            }
            file.close();
        }

        if (textBuffer.empty()) {
            textBuffer.emplace_back("");
        }
    }

    void save() const {
        std::ofstream file(filename);
        if (file.is_open()) {
            for (const auto& line : textBuffer) {
                file << line << "\n";
            }
            file.close();
            std::cout << "[Успех] Файл сохранен.\n";
        } else {
            std::cerr << "[Ошибка] Не удалось сохранить файл.\n";
        }
    }

    void print() {
        std::cout << "\n--- Содержимое файла: " << filename << " ---\n";
        auto it = textBuffer.begin();
        int lineNum = 1;

        while (it != textBuffer.end()) {
            if (it == cursor) {
                std::cout << "-> " << lineNum << ": " << *it << "\n";
            } else {
                std::cout << "   " << lineNum << ": " << *it << "\n";
            }
            ++it;
            ++lineNum;
        }
        std::cout << "------------------------------------\n";
    }


    void moveUp() {
        if (cursor != textBuffer.begin()) {
            --cursor;
        } else {
            std::cout << "Курсор уже на первой строке.\n";
        }
    }

    void moveDown() {
        auto temp = cursor;
        ++temp;
        if (temp != textBuffer.end()) {
            ++cursor;
        } else {
            std::cout << "Курсор уже на последней строке.\n";
        }
    }


    void editCurrentLine(const std::string& newText) const {
        *cursor = newText;
    }

    void insertLine(const std::string& newLine) {
        auto insertPos = cursor;
        ++insertPos;
        textBuffer.insert(insertPos, newLine);
        ++cursor;
    }


    void deleteLine() {
        if (textBuffer.size() > 1) {
            cursor = textBuffer.erase(cursor);
            if (cursor == textBuffer.end()) {
                --cursor;
            }
            std::cout << "Строка успешно удалена.\n";
        }
        else if (textBuffer.size() == 1 and !cursor->empty()) {
            cursor -> clear();
            std::cout << "Очищена единственная строка.\n";
        }
        else {
             std::cout << "Строк нет.\n";
         }
    }
};

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);


    JournalEditor editor("my_journal.txt");
    std::string command;
    std::string input;

    std::cout << "Добро пожаловать в текстовый редактор!\n";
    std::cout << "Команды: [u]p, [d]own, [e]dit, [i]nsert, [del]ete, [s]ave, [q]uit\n";

    while (true) {
        editor.print();
        std::cout << "Команда: ";
        std::cin >> command;

        if (command == "q") {
            break;
        } else if (command == "u") {
            editor.moveUp();
        } else if (command == "d") {
            editor.moveDown();
        } else if (command == "e") {
            std::cout << "Введите новый текст: ";
            std::cin.ignore(1000,'\n');
            std::getline(std::cin, input);
            editor.editCurrentLine(input);
        } else if (command == "i") {
            std::cout << "Введите текст новой строки: ";
            std::cin.ignore(1000,'\n');
            std::getline(std::cin, input);
            editor.insertLine(input);
        } else if (command == "del") {
            editor.deleteLine();
        } else if (command == "s") {
            editor.save();
        } else if (command == "h") {
            std::cout << "Команды: [u]p, [d]own, [e]dit, [i]nsert, [del]ete, [s]ave, [q]uit\n";
            }else {
            std::cout << "Неизвестная команда.\n" << "Чтобы вывести справку введите [h]elp";
        }
    }

    return 0;
}