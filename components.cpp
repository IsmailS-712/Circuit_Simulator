//
// Created by Oliver on 12/05/2021.
//

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <regex>

#include "components.h"


std::unordered_map<std::string, double> value_map = {
        {"p", 1.0e-12},
        {"n", 1.0e-9},
        {"u", 1.0e-6},
        {"m", 1.0e-3},
        {"", 1},
        {"k", 1.0e3},
        {"Meg", 1.0e6},
        {"G", 1.0e9}
};


void split_netlist_line(std::vector<std::string>& segments, const std::string& line) {
    int stack = 0;
    std::string current_segment;

    for (char c: line) {
        if (c == ' ' && !stack) {
            if (!current_segment.empty()) {
                segments.push_back(current_segment);
                current_segment.clear();
            }
            continue;
        }

        if (c == '(') {
            ++stack;
        } else if (c == ')') {
            --stack;
        }

        current_segment.push_back(c);
    }
    if (!current_segment.empty()) {
        segments.push_back(current_segment);
    }
}


ParseReturn* parse_netlist_line(const std::string &token_str) {
    auto* parsed = new ParseReturn;

    std::vector<std::string> tokens;
    split_netlist_line(tokens, token_str);

    parsed->identifier = tokens[0];
    parsed->value = tokens[tokens.size() - 1];

    for (int i = 1; i < tokens.size() - 1; i++) {
        //std::cout << regex_extract(tokens[i], "N?(\\d+)")[0] << std::endl;
        parsed->nodes.push_back(std::stoi(regex_extract(tokens[i], "N?(\\d+)")[1]));
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


void split_string_by_space(std::vector<std::string>& out_vector, const std::string& string) {
    std::istringstream iss(string);
    for(std::string s; iss >> s;)
        out_vector.push_back(s);
}


double convert_value(const std::string& value) {
    double numeric_val = std::stod(regex_extract(value, "\\d+(\\.\\d+)?")[0]);
    double multiplier = value_map[regex_extract(value, "[a-zA-Z]+")[0]];
    return numeric_val * multiplier;
}


// Definition of Component members
Component::Component(ParseReturn *parsed_line) {
    identifier = parsed_line->identifier[0];
    id = std::stoi(regex_extract(parsed_line->identifier, "\\d+")[0]);
    value = parsed_line->value;
    nodes = parsed_line->nodes;
}

double Component::converted_value() {
    return convert_value(value);
}

bool Component::has_converted_value() {
    // Value for diodes, BJTs and MOSFETs cannot be converted as it is a model number.
    if (identifier == 'D' || identifier == 'Q' || identifier == 'M' || is_vs()) {
        return false;
    }
    return true;
}

bool Component::is_vs() {
    return std::regex_match(value, std::regex("^AC\\([^\\s\\)]\\s[^\\)]\\)$"));
}

float Component::get_amplitude() {
    std::vector<std::string> parts;
    split_string_by_space(parts, value.substr(3, value.length() - 4));
    return std::stof(parts[0]);
}

float Component::get_phase() {
    std::vector<std::string> parts;
    split_string_by_space(parts, value.substr(3, value.length() - 4));
    return std::stof(parts[1]);
}
