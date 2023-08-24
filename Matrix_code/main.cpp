#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <complex>
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>

// Finds the largest node in the vector of strings, so the matrix can be initialised with it's size
int node_Max(std::vector<std::string> netLine, int& node_max) {

	int node_a, node_b;
	node_a = std::stoi(netLine[1]); // The first node mentioned in the vector string
	node_b = std::stoi(netLine[2]); // The last node mentioned in the vector string

	if (node_a > node_b && node_a > node_max) { // If the first node is greater than the last node, and greater than the maximum so far then... *
		node_max = node_a; // Set node_max to the first node
	}

	else if (node_b > node_a && node_b > node_max) { // * -- last -- first
		node_max = node_b; // Set node_max to the last node
	}
	return node_max; // Returns the maximum node found
}

// Creates a matrix using the passive elements in the circuit of size (nxn) for n nodes
void G_matrix(int ith, int jth, double val, std::string type, bool is_AC, double omega, Eigen::MatrixXcd &G) {

	std::complex <double> complex_val; // Used for capacitors/inductors in an AC circuit

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
				G(ith, jth) += -complex_val;
				G(jth, ith) += -complex_val;
			}
		}
	}
	else if (type == "L") { // If the type is Inductor...
		if (is_AC) { // --
			complex_val.imag(1/(val*omega)); // Reactive component is equal to j*(1/w*L)
			if (ith == -1) {
				G(jth, jth) += complex_val; // Admittance of an inductor (Similar to resistance)
			}
			else if (jth == -1) {
				G(ith, ith) += complex_val; // Minus is used because the admittance of an inductor is 1/jwL, and 1/j is equal to -j, hence the minus
			}
			else {
				G(ith, jth) += -complex_val;
				G(jth, ith) += -complex_val;
			}
		}
		else {
			if (ith == -1) {
				G(jth, jth) = (1 / val);
			}
			else if (jth == -1) {
				G(ith, ith) = (1 / val);
			}
			else {
				G(ith, jth) = -(1 / val);
				G(jth, ith) = -(1 / val);
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
void Curr_matrix(int node_max, int ith, int jth, double val, std::string type, int Vsource_pos, Eigen::VectorXcd &Curr){

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
void mat_Populate(std::vector<std::vector<std::string>> const& netList, const int& node_max, double& omega, Eigen::MatrixXcd& Cond, Eigen::VectorXcd& Volt, Eigen::VectorXcd& Curr) {

	int ith, jth, // [ith, jth] positon in the matrix/vector
		total_Vsources = 0, // Total number of Independant Voltage Sources
		Vsource_pos = 0;  // Keeps track of the kth voltage source
	double val; // Value of each component
	bool is_AC = false; // Whether the source is AC/DC

	std::string type; // Type of component

	for (int i = 0; i < netList.size(); i++) {
		type = netList[i][0]; // First string in a line of strings indicates the type of component

		if (type == "V") { // If a voltage source is found...
			total_Vsources += 1; // Add to the total number of IVSs
		}
		else if (type == "VS") { // If an AC voltage source is found
			total_Vsources += 1; // --
			is_AC = true;
		}
	}

	Cond.resize(node_max + total_Vsources, node_max + total_Vsources); // Conductance matrix resized to the nth node plus the kth IVS
	Volt.resize(node_max + total_Vsources); // Vector of unknowns
	Curr.resize(node_max + total_Vsources); // Vector of knowns

	Cond.setZero(), Volt.setZero(), Curr.setZero(); // Initialise coefficients to 0, otherwise they have a random value

	Eigen::MatrixXcd G(node_max, node_max), // Used in G_matrix()
					 B(node_max, total_Vsources), // Used in B_matrix()
					 C(total_Vsources, node_max), // Transpose of B
					 D(total_Vsources, total_Vsources); // Zero matrix

	G.setZero(), B.setZero(), C.setZero(), D.setZero();

	for (int j = 0; j < netList.size(); j++) {

		type = netList[j][0];
		ith = std::stoi(netList[j][1]) - 1; // ith position ( "-1" is there because Eigen::Matrix matrices start from [0,0]
		jth = std::stoi(netList[j][2]) - 1; // jth position --
		val = std::stod(netList[j][3]);

		if (type == "V") { // If an IVS is found...
			B_matrix(ith, jth, Vsource_pos, B); // Constructs B matrix, which is used to make the C matrix
			Curr_matrix(node_max, ith, jth, val, type, Vsource_pos, Curr); // IVS added to vector of knowns
			Vsource_pos += 1; // Only used for more than one source
		}
		else if (type == "VS") { // If an AC IVS is found...
			B_matrix(ith, jth, Vsource_pos, B); // --
			Curr_matrix(node_max, ith, jth, val, type, Vsource_pos, Curr); // --
			Vsource_pos += 1; // --
		}
		else if (type == "I") { // If a Current Source is found...
			Curr_matrix(node_max, ith, jth, val, type, Vsource_pos, Curr); // Adds the corresponding source value to the vector of knowns
		}
		else {
			G_matrix(ith, jth, val, type, is_AC, omega, G); // Constructs the matrix G
		}
	}

	C = B.transpose(); // As mentioned

	Cond << G, B, C, D; // Conductance matrix of size (m+n) x (n+m)
}	// Visual representation below

/*

Number of nodes: 3
Number of IVSs: 2

[ " , " , " ] [ " , " ]		[v_1]		[0]
[ " , G , " ] [ B , B ]		[v_2]		[0]
[ " , " , " ] [ " , " ]  X  [v_3]	=	[0]
[ " , C , " ] [ 0 , 0 ]		[I_V1]		[V1]
[ " , C , " ] [ 0 , 0 ]		[I_V2]		[V2]

*/

// Sums all the conductances connected to a node, and places it along the diagonal
void cond_Diagonal(Eigen::MatrixXcd& Cond, int k, int node_max) {
	double total_real = 0; // Total of the passive components
	double total_imag = 0; // Total of the reactive components
	for (int m = 0; m < node_max; m++) { // Iterate through all nodes
		if (Cond(k, m).real() != 0 || Cond(k, m).imag() != 0) { // If the [k, m] real or imaginary positions aren't 0...
			total_real += abs(Cond(k, m).real()); // The conductance at A[k, m] is added to the total (Absolute value to remove the negatives)
			total_imag += abs(Cond(k, m).imag()); // The admittance --
		}
		else { // Else, continue the iteration 
			continue;
		}
	}
	Cond(k, k).real(total_real); // Total real values are placed along the diagonal, e.g. all conductances connected to node 2 are summed and placed in the [2, 2] position ([1, 1] in Eigen)
	Cond(k, k).imag(total_imag); // Total imaginary values --
}

int main(){

	double omega = 0; // Omega from netlist
	int node_max = 0;
	
	std::vector<std::vector<std::string>> netList{
		{"R", "1", "2", "10"},
		{"I", "3", "2", "0.5"},
		{"R", "2", "3", "10"},
		{"C", "2", "0", "0.001"},
		{"L", "3", "0", "0.001"},
		{"V", "1", "0", "10"}
	};
	
	for (int i = 0; i < netList.size(); i++) {
		node_max = node_Max(netList[i], node_max);
	}
	
	Eigen::MatrixXcd cond_Mat;
	Eigen::VectorXcd volt_Mat;
	Eigen::VectorXcd curr_Mat;
	
	mat_Populate(netList, node_max, omega, cond_Mat, volt_Mat, curr_Mat);
	
	for (int k = 0; k < node_max; k++) {
		cond_Diagonal(cond_Mat, k, node_max);
	}
	
	std::cout << "Conductance Matrix: " << std::endl;
	std::cout << cond_Mat << std::endl;
	
	std::cout << "Voltage Vector: " << std::endl;
	std::cout << volt_Mat << std::endl;

	std::cout << "Current Vector: " << std::endl;
	std::cout << curr_Mat << std::endl;
	
	std::cout << "Voltage Solutions: " << std::endl;
	volt_Mat << cond_Mat.partialPivLu().solve(curr_Mat);
	std::cout << volt_Mat << std::endl;


	
}	



