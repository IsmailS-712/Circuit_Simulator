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
    std::vector<int> nodes;
    std::string value;
};


ParseReturn* parse_netlist_line(const std::string &token_str);
std::vector<std::string> regex_extract(const std::string& s, const std::string& re);
void split_string_by_space(std::vector<std::string>& out_vector, const std::string& string);
double convert_value(const std::string& value);


class Component {
public:
    Component(ParseReturn* parsed_line);
    char get_identifier() const {return identifier;};
    int get_id() const {return id;};
    std::string get_value() {return value;};
    bool has_converted_value();
    double converted_value();
    std::vector<int> get_nodes() {return nodes;};
    bool is_vs();
    float get_amplitude();
    float get_phase();

private:
    char identifier;
    int id;
    std::string value;
    std::vector<int> nodes;
};

#endif
