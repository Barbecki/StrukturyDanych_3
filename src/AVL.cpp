#include "../include/AVL.hpp"

// Implementacja AVLTree

template<typename Key, typename Value>
AVLTree<Key, Value>::AVLTree() : root(nullptr) {}

template<typename Key, typename Value>
AVLTree<Key, Value>::~AVLTree() { destroy(root); }

template<typename Key, typename Value>
void AVLTree<Key, Value>::destroy(Node* node) {
    if (!node) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

template<typename Key, typename Value>
int AVLTree<Key, Value>::height(Node* node) const { return node ? node->height : 0; }

template<typename Key, typename Value>
int AVLTree<Key, Value>::balanceFactor(Node* node) const { return height(node->left) - height(node->right); }

template<typename Key, typename Value>
void AVLTree<Key, Value>::updateHeight(Node* node) {
    int hl = height(node->left);
    int hr = height(node->right);
    node->height = 1 + (hl > hr ? hl : hr);
}

template<typename Key, typename Value>
void AVLTree<Key, Value>::rotateLeft(Node*& node) {
    Node* r = node->right;
    node->right = r->left;
    updateHeight(node);
    r->left = node;
    updateHeight(r);
    node = r;
}

template<typename Key, typename Value>
void AVLTree<Key, Value>::rotateRight(Node*& node) {
    Node* l = node->left;
    node->left = l->right;
    updateHeight(node);
    l->right = node;
    updateHeight(l);
    node = l;
}

template<typename Key, typename Value>
void AVLTree<Key, Value>::balance(Node*& node) {
    updateHeight(node);
    int bf = balanceFactor(node);
    if (bf > 1) {
        if (balanceFactor(node->left) < 0)
            rotateLeft(node->left);
        rotateRight(node);
    } else if (bf < -1) {
        if (balanceFactor(node->right) > 0)
            rotateRight(node->right);
        rotateLeft(node);
    }
}

template<typename Key, typename Value>
void AVLTree<Key, Value>::insert(const Key& key, const Value& value) { insert(root, key, value); }

template<typename Key, typename Value>
void AVLTree<Key, Value>::insert(Node*& node, const Key& key, const Value& value) {
    if (!node) { node = new Node(key, value); return; }
    if (key < node->key)
        insert(node->left, key, value);
    else if (key > node->key)
        insert(node->right, key, value);
    else
        node->value = value;
    balance(node);
}

template<typename Key, typename Value>
void AVLTree<Key, Value>::remove(const Key& key) { remove(root, key); }

template<typename Key, typename Value>
void AVLTree<Key, Value>::remove(Node*& node, const Key& key) {
    if (!node) return;
    if (key < node->key)
        remove(node->left, key);
    else if (key > node->key)
        remove(node->right, key);
    else {
        if (!node->left) {
            Node* tmp = node;
            node = node->right;
            delete tmp;
        } else if (!node->right) {
            Node* tmp = node;
            node = node->left;
            delete tmp;
        } else {
            Node* minLarger = node->right;
            while (minLarger->left) minLarger = minLarger->left;
            node->key = minLarger->key;
            node->value = minLarger->value;
            remove(node->right, minLarger->key);
        }
    }
    if (node) balance(node);
}

template<typename Key, typename Value>
bool AVLTree<Key, Value>::find(const Key& key, Value& out) const {
    Node* n = find(root, key);
    if (n) { out = n->value; return true; }
    return false;
}

template<typename Key, typename Value>
typename AVLTree<Key, Value>::Node* AVLTree<Key, Value>::find(Node* node, const Key& key) const {
    if (!node) return nullptr;
    if (key < node->key)
        return find(node->left, key);
    else if (key > node->key)
        return find(node->right, key);
    else
        return node;
}
