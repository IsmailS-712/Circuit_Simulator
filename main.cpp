#include <iostream>
#include <vector>
#include <string>
#include <cmath>

#include "netparse.h"
#include "matrix_calc.h"
#include "csv.h"


int main() {
    std::string filename = "test_netlist.txt";
    std::cout << "Enter netlist filename:" << std::endl;
    std::cin >> filename;

    std::vector<std::string> netlist;
    load_netlist(netlist, filename);

    int output_node = 3;
    std::cout << "Select output node:" << std::endl;
    std::cin >> output_node;

    double points_per_decade, start_frequency, stop_frequency;
    parse_simulation_description(netlist, points_per_decade, start_frequency, stop_frequency);

    std::vector<Component*> components;
    parse_netlist_to_components(components, netlist);

    double ac_vin;
    bool ac_circuit = false;
    double dc_vin;
    bool dc_vin_set = false;

    int node_max = 0;
    for (auto* c: components) {
        node_max = get_node_max(c, node_max);
        if (c->is_vs()) {
            ac_vin = c->get_amplitude();
            ac_circuit = true;
        } else if (c->get_identifier() == 'V' && !dc_vin_set) {
            dc_vin = c->converted_value();
            dc_vin_set = true;
        }
    }

    int n = 0;
    double omega = 0.0;
    std::complex<double> output;
    std::vector<std::string> output_values;

    std::vector<std::string> output_rows;
    if (ac_circuit) {
        while (omega < stop_frequency) {
            output_values.clear();
            omega = get_omega(n, points_per_decade, start_frequency);
            output = run_simulation(components, omega, node_max, output_node);

            output_values.push_back(std::to_string(omega));
            output_values.push_back(std::to_string(20 * std::log10(std::abs(output) / ac_vin)));
            output_values.push_back(std::to_string(std::atan(output.imag() / output.real())));

            output_rows.push_back(csv::join_items_into_row(output_values, ','));

            ++n;
        }
    } else {
        output = run_simulation(components, 0, node_max, output_node);

        output_values.push_back(std::to_string(omega));
        output_values.push_back(std::to_string(20 * std::log10(std::abs(output) / dc_vin)));
        output_values.push_back(std::to_string(std::atan(output.imag() / output.real())));

        output_rows.push_back(csv::join_items_into_row(output_values, ','));
    }

    std::string output_filename = regex_extract(filename, "^([^\\.]+)")[0] + "_output.txt";
    csv::write_rows(output_rows, output_filename);

    return 0;
}
