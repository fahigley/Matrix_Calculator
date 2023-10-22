#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//enum and structs

enum Option{
	QUIT = 'q',
	HELP = 'h',
	TEST = 't',
	ROWECHELONFORM = 'r',
	DETERMINANT = 'd',
};

typedef struct{
	int rows; //number of rows
	int cols; // number of columns
	float **data; //2d array for matrix values
}Matrix;


//functions

void helpMenu(){
	printf("HELP MENU\n");
}


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


void freeMatrix(Matrix *mat){\
	//iterate throught the matrix and free memory
	for(int i = 0; i < mat->rows; i++){
		free(mat->data[i]);
	}
	//free matrix itself
	free(mat);
}


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


//adds row 2 to row 1 by the multiple factor
void addRowsByMultiple(Matrix *mat, int row1, int row2, float factor){
	if(mat == NULL){
		return;
	}

	for(int i = 0; i < mat->rows; i++){
		mat->data[row1][i] += mat->data[row2][i] * factor;
	}

}


void multiplyRowBy(Matrix *mat, int row, int multiplier){
	if(mat == NULL){
		return;
	}

	for(int i = 0; i < mat->rows; i++){
		mat->data[row][i] *= multiplier;
	}
	
}

void ReducedRowEchelonForm(Matrix *mat){
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


void rowEchelonFormDriver(){
	Matrix mat = initializeMatrix();
	Matrix *pmat = &mat;
	printMatrix(pmat);
	printf("The Reduced Row Echelon Form is:\n");
	ReducedRowEchelonForm(pmat);
	printMatrix(pmat);

	freeMatrix(pmat);
	pmat = NULL;
}

void determinant(Matrix *mat){

}



void test(){
	Matrix mat = initializeMatrix();
	Matrix *pmat = &mat;
	printMatrix(pmat);
	printf("\n");
	multiplyRowBy(pmat,0,8);
	printMatrix(pmat);
	printf("Test done:\n");
	
}


//Main method

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

 			case TEST:
 				test();
 				break;

 			default:
 				printf("Matrix operation not found try again:");

 		}
 	}
   return 0;
}

