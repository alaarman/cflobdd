#ifndef W_MATRIX1234_COMPLEX_FB_MUL_GUARD
#define W_MATRIX1234_COMPLEX_FB_MUL_GUARD


#include <iostream>
#include <fstream>
#include <random>
#include <boost/multiprecision/cpp_complex.hpp>
#include "weighted_cflobdd_t.h"


namespace CFL_OBDD {


	typedef boost::multiprecision::cpp_complex_double BIG_COMPLEX_FLOAT;
    // typedef double BIG_COMPLEX_FLOAT;
	typedef WEIGHTED_CFLOBDD_T<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>> WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL;

	namespace WeightedMatrix1234ComplexFloatBoostMul {
		// Initialization routine
		extern void Matrix1234Initializer();

		extern WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL MkIdRelationInterleaved(unsigned int i); // Representation of identity relation
		extern WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL MkWalshInterleaved(unsigned int i);              // Representation of Walsh matrix
		extern WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL MkNegationMatrixInterleaved(unsigned int i);
		extern WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL MkCNOTInterleaved(unsigned int i);
		extern WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL MkExchangeInterleaved(unsigned int i); // Representation of exchange matrix
		extern WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL MkCNOT(unsigned int level, unsigned int n, long int controller, long int controlled); // Representation of CNOT matrix with index1 as controller and index2 as controlled bits

		extern WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL KroneckerProduct2Vocs(WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL m1, WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL m2);

		extern void MatrixPrintRowMajor(WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL c, std::ostream & out);
		extern void MatrixPrintRowMajorInterleaved(WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL c, std::ostream & out);

		extern WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL MatrixMultiplyV4(WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL m1, WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL m2);
        extern WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL CreateBalancedFn(int n, std::mt19937 mt);

		extern WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL MkSwapGate(unsigned int i, long int c1, long int c2);
		extern WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL MkCPGate(unsigned int i, long int c1, long int c2, double theta);
	}
}

#endif

