#include <math.h>
#include <stdlib.h>

typedef struct {
  unsigned rows;
  unsigned columns;
  double **data;
} Matrix;

void destroyMatrix(Matrix *matrix);
bool isValidMatrix(const Matrix *matrix);
double calculateMatrixTrace(const Matrix *matrix);
double calculateMatrixDeterminant(Matrix *matrix);
Matrix *createIdentityMatrix(unsigned n);
Matrix *createZeroMatrix(unsigned rows, unsigned columns);
Matrix *copyMatrix(const Matrix *matrix);
Matrix *transposeMatrix(const Matrix *matrix);
Matrix *powerMatrix(const Matrix *matrix, unsigned k);
Matrix *addMatrices(const Matrix *A, const Matrix *B);
Matrix *multiplyMatrices(const Matrix *A, const Matrix *B);

int main();



void destroyMatrix(Matrix *matrix) {
  if (!matrix) return;
  if (matrix->data) {
    for (unsigned i = 0; i < matrix->rows; i++) free(matrix->data[i]);
    free(matrix->data);
  }
  free(matrix);
}

bool isValidMatrix(const Matrix *matrix) {
  if (!matrix || (matrix->rows > 0 && !matrix->data)) return false;
  if (matrix->columns > 0)
    for (unsigned i = 0; i < matrix->rows; i++)
      if (!matrix->data[i])
        return false;
  return true;
}

double calculateMatrixTrace(const Matrix *matrix) {
  if (!isValidMatrix(matrix) || matrix->rows != matrix->columns) return 0;
  double trace = 0;
  for (unsigned i = 0; i < matrix->rows; i++) trace += matrix->data[i][i];
  return trace;
}

double calculateMatrixDeterminant(Matrix *matrix) {
  if (!isValidMatrix(matrix) || matrix->rows != matrix->columns) return 1;
  double determinant = 1;
  for (unsigned i = 0; i < matrix->rows; i++) {
    unsigned pivot = i;
    for (unsigned j = i + 1; j < matrix->rows; j++)
      if (fabs(matrix->data[j][i]) > fabs(matrix->data[pivot][i]))
        pivot = j;
    if (fabs(matrix->data[pivot][i]) < 1e-9) return 0;
    if (pivot != i) {
      double *temporary = matrix->data[i];
      matrix->data[i] = matrix->data[pivot];
      matrix->data[pivot] = temporary;
      determinant *= -1;
    }
    determinant *= matrix->data[i][i];
    for (unsigned j = i + 1; j < matrix->rows; ++j) {
      double factor = matrix->data[j][i] / matrix->data[i][i];
      for (unsigned k = i; k < matrix->rows; ++k) matrix->data[j][k] -= factor * matrix->data[i][k];
    }
  }
  return determinant;
}

[[nodiscard]] Matrix *createIdentityMatrix(unsigned n) {
  Matrix *identity = createZeroMatrix(n, n);
  if (!identity) return nullptr;
  for (unsigned i = 0; i < n; i++) identity->data[i][i] = 1;
  return identity;
}

[[nodiscard]] Matrix *createZeroMatrix(unsigned rows, unsigned columns) {
  Matrix *matrix = malloc(sizeof(Matrix));
  if (!matrix) return nullptr;
  matrix->rows = rows;
  matrix->columns = columns;
  matrix->data = malloc(rows * sizeof(double *));
  if (rows > 0 && !matrix->data) {
    free(matrix);
    return nullptr;
  }
  for (unsigned i = 0; i < rows; ++i) {
    matrix->data[i] = calloc(columns, sizeof(double));
    if (columns > 0 && !matrix->data[i]) {
      for (unsigned j = 0; j < i; ++j) free(matrix->data[j]);
      free(matrix->data);
      free(matrix);
      return nullptr;
    }
  }
  return matrix;
}

[[nodiscard]] Matrix *copyMatrix(const Matrix *matrix) {
  if (!isValidMatrix(matrix)) return nullptr;
  Matrix *copy = createZeroMatrix(matrix->rows, matrix->columns);
  if (!copy) return nullptr;
  for (unsigned i = 0; i < matrix->rows; i++)
    for (unsigned j = 0; j < matrix->columns; j++)
      copy->data[i][j] = matrix->data[i][j];
  return copy;
}

[[nodiscard]] Matrix *transposeMatrix(const Matrix *matrix) {
  if (!isValidMatrix(matrix)) return nullptr;
  Matrix *transpose = createZeroMatrix(matrix->columns, matrix->rows);
  if (!transpose) return nullptr;
  for (unsigned i = 0; i < matrix->rows; i++)
    for (unsigned j = 0; j < matrix->columns; j++)
      transpose->data[j][i] = matrix->data[i][j];
  return transpose;
}

[[nodiscard]] Matrix *powerMatrix(const Matrix *matrix, unsigned k) {
  if (!matrix || matrix->rows != matrix->columns) return nullptr;
  Matrix *power = createIdentityMatrix(matrix->rows);
  Matrix *base = copyMatrix(matrix);
  while (k > 0) {
    if (k % 2 == 1) {
      Matrix *multiplication = multiplyMatrices(power, base);
      destroyMatrix(power);
      power = multiplication;
    }
    Matrix *square = multiplyMatrices(base, base);
    destroyMatrix(base);
    base = square;
    k /= 2;
  }
  destroyMatrix(base);
  return power;
}

[[nodiscard]] Matrix *addMatrices(const Matrix *A, const Matrix *B) {
  if (!isValidMatrix(A) || !isValidMatrix(B) || A->rows != B->rows || A->columns != B->columns) return nullptr;
  Matrix *C = createZeroMatrix(A->rows, A->columns);
  if (!C) return nullptr;
  for (unsigned i = 0; i < A->rows; i++)
    for (unsigned j = 0; j < A->columns; j++)
      C->data[i][j] = A->data[i][j] + B->data[i][j];
  return C;
}

[[nodiscard]] Matrix *multiplyMatrices(const Matrix *A, const Matrix *B) {
  if (!isValidMatrix(A) || !isValidMatrix(B) || A->columns != B->rows) return nullptr;
  Matrix *C = createZeroMatrix(A->rows, B->columns);
  if (!C) return nullptr;
  for (unsigned i = 0; i < A->rows; i++)
    for (unsigned j = 0; j < B->columns; j++)
      for (unsigned k = 0; k < A->columns; k++)
        C->data[i][j] += A->data[i][k] * B->data[k][j];
  return C;
}



int main() {
  return 0;
}
