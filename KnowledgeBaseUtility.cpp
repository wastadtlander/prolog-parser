#include "KnowledgeBaseUtility.hpp"

#include <atomic>

namespace vtpl {
    ExpressionTreeNode apply(const ExpressionTreeNode & t, const Substitution & sub) {
        ExpressionTreeNode etn = t;

        applyHelper(etn, sub);

        return etn == t ? t : etn;
    }

    void applyHelper(ExpressionTreeNode & t, const Substitution & sub) {
        if (t.type == ExpressionTreeNodeType::VARIABLE) {
            std::list<ExpressionTreeNode> l = sub.lookup(t);

            if (!l.empty()) {
                t = l.front();
            }
        }

        auto it = t.children.begin();

        while (it != t.children.end()) {
            applyHelper(*it, sub);
            it++;
        }
    }

    static int count = 1;
    std::atomic<int> atomic_int(count);

    Clause apart(const Clause & clause) {
        Clause c = clause;

        std::unordered_map<std::string, std::string> umap;

        apartHelper(c.head, umap, false);
        apartHelper(c.body, umap, true);

        return c;
    }

    void apartHelper(ExpressionTreeNode & etn, std::unordered_map<std::string, std::string> & umap, bool readOrWrite) {
        if (readOrWrite) {
            if (etn.type == ExpressionTreeNodeType::VARIABLE) {
                if (umap.find(etn.value) != umap.end()) {
                    etn.value = umap[etn.value];
                }
            }

            auto it = etn.children.begin();

            while (it != etn.children.end()) {
                apartHelper(*it, umap, readOrWrite);
                it++;
            }
        } else {
            if (etn.type == ExpressionTreeNodeType::VARIABLE) {
                umap[etn.value] = etn.value + "_" + std::to_string(atomic_int++);
                etn.value = umap[etn.value];
            }

            auto it = etn.children.begin();

            while (it != etn.children.end()) {
                apartHelper(*it, umap, readOrWrite);
                it++;
            }
        }
    }

    Substitution compose(const Substitution & sub1, const Substitution & sub2) {
        Substitution result;

        Substitution::ConstIteratorType it1 = sub1.constBegin();
        while (it1 != sub1.constEnd()) {
            std::list<ExpressionTreeNode> l1 = sub2.lookup(it1->second);
            if (l1.empty()) {
                result.insert(it1->first, it1->second);
            } else {
                std::list<ExpressionTreeNode>::const_iterator it2 = l1.begin();
                while (it2 != l1.end()) {
                    result.insert(it1->first, *it2);
                    it2++;
                }
            }
            it1++;
        }

        it1 = sub2.constBegin();
        while (it1 != sub2.constEnd()) {
            result.insert(it1->first, it1->second);
            it1++;
        }
        
        return result;
    }
}