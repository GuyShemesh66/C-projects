# define PY_SSIZE_T_CLEAN
# include <Python.h>
# include <stdio.h>
# include <stdlib.h>
# include <math.h>


void free_matrix(int rows_num,double** matrix){
    int i;
    for (i=0; i < rows_num; i++){
        free(matrix[i]);
    }
    free(matrix);
}


/* len = len(v) = len(u)*/
double vector_dist_squared(int len,const double* v,const double* u){
    int i;
    double sum_of_squares = 0;
    for (i=0; i < len; i++){
        sum_of_squares += (v[i] - u[i]) * (v[i] - u[i]);
    }
    return sum_of_squares;
}

double** create_empty_matrix(int row_num, int col_num){
    double** matrix = malloc(sizeof(double*) * row_num);
    int i;
    if (matrix == 0){
        return NULL;
    }
    
    for (i=0; i < row_num; i++){
        matrix[i] = malloc(sizeof(double ) * col_num);
        if (matrix[i] == NULL){
            free_matrix(row_num, matrix);
            return NULL;
        }
    }
    return matrix;
}

/* ret is the array the values are put into, can be the same as one of the inputs */
void vector_addition(int len ,double* output , const double* v , const double* u){
    int i;
    double temp;
    for (i=0; i < len; i++){
        temp = v[i] + u[i];
        output[i] = temp;
    }
}
 

double** k_means_skeleton(int n, int d, int k, int iter, double** input, double epsilon, double** centroid_lst);

double** k_means(double** centroids, int k, int iter, double** input_matrix, int d, int n, double epsilon) {
    if (input_matrix==NULL){
        return NULL;
    }
    return k_means_skeleton(n,d,k,iter,input_matrix, epsilon, centroids);
}
 
void free_matrix(int rows_num, double** matrix);
double vector_dist_squared(int len, const double* v, const double* u);
double** create_empty_matrix(int row_num, int col_num);
void vector_addition(int len, double* output, const double* v, const double* u);
PyObject* convertToPyObjectArray(double** doubleArray, int numRows, int numCols);
void convertToDoubleArray(PyObject* pyObject, int numRows, int numCols, double** doubleArray);
 
double** k_means_skeleton(int n,int d,int k,int iter, double** input, double epsilon, double** centroid_lst){
    int i;
    int j;
    int c;
    int t;
    double** centroid_update;
    int* current_cluster_sizes;
    centroid_update = create_empty_matrix(k,d);
    if (centroid_update == NULL){
        free_matrix(n,input);
        free_matrix(k,centroid_lst);
        return NULL;
    }
    current_cluster_sizes = malloc(sizeof(int)*k);
    if (current_cluster_sizes == NULL){
        free_matrix(n,input);
        free_matrix(k,centroid_update);
        free_matrix(k,centroid_lst);
        return NULL;
    }
    
    for (i=0;i<k;i++){
        for (j=0;j<d;j++) {
            centroid_update[i][j] = 0;
        }
    }

    
    for (i = 0; i < iter; i++){
        int converged = 1; /*main loop, index i is the number of the iteration*/
        for (t = 0 ; t < k; t++){ /* reset cluster sizes to 0*/
            current_cluster_sizes[t] = 0;
        }
        for (t=0;t<k;t++){ /* reset updated cluster matrix to 0*/
            for (j=0;j<d;j++){
                centroid_update[t][j] = 0;
            }
        }
        for (j = 0; j < n; j++) { /*for vector in input, j is the vector index*/
            double min_dist = -1;
            int closest_centroid = -1;
            for (c = 0; c < k; c++){ /*find the closest centroid to the vector, c is the centroid index*/
                double dist = vector_dist_squared(d, input[j], centroid_lst[c]);
            if (dist < min_dist || min_dist == -1) {
                min_dist = dist;
                closest_centroid = c;
            }
        }
            vector_addition(d ,centroid_update[closest_centroid] ,centroid_update[closest_centroid] ,input[j]);
            current_cluster_sizes[closest_centroid] += 1;
        }
        for (c = 0; c < k; c++){
            for(t = 0; t < d; t++){
                centroid_update[c][t] = centroid_update[c][t]/current_cluster_sizes[c];
            }
        }
        

        for(c = 0; c < k; c++) { /* update the centroids and check how much each one moved*/
            double dist_squared = vector_dist_squared(d,centroid_update[c],centroid_lst[c]);
            if (sqrt(dist_squared) >= epsilon){ /* check for convergence for centroid c*/
                converged=0;
            }
            for(j = 0; j < d; j++){ /* fill centroid c with updated values*/
                centroid_lst[c][j] = centroid_update[c][j];
            }
        }
        if(converged){
            break;
        }
    }


    free_matrix(n, input);
    free_matrix(k, centroid_update);
    free(current_cluster_sizes);
 
    return centroid_lst;
}


PyObject* kmeans_algorithm(PyObject* self, PyObject* args) {
    PyObject* centroids_obj;
    PyObject* data_obj;
    int iter;
    double epsilon;
    int n;
    int d;
    int k;
    double** centroids_data;
    double** data_array;
    PyObject* res;
    
    /* This parses the Python arguments into generic PyObject* variables */
    if (!PyArg_ParseTuple(args, "OOidiii", &centroids_obj, &data_obj, &iter, &epsilon, &n, &d, &k)) {
        return NULL;
    }
    centroids_data = (double**)malloc(k * sizeof(double*));
    data_array = (double**)malloc(n * sizeof(double*));
    /* Convert the objects to arrays if desired */
   
    convertToDoubleArray(data_obj, n, d, data_array);
    convertToDoubleArray(centroids_obj, k ,d, centroids_data);
    
    

    double** result = k_means(centroids_data, k, iter, data_array, d, n, epsilon);
    res = convertToPyObjectArray(result, k ,d);
    free_matrix(k, result);
    /* Return the final centroids as a Python object */
    return Py_BuildValue("O", res);
}


static PyMethodDef kmeans_methods[] = {
        {"fit", (PyCFunction)kmeans_algorithm, METH_VARARGS, PyDoc_STR( "Run K-means algorithm with given initial centroids and data points.")},
        {NULL, NULL, 0, NULL}
};


static struct PyModuleDef kmeans_module = {
        PyModuleDef_HEAD_INIT,
        "mykmeanssp",
        "Extension module for K-means algorithm",
        -1,
        kmeans_methods
};


PyMODINIT_FUNC PyInit_mykmeanssp(void) {
    PyObject *m;
    m = PyModule_Create(&kmeans_module);
    if (!m){
        return NULL;
    }
    return m;
}


void convertToDoubleArray(PyObject* pyList, int numRows, int numCols, double** doubleArray) {
    if (numRows < 0){
        return;
    }
    PyObject* point;
    PyObject* item;
    int i;
    int j;
    if (!doubleArray) {
        return;
    }
    for (i = 0; i < numRows; i++) {
        doubleArray[i] = (double*)malloc(numCols * sizeof(double));
        if (!doubleArray[i]) {
            free_matrix(numRows, doubleArray);
            return;
        }
        point = PyList_GetItem(pyList, i);
        if (!PyList_Check(point)) {
            free_matrix(numRows, doubleArray);
            return;
        }
        for (j = 0; j < numCols; j++) {
            item = PyList_GetItem(point, j);
            if (!PyFloat_Check(item)) {
                free_matrix(numRows, doubleArray);
                return;
            }
            doubleArray[i][j] = PyFloat_AsDouble(item);
        }
    }
}


PyObject* convertToPyObjectArray(double** doubleArray, int numRows, int numCols) {
    PyObject* pyObjectArray = PyList_New(numRows);
    if (!pyObjectArray) {
        return NULL;
    }

    for (int i = 0; i < numRows; i++) {
        PyObject* innerListObj = PyList_New(numCols);
        if (!innerListObj) {
            Py_DECREF(pyObjectArray);
            return NULL;
        }

        for (int j = 0; j < numCols; j++) {
            PyObject* item = PyFloat_FromDouble(doubleArray[i][j]);
            if (!item) {
                Py_DECREF(innerListObj);
                Py_DECREF(pyObjectArray);
                return NULL;
            }
            PyList_SET_ITEM(innerListObj, j, item);  // This steals a reference to item
        }
        PyList_SET_ITEM(pyObjectArray, i, innerListObj);  // This steals a reference to innerListObj
    }

    return pyObjectArray;
}

