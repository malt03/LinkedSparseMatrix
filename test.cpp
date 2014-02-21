#include "LinkedSparseMatrix.hpp"
using namespace lsm;

int main(){
	SparseMatrix psi(5,5), phi(5,5);

	for(int i=0;i<5;i++){
		psi[i][i] = cmplx(2);
		phi[i][i] = cmplx(-2);
	}
	/*
	phi[0][0] = cmplx(-2);
	phi[0][3] = cmplx(-3);
	psi[0][3] = cmplx(3);
	phi[1][0] = cmplx(2,2);
	phi[1][4] = cmplx(3,3);
	phi[4][4] = cmplx(-2);
	*/
	cout << psi << "\n" << phi << "\n" << psi+phi << endl;

  return 0;
}


