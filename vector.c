#include <vector.h>

/*Create a new vector*/
vector *v_make(int n)
{
    vector *v = (vector *)calloc(n + 1, sizeof(vector));
    if (v == NULL)
    {
        printf("Error: Failed to allocate memory for vector\n");
        exit(EXIT_FAILURE);
    }
    v[n] = VEC_END_FLAG;
    return v;
}

/* Releases memory held by the vector v*/
void v_destroy(vector *v)
{
    free(v);
}

/* Error handling for null vector */
// Exit EXIT_FAILURE
void assertValidVec(vector *vec)
{
    if (vec == NULL)
    {
        fprintf(stderr, "Attempting to manipulate NULL vector\n");
        exit(EXIT_FAILURE);
    }
}

// Determines the size of a vector by traversing the memory
// block associated with v until VEC_END_FLAG is found.
// VEC_END_FLAG marks the end of the vector.
int v_len(vector *v)
{
    int i = 0;
    while (v[i] != VEC_END_FLAG)
    {
        i++;
    }
    return i;
}

// Prints a vector in the format
// [v1 v2 .. vn] n={length}\n
// PRINTV_SIGDIGITS indicates the number of significant digits you
// e.g. PRINTV_SIGDIGITS=2; length=3--> [1.00 1.00 1.00] n=3
// e.g. PRINTV_SIGDIGITS=1; length=3;--> [1.0 1.0 1.0] n=3

void v_print(vector *vec)
{
    int len = v_len(vec);
    printf("[");
    for (int i = 0; i < len; i++)
    {
        printf("%.*f", PRINTV_SIGDIGITS, vec[i]);
        if (i < len - 1)
        {
            printf(" ");
        }
    }
    printf("] n=%d\n", len);
}

// Creates a vector of size n and fills it with scalar s.
vector *v_fill(int n, float s)
{
    vector *v = v_make(n);
    for (int i = 0; i < n; i++)
    {
        v[i] = s;
    }
    return v;
}

// Creates a vector of size n and fills it with ones.
vector *v_ones(int n)
{
    return v_fill(n, 1.0);
}

/* Reads vector from file in the .dat format */
vector *v_read(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "File does not exist: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    int num_lines = countLines(filename);
    vector *v = v_make(num_lines);
    int i = 0;
    char buffer[BUFFER_SIZE];

    while (fgets(buffer, BUFFER_SIZE, file) != NULL)
    {
        v[i++] = atof(buffer);
    }

    fclose(file);
    return v;
}

/* Writes vector to file in the .dat format */
void v_write(char *filename, vector *v)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        fprintf(stderr, "File does not exist: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    int i;
    for (i = 0; i < v_len(v); i++)
    {
        fprintf(file, "%f\n", v[i]);
    }

    fclose(file);
}
// Verifies that vector a and b have equivalent lengths.
// Outputs an informative error message to std error with the following format
// "Attempting to add vectors of unequal length: {alength} - {blength}"
// If not equal, this function will exit out of program with EXIT_FAILURE CODE
void assertValidOperation(vector *a, vector *b)
{
    if (v_len(a) != v_len(b))
    {
        fprintf(stderr, "Attempting to add vectors of unequal length: %d - %d\n", v_len(a), v_len(b));
        exit(EXIT_FAILURE);
    }
}

// Pairwise add of vectors a and b
// Verifies that addition is legal -- i.e., lengths of a and b are equal
// Result is in a new vector
vector *v_add(vector *a, vector *b)
{
    assertValidOperation(a, b);

    int n = v_len(a);
    vector *result = v_make(n);

    for (int i = 0; i < n; i++)
    {
        result[i] = a[i] + b[i];
    }

    return result;
}

// Pairwise subtraction of vectors a and b
// Verifies that subtraction is legal -- i.e., lengths of a and b are equal
// Result is in a new vector
vector *v_sub(vector *a, vector *b)
{
    assertValidOperation(a, b);

    int n = v_len(a);
    vector *result = v_make(n);

    for (int i = 0; i < n; i++)
    {
        result[i] = a[i] - b[i];
    }

    return result;
}

// Multiplies every member of vector a with c
// Result is in a new vector
vector *v_mul(vector *a, float c)
{
    int n = v_len(a);
    vector *result = v_make(n);

    for (int i = 0; i < n; i++)
    {
        result[i] = a[i] * c;
    }

    return result;
}

// Divides every member of vector a with c
// Result is in a new vector
vector *v_div(vector *a, float c)
{
    int n = v_len(a);
    vector *result = v_make(n);

    for (int i = 0; i < n; i++)
    {
        result[i] = a[i] / c;
    }

    return result;
}

float v_mag(vector *vec)
{
    int n = v_len(vec);
    float sum = 0;
    for (int i = 0; i < n; i++)
    {
        float f = vec[i];
        sum += f * f;
    }
    return sqrt(sum);
}
float v_dot(vector *a, vector *b)
{
    int n1 = v_len(a);
    int n2 = v_len(b);

    if (n1 != n2)
    {
        fprintf(stderr, "Attempting to add vectors of unequal length: %d - %d", n1, n2);
        exit(EXIT_FAILURE);
    }

    int n = n1;
    float sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum += a[i] * b[i];
    }
    return sum;
}
float v_project(vector *a, vector *b)
{
    vector *bnorm = v_unit(b);
    float dp = v_dot(a, bnorm);
    return dp;
}

vector *v_unit(vector *vec)
{
    int n = v_len(vec);
    float mag = v_mag(vec);
    vector *c = v_make(n);
    c = v_div(vec, mag);
    return c;
}
float v_cosine(vector *a, vector *b)
{
    float dp = v_dot(a, b);
    float a_mag = v_mag(a);
    float b_mag = v_mag(b);
    return dp / (a_mag * b_mag);
}
float v_sum(vector *vec)
{
    float sum = 0;
    int n = v_len(vec);
    for (int i = 0; i < n; i++)
    {
        float f = vec[i];
        sum += f;
    }
    return sum;
}

float v_mean(vector *vec)
{
    assertValidVec(vec);

    int len = v_len(vec);
    float sum = v_sum(vec);
    return sum / len;
}

float v_range(vector *vec)
{
    assertValidVec(vec);

    int len = v_len(vec);
    float min_val = vec[0];
    float max_val = vec[0];

    for (int i = 1; i < len; i++)
    {
        if (vec[i] < min_val)
        {
            min_val = vec[i];
        }
        if (vec[i] > max_val)
        {
            max_val = vec[i];
        }
    }

    return max_val - min_val;
}

float v_std(vector *vec)
{
    assertValidVec(vec);

    int len = v_len(vec);
    float mean = v_mean(vec);

    float sum = 0;
    for (int i = 0; i < len; i++)
    {
        sum += pow(vec[i] - mean, 2);
    }

    return sqrt(sum / len);
}

vector *v_clamp(vector *a, float minV, float maxV)
{
    assertValidVec(a);

    int len = v_len(a);
    vector *result = v_make(len);

    for (int i = 0; i < len; i++)
    {
        if (a[i] < minV)
        {
            result[i] = minV;
        }
        else if (a[i] > maxV)
        {
            result[i] = maxV;
        }
        else
        {
            result[i] = a[i];
        }
    }

    return result;
}

vector *v_minmaxscale(vector *a)
{
    assertValidVec(a);

    int len = v_len(a);
    vector *result = v_make(len);

    float min_val = a[0];
    float max_val = a[0];

    for (int i = 1; i < len; i++)
    {
        if (a[i] < min_val)
        {
            min_val = a[i];
        }
        if (a[i] > max_val)
        {
            max_val = a[i];
        }
    }

    float range = max_val - min_val;

    for (int i = 0; i < len; i++)
    {
        result[i] = (a[i] - min_val) / range;
    }

    return result;
}

vector *v_standardize(vector *a)
{
    assertValidVec(a);

    int len = v_len(a);
    vector *result = v_make(len);

    float mean = v_mean(a);
    float std_dev = v_std(a);

    for (int i = 0; i < len; i++)
    {
        result[i] = (a[i] - mean) / std_dev;
    }

    return result;
}
