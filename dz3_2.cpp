//
// Created by daniknik on 23.11.18.
//

//Дано число N < 10^6 и последовательность пар целых чисел из [-2^31, 2^31] длиной N.
//Построить декартово дерево из N узлов, характеризующихся парами чисел (Xi, Yi).
//Каждая пара чисел (Xi, Yi) определяет ключ Xi и приоритет Yi в декартовом дереве.
//Добавление узла в декартово дерево выполняйте второй версией алгоритма, рассказанного на лекции:
//При добавлении узла (x, y) выполняйте спуск по ключу до узла P с меньшим приоритетом.
//Затем разбейте найденное поддерево по ключу x так, чтобы в первом поддереве все ключи меньше x, а во втором больше или равны x.
//Получившиеся два дерева сделайте дочерними для нового узла (x, y). Новый узел вставьте на место узла P.
//Построить также наивное дерево поиска по ключам Xi. Равные ключи добавляйте в правое поддерево.
//Вычислить количество узлов в самом широком слое декартового дерева и количество узлов в самом широком слое наивного дерева поиска.
//Вывести их разницу. Разница может быть отрицательна.

#include <fstream>
#include <iostream>
#include <vector>

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
            return T(-1);
        }
    }

    T Top() {
        return data[size];
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

    Node() : key(0) {
        left = nullptr;
        right = nullptr;
    }

    explicit Node(int v) : key(v) {
        left = nullptr;
        right = nullptr;
    }

    ~Node() = default;
};

struct D_Node {
    int priority;
    int key;
    D_Node *left;
    D_Node *right;

    D_Node() : key(0), priority(0) {
        left = nullptr;
        right = nullptr;
    }

    explicit D_Node(int key, int prior) : key(key), priority(prior) {
        left = nullptr;
        right = nullptr;
    }

    ~D_Node() = default;
};

class BinTree {
private:
    Node *_root;
public:
    BinTree() : _root(nullptr) {}

    ~BinTree() {
        PostOrderDFS(_root, [](Node *x) { delete x; });
    }

    void Push(int _elem) {
        auto new_node = new Node(_elem);
        if (_root == nullptr) {
            _root = new_node;
            return;
        }
        auto cur_node = _root;
        while (true) {
            if (new_node->key < cur_node->key) {
                if (cur_node->left == nullptr) {
                    cur_node->left = new_node;
                    return;
                } else {
                    cur_node = cur_node->left;
                }
            } else {
                if (cur_node->right == nullptr) {
                    cur_node->right = new_node;
                    return;
                } else {
                    cur_node = cur_node->right;
                }
            }
        }
    }

    Node *root() {
        return _root;
    }
};

class DecartTree {
private:
    D_Node *_root;

    void split(D_Node *node, int key, D_Node *&left, D_Node *&right) {
        if (node == nullptr) {
            left = nullptr;
            right = nullptr;
        } else if (node->key < key) {
            split(node->right, key, node->right, right);
            left = node;
        } else {
            split(node->left, key, left, node->left);
            right = node;
        }
    }

    D_Node *merge(D_Node *left, D_Node *right) {
        if (left == nullptr || right == nullptr) {
            return left == nullptr ? right : left;
        }
        if (left->priority > right->priority) {
            left->right = merge(left->right, right);
            return left;
        } else {
            right->left = merge(left, right->left);
            return right;
        }
    }

public:
    DecartTree() : _root(nullptr) {}

    ~DecartTree() {
        PostOrderDFS(_root, [](D_Node *x) { delete x; });
    }

    void Push(int key, int priority) {
        auto node = new D_Node(key, priority);
        if (_root == nullptr) {
            _root = node;
        } else {
            D_Node *parent = nullptr;    //искомый родитель
            auto less_prior_node = _root;
            while (less_prior_node != nullptr && priority < less_prior_node->priority) {
                parent = less_prior_node;
                if (key < less_prior_node->key) {
                    less_prior_node = less_prior_node->left;
                } else {
                    less_prior_node = less_prior_node->right;
                }
            }
            split(less_prior_node, key, node->left, node->right);

            if (parent != nullptr) {
                if (node->key < parent->key) {
                    parent->left = node;
                } else {
                    parent->right = node;
                }
            } else {
                _root = node;
            }
        }
    }

    D_Node *root() {
        return _root;
    }
};

template<class T_Node>
int find_max_width(T_Node* root) {
    if (root == nullptr) {
        return -1;
    }
    Queue<T_Node *> traverse_queue;
    traverse_queue.Push(root);
    traverse_queue.Push(nullptr);
    int max_width = 0;
    while (!traverse_queue.IsEmpty()) {
        static int cur_width = 0;
        T_Node *node = traverse_queue.Pop();
        if (node == nullptr) {
            if (traverse_queue.IsEmpty()) {
                break;
            } else {
                max_width = max(max_width, cur_width);
                cur_width = 0;
                traverse_queue.Push(nullptr);
            }
        } else {
            if (node->left != nullptr) {
                traverse_queue.Push(node->left);
                cur_width++;
            }
            if (node->right != nullptr) {
                traverse_queue.Push(node->right);
                cur_width++;
            }
        }
    }
    return max_width;
}

int main(int argc, char *argv[]) {
    DecartTree d_tree;
    BinTree b_tree;

    ifstream fin("input.txt");
    int n;
    fin >> n;
    for (int i = 0; i < n; ++i) {
        int key, priority;
        fin >> key >> priority;
        b_tree.Push(key);
        d_tree.Push(key, priority);
    }

//    InOrderDFS(b_tree.root(), [](Node *x) { cout << x->key << ' '; });
//    cout << endl;
//    InOrderDFS(d_tree.root(), [](D_Node *x) { cout << x->key << ' '; });
//    cout << endl;

    cout << (find_max_width(d_tree.root()) - find_max_width(b_tree.root())) << endl;
    return 0;
}