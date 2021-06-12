//
// Created by Oliver on 11/05/2021.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "components.h"


// Reads the given netlist file and returns a reference to a vector of lines in the file
void load_netlist(std::vector<std::string>& out_vector, const std::string& filename) {
    std::ifstream infile(filename);

    std::string line;
    while (std::getline(infile, line)) {

        if (line.rfind('*', 0) == 0) {
            // Skip to the next line in the file if the current one is a comment
            continue;
        } else if (line.rfind(".end", 0) == 0) {
            // Stop looking for extra lines if .end has been found by breaking out of the loop
            break;
        }

        out_vector.push_back(line);
    }
}


void parse_netlist_to_components(std::vector<Component*>& components, std::vector<std::string>& netlist) {
    for (const std::string& line: netlist) {
        if (line.rfind('.', 0) == 0) {
            // Skip to next line if current line starts with a '.' as this line cannot be parsed into a Component
            continue;
        }
        ParseReturn* parsed_line = parse_netlist_line(line);
        components.push_back(new Component(parsed_line));
        delete parsed_line;
    }
}


void parse_simulation_description(const std::vector<std::string>& netlist, double& points_per_decade, double& start_frequency, double& stop_frequency) {
    std::string simulation_description;
    for (const auto& line: netlist) {
        if (line.substr(0, 3) == ".ac") {
            simulation_description = line;
            break;
        }
    }
    std::vector<std::string> simulation_parts;
    split_string_by_space(simulation_parts, simulation_description);
    points_per_decade = convert_value(simulation_parts[2]);
    start_frequency = convert_value(simulation_parts[3]);
    stop_frequency = convert_value(simulation_parts[4]);
}
