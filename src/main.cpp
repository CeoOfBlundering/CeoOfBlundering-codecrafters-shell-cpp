#include <iostream>
#include <string>
#include <algorithm>
#include <complex>
#include <vector>
#include <array>
#include <filesystem>
#include <cstdlib>

#ifdef _WIN32
constexpr char PATH_DELIMITER = ';';
#else
constexpr char PATH_DELIMITER = ':';
#endif

namespace fs = std::filesystem;
constexpr std::array<std::string, 5> builtinCommands = {"exit", "echo", "type", "pwd", "cd"};

void inputParse(const std::string &input, std::string &command, std::string &argument);
void pathParse(std::string &path, std::vector<std::string> &paths);
bool commandSearch(const std::vector<std::string> &paths, const std::string& arg, fs::path &full_path);
int commandProcess(std::string &path, const std::string &input, const std::string &command, std::string &argument);

int exit();
void echo(const std::string &argument);
void type(std::string &path, const std::string &argument);
void pwd();
void cd(std::string &p);
void cd(const std::string &p, std::error_code &ec) noexcept;

void executableSearch(std::string &path, const std::string &argument);
void executableSearchAndRun(const std::string &input, const std::string &command, std::string &path);


int main() {
    // Flush after every std::cout / std::cerr to prevent test timeout
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    std::string input, command, argument;
    const char* path_env = std::getenv("PATH");
    std::string envPath = path_env ? path_env : "";

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
        inputParse(input, command, argument);

        // Process Command
        int returnCode = commandProcess(envPath, input, command, argument);
        if (returnCode == -1)
            break;
    }
    return 0;
}

void inputParse(const std::string &input, std::string &command, std::string &argument) {
    if (size_t firstSpace = input.find(' '); firstSpace == std::string::npos) {
        command = input;
        argument = "";
    }
    else {
        command = input.substr(0, firstSpace);
        argument = input.substr(firstSpace + 1);
    }
}
void pathParse(std::string &path, std::vector<std::string> &paths) {
    // Split the paths and put into vector
    size_t start = 0, end = 0;
    while (start <= path.length()) {
        end = path.find(PATH_DELIMITER, start);
        if (end == std::string::npos)
            end = path.length();
        paths.push_back(path.substr(start, end - start));
        start = end + 1;
    }
}
bool commandSearch(const std::vector<std::string> &paths, const std::string& arg, fs::path &full_path) {
    bool found = false;
    for (const std::string& p : paths) {
        full_path = fs::path(p) / arg;

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
    return found;
}

int exit() {
    return -1;
}
void echo(const std::string &argument) {
    std::cout << argument << "\n";
}
void type(std::string &path, const std::string &argument) {
    if (std::ranges::contains(builtinCommands, argument)) {
        std::cout << argument << " is a shell builtin\n";
    }
    else if (!path.empty()) {
        executableSearch(path, argument);
    }
    else {
        std::cout << argument << ": not found\n";
    }
}
void pwd() {
    std::cout << fs::current_path().string() << "\n";
}
void cd(std::string &p) {
    std::error_code ec;
    cd(p, ec);
}
void cd(const std::string &p, std::error_code &ec) noexcept {
    if (fs::is_directory(p)) {
        fs::path temp = fs::current_path();
        fs::current_path(p, ec);
        if (ec) {
            fs::current_path(temp);
        }
    }
    else {
        std::cout << "cd: " << p << ": No such file or directory\n";
    }
}

void executableSearch(std::string &path, const std::string &argument) {
    std::vector<std::string> paths;
    pathParse(path, paths);
    if (fs::path full_path; commandSearch(paths, argument, full_path))
        std::cout << argument << " is " << full_path.generic_string() << "\n";
    else
        std::cout << argument << ": not found\n";
}
void executableSearchAndRun(const std::string &input, const std::string &command, std::string &path) {
    std::vector<std::string> paths;
    pathParse(path, paths);
    if (fs::path full_path; commandSearch(paths, command, full_path)) {
        std::system(input.c_str());
    }
    else
        std::cout << command << ": command not found\n";
}

int commandProcess(std::string &path, const std::string &input, const std::string &command, std::string &argument) {
    if (command == "exit") {
        return exit();
    }
    else if (command == "echo") {
        echo(argument);
    }
    else if (command == "type") {
        type(path, argument);
    }
    else if (command == "pwd") {
        pwd();
    }
    else if (command == "cd") {
        cd(argument);
    }
    else {
        if (!path.empty()) {
            executableSearchAndRun(input, command, path);
        }
        else
            std::cout << command << ": command not found\n";
    }
    return 1;
}