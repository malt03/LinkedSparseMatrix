#include <iostream>
#include <complex>
using namespace std;

namespace lsm{
  typedef complex<double> cmplx;
  
  class SparseMatrix;
  class tmpForArraySubScript;
  class element;
  class headElement;

  class SparseMatrix{
	cmplx default_num_;
	int col_num_, row_num_;
	headElement* first_col_element_;
	headElement* first_row_element_;
	headElement* getHeadElement(const int num, const bool is_col);
	tmpForArraySubScript* tmpForArraySubScript_;
  public:
	SparseMatrix(){};
	SparseMatrix(const int row_num, const int col_num, const cmplx default_num = cmplx(0));
	~SparseMatrix();

	element& e(const int row, const int col);
	cmplx val(const int row, const int col) const;

	SparseMatrix& operator=(const SparseMatrix& phi);
	const SparseMatrix operator+(const SparseMatrix& phi) const;
	const SparseMatrix operator*(const SparseMatrix& phi) const;
	SparseMatrix& operator+=(const SparseMatrix& phi);
	SparseMatrix& operator*=(const SparseMatrix& phi);
	bool operator==(const SparseMatrix& phi) const;
	bool operator!=(const SparseMatrix& phi) const;
	const SparseMatrix tensor(SparseMatrix& phi) const;
	SparseMatrix& tensorEqual(SparseMatrix& phi);

	tmpForArraySubScript& operator[](int row);
	
	cmplx getDefaultNum() const{return default_num_;}
	headElement* getFirstRowElement() const{return first_row_element_;}
  };

  class tmpForArraySubScript{
	SparseMatrix* root_;
	int row_;
  public:
	tmpForArraySubScript(SparseMatrix* root);
	tmpForArraySubScript& setRow(int row);
	element& operator[](int col);
  };

  class element{
  public:
	cmplx value_;
	int col_;
	int row_;
	element* col_next_;
	element* row_next_;
	
	element(const int row, const int col, element* row_next = NULL, element* col_next = NULL, const cmplx value = cmplx(0));

	cmplx operator=(const cmplx value);
	cmplx operator=(const element e);
  };
  
  class headElement{
  public:
	int num_;
	headElement* next_;
	element* first_element_;

	headElement(const int num, headElement* next = NULL, element* first_element = NULL);
  };


  /*********************************functions*********************************/

  SparseMatrix::SparseMatrix(const int row_num, const int col_num, const cmplx default_num){
	row_num_ = row_num;
	col_num_ = col_num;
	default_num_ = default_num;
	first_row_element_ = NULL;
	first_col_element_ = NULL;
	tmpForArraySubScript_ = new tmpForArraySubScript(this);
  }

  SparseMatrix::~SparseMatrix(){
	element *tmp1, *tmp2;
	headElement *htmp1, *htmp2;
	
	for(htmp1 = first_col_element_; htmp1 != NULL; htmp1 = htmp2){
	  for(tmp1 = htmp1->first_element_ ; tmp1 != NULL; tmp1 = tmp2){
		tmp2 = tmp1->row_next_;
		delete tmp1;
	  }
	  htmp2 = htmp1->next_;
	  delete htmp1;
	}
	for(htmp1 = first_row_element_; htmp1 != NULL; htmp1 = htmp2){
	  htmp2 = htmp1->next_;
	  delete htmp1;
	}

	delete tmpForArraySubScript_;
  }

  headElement* SparseMatrix::getHeadElement(const int num, const bool is_col){
	headElement* front;
	headElement* back;
	if(is_col){
	  if(first_col_element_ == NULL) return (first_col_element_ = new headElement(num));
	  if(num < first_col_element_->num_) return (first_col_element_ = new headElement(num, first_col_element_));
	  back = front = first_col_element_;
	}else{
	  if(first_row_element_ == NULL) return (first_row_element_ = new headElement(num));
	  if(num < first_row_element_->num_) return (first_row_element_ = new headElement(num, first_row_element_));
	  back = front = first_row_element_;
	}

	for(;(front != NULL && front->num_ <= num);
		front = front->next_) back = front;

	if(back->num_ == num) return back;
	return (back->next_ = new headElement(num, front));
  }

  element& SparseMatrix::e(const int row, const int col){
	element* front;
	element* back;

	headElement* head = getHeadElement(col, true);
	element* tmp;

	if(head->first_element_ == NULL)  tmp = head->first_element_ = new element(row, col);
	else if(row < head->first_element_->row_) tmp = head->first_element_ = new element(row, col, head->first_element_);
	else{
	  for(back = front = head->first_element_;
		  front != NULL && (front->row_ <= row);
		  front = front->row_next_) back = front;
	  if(back->row_ == row) return *back;
	  tmp = back->row_next_ = new element(row, col, front);
	}

	head = getHeadElement(row, false);

	if(head->first_element_ == NULL)  head->first_element_ = tmp;
	else if(col < head->first_element_->col_){
	  tmp->col_next_ = head->first_element_;
	  head->first_element_ = tmp;
	}else{
	  for(back = front = head->first_element_;
		  (front != NULL && front->col_ <= col);
		  front = front->col_next_) back = front;
	  back->col_next_ = tmp;
	  tmp->col_next_  = front;
	}

	return *tmp;
  }

  cmplx SparseMatrix::val(const int row, const int col) const{
	headElement *hfront, *hback;
	for(hfront = hback = first_col_element_;
		(hfront != NULL && hfront->num_ <= col);
		hfront = hfront->next_) hback = hfront;
	if(hback->num_ != col) return cmplx(0);

	element *efront, *eback;
	for(efront = eback = hback->first_element_;
		(efront != NULL && efront->row_ <= row);
		efront = efront->row_next_) eback = efront;
	if(eback->row_ != row) return cmplx(0);

	return eback->value_;
  }
  /*
  SparseMatrix& SparseMatrix::operator=(const SparseMatrix& phi){
	
  }

  const SparseMatrix SparseMatrix::operator+(const SparseMatrix& phi) const{
  }
  
  const SparseMatrix SparseMatrix::operator*(const SparseMatrix& phi) const{
  }
  
  SparseMatrix& SparseMatrix::operator+=(const SparseMatrix& phi){
  }
  
  SparseMatrix& SparseMatrix::operator*=(const SparseMatrix& phi){
  }
  
  bool SparseMatrix::operator==(const SparseMatrix& phi) const{
  }
  
  bool SparseMatrix::operator!=(const SparseMatrix& phi) const{
  }
  
  const SparseMatrix SparseMatrix::tensor(SparseMatrix& phi) const{
  }
  
  SparseMatrix& SparseMatrix::tensorEqual(SparseMatrix& phi){
  }*/

  tmpForArraySubScript& SparseMatrix::operator[](int row){
	return tmpForArraySubScript_->setRow(row);
  }
  tmpForArraySubScript::tmpForArraySubScript(SparseMatrix* root){root_ = root;}
  tmpForArraySubScript& tmpForArraySubScript::setRow(int row){
	row_ = row;
	return *this;
  }
  element& tmpForArraySubScript::operator[](int col){
	return root_->e(row_, col);
  }

  element::element(const int row, const int col, element* row_next, element* col_next, const cmplx value){
	row_ = row;
	col_ = col;
	row_next_ = row_next;
	col_next_ = col_next;
	value_ = value;
  }

  cmplx element::operator=(const cmplx value){
	value_ = value;
  }
  cmplx element::operator=(const element e){
	value_ = e.value_;
  }

  headElement::headElement(const int num, headElement* next, element* first_element){
	num_ = num;
	next_ = next;
	first_element_ = first_element;
  }

  /*********************************operators*********************************/
  ostream& operator<<(ostream& os, const SparseMatrix& phi){
	element* tmp;
	headElement* htmp;

	for(htmp = phi.getFirstRowElement(); htmp != NULL; htmp = htmp->next_){
	  os << htmp->num_ << "; ";
	  for(tmp = htmp->first_element_; tmp != NULL; tmp = tmp->col_next_)
		os << tmp->col_ << ":" << tmp->value_ << ", ";
	  os << "\n";
	}

	return os;
  }

  ostream& operator<<(ostream& os, const element e){
	os << e.value_;
	return os;
  }
}
