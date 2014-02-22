namespace lsm{
	//HeadElement
	template<class TYPE>
	SparseMatrix<TYPE>::
	HeadElement::HeadElement(const HeadElement& origin){
		row_ = origin.row_;
		next_ = NULL;

		Element* origin_tmp = origin.first_element_->next();
		Element* this_tmp = first_element_ = new Element(*origin.first_element_);
		for(; origin_tmp != NULL; origin_tmp = origin_tmp->next()){
			this_tmp = (this_tmp->setNext(new Element(*origin_tmp)));
		}
	}

	template<class TYPE>
	SparseMatrix<TYPE>::
	HeadElement::HeadElement(const HeadElement& phi, const HeadElement& psi, const int row){
		row_ = row;
		next_ = NULL;

		Element* phi_tmp = phi.first_element_;
		Element* psi_tmp = psi.first_element_;
		Element* this_tmp;

		while(phi_tmp != NULL || psi_tmp != NULL){
			if(phi_tmp->col() == psi_tmp->col()){
				if(phi_tmp->val() + psi_tmp->val() == TYPE(0)){
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
				if(phi_tmp->val() + psi_tmp->val() == TYPE(0)){
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

	template<class TYPE>
	SparseMatrix<TYPE>::
	HeadElement::HeadElement(const HeadElement& phi, const SparseMatrix& psi, const int row){
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
	
	template<class TYPE>
	SparseMatrix<TYPE>::
	HeadElement::HeadElement(const int row, HeadElement* next, Element* first_element){
		row_ = row;
		next_ = next;
		first_element_ = first_element;
	}

	template<class TYPE>
	SparseMatrix<TYPE>::
	HeadElement::~HeadElement(){
		Element *tmp1, *tmp2;
		for(tmp1 = first_element_; tmp1 != NULL; tmp1 = tmp2){
			tmp2 = tmp1->next();
			delete tmp1;
		}
	}

	template<class TYPE>
	TYPE
	SparseMatrix<TYPE>::
	HeadElement::val(const int col) const{
		Element *front, *back;
		for(front = back = first_element_;
				(front != NULL && front->col() <= col);
				front = front->next()) back = front;
		if(back->col() != col) return TYPE(0);

		return back->val();
	}

	template<class TYPE>
	std::string
	SparseMatrix<TYPE>::
	HeadElement::str() const{
		Element* tmp;
		std::ostringstream ostr;
		for(tmp = first_element_; tmp != NULL; tmp = tmp->next()){
			ostr << tmp->col() << ":" << tmp->val() << " ";
		}
		return ostr.str();
	}

	template<class TYPE>
	class SparseMatrix<TYPE>::HeadElement::Element&
	SparseMatrix<TYPE>::
	HeadElement::operator[](const int col){
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

	template<class TYPE>
	class SparseMatrix<TYPE>::HeadElement::HeadElement&
	SparseMatrix<TYPE>::
	HeadElement::operator+=(const HeadElement& phi){
		Element* phi_tmp = phi.first_element_;
		Element* this_front = first_element_;
		Element* this_back = first_element_;

		while(phi_tmp != NULL){
			if(this_front == NULL){
				this_back = (this_back->setNext(new Element(*phi_tmp)));
				phi_tmp = phi_tmp->next();
			}else if(phi_tmp->col() == this_front->col()){
				*this_front += phi_tmp->val();
				this_back = this_front;
				this_front = this_front->next();
				phi_tmp = phi_tmp->next();
			}else if(phi_tmp->col() < this_front->col()){
				this_back = (this_back->setNext(new Element(phi_tmp->col(), this_front, phi_tmp->val())));
				phi_tmp = phi_tmp->next();
			}else{
				for(; this_front != NULL && this_front->col() < phi_tmp->col(); this_front = this_front->next()){
					this_back = this_front;
				}
			}
		}

		return *this;
	}
}
