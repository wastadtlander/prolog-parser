#ifndef EXPRESSION_TREE_HPP
#define EXPRESSION_TREE_HPP

#include <list>
#include <string>

namespace vtpl {
    enum class ExpressionTreeNodeType {
        NONE,
        ATOM,
        VARIABLE,
        COMPOUND
    };

    class ExpressionTreeNode {
        public:
            ExpressionTreeNode();

            ExpressionTreeNode(ExpressionTreeNodeType type, std::string value, std::list<ExpressionTreeNode> children);

            ExpressionTreeNode(const ExpressionTreeNode& etn);

            ExpressionTreeNode(ExpressionTreeNode&& etn);

            ExpressionTreeNode& operator=(const ExpressionTreeNode& etn);

            ExpressionTreeNode& operator=(ExpressionTreeNode&& etn);
            
            const std::string toString() const;
            
            ExpressionTreeNodeType type;
            std::string value;
            std::list<ExpressionTreeNode> children;
        private:
            void toStringHelper(std::list<ExpressionTreeNode> children, std::string &ret) const;
    };

    class ExpressionTreeNodeHasher {
        public:
            std::size_t operator()(const ExpressionTreeNode &node) const;
    };

    ExpressionTreeNode makeAtom(const std::string &value);

    ExpressionTreeNode makeVariable(const std::string &value);

    ExpressionTreeNode makeCompound(const std::string &value, const std::list<ExpressionTreeNode> &nodes);

    bool isNone(const ExpressionTreeNode &node);

    bool isAtom(const ExpressionTreeNode &node);

    bool isVariable(const ExpressionTreeNode &node);
    
    bool isCompound(const ExpressionTreeNode &node);

    bool isList(const ExpressionTreeNode &node);

    std::size_t arity(const ExpressionTreeNode &node);

    bool operator==(const ExpressionTreeNode &node1, const ExpressionTreeNode &node2);
}

#endif // EXPRESSION_TREE_HPP