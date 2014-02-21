#include <iostream>
#include <complex>
using namespace std;

namespace lsm{
	typedef complex<double> cmplx;

	class SparseMatrix;

	class SparseMatrix{

		class HeadElement{

			class Element{
				cmplx val_;
				int col_;
				Element* next_;
			public:
				Element(){}
				Element(const Element& origin);
				Element(const int col, const cmplx& val);
				Element(const int col, Element* next = NULL, const cmplx val = cmplx(0));

				cmplx val() const{return val_;}
				int col() const{return col_;}
				Element* next() const{return next_;}
				Element* setNext(Element* next){return (next_ = next);}
		
				Element& operator=(const cmplx val);
				Element& operator+=(const cmplx val);
			};

			int row_;
			HeadElement* next_;
			Element* first_element_;
		public:
			HeadElement(const HeadElement& origin);
			HeadElement(const HeadElement& phi, const HeadElement& psi, const int row);
			HeadElement(const HeadElement& phi, const SparseMatrix& psi, const int row);
			HeadElement(const int row, HeadElement* next = NULL, Element* first_element = NULL);
			~HeadElement();

			int row() const{return row_;}
			HeadElement* next() const{return next_;}
			HeadElement* setNext(HeadElement* next){return (next_ = next);}
			bool isNull() const{return (first_element_ == NULL);}
			string str() const;

			cmplx val(const int col) const;
			Element& operator[](int col);

			HeadElement& operator+=(const HeadElement& phi);
		};
	
		cmplx default_num_;
		int col_num_, row_num_;
		HeadElement* first_head_element_;

		HeadElement* getHeadElement(const int num);
	public:
		SparseMatrix(){};
		SparseMatrix(const SparseMatrix& origin);
		SparseMatrix(const int row_num, const int col_num, const cmplx default_num = cmplx(0));
		~SparseMatrix();

		string str() const;

		cmplx val(const int row, const int col) const;
		HeadElement& operator[](const int row);

		SparseMatrix& operator=(const SparseMatrix& phi);
		const SparseMatrix operator+(const SparseMatrix& phi) const;
		const SparseMatrix operator*(const SparseMatrix& phi) const;
		SparseMatrix& operator+=(const SparseMatrix& phi);
		SparseMatrix& operator*=(const SparseMatrix& phi);
		bool operator==(const SparseMatrix& phi) const;
		bool operator!=(const SparseMatrix& phi) const;
		const SparseMatrix tensor(SparseMatrix& phi) const;
		SparseMatrix& tensorEqual(SparseMatrix& phi);
	
		cmplx getDefaultNum() const{return default_num_;}
	};


	/*********************************functions*********************************/

	//Element
	SparseMatrix::HeadElement::Element::Element(const Element& origin){
		col_ = origin.col_;
		val_ = origin.val_;
		next_ = NULL;
	}

	SparseMatrix::HeadElement::Element::Element(const int col, const cmplx& val){
		col_ = col;
		val_ = val;
		next_ = NULL;
	}
	
	SparseMatrix::HeadElement::Element::Element(const int col, Element* next, const cmplx val){
		col_ = col;
		next_ = next;
		val_ = val;
	}

	SparseMatrix::HeadElement::Element& SparseMatrix::HeadElement::Element::operator=(const cmplx val){
		val_ = val;
		return *this;
	}

	SparseMatrix::HeadElement::Element& SparseMatrix::HeadElement::Element::operator+=(const cmplx val){
		val_ += val;
		return *this;
	}

	//HeadElement
	SparseMatrix::HeadElement::HeadElement(const HeadElement& origin){
		row_ = origin.row_;
		next_ = NULL;

		Element* origin_tmp = origin.first_element_->next();
		Element* this_tmp = first_element_ = new Element(*origin.first_element_);
		for(; origin_tmp != NULL; origin_tmp = origin_tmp->next()){
			this_tmp = (this_tmp->setNext(new Element(*origin_tmp)));
		}
	}

	SparseMatrix::HeadElement::HeadElement(const HeadElement& phi, const HeadElement& psi, const int row){
		row_ = row;
		next_ = NULL;

		Element* phi_tmp = phi.first_element_;
		Element* psi_tmp = psi.first_element_;
		Element* this_tmp;

		while(phi_tmp != NULL || psi_tmp != NULL){
			if(phi_tmp->col() == psi_tmp->col()){
				if(phi_tmp->val() + psi_tmp->val() == cmplx(0)){
					first_element_ = NULL;
					phi_tmp = phi_tmp->next();
					psi_tmp = psi_tmp->next();
					continue;
				}
				this_tmp = first_element_ = new Element(phi_tmp->col(), phi_tmp->val() + psi_tmp->val());
				phi_tmp = phi_tmp->next();
				psi_tmp = psi_tmp->next();
				break;
			}else if(phi_tmp->col() < psi_tmp->col()){
				this_tmp = first_element_ = new Element(*phi_tmp);
				phi_tmp = phi_tmp->next();
				break;
			}else{
				this_tmp = first_element_ = new Element(*psi_tmp);
				psi_tmp = psi_tmp->next();
				break;
			}
		}

		while(phi_tmp != NULL && psi_tmp != NULL){
			if(phi_tmp->col() == psi_tmp->col()){
				if(phi_tmp->val() + psi_tmp->val() == cmplx(0)){
					phi_tmp = phi_tmp->next();
					psi_tmp = psi_tmp->next();
					continue;
				}
				this_tmp = (this_tmp->setNext(new Element(phi_tmp->col(), phi_tmp->val() + psi_tmp->val())));
				phi_tmp = phi_tmp->next();
				psi_tmp = psi_tmp->next();
			}else if(phi_tmp->col() < psi_tmp->col()){
				this_tmp = (this_tmp->setNext(new Element(*phi_tmp)));
				phi_tmp = phi_tmp->next();
			}else{
				this_tmp = (this_tmp->setNext(new Element(*psi_tmp)));
				psi_tmp = psi_tmp->next();
			}
		}

		if(phi_tmp == NULL) phi_tmp = psi_tmp;
		for(; phi_tmp != NULL; phi_tmp = phi_tmp->next()){
			this_tmp = (this_tmp->setNext(new Element(*phi_tmp)));
		}
	}

	SparseMatrix::HeadElement::HeadElement(const HeadElement& phi, const SparseMatrix& psi, const int row){
		row_ = row;
		next_ = NULL;
		first_element_ = NULL;

		HeadElement* psi_htmp = psi.first_head_element_;
		Element* psi_etmp;
		Element* phi_etmp = phi.first_element_;
		Element *this_efront, *this_eback;

		while(phi_etmp != NULL && psi_htmp != NULL){
			if(phi_etmp->col() == psi_htmp->row()){
				psi_etmp = psi_htmp->first_element_;

				if(first_element_ == NULL){
					this_efront = NULL;
					this_eback = first_element_ = new Element(psi_etmp->col(), psi_etmp->val() * phi_etmp->val());
					psi_etmp = psi_etmp->next();
				}else if(psi_etmp->col() < first_element_->col()){
					this_efront = first_element_;
					this_eback = first_element_ = new Element(psi_etmp->col(), first_element_, psi_etmp->val() * phi_etmp->val());
					psi_etmp = psi_etmp->next();
				}else{
					this_efront = this_eback = first_element_;
				}
			
				while(psi_etmp != NULL){
					if(this_efront == NULL){
						this_eback = (this_eback->setNext(new Element(psi_etmp->col(), psi_etmp->val() * phi_etmp->val())));
						psi_etmp = psi_etmp->next();
					}else if(psi_etmp->col() == this_efront->col()){
						*this_efront += psi_etmp->val() * phi_etmp->val();
						psi_etmp = psi_etmp->next();
					}else if(psi_etmp->col() < this_efront->col()){
						this_eback = (this_eback->setNext(new Element(psi_etmp->col(), this_efront, psi_etmp->val() * phi_etmp->val())));
						psi_etmp = psi_etmp->next();
					}else{
						for(; this_efront != NULL && this_efront->col() < psi_etmp->col(); this_efront = this_efront->next()){
							this_eback = this_efront;
						}
					}
				}

				phi_etmp = phi_etmp->next();
				psi_htmp = psi_htmp->next();
			}else if(phi_etmp->col() < psi_htmp->row()){
				phi_etmp = phi_etmp->next();
			}else{
				psi_htmp = psi_htmp->next();
			}
		}
	}
	
	SparseMatrix::HeadElement::HeadElement(const int row, HeadElement* next, Element* first_element){
		row_ = row;
		next_ = next;
		first_element_ = first_element;
	}

	SparseMatrix::HeadElement::~HeadElement(){
		Element *tmp1, *tmp2;
		for(tmp1 = first_element_; tmp1 != NULL; tmp1 = tmp2){
			tmp2 = tmp1->next();
			delete tmp1;
		}
	}

	cmplx SparseMatrix::HeadElement::val(const int col) const{
		Element *front, *back;
		for(front = back = first_element_;
				(front != NULL && front->col() <= col);
				front = front->next()) back = front;
		if(back->col() != col) return cmplx(0);

		return back->val();
	}

	string SparseMatrix::HeadElement::str() const{
		Element* tmp;
		ostringstream ostr;
		for(tmp = first_element_; tmp != NULL; tmp = tmp->next()){
			ostr << tmp->col() << ":" << tmp->val() << " ";
		}
		return ostr.str();
	}

	SparseMatrix::HeadElement::Element& SparseMatrix::HeadElement::operator[](const int col){
		Element* front;
		Element* back;

		if(first_element_ == NULL) return *(first_element_ = new Element(col));
		if(col < first_element_->col()) return *(first_element_ = new Element(col, first_element_));

		back = front = first_element_;

		for(;(front != NULL && front->col() <= col);
				front = front->next()) back = front;

		if(back->col() == col) return *back;
		return *(back->setNext(new Element(col, front)));
	}
	/*
	SparseMatrix::HeadElement::HeadElement& SparseMatrix::HeadElement::operator+=(const HeadElement& phi){
		Element* phi_tmp = phi.first_element_;
		Element* this_front = first_element_;
		Element* this_back = first_element_;

		while(phi_tmp != NULL){
			if(this_front == NULL){
				this_back = (this_back->setNext(new Element(*phi_tmp)));
				phi_tmp = phi_tmp->next();
			}else if(phi_tmp->col() == this_front->col()){
				if(phi_tmp->val() + this_front->val() == cmplx(0)){
					Element* tmp = this_front;
					this_front = this_front->next();
					this_back->setNext(this_front);
					delete tmp;
				}else{
					*phi_tmp += phi_tmp->val();
				}
				phi_tmp = phi_tmp->next();
			}else if(phi_tmp->col() < this_front->col()){
				this_back = (this_back->setNext(new Element(phi_tmp->col(), this_front, phi_tmp->val())));
				phi_tmp = phi_tmp->next();
			}else{
				for(; this_front->col() < phi_tmp->col(); this_front = this_front->next()){
					this_back = this_front;
				}
			}
		}

		return *this;
	}
	*/
	//SparseMatrix
	SparseMatrix::SparseMatrix(const SparseMatrix& origin){
		row_num_ = origin.row_num_;
		col_num_ = origin.col_num_;
		default_num_ = origin.default_num_;

		HeadElement* origin_tmp = origin.first_head_element_->next();
		HeadElement* this_tmp = first_head_element_ = new HeadElement(*origin.first_head_element_);
		for(; origin_tmp != NULL; origin_tmp = origin_tmp->next()){
			this_tmp = (this_tmp->setNext(new HeadElement(*origin_tmp)));
		}
	}
	
	SparseMatrix::SparseMatrix(const int row_num, const int col_num, const cmplx default_num){
		row_num_ = row_num;
		col_num_ = col_num;
		default_num_ = default_num;
		first_head_element_ = NULL;
	}

	SparseMatrix::~SparseMatrix(){
		HeadElement *tmp1, *tmp2;
		for(tmp1 = first_head_element_; tmp1 != NULL; tmp1 = tmp2){
			tmp2 = tmp1->next();
			delete tmp1;
		}
	}

	string SparseMatrix::str() const{
		HeadElement* tmp;
		ostringstream ostr;

		for(tmp = first_head_element_; tmp != NULL; tmp = tmp->next()){
			ostr << tmp->row() << ":	";
			ostr << tmp->str();
			ostr << "\n";
		}

		return ostr.str();
	}

	cmplx SparseMatrix::val(const int row, const int col) const{
		HeadElement *hfront, *hback;
		for(hfront = hback = first_head_element_;
				(hfront != NULL && hfront->row() <= row);
				hfront = hfront->next()) hback = hfront;
		if(hback->row() != row) return cmplx(0);

		return hback->val(col);
	}

	SparseMatrix::HeadElement& SparseMatrix::operator[](const int row){
		HeadElement* front;
		HeadElement* back;

		if(first_head_element_ == NULL) return *(first_head_element_ = new HeadElement(row));
		if(row < first_head_element_->row()) return *(first_head_element_ = new HeadElement(row, first_head_element_));

		back = front = first_head_element_;

		for(;(front != NULL && front->row() <= row);
				front = front->next()) back = front;

		if(back->row() == row) return *back;
		return *(back->setNext(new HeadElement(row, front)));
	}
	
	SparseMatrix& SparseMatrix::operator=(const SparseMatrix& phi){
		HeadElement *tmp1, *tmp2;
		for(tmp1 = first_head_element_; tmp1 != NULL; tmp1 = tmp2){
			tmp2 = tmp1->next();
			delete tmp1;
		}

		row_num_ = phi.row_num_;
		col_num_ = phi.col_num_;
		default_num_ = phi.default_num_;

		HeadElement* phi_tmp = phi.first_head_element_->next();
		HeadElement* this_tmp = first_head_element_ = new HeadElement(*phi.first_head_element_);
		for(; phi_tmp != NULL; phi_tmp = phi_tmp->next()){
			this_tmp = (this_tmp->setNext(new HeadElement(*phi_tmp)));
		}
		return *this;
	}

	const SparseMatrix SparseMatrix::operator+(const SparseMatrix& phi) const{
		SparseMatrix tmp(row_num_, col_num_, default_num_ + phi.default_num_);
		if(row_num_ != phi.row_num_ || col_num_ != phi.col_num_){
			cerr << "add error!" << endl;
			return tmp;
		}

		HeadElement* phi_tmp = phi.first_head_element_;
		HeadElement* this_tmp = first_head_element_;
		HeadElement* tmp_tmp;

		while(phi_tmp != NULL || this_tmp != NULL){
			if(phi_tmp->row() == this_tmp->row()){
				tmp_tmp = new HeadElement(*phi_tmp, *this_tmp, phi_tmp->row());
				phi_tmp = phi_tmp->next();
				this_tmp = this_tmp->next();
				if(tmp_tmp->isNull()){
					delete tmp_tmp;
					continue;
				}
				tmp.first_head_element_ = tmp_tmp;
				break;
			}else if(phi_tmp->row() < this_tmp->row()){
				tmp_tmp = tmp.first_head_element_ = new HeadElement(*phi_tmp);
				phi_tmp = phi_tmp->next();
				break;
			}else{
				tmp_tmp = tmp.first_head_element_ = new HeadElement(*this_tmp);
				this_tmp = this_tmp->next();
				break;
			}
		}

		while(phi_tmp != NULL && this_tmp != NULL){
			if(phi_tmp->row() == this_tmp->row()){
				tmp_tmp->setNext(new HeadElement(*phi_tmp, *this_tmp, phi_tmp->row()));
				phi_tmp = phi_tmp->next();
				this_tmp = this_tmp->next();
				if(tmp_tmp->next()->isNull()){
					delete tmp_tmp->next();
					tmp_tmp->setNext(NULL);
				}else{
					tmp_tmp = tmp_tmp->next();
				}
			}else if(phi_tmp->row() < this_tmp->row()){
				tmp_tmp = (tmp_tmp->setNext(new HeadElement(*phi_tmp)));
				phi_tmp = phi_tmp->next();
			}else{
				tmp_tmp = (tmp_tmp->setNext(new HeadElement(*this_tmp)));
				this_tmp = this_tmp->next();
			}
		}

		if(phi_tmp == NULL) phi_tmp = this_tmp;
		for(; phi_tmp != NULL; phi_tmp = phi_tmp->next()){
			tmp_tmp = (tmp_tmp->setNext(new HeadElement(*phi_tmp)));
		}

		return tmp;
	}
	
	const SparseMatrix SparseMatrix::operator*(const SparseMatrix& phi) const{
		SparseMatrix tmp(row_num_, phi.col_num_);
		if(col_num_ != phi.row_num_){
			cerr << "times error!" << endl;
			return tmp;
		}else if(first_head_element_ == NULL){
			return tmp;
		}

		HeadElement* this_tmp = first_head_element_;;
		HeadElement* tmp_tmp = new HeadElement(*this_tmp, phi, this_tmp->row());

		while(tmp_tmp->isNull()){
			delete tmp_tmp;
			this_tmp = this_tmp->next();
			tmp_tmp = new HeadElement(*this_tmp, phi, this_tmp->row());
		}
		tmp.first_head_element_ = tmp_tmp;

		for(this_tmp = this_tmp->next(); this_tmp != NULL; this_tmp = this_tmp->next()){
			tmp_tmp->setNext(new HeadElement(*this_tmp, phi, this_tmp->row()));
			if(tmp_tmp->next()->isNull()){
				delete tmp_tmp->next();
				tmp_tmp->setNext(NULL);
			}else{
				tmp_tmp = tmp_tmp->next();
			}
		}

		return tmp;
	}
	
	/*
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

	/*********************************operators*********************************/
	ostream& operator<<(ostream& os, const SparseMatrix& phi){
		os << phi.str();

		return os;
	}
}
