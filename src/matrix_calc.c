#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//enums for user interface
enum Option{
	QUIT = 'q',
	HELP = 'h',
	TEST = 't',
	ROWECHELONFORM = 'r',
	DETERMINANT = 'd',
	MULTIPLICATION = 'm'
};


/**
 * the matrix struct
*/
typedef struct{
	int rows; //number of rows
	int cols; // number of columns
	float **data; //2d array for matrix values
}Matrix;


//functions


/**
 * prints out some help text like the different calculations and how to run them
*/
void helpMenu(){
  printf("Enter the character associated with the matrix calculation you want\n");
  printf("To quit the program use 'q' \n");
  printf("The current version supports the following calculations:\n");
  printf("	Reduced row echelon form: 'r' \n");
  printf("	Determinant: 'd' \n");
  printf("	Matirx Multiplication: 'm'\n");
  }


/**
 * initializes a matrix using user input
 * 
 * @return the new matrix
*/
Matrix initializeMatrix(){
	//initialize matrix
	Matrix matrix;
    
    //get dimensions from user
	printf("Rows:");
	scanf("%d", &matrix.rows);
	printf("Columns:");
	scanf("%d", &matrix.cols);

	//allocate memory for rows
    matrix.data = (float **)malloc(matrix.rows * sizeof(float *));
    if(matrix.data == NULL){
    	fprintf(stderr, "Memory allocation failed for matrix.data\n");
        exit(1);
    }

    //allocate memory for coumns
    for (int i = 0; i < matrix.rows; i++) {
        matrix.data[i] = (float *)malloc(matrix.cols * sizeof(int));
        if(matrix.data[i] == NULL){
        	fprintf(stderr, "Memory allocation failed for matrix.data\n");
        	exit(1);
        }
    }

    // Initialize the matrix elements
    for (int i = 0; i < matrix.rows; i++) {
        for (int j = 0; j < matrix.cols; j++) {
            printf("Matrix[%d][%d]:", i, j);
            scanf("%f", &matrix.data[i][j]);
        }
    }

    return matrix;
}


/**
 * initializes a matrix without user input
 * all values of the matrix are set to 0
 * 
 * @param rows the number of rows of the new matrix
 * @param cols the number of columns of the new matrix
 * 
 * @return the new matrix struct
*/
Matrix initailizeMatrixNoInput(int rows, int cols) {
    Matrix matrix;

    // allocate memory for rows
    matrix.data = (float**)malloc(rows * sizeof(float*));
    if (matrix.data == NULL) {
        fprintf(stderr, "Memory allocation failed for matrix.data\n");
        exit(1);
    }

    // allocate memory for columns
    for (int i = 0; i < rows; i++) {
        matrix.data[i] = (float*)malloc(cols * sizeof(float));
        if (matrix.data[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for matrix.data\n");
            exit(1);
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix.data[i][j] = 0;
        }
    }

    matrix.rows = rows;
    matrix.cols = cols;

    return matrix;
}

/**
 * frees the memory of a given matrix 
 * 
 * @param mat the given matrix to be freed
*/
void freeMatrix(Matrix *mat) {
    // Iterate through the matrix and free memory
    for (int i = 0; i < mat->rows; i++) {
        free(mat->data[i]);
    }

    // Free matrix data
    free(mat->data);

    // Set matrix data pointer to NULL
    mat->data = NULL;

    // Free matrix structure itself
    free(mat);
}


/**
 * prints a given matrix to the terminal
 * 
 * @param mat the given martix to be printed
*/
void printMatrix(Matrix *mat){
	//iterate through the matrix and print each element
	if(mat->data == NULL){
		printf("Error cannot print matrix");
	}

	for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
        	if(mat->data[i] == NULL){
        		printf("Error cannot print matrix");
        	}
        	printf("%.1f ", mat->data[i][j]);
        }
    printf("\n");
    }
}



/**
 * swaps 2 rows in a matrix
 * 
 * @param mat the given matrix 
 * @param row1 the first row
 * @param row2 the second row
*/
void swapRows(Matrix *mat, int row1, int row2){
	//check for valid row1 and row 2 values
	if (row1 < 0 || row1 >= mat->rows || row2 < 0 || row2 >= mat->rows) {
       printf("Error: Cannot swap rows");
       return;
    }

    //allocate temporary row
	float *temp = malloc(sizeof(float) * mat->rows);

	//copy row1 into temp
	for(int i = 0; i < mat->rows; i++){
		temp[i] = mat->data[row1][i];
	}

	//copy row2 to row1
	for(int i = 0; i < mat->rows; i++){
		mat->data[row1][i] = mat->data[row2][i];
	}

	//copy temp to row2
	for(int i = 0; i < mat->rows; i++){
		mat->data[row2][i] = temp[i];
	}

	//free temp & return
	free(temp);
	temp = NULL;

}



/** 
 * adds a multiple of row1 to row2
 * 
 * @param mat the given matrix
 * @param row1 the row where the sum will be
 * @param row2 the row to be multiplied then added to row1
 * @param factor the number that row 2 will be multiplied by
*/
void addRowsByMultiple(Matrix *mat, int row1, int row2, float factor){
	if(mat == NULL){
		return;
	}

	for(int i = 0; i < mat->rows; i++){
		mat->data[row1][i] += mat->data[row2][i] * factor;
	}

}



/**
 * multiplies a given row by a multiplier
 * 
 * @param mat the given matrix
 * @param row the row to be multiplied
 * @param multiplier the multiplier for the row
*/
void multiplyRowBy(Matrix *mat, int row, int multiplier){
	if(mat == NULL){
		return;
	}

	for(int i = 0; i < mat->rows; i++){
		mat->data[row][i] *= multiplier;
	}
	
}


/**
 * reduced a matrix to reduced row echelon form 
 * 
 * @param mat given matrix
*/
void reducedRowEchelonForm(Matrix *mat){
	int lead = 0;
	int rowCount = mat->rows;
	int columnCount = mat->cols;
	for(int r = 0; r < rowCount; r++){
		if(columnCount <= lead){
			return;
		}
		int i = r;
		while(mat->data[i][lead] == 0){
			i++;
			if(rowCount == i){
				i = r;
				lead++;
				if(columnCount == lead){
					return;
				}
			}
		}
		swapRows(mat, i, r);
		if(mat->data[i][lead] != 0){
			multiplyRowBy(mat, r, 1/mat->data[r][lead]);
		}
		for(i = 0; i <rowCount; i++){
			if( i != r){
				float factor = mat->data[i][lead];
				addRowsByMultiple(mat, i, r, -factor);
			}
		}
		lead++;
	}
}


/**
 * driver function for the reduced row echelon calculation
*/
void rowEchelonFormDriver(){
	Matrix mat = initializeMatrix();
	Matrix *pmat = &mat;
	printMatrix(pmat);
	printf("The Reduced Row Echelon Form is:\n");
	reducedRowEchelonForm(pmat);
	printMatrix(pmat);

	freeMatrix(pmat);
	pmat = NULL;
}


/**
 * calculates the determinant for a 2x2 matrix
 *
 * @param a (1,1) of the given matrix
 * @param b (1,2) of the given matrix
 * @param c (2,1) of the given matrix
 * @param d (2,2) of the given matrix
*/
int determinant2x2(int a, int b, int c, int d){
  return (a*d)-(b*c);
}

/**
 * driver function for the determinant calculation
*/
void determinantDriver(){
	//initialize the matrix
  	printf("As of now the determinant calculation only supports 2x2 and 3x3 matricies:\n");
  	Matrix mat = initializeMatrix();
  	Matrix *pmat = &mat;
  	printMatrix(pmat);

  	if(pmat->rows == 2 && pmat->cols == 2){
    	int det = determinant2x2(pmat->data[0][0], pmat->data[1][1], pmat->data[0][1], pmat->data[1][0]);
    	printf("The determinant of the matrix is: %d\n", det);

  	} else if(pmat->rows == 3 && pmat->cols == 3){
    	int a, b, c;
    	a = pmat->data[0][0] * determinant2x2(pmat->data[1][1], pmat->data[1][2], pmat->data[2][1], pmat->data[2][2]);
    	b = pmat->data[0][1] * determinant2x2(pmat->data[1][0], pmat->data[1][2], pmat->data[2][0], pmat->data[2][2]);
    	c = pmat->data[0][2] * determinant2x2(pmat->data[1][0], pmat->data[1][1], pmat->data[2][0], pmat->data[2][1]);
    	int det = a-b+c;
    	printf("The determinant of the matrix is: %d\n", det);

  	}else{
    	printf("Invalid matrix dimentions\n");
  	}

  	freeMatrix(pmat);
  	pmat = NULL;
}

/**
 * multiplies the 2 given matricies
 * 
 * @param mat1 the first matrix
 * @param mat2 the second matrix
 * 
 * @return the product of the matrix multiplication
*/
Matrix matrixMultiplication(Matrix* mat1, Matrix* mat2) {
    Matrix ans = initailizeMatrixNoInput(mat1->rows, mat2->cols);

    for (int i = 0; i < mat1->rows; i++) {
        for (int j = 0; j < mat2->cols; j++) {
            ans.data[i][j] = 0;

            for (int k = 0; k < mat2->rows; k++) {
                ans.data[i][j] += mat1->data[i][k] * mat2->data[k][j];
            }
        }
    }
    return ans;
}

/**
 * simple driver function for matrix multiplication
*/
void multiplicationDriver() {
    Matrix ans;

    printf("Enter the first matrix\n");
    Matrix matrix1 = initializeMatrix();
    Matrix *pmat1 = &matrix1;

    printf("Enter the second matrix:\n");
    Matrix matrix2 = initializeMatrix();
    Matrix *pmat2 = &matrix2;

    if (pmat1->cols == pmat2->rows) {
        ans = matrixMultiplication(pmat1, pmat2);
        printf("The answer is:\n");
        printMatrix(&ans);

        // Free the matrices after using the result
		freeMatrix(&ans);
        freeMatrix(pmat1);
        freeMatrix(pmat2);
        
    } else {
        printf("Matrix dimensions are invalid:\n");

        // Free matrices if dimensions are invalid
        freeMatrix(pmat1);
        freeMatrix(pmat2);
    }

    // Set pointers to NULL after freeing
    pmat1 = NULL;
    pmat2 = NULL;
}

/**
 * extra function for testing
*/
void test(){
}

/**
 * main function
*/
int main(){
 	printf("Welcome to matrix Calculator!\n");
 	helpMenu();
 	
 	char optionbuf[5];
	
 	while(optionbuf[0] != QUIT){
 		printf("Select a matrix calculation or type 'h' for help: ");
 		scanf("%4s", optionbuf);

 		switch(optionbuf[0]){
 			case QUIT:
 				exit(0);
 				break;

 			case HELP:
 				helpMenu();
 				break;

 			case ROWECHELONFORM:
 				rowEchelonFormDriver();
 				break;

            case DETERMINANT:
            	determinantDriver();
                break;

			case MULTIPLICATION:
				multiplicationDriver();
				break;

 			case TEST:
 				test();
 				break;

 			default:
 				printf("Matrix operation not found try again:");

 		}
 	}
   return 0;
}

