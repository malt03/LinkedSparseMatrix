namespace lsm{
	template<class TYPE>
	class SparseMatrix{

		class HeadElement{

			class Element{
				TYPE val_;
				int col_;
				Element* next_;
			public:
				Element(){}
				Element(const Element& origin);
				Element(const int col, const TYPE& val);
				Element(const int col, Element* next = NULL, const TYPE val = TYPE(0));

				TYPE val() const{return val_;}
				int col() const{return col_;}
				Element* next() const{return next_;}
				Element* setNext(Element* next){return (next_ = next);}

				Element& operator=(const TYPE val);
				Element& operator+=(const TYPE val);
				Element& operator-=(const TYPE val);
			};

			int row_;
			HeadElement* next_;
			Element* first_element_;
		public:
			HeadElement(const HeadElement& origin);
			HeadElement(const HeadElement& origin, HeadElement* next);
			HeadElement(const HeadElement& phi, const HeadElement& psi, const int row);
			HeadElement(const HeadElement& phi, const SparseMatrix& psi, const int row);
			HeadElement(const int row, HeadElement* next = NULL, Element* first_element = NULL);
			~HeadElement();

			int row() const{return row_;}
			HeadElement* next() const{return next_;}
			HeadElement* setNext(HeadElement* next){return (next_ = next);}
			bool isNull() const{return (first_element_ == NULL);}
			std::string str() const;

			TYPE val(const int col) const;
			Element& operator[](int col);

			const HeadElement operator-() const;
			const HeadElement operator*(const TYPE& n) const;
			HeadElement& operator+=(const HeadElement& phi);
			HeadElement& operator-=(const HeadElement& phi);
			HeadElement& operator*=(const TYPE& n);
			const bool operator==(const HeadElement& phi) const;
		};
	
		TYPE default_num_;
		int col_num_, row_num_;
		HeadElement* first_head_element_;

		HeadElement* getHeadElement(const int num);
	public:
		SparseMatrix(){};
		SparseMatrix(const SparseMatrix& origin);
		SparseMatrix(const int row_num, const int col_num, const TYPE default_num = TYPE(0));
		~SparseMatrix();

		std::string str() const;

		TYPE val(const int row, const int col) const;
		HeadElement& operator[](const int row);

		const SparseMatrix operator-() const;
		SparseMatrix& operator=(const SparseMatrix& phi);
		const SparseMatrix operator+(const SparseMatrix& phi) const;
		const SparseMatrix operator-(const SparseMatrix& phi) const;
		const SparseMatrix operator*(const SparseMatrix& phi) const;
		const SparseMatrix operator*(const TYPE& n) const;
		SparseMatrix& operator+=(const SparseMatrix& phi);
		SparseMatrix& operator-=(const SparseMatrix& phi);
		SparseMatrix& operator*=(const SparseMatrix& phi);
		SparseMatrix& operator*=(const TYPE& phi);
		const bool operator==(const SparseMatrix& phi) const;
		const bool operator!=(const SparseMatrix& phi) const;
		const SparseMatrix tensor(SparseMatrix& phi) const;
		SparseMatrix& tensorEqual(SparseMatrix& phi);
	
		TYPE getDefaultNum() const{return default_num_;}
	};
}
