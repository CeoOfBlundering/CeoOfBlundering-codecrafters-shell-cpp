#include <iostream>
#include <string>

int main() {
    // Flush after every std::cout / std:cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    while (true) {
        std::cout << "$ ";
        std::string input;
        std::cin >> input;
        std::getline(std::cin, input);
        size_t firstSpace = input.find(' ');
        std::string command, message;
        if (firstSpace == std::string::npos) {
            command = input;
        }
        else {
            command = input.substr(0, firstSpace);
            message = input.substr(firstSpace);
        }

        if (command == "exit") {
            break;
        }
        else if (command == "echo") {
            std::cout << message << "\n";
        }
        else{
        std::cout << input << ": command not found\n";
        }
    }
}