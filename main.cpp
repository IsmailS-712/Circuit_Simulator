#include <iostream>
#include <vector>
#include <string>

#include "netparse.h"


int main() {
    std::vector<std::string>* lines = load_netlist("test_netlist.txt");
    for (const std::string& line: *lines) {
        std::cout << line << std::endl;
    }
    return 0;
}
