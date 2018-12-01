//
// Дано число N и N строк.
// Каждая строка содержащит команду добавления или удаления натуральных чисел, а также запрос на получение k-ой порядковой статистики.
// Команда добавления числа A задается положительным числом A, команда удаления числа A задается отрицательным числом “-A”.
// Запрос на получение k-ой порядковой статистики задается числом k.
// Требуемая скорость выполнения запроса - O(log n).
//
#include <iostream>
#include <stack>

using namespace std;

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
        if (!IsEmpty()) {
            return data[size - 1];
        } else {
            return NULL;
        }
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

    int b_factor(Node *root) {
        return height(root->right) - height(root->left);
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

    Node *find_max(Node *root) {
        if (root->right) {
            return find_max(root->right);
        } else {
            return root;
        }
    }

    Node *remove_max(Node *root) {
        if (!root->right)
            return root->left;
        root->right = remove_max(root->right);
        balance(root);
        return root;
    }

    Node *_root;
public:

    AVLTree() : _root(nullptr) {}

    ~AVLTree() = default;

    Node *root() {
        return _root;
    };

    void Insert(int key) {
        if (!_root) {
            _root = new Node(key);
            return;
        }
        auto new_node = new Node(key);
        std::stack<Node *> visited;
        Node *cur_node = _root;

        while (true) {
            if (new_node->key < cur_node->key) {
                if (cur_node->left == nullptr) {
                    cur_node->left = new_node;
                    break;
                } else {
                    visited.push(cur_node);
                    cur_node = cur_node->left;
                }
            } else {
                if (cur_node->right == nullptr) {
                    cur_node->right = new_node;
                    break;
                } else {
                    visited.push(cur_node);
                    cur_node = cur_node->right;
                }
            }
        }

        while(!visited.empty()){
            auto node = visited.top();
            visited.pop();
            auto tmp = visited.empty() ? NULL : visited.top();
            if(tmp){
                if(node->key < tmp->key){
                    tmp->left = balance(node);
                } else {
                    tmp->right= balance(node);
                }
            } else {
                if(_root->key < _root->key){
                    _root->left = balance(node);
                } else {
                    _root->right= balance(node);
                }
            }
        }
    }


    bool Delete(int key) {
        if (!_root) return false;
        Stack<Node *> visited;
        auto cur_node = _root;
        while (cur_node->key != key) {
            if (!cur_node) return false; //didn't find they key
            visited.Push(cur_node);
            if (key < cur_node->key) {
                cur_node = cur_node->left;
            } else {
                cur_node = cur_node->right;
            }
        }
        // cur_node stores the pointer to the node to delete
        visited.Push(cur_node);
        auto parent = visited.Top();
        auto cur_node_l = cur_node->left;
        auto cur_node_r = cur_node->right;
        delete cur_node;

        // this leaves us with two sub-trees -- cur_node_l and cur_node_r
        // we need to find max at left sub-tree and place it instead of cur_node, balancing required
        auto max_node = find_max(cur_node_l);
        cur_node_l = remove_max(cur_node_l);
        max_node->left = cur_node_l;
        max_node->right = cur_node_r;

        if (max_node->key < parent->key) {
            parent->left = max_node;
        } else {
            parent->right = max_node;
        }

        while (Node *p = visited.Pop()) {
            balance(p);
        }

        return true;
    }

//    int k_stat(int k) {
//        auto node = _root;
//        while (node != nullptr) {
//            int h_left = height(node->left);
//
//            if (h_left == k) {
//                return node->key;
//            }
//
//            if (h_left > k) {
//                node = node->left;
//            } else {
//                node = node->right;
//                k -= h_left + 1;
//            }
//        }
//        return -1;
//    }

};


struct Operation {
    char sign;
    int value;
};

void perform_op(Operation op, AVLTree &tree) {
    switch (op.sign) {
        case '+':
            tree.Insert(op.value);
            cout << "[] ";
            InOrderDFS(tree.root(), [](Node *x) { cout << x->key << ' '; });
            cout << '\n';
            return;
        case '-':
            tree.Delete(op.value);
            cout << "[] ";
            InOrderDFS(tree.root(), [](Node *x) { cout << x->key << ' '; });
            cout << '\n';
            return;
        default:
            return;
    }
}


int main() {
//    int n;
//    cin >> n;
    AVLTree tree;
    Operation operation;
    while (cin >> operation.sign >> operation.value) {
        perform_op(operation, tree);
    }
    return 0;
}