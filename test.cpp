#include "LinkedSparseMatrix/LinkedSparseMatrix.hpp"
using namespace lsm;

int main(){
	SparseMatrix<int> psi(5,5), phi(5,5);

	for(int i=0;i<5;i++){
		psi[i][i] = 2;
		phi[i][i] = -2;
	}

	phi[0][0] = -2;
	phi[0][3] = -3;
	psi[0][3] = 3;
	phi[1][0] = 2;
	phi[1][4] = 3;
	phi[4][4] = -2;

	std::cout << psi << "\n" << phi << "\n" << psi+phi << std::endl;

  return 0;
}
