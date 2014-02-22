#ifndef __LINKED_SPARSE_MATRIX__
#define __LINKED_SPARSE_MATRIX__

#include <iostream>
#include <sstream>

#include "ProtoType.hpp"
#include "Element.hpp"
#include "HeadElement.hpp"
#include "SparseMatrix.hpp"	

namespace lsm{
	template<class TYPE>
	std::ostream& operator<<(std::ostream& os, const SparseMatrix<TYPE>& phi){
		os << phi.str();

		return os;
	}
}

#endif
