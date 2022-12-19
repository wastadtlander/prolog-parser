#ifndef UNIFICATION_HPP
#define UNIFICATION_HPP

#include <unordered_map>

#include "ExpressionTree.hpp"

namespace vtpl {
    class Substitution {
        public:
            typedef std::unordered_multimap<ExpressionTreeNode, ExpressionTreeNode, ExpressionTreeNodeHasher> SubstitutionData;

            typedef typename SubstitutionData::iterator IteratorType;
            typedef typename SubstitutionData::const_iterator ConstIteratorType;

            // lookup an expression key in the substitution set and return a list of Expressions
            // the key maps to, or a list of size zero if no mapping exists
            std::list<ExpressionTreeNode> lookup(const ExpressionTreeNode& key) const;

            // insert a mapping from Expression key to Expression value, appending it if a mapping already exists.
            void insert(const ExpressionTreeNode& key, const ExpressionTreeNode& value);

            // return an iterator to the first element of the arbitrarily ordered set
            IteratorType begin();

            // return an iterator to one past the last element of the arbitrarily ordered set
            IteratorType end();
            
            // return a const iterator to the first element of the arbitrarily ordered set
            ConstIteratorType constBegin() const;

            // return an const iterator to one past the last element of the arbitrarily ordered set
            ConstIteratorType constEnd() const;
        private:
            SubstitutionData data;
    };

    struct UnificationResult{
        bool failed = false;
        Substitution substitution;
    };

    void unify(const ExpressionTreeNode& x, const ExpressionTreeNode& y, UnificationResult& subst);

    void unifyHelper(const ExpressionTreeNode& var, const ExpressionTreeNode& x, UnificationResult& subst);

    bool operator==(const Substitution s1, const Substitution s2);
}

#endif