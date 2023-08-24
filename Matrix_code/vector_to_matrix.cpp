#include <iostream>
#include <string>
#include <vector>
#include <Eigen/Dense>

int main() {

	int nodes=0, nums;
	std::vector<std::vector<int>> v;

	std::cout << "Enter number of nodes: " << std::endl;
	std::cin >> nodes;

	for (int i = 0; i < nodes; i++) {
		for (int j = 0; j < nodes; j++) {
			std::cout << std::endl << "Enter number at position [" << i + 1 << j + 1 << "] ";
			std::cin >> nums;
			v[i][j] = nums;
			std::cout << v[i][j] << std::endl;
		}
	}

	Eigen::MatrixXd A;
	std::cout << "Number of nodes: " << std::endl;
	std::cin >> nodes;
	A.resize(nodes, nodes);

	for (int a = 0; a < nodes; a++) {
		for (int b = 0; b < nodes; b++) {
			A(a, b) = v[a][b];
		}
	}

	std::cout << "Matrix A looks like: " << std::endl << A << std::endl;
}