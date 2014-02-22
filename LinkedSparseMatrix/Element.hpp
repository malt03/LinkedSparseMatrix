namespace lsm{
	template<class TYPE>
	SparseMatrix<TYPE>::HeadElement::
	Element::Element(const Element& origin){
		col_ = origin.col_;
		val_ = origin.val_;
		next_ = NULL;
	}

	template<class TYPE>
	SparseMatrix<TYPE>::HeadElement::
	Element::Element(const int col, const TYPE& val){
		col_ = col;
		val_ = val;
		next_ = NULL;
	}
	
	template<class TYPE>
	SparseMatrix<TYPE>::HeadElement::
	Element::Element(const int col, Element* next, const TYPE val){
		col_ = col;
		next_ = next;
		val_ = val;
	}

	template<class TYPE>
	class SparseMatrix<TYPE>::HeadElement::Element&
	SparseMatrix<TYPE>::HeadElement::
	Element::operator=(const TYPE val){
		val_ = val;
		return *this;
	}

	template<class TYPE>
	class SparseMatrix<TYPE>::HeadElement::Element&
	SparseMatrix<TYPE>::HeadElement::
	Element::operator+=(const TYPE val){
		val_ += val;
		return *this;
	}

	template<class TYPE>
	class SparseMatrix<TYPE>::HeadElement::Element&
	SparseMatrix<TYPE>::HeadElement::
	Element::operator-=(const TYPE val){
		val_ -= val;
		return *this;
	}
}
