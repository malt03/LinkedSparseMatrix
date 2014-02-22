#include "LinkedSparseMatrix/LinkedSparseMatrix.hpp"
using namespace std;

int main(){
	lsm::SparseMatrix<int> psi(5,5), phi(5,5);

	for(int i=0;i<5;i++){
		psi[i][i] = 2;
	}

	phi[0][0] = -3;
	phi[0][3] = -2;
	psi[0][3] = 3;
	phi[1][0] = 2;
	phi[1][4] = 3;
	phi[3][3] = 2;
	phi[3][0] = 2;

	cout << psi << "\n" << phi << endl;

	psi *= phi;
	cout << psi << endl;

  return 0;
}
