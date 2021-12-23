#include <iostream>
#include "CMatrix.h"
using namespace std;

int main() {
    int error_matrix1 = I_NO_ERROR;
    int error_matrix2 = I_NO_ERROR;
    int error_matrix3 = I_NO_ERROR;

    int error_vector1 = I_NO_ERROR;
    int error_vector2 = I_NO_ERROR;

    int error_test_add = I_NO_ERROR;
    int error_test_subtract = I_NO_ERROR;
    int error_test_multiply_by_matrix = I_NO_ERROR;
    int error_test_multiply_by_number = I_NO_ERROR;
    int error_test_scalar = I_NO_ERROR;
    int error_test_transpose = I_NO_ERROR;
    int error_test_read_int = I_NO_ERROR;
    int error_test_read_double = I_NO_ERROR;
    
    CMatrix<int> empty = CMatrix<int>();
    cout << endl;

    cout << "---------- MATRIX 1 (FILL SQUARE MATRIX TEST)" << endl;
    CMatrix<int> matrix1 = CMatrix<int>(5, 5, &error_matrix1);
    matrix1.vFillMatrix(&error_matrix1);
    cout << "Operation successful? " << (error_matrix1 == I_NO_ERROR) << endl;
    matrix1.vPrintMatrix();
    cout << endl;

    cout << "---------- MATRIX 2 (FILL IDENTITY MATRIX TEST)" << endl;
    CMatrix<int> matrix2 = CMatrix<int>(5, 5, &error_matrix2);
    matrix2.vFillIdentityMatrix(&error_matrix2);
    cout << "Operation successful? " << (error_matrix2 == I_NO_ERROR) << endl;
    matrix2.vPrintMatrix();
    cout << endl;

    cout << "---------- MATRIX 3 (FILL RECTANGULAR MATRIX TEST)" << endl;
    CMatrix<int> matrix3 = CMatrix<int>(3, 5, &error_matrix3);
    matrix3.vFillMatrix(&error_matrix3);
    cout << "Operation successful? " << (error_matrix3 == I_NO_ERROR) << endl;
    matrix3.vPrintMatrix();
    cout << endl;

    cout << "---------- CREATE VECTOR FROM ROW TEST" << endl;
    CMatrix<int> vector1 = matrix1.cCreateVectorFromRow(1, &error_vector1);
    cout << "Operation successful? " << (error_vector1 == I_NO_ERROR) << endl;
    vector1.vPrintMatrix();
    cout << endl;

    cout << "---------- CREATE VECTOR FROM COLUMN TEST" << endl;
    CMatrix<int> vector2 = matrix1.cCreateVectorFromColumn(1, &error_vector2);
    cout << "Operation successful? " << (error_vector2 == I_NO_ERROR) << endl;
    vector2.vPrintMatrix();
    cout << endl;

    cout << "---------- ADD METHOD / + OPERATOR TEST" << endl;
    try {
        CMatrix<int> test_add = matrix1 + matrix2;
        test_add.vPrintMatrix();
    }
    catch (invalid_argument& ia_exception) {
        cout << "Operation unsuccessful!" << endl;
    }
    /*CMatrix<int> test_add = matrix1.cAdd(matrix2, &error_test_add);
    cout << "Operation successful? " << (error_test_add == I_NO_ERROR) << endl;*/
    cout << endl;

    cout << "---------- SUBTRACT METHOD / - OPERATOR TEST" << endl;
    try {
        CMatrix<int> test_subtract = matrix1 - matrix2;
        test_subtract.vPrintMatrix();
    }
    catch (invalid_argument& ia_exception) {
        cout << "Operation unsuccessful!" << endl;
    }
    /*CMatrix<int> test_subtract = matrix1.cSubtract(matrix2, &error_test_subtract);
    cout << "Operation successful? " << (error_test_subtract == I_NO_ERROR) << endl;*/
    cout << endl;

    cout << "---------- MULTIPLY BY MATRIX METHOD / * OPERATOR TEST" << endl;
    try {
        CMatrix<int> test_multiply_by_matrix = vector1 * matrix1;
        test_multiply_by_matrix.vPrintMatrix();
    }
    catch (invalid_argument& ia_exception) {
        cout << "Operation unsuccessful!" << endl;
    }
    /*CMatrix<int> test_multiply_by_matrix = vector1.cMultiply(matrix1, &error_test_multiply_by_matrix);
    cout << "Operation successful? " << (error_test_multiply_by_matrix == I_NO_ERROR) << endl;*/
    cout << endl;

    cout << "---------- MULTIPLY BY NUMBER METHOD / * OPERATOR TEST" << endl;
    try {
        CMatrix<int> test_multiply_by_number = matrix1 * (-1);
        test_multiply_by_number.vPrintMatrix();
    }
    catch (invalid_argument& ia_exception) {
        cout << "Operation unsuccessful!" << endl;
    }
    /*CMatrix<int> test_multiply_by_number = matrix1.cMultiply(-1, &error_test_multiply_by_number);
    cout << "Operation successful? " << (error_test_multiply_by_number == I_NO_ERROR) << endl;*/
    cout << endl;

    cout << "---------- GET SCALAR PRODUCT METHOD / % OPERATOR TEST" << endl;
    try {
        int test_scalar = matrix1 % matrix2;
        cout << "Scalar product: " << test_scalar << endl;
    }
    catch (invalid_argument& ia_exception) {
        cout << "Operation unsuccessful!" << endl;
    }
    /*int test_scalar = matrix1.tGetScalarProduct(matrix2, &error_test_scalar);
    cout << "Operation successful? " << (error_test_scalar == I_NO_ERROR) << endl;*/
    cout << endl;

    cout << "---------- TRANSPOSE METHOD / ~ OPERATOR TEST" << endl;
    try {
        CMatrix<int> test_transpose = ~matrix3;
        test_transpose.vPrintMatrix();
    }
    catch (invalid_argument& ia_exception) {
        cout << "Operation unsuccessful!" << endl;
    }
    /*CMatrix<int> test_transpose = matrix3.cTranspose(&error_test_transpose);
    cout << "Operation successful? " << (error_test_transpose == I_NO_ERROR) << endl;*/
    cout << endl;

    cout << "---------- READ FROM FILE (INT) TEST" << endl;
    CMatrix<int> test_read_int = CMatrix<int>();
    test_read_int.vReadFromFile("int.txt", &error_test_read_int);
    //test_read_int.vReadFromFile("int_errors.txt", &error_test_read_int);
    cout << "Operation successful? " << (error_test_read_int == I_NO_ERROR || error_test_read_int == I_ERROR_MATRIX_CREATED_INVALID_ELEMENTS_REPLACED) << endl;
    cout << "Error code value: " << error_test_read_int << endl;
    test_read_int.vPrintMatrix();
    cout << endl;

    cout << "---------- READ FROM FILE (DOUBLE) TEST" << endl;
    CMatrix<double> test_read_double = CMatrix<double>();
    test_read_double.vReadFromFile("double.txt", &error_test_read_double);
    //test_read_double.vReadFromFile("double_errors.txt", &error_test_read_double);
    cout << "Operation successful? " << (error_test_read_double == I_NO_ERROR || error_test_read_double == I_ERROR_MATRIX_CREATED_INVALID_ELEMENTS_REPLACED) << endl;
    cout << "Error code value: " << error_test_read_double << endl;
    test_read_double.vPrintMatrix();
    cout << endl;
}
