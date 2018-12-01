#include <iostream>

using namespace std;

struct Node {
    int key;
    Node *left;
    Node *right;
    int height;
    int node_num;

    Node() : key(0), height(1), node_num(1) {
        left = nullptr;
        right = nullptr;
    }

    explicit Node(int key) : key(key), height(1), node_num(1) {
        left = nullptr;
        right = nullptr;
    }

    ~Node() = default;
};

template<class T>
class Stack {
private:
    int size;
    int capacity;

    void resize() {
        this->capacity *= 2;
        auto *tmp = new T[this->capacity];
        for (int i = 0; i < this->size; ++i) {
            tmp[i] = this->data[i];
        }

        delete[] data;
        this->data = tmp;
    }

    bool is_full() {
        return this->size == this->capacity;
    }

    T *data;

public:
    Stack() : size(0), capacity(8) {
        this->data = new T[this->capacity];
    }

    ~Stack() {
        delete[] this->data;
    }

    void Push(T elem) {
        if (is_full()) {
            resize();
        }
        this->size++;
        this->data[this->size - 1] = elem;
    }

    T Pop() {
        if (!IsEmpty()) {
            this->size--;
            return this->data[this->size];
        } else {
            return NULL;
        }
    }

    T Top() {
        return IsEmpty() ? NULL : data[size - 1];
    }

    bool IsEmpty() {
        return this->size == 0;
    }

};

template<class T>
class Queue {
private:
    Stack<T> st_in = Stack<T>();
    Stack<T> st_out = Stack<T>();

public:
    Queue() = default;

    void Push(T elem) {
        st_in.Push(elem);
    }

    T Pop() {
        if (!st_out.IsEmpty()) {
            return st_out.Pop();
        } else {
            while (!st_in.IsEmpty()) {
                st_out.Push(st_in.Pop());
            }
            return st_out.Pop();
        }
    }

    bool IsEmpty() {
        return (st_out.IsEmpty() && st_in.IsEmpty());
    }

    ~Queue() = default;
};

template<class T_Node, class Op>
void InOrderDFS(T_Node *node, Op func) {
    Stack<T_Node *> call_stack;
    T_Node *current_node = node;

    while (current_node != nullptr || !call_stack.IsEmpty()) {
        while (current_node != nullptr) {
            call_stack.Push(current_node);
            current_node = current_node->left;
        }
        current_node = call_stack.Pop();
        func(current_node);
        current_node = current_node->right;
    }
}

template<class T_Node, class Op>
void PostOrderDFS(T_Node *root, Op func) {
    Stack<T_Node *> stack1;
    Stack<T_Node *> stack2;
    stack1.Push(root);
    while (!stack1.IsEmpty()) {
        auto node = stack1.Pop();
        stack2.Push(node);
        if (node->left != nullptr)stack1.Push(node->left);
        if (node->right != nullptr)stack1.Push(node->right);
    }
    while (!stack2.IsEmpty()) {
        auto node = stack2.Pop();
        func(node);
    }
}

template<class T_Node, class Op>
void TraverseBFS(T_Node *root, Op func) {
    if (root == nullptr) {
        return;
    }
    Queue<T_Node *> traverse_queue;
    traverse_queue.Push(root);
    while (!traverse_queue.IsEmpty()) {
        T_Node *node = traverse_queue.Pop();
        func(node);
        if (node->left != nullptr)
            traverse_queue.Push(node->left);
        if (node->right != nullptr)
            traverse_queue.Push(node->right);
    }
}


int num_node(Node *node) {
    if (node) {
        return node->node_num;
    } else {
        return 0;
    }
}

int height(Node *node) {
    if (node) {
        return node->height;
    } else {
        return 0;
    }
}

int b_factor(Node *root) {
    return height(root->right) - height(root->left);
}

void fix(Node *node) {
    int height_left = height(node->left);
    int height_right = height(node->right);
    if (height_left > height_right) {
        node->height = height_left + 1;
    } else {
        node->height = height_right + 1;
    }

    int num_left = num_node(node->left);
    int num_right = num_node(node->right);
    node->node_num = num_left + num_right + 1;
}

Node *rotate_right(Node *root) {
    Node *node = root->left;
    root->left = node->right;
    node->right = root;
    fix(root);
    fix(node);
    return node;
}

Node *rotate_left(Node *root) {
    Node *node = root->right;
    root->right = node->left;
    node->left = root;
    fix(root);
    fix(node);
    return node;
}

Node *balance(Node *root) {
    fix(root);
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

Node *find_max(Node *root) {
    if (root->right) {
        return find_max(root->right);
    } else {
        return root;
    }
}

Node *remove_max(Node *root) {
    if (root->right == nullptr)
        return root;
    root->right = remove_max(root->right);
    return balance(root);
}


Node *Insert(Node *root, int key) {
    if (!root) return new Node(key);
    if (key < root->key) {
        root->left = Insert(root->left, key);
    } else {
        root->right = Insert(root->right, key);
    }
    return balance(root);
}

Node *Delete(Node *root, int key) {
    if (!root) return nullptr;
    if (key < root->key) {
        root->left = Delete(root->left, key);
    } else if (key > root->key) {
        root->right = Delete(root->right, key);
    } else if (key == root->key) {
        Node *root_l = root->left;
        Node *root_r = root->right;
        delete root;
        if (!root_l) return root_r;
        Node *max_node = find_max(root_r);
        max_node->left = remove_max(root_l);
        max_node->right = root_r;
        return balance(max_node);
    }
    return balance(root);
}

int k_stat(Node *_root, int k) {
    auto node = _root;
    while (node != nullptr) {
        int n_left = num_node(node->left);

        if (n_left == k) {
            return node->key;
        }

        if (n_left > k) {
            node = node->left;
        } else {
            node = node->right;
            k -= n_left + 1;
        }
    }
    return -1;
}

struct Operation {
    int node_v;
    int k_stat;
};

void perform_op(Operation op, Node *&tree) {
    if (op.node_v > 0) {
        tree = Insert(tree, op.node_v);
    } else if (op.node_v < 0) {
        tree = Delete(tree, abs(op.node_v));
    }

    cout << k_stat(tree, op.k_stat) << endl;
}

int main() {
    Node *tree = nullptr;
//    int n;
//    cin >> n;

    auto operation = Operation();
//    for (int i = 0; i < n; i++) {
    while (cin >> operation.node_v >> operation.k_stat) {
        perform_op(operation, tree);
    }
    return 0;
}


