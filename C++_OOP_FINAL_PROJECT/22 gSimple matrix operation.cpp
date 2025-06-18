#include <iostream>
using namespace std;

// ==============================
// ABSTRACT CLASS: MatrixOp
// ==============================
class MatrixOp {
public:
    virtual int** apply(int** A, int** B, int r1, int c1, int r2, int c2) = 0;
    virtual ~MatrixOp() {}
};

// ==============================
// DERIVED CLASS: AddOp
// ==============================
class AddOp : public MatrixOp {
public:
    int** apply(int** A, int** B, int r1, int c1, int r2, int c2) {
        if (r1 != r2 || c1 != c2) {
            cout << "Error: Matrices must have same dimensions for addition!\n";
            return NULL;
        }
        
        // Allocate result matrix
        int** result = new int*[r1];
        for (int i = 0; i < r1; i++) {
            result[i] = new int[c1];
        }
        
        // Perform addition using pointer arithmetic
        for (int i = 0; i < r1; i++) {
            int* rowA = *(A + i);
            int* rowB = *(B + i);
            int* rowRes = *(result + i);
            
            for (int j = 0; j < c1; j++) {
                *(rowRes + j) = *(rowA + j) + *(rowB + j);
            }
        }
        return result;
    }
};

// ==============================
// DERIVED CLASS: MulOp
// ==============================
class MulOp : public MatrixOp {
public:
    int** apply(int** A, int** B, int r1, int c1, int r2, int c2) {
        if (c1 != r2) {
            cout << "Error: Columns of first matrix must match rows of second matrix!\n";
            return NULL;
        }
        
        // Allocate result matrix
        int** result = new int*[r1];
        for (int i = 0; i < r1; i++) {
            result[i] = new int[c2](); // Initialize to zero
        }
        
        // Perform multiplication using pointer arithmetic
        for (int i = 0; i < r1; i++) {
            int* rowA = *(A + i);
            int* rowRes = *(result + i);
            
            for (int k = 0; k < c1; k++) {
                int* rowB = *(B + k);
                int a_val = *(rowA + k);
                
                for (int j = 0; j < c2; j++) {
                    *(rowRes + j) += a_val * *(rowB + j);
                }
            }
        }
        return result;
    }
};

// ==============================
// FUNCTION: allocateMatrix
// ==============================
int** allocateMatrix(int rows, int cols) {
    int** matrix = new int*[rows];
    for (int i = 0; i < rows; i++) {
        matrix[i] = new int[cols];
    }
    return matrix;
}

// ==============================
// FUNCTION: deallocateMatrix
// ==============================
void deallocateMatrix(int** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

// ==============================
// FUNCTION: inputMatrix
// ==============================
void inputMatrix(int** matrix, int rows, int cols, char name) {
    cout << "Enter values for matrix " << name << " (" << rows << "x" << cols << "):\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << name << "[" << i << "][" << j << "] = ";
            cin >> matrix[i][j];
        }
    }
}

// ==============================
// FUNCTION: printMatrix
// ==============================
void printMatrix(int** matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << matrix[i][j] << "\t";
        }
        cout << "\n";
    }
}

// ==============================
// MAIN FUNCTION
// ==============================
int main() {
    int r1, c1, r2, c2;
    
    // ---------------------------
    // GET MATRIX DIMENSIONS
    // ---------------------------
    cout << "MATRIX DIMENSION INPUT\n";
    cout << "----------------------\n";
    cout << "Enter dimensions for matrix A (rows columns): ";
    cin >> r1 >> c1;
    cout << "Enter dimensions for matrix B (rows columns): ";
    cin >> r2 >> c2;
    
    // ---------------------------
    // ALLOCATE MATRICES
    // ---------------------------
    int** A = allocateMatrix(r1, c1);
    int** B = allocateMatrix(r2, c2);
    
    // ---------------------------
    // INPUT MATRIX VALUES
    // ---------------------------
    cout << "\nMATRIX DATA ENTRY\n";
    cout << "-----------------\n";
    inputMatrix(A, r1, c1, 'A');
    inputMatrix(B, r2, c2, 'B');
    
    // ---------------------------
    // SET UP OPERATIONS
    // ---------------------------
    MatrixOp** ops = new MatrixOp*[2];
    ops[0] = new AddOp();
    ops[1] = new MulOp();
    
    // ---------------------------
    // PERFORM MATRIX OPERATIONS
    // ---------------------------
    cout << "\nMATRIX OPERATION RESULTS\n";
    cout << "------------------------\n";
    for (int i = 0; i < 2; i++) {
        cout << "\nOperation " << (i == 0 ? "Addition" : "Multiplication") << ":\n";
        cout << "=======================\n";
        
        int** result = ops[i]->apply(A, B, r1, c1, r2, c2);
        if (result != NULL) {
            // Determine result dimensions
            int res_rows = (i == 0) ? r1 : r1;
            int res_cols = (i == 0) ? c1 : c2;
            
            printMatrix(result, res_rows, res_cols);
            deallocateMatrix(result, res_rows);
        } else {
            cout << "Operation could not be performed.\n";
        }
    }
    
    // ---------------------------
    // CLEANUP RESOURCES
    // ---------------------------
    deallocateMatrix(A, r1);
    deallocateMatrix(B, r2);
    
    for (int i = 0; i < 2; i++) {
        delete ops[i];
    }
    delete[] ops;
    
    cout << "\nPROGRAM EXECUTION COMPLETE\n";
    return 0;
}

