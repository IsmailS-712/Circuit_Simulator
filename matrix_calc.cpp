#include <iostream>
#include <string>
#include <vector>
#include <complex>
#include "Eigen/Dense"

#include "matrix_calc.h"


double get_omega(int n, double points_per_decade, double start_frequency) {
    return std::pow(10, (n/points_per_decade)) * start_frequency;
}


// Finds the largest node in the vector of strings, so the matrix can be initialised with it's size
int get_node_max(Component* component, int node_max) {
    int max_elem = node_max;

    for (auto elem: component->get_nodes()) {
        if (elem > max_elem) {
            max_elem = elem;
        }
    }

    return max_elem;
}

// Creates a matrix using the passive elements in the circuit of size (nxn) for n nodes
void G_matrix(int ith, int jth, double val, const std::string& type, bool is_AC, double omega, Eigen::MatrixXcd &G) {
    std::complex<double> complex_val; // Used for capacitors/inductors in an AC circuit

    if (type == "R") { // If the type is a Resistor...
        if (ith == -1) { // If connected to ground...
            G(jth, jth) += (1 / val); // Store the reciprocal (Conductance) on the main diagonal
        }
        else if (jth == -1) { // --
            G(ith, ith) += (1 / val); // --
        }
        else { // If not connected to ground...
            G(ith, jth) += (-1 / val); // The corresponding negative conductance of the value is stored in the [ith, jth] position
            G(jth, ith) += (-1 / val); // Because of Kirchoff's Current Law, the matrix is symmetric
        }
    }
    else if (type == "C") { // If the type is Capacitor...
        if (is_AC) { // If the source is AC...
            complex_val.imag(val*omega); // Reactive component is equal to j*w*C
            if (ith == -1) {
                G(jth, jth) += complex_val; // Admittance of a capacitor (1/Impedance)
            }
            else if (jth == -1) {
                G(ith, ith) += complex_val;
            }
            else {
                G(ith, jth) += complex_val;
                G(jth, ith) += complex_val;
            }
        }
    }
    else if (type == "L") { // If the type is Inductor...
        if (is_AC) { // --
            complex_val.imag(1/(val*omega)); // Reactive component is equal to j*(1/w*L)
            if (ith == -1) {
                G(jth, jth) += -complex_val; // Admittance of an inductor (Similar to resistance)
            }
            else if (jth == -1) {
                G(ith, ith) += -complex_val; // Minus is used because the admittance of an inductor is 1/jwL, and 1/j is equal to -j, hence the minus
            }
            else {
                G(ith, jth) += complex_val;
                G(jth, ith) += complex_val;
            }
        }
        else {
            if (ith == -1) {
                G(jth, jth) += 1 / val;
            }
            else if (jth == -1) {
                G(ith, ith) += 1 / val;
            }
            else {
                G(ith, jth) += -(1 / val);
                G(jth, ith) += -(1 / val);
            }
        }
    }
}

// Creates a matrix of size (nxm) where 'n' is the number of nodes and 'm' is the number of Independant Voltage Sources
void B_matrix(int ith, int jth, int Vsource_pos, Eigen::MatrixXcd &B) {

    if (ith == -1) { // If the ith node is 0 (meaning positive terminal faces ground)...
        B(jth, Vsource_pos) = -1; //
    }
    else if (jth == -1) {
        B(ith, Vsource_pos) = 1;
    }
    else if (ith >= 0 && jth >= 0) {
        B(ith, Vsource_pos) = 1;
        B(jth, Vsource_pos) = -1;
    }
}

// Creates a matrix of known values containing the sum of current values at each node or a Current Source, and the value of Independant Voltage Sources
void Curr_matrix(int node_max, int ith, int jth, double val, std::string type, int Vsource_pos, Eigen::VectorXcd &Curr) {

    if (type == "I") { // If a current source is in the circuit...
        if (ith == -1) { // And it is facing towards the node...
            Curr(jth) = val; // Store the positive current value at the nth node position
        }
        else if (jth == -1) { // If it is facing away from the node...
            Curr(ith) = -(val); // -- negative --
        }
        else if (ith >= 0 && jth >= 0) { // If connected between 2 non-reference nodes...
            Curr(ith) = -(val);
            Curr(jth) = val;
        }
    }

    if (type == "V" || type == "VS") { // If a voltage source is defined...
        Curr(node_max + Vsource_pos) = val; // The corresponding value is stored after the nth node in the vector of knowns, curr_Mat
    }

}

// Populates the matrix coefficients of the conductance matrix and the vector of knowns
void mat_Populate(const std::vector<Component*>& components, int node_max, double omega, Eigen::MatrixXcd& Cond, Eigen::VectorXcd& Volt, Eigen::VectorXcd& Curr) {

    int ith, jth; // [ith, jth] positon in the matrix/vector

    int total_Vsources = 0; // Total number of Independant Voltage Sources
    int Vsource_pos = 0;  // Keeps track of the kth voltage source

    bool is_AC = false; // Whether the source is AC/DC

    for (auto* component: components) {
        if (component->get_identifier() == 'V') { // If a voltage source is found...
            total_Vsources += 1; // Add to the total number of IVSs
            if (component->is_vs()) {
                is_AC = true;
            }
        }
    }

    Cond.resize(node_max + total_Vsources, node_max + total_Vsources); // Conductance matrix resized to the nth node plus the kth IVS
    Volt.resize(node_max + total_Vsources); // Vector of unknowns
    Curr.resize(node_max + total_Vsources); // Vector of knowns

    Cond.setZero(), Volt.setZero(), Curr.setZero(); // Initialise coefficients to 0, otherwise they have a random value

    Eigen::MatrixXcd G(node_max, node_max); // Used in G_matrix()
    Eigen::MatrixXcd B(node_max, total_Vsources); // Used in B_matrix()
    Eigen::MatrixXcd C(total_Vsources, node_max); // Transpose of B
    Eigen::MatrixXcd D(total_Vsources, total_Vsources); // Zero matrix

    G.setZero(), B.setZero(), C.setZero(), D.setZero();

    for (auto* component: components) {
        ith = component->get_nodes()[0] - 1; // ith position ( "-1" is there because Eigen::Matrix matrices start from [0,0]
        jth = component->get_nodes()[1] - 1; // jth position --

        if (component->get_identifier() == 'V') { // If an IVS is found...
            if (component->is_vs()) {
                B_matrix(ith, jth, Vsource_pos, B); // --
                Curr_matrix(node_max, ith, jth, component->get_amplitude(), "VS", Vsource_pos, Curr); // --
                Vsource_pos += 1; // --
            } else {
                B_matrix(ith, jth, Vsource_pos, B); // Constructs B matrix, which is used to make the C matrix
                Curr_matrix(node_max, ith, jth, component->converted_value(), "V", Vsource_pos, Curr); // IVS added to vector of knowns
                Vsource_pos += 1; // Only used for more than one source
            }
        } else if (component->get_identifier() == 'I') { // If a Current Source is found...
            Curr_matrix(node_max, ith, jth, component->converted_value(), "I", Vsource_pos, Curr); // Adds the corresponding source value to the vector of knowns
        } else {
            G_matrix(ith, jth, component->converted_value(), std::string(1, component->get_identifier()), is_AC, omega, G); // Constructs the matrix G
        }
    }

    C = B.transpose(); // As mentioned

    Cond << G, B, C, D; // Conductance matrix of size (m+n) x (n+m)
}    // Visual representation below

/*

Number of nodes: 3
Number of IVSs: 2

[ " , " , " ] [ " , " ]        [v_1]        [0]
[ " , G , " ] [ B , B ]        [v_2]        [0]
[ " , " , " ] [ " , " ]  X  [v_3]    =    [0]
[ " , C , " ] [ 0 , 0 ]        [I_V1]        [V1]
[ " , C , " ] [ 0 , 0 ]        [I_V2]        [V2]

*/

// Sums all the conductances connected to a node, and places it along the diagonal
void cond_Diagonal(Eigen::MatrixXcd& Cond, int k, int node_max) {
    double total_real = 0; // Total of the passive components
    double total_imag = 0; // Total of the reactive components
    for (int m = 0; m < node_max; m++) { // Iterate through all nodes
        if (Cond(k, m).real() != 0 || Cond(k, m).imag() != 0) { // If the [k, m] real or imaginary positions aren't 0...
            total_real += abs(Cond(k, m).real()); // The conductance at A[k, m] is added to the total (Absolute value to remove the negatives)
            total_imag += Cond(k, m).imag(); // The admittance --
        }
    }
    Cond(k, k).real(total_real); // Total real values are placed along the diagonal, e.g. all conductances connected to node 2 are summed and placed in the [2, 2] position ([1, 1] in Eigen)
    Cond(k, k).imag(total_imag); // Total imaginary values --
}


std::complex<double> run_simulation(const std::vector<Component*>& components, double omega, int node_max, int output_node) {
    Eigen::MatrixXcd cond_Mat;
    Eigen::VectorXcd volt_Mat;
    Eigen::VectorXcd curr_Mat;

    mat_Populate(components, node_max, omega, cond_Mat, volt_Mat, curr_Mat);

    for (int k = 0; k < node_max; k++) {
        cond_Diagonal(cond_Mat, k, node_max);
    }

    return cond_Mat.partialPivLu().solve(curr_Mat)[output_node - 1];
}
