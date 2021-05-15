//
// Created by Oliver on 11/05/2021.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "components.h"


// Reads the given netlist file and returns a reference to a vector of lines in the file
std::vector<std::string>* load_netlist(const std::string& filename) {
    auto *lines = new std::vector<std::string>;
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

        lines->push_back(line);
    }

    return lines;
}


void parse_netlist_to_components(std::vector<Component*>& components, std::vector<std::string>* netlist) {
    for (const std::string& line: *netlist) {
        if (line.rfind('.', 0) == 0) {
            // Skip to next line if current line starts with a '.' as this line cannot be parsaed into a Component
            continue;
        }
        ParseReturn* parsed_line = parse_netlist_line(line);
        components.push_back(new Component(parsed_line));
        delete parsed_line;
    }
}
