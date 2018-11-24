//
// Created by daniknik on 24.11.18.
//
#include <iostream>

using namespace std;

struct Node {
    int key;
    Node *left;
    Node *right;
    int height;

    Node() : key(0), height(1) {
        left = nullptr;
        right = nullptr;
    }

    explicit Node(int key) : key(key), height(1) {
        left = nullptr;
        right = nullptr;
    }

    ~Node() = default;
};


class AVLTree {
private:
    Node *_root;

    int height(Node *root) {
        if (root != nullptr) {
            return root->height;
        } else {
            return 0;
        }
    }

    void fix_height(Node *root) {
        int height_left = height(root->left);
        int height_right = height(root->right);
        root->height = max(height_right, height_left) + 1;
    }

    int b_factor(Node *p) {
        return height(p->right) - height(p->left);
    }

    Node *rotate_right(Node *root) {
        Node *node = root->left;
        root->left = node->right;
        node->right = root;
        fix_height(root);
        fix_height(node);
        return node;
    }

    Node *rotate_left(Node *root) {
        Node *node = root->right;
        root->right = node->left;
        node->left = root;
        fix_height(root);
        fix_height(node);
        return node;
    }

    Node *balance(Node *root) {
        fix_height(root);
        if (b_factor(root) == 2) {
            if (b_factor(root->right) < 0)
                root->right = rotate_right(root->right);
            return rotate_left(root);
        }
        if (b_factor(root) == -2) {
            if (b_factor(root->left) > 0)
                root->left = rotate_left(root->left);
            return rotate_right(root);
        }
        return root;
    }

    Node *find_min(Node *root) {
        if (root->left != nullptr) {
            return find_min(root->left);
        } else {
            return root;
        }
    }

    Node *remove_min(Node *root) {
        if (root->left == nullptr) return root->right;
        root->left = remove_min(root->left);
        return balance(root);
    }

public:
    AVLTree() = default;

    ~AVLTree() = default;

    Node* root(){
        return _root;
    };

    Node *Insert(Node *root, int key) {
        if (root == nullptr) return new Node(key);
        if (key < root->key)
            root->left = Insert(root->left, key);
        else
            root->right = Insert(root->right, key);
        return balance(root);
    }

    Node *Remove(Node *root, int key) {
        if (root == nullptr) return nullptr;
        Node* node = root;
        if (key < root->key)
            root->left = Remove(root->left, key);
        else if (key > root->key)
            root->right = Remove(root->right, key);
        else {
            Node *root_l = root->left;
            Node *root_r = root->right;
            delete root;
            if (root_r == nullptr) return root_l;
            Node *min = find_min(root_r);
            min->right = remove_min(root_r);
            min->left = root_l;
            return balance(min);
        }
        return balance(root);
    }
};

int main(){
    return 0;
}