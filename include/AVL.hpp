#pragma once
// Prosty szkielet AVLTree do użycia jako kubełek

template<typename Key, typename Value>
class AVLTree {
public:
    AVLTree();
    ~AVLTree();
    void insert(const Key& key, const Value& value);
    void remove(const Key& key);
    bool find(const Key& key, Value& out) const;
private:
    struct Node {
        Key key;
        Value value;
        Node* left;
        Node* right;
        int height;
        Node(const Key& k, const Value& v) : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
    };
    Node* root;
    int height(Node* node) const;
    int balanceFactor(Node* node) const;
    void updateHeight(Node* node);
    void rotateLeft(Node*& node);
    void rotateRight(Node*& node);
    void balance(Node*& node);
    void insert(Node*& node, const Key& key, const Value& value);
    void remove(Node*& node, const Key& key);
    Node* find(Node* node, const Key& key) const;
    void destroy(Node* node);
};
