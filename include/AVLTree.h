#pragma once
// plik nagłówkowy dla drzewa AVL
class AVLTree {
public:
    AVLTree();
    ~AVLTree();
    void insert(int key, int value);
    void remove(int key);
    bool find(int key, int& value) const;
private:
    struct Node {
        int key, value, height;
        Node *left, *right;
        inline Node(int k, int v) : key(k), value(v), height(1), left(nullptr), right(nullptr) {}
    };
    Node* root;
    Node* insert(Node* node, int key, int value);
    Node* remove(Node* node, int key);
    void destroy(Node* node);
    int height(Node* node);
    int balance(Node* node);
    Node* rotateLeft(Node* y);
    Node* rotateRight(Node* x);
    Node* minValueNode(Node* node);
};
