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

template<class T_Node>
class LayerCount {
private:
    vector<int> tree_layers;
public:
    LayerCount() = default;
    void operator()(T_Node* node) {
        if(tree_layers.empty()){
            tree_layers.emplace_back(1);
        }
        for (int i = 0; i < tree_layers.size(); ++i) {
            if (node->height == i){
                tree_layers[i] ++;
            }
            else{
                tree_layers.emplace_back(1);
            }
        }
    }

    int GetMaxWidth() {
        int max_width = 0;
        for (auto i : tree_layers) {
            max_width = max(i, max_width);
        }
        return max_width;
    }
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
void PostOrderDFS(T_Node *node, Op func) {
//        Stack<Node *> s;
//        Node *lastNodeVisited = nullptr;
//        while (!s.IsEmpty() || node != nullptr) {
//            if (node != nullptr) {
//                s.Push(node);
//                node = node->left;
//            } else {
//                Node *peekNode = s.Top();
//                // если правый потомок существует и обход пришёл из левого потомка, двигаемся вправо
//                if (peekNode->right != nullptr and lastNodeVisited != peekNode->right) {
//                    node = peekNode->right;
//                } else {
//                    func(peekNode);
//                    lastNodeVisited = s.Pop();
//                }
//            }
//        }
    if (node == nullptr) {
        return;
    }
    PostOrderDFS(node->left, func);
    PostOrderDFS(node->right, func);
    func(node);
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
    int value;
    Node *left;
    Node *right;
    int height;

    Node() : value(0), height(0) {
        left = nullptr;
        right = nullptr;
    }

    explicit Node(int v) : value(v), height(0) {
        left = nullptr;
        right = nullptr;
    }

    ~Node() = default;
};

struct D_Node {
    int priority;
    int value;
    D_Node *left;
    D_Node *right;
    int height;

    D_Node() : value(0), priority(0), height(0) {
        left = nullptr;
        right = nullptr;
    }

    explicit D_Node(int key, int prior) : value(key), priority(prior), height(0) {
        left = nullptr;
        right = nullptr;
    }

    ~D_Node() = default;
};

class BinTree {
private:
    Node *root;
public:
    BinTree() : root(nullptr) {}

    ~BinTree() {
        PostOrderDFS(root, [](Node *x) { x = nullptr; });
    }

    void Push(int _elem) {
        auto new_node = new Node(_elem);
        if (root == nullptr) {
            root = new_node;
            return;
        }
        auto cur_node = root;
        while (true) {
            if (new_node->value < cur_node->value) {
                if (cur_node->left == nullptr) {
                    cur_node->left = new_node;
                    return;
                } else {
                    cur_node = cur_node->left;
                    new_node->height++;
                }
            } else {
                if (cur_node->right == nullptr) {
                    cur_node->right = new_node;
                    return;
                } else {
                    cur_node = cur_node->right;
                    new_node->height++;
                }
            }
        }
    }

    Node *get_root() {
        return root;
    }
};

class DecartTree {
private:
    D_Node *root;

    void split(D_Node *currentNode, int key, D_Node *&left, D_Node *&right) {
        if (currentNode == nullptr) {
            left = nullptr;
            right = nullptr;
        } else if (currentNode->value <= key) {
            split(currentNode->right, key, currentNode->right, right);
            left = currentNode;
        } else {
            split(currentNode->left, key, left, currentNode->left);
            right = currentNode;
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
    DecartTree() : root(nullptr) {}

    ~DecartTree() {
        PostOrderDFS(root, [](D_Node *x) { x = nullptr; });
    }

    void Insert(int key, int priority) {
        if (root == nullptr) {
            root = new D_Node(key, priority);
            return;
        }

        D_Node *tmp = root;
        D_Node *parent = nullptr;

        while (tmp != nullptr && tmp->priority > priority) {
            parent = tmp;

            if (tmp->value < key) {
                tmp = tmp->right;
            } else {
                tmp = tmp->left;
            }
        }

        D_Node *left = nullptr;
        D_Node *right = nullptr;

        split(tmp, key, left, right);

        auto *elem = new D_Node(key, priority);
        elem->left = left;
        elem->right = right;
        if (parent != nullptr) {
            if (parent->value > elem->value) {
                parent->left = elem;
                elem->height++;
            } else {
                parent->right = elem;
                elem->height++;
            }
        } else {
            root = elem;
        }
    }

    D_Node *get_root() {
        return root;
    }
};


int main(int argc, char *argv[]) {
    DecartTree d_tree;
    BinTree b_tree;

    int n;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        int key, priority;
        cin >> key >> priority;
        b_tree.Push(key);
        d_tree.Insert(key, priority);
    }

    LayerCount<Node> b_counter;
    LayerCount<D_Node> d_counter;

    TraverseBFS(b_tree.get_root(), b_counter);
    cout << '\n';
    TraverseBFS(d_tree.get_root(), d_counter);

    cout << b_counter.GetMaxWidth() - d_counter.GetMaxWidth() << endl;
    return 0;
}