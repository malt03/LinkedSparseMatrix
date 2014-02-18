#include "LinkedSparseMatrix.hpp"
using namespace lsm;

int main(){
  SparseMatrix phi(10,10);

  phi[100][2] = cmplx(1.2, 2.1);
  phi[100][3] = cmplx(0.2, 2.1);

  phi[100][200] = cmplx(0.2, 2.1);

  phi[10][200] = cmplx(0.2, 2.1);
  phi[10][300] = cmplx(0.2, 2.1);

  phi[100][300] = cmplx(0.2, 2.1);
  phi[100][200] = cmplx(0.1, 2.2);
  
  cout << phi << endl;
  cout << phi.val(10,200) << endl;
  cout << phi.val(5,5) << endl;
  cout << phi[100][200] << endl;

  return 0;
}


