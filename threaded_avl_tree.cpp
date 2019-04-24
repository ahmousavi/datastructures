#ifndef HEADER
#define HEADER
//#define DEBUG

#include<iostream>
#include<stack>
using namespace std;

template<typename type, typename ktype>
class threaded_avl;

template<typename type, typename ktype>
class node {
public:
    friend class threaded_avl<type, ktype>;
    ktype key;
    type data;
    node<type, ktype> * right;
    node<type, ktype> * left;
    bool rightT, leftT;
    int ht;
    node() {
        right = nullptr;
        left = nullptr;
        rightT = leftT = true;
        ht = 1;
    }
    node(type _data, ktype _key):key(_key), data(_data) {
        right = nullptr;
        left = nullptr;
        rightT = leftT = true;
        ht = 1;
    }
};

template<typename type, typename ktype>
class threaded_avl {
private:
    node<type, ktype> * root;
    // todo: remove in production
    int height(node<type, ktype> * _root) {
        int leftH = _root->leftT ? 0 : height(_root->left);
        int rightH = _root->rightT ? 0 : height(_root->right);
        return 1 + max(leftH, rightH);
    }

    static node<type, ktype> * leftMost(node<type, ktype> * _root) {
        while(! _root->leftT)
            _root = _root->left;
        return _root;
    }

    static node<type, ktype> * rightMost(node<type, ktype> * _root) {
        while(! _root->rightT)
            _root = _root->right;
        return _root;
    }

    void updateHeight(node<type, ktype> *_p) {
        _p->ht = _p->leftT ? 1 : _p->left->ht+1;
        if (! _p->rightT && _p->right->ht + 1 > _p->ht)
            _p->ht = _p->right->ht + 1;
    }

    node<type, ktype> * insert(node<type, ktype> * _now, type _data, ktype _key);

    int balance_factor(node<type, ktype> * _root) {
        int leftH = _root->leftT ? 0 : _root->left->ht;
        int rightH = _root->rightT ? 0 : _root->right->ht;
        return rightH - leftH;
    }

    node<type, ktype> * LL(node<type, ktype> *_p) {
        
        node<type, ktype> * _q = _p->left;
        if (_q->rightT) {
            _p->left = _q;
            _p->leftT = true;
        }
        else {
            _p->left = _q->right;
        }
        _q->right = _p;
        _q->rightT = false;

        // update heights
        updateHeight(_p);
        updateHeight(_q);

        return _q;
    }

    node<type, ktype> * RR(node <type, ktype> * _p) {
        
        node<type, ktype> * _q = _p->right;

        if (_q->leftT) {
            _p->right = _q;
            _p->rightT = true;
        }
        else {
            _p->right = _q->left;
            _p->ht = _p->leftT ? _q->ht + 1 : max(_p->left->ht + 1, _p->right->ht + 1);
        }

        _q->left = _p;
        _q->leftT = false;

        // update heights
        updateHeight(_p);
        updateHeight(_q);

        return _q;
    }

    node<type, ktype> * RL(node<type, ktype> *_p) {
        
        node<type, ktype> *_q = _p->right;
        node<type, ktype> *_s = _q->left;

        if (_s->leftT) {
            _p->right = _s;
            _p->rightT = true;
        }
        else
            _p->right = _s->left;

        if (_s->rightT) {
            _q->left = _s;
            _q->leftT = true;
        }
        else
            _q->left = _s->right;

        _s->left = _p;
        _s->right = _q;

        _s->rightT = _s->leftT = false;

        updateHeight(_q);
        updateHeight(_p);
        updateHeight(_s);
        return _s;
    }

    node<type, ktype> * LR(node<type, ktype> *_p) {
        
        node<type, ktype> *_q = _p->left;
        node<type, ktype> *_s = _q->right;

        if (_s->rightT) {
            _p->left = _s;
            _p->leftT = true;
        }
        else
            _p->left = _s->right;

        if (_s->leftT) {
            _q->right = _s;
            _q->rightT = true;
        }
        else
            _q->right = _s->left;


        _s->right = _p;
        _s->left = _q;
        _s->leftT = _s->rightT = false;

        updateHeight(_q);
        updateHeight(_p);
        updateHeight(_s);
        return _s;
    }
    

    node<type, ktype> * balance(node<type, ktype> * _root) {

        int _bf = this->balance_factor(_root);

        // right sub-tree is deeper
        if (_bf > 1) {
            if (this->balance_factor(_root->right) >= 0)
                _root = this->RR(_root);
            else
                _root = this->RL(_root);
        }
            // left sub-tree is deeper
        else if (_bf < -1) {
            if (this->balance_factor(_root->left) >= 0)
                _root = this->LR(_root);
            else
                _root = this->LL(_root);
        }
        return _root;
    }

    node<type, ktype> * deleteNode(node<type, ktype> * _parent, node<type, ktype> *_target);

    node<type, ktype> * removeKey (node<type, ktype> * _parent, node<type, ktype> * _root, const ktype & _key);

public:
    // create empty tree
    threaded_avl() {
        root = new node<type, ktype>;
        root->right = root;
        root->left = root;
    }

//    ~threaded_avl() {
//        node<type, ktype> * current = root->left;
//        node<type, ktype> * prev = nullptr;
//        if (current == root)
//            return;
//        current = leftMost(current);
//        int i=0;
//        while (true) {
//            delete prev;
//            prev = current;
//            if (! current->rightT) {
//                current = current->right;
//                current = leftMost(current);
//                continue;
//            }
//            current = current->right;
//            if (current == root)
//                break;
//        }
//    }

    // call the private insert function
    void insert(type _data, ktype _key) {
        root->left = this->insert(root->left, _data ,_key);
    }

    void removeKey (const ktype & _key) {
        root = this->removeKey(root, root->left, _key);
    }
    node<type, ktype> * findKey(const ktype & _key) {
        node<type, ktype> * _current = root->left;
        while (true) {
            if (_key == _current->key)
                return _current;
            if (_key > _current->key && ! _current->rightT)
                _current = _current->right;
            else if (! _current->leftT)
                _current = _current->left;
            else
                return NULL;
        }
    }
    node<type, ktype> * findLowerBound(ktype _key) {
        node<type, ktype> * _current = root->left;
        node<type, ktype> * _lb = root->left; // new node<type, ktype>(type(), _key)

        while (true) {
            if (_key == _current->key)
                return _current;
            if (_current->key > _key)
                _lb = _current;
            if (_key > _current->key && ! _current->rightT)
                _current = _current->right;
            else if (! _current->leftT) {
                _current = _current->left;
            }
            else
                return _lb->key < _key ? NULL : _lb;
        }
    }
    void saveToFile(ostream &file, node<type, ktype> *_root);
    void saveToFile(ostream &file) {
        saveToFile(file, root->left);
        file << "$";
    }

    void readFromFile(istream &file, node<type, ktype> *& _parent, node<type, ktype> *&_root);
    void readFromFile(istream &file) {
        readFromFile(file, root, root->left);
        string x;
        while(file.peek() == '\n')
            file.ignore();
        if (file.peek() == '$')
            file >> x;
    }

    class iterator;
    class vlrIterator;
};

template <typename type, typename ktype>
node<type, ktype> * threaded_avl<type, ktype>::insert(node<type, ktype> * _now, type _data, ktype _key) {
    if (root->left == root) {
        node<type, ktype> * temp = new node<type, ktype> (_data, _key);
        temp->right = temp->left = root;
        temp->leftT = temp->rightT = true;
        _now->leftT = false;
        _now = temp;
    }
    else if ((_key > _now->key || (_key == _now->key && _data > _now->data)) && _now->rightT) {
        node<type, ktype> * temp = new node<type, ktype> (_data, _key);
        temp->right = _now->right;
        temp->left = _now;
        _now->right = temp;
        _now->rightT = false;
        _now->ht = max(_now->ht, temp->ht + 1);
    }
    else if ((_key < _now->key || (_key == _now->key && _data < _now->data)) && _now->leftT) {
        node<type, ktype> * temp = new node<type, ktype> (_data, _key);
        temp->left = _now->left;
        temp->right = _now;
        _now->left = temp;
        _now->leftT = false;
        _now->ht = max(_now->ht, temp->ht + 1);
    }
    else if (_key < _now->key || (_key == _now->key && _data < _now->data)) {
        _now->left = insert(_now->left, _data, _key);
        _now->ht = max(_now->ht, _now->left->ht + 1);
        _now = balance(_now);
    }
    else if (_key > _now->key || (_key == _now->key && _data > _now->data)) {
        _now->right = insert(_now->right, _data, _key);
        _now->ht = max(_now->ht, _now->right->ht + 1);
        _now = balance(_now);
    }
    return _now;
}

template <typename type, typename ktype>
void threaded_avl<type, ktype>::saveToFile(ostream &file, node<type, ktype> *_root) {
    file << _root->key << " " <<_root->ht << " " << _root->data << endl;
    if (_root->leftT)
        file << "#\n";
    else
        saveToFile(file, _root->left);
    if (_root->rightT)
        file << "#\n";
    else
        saveToFile(file, _root->right);
}

template <typename type, typename ktype>
void threaded_avl<type, ktype>::readFromFile(istream &file, node<type, ktype> *& _parent, node<type, ktype> *&_root) {
    string _line;
    node<type, ktype> cp_p = *_parent;
    bool pleft = _parent->left == _root;
    char xxx;
    while((xxx = file.peek()) == '\n')
        file.ignore();
    if (xxx == '$') {
        return;
    }
    if (xxx == '#') {
        file >> _line;
        return;
    }
    ktype _key;
    if (file >> _key) {
        type _data;
        int h = 1;
        file >> h >> _data;
        _root = new node<type, ktype> (_data, _key);
        _root->ht = h;

        if (pleft) {
            _root->left = cp_p.left;
            _root->right = _parent;
            _parent->leftT = false;

        } else {
            _root->right = cp_p.right;
            _parent->rightT = false;
            _root->left = _parent;
        }

        readFromFile(file, _root, _root->left);
        readFromFile(file, _root, _root->right);
    }
}

template<typename type, typename ktype>
node<type, ktype> *threaded_avl<type, ktype>::removeKey(node<type, ktype> *_parent, node<type, ktype> *_root, const ktype &_key) {
    bool pLeft = _parent->left == _root;

    if (_parent == root && _root == root)
        return root;
    if (_key > _root->key && ! _root->rightT) {
        _root = removeKey(_root, _root->right, _key);
        updateHeight(_root);
        _root = balance(_root);
        if (pLeft)
            _parent->left = _root;
        else
            _parent->right = _root;
    }
    else if (_key < _root->key && ! _root->leftT) {
        _root = removeKey(_root, _root->left, _key);
        updateHeight(_root);
        _root = balance(_root);
        if (pLeft)
            _parent->left = _root;
        else
            _parent->right = _root;
    }
    else if (_key == _root->key) {
        if (_root->rightT || _root->leftT) {
            _parent = deleteNode(_parent, _root);
            updateHeight(_parent);
        }
            // has both
        else {
            node<type, ktype> * _leftMax = _root->left, *_pre = _root;
            while(! _leftMax->rightT) {
                _pre = _leftMax;
                _leftMax = _leftMax->right;
            }
            _root->data = _leftMax->data;
            _root->key = _leftMax->key;

            _pre = deleteNode(_pre, _leftMax);
        }
    }
    return _parent;
}

template<typename type, typename ktype>
node<type, ktype> *threaded_avl<type, ktype>::deleteNode(node<type, ktype> *_parent, node<type, ktype> *_target) {
    // is leaf
    if (_target->rightT && _target->leftT) {
        node<type, ktype> * _temp = _target;
        if (_parent->right == _target) {
            _parent->right = _target->right;
            _parent->rightT = true;
        }
        else {
            _parent->left = _target->left;
            _parent->leftT = true;
        }
        delete _temp;
    }
        // has no right
    else if (_target->rightT && !_target->leftT) {
        node<type, ktype> * _temp = _target;
        if (_parent->right == _target) {
            _target->left->right = _target->right;
            _parent->right = _target->left;
        }
        else {
            _target->left->right = _parent;
            _parent->left = _target->left;
        }
        delete _temp;
    }
        // has no left
    else if (! _target->rightT && _target->leftT) {
        node<type, ktype> * _temp = _target;
        if (_parent->right == _target)
            _parent->right = _target->right;
        else {
            _target->right->left = _target->left;
            _parent->left = _target->right;
        }
        delete _temp;
    }
    return _parent;
}

template <typename t, typename k>
class threaded_avl<t, k>::iterator {
    node<t, k> * root;
    node<t, k> * current;
public:
    iterator(const threaded_avl<t,k> & _tree):root(_tree.root) {
        current = threaded_avl<t,k>::leftMost(_tree.root->left);
    }

    iterator(const threaded_avl<t,k> & _tree, node<t, k> *_start):root(_tree.root) {
        current = _start;
    }

    node<t,k>* getNode() const {
        return current;
    }
    bool next() {
        if (current->rightT && current->right == root)
            return false;
        if (! current->rightT)
            current = threaded_avl<t,k>::leftMost(current->right);
        else
            current = current->right;
        return true;
    }
    bool prev() {
        if (current->leftT && current->left == root)
            return false;
        if (! current->leftT)
            current = threaded_avl<t,k>::rightMost(current->left);
        else
            current = current->left;
        return true;
    }
};

template <typename T, typename K>
class threaded_avl<T, K>::vlrIterator {
    node<T, K> *current;
    node<T, K> *root;
    stack<node<T, K> *> stk;
public:
    vlrIterator(const threaded_avl<T, K> & _tree):root(_tree.root), current(_tree.root) {
        stk.push(_tree.root);
    }

    T get() const {
        return current->data;
    }
    bool isNull () {
        return current == nullptr;
    }
    bool next() {
        if (! current && ! stk.empty()) {
            node<T, K> * parent = stk.top();
            stk.pop();
            if (parent == root)
                return false;
            if (parent->rightT)
                current = nullptr;
            else
                current = parent->right;
            return true;
        }
        if (current != root)
            stk.push(current);
        if (current->leftT)
            current = nullptr;
        else
            current = current->left;

        return current != root;

    }

};

#endif
