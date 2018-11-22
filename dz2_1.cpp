//Дано число N ≤ 104 и последовательность целых чисел из [-231..231] длиной N.
//Требуется построить бинарное дерево, заданное наивным порядком вставки.
// Т.е., при добавлении очередного числа K в дерево с корнем root,
// если root→Key ≤ K, то узел K добавляется в правое поддерево root;
// иначе в левое поддерево root. Выведите элементы в порядке in-order (слева направо).
// Рекурсия запрещена.

#include <iostream>
#include <stack>

using namespace std;

template <class T>
class Stack {
private:
    int size;
    int capacity;

    void resize() {
        this->capacity *= 2;
        T *tmp = new T[this->capacity];
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

//    Stack(const Stack &other) = default;

//    Stack(Stack &&other) = default;

    ~Stack() {
        if (this->data != 0)
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

    bool IsEmpty() {
        return this->size == 0;
    }

    int Size() {
        return this->size;
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

    Node(int v) : value(v) {
        left = nullptr;
        right = nullptr;
    }

    ~Node() = default;
};

class BinTree {
private:
    Node *root;
public:
    BinTree() {
        root = nullptr;
    }

    BinTree(int _root_val) {
        root = new Node(_root_val);
    }

    ~BinTree() {

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

    void Print(Node *_root) {
        if(_root == nullptr){
            return;
        }
        Print(_root->left);
        cout << _root->value << ' ';
        Print(_root->right);

//        std::stack<Node *> traverse_stack;

    }

    Node *get_root() {
        return root;
    }
};


int main(int argc, char *argv[]) {
    int n;
    cin >> n;
    BinTree tree;
    for (int i = 1; i < n; ++i) {
        int val_to_insert;
        cin >> val_to_insert;
        tree.Push(val_to_insert);
    }
    tree.Print(tree.get_root());
    return 0;
}
