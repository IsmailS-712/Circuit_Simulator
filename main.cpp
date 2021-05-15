#include <iostream>
#include <vector>
#include <string>

#include "netparse.h"


//int main() {
//    std::vector<std::string>* lines = load_netlist("test_netlist.txt");
//    for (const std::string& line: *lines) {
//        std::cout << line << std::endl;
//    }
//    delete lines;
//    return 0;
//}


int main() {
    // std::vector<std::string>* netlist = load_netlist("test_netlist.txt");
    auto* netlist = new std::vector<std::string>;
    netlist->push_back("R1 N001 N003 1k");
    netlist->push_back("R2 N002 N001 1Meg");

    std::vector<Component*> components;
    parse_netlist_to_components(components, netlist);

    for (Component* component: components) {
        std::cout << "Converted Value: " << component->converted_value() << std::endl;
    }
    return 0;
}
