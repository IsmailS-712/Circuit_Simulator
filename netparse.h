//
// Created by Oliver on 11/05/2021.
//

#ifndef ICL_CIRCUIT_SIMULATOR_NETPARSE_H
#define ICL_CIRCUIT_SIMULATOR_NETPARSE_H

#include <vector>
#include <string>

#include "components.h"


std::vector<std::string>* load_netlist(const std::string& filename);
std::vector<Component*>* parse_netlist_to_components(std::vector<std::string>* netlist);

#endif
