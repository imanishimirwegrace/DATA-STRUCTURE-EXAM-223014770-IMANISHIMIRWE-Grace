Task description
Design a program using OOP principles (abstraction and polymorphism) to perform addition and multiplication of matrices using pointer arithmetic. It should:
- Use an abstract class MatrixOp with a pure virtual function apply.
- Define AddOp and MulOp as subclasses of MatrixOp, implementing apply for matrix addition and multiplication respectively.
- Accept matrix dimensions and values from the user.
- Use dynamic memory allocation for matrix creation.
- Display the result or an error if the operation is not valid.
How the Task Was Completed
An abstract class MatrixOp was created to represent a general matrix operation.
 Two derived classes AddOp and MulOp implemented the logic for addition and multiplication. 
Functions were defined to allocate and deallocate matrices, input values, and print matrices.
 The main function collects input, applies both operations using polymorphism, outputs the result, and cleans up dynamically allocated memory.
Annotated C++ Code with comment

#include <iostream>                // Provides std::cin, std::cout for console I/O
using namespace std;               // Saves us from prefixing std:: everywhere

// ==============================<img width="398" alt="Screenshot 4" src="https://github.com/user-attachments/assets/b0ec5e83-207e-4e94-b9b5-f9201e94e9c9" />

// ABSTRACT BASE CLASS: MatrixOp
// ==============================
// This class defines the interface for any matrix operation.
// It uses a pure virtual function so derived classes MUST implement `apply`.
class MatrixOp {
public:
    // apply(): takes pointers to two matrices A and B, their row/col dims
    // Returns a newly-allocated matrix containing the result or NULL on error.
    virtual int** apply(int** A, int** B,
                        int r1, int c1,
                        int r2, int c2) = 0;

    // Virtual destructor ensures derived destructors get called correctly
    virtual ~MatrixOp() {}
};

// ==============================
// DERIVED CLASS: AddOp
// ==============================
// Implements element-wise addition of two matrices.
class AddOp : public MatrixOp {
public:
    int** apply(int** A, int** B,
                int r1, int c1,
                int r2, int c2) override {
        // 1) Dimension check: both matrices must have same rows and columns
        if (r1 != r2 || c1 != c2) {
            cout << "Error: Matrices must be same size for addition.\n";
            return NULL;  // Early exit on dimension mismatch
        }

        // 2) Allocate the result matrix dynamically: array of r1 row pointers
        int** result = new int*[r1];
        for (int i = 0; i < r1; i++) {
            // Each row holds c1 integers
            result[i] = new int[c1];
        }

        // 3) Perform the addition
        // Outer loop over each row
        for (int i = 0; i < r1; i++) {
            // Pointer to the start of row i in A, B, and result
            int* rowA   = *(A + i);
            int* rowB   = *(B + i);
            int* rowRes = *(result + i);

            // Inner loop over each column element
            for (int j = 0; j < c1; j++) {
                // *(rowRes+j) is the cell [i][j] in result
                // Sum the corresponding cells from A and B
                *(rowRes + j) = *(rowA + j) + *(rowB + j);
            }
        }

        // 4) Return pointer to the result matrix
        return result;
    }
};

// ==============================
// DERIVED CLASS: MulOp
// ==============================
// Implements standard matrix multiplication (dot-product style).
class MulOp : public MatrixOp {
public:
    int** apply(int** A, int** B,
                int r1, int c1,
                int r2, int c2) override {
        // 1) Check: number of columns in A must match rows in B
        if (c1 != r2) {
            cout << "Error: Incompatible dimensions for multiplication.\n";
            return NULL;
        }

        // 2) Allocate the result matrix [r1 x c2], zero-initialize it
        int** result = new int*[r1];
        for (int i = 0; i < r1; i++) {
            // The () after c2 ensures all cells start at 0
            result[i] = new int[c2]();
        }

        // 3) Multiply: for each row i of A and column j of B
        for (int i = 0; i < r1; i++) {
            int* rowA   = *(A + i);       // pointer to A[i][0]
            int* rowRes = *(result + i);  // pointer to result[i][0]

            // Iterate over each element k in the row of A / column of B
            for (int k = 0; k < c1; k++) {
                int a_val = *(rowA + k);  // value A[i][k]
                int* rowB = *(B + k);     // pointer to B[k][0]

                // Multiply-add across the entire row of result
                for (int j = 0; j < c2; j++) {
                    // result[i][j] += A[i][k] * B[k][j]
                    *(rowRes + j) += a_val * *(rowB + j);
                }
            }
        }

        return result;  // Return the computed product
    }
};

// ==============================
// UTILITY FUNCTION: allocateMatrix
// ==============================
// Allocates a 2D array of size [rows x cols] and returns its pointer.
int** allocateMatrix(int rows, int cols) {
    int** matrix = new int*[rows];      // Array of row pointers
    for (int i = 0; i < rows; i++) {
        matrix[i] = new int[cols];      // Each row is an array of ints
    }
    return matrix;
}

// ==============================
// UTILITY FUNCTION: deallocateMatrix
// ==============================
// Frees memory allocated for a 2D array.
void deallocateMatrix(int** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        delete[] matrix[i];             // Free each row
    }
    delete[] matrix;                    // Free array of pointers
}

// ==============================
// UTILITY FUNCTION: inputMatrix
// ==============================
// Reads values from the user into a 2D array.
void inputMatrix(int** matrix,
                 int rows, int cols,
                 char name) {
    cout << "Enter values for matrix " << name
         << " (" << rows << "x" << cols << "):\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // Prompt e.g. "A[0][1] = "
            cout << name << "[" << i << "][" << j << "] = ";
            cin >> matrix[i][j];
        }
    }
}

// ==============================
// UTILITY FUNCTION: printMatrix
// ==============================
// Displays the contents of a 2D array in a tabular format.
void printMatrix(int** matrix,
                 int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // Print each value followed by a tab
            cout << matrix[i][j] << "\t";
        }
        cout << "\n";  // Newline after each row
    }
}

// ==============================
// MAIN ENTRY POINT
// ==============================
int main() {
    int r1, c1, r2, c2;

    // ------ Get matrix dimensions from user ------
    cout << "Enter dimensions for matrix A (rows columns): ";
    cin >> r1 >> c1;
    cout << "Enter dimensions for matrix B (rows columns): ";
    cin >> r2 >> c2;

    // ------ Allocate matrices A and B dynamically ------
    int** A = allocateMatrix(r1, c1);
    int** B = allocateMatrix(r2, c2);

    // ------ Input values into A and B ------
    inputMatrix(A, r1, c1, 'A');
    inputMatrix(B, r2, c2, 'B');

    // ------ Create polymorphic operations array ------
    MatrixOp* ops[2];
    ops[0] = new AddOp();              // Addition operation
    ops[1] = new MulOp();              // Multiplication operation

    // ------ Perform addition and multiplication ------
    for (int i = 0; i < 2; i++) {
        cout << "\n--- "
             << (i == 0 ? "Addition" : "Multiplication")
             << " Result ---\n";

        // Dynamically dispatch to correct apply()
        int** result = ops[i]->apply(A, B,
                                     r1, c1,
                                     r2, c2);

        if (result) {
            // Determine result dimensions
            int rr = r1;
            int cc = (i == 0 ? c1 : c2);

            // Print and then deallocate the result
            printMatrix(result, rr, cc);
            deallocateMatrix(result, rr);
        } else {
            cout << "Operation failed due to invalid dimensions.\n";
        }
    }

    // ------ Clean up all allocated memory ------
    deallocateMatrix(A, r1);
    deallocateMatrix(B, r2);
    delete ops[0];
    delete ops[1];

    cout << "\nProgram complete.\n";
    return 0;
}


<img width="286" alt="Screenshot 3" src="https://github.com/user-attachments/assets/0ea385df-49e4-45a9-80be-238e1ca7c233" />
<img width="365" alt="Screenshot 2" src="https://github.com/user-attachments/assets/d8bf2315-28a7-42da-b7cc-e3848e691611" />
<img width="361" alt="Screenshot 1" src="https://github.com/user-attachments/assets/d21b6e8e-fed2-4ed7-abae-d3940ddfc023" />


