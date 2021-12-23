#pragma once
#include <fstream>
#include <vector>
#include <sstream>
#include "ErrorCodes.h"
#include "ConstChar.h"
using namespace std;

template <typename T>
class CMatrix {
public:
	//konstruktory i destruktor
	CMatrix();																		//konstruktor domyslny - tworzy macierz 0x0
	CMatrix(int iSizeX, int iSizeY, int* piErrorCode);								//konstruktor z dwoma parametrami
	CMatrix(const CMatrix<T>& cOther);												//konstruktor kopiujacy
	CMatrix(CMatrix<T>&& cOther);													//konstruktor przenoszacy
	~CMatrix();																		//destruktor

	//metody wymagane
	CMatrix<T> cAdd(const CMatrix<T>& cOther, int* piErrorCode);					//dodawanie macierzy
	CMatrix<T> cSubtract(const CMatrix<T>& cOther, int* piErrorCode);				//odejmowanie macierzy
	CMatrix<T> cMultiply(const CMatrix<T>& cOther, int* piErrorCode);				//mnozenie macierzy przez macierz
	CMatrix<T> cMultiply(const T tVal, int* piErrorCode);							//mnozenie macierzy przez liczbe
	
	T tGetScalarProduct(const CMatrix<T>& cOther, int* piErrorCode);				//wyliczanie iloczynu skalarnego macierzy
	CMatrix<T> cTranspose(int* piErrorCode);										//transponowanie macierzy
	
	void vReadFromFile(string sFileName, int* piErrorCode);							//czytanie macierzy z pliku
	
	void vCreateNewMatrix(int iSizeX, int iSizeY, int* piErrorCode);				//tworzenie macierzy o zadanych wymiarach
	void vResize(int iSizeX, int iSizeY, int* piErrorCode);							//zmiana wymiarow macierzy

	void vSetElement(int iIndexX, int iIndexY, const T tVal, int* piErrorCode);		//zmiana podanego elementu macierzy
	T tGetElement(int iIndexX, int iIndexY, int* piErrorCode);						//odczyt podanego elementu macierzy

	CMatrix<T> cCreateVectorFromRow(int iIndexX, int* piErrorCode);					//tworzenie wektora z podanego wiersza
	CMatrix<T> cCreateVectorFromColumn(int iIndexY, int* piErrorCode);				//tworzenie wektora z podanej kolumny

	void vFillIdentityMatrix(int* piErrorCode);										//wypelnianie macierzy kwadratowych wartosciami macierzy jednostkowej

	//operatory
	CMatrix<T> operator+(const CMatrix<T>& cOther);									//operator dodawania
	CMatrix<T> operator-(const CMatrix<T>& cOther);									//operator odejmowania
	CMatrix<T> operator*(const CMatrix<T>& cOther);									//operator mnozenia przez macierz
	CMatrix<T> operator*(const T tVal);												//operator mnozenia przez liczbe
	
	T operator%(const CMatrix<T>& cOther);											//operator wyliczania iloczynu skalarnego
	CMatrix<T> operator~();															//operator transponowania
	
	void operator()(int iIndexX, int iIndexY, const T tVal);						//operator zmiany elementu
	T operator()(int iIndexX, int iIndexY);											//operator odczytu elementu

	void operator=(const CMatrix<T>& cOther);										//operator = kopiujacy
	void operator=(CMatrix<T>&& cOther);											//operator = przenoszacy

	//metody pomocnicze
	void vFillMatrix(int* piErrorCode);												//wypelnianie macierzy wartosciami 10, 11, 12, 13, ...
	void vPrintMatrix();															//wyswietlanie macierzy

private:
	//konstruktor i metody prywatne
	CMatrix(T** ptMatrix, int iSizeX, int iSizeY);									//prywatny konstruktor z trzema parametrami - poprawnosc wymiarow sprawdza metoda wywolujaca

	T** ptAllocateNewMatrix(int iSizeX, int iSizeY);								//alokowanie macierzy o zadanych wymiarach - poprawnosc wymiarow sprawdza metoda wywolujaca

	void vCreate(int iSizeX, int iSixeY, int* piErrorCode);							//tworzenie obiektu na podstawie podanych wymiarow
	void vCopy(const CMatrix<T>& cOther);											//tworzenie obiektu na podstawie innego obiektu - kopiowanie
	void vMove(CMatrix<T>& cOther);													//tworzenie obiektu na podstawie "przenoszenia" innego obiektu - move semantics
	void vDelete();																	//usuwanie obiektu

	//pola klasy
	T** pt_matrix;
	int i_size_x, i_size_y;
};

//////////////////////////////////////////////////////////////////////////////konstruktory i destruktor//////////////////////////////////////////////////////////////////////////////

template <typename T>
CMatrix<T>::CMatrix() {
	//cout << "     Default constr: creating matrix " << 0 << "x" << 0 << endl;
	int i_error_code = I_NO_ERROR;
	vCreate(0, 0, &i_error_code);
}

template <typename T>
CMatrix<T>::CMatrix(int iSizeX, int iSizeY, int* piErrorCode) {
	//cout << "     Parametrized constr: creating matrix " << iSizeX << "x" << iSizeY << endl;
	vCreate(iSizeX, iSizeY, piErrorCode);
}

template <typename T>
CMatrix<T>::CMatrix(const CMatrix<T>& cOther) {
	//cout << "     Copy constr: creating matrix " << cOther.i_size_x << "x" << cOther.i_size_y << endl;
	vCopy(cOther);
}

template <typename T>
CMatrix<T>::CMatrix(CMatrix<T>&& cOther) {
	//cout << "     Move constr: creating matrix " << cOther.i_size_x << "x" << cOther.i_size_y << endl;
	vMove(cOther);
}

template <typename T>
CMatrix<T>::~CMatrix() {
	//cout << "     Destr: deleting matrix " << i_size_x << "x" << i_size_y << endl;
	vDelete();
}

///////////////////////////////////////////////////////////////////////////////////metody wymagane///////////////////////////////////////////////////////////////////////////////////

template <typename T>
CMatrix<T> CMatrix<T>::cAdd(const CMatrix<T>& cOther, int* piErrorCode) {
	*piErrorCode = I_NO_ERROR;
	if (pt_matrix != nullptr && cOther.pt_matrix != nullptr)
		if (i_size_x == cOther.i_size_x && i_size_y == cOther.i_size_y) {
			T** pt_matrix_tmp = ptAllocateNewMatrix(i_size_x, i_size_y);
			for (int ii = 0; ii < i_size_x; ii++)
				for (int ij = 0; ij < i_size_y; ij++)
					pt_matrix_tmp[ii][ij] = pt_matrix[ii][ij] + cOther.pt_matrix[ii][ij];
			return CMatrix<T>(pt_matrix_tmp, i_size_x, i_size_y);
		}
		else
			*piErrorCode = I_ERROR_METHOD_FAILED_INCORRECT_MATRIX_SIZES;
	else 
		*piErrorCode = I_ERROR_METHOD_FAILED_MATRIX_IS_NULL;
	return CMatrix<T>();
}

template <typename T>
CMatrix<T> CMatrix<T>::cSubtract(const CMatrix<T>& cOther, int* piErrorCode) {
	*piErrorCode = I_NO_ERROR;
	if (pt_matrix != nullptr && cOther.pt_matrix != nullptr)
		if (i_size_x == cOther.i_size_x && i_size_y == cOther.i_size_y) {
			T** pt_matrix_tmp = ptAllocateNewMatrix(i_size_x, i_size_y);
			for (int ii = 0; ii < i_size_x; ii++)
				for (int ij = 0; ij < i_size_y; ij++)
					pt_matrix_tmp[ii][ij] = pt_matrix[ii][ij] - cOther.pt_matrix[ii][ij];
			return CMatrix<T>(pt_matrix_tmp, i_size_x, i_size_y);
		}
		else
			*piErrorCode = I_ERROR_METHOD_FAILED_INCORRECT_MATRIX_SIZES;
	else
		*piErrorCode = I_ERROR_METHOD_FAILED_MATRIX_IS_NULL;
	return CMatrix<T>();
}

template <typename T>
CMatrix<T> CMatrix<T>::cMultiply(const CMatrix<T>& cOther, int* piErrorCode) {
	*piErrorCode = I_NO_ERROR;
	if (pt_matrix != nullptr && cOther.pt_matrix != nullptr)
		if (i_size_y == cOther.i_size_x) {
			T** pt_matrix_tmp = ptAllocateNewMatrix(i_size_x, cOther.i_size_y);
			for (int ii = 0; ii < i_size_x; ii++)
				for (int ij = 0; ij < cOther.i_size_y; ij++) {
					pt_matrix_tmp[ii][ij] = 0;
					for (int ik = 0; ik < i_size_y; ik++)
						pt_matrix_tmp[ii][ij] += pt_matrix[ii][ik] * cOther.pt_matrix[ik][ij];
				}
			return CMatrix<T>(pt_matrix_tmp, i_size_x, cOther.i_size_y);
		}
		else
			*piErrorCode = I_ERROR_METHOD_FAILED_INCORRECT_MATRIX_SIZES;
	else
		*piErrorCode = I_ERROR_METHOD_FAILED_MATRIX_IS_NULL;
	return CMatrix<T>();
}

template <typename T>
CMatrix<T> CMatrix<T>::cMultiply(const T tVal, int* piErrorCode) {
	*piErrorCode = I_NO_ERROR;
	if (pt_matrix != nullptr) {
		T** pt_matrix_tmp = ptAllocateNewMatrix(i_size_x, i_size_y);
		for (int ii = 0; ii < i_size_x; ii++)
			for (int ij = 0; ij < i_size_y; ij++)
				pt_matrix_tmp[ii][ij] = pt_matrix[ii][ij] * tVal;
		return CMatrix<T>(pt_matrix_tmp, i_size_x, i_size_y);
	}
	else {
		*piErrorCode = I_ERROR_METHOD_FAILED_MATRIX_IS_NULL;
		return CMatrix<T>();
	}
}

template <typename T>
T CMatrix<T>::tGetScalarProduct(const CMatrix<T>& cOther, int* piErrorCode) {
	*piErrorCode = I_NO_ERROR;
	if (pt_matrix != nullptr && cOther.pt_matrix != nullptr)
		if (i_size_x == cOther.i_size_x && i_size_y == cOther.i_size_y) {
			T t_result = 0;
			for (int ii = 0; ii < i_size_x; ii++)
				for (int ij = 0; ij < i_size_y; ij++)
					t_result += pt_matrix[ii][ij] * cOther.pt_matrix[ii][ij];
			return t_result;
		}
		else
			*piErrorCode = I_ERROR_METHOD_FAILED_INCORRECT_MATRIX_SIZES;
	else
		*piErrorCode = I_ERROR_METHOD_FAILED_MATRIX_IS_NULL;
	return NAN;
}

template <typename T>
CMatrix<T> CMatrix<T>::cTranspose(int* piErrorCode) {
	*piErrorCode = I_NO_ERROR;
	if (pt_matrix != nullptr) {
		T** pt_matrix_tmp = ptAllocateNewMatrix(i_size_y, i_size_x);
		for (int ii = 0; ii < i_size_y; ii++)
			for (int ij = 0; ij < i_size_x; ij++)
				pt_matrix_tmp[ii][ij] = pt_matrix[ij][ii];
		return CMatrix<T>(pt_matrix_tmp, i_size_y, i_size_x);
	}
	else {
		*piErrorCode = I_ERROR_METHOD_FAILED_MATRIX_IS_NULL;
		return CMatrix<T>();
	}
}

template <typename T>
void CMatrix<T>::vReadFromFile(string sFileName, int* piErrorCode) {
	*piErrorCode = I_NO_ERROR;
	fstream f_file;
	vDelete();
	f_file.open(sFileName);
	if (f_file.is_open()) {
		vector<string> v_all_rows, v_single_row;
		vector<vector<string>> v_matrix;
		string s_row, s_element;

		//wlasciwe czytanie z pliku - czytam caly wiersz na raz, wiec liczba elementow v_all_rows to wymiar x macierzy
		while (getline(f_file, s_row))
			if (s_row.length() != 0)
				v_all_rows.push_back(s_row);
		f_file.close();

		//podzial wierszy na poszczegolne elementy - z wykorzystaniem stringstream, liczba elementow najdluzszego wektora v_single_row to wymiar y macierzy
		for (int ii = 0; ii < v_all_rows.size(); ii++) {
			stringstream s_splitter(v_all_rows.at(ii));
			while (getline(s_splitter, s_element, C_SPACE))
				if (s_element.length() != 0)
					v_single_row.push_back(s_element);
			v_matrix.push_back(v_single_row);
			v_single_row.clear();
		}

		//zamiana przecinkow na kropki aby elementy double zostaly odpowiednio przekonwertowane ze string
		for (int ii = 0; ii < v_matrix.size(); ii++)
			for (int ij = 0; ij < v_matrix.at(ii).size(); ij++) {
				for (int ik = 0; ik < v_matrix.at(ii).at(ij).length(); ik++)
					if (v_matrix.at(ii).at(ij)[ik] == C_COMMA) {
						v_matrix.at(ii).at(ij)[ik] = C_DOT;
						*piErrorCode = I_ERROR_MATRIX_CREATED_INVALID_ELEMENTS_REPLACED;
					}
			}

		//znalezienie liczby wierszy i kolumn
		i_size_x = v_matrix.size();
		i_size_y = 0;
		for (int ii = 0; ii < i_size_x; ii++)
			if (v_matrix.at(ii).size() > i_size_y)
				i_size_y = v_matrix.at(ii).size();

		//wypelnianie macierzy
		pt_matrix = ptAllocateNewMatrix(i_size_x, i_size_y);
		for (int ii = 0; ii < i_size_x; ii++)
			for (int ij = 0; ij < i_size_y; ij++)
				if (ij < v_matrix.at(ii).size())
					try {
						pt_matrix[ii][ij] = (T)stod(v_matrix.at(ii).at(ij));
					}
					catch (invalid_argument& ia_exception) {
						pt_matrix[ii][ij] = 0;
						*piErrorCode = I_ERROR_MATRIX_CREATED_INVALID_ELEMENTS_REPLACED;
					}
				else {
					pt_matrix[ii][ij] = 0;
					*piErrorCode = I_ERROR_MATRIX_CREATED_INVALID_ELEMENTS_REPLACED;
				}
	}
	else {
		i_size_x = 0;
		i_size_y = 0;
		pt_matrix = nullptr;
		*piErrorCode = I_ERROR_MATRIX_NOT_CREATED_CANNOT_OPEN_FILE;
	}
}

template <typename T>
void CMatrix<T>::vCreateNewMatrix(int iSizeX, int iSizeY, int* piErrorCode) {
	vDelete();
	vCreate(iSizeX, iSizeY, piErrorCode);
}

template <typename T>
void CMatrix<T>::vResize(int iSizeX, int iSizeY, int* piErrorCode) {
	*piErrorCode = I_NO_ERROR;
	if (iSizeX > 0 && iSizeY > 0) {
		T** pt_matrix_tmp = ptAllocateNewMatrix(iSizeX, iSizeY);
		for (int ii = 0; ii < iSizeX; ii++)
			for (int ij = 0; ij < iSizeY; ij++)
				if (ii < i_size_x && ij < i_size_y)
					pt_matrix_tmp[ii][ij] = pt_matrix[ii][ij];
				else
					pt_matrix_tmp[ii][ij] = 0;
		vDelete();
		pt_matrix = pt_matrix_tmp;
	}
	else {
		pt_matrix = nullptr;
		if (iSizeX != 0 || iSizeY != 0)
			*piErrorCode = I_ERROR_MATRIX_NOT_CREATED_INCORRECT_SIZE;
	}
	i_size_x = iSizeX;
	i_size_y = iSizeY;
}

template <typename T>
void CMatrix<T>::vSetElement(int iIndexX, int iIndexY, const T tVal, int* piErrorCode) {
	*piErrorCode = I_NO_ERROR;
	if (pt_matrix != nullptr)
		if (iIndexX >= 0 && iIndexX < i_size_x && iIndexY >= 0 && iIndexY < i_size_y)
			pt_matrix[iIndexX][iIndexY] = tVal;
		else
			*piErrorCode = I_ERROR_ELEMENT_NOT_FOUND_INCORRECT_INDEX;
	else
		*piErrorCode = I_ERROR_METHOD_FAILED_MATRIX_IS_NULL;
}

template <typename T>
T CMatrix<T>::tGetElement(int iIndexX, int iIndexY, int* piErrorCode) {
	*piErrorCode = I_NO_ERROR;
	if (pt_matrix != nullptr)
		if (iIndexX >= 0 && iIndexX < i_size_x && iIndexY >= 0 && iIndexY < i_size_y)
			return pt_matrix[iIndexX][iIndexY];
		else
			*piErrorCode = I_ERROR_ELEMENT_NOT_FOUND_INCORRECT_INDEX;
	else
		*piErrorCode = I_ERROR_METHOD_FAILED_MATRIX_IS_NULL;
	return NAN;
}

template <typename T>
CMatrix<T> CMatrix<T>::cCreateVectorFromRow(int iIndexX, int* piErrorCode) {
	*piErrorCode = I_NO_ERROR;
	if (pt_matrix != nullptr)
		if (iIndexX >= 0 && iIndexX < i_size_x) {
			T** pt_matrix_tmp = ptAllocateNewMatrix(1, i_size_y);
			for (int ii = 0; ii < i_size_y; ii++)
				pt_matrix_tmp[0][ii] = pt_matrix[iIndexX][ii];
			return CMatrix<T>(pt_matrix_tmp, 1, i_size_y);
		}
		else
			*piErrorCode = I_ERROR_ELEMENT_NOT_FOUND_INCORRECT_INDEX;
	else
		*piErrorCode = I_ERROR_METHOD_FAILED_MATRIX_IS_NULL;
	return CMatrix<T>();
}

template <typename T>
CMatrix<T> CMatrix<T>::cCreateVectorFromColumn(int iIndexY, int* piErrorCode) {
	*piErrorCode = I_NO_ERROR;
	if (pt_matrix != nullptr)
		if (iIndexY >= 0 && iIndexY < i_size_y) {
			T** pt_matrix_tmp = ptAllocateNewMatrix(i_size_x, 1);
			for (int ii = 0; ii < i_size_x; ii++)
				pt_matrix_tmp[ii][0] = pt_matrix[ii][iIndexY];
			return CMatrix<T>(pt_matrix_tmp, i_size_x, 1);
		}
		else
			*piErrorCode = I_ERROR_ELEMENT_NOT_FOUND_INCORRECT_INDEX;
	else
		*piErrorCode = I_ERROR_METHOD_FAILED_MATRIX_IS_NULL;
	return CMatrix<T>();
}

template <typename T>
void CMatrix<T>::vFillIdentityMatrix(int* piErrorCode) {
	*piErrorCode = I_NO_ERROR;
	if (pt_matrix != nullptr) {
		if (i_size_x == i_size_y) {
			for (int ii = 0; ii < i_size_x; ii++)
				for (int ij = 0; ij < i_size_y; ij++)
					if (ii == ij)
						pt_matrix[ii][ij] = 1;
					else
						pt_matrix[ii][ij] = 0;
		}
		else
			*piErrorCode = I_ERROR_IDENTITY_MATRIX_NOT_FILLED_MATRIX_NOT_SQUARE;
	}
	else
		*piErrorCode = I_ERROR_METHOD_FAILED_MATRIX_IS_NULL;
}

//////////////////////////////////////////////////////////////////////////////////////operatory//////////////////////////////////////////////////////////////////////////////////////

template <typename T>
CMatrix<T> CMatrix<T>::operator+(const CMatrix<T>& cOther) {
	int i_error_code = I_NO_ERROR;
	CMatrix<T> c_result = this->cAdd(cOther, &i_error_code);
	if (i_error_code != I_NO_ERROR)
		throw new invalid_argument(to_string(i_error_code));
	return move(c_result);
}

template <typename T>
CMatrix<T> CMatrix<T>::operator-(const CMatrix<T>& cOther) {
	int i_error_code = I_NO_ERROR;
	CMatrix<T> c_result = this->cSubtract(cOther, &i_error_code);
	if (i_error_code != I_NO_ERROR)
		throw new invalid_argument(to_string(i_error_code));
	return move(c_result);
}

template <typename T>
CMatrix<T> CMatrix<T>::operator*(const CMatrix<T>& cOther) {
	int i_error_code = I_NO_ERROR;
	CMatrix<T> c_result = this->cMultiply(cOther, &i_error_code);
	if (i_error_code != I_NO_ERROR)
		throw new invalid_argument(to_string(i_error_code));
	return move(c_result);
}

template <typename T>
CMatrix<T> CMatrix<T>::operator*(const T tVal) {
	int i_error_code = I_NO_ERROR;
	CMatrix<T> c_result = this->cMultiply(tVal, &i_error_code);
	if (i_error_code != I_NO_ERROR)
		throw new invalid_argument(to_string(i_error_code));
	return move(c_result);
}

template <typename T>
T CMatrix<T>::operator%(const CMatrix<T>& cOther) {
	int i_error_code = I_NO_ERROR;
	T t_result = this->tGetScalarProduct(cOther, &i_error_code);
	if (i_error_code != I_NO_ERROR)
		throw new invalid_argument(to_string(i_error_code));
	return t_result;
}

template <typename T>
CMatrix<T> CMatrix<T>::operator~() {
	int i_error_code = I_NO_ERROR;
	CMatrix<T> c_result = this->cTranspose(&i_error_code);
	if (i_error_code != I_NO_ERROR)
		throw new invalid_argument(to_string(i_error_code));
	return move(c_result);
}

template <typename T>
void CMatrix<T>::operator()(int iIndexX, int iIndexY, const T tVal) {
	int i_error_code = I_NO_ERROR;
	this->vSetElement(iIndexX, iIndexY, tVal, &i_error_code);
	if (i_error_code != I_NO_ERROR)
		throw new invalid_argument(to_string(i_error_code));
}

template <typename T>
T CMatrix<T>::operator()(int iIndexX, int iIndexY) {
	int i_error_code = I_NO_ERROR;
	T t_result = this->tGetElement(iIndexX, iIndexY, &i_error_code);
	if (i_error_code != I_NO_ERROR)
		throw new invalid_argument(to_string(i_error_code));
	return t_result;
}

template <typename T>
void CMatrix<T>::operator=(const CMatrix<T>& cOther) {
	vDelete();
	vCopy(cOther);
}

template <typename T>
void CMatrix<T>::operator=(CMatrix<T>&& cOther) {
	vDelete();
	vMove(cOther);
}

//////////////////////////////////////////////////////////////////////////////////metody pomocnicze//////////////////////////////////////////////////////////////////////////////////

template <typename T>
void CMatrix<T>::vFillMatrix(int* piErrorCode) {
	*piErrorCode = I_NO_ERROR;
	if (pt_matrix != nullptr) {
		int i_val = 10;
		for (int ii = 0; ii < i_size_x; ii++)
			for (int ij = 0; ij < i_size_y; ij++) {
				pt_matrix[ii][ij] = i_val;
				i_val++;
			}
	}
	else
		*piErrorCode = I_ERROR_METHOD_FAILED_MATRIX_IS_NULL;
}

template <typename T>
void CMatrix<T>::vPrintMatrix() {
	if (pt_matrix != nullptr)
		for (int ii = 0; ii < i_size_x; ii++) {
			for (int ij = 0; ij < i_size_y; ij++)
				cout << pt_matrix[ii][ij] << " ";
			cout << endl;
		}
}

////////////////////////////////////////////////////////////////////////////konstruktor i metody prywatne////////////////////////////////////////////////////////////////////////////

template <typename T>
CMatrix<T>::CMatrix(T** ptMatrix, int iSizeX, int iSizeY) {
	//cout << "     Private constr: creating matrix " << iSizeX << "x" << iSizeY << endl;
	i_size_x = iSizeX;
	i_size_y = iSizeY;
	pt_matrix = ptMatrix;
}

template <typename T>
T** CMatrix<T>::ptAllocateNewMatrix(int iSizeX, int iSizeY) {
	T** pt_matrix_tmp;
	pt_matrix_tmp = new T * [iSizeX];
	for (int ii = 0; ii < iSizeX; ii++)
		pt_matrix_tmp[ii] = new T[iSizeY];
	return pt_matrix_tmp;
}

template <typename T>
void CMatrix<T>::vCreate(int iSizeX, int iSizeY, int* piErrorCode) {
	*piErrorCode = I_NO_ERROR;
	i_size_x = iSizeX;
	i_size_y = iSizeY;
	if (i_size_x > 0 && i_size_y > 0)
		pt_matrix = ptAllocateNewMatrix(i_size_x, i_size_y);
	else {
		pt_matrix = nullptr;
		if (i_size_x != 0 || i_size_y != 0)
			*piErrorCode = I_ERROR_MATRIX_NOT_CREATED_INCORRECT_SIZE;
	}
}

template <typename T>
void CMatrix<T>::vCopy(const CMatrix<T>& cOther) {
	i_size_x = cOther.i_size_x;
	i_size_y = cOther.i_size_y;
	if (cOther.pt_matrix != nullptr) {
		pt_matrix = ptAllocateNewMatrix(i_size_x, i_size_y);
		for (int ii = 0; ii < i_size_x; ii++)
			for (int ij = 0; ij < i_size_y; ij++)
				pt_matrix[ii][ij] = cOther.pt_matrix[ii][ij];
	}
	else
		pt_matrix = nullptr;
}

template <typename T>
void CMatrix<T>::vMove(CMatrix<T>& cOther) {
	i_size_x = cOther.i_size_x;
	i_size_y = cOther.i_size_y;
	pt_matrix = cOther.pt_matrix;
	cOther.pt_matrix = nullptr;
}

template <typename T>
void CMatrix<T>::vDelete() {
	if (pt_matrix != nullptr) {
		for (int ii = 0; ii < i_size_x; ii++)
			delete[] pt_matrix[ii];
		delete[] pt_matrix;
	}
}
