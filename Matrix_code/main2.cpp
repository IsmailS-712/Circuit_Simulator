#include <iostream>
#include <string>
#include <vector>
#include <Eigen/Dense>

// Finds the largest node in the vector of strings, so the matrix can be initialised with it's size
int nodeMax(std::vector<std::string> nodeVector, int& node_max) {
	int node_a, node_b;
	node_a = std::stoi(nodeVector[1]); // The first node mentioned in the vector string
	node_b = std::stoi(nodeVector[2]); // The last node mentioned in the vector string

	if (node_a > node_b && node_a > node_max) { // If the first node is greater than the last node, and greater than the maximum so far then... *
		node_max = node_a; // Set node_max to the first node
	}

	else if (node_b > node_a && node_b > node_max) { // * -- last -- first
		node_max = node_b; // Set node_max to the last node
	}
	return node_max; // Returns the maximum node found
}

void G_matrix(std::vector<std::string> nodeVector, int node_max, int ith, int jth, double val, std::string type, Eigen::MatrixXd &G) {

	if (type == "R") { // If the type is a Resistor...
		if (ith == -1) {
			G(jth, jth) = val;
		}
		else if (jth == -1) {
			G(ith, ith) = val;
		}
		else {
			G(ith, jth) = -(val); // The corresponding negative reciprocal of the value is stored in  the [ith, jth] position (Conductance of a resistor is it's reciprocal)
			G(jth, ith) = -(val); // Because of Kirchoff's Current Law, the matrix is symmetric
		}
	}
	else if (type == "C") { // If the type is Capacitor...
		G(ith, jth) = -(val); // Admittance of a capacitor (1/Impedance)
		G(jth, ith) = -(val); // --
	}
	else if (type == "L") { // If the type is Inductor...
		G(ith, jth) = -(1 / val); // Admittance of an inductor (Similar to resistance)
		G(jth, ith) = -(1 / val); // --
	}

}

void B_matrix(std::vector<std::string> nodeVector, int node_max, int ith, int jth, int total_Vsources, double val, Eigen::MatrixXd &B) {
	
	Eigen::MatrixXd B(node_max, total_Vsources);
}

// Places conductances/admittances at their appropriate [i, j] position from the vector, nodeVector
void cond_Populate(std::vector<std::string> nodeVector, int node_max, Eigen::MatrixXd &C) {
	int ith, jth, total_Vsources = 0; // ith/jth positon in the matrix, and the value that will be stored there
	double val;
	std::string type;

	type = nodeVector[0]; // Type of component
	ith = std::stod(nodeVector[1]) - 1; // ith position ( "-1" is there because Eigen::Matrix matrices start from [0,0]
	jth = std::stod(nodeVector[2]) - 1; // jth position --
	val = std::stod(nodeVector[3]); // Value of the component

	Eigen::MatrixXd Cond, G, B, C, D;

	G_matrix(nodeVector, node_max, ith, jth, val, type, C);
	if (type == "V") {
		total_Vsources += 1;
		B.resize(node_max, total_Vsources);
		B_matrix(nodeVector, node_max, ith, jth, total_Vsources, val, B);
	}

	/*int ith, jth; // ith/jth positon in the matrix, and the value that will be stored there
	double val;
	std::string type;

	type = nodeVector[0]; // Type of component
	ith = std::stod(nodeVector[1]) - 1; // ith position ( "-1" is there because Eigen::Matrix matrices start from [0,0]
	jth = std::stod(nodeVector[2]) - 1; // jth position --
	val = std::stod(nodeVector[3]); // Value of the component

	else if (type == "V") { // If the type is an Independant Voltage Source...
		if (ith == -1) { // If the ith node is 0 (meaning positive terminal faces ground)...
			for (int i = 0; i < node_max; i++) { // Iterate through the row and column connected to the jth node
				C(jth, i) = 0; // Set all nodes connected to the jth node to 0
				C(i, jth) = 0; // --
			}
			C(jth, jth) = -1; // Set the value at the jth node to -1
			I(jth) = val; // Value of voltage source stored in the corresponding row in the current vector
		}
		else if (jth == -1) { // If the jth node is 0 (meaning negative terminal faces ground)...
			for (int j = 0; j < node_max; j++) { // Iterate through the row and column connected to the ith node
				C(ith, j) = 0; // Set all nodes connected to the ith node to 0
				C(j, ith) = 0; // --
			}
			C(ith, ith) = 1; // Set the value at the ith node to 1
			I(ith) = val; // --
		}
	}
	else if (type == "I") {
		if (ith == -1) {
			I(jth) = val;
		}
		else if (jth == -1) {
			I(ith) = -val;
		}
	}*/
}

// Sums all the conductances connected to a node, and places it along the diagonal
void cond_Diagonal(Eigen::MatrixXd& A, int k, int node_max) {
	double total = 0;
	for (int m = 0; m < node_max; m++) { // Iterate through all nodes
		if (A(k, m) != 0) { // If the [k, m] position isn't 0...
			total += abs(A(k, m)); // The conductance at A[k, m] is added to the total (Absolute value to remove the negatives)
		}
		else { // Else, continue the iteration 
			continue;
		}
	}
	if (A(k, k) >= 0) {
		A(k, k) = total; // Total is placed along the diagonal, e.g. all conductances connected to node 2 are summed and placed in the [2, 2] position ([1, 1] in Eigen)
	}
}


int main(){

	float omega = 0;
	int node_max = 0;
	
	std::vector<std::vector<std::string>> v{
	{ "R", "2", "0", "1" },
	{ "R", "1", "3", "2" },
	{ "R", "4", "1", "3" },
	{ "R", "2", "3", "7" },
	{ "R", "3", "4", "5" },
	//{ "I", "1", "0", "20" },
	{ "V", "0", "2", "30" }
	};

	for (int i = 0; i < v.size(); i++) {
		node_max = nodeMax(v[i], node_max);
	}

	Eigen::MatrixXd cond_Mat(node_max, node_max);
	Eigen::VectorXd volt_Mat(node_max, 1);
	Eigen::VectorXd curr_Mat(node_max, 1);
	cond_Mat.setZero();
	volt_Mat.setZero();
	curr_Mat.setZero();

	std::cout << cond_Mat << std::endl;
	
	for (int j = 0; j < v.size(); j++) {
		cond_Populate(v[j], node_max, cond_Mat);
	}

	for (int k = 0; k < node_max; k++) {
		cond_Diagonal(cond_Mat, k, node_max);
	}

	std::cout << "Conductance Matrix: " << std::endl;
	std::cout << cond_Mat << std::endl;

	//std::cout << "Voltage Vector: " << std::endl;
	//std::cout << volt_Mat << std::endl;

	//std::cout << "Current Vector: " << std::endl;
	//std::cout << curr_Mat << std::endl;

	//std::cout << "Inverse of Conductance Matrix: " << std::endl;
	//std::cout << cond_Mat.inverse() << std::endl;
}	



