#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <filesystem>
#include <cstdlib>

#ifdef _WIN32
    const char PATH_DELIMITER = ';';
#else
    const char PATH_DELIMITER = ':';
#endif

namespace fs = std::filesystem;

int main() {
    // Flush after every std::cout / std::cerr to prevent test timeout
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    std::string input, command, argument;
    const char* path_env = std::getenv("PATH");
    std::string path = path_env ? path_env : "";

    while (true) {
        // Get Input
        std::cout << "$ ";
        if (!std::getline(std::cin, input)) {
            break;
        }

        // Strip carriage return if tests are running in a mixed OS environment (Gemini AI)
        if (!input.empty() && input.back() == '\r') {
            input.pop_back();
        }

        if (input.empty()) {
            continue;
        }

        // Parse input
        size_t firstSpace = input.find(' ');
        if (firstSpace == std::string::npos) {
            command = input;
            argument = "";
        }
        else {
            command = input.substr(0, firstSpace);
            argument = input.substr(firstSpace + 1);
        }

        // Process Command
        if (command == "exit") {
            break;
        }
        else if (command == "echo") {
            std::cout << argument << "\n";
        }
        else if (command == "type") {
            if (argument == "exit" || argument == "echo" || argument == "type") {
                std::cout << argument << " is a shell builtin\n";
            }
            else if (!path.empty()) {
                std::vector<std::string> paths;
                size_t start = 0, end = 0;
                while (start <= path.length()) {
                    end = path.find(PATH_DELIMITER, start);
                    if (end == std::string::npos)
                        end = path.length();
                    paths.push_back(path.substr(start, end - start));
                    start = end + 1;
                }

                bool found = false;
                for (const std::string& p : paths) {
                    fs::path full_path = fs::path(p) / argument;

                    if (!fs::exists(full_path) || !fs::is_regular_file(full_path)) {
                        continue;
                    }

                    fs::perms permissions = fs::status(full_path).permissions();
                    fs::perms executable = fs::perms::owner_exec | fs::perms::group_exec | fs::perms::others_exec;

                    if ((permissions & executable) != fs::perms::none) {
                        std::cout << argument << " is " << full_path.generic_string() << "\n";
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    std::cout << argument << ": not found\n";
                }
            }
            else {

                std::cout << argument << ": not found\n";
            }
        }
        else {
            if (!path.empty()) {
                std::vector<std::string> paths;
                size_t start = 0, end = 0;
                while (start <= path.length()) {
                    end = path.find(PATH_DELIMITER, start);
                    if (end == std::string::npos)
                        end = path.length();
                    paths.push_back(path.substr(start, end - start));
                    start = end + 1;
                }
                bool found = false;
                for (const std::string& p : paths) {
                    fs::path full_path = fs::path(p) / command;

                    if (!fs::exists(full_path) || !fs::is_regular_file(full_path)) {
                        continue;
                    }

                    fs::perms permissions = fs::status(full_path).permissions();
                    fs::perms executable = fs::perms::owner_exec | fs::perms::group_exec | fs::perms::others_exec;

                    if ((permissions & executable) != fs::perms::none) {
                        found = true;
                        break;
                    }
                }
                if (found) {
                    std::system(input.c_str());
                }
                else
                    std::cout << command << ": command not found\n";
            }
            else
                std::cout << command << ": command not found\n";
        }
    }
    return 0;
}