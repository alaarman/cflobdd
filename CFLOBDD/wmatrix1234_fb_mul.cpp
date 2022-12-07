#include <cassert>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <utility>
#include <cstdarg>
#include <boost/random.hpp>
#include <boost/preprocessor/logical/xor.hpp>
#include "wmatrix1234_fb_mul.h"
#include "wmatrix1234_top_node_fb_mul.h"

namespace CFL_OBDD {


	namespace WeightedMatrix1234FloatBoostMul {

		void Matrix1234Initializer()
		{
			Matrix1234InitializerTop();
			return;
		}

		// Create representation of identity relation
		WEIGHTED_CFLOBDD_FLOAT_BOOST_MUL MkIdRelationInterleaved(unsigned int i)
		{
			// TODO: Check for error - "CodeConvert"
			WeightedCFLOBDDTopNodeFloatBoostRefPtr tmp = MkIdRelationInterleavedTop(i);
			return WEIGHTED_CFLOBDD_FLOAT_BOOST_MUL(tmp);
		}

		WEIGHTED_CFLOBDD_FLOAT_BOOST_MUL MkNegationMatrixInterleaved(unsigned int i)
		{
			return WEIGHTED_CFLOBDD_FLOAT_BOOST_MUL(MkNegationMatrixInterleavedTop(i));
		}

		// Create representation of the Walsh matrix W(2**(i-1))
		// [i.e., a matrix of size 2**(2**(i-1))) x 2**(2**(i-1)))]
		// with interleaved indexing of components: that is, input
		// (x0,y0,x1,y1,...,xN,yN) yields W[(x0,x1,...,xN)][(y0,y1,...,yN)]
		WEIGHTED_CFLOBDD_FLOAT_BOOST_MUL MkWalshInterleaved(unsigned int i)
		{
			return WEIGHTED_CFLOBDD_FLOAT_BOOST_MUL(MkWalshInterleavedTop(i));
		}

		WEIGHTED_CFLOBDD_FLOAT_BOOST_MUL MkCNOTInterleaved(unsigned int i)
		{
			return WEIGHTED_CFLOBDD_FLOAT_BOOST_MUL(MkCNOTInterleavedTop(i));
		}

		WEIGHTED_CFLOBDD_FLOAT_BOOST_MUL MkExchangeInterleaved(unsigned int i)
		{
			return WEIGHTED_CFLOBDD_FLOAT_BOOST_MUL(MkExchangeInterleavedTop(i));
		}

		// Return the Kronecker product of two matrices
		WEIGHTED_CFLOBDD_FLOAT_BOOST_MUL KroneckerProduct2Vocs(WEIGHTED_CFLOBDD_FLOAT_BOOST_MUL m1, WEIGHTED_CFLOBDD_FLOAT_BOOST_MUL m2)
		{
			return WEIGHTED_CFLOBDD_FLOAT_BOOST_MUL(KroneckerProduct2VocsTop(m1.root, m2.root)); 
		}

		// Naive matrix multiplication
		WEIGHTED_CFLOBDD_FLOAT_BOOST_MUL MatrixMultiplyV4(WEIGHTED_CFLOBDD_FLOAT_BOOST_MUL m1, WEIGHTED_CFLOBDD_FLOAT_BOOST_MUL m2)
		{
			return WEIGHTED_CFLOBDD_FLOAT_BOOST_MUL(MatrixMultiplyV4TopNode(m1.root, m2.root));
		}

        WEIGHTED_CFLOBDD_FLOAT_BOOST_MUL MkCNOT(unsigned int level, unsigned int n, long int controller, long int controlled){
			return WEIGHTED_CFLOBDD_FLOAT_BOOST_MUL(MkCNOTTopNode(level, n, controller, controlled));
		}

		void MatrixPrintRowMajor(WEIGHTED_CFLOBDD_FLOAT_BOOST_MUL c, std::ostream & out)
		{
			MatrixPrintRowMajorTop(c.root, out);
			return;
		}

		void MatrixPrintRowMajorInterleaved(WEIGHTED_CFLOBDD_FLOAT_BOOST_MUL c, std::ostream & out)
		{
			MatrixPrintRowMajorInterleavedTop(c.root, out);
			return;
		}
	}
}

