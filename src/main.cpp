#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <filesystem>

#ifdef _WIN32
    const char PATH_DELIMITER = ';';
#else
    const char PATH_DELIMITER = ':';
#endif

namespace fs = std::filesystem;

int main() {
    // Flush after every std::cout / std:cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    std::string input, command, argument;
    std::string path = std::getenv("PATH");

    while (true) {
        // Taking inputs
        std::cout << "$ ";
        std::getline(std::cin, input);
        size_t firstSpace = input.find(' ');

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
            else if (!path.empty()) {
                std::vector<std::string> paths;
                size_t start = 0, end = 0;
                while (start <= path.length()) {
                    end = path.find(PATH_DELIMITER, start);
                    if (end == std::string_view::npos)
                        end = path.length();
                    paths.push_back(path.substr(start, end-start));
                    start = end+1;
                }

                for (std::string p : paths) {
                    if (!fs::exists(p ) || !fs::is_regular_file(p)) {
                        continue;
                    }
                    fs::perms permissions = fs::status(p).permissions();
                    fs::perms executable = fs::perms::owner_exec | fs::perms::group_exec | fs::perms::others_exec;
                    if ((permissions  & executable) != fs::perms::none) {
                        std::cout << argument << " is " << p;
                        break;
                    }
                }
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