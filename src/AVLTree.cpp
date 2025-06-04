#include "../include/AVLTree.h"
#include <algorithm>

AVLTree::AVLTree() : root(nullptr) {}
AVLTree::~AVLTree() { destroy(root); }

// Rekurencyjne usuwanie całego drzewa
void AVLTree::destroy(Node* node) {
    if (!node) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

// Zwraca wysokość węzła
int AVLTree::height(Node* node) { return node ? node->height : 0; }
// Oblicza balans węzła
int AVLTree::balance(Node* node) { return node ? height(node->left) - height(node->right) : 0; }

// Rotacja w lewo
AVLTree::Node* AVLTree::rotateLeft(Node* y) {
    Node* x = y->right;
    Node* T2 = x->left;
    x->left = y;
    y->right = T2;
    y->height = std::max(height(y->left), height(y->right)) + 1;
    x->height = std::max(height(x->left), height(x->right)) + 1;
    return x;
}

// Rotacja w prawo
AVLTree::Node* AVLTree::rotateRight(Node* x) {
    Node* y = x->left;
    Node* T2 = y->right;
    y->right = x;
    x->left = T2;
    x->height = std::max(height(x->left), height(x->right)) + 1;
    y->height = std::max(height(y->left), height(y->right)) + 1;
    return y;
}

// Znajdź węzeł o najmniejszym kluczu
AVLTree::Node* AVLTree::minValueNode(Node* node) {
    Node* current = node;
    while (current && current->left)
        current = current->left;
    return current;
}

// Rekurencyjne wstawianie do drzewa AVL
AVLTree::Node* AVLTree::insert(Node* node, int key, int value) {
    if (!node) return new Node(key, value);
    if (key < node->key)
        node->left = insert(node->left, key, value);
    else if (key > node->key)
        node->right = insert(node->right, key, value);
    else {
        node->value = value;
        return node;
    }
    node->height = 1 + std::max(height(node->left), height(node->right));
    int bal = balance(node);
    // Rotacje dla zachowania balansu
    if (bal > 1 && key < node->left->key)
        return rotateRight(node);
    if (bal < -1 && key > node->right->key)
        return rotateLeft(node);
    if (bal > 1 && key > node->left->key) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (bal < -1 && key < node->right->key) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}

// Wstawianie do drzewa AVL
void AVLTree::insert(int key, int value) {
    root = insert(root, key, value);
}

// Rekurencyjne usuwanie z drzewa AVL
AVLTree::Node* AVLTree::remove(Node* node, int key) {
    if (!node) return node;
    if (key < node->key)
        node->left = remove(node->left, key);
    else if (key > node->key)
        node->right = remove(node->right, key);
    else {
        // Usuwanie węzła
        if (!node->left || !node->right) {
            Node* temp = node->left ? node->left : node->right;
            if (!temp) {
                temp = node;
                node = nullptr;
            } else {
                *node = *temp;
            }
            delete temp;
        } else {
            Node* temp = minValueNode(node->right);
            node->key = temp->key;
            node->value = temp->value;
            node->right = remove(node->right, temp->key);
        }
    }
    if (!node) return node;
    node->height = 1 + std::max(height(node->left), height(node->right));
    int bal = balance(node);
    // Rotacje po usunięciu
    if (bal > 1 && balance(node->left) >= 0)
        return rotateRight(node);
    if (bal > 1 && balance(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (bal < -1 && balance(node->right) <= 0)
        return rotateLeft(node);
    if (bal < -1 && balance(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}

// Usuwanie z drzewa AVL
void AVLTree::remove(int key) {
    root = remove(root, key);
}
// wyszukiwanie klucza
bool AVLTree::find(int key, int& value) const {
    Node* current = root;
    while (current) {
        if (key == current->key) {
            value = current->value;
            return true;
        } else if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return false;
}
