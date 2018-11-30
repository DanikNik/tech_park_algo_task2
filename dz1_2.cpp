#include <iostream>
#include <cassert>

using namespace std;

struct Bin {
    bool DELETED;
    bool EMPTY;
    string data;

    Bin() : DELETED(false), EMPTY(true) {}

    Bin(const Bin &bin) {
        DELETED = bin.DELETED;
        EMPTY = bin.EMPTY;
        data = bin.data;
    }

    Bin(Bin &&bin) noexcept {
        DELETED = bin.DELETED;
        bin.DELETED = false;
        EMPTY = bin.EMPTY;
        bin.EMPTY = true;
        data = bin.data;
        bin.data.clear();
    }

    Bin &operator=(const Bin &bin) = default;

    Bin &operator=(Bin &&bin) noexcept {
        DELETED = bin.DELETED;
        bin.DELETED = false;
        EMPTY = bin.EMPTY;
        bin.EMPTY = true;
        data = bin.data;
        bin.data.clear();
        return *this;
    }

    ~Bin() = default;

    string Key() {
        return data;
    }


};

class StringHasher {
private:
    int table_capacity;
    int rand_cf_1 = 3;
    int rand_cf_2 = 5;
public:
    StringHasher() = delete;

    explicit StringHasher(int capacity) : table_capacity(capacity) {}

    ~StringHasher() = default;

    int hash1(const string &elem) {
        int hash = 0;
        for (char i : elem) {
            hash = (hash * rand_cf_1 + (int) i) % table_capacity;
        }
        return hash;
    }

    int hash2(const string &elem) {
        int hash = 0;
        for (char i : elem) {
            hash = (hash * rand_cf_2 + (int) i) % table_capacity;
        }
        return (hash * 2 + 1) % table_capacity;
    }

    void set_capacity(int capacity) {
        table_capacity = capacity;
    }
};

class HashTable {
private:
    const int DEFAULT_SIZE = 8;
    int table_size;
    int elem_count;
    Bin *table;
    StringHasher *hasher;
    float solidity;


    void recalc_solidity() {
        solidity = float(elem_count) / float(table_size);
    }

    void rehash() {
        int old_size = table_size;
        table_size *= 2;
        auto new_table = new Bin[table_size];
        hasher->set_capacity(table_size);

        for (int i = 0; i < old_size; ++i) {
            Bin &elem = table[i];
            if (!elem.EMPTY) {
                for (int j = 0; j < table_size; ++j) {
                    int index = get_probe(j, elem.Key());
                    if (new_table[index].EMPTY) {
                        new_table[index] = elem;
                        break;
                    }
                }
            }
        }
        delete[] table;
        table = new_table;
        recalc_solidity();
    }

    int get_probe(int index, const string &key) {
        return (hasher->hash1(key) + index * hasher->hash2(key)) % table_size;
    }

public:
    HashTable() :
            table(new Bin[DEFAULT_SIZE]),
            table_size(DEFAULT_SIZE),
            solidity(0),
            elem_count(0),
            hasher(new StringHasher(table_size)) {}

    ~HashTable() {
        delete hasher;
        delete[] table;
    }

    bool Insert(const string &data) {
        if(Contains(data)) { return false;}
        if (solidity > 3.0 / 4.0) rehash();
        for (int i = 0; i < table_size; ++i) {
            int index = get_probe(i, data);
            Bin &elem = table[index];
            if(elem.EMPTY || elem.DELETED){
                elem.data = data;
                elem.DELETED = false;
                elem.EMPTY = false;
                elem_count++;
                recalc_solidity();
                return true;
            }

        }
        return false;
    }

    bool Contains(const string &data) {
        for (int i = 0; i < table_size; ++i) {
            int index = get_probe(i, data);
//            if (table[index].DELETED){
//                continue;
//            }
            if (table[index].EMPTY) {
                return false;
            }
            if (table[index].Key() == data && !table[index].DELETED) {
                return true;
            }
        }
        return false;
    }

    bool Delete(const string &data) {
        for (int i = 0; i < table_size; ++i) {
            int index = get_probe(i, data);
            Bin &elem = table[index];
            if (elem.EMPTY) {
                return false;
            }
            if (elem.Key() == data && !elem.DELETED && !elem.EMPTY) {
                elem.DELETED = true;
//                elem.data.clear();
                elem_count--;
                recalc_solidity();
                return true;
            }
        }
        return false;
    }

    void _print() {
        cout << '[' << elem_count << '|' << solidity << ']' << ' ';
        for (int i = 0; i < table_size; ++i) {
            if (!table[i].EMPTY && !table[i].DELETED) {
                cout << table[i].data << ' ';
            }
        }
        cout << endl;
    }
};

struct Operation {
    char sign;
    string value;
};

bool perform_op(Operation op, HashTable &table) {
    bool code;
    switch (op.sign) {
        case '+':
            code = table.Insert(op.value);
//            table._print();
            return code;
        case '-':
            code = table.Delete(op.value);
//            table._print();
            return code;
        case '?':
            code = table.Contains(op.value);
//            table._print();
            return code;
        default:
            return false;
    }
}

int main() {
    HashTable table;
    Operation op;
    while (cin >> op.sign >> op.value) {
//        cout << "[+]OPERATION: " << op.sign << ' ' << op.value << endl;
        cout << (perform_op(op, table) ? "OK" : "FAIL") << endl;
    }
    return 0;
}