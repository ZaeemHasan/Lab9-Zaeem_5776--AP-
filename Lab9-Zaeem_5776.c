#include <stdio.h>
#include <string.h>

typedef void(*cal_Product)(struct Matrix_ *A, struct Matrix_ *B);			// Function prototype for Calculating Product
typedef void(*cal_Sum)(struct Matrix_ *A, struct Matrix_ *B);				// Function prototype for Calculating Sum
typedef void(*computeL1Norm)(struct Matrix_ *A);								// Function prototype for Calculating L1 form
typedef void(*print)(struct Matrix_ *A);										// For Printing matrices and Vectors
typedef void(*ini_Matrix)(struct Matrix_ *A, int rows, int columns);		// Matrix_ class constructor
typedef void(*ini_Vector)(struct Vector_ *A, int rows);					// Vector_ class Constructor

typedef struct Matrix_{							//Matrix_ class
	int rows;
	int columns;
	int **array;
	
	cal_Product compute;
	cal_Sum computeSm;

	computeL1Norm *vtable;
	
	print printthis; 
	ini_Matrix init;
}Matrix;

void Matrix_Product(struct Matrix_ *A, struct Matrix_ *B)
{
	int sum = 0, multiply[20][20];

	if (A->columns != B->rows)
		printf("Matrices with specified orders can't be multiplied.\n");
	else {
		for (int c = 0; c < A->rows; c++) {
			for (int d = 0; d < B->columns; d++) {
				for (int k = 0; k < B->rows; k++) {
					sum = sum + (A->array[c][k]) * (B->array[k][d]);
				}
				multiply[c][d] = sum;
				sum = 0;
			}
		}

		for (int c = 0; c < A->rows; c++) {
			for (int d = 0; d < B->columns; d++){
				printf("%d\t", multiply[c][d]);
			}
			printf("\n");
		}
	}
}

void Matrix_Sum(struct Matrix_ *A, struct Matrix_ *B){
	int sum = 0, sumAns[20][20];

	if ((A->rows != B->rows) || (A->columns != B->columns))
		printf("Matrices have unequal rows or columns cant be added.\n");
	else{
		for (int c = 0; c < A->rows; c++) {
			for (int d = 0; d < A->columns; d++) {
				sumAns[c][d] = A->array[c][d] + B->array[c][d];
			}
		}

		for (int c = 0; c < A->rows; c++) {
			for (int d = 0; d < A->columns; d++){
				printf("%d\t", sumAns[c][d]);
			}
			printf("\n");
		}
	}
}

void Matrix_L1Norm(struct Matrix_ *A){
	int L1Ans[20];

	if ((A->rows == 0) || (A->columns == 0))
		printf("Matrix having row or column 0 cant be calculated.\n");
	else{
		for (int c = 0; c < A->columns; c++) {
			L1Ans[c] = 0;
			for (int d = 0; d < A->rows; d++) {
				L1Ans[c] += A->array[d][c];
			}
		}

		int largest = 0;

		for (int i = 0; i < A->columns; i++){
			if (L1Ans[i] > largest){
				largest = L1Ans[i];
			}
		}

		printf("%d\n", largest);
	}
}

void printMatrix(struct Matrix_ *A){
	for (int i = 0; i < A->rows; i++){
		for (int j = 0; j < A->columns; j++){
			printf("%d\t", A->array[i][j]);
		}
		printf("\n");
	}
}

void initMatrix(struct Matrix_ *A, int rows, int columns){	// Matrix_ class Constructor Definition
	Matrix temp;
	
	temp.rows = rows;
	temp.columns = columns;

	temp.compute = Matrix_Product;
	temp.computeSm = Matrix_Sum;
	temp.printthis = printMatrix;
	
	temp.array = malloc(rows * sizeof(int *));
	for (int i = 0; i < rows; i++){
		temp.array[i] = malloc(columns * sizeof(int));
	}
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			temp.array[i][j] = i + j;
		}
	}

	*A = temp;
}



typedef struct Vector_{									// Vector_ class which inherits Matrix_ class
	Matrix vec;
	ini_Vector init;
}Vector;

void Vector_L1Norm(struct Vector_ *A)
{
	int c, d;
	int sum = 0;
	if (A->vec.rows == 0)
		printf("Vector having row 0 cant be calculated.\n");
	for (int d = 0; d < A->vec.rows; d++) {
		sum += A->vec.array[d][0];
	}
	printf("%d\n", sum);
}

void initVector(struct Vector_ *A, int rows){			// Vector_ class Constructor Definition
	Vector nvec;
	nvec.vec.init = initMatrix;
	nvec.vec.computeSm = Matrix_Sum;
	nvec.vec.init(&nvec.vec, rows, 1);
	*A = nvec;
}



int main() {
	computeL1Norm *vtable_MatrixL1Form = malloc(sizeof(computeL1Norm));
	computeL1Norm *vtable_VectorL1Form = malloc(sizeof(computeL1Norm));

	vtable_MatrixL1Form[0] = &Matrix_L1Norm;
	vtable_VectorL1Form[0] = &Vector_L1Norm;

	Matrix mat1;					// Matrix 1
	mat1.init = initMatrix;
	mat1.init(&mat1, 3, 3);
	mat1.vtable = vtable_MatrixL1Form;	// Matrix 1 vtable
	printf("Matrix 1: \n");
	mat1.printthis(&mat1);

	Matrix mat2;
	mat2.init = initMatrix;			// Matrix 1
	mat2.init(&mat2, 3, 3);
	mat2.vtable = vtable_MatrixL1Form;	// Matrix 2 vtable
	printf("\nMatrix 2: \n");
	mat2.printthis(&mat2);

	printf("\nMatrix Right Multiplication: \n");
	mat1.compute(&mat1, &mat2);
	printf("\nMatrix Sum: \n");
	mat1.computeSm(&mat1, &mat2);
	printf("\nMatrix L1 Norm: \n");
	mat1.vtable[0](&mat1);
	
	Vector vect;
	vect.init = initVector;
	vect.init(&vect, 3);
	vect.vec.vtable = vtable_VectorL1Form;

	printf("\nVector: \n");
	vect.vec.printthis(&vect.vec);
	printf("\nVector L1 Norm: \n");
	vect.vec.vtable[0](&vect);

	return 0;
}