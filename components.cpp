//
// Created by Oliver on 12/05/2021.
//

#include <vector>
#include <string>
#include <iterator>

#include "components.h"


int get_id_from_identifier(const std::string& identifier) {
    return std::stoi(identifier.substr(1, identifier.length()));
}

ParseReturn* parse_netlist_line(const std::string &token_str) {
    auto* parsed = new ParseReturn;

    std::istringstream buf(token_str);
    std::istream_iterator<std::string> beg(buf), end;
    std::vector<std::string> tokens(beg, end);

    parsed->identifier = tokens[0];
    parsed->value = tokens[tokens.size() - 1];

    for (int i = 1; i < tokens.size(); i++) {
        parsed->nodes.push_back(tokens[i]);
    }
    return parsed;
}


// Definition of Component members
Component::Component(ParseReturn *parsed_line) {
    identifier = parsed_line->identifier[0];
    id = get_id_from_identifier(parsed_line->identifier);
    value = parsed_line->value;
    nodes = parsed_line->nodes;
}
