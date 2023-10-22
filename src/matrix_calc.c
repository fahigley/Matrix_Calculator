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
};


// struct for matrix
//
// @param rows number of rows
// @param cols number of columns
// @param data 2d array that stores matrix values
typedef struct{
	int rows; //number of rows
	int cols; // number of columns
	float **data; //2d array for matrix values
}Matrix;


//functions


//help menu printout
void helpMenu(){
	printf("HELP MENU\n");
}


//initializes a matrix struct
//
//@return matrix struct
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


//frees the memory of the given matrix
//
//@param mat given matrix to be freed
void freeMatrix(Matrix *mat){\
	//iterate throught the matrix and free memory
	for(int i = 0; i < mat->rows; i++){
		free(mat->data[i]);
	}
	//free matrix itself
	free(mat);
}


//prints a given matrix to the terminal
//
//@param mat given matrix to be printed
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



// swaps two given rows in a matrix
//
//@param mat given matrix
//@param row1 first row to be swapped
//@param row2 second row to be swapped
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



//adds  a multiple of row2 to row 1
//set factor to 1 to keep same values
//
//@param mat given matrix
//@param row1 first row
//@param row2 second row
//@param factor multiple row 2 will be multiplied
//
//
void addRowsByMultiple(Matrix *mat, int row1, int row2, float factor){
	if(mat == NULL){
		return;
	}

	for(int i = 0; i < mat->rows; i++){
		mat->data[row1][i] += mat->data[row2][i] * factor;
	}

}



//multiplies a given row by a multiplier
//
//@param mat given matrix
// @param row given row to be multiplied
// @param mulitplier given multiplier
//
void multiplyRowBy(Matrix *mat, int row, int multiplier){
	if(mat == NULL){
		return;
	}

	for(int i = 0; i < mat->rows; i++){
		mat->data[row][i] *= multiplier;
	}
	
}


//Reduces a given matrix to Reduced row echelon form
//
// @param mat given matrix
//
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


// driver function for the reduced row echelon calculation
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


//calculates the determinant for a 3x3 matrix
//
//@param a (1,1) of the given matrix
//@param b (1,2) of the given matrix
//@param c (2,1) of the given matrix
//@param d (2,2) of the given matrix
int determinant2x2(int a, int b, int c, int d){
  return (a*d)-(b*c);
}


//driver for the determinant calculation
void determinantDriver(){
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

  } else{
    printf("Invalid matrix dimentions\n");
  }

  freeMatrix(pmat);
  pmat = NULL;
}



//extra function for testing
void test(){
}


//main driver function
int main(){
 	printf("Welcome to matrix Calculator!\n");
 	printf("This apps purpose is to help with matrix calculations\n");
 	
 	char optionbuf[4];
 	
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

 			case TEST:
 				test();
 				break;

 			default:
 				printf("Matrix operation not found try again:");

 		}
 	}
   return 0;
}

