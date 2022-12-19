#include "Unification.hpp"

#include <algorithm>

namespace vtpl {
    std::list<ExpressionTreeNode> Substitution::lookup(const ExpressionTreeNode& key) const {
        if (data.find(key) != data.end()) {
            std::list<ExpressionTreeNode> l;
            auto range = data.equal_range(key);
            for (auto it = range.first; it != range.second; ++it) {
                l.push_back(it->second);
            }
            return l;
        }
        return std::list<ExpressionTreeNode>();
    }

    void Substitution::insert(const ExpressionTreeNode& key, const ExpressionTreeNode& value) {
        data.emplace(std::make_pair(key, value));
    }

    Substitution::IteratorType Substitution::begin() {
        return data.begin();
    }

    Substitution::IteratorType Substitution::end() {
        return data.end();
    }

    Substitution::ConstIteratorType Substitution::constBegin() const {
        return data.cbegin();
    }

    Substitution::ConstIteratorType Substitution::constEnd() const {
        return data.cend();
    }

    void unify(const ExpressionTreeNode& x, const ExpressionTreeNode& y, UnificationResult& subst) {
        if (subst.failed) {
            return;
        } else if (x == y) {
            return;
        } else if (isVariable(x)) {
            unifyHelper(x, y, subst);
            return;
        } else if (isVariable(y)) {
            unifyHelper(y, x, subst);
            return;
        } else if (isCompound(x) && isCompound(y)) {
            if ((x.value != y.value) || (x.children.size() != y.children.size())) {
                subst.failed = true;
                return;
            }

            auto xIt = x.children.begin();
            auto yIt = y.children.begin();

            auto xEnd = x.children.end();
            auto yEnd = y.children.end();

            for (; xIt != xEnd && yIt != yEnd; ++xIt, ++yIt) {
                unify(*xIt, *yIt, subst);
            }

            return;
        } else if (isList(x) && isList(y)) {
            if (x.children.size() != y.children.size()) {
                subst.failed = true;
                return;
            }

            auto xIt = x.children.begin();
            auto yIt = y.children.begin();

            auto xEnd = x.children.end();
            auto yEnd = y.children.end();

            for (; xIt != xEnd && yIt != yEnd; ++xIt, ++yIt) {
                unify(*xIt, *yIt, subst);
            }

            return;
        } else {
            subst.failed = true;
            return;
        }
    }

    void unifyHelper(const ExpressionTreeNode& var, const ExpressionTreeNode& x, UnificationResult& subst) {
        std::list<ExpressionTreeNode> lookupListVar = subst.substitution.lookup(var);
        std::list<ExpressionTreeNode> lookupListX = subst.substitution.lookup(x);
        if (!lookupListVar.empty()) {
            unify(lookupListVar.front(), x, subst);
            return;
        } else if (!lookupListX.empty()) { 
            unify(var, lookupListX.front(), subst);
            return;
        } else {
            subst.substitution.insert(var, x);
            return;
        }
    }

    bool operator==(const Substitution s1, const Substitution s2) {
        Substitution::SubstitutionData sd1(s1.constBegin(), s1.constEnd());
        Substitution::SubstitutionData sd2(s2.constBegin(), s2.constEnd()); 
        return sd1 == sd2;
    }
}