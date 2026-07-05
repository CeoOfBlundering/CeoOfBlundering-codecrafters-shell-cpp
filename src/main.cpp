#include <iostream>
#include <string>

int main() {
    // Flush after every std::cout / std:cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    while (true) {
        // Taking inputs
        std::cout << "$ ";
        std::string input;
        std::getline(std::cin, input);
        size_t firstSpace = input.find(' ');
        std::string command, argument;

        // Parsing the input to split command and argument
        if (firstSpace == std::string::npos) {
            command = input;
        }
        else {
            command = input.substr(0, firstSpace);
            argument = input.substr(firstSpace+1);
        }

        // Command processing
        if (command == "exit") {
            break;
        }
        else if (command == "echo") {
            std::cout << argument << "\n";
        }
        else if (command == "type") {
            // Create functions to clean up future-mess inside type function
            if (argument == "exit" || argument == "echo" || argument == "type") {
                std::cout << argument << " is a shell builtin\n";
            }
            else {
                std::cout << argument << ": not found\n";
            }
        }
        else{
        std::cout << input << ": command not found\n";
        }
    }
}