#define MATRIX_MAIN
#include "IQmathLib.h"
#include "Matrix.h"

void Matrix_Init(MatrixStruct *m) 
{
	int i,j;
	m->Matrix = (long **) malloc(m->nrows*sizeof(long *));
	for (i=0; i<m->nrows; i++) {
		(m->Matrix)[i] = (long *) malloc(m->ncolumns*sizeof(long));
	}
	for (i=0; i<m->nrows; i++) {
		for (j=0; j<m->ncolumns; j++) {
			(m->Matrix)[i][j] = 0;
		}
	}
 
}

void Matrix_Copy(MatrixStruct *m, MatrixStruct *x)
{
	int i,j;
	for (i=0; i<m->nrows; i++) {
		for (j=0; j<m->ncolumns; j++) {
			m->Matrix[i][j] = x->Matrix[i][j];
		}
	}
}

void Matrix_Assign(MatrixStruct *m, long a[])
{
	int i,j;
	for (i=0; i<m->nrows; i++) {
		for (j=0; j<m->ncolumns; j++) {
			m->Matrix[i][j] = a[i*(m->ncolumns)+j];
		}
	}
}

void Matrix_Add(MatrixStruct *m,MatrixStruct *x,MatrixStruct *y) 
{
	int i,j;
	for (i=0; i<m->nrows; i++) {
		for (j=0; j<m->ncolumns; j++) {
			m->Matrix[i][j] = x->Matrix[i][j] + y->Matrix[i][j];
		}
	}
}

void Matrix_Substract(MatrixStruct *m,MatrixStruct *x, MatrixStruct *y) 
{
	int i,j;
	for (i=0; i<m->nrows; i++) {
		for (j=0; j<m->ncolumns; j++) {
			m->Matrix[i][j] = x->Matrix[i][j] - y->Matrix[i][j];
		}
	}
}

void Matrix_DotProduct(MatrixStruct *m,long x, MatrixStruct *y) 
{
	int i,j;
	for (i=0; i<m->nrows; i++) {
		for (j=0; j<m->ncolumns; j++) {
			m->Matrix[i][j] = _IQmpy(x,y->Matrix[i][j]);
		}
	}
}

void Matrix_CrossProduct(MatrixStruct *m, MatrixStruct *x, MatrixStruct *y) 
{
	int i,j,k;
	long temp;
	if ((m->nrows == x->nrows) && (m->ncolumns == y->ncolumns) ) {
		for (i=0; i<m->nrows; i++) {
			for (j=0; j<m->ncolumns; j++) {
				temp = 0;
				for(k=0; k<x->ncolumns; k++) {
					//if ( (x->Matrix[i][k] == 0) || (y->Matrix[k][j] == 0) ) {
					//   temp = 0;	
					//} else {
					   temp = temp + _IQmpy(x->Matrix[i][k],y->Matrix[k][j]);	
					//}
					
				}
				m->Matrix[i][j] = temp;
			}
		}
	}
		
}


