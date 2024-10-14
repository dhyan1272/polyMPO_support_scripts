#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

// Function to calculate the dot product of two vectors
double dotProduct(const vector<double>& v1, const vector<double>& v2) {
    double result = 0.0;
    for (int i = 0; i < v1.size(); ++i) {
        result += v1[i] * v2[i];
    }
    return result;
}

// Function to calculate the norm of a vector
double norm(const vector<double>& v) {
    return sqrt(dotProduct(v, v));
}


std::vector<std::vector<double>> matrix_product(const std::vector<std::vector<double>>& A, const std::vector<std::vector<double>>& B) {
  int A_rows = A.size();
  int A_cols = A[0].size();
  int B_rows = B.size();
  int B_cols = B[0].size();
  // Ensure that the number of columns in A is equal to the number of rows in B
  if (A_cols != B_rows) {
     throw std::invalid_argument("Number of columns in A must be equal to the number of rows in B.");
  }
  std::vector<std::vector<double>> C(A_rows, std::vector<double>(B_cols, 0.0));
  for (int i = 0; i < A_rows; ++i) {
    for (int j = 0; j < B_cols; ++j) {
      C[i][j]=0;  
      for (int k = 0; k < B_rows; ++k) {
          C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
  return C; // Return the resulting matrix
}

// Function to perform QR decomposition
std::vector<std::vector<double>> getTranspose(const vector<vector<double>>& A) {
   int n=A.size();
   std::vector<std::vector<double>> C(n, std::vector<double>(n, 0.0));
   for (int i = 0; i < n; ++i) 
     for (int j = 0; j < n; ++j) 
       C[i][j]=A[j][i];
   return C;
} 

// Function to perform QR decomposition
void qrDecomposition(const vector<vector<double>>& A, vector<vector<double>>& Q, vector<vector<double>>& R) {
    int n = A.size();
    Q.resize(n, vector<double>(n));
    R.resize(n, vector<double>(n));

    for (int i = 0; i < n; ++i) {
        
        vector<double> A_column(n);
	vector<double> u_column(n);
        
	for (int l = 0; l < n; ++l){
              A_column[l] = A[l][i];
	      u_column[l] = A[l][i];  
	}

	for (int j = 0; j < i; ++j) {   
	   vector<double> q_column(n); 
           for (int l = 0; l < n; ++l) 
              q_column[l] = Q[l][j];
           double r = dotProduct(q_column, A_column);
           R[j][i] = r;
           for (int k = 0; k < n; ++k) {
              u_column[k] -= r * Q[k][j];
           }
        }

        double uNorm = norm(u_column);
        R[i][i] = uNorm;
        for (int k = 0; k < n; ++k) {
            Q[k][i] = u_column[k] / uNorm;
        }
    }
}

int main() {
    
    vector<vector<double>> A(4, vector<double>(4));
    A[0][0] = 1;//3.1153563917442517E-1;
    A[0][1] = 0.1;//-1.3830582025064361E-2;
    A[0][2] = 0.2;//-2.5538924485646350E-3;
    A[0][3] = 0.3;//1.8981316353936036E-3;

    A[1][0] = 0.1;//A[0][1];
    A[1][1] = 1;//6.3488037352529109E-4;
    A[1][2] = 0.1;//1.1655718251858709E-4;
    A[1][3] = 0.1;//-9.5296099755651660E-5;

    A[2][0] =  0.1;//A[0][2];
    A[2][1] =  0.1;//A[1][2];
    A[2][2] =  1;//2.1420082841408966E-5;
    A[2][3] =  0.1;//-1.7139152480597098E-5;
    
    A[3][0] =  0.1;//A[0][3];
    A[3][1] =  0.1;//A[1][3];
    A[3][2] =  0.1;//A[2][3];
    A[3][3] =  1;//6.4998618206511742E-5;

    vector<vector<double>> Q, R;
    auto A_transpose=getTranspose(A);
    qrDecomposition(A, Q, R);

    // Print A, Q, R, A_reconstructed matrices
    cout << "A:" << endl;
    for (const auto& row : A) {
        for (double val : row) {
            cout << val << " ";
        }
        cout << endl;
    }

    cout << "Q:" << endl; 
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            cout << Q[i][j] << " ";
        }
        cout << endl;
    }

    cout << "R:" << endl;
    for (const auto& row : R) {
        for (double val : row) {
            cout << val << " ";
        }
        cout << endl;
    }

    auto A_r=matrix_product(Q,R);
    
    cout << "A_reconstructed:" << endl;
    for (const auto& row : A_r) {
        for (double val : row) {
            printf("%.5e:   ",val);
        }
        cout << endl;
    }

    return 0;
}

