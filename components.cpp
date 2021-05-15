//
// Created by Oliver on 12/05/2021.
//

#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <unordered_map>
#include <regex>

#include "components.h"


std::unordered_map<std::string, double> value_map = {
        {"p", 1.0e-12},
        {"n", 1.0e-9},
        {"u", 1.0e-6},
        {"m", 1.0e-3},
        {"k", 1.0e3},
        {"Meg", 1.0e6},
        {"G", 1.0e9}
};


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

std::vector<std::string> regex_extract(const std::string& s, const std::string& re) {
    std::regex r(re);
    std::smatch m;
    std::regex_search(s, m, r);

    std::vector<std::string> matches;
    for (auto v: m) {
        matches.push_back(v);
    }
    return matches;
}


// Definition of Component members
Component::Component(ParseReturn *parsed_line) {
    identifier = parsed_line->identifier[0];
    id = std::stoi(regex_extract(parsed_line->identifier, "\\d+")[0]);
    value = parsed_line->value;
    nodes = parsed_line->nodes;
}

double Component::converted_value() {
    double numeric_val = std::stod(regex_extract(value, "\\d+(\\.\\d+)?")[0]);
    double multiplier = value_map[regex_extract(value, "[a-zA-Z]+")[0]];
    return numeric_val * multiplier;
}
