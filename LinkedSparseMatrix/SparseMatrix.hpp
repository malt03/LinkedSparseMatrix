namespace lsm{
	template<class TYPE>
	SparseMatrix<TYPE>::SparseMatrix(const SparseMatrix& origin){
		row_num_ = origin.row_num_;
		col_num_ = origin.col_num_;
		default_num_ = origin.default_num_;

		HeadElement* origin_tmp = origin.first_head_element_->next();
		HeadElement* this_tmp = first_head_element_ = new HeadElement(*origin.first_head_element_);
		for(; origin_tmp != NULL; origin_tmp = origin_tmp->next()){
			this_tmp = (this_tmp->setNext(new HeadElement(*origin_tmp)));
		}
	}
	
	template<class TYPE>
	SparseMatrix<TYPE>::SparseMatrix(const int row_num, const int col_num, const TYPE default_num){
		row_num_ = row_num;
		col_num_ = col_num;
		default_num_ = default_num;
		first_head_element_ = NULL;
	}

	template<class TYPE>
	SparseMatrix<TYPE>::~SparseMatrix(){
		HeadElement *tmp1, *tmp2;
		for(tmp1 = first_head_element_; tmp1 != NULL; tmp1 = tmp2){
			tmp2 = tmp1->next();
			delete tmp1;
		}
	}

	template<class TYPE>
	std::string
	SparseMatrix<TYPE>::str() const{
		HeadElement* tmp;
		std::ostringstream ostr;

		for(tmp = first_head_element_; tmp != NULL; tmp = tmp->next()){
			ostr << tmp->row() << ":	";
			ostr << tmp->str();
			ostr << "\n";
		}

		return ostr.str();
	}

	template<class TYPE>
	TYPE
	SparseMatrix<TYPE>::val(const int row, const int col) const{
		HeadElement *hfront, *hback;
		for(hfront = hback = first_head_element_;
				(hfront != NULL && hfront->row() <= row);
				hfront = hfront->next()) hback = hfront;
		if(hback->row() != row) return TYPE(0);

		return hback->val(col);
	}

	template<class TYPE>
	class SparseMatrix<TYPE>::HeadElement&
	SparseMatrix<TYPE>::operator[](const int row){
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
	
	template<class TYPE>
	SparseMatrix<TYPE>&
	SparseMatrix<TYPE>::operator=(const SparseMatrix& phi){
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

	template<class TYPE>
	const SparseMatrix<TYPE>
	SparseMatrix<TYPE>::operator+(const SparseMatrix& phi) const{
		SparseMatrix tmp(row_num_, col_num_, default_num_ + phi.default_num_);
		if(row_num_ != phi.row_num_ || col_num_ != phi.col_num_){
			std::cerr << "add error!" << std::endl;
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

	template<class TYPE>
	const SparseMatrix<TYPE>
	SparseMatrix<TYPE>::operator*(const SparseMatrix& phi) const{
		SparseMatrix tmp(row_num_, phi.col_num_);
		if(col_num_ != phi.row_num_){
			std::cerr << "times error!" << std::endl;
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

	template<class TYPE>
	SparseMatrix<TYPE>&
	SparseMatrix<TYPE>::operator+=(const SparseMatrix& phi){
		HeadElement* phi_tmp = phi.first_head_element_;
		HeadElement* this_front = first_head_element_;
		HeadElement* this_back = first_head_element_;

		while(phi_tmp != NULL){
			if(this_front == NULL){
				this_back = (this_back->setNext(new HeadElement(*phi_tmp)));
				phi_tmp = phi_tmp->next();
			}else if(phi_tmp->row() == this_front->row()){
				*this_front += *phi_tmp;
				this_back = this_front;
				this_front = this_front->next();
				phi_tmp = phi_tmp->next();
			}else if(phi_tmp->row() < this_front->row()){
				this_back = (this_back->setNext(new HeadElement(phi_tmp->col(), this_front, phi_tmp->val())));
				phi_tmp = phi_tmp->next();
			}else{
				for(; this_front != NULL && this_front->col() < phi_tmp->col(); this_front = this_front->next()){
					this_back = this_front;
				}
			}
		}
	}
	
	/*
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
}
