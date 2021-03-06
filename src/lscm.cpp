#include "lscm.h"
#include "vector_area_matrix.h"
#include <igl/cotmatrix.h>
#include <igl/massmatrix.h>
#include <igl/min_quad_with_fixed.h>
#include <igl/repdiag.h>
#include <Eigen/SVD>
#include <igl/eigs.h>
#include <iostream>

void lscm(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  Eigen::MatrixXd & U)
{
  //Replace with your code
  //U = V.leftCols(2);

  Eigen::SparseMatrix<double> L, M, Q, M_in, Aeq, I, A, B;
  Eigen::MatrixXd sU;
  Eigen::VectorXd sS;

  igl::cotmatrix(V, F, L);
  vector_area_matrix(F, A);
  igl::massmatrix(V, F, igl::MassMatrixType::MASSMATRIX_TYPE_VORONOI, M);

  //tranform L to 
  //L 0
  //0 L
  igl::repdiag(L,2,Q);

  //same for M
  igl::repdiag(M,2,B);

  //Move 1/2  out
  Q = Q - 2 * A;
  
  //Generalized Eigenvalue problem
  igl::eigs(Q,B,3,igl::EigsType::EIGS_TYPE_SM, sU, sS);
  U.resize(V.rows(), 2);
  U.col(0) = sU.col(2).topRows(V.rows());
  U.col(1) = sU.col(2).bottomRows(V.rows());

  Eigen::JacobiSVD<Eigen::MatrixXd> svd(U.transpose() * U, Eigen::ComputeThinU | Eigen::ComputeThinV);
  Eigen::MatrixXd u = svd.matrixU();
  U = U*u;
}
