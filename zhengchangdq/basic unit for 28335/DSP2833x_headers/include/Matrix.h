#ifndef MATRIX_H
#define MATRIX_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MATRIX_MAIN
#define EXTERN
#else
#define EXTERN extern
#endif

#include <stdlib.h>
typedef struct
{
	int nrows;
	int ncolumns;
	long **Matrix;
	void (*Init)();

}MatrixStruct;
#define MATRIXSTRUCT_DEFAULTS {0,0,NULL,(void (*)(Uint32))Matrix_Init}

EXTERN int Matrix_temp_i;
EXTERN int Matrix_temp_j;
EXTERN int Matrix_temp_k;

#define MATRIX_CROSSPRODUCT_MARCO(m,x,y)																													\
																																							\
	if ((m.nrows == x.nrows) && (m.ncolumns == y.ncolumns) ) {																								\
		for (Matrix_temp_i=0; Matrix_temp_i<m.nrows; Matrix_temp_i++) {																						\
			for (Matrix_temp_j=0; Matrix_temp_j<m.ncolumns; Matrix_temp_j++) {																				\
				m.Matrix[Matrix_temp_i][Matrix_temp_j] = 0;																									\
				for(Matrix_temp_k=0; Matrix_temp_k<m.ncolumns; Matrix_temp_k++) {																			\
					if ( (x.Matrix[Matrix_temp_i][Matrix_temp_k] == 0) || (y.Matrix[Matrix_temp_k][Matrix_temp_j] == 0) ) {									\
					   m.Matrix[Matrix_temp_i][Matrix_temp_j] = 0;																							\
					} else {																																\
					   m.Matrix[Matrix_temp_i][Matrix_temp_j] += _IQmpy(x.Matrix[Matrix_temp_i][Matrix_temp_k],y.Matrix[Matrix_temp_k][Matrix_temp_j]);		\
					}																																		\
				}																																			\
			}																																				\
		}																																					\
	}																																						\



#define MATRIX_ADD_MARCO(m,x,y)																																\
																																							\
	for (Matrix_temp_i=0; Matrix_temp_i<m.nrows; Matrix_temp_i++) {																							\
		for (Matrix_temp_j=0; Matrix_temp_j<m.ncolumns; Matrix_temp_j++) {																					\
			m.Matrix[Matrix_temp_i][Matrix_temp_j] = x.Matrix[Matrix_temp_i][Matrix_temp_j] + y.Matrix[Matrix_temp_i][Matrix_temp_j];						\
		}																																					\
	}																																						\



#define MATRIX_COPY_MARCO(m,x)																																\
																																							\
	for (Matrix_temp_i=0; Matrix_temp_i<m.nrows; Matrix_temp_i++) {																							\
		for (Matrix_temp_j=0; Matrix_temp_j<m.ncolumns; Matrix_temp_j++) {																					\
			m.Matrix[Matrix_temp_i][Matrix_temp_j] = x.Matrix[Matrix_temp_i][Matrix_temp_j];																\
		}																																					\
	}																																						\



extern void Matrix_Init(MatrixStruct *);
extern void Matrix_Assign(MatrixStruct *, long []);
extern void Matrix_Copy(MatrixStruct *, MatrixStruct *);
extern void Matrix_Add(MatrixStruct *, MatrixStruct *, MatrixStruct *);
extern void Matrix_Substract(MatrixStruct *, MatrixStruct *, MatrixStruct *);
extern void Matrix_DotProduct(MatrixStruct *, long, MatrixStruct *);
extern void Matrix_CrossProduct(MatrixStruct *, MatrixStruct *, MatrixStruct *);

#pragma CODE_SECTION(Matrix_Copy, "myfuncs");
#pragma CODE_SECTION(Matrix_Add, "myfuncs");
#pragma CODE_SECTION(Matrix_Substract, "myfuncs");
#pragma CODE_SECTION(Matrix_DotProduct, "myfuncs");
#pragma CODE_SECTION(Matrix_CrossProduct, "myfuncs");


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif // end of MATRIX_H definition
