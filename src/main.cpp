#include <iostream>
#include <string>

int main() {
    // Flush after every std::cout / std:cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    while (true){
        std::cout << "$ ";
        std::string input;
        std::cin >> input;
        if (input == "exit") {
            break;
        }

        if (input.substr(0,input.find(' ')) == "echo") {
            std::cout << input.substr(input.find(' '), input.length()) << "\n";
            continue;
        }

        std::cout << input << ": command not found\n";
    }
}