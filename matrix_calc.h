//
// Created by Oliver on 12/06/2021.
//

#ifndef ICL_CIRCUIT_SIMULATOR_MATRIX_CALC_H
#define ICL_CIRCUIT_SIMULATOR_MATRIX_CALC_H

#include <vector>

#include "Eigen/Dense"

#include "components.h"

double get_omega(int n, double points_per_decade, double start_frequency);
int get_node_max(Component* component, int node_max);
void G_matrix(int ith, int jth, double val, std::string type, bool is_AC, float omega, Eigen::MatrixXcd &G);
void B_matrix(int ith, int jth, int Vsource_pos, Eigen::MatrixXcd &B);
void Curr_matrix(int node_max, int ith, int jth, double val, std::string type, int Vsource_pos, Eigen::VectorXcd &Curr);
void mat_Populate(const std::vector<Component*>& components, int node_max, float omega, Eigen::MatrixXcd& Cond, Eigen::VectorXcd& Volt, Eigen::VectorXcd& Curr);
void cond_Diagonal(Eigen::MatrixXcd& Cond, int k, int node_max);
std::complex<double> run_simulation(const std::vector<Component*>& components, double omega, int node_max, int output_node);

#endif //ICL_CIRCUIT_SIMULATOR_MATRIX_CALC_H
