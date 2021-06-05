//
// Created by olijd on 03/06/2021.
//

#include <fstream>
#include <string>
#include <vector>

#include "csv.h"


std::string csv::join_items_into_row(const std::vector<std::string>& items, const char delimiter = ',') {
    // Join a vector of items with the given delimiter.
    // The row will have a newline appended to it and no trailing delimiter.
    std::string row;

    for (int i=0; i < items.size(); i++) {
        row += items[i];
        if (i != (items.size() - 1)) {
            row.push_back(delimiter);
        }
    }
    row.push_back('\n');

    return row;
}


void csv::write_row(const std::vector<std::string>& items, const std::string& filename) {
    // Append a single row to the file with given name.
    // Items will be joined with commas and a newline appended before the row is written.
    std::string row = csv::join_items_into_row(items);

    std::ofstream outfile;
    outfile.open(filename, std::ios_base::app);
    outfile << row;
    outfile.close();
}


void csv::write_rows(const std::vector<std::string>& rows, const std::string& filename) {
    // Append multiple rows to the file with given name.
    // Each row in the rows vector should end with a linebreak.
    std::ofstream outfile;
    outfile.open(filename, std::ios_base::app);

    for (const std::string& row: rows) {
        outfile << row;
    }
    outfile.close();
}
