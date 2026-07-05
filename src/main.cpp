#include <iostream>
#include <string>

int main() {
    // Flush after every std::cout / std:cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    while (true){
        std::cout << "$ ";
        std::string input, msg;
        std::cin >> input >> msg;
        if (input == "exit") {
            break;
        }

        if (input == "echo") {
            std::cout << msg << "\n";
            continue;
        }

        std::cout << input << ": command not found\n";
    }
}