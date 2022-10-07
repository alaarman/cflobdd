//
//    Copyright (c) 2017 Thomas W. Reps
//    All Rights Reserved.
//
//    This software is furnished under a license and may be used and
//    copied only in accordance with the terms of such license and the
//    inclusion of the above copyright notice.  This software or any
//    other copies thereof or any derivative works may not be provided
//    or otherwise made available to any other person.  Title to and
//    ownership of the software and any derivative works is retained
//    by Thomas W. Reps.
//
//    THIS IMPLEMENTATION MAY HAVE BUGS, SOME OF WHICH MAY HAVE SERIOUS
//    CONSEQUENCES.  THOMAS W. REPS PROVIDES THIS SOFTWARE IN ITS "AS IS"
//    CONDITION, AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
//    BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
//    AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
//    THOMAS W. REPS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
//    TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#include <cassert>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdarg>
//#include <mpirxx.h>

#include "cflobdd_top_node_float_boost.h"
#include "cflobdd_node.h"

using namespace CFL_OBDD;

// TODO: uncomment ApplyAndReduce<BIG_FLOAT> and change to correct output

// FindOneSatisfyingAssignment
//
// If a satisfying assignment exists, allocate and place such an
//    assignment in variable "assignment" and return true.
// Otherwise return false.
//
// Running time: Linear in the number of variables
//
template <>
bool CFLOBDDTopNodeT<BIG_FLOAT>::FindOneSatisfyingAssignment(SH_OBDD::Assignment * &assignment)
{
	for (unsigned int i = 0; i < rootConnection.entryPointHandle->handleContents->numExits; i++) {
		unsigned int k = rootConnection.returnMapHandle.Lookup(i).convert_to<unsigned int>();
		if (k == 1) {  // A satisfying assignment must exist
			unsigned int size = 1 << CFLOBDDTopNodeT::maxLevel;
			assignment = new SH_OBDD::Assignment(size);
			rootConnection.entryPointHandle->handleContents->FillSatisfyingAssignment(i, *assignment, size);
			return true;
		}
	}
	return false;
}

#ifdef PATH_COUNTING_ENABLED
// NumSatisfyingAssignments
//
// Return the number of satisfying assignments
//
// Running time: Linear in the size of the CFLOBDDTopNode
//
template <>
unsigned int CFLOBDDTopNodeT<BIG_FLOAT>::NumSatisfyingAssignments()
{
	unsigned int ans = 0;

	for (unsigned int i = 0; i < rootConnection.entryPointHandle->handleContents->numExits; i++) {
		unsigned int k = rootConnection.returnMapHandle.Lookup(i).convert_to<unsigned int>();
		//unsigned int k = 1;
		if (k == 1) {
			ans += (int)rootConnection.entryPointHandle->handleContents->numPathsToExit[i];
		}
	}
	return ans;
}
#endif

/*
template <>
void CFLOBDDTopNodeT<BIG_FLOAT>::CountNodesAndEdges(Hashset<CFLOBDDNodeHandle> *visitedNodes, Hashset<CFLOBDDReturnMapBody> *visitedEdges, unsigned int &nodeCount, unsigned int &edgeCount)
{
rootConnection.entryPointHandle.handleContents->CountNodesAndEdges(visitedNodes, visitedEdges, nodeCount, edgeCount);
if (visitedEdges->Lookup(rootConnection.returnMapHandle.mapContents) == NULL) {
visitedEdges->Insert(rootConnection.returnMapHandle.mapContents);
edgeCount += rootConnection.returnMapHandle.Size();
}
}
*/

namespace CFL_OBDD {
	template class CFLOBDDTopNodeT<BIG_FLOAT>;
}

//********************************************************************
// CFLOBDDTopNode
//********************************************************************

namespace CFL_OBDD {

	// CFLOBDDTopNode-creation operations --------------------------------------

	// Create representation of \x.true
	CFLOBDDTopNodeFloatBoostRefPtr MkTrueFloatTop()
	{
		CFLOBDDTopNodeFloatBoostRefPtr v;
		FloatBoostReturnMapHandle m;

		m.AddToEnd(1);  // Map the only exit of the body to 1 (i.e., T)
		m.Canonicalize();
		v = new CFLOBDDFloatBoostTopNode(CFLOBDDNodeHandle::NoDistinctionNode[CFLOBDD_MAX_LEVEL], m);
		return v;
	}

// 	// Create representation of \x.false
// 	CFLOBDDTopNodeFloatBoostRefPtr MkFalseTop()
// 	{
// 		CFLOBDDTopNodeFloatBoostRefPtr v;
// 		FloatBoostReturnMapHandle m;

// 		m.AddToEnd(0);  // Map the only exit of the body to 0 (i.e., F)
// 		m.Canonicalize();
// 		v = new CFLOBDDFloatBoostTopNode(CFLOBDDNodeHandle::NoDistinctionNode[CFLOBDD_MAX_LEVEL], m);
// 		return v;
// 	}

// 	// Create representation of \x.x_i
// 	CFLOBDDTopNodeFloatBoostRefPtr MkDistinction(unsigned int i)
// 	{
// 		CFLOBDDTopNodeFloatBoostRefPtr v;
// 		CFLOBDDNodeHandle tempHandle;
// 		FloatBoostReturnMapHandle m;

// 		assert(i < (1 << CFLOBDD_MAX_LEVEL));   // i.e., i < 2**maxLevel

// 		tempHandle = MkDistinction(CFLOBDD_MAX_LEVEL, i);
// 		m.AddToEnd(0);
// 		m.AddToEnd(1);
// 		m.Canonicalize();
// 		v = new CFLOBDDFloatBoostTopNode(tempHandle, m);
// 		return v;
// 	}

// 	// Create representation of addition relation with interleaved variables
// 	// { (xi yi zi _)* | vec{x} + vec{y} = vec{z} }
// 	CFLOBDDTopNodeFloatBoostRefPtr MkAdditionInterleavedRecursiveTop()
// 	{
// 		CFLOBDDTopNodeFloatBoostRefPtr v;
// 		CFLOBDDNodeHandle n;
// 		FloatBoostReturnMapHandle m10;

// 		n = MkAdditionInterleavedRecursive(CFLOBDD_MAX_LEVEL, false);

// 		// Reduce n by mapping the "carry=0" and "carry=1" exits to accept
// 		FloatBoostReturnMapHandle retMapHandle;
// 		m10.AddToEnd(1);
// 		m10.AddToEnd(0);
// 		m10.Canonicalize();
// 		ReductionMapHandle reductionMapHandle;
// 		reductionMapHandle.AddToEnd(0);
// 		reductionMapHandle.AddToEnd(1);
// 		reductionMapHandle.AddToEnd(0);
// 		//CFLOBDDNodeHandle::InitReduceCache();
// 		CFLOBDDNodeHandle reduced_n = n.Reduce(reductionMapHandle, m10.Size());
// 		//CFLOBDDNodeHandle::DisposeOfReduceCache();

// 		// Create and return CFLOBDDTopNode
// 		v = new CFLOBDDFloatBoostTopNode(reduced_n, m10);
// 		return(v);
// 	}

// 	// Create representation of addition relation with interleaved variables
// 	// { (xi yi zi _)* | vec{x} + vec{y} = vec{z} }
// 	CFLOBDDTopNodeFloatBoostRefPtr MkAdditionInterleavedTop()
// 	{
// 		CFLOBDDTopNodeFloatBoostRefPtr v;
// 		CFLOBDDNodeHandle n;
// 		FloatBoostReturnMapHandle m10;

// 		n = CFLOBDDNodeHandle::AdditionInterleavedNoCarryNode[CFLOBDD_MAX_LEVEL - 2];

// 		// Reduce n by mapping the "carry=0" and "carry=1" exits to accept
// 		FloatBoostReturnMapHandle retMapHandle;
// 		m10.AddToEnd(1);
// 		m10.AddToEnd(0);
// 		m10.Canonicalize();
// 		ReductionMapHandle reductionMapHandle;
// 		reductionMapHandle.AddToEnd(0);
// 		reductionMapHandle.AddToEnd(1);
// 		reductionMapHandle.AddToEnd(0);
// 		//CFLOBDDNodeHandle::InitReduceCache();
// 		CFLOBDDNodeHandle reduced_n = n.Reduce(reductionMapHandle, m10.Size());
// 		//CFLOBDDNodeHandle::DisposeOfReduceCache();

// 		// Create and return CFLOBDDTopNode
// 		v = new CFLOBDDFloatBoostTopNode(reduced_n, m10);
// 		return(v);
// 	}

// 	// Create representation of parity function
// 	CFLOBDDTopNodeFloatBoostRefPtr MkParityTop()
// 	{
// 		CFLOBDDTopNodeFloatBoostRefPtr v;
// 		CFLOBDDNodeHandle tempHandle;
// 		FloatBoostReturnMapHandle m;

// 		tempHandle = MkParity(CFLOBDD_MAX_LEVEL);
// 		m.AddToEnd(0);
// 		m.AddToEnd(1);
// 		m.Canonicalize();
// 		v = new CFLOBDDFloatBoostTopNode(tempHandle, m);
// 		return v;
// 	}

// 	// Create representation of step function
// 	CFLOBDDTopNodeFloatBoostRefPtr MkStepUpOneFourthTop()
// 	{
// 		CFLOBDDTopNodeFloatBoostRefPtr v;
// 		CFLOBDDNodeHandle tempHandle;
// 		FloatBoostReturnMapHandle m;

// 		assert(CFLOBDD_MAX_LEVEL >= 1);
// 		tempHandle = MkStepOneFourth(CFLOBDD_MAX_LEVEL);
// 		m.AddToEnd(0);
// 		m.AddToEnd(1);
// 		m.Canonicalize();
// 		v = new CFLOBDDFloatBoostTopNode(tempHandle, m);
// 		return v;
// 	}

// 	// Create representation of step function
// 	CFLOBDDTopNodeFloatBoostRefPtr MkStepDownOneFourthTop()
// 	{
// 		return MkNot(MkStepUpOneFourthTop());
// 	}

// #ifdef ARBITRARY_STEP_FUNCTIONS
// 	// Create representation of step function
// 	CFLOBDDTopNodeFloatBoostRefPtr MkStepUpTop(unsigned int i)
// 	{
// 		CFLOBDDTopNodeFloatBoostRefPtr v;
// 		CFLOBDDNodeHandle tempHandle;
// 		FloatBoostReturnMapHandle m;

// 		assert(CFLOBDDTopNode::maxLevel <= 5);
// 		tempHandle = MkStepNode(CFLOBDDTopNode::maxLevel, i, 0, (1 << (1 << CFLOBDDTopNode::maxLevel)) - i);
// 		if (i != 0) {
// 			m.AddToEnd(0);
// 		}
// 		if (i != (1 << (1 << CFLOBDDTopNode::maxLevel))) {
// 			m.AddToEnd(1);
// 		}
// 		m.Canonicalize();
// 		v = new CFLOBDDFloatBoostTopNode(tempHandle, m);
// 		return v;
// 	}

// 	// Create representation of step function
// 	CFLOBDDTopNodeFloatBoostRefPtr MkStepDownTop(unsigned int i)
// 	{
// 		return MkNot(MkStepUpTop(i));
// 	}
// #endif

// 	CFLOBDDTopNodeFloatBoostRefPtr shiftAtoBTop(CFLOBDDTopNodeFloatBoostRefPtr f, const unsigned int levelAtWhichToShift)
// 	{
// 		CFLOBDDNodeHandle tempHandle = shiftAtoB(*(f->rootConnection.entryPointHandle), levelAtWhichToShift);
// 		CFLOBDDTopNodeFloatBoostRefPtr v = new CFLOBDDFloatBoostTopNode(tempHandle, f->rootConnection.returnMapHandle);

// 		return v;
// 	}

// 	CFLOBDDTopNodeFloatBoostRefPtr shiftBtoATop(CFLOBDDTopNodeFloatBoostRefPtr f, const unsigned int levelAtWhichToShift)
// 	{
// 		CFLOBDDNodeHandle tempHandle = shiftBtoA(*(f->rootConnection.entryPointHandle), levelAtWhichToShift);
// 		CFLOBDDTopNodeFloatBoostRefPtr v = new CFLOBDDFloatBoostTopNode(tempHandle, f->rootConnection.returnMapHandle);
// 		return v;
// 	}

// 	CFLOBDDTopNodeFloatBoostRefPtr shiftAtoBAtLevelOneTop(Hashset<CFLOBDDNode> *visitedNodes, unsigned int &totalVisitCount, unsigned int &redundantVisitCount, CFLOBDDTopNodeFloatBoostRefPtr f)
// 	{
// 		CFLOBDDNodeHandle tempHandle = shiftAtoBAtLevelOne(visitedNodes, totalVisitCount, redundantVisitCount, *(f->rootConnection.entryPointHandle));
// 		CFLOBDDTopNodeFloatBoostRefPtr v = new CFLOBDDFloatBoostTopNode(tempHandle, f->rootConnection.returnMapHandle);
// 		return v;
// 	}

// 	CFLOBDDTopNodeFloatBoostRefPtr shiftBtoAAtLevelOneTop(Hashset<CFLOBDDNode> *visitedNodes, unsigned int &totalVisitCount, unsigned int &redundantVisitCount, CFLOBDDTopNodeFloatBoostRefPtr f)
// 	{
// 		CFLOBDDNodeHandle tempHandle = shiftBtoAAtLevelOne(visitedNodes, totalVisitCount, redundantVisitCount, *(f->rootConnection.entryPointHandle));
// 		CFLOBDDTopNodeFloatBoostRefPtr v = new CFLOBDDFloatBoostTopNode(tempHandle, f->rootConnection.returnMapHandle);
// 		return v;
// 	}

// 	CFLOBDDTopNodeFloatBoostRefPtr duplicateAinBAtLevelOneTop(CFLOBDDTopNodeFloatBoostRefPtr f)
// 	{
// 		CFLOBDDNodeHandle tempHandle = duplicateAinBAtLevelOne(*(f->rootConnection.entryPointHandle));
// 		CFLOBDDTopNodeFloatBoostRefPtr v = new CFLOBDDFloatBoostTopNode(tempHandle, f->rootConnection.returnMapHandle);
// 		return v;
// 	}


// 	// Unary operations on CFLOBDDTopNodes --------------------------------------

// 	// Implements \f.!f
// 	CFLOBDDTopNodeFloatBoostRefPtr MkNot(CFLOBDDTopNodeFloatBoostRefPtr f)
// 	{
// 		CFLOBDDTopNodeFloatBoostRefPtr answer;
// 		FloatBoostReturnMapHandle m = f->rootConnection.returnMapHandle.Complement();
// 		answer = new CFLOBDDFloatBoostTopNode(*(f->rootConnection.entryPointHandle), m);
// 		return answer;
// 	}

// 	// Binary operations on CFLOBDDTopNodes ------------------------------------

// 	// \f.\g.(f && g)
// 	CFLOBDDTopNodeFloatBoostRefPtr MkAnd(CFLOBDDTopNodeFloatBoostRefPtr f, CFLOBDDTopNodeFloatBoostRefPtr g)
// 	{
// 		return f;// ApplyAndReduce<BIG_FLOAT>(f, g, andOp);
// 	}

// 	// \f.\g.!(f && g)
// 	CFLOBDDTopNodeFloatBoostRefPtr MkNand(CFLOBDDTopNodeFloatBoostRefPtr f, CFLOBDDTopNodeFloatBoostRefPtr g)
// 	{
// 		return f;// ApplyAndReduce<BIG_FLOAT>(f, g, nandOp);
// 	}

// 	// \f.\g.(f || g)
// 	CFLOBDDTopNodeFloatBoostRefPtr MkOr(CFLOBDDTopNodeFloatBoostRefPtr f, CFLOBDDTopNodeFloatBoostRefPtr g)
// 	{
// 		return f;// ApplyAndReduce<BIG_FLOAT>(f, g, orOp);
// 	}

// 	// \f.\g.!(f || g)
// 	CFLOBDDTopNodeFloatBoostRefPtr MkNor(CFLOBDDTopNodeFloatBoostRefPtr f, CFLOBDDTopNodeFloatBoostRefPtr g)
// 	{
// 		return f;// ApplyAndReduce<BIG_FLOAT>(f, g, norOp);
// 	}

// 	// \f.\g.(f == g)
// 	CFLOBDDTopNodeFloatBoostRefPtr MkIff(CFLOBDDTopNodeFloatBoostRefPtr f, CFLOBDDTopNodeFloatBoostRefPtr g)
// 	{
// 		return f;// ApplyAndReduce<BIG_FLOAT>(f, g, iffOp);
// 	}

// 	// \f.\g.(f != g)
// 	CFLOBDDTopNodeFloatBoostRefPtr MkExclusiveOr(CFLOBDDTopNodeFloatBoostRefPtr f, CFLOBDDTopNodeFloatBoostRefPtr g)
// 	{
// 		return f;// ApplyAndReduce<BIG_FLOAT>(f, g, exclusiveOrOp);
// 	}

// 	// \f.\g.(!f || g)
// 	CFLOBDDTopNodeFloatBoostRefPtr MkImplies(CFLOBDDTopNodeFloatBoostRefPtr f, CFLOBDDTopNodeFloatBoostRefPtr g)
// 	{
// 		return f;// ApplyAndReduce<BIG_FLOAT>(f, g, impliesOp);
// 	}

// 	// \f.\g.(f && !g)
// 	CFLOBDDTopNodeFloatBoostRefPtr MkMinus(CFLOBDDTopNodeFloatBoostRefPtr f, CFLOBDDTopNodeFloatBoostRefPtr g)
// 	{
// 		return f;// ApplyAndReduce<BIG_FLOAT>(f, g, minusOp);
// 	}

// 	// \f.\g.(!g || f)
// 	CFLOBDDTopNodeFloatBoostRefPtr MkQuotient(CFLOBDDTopNodeFloatBoostRefPtr f, CFLOBDDTopNodeFloatBoostRefPtr g)
// 	{
// 		return f;// ApplyAndReduce<BIG_FLOAT>(f, g, quotientOp);
// 	}

// 	// \f.\g.(g && !f)
// 	CFLOBDDTopNodeFloatBoostRefPtr MkNotQuotient(CFLOBDDTopNodeFloatBoostRefPtr f, CFLOBDDTopNodeFloatBoostRefPtr g)
// 	{
// 		return f;// ApplyAndReduce<BIG_FLOAT>(f, g, notQuotientOp);
// 	}

// 	// \f.\g.f
// 	CFLOBDDTopNodeFloatBoostRefPtr MkFirst(CFLOBDDTopNodeFloatBoostRefPtr f, CFLOBDDTopNodeFloatBoostRefPtr)
// 	{
// 		return f;
// 	}

// 	// \f.\g.!f
// 	CFLOBDDTopNodeFloatBoostRefPtr MkNotFirst(CFLOBDDTopNodeFloatBoostRefPtr f, CFLOBDDTopNodeFloatBoostRefPtr)
// 	{
// 		return MkNot(f);
// 	}

// 	// \f.\g.g
// 	CFLOBDDTopNodeFloatBoostRefPtr MkSecond(CFLOBDDTopNodeFloatBoostRefPtr, CFLOBDDTopNodeFloatBoostRefPtr g)
// 	{
// 		return g;
// 	}

// 	// \f.\g.!g
// 	CFLOBDDTopNodeFloatBoostRefPtr MkNotSecond(CFLOBDDTopNodeFloatBoostRefPtr, CFLOBDDTopNodeFloatBoostRefPtr g)
// 	{
// 		return MkNot(g);
// 	}

// 	/*
// 	// \f.\g.(f + g)
// 	CFLOBDDTopNodeFloatBoostRefPtr MkPlus(CFLOBDDTopNodeFloatBoostRefPtr f, CFLOBDDTopNodeFloatBoostRefPtr g)
// 	{
// 	return ApplyAndReduce<BIG_FLOAT>(f, g, PlusFunc);
// 	}

// 	// \f.\g.(f * g)
// 	CFLOBDDTopNodeFloatBoostRefPtr MkTimes(CFLOBDDTopNodeFloatBoostRefPtr f, CFLOBDDTopNodeFloatBoostRefPtr g)
// 	{
// 	return ApplyAndReduce<BIG_FLOAT>(f, g, TimesFunc);
// 	}
// 	*/

// 	// Ternary operations on CFLOBDDTopNodes ------------------------------------

// 	// \a.\b.\c.(a && b) || (!a && c)
// 	CFLOBDDTopNodeFloatBoostRefPtr MkIfThenElse(CFLOBDDTopNodeFloatBoostRefPtr f, CFLOBDDTopNodeFloatBoostRefPtr g, CFLOBDDTopNodeFloatBoostRefPtr h)
// 	{
// 		return f;// ApplyAndReduce<BIG_FLOAT>(f, g, h, ifThenElseOp);
// 	}

// 	// \a.\b.\c.(b && !a) || (c && !a) || (b && c)
// 	CFLOBDDTopNodeFloatBoostRefPtr MkNegMajority(CFLOBDDTopNodeFloatBoostRefPtr f, CFLOBDDTopNodeFloatBoostRefPtr g, CFLOBDDTopNodeFloatBoostRefPtr h)
// 	{
// 		return f;// ApplyAndReduce<BIG_FLOAT>(f, g, h, negMajorityOp);
// 	}

// 	CFLOBDDTopNodeFloatBoostRefPtr MkRestrict(CFLOBDDTopNodeFloatBoostRefPtr n, unsigned int i, bool val)
// 	{
// 		CFLOBDDReturnMapHandle MapHandle;
// 		CFLOBDDNodeHandle g = Restrict(*(n->rootConnection.entryPointHandle), i, val,
// 			MapHandle);

// 		// Create returnMapHandle from MapHandle
// 		FloatBoostReturnMapHandle returnMapHandle;
// 		unsigned MapSize = MapHandle.mapContents->mapArray.size();
// 		for (unsigned sBI = 0; sBI < MapSize; sBI++)
// 		{
// 			int d = MapHandle.mapContents->mapArray[sBI];
// 			BIG_FLOAT c = n->rootConnection.returnMapHandle.Lookup(d);
// 			returnMapHandle.AddToEnd(c);
// 		}
// 		returnMapHandle.Canonicalize();

// 		// Create and return CFLOBDDTopNode
// 		return(new CFLOBDDFloatBoostTopNode(g, returnMapHandle));
// 	}

// 	// Create representation of \f . exists x_i : f
// 	CFLOBDDTopNodeFloatBoostRefPtr MkExists(CFLOBDDTopNodeFloatBoostRefPtr f, unsigned int i)
// 	{
// 		CFLOBDDTopNodeFloatBoostRefPtr tempTrue = MkRestrict(f, i, true);
// 		CFLOBDDTopNodeFloatBoostRefPtr tempFalse = MkRestrict(f, i, false);
// 		return MkOr(tempTrue, tempFalse);
// 	}

// 	// Create representation of \f . forall x_i : f
// 	CFLOBDDTopNodeFloatBoostRefPtr MkForall(CFLOBDDTopNodeFloatBoostRefPtr f, unsigned int i)
// 	{
// 		CFLOBDDTopNodeFloatBoostRefPtr tempTrue = MkRestrict(f, i, true);
// 		CFLOBDDTopNodeFloatBoostRefPtr tempFalse = MkRestrict(f, i, false);
// 		return MkAnd(tempTrue, tempFalse);
// 	}

// 	CFLOBDDTopNodeFloatBoostRefPtr MkComposeTop(CFLOBDDTopNodeFloatBoostRefPtr f, int i, CFLOBDDTopNodeFloatBoostRefPtr g)              // \f. f | x_i = g
// 	{
// 		// DLC inefficient

// 		// Simple but slow method, see Bryant 1986 _GBAfBFM_:

// 		// f|x_i=g is g*(f|x_i=1) + (!g)*(f|x_i=0)
// 		// Better would be MkITE(g, f__x_i_1, f__x_i_0);

// 		CFLOBDDTopNodeFloatBoostRefPtr f__x_i_1 = MkRestrict(f, i, true);
// 		CFLOBDDTopNodeFloatBoostRefPtr f__x_i_0 = MkRestrict(f, i, false);
// 		CFLOBDDTopNodeFloatBoostRefPtr not_g = MkNot(g);

// 		CFLOBDDTopNodeFloatBoostRefPtr left_product = MkAnd(g, f__x_i_1);

// 		CFLOBDDTopNodeFloatBoostRefPtr right_product = MkAnd(not_g, f__x_i_0);

// 		CFLOBDDTopNodeFloatBoostRefPtr composition = MkOr(left_product, right_product);

// 		return composition;
// 	}

}
