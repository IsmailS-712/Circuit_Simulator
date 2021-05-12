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


std::vector<Component>* parse_netlist_to_components(std::vector<std::string>& netlist) {
    std::vector<Component>* components = new std::vector<Component>;

    for (const std::string& line: netlist) {
        if (IndependentVoltageSource::represented_by(line)) {
            components->push_back(IndependentVoltageSource(line));
        } else if (CurrentSource::represented_by(line)) {
            components->push_back(CurrentSource(line));
        } else if (Resistor::represented_by(line)) {
            components->push_back(Resistor(line));
        } else if (Capacitor::represented_by(line)) {
            components->push_back(Capacitor(line));
        } else if (Inductor::represented_by(line)) {
            components->push_back(Inductor(line));
        } else if (Diode::represented_by(line)) {
            components->push_back(Diode(line));
        } else if (Diode::represented_by(line)) {
            components->push_back(Diode(line));
        } else if (BipolarJunctionTransistor::represented_by(line)) {
            components->push_back(BipolarJunctionTransistor(line));
        } else if (Mosfet::represented_by(line)) {
            components->push_back(Mosfet(line));
        } else if (VoltageControlledCurrentSource::represented_by(line)) {
            components->push_back(VoltageControlledCurrentSource(line));
        }
    }

    return components;
}