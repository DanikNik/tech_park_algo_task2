//Дано число N ≤ 104 и последовательность целых чисел из [-2^31..2^31] длиной N.
//Требуется построить бинарное дерево, заданное наивным порядком вставки.
// Т.е., при добавлении очередного числа K в дерево с корнем root,
// если root→Key ≤ K, то узел K добавляется в правое поддерево root;
// иначе в левое поддерево root. Выведите элементы в порядке in-order (слева направо).
// Рекурсия запрещена.

#include <iostream>

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


struct Node {
    int value;
    Node *left;
    Node *right;

    Node() : value(0) {
        left = nullptr;
        right = nullptr;
    }

    explicit Node(int v) : value(v) {
        left = nullptr;
        right = nullptr;
    }

    ~Node() = default;
};

template<class T_Node, class Op>
void InOrder(T_Node *node, Op func) {
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
void PostOrder(T_Node *root, Op func) {
    Stack<T_Node *> stack1;
    Stack<T_Node *> stack2;
    stack1.Push(root);
    while (!stack1.IsEmpty()) {
        auto node = stack1.Pop();
        stack2.Push(node);
        if (node->left != nullptr)stack1.Push(node->left);
        if (node->right != nullptr)stack1.Push(node->right);
    }
    while(!stack2.IsEmpty()){
        auto node = stack2.Pop();
        func(node);
    }
}

class BinTree {
private:
    Node *root;
public:
    BinTree() {
        root = nullptr;
    }

    ~BinTree() {
        PostOrder(root, [](Node *x) { delete x; });
    }

    void Push(int _elem) {
        auto new_node = new Node(_elem);
        if (root == nullptr) {
            root = new_node;
            return;
        }
        auto cur_node = root;
        while (true) {
            if (new_node->value <= cur_node->value) {
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

    Node *get_root() {
        return root;
    }
};


int main(int argc, char *argv[]) {
    int n;
    cin >> n;
    BinTree tree;
    for (int i = 0; i < n; ++i) {
        int val_to_insert;
        cin >> val_to_insert;
        tree.Push(val_to_insert);
    }
    InOrder(tree.get_root(), [](Node *x) { cout << x->value << ' '; });
    return 0;
}
