/**
 * @file mult_div_update_rules.hpp
 * @author Mohan Rajendran
 *
 * Update rules for the Non-negative Matrix Factorization. This follows a method
 * described in the paper 'Algorithms for Non-negative Matrix Factorization'
 * by D. D. Lee and H. S. Seung. This is a multiplicative rule that ensures
 * that the Kullback–Leibler divergence
 * \f$ \sum_i \sum_j (V_{ij} log\frac{V_{ij}}{(WH)_{ij}}-V_{ij}+(WH)_{ij}) \f$is
 * non-increasing between subsequent iterations. Both of the update rules
 * for W and H are defined in this file.
 */
#ifndef __MLPACK_METHODS_LMF_UPDATE_RULES_NMF_MULT_DIV_HPP
#define __MLPACK_METHODS_LMF_UPDATE_RULES_NMF_MULT_DIV_HPP

#include <mlpack/core.hpp>

namespace mlpack {
namespace amf {


class NMFMultiplicativeDivergenceUpdate
{
 public:
  // Empty constructor required for the WUpdateRule template.
  NMFMultiplicativeDivergenceUpdate() { }

  /**
   * The update rule for the basis matrix W. The formula used is
   * \f[
   * W_{ia} \leftarrow W_{ia} \frac{\sum_{\mu} H_{a\mu} V_{i\mu}/(WH)_{i\mu}}
   * {\sum_{\nu} H_{a\nu}}
   * \f]
   * The function takes in all the matrices and only changes the 
   * value of the W matrix.
   *
   * @param V Input matrix to be factorized.
   * @param W Basis matrix to be updated.
   * @param H Encoding matrix.
   */
  template<typename MatType>
  inline static void WUpdate(const MatType& V,
                            arma::mat& W,
                            const arma::mat& H)
  {
    // Simple implementation left in the header file.
    arma::mat t1;
    arma::rowvec t2;

    t1 = W * H;
    for (size_t i = 0; i < W.n_rows; ++i)
    {
      for (size_t j = 0; j < W.n_cols; ++j)
      {
        // Writing this as a single expression does not work as of Armadillo
        // 3.920.  This should be fixed in a future release, and then the code
        // below can be fixed.
        //t2 = H.row(j) % V.row(i) / t1.row(i);
        t2.set_size(H.n_cols);
        for (size_t k = 0; k < t2.n_elem; ++k)
        {
          t2(k) = H(j, k) * V(i, k) / t1(i, k);
        }

        W(i, j) = W(i, j) * sum(t2) / sum(H.row(j));
      }
    }
  }

  /**
   * The update rule for the encoding matrix H. The formula used is
   * \f[
   * H_{a\mu} \leftarrow H_{a\mu} \frac{\sum_{i} W_{ia} V_{i\mu}/(WH)_{i\mu}}
   * {\sum_{k} H_{ka}}
   * \f]
   * The function takes in all the matrices and only changes the value 
   * of the H matrix.
   *
   * @param V Input matrix to be factorized.
   * @param W Basis matrix.
   * @param H Encoding matrix to updated.
   */
  template<typename MatType>
  inline static void HUpdate(const MatType& V,
                            const arma::mat& W,
                            arma::mat& H)
  {
    // Simple implementation left in the header file.
    arma::mat t1;
    arma::colvec t2;

    t1 = W * H;
    for (size_t i = 0; i < H.n_rows; i++)
    {
      for (size_t j = 0; j < H.n_cols; j++)
      {
        // Writing this as a single expression does not work as of Armadillo
        // 3.920.  This should be fixed in a future release, and then the code
        // below can be fixed.
        //t2 = W.col(i) % V.col(j) / t1.col(j);
        t2.set_size(W.n_rows);
        for (size_t k = 0; k < t2.n_elem; ++k)
        {
          t2(k) = W(k, i) * V(k, j) / t1(k, j);
        }

        H(i,j) = H(i,j) * sum(t2) / sum(W.col(i));
      }
    }
  }
};

}; // namespace amf
}; // namespace mlpack

#endif