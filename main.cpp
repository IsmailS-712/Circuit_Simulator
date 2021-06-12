#include <iostream>
#include <vector>
#include <string>
#include <cmath>

#include "netparse.h"
#include "matrix_calc.h"
#include "csv.h"


int main() {
    //std::vector<std::string> netlist;
    //load_netlist(netlist, "test_netlist.txt");

    std::vector<std::string> netlist;
    netlist.push_back("C1 N002 0 0.1");
    netlist.push_back("R1 N002 N001 10");
    netlist.push_back("V1 N001 N004 AC(1 0)");
    netlist.push_back("L1 N003 N004 0.2");
    netlist.push_back("R2 N003 N002 10");
    netlist.push_back(".ac dec 10 10 100k");

    int output_node = 3;
//    std::cout << "Select output node:" << std::endl;
//    std::cin >> output_node;

    double points_per_decade, start_frequency, stop_frequency;
    parse_simulation_description(netlist, points_per_decade, start_frequency, stop_frequency);

    // std::cout << points_per_decade << " " << start_frequency << " " << stop_frequency << std::endl;

    std::vector<Component*> components;
    parse_netlist_to_components(components, netlist);

    double vin;
    int node_max = 0;
    for (auto* c: components) {
        node_max = get_node_max(c, node_max);
        if (c->is_vs()) {
            vin = c->get_amplitude();
        }
    }

    int n = 0;
    double omega = 0.0;
    std::complex<double> output;
    std::vector<std::string> output_values;

    while (omega < stop_frequency) {
        output_values.clear();
        omega = get_omega(n, points_per_decade, start_frequency);
        output = run_simulation(components, omega, node_max, output_node);

        output_values.push_back(std::to_string(omega));
        output_values.push_back(std::to_string(20 * std::log10(std::abs(output) / vin)));
        output_values.push_back(std::to_string(std::atan(output.imag()/output.real())));
        csv::write_row(output_values, "output.txt");

        ++n;
    }

    return 0;
}
