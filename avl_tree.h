#ifndef AVL
#define AVL

#include<iostream>
#include<stack>
using namespace std;

template<typename type, typename ktype>
class avl_tree;

// ktype => key type
template<typename type, typename ktype>
class node {
    friend class avl_tree<type, ktype>;
    ktype key;
    type data;
    node<type, ktype> * right;
    node<type, ktype> * left;
public:
    node() {
        right = nullptr;
        left = nullptr;
    }
    node(type _data, ktype _key):key(_key), data(_data) {
        right = nullptr;
        left = nullptr;
    }
};

template<typename type, typename ktype>
class avl_tree {
private:
    node<type, ktype> * root;
    // function for calculate max sub tree height for a node
    int height(node<type, ktype> * _root) {
        if (_root)
            return 1 + max(height(_root->left), height(_root->right));
        return 0;
    }

    node<type, ktype> * insert(node<type, ktype> * _now, const type & _data, const ktype & _key) {
        if (! _now)
            _now = new node<type, ktype>(_data, _key);
        else if (_key < _now->key) {
            _now->left = insert(_now->left, _data, _key);
            _now = balance(_now);
        }
        else if (_key > _now->key) {
            _now->right = insert(_now->right, _data, _key);
            _now = balance(_now);
        }
        return _now;
    }

    int balance_factor(node<type, ktype> * _root) {
        return height(_root->right) - height(_root->left);
    }
    // inserted in left-left child
    node<type, ktype> * LL(node<type, ktype> *_parent) {
        node<type, ktype> * _temp = _parent->left;
        _parent->left = _temp->right;
        _temp->right = _parent;
        return _temp;
    }
    // inserted in right-right child
    node<type, ktype> * RR(node <type, ktype> * _parent) {
        node<type, ktype> * _temp = _parent->right;
        _parent->right = _temp->left;
        _temp->left = _parent;
        return _temp;
    }
    // inserted in right-left child
    node<type, ktype> * RL(node<type, ktype> *_parent) {
        node<type, ktype> *_pr = _parent->right;
        node<type, ktype> *_temp = _pr->left;
        _parent->right = _temp->left;
        _temp->left = _parent;
        _pr->left = _temp->right;
        _temp->right = _pr;
        return _temp;
    }
    // inserted in left-right child
    node<type, ktype> * LR(node<type, ktype> *_parent) {
        node<type, ktype> *_pl = _parent->left;
        node<type, ktype> *_temp = _pl->right;

        _parent->left = _temp->right;
        _pl->right = _temp->left;
        _temp->right = _parent;
        _temp->left = _pl;
        return _temp;
    }
    
    // rebalance node tree when need
    node<type, ktype> * balance(node<type, ktype> * _root) {
        int _bf = this->balance_factor(_root);

        // right sub-tree is deeper
        if (_bf > 1) {
            if (this->balance_factor(_root->right) > 0)
                _root = this->RR(_root);
            else
                _root = this->RL(_root);
        }
        // left sub-tree is deeper
        else if (_bf < -1) {
            if (this->balance_factor(_root->left) > 0)
                _root = this->LR(_root);
            else
                _root = this->LL(_root);
        }
        return _root;
    }
public:
    // create empty tree
    avl_tree() {
        root = nullptr;
    }

    // call the private insert function
    void insert(const type & _data, const ktype & _key) {
        root = this->insert(root, _data ,_key);
    }

    // print inorder iteration (non-recursive)
    void print_sorted() {
        stack<node<type, ktype>*> _stack;
        node<type, ktype> *_current = root;

        while (_current || ! _stack.empty()) {
            while (_current) {
                _stack.push(_current);
                _current = _current->left;
            }
            _current = _stack.top();
            _stack.pop();

            cout << _current->key << " , ";

            _current = _current->right;
        }
        cout << endl;
    }

    // find a node by value and return a pointer to it
    node<type, ktype> * find(type _target) {
        node<type, ktype> *_current = root;
        while (_current) {
            if (_current->key == _target)
                return _current;
            _current = _target > _current->key ? _current->right : _current->left;
        }
        return NULL;
    }

};

#endif
