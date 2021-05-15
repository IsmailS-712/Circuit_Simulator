//
// Created by Oliver on 12/05/2021.
//

#ifndef ICL_CIRCUIT_SIMULATOR_COMPONENTS_H
#define ICL_CIRCUIT_SIMULATOR_COMPONENTS_H

#include <vector>
#include <string>
#include <sstream>
#include <iterator>


struct ParseReturn {
    std::string identifier;
    std::vector<std::string> nodes;
    std::string value;
};

ParseReturn* parse_netlist_line(const std::string &token_str);


class Component {
public:
    Component(ParseReturn* parsed_line);
    char get_identifier() const {return identifier;};
    int get_id() const {return id;};
    std::string get_value() {return value;};
    std::vector<std::string> get_nodes() {return nodes;};

private:
    char identifier;
    int id;
    std::string value;
    std::vector<std::string> nodes;
};

#endif
