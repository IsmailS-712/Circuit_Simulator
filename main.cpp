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
    // auto* netlist = load_netlist("test_netlist.txt");
    auto* netlist = new std::vector<std::string>;
    netlist->push_back("V1 N003 0 AC(1 0)");
    netlist->push_back("R1 N001 N003 1k");
    netlist->push_back("C1 N001 0 1u");
    netlist->push_back("I1 0 N004 0.1");
    netlist->push_back("D1 N004 N002 D");
    netlist->push_back("L1 N002 N001 1m");
    netlist->push_back("R2 N002 N001 1Meg");
    netlist->push_back("Q1 N003 N001 0 NPN");

    std::vector<Component*> components;
    parse_netlist_to_components(components, netlist);

    for (auto c: components) {
        std::cout << c->get_identifier() << " " << c->get_id() << " " << c->get_value() << " " << (c->has_converted_value() ? std::to_string(c->converted_value()) : "_") << std::endl;
    }
    return 0;
}
