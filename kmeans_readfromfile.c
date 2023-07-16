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

int len_str(char *str);

char *stduplic(char *str){
    int i;
    char *dup;
    dup = malloc((int)len_str(str) * sizeof(char));
    if (dup == NULL){
        return NULL;
    }
    for(i=0; i < len_str(str);i++){
        dup[i] = str[i];
    }
    return dup;

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
 
char* readline(FILE* fp){
    unsigned long buff_size = 256;
    char* line = NULL;
    char c;
    unsigned int i = 0;
    while (1){
        line = realloc(line,buff_size*sizeof(char));
        if (line == NULL){
            free(line);
            return NULL;
        }
        while (i * sizeof(char) < buff_size) {
            c = (char) fgetc(fp);
            if (c == '\n' || c == EOF) {
                line[i] = '\0';
                return line;
            }
            line[i] = c;
            i++;
        }
        buff_size *= 2;
    }
}
 
int isNumeric(char *str)
{
    while (!(*str == '\0'))
    {
        if (*str < '0' || *str > '9')
        {
            return 0;
        }
        str++;
    }
    return 1;
}

int len_str(char* str){
    int i, length = 0;
    for(i = 0; str[i] != '\0'; i++){
        length++;
    }
    return length;
}

char* my_strchr(const char* str, int c) {
    while (*str != '\0') {
        if (*str == c) {
            return (char*)str;
        }
        str++;
    }
    return NULL;
}

char* my_strtok(char* str, const char* delimiters) {
    static char* nextToken = NULL;
    char* tokenStart;

    if (str != NULL) {
        nextToken = str;
    }
    else if (nextToken == NULL) {
        return NULL;
    }

    while (*nextToken != '\0' && my_strchr(delimiters, *nextToken) != NULL) {
        nextToken++;
    }

    if (*nextToken == '\0') {
        return NULL;
    }

    tokenStart = nextToken;
    while (*nextToken != '\0' && my_strchr(delimiters, *nextToken) == NULL) {
        nextToken++;
    }

    if (*nextToken == '\0') {
        nextToken = NULL;
    }
    else {
        *nextToken = '\0';
        nextToken++;
    }

    return tokenStart;
}

int k_means_skeleton(int n, int d, int k, int iter, double** input);

int k_means(int k, int iter) {
    int d = -1;
    int n=0;
    int i;
    unsigned int file_ended = 0;
    unsigned long buff_size = 256; /* check for small value*/
    double** input_matrix = malloc(buff_size*sizeof(double*));

    FILE* fp = stdin;
    if (fp == NULL){
        printf("An Error Has Occurred: fp == NULL");
        free(input_matrix);
        return 1;
    }
    
    if (input_matrix==NULL){
        return 1;
    }
    while (1){
        double ** temp;
        while (((n + 1)*sizeof(double*)) < buff_size) {
            char *line;
            char *line_dup;
            char *token;
            if (feof(fp)){
                file_ended = 1;
                break;
            }
            line = readline(fp);
            if (len_str(line) == 0 ){
                free(line);
                file_ended = 1;
                break;
            }
            if (d == -1) { /* only on the first iteration we first calculate the dimension of the vectors*/
                line_dup = stduplic(line);
                if (line_dup == NULL){
                    free(input_matrix);
                    return 1;
                }
                token = my_strtok(line_dup, ",");
                d = 0;
                while (token != NULL) {
                    token = my_strtok(NULL, ",");
                    d++;
                }
                free(line_dup);
            }
            input_matrix[n] = malloc(d*sizeof(double));
            if (input_matrix[n] == NULL){
                free_matrix(n,input_matrix);
                return 1;
            }
            token = my_strtok(line,",");
            for (i=0;i<d;i++){
                input_matrix[n][i] = atof(token);
                token = my_strtok(NULL,",");
            }
            free(line);
            n++;
        }
        if (file_ended){
            break;
        }
        buff_size*=2;
        temp = realloc(input_matrix,buff_size*sizeof(double*));
        if (temp==NULL){
            free_matrix(n,input_matrix);
        }
        input_matrix = temp;
    }
    if (k>=n){
        printf("Invalid number of clusters!");
        return 1;
    }
    return k_means_skeleton(n,d,k,iter,input_matrix);
}
 
void free_matrix(int rows_num, double** matrix);

char* stduplic(char *str);
double vector_dist_squared(int len, const double* v, const double* u);
double** create_empty_matrix(int row_num, int col_num);
void vector_addition(int len, double* output, const double* v, const double* u);
char* readline(FILE* fp);
int isNumeric(char *str);
char* my_strchr(const char* str, int c);
char* my_strtok(char* str, const char* delimiters);




int main(int argc, char** argv) {
    int k;
    int iter;
    if (argc < 2 || argc >3){
        printf( "An Error Has Occurred: argc < 2 || argc > 3");
        return 1;
    }
    k = atoi(argv[1]);
    if (argc == 2) {
        iter = 200;
    }
    else{
        iter = atoi(argv[2]);
    }
    if (!isNumeric(argv[1]) || k<1){
        printf("Invalid number of clusters!");
        return 1;
    }
    if (argc == 3){
        if (!isNumeric(argv[2]) || iter<1 || iter>999){
            printf("Invalid maximum iteration!");
            return 1;
        }
    }
    return k_means(k,iter);
}

 
 
int k_means_skeleton(int n,int d,int k,int iter, double** input){
    int i;
    int j;
    int c;
    int t;
    double epsilon = 0.001;
    double** centroid_update;
    int* current_cluster_sizes;
    double** centroid_lst = create_empty_matrix(k,d);
    if (centroid_lst == NULL){
        free_matrix(n,input);
        return 1;
    }
    centroid_update = create_empty_matrix(k,d);
    if (centroid_update == NULL){
        free_matrix(n,input);
        free_matrix(k,centroid_lst);
        return 1;
    }
    current_cluster_sizes = malloc(sizeof(int)*k);
    if (current_cluster_sizes == NULL){
        free_matrix(n,input);
        free_matrix(k,centroid_update);
        free_matrix(k,centroid_lst);
        return 1;
    }
    
    for (i=0;i<k;i++){
        for (j=0;j<d;j++) {
            centroid_lst[i][j] = input[i][j];
            centroid_update[i][j] = 0;
        }
    }
    
    for (i=0;i<iter;i++){
        int converged = 1; /*main loop, index i is the number of the iteration*/
        for (t=0;t<k;t++){ /* reset cluster sizes to 0*/
            current_cluster_sizes[t] = 0;
        }
        for (t=0;t<k;t++){ /* reset updated cluster matrix to 0*/
            for (j=0;j<d;j++){
                centroid_update[t][j] = 0;
            }
        }
        for (j=0;j<n;j++) { /*for vector in input, j is the vector index*/
            double min_dist = -1;
            int closest_centroid = -1;
            for (c = 0; c < k; c++){ /*find the closest centroid to the vector, c is the centroid index*/
                double dist = vector_dist_squared(d, input[j], centroid_lst[c]);
            if (dist < min_dist || min_dist == -1) {
                min_dist = dist;
                closest_centroid = c;
            }
        }
            vector_addition(d,centroid_update[closest_centroid],centroid_update[closest_centroid],input[j]);
            current_cluster_sizes[closest_centroid]+=1;
        }
        for (c=0;c<k;c++){
            for(t=0;t<d;t++){
                centroid_update[c][t] = centroid_update[c][t]/current_cluster_sizes[c];
            }
        }
        
        for(c=0;c<k;c++) { /* update the centroids and check how much each one moved*/
            double dist_squared = vector_dist_squared(d,centroid_update[c],centroid_lst[c]);
            if (sqrt(dist_squared)>=epsilon){ /* check for convergence for centroid c*/
                converged=0;
            }
            for(j=0;j<d;j++){ /* fill centroid c with updated values*/
                centroid_lst[c][j] = centroid_update[c][j];
            }
        }
        if(converged){
            break;
        }
    }

    for(i=0; i<k; i++){
        for(j=0; j<d-1; j++){
                printf("%.4f,",centroid_lst[i][j]);
        }
        printf("%.4f\n",centroid_lst[i][d-1]);
    }
    free_matrix(n, input);
    free_matrix(k, centroid_lst);
    free_matrix(k, centroid_update);
    free(current_cluster_sizes);
 
    return 0;
}
