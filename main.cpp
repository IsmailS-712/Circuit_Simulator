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
    std::vector<std::string>* netlist = load_netlist("test_netlist.txt");
    std::vector<Component>* components = parse_netlist_to_components(netlist);

    for (Component component: *components) {
        std::cout << component.get_id() << " " << component.get_value() << std::endl;
    }
    return 0;
}
