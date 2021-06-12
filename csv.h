//
// Created by olijd on 03/06/2021.
//

#ifndef ICL_CIRCUIT_SIMULATOR_CSV_H
#define ICL_CIRCUIT_SIMULATOR_CSV_H

#include <vector>
#include <string>

namespace csv {
    std::string join_items_into_row(const std::vector<std::string>& items, char delimiter);
    void write_row(const std::vector<std::string>& items, const std::string& filename);
    void write_rows(const std::vector<std::string>& rows, const std::string& filename);
}


#endif //ICL_CIRCUIT_SIMULATOR_CSV_H
