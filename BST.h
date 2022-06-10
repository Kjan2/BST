

#include <utility>
#include <iostream>
/*!
    Имплементация бинарного дерева поиска
    Допускается дублирование ключей (аналог multimap)
*/
template <typename Key = int, typename Value = int>
class BinarySearchTree
{
    struct Node
    {
        Node(Key key, 
             Value value, 
             Node* parent = nullptr, 
             Node* left = nullptr, 
             Node* right = nullptr)
        {
            keyValuePair = std::make_pair(key, value);
        }

        std::pair<Key, Value> keyValuePair;
        Node* parent = nullptr;
        Node* left = nullptr;
        Node* right = nullptr;
    };

public:
    //! Конструктор по умолчанию
    BinarySearchTree() = default;
    
    //! Копирование
    explicit BinarySearchTree(const BinarySearchTree& other) : _root(copy(other._root))
    {
        this->_size = other._size;
    }
    BinarySearchTree& operator=(const BinarySearchTree& other)
    {
        destruct(this->_root);
        this->_root = copy(other._root);
        this->_size = other._size;
    }   
    //! Перемещение
    explicit BinarySearchTree(BinarySearchTree&& other) noexcept
    {
        this->_root = other._root;
        this->_size = other._size;
        other._root = nullptr;
    }
    BinarySearchTree& operator=(BinarySearchTree&& other) noexcept
    {
        destruct(this->_root);
        this->_root = other._root;
        this->_size = other._size;
        other._root = nullptr;
    }

    //! Деструктор
    ~BinarySearchTree()
    {
        destruct(_root);
    }

    /*!
        Итератор бинарного дерева поиска

        Обходит дерево последовательно от меньшего ключа к большему 
    */
    class Iterator
    {
    public:
        explicit Iterator(Node* node) : _node(node)
        {

        }

        std::pair<Key, Value>& operator*()
        {
            return _node->keyValuePair;
        }

        const std::pair<Key, Value>& operator*() const
        {
            return _node->keyValuePair;
        }

        std::pair<Key, Value>* operator->()
        {
            return &(_node->keyValuePair);
        }

        const std::pair<Key, Value>* operator->() const
        {
            return &(_node->keyValuePair);
        }

        Iterator operator++()
        {
            if (_node->right != nullptr)
            {
                _node = _node->right;

                while (_node->left != nullptr)
                {
                    _node = _node->left;
                }

                return Iterator(_node);
            }

            
            Node* y = _node->parent;
            
            while ((y != nullptr) && (_node == y->right))
            {
                _node = y;
                y = _node->parent; 
            }

            _node = y;

            return Iterator(_node);
        }

        Iterator operator++(int)
        {
            Iterator it(*this);
            ++(*this);
            return it;
        }

        Iterator operator--()
        {
            if (_node->left != nullptr)
            {
                _node = _node->left;

                while (_node->right != nullptr)
                {
                    _node = _node->right;
                }

                return Iterator(_node);
            }

            
            Node* y = _node->parent;

            while ((y != nullptr) && (_node == y->left))
            {
                _node = y;
                y = _node->parent; 
            }

            _node = y;

            return Iterator(_node);

        }
        Iterator operator--(int)
        {
            Iterator it(*this);
            --(*this);
            return it;
        }

        bool operator==(const Iterator& other) const
        {
            return (this->_node == other._node);
        }
        bool operator!=(const Iterator& other) const
        {
            return !(this->_node == other._node);
        }

    private:
        Node* _node;
    };

    /*!
        Константный итератор
    */
    class ConstIterator
    {
    public:
        explicit ConstIterator(const Node* node) : _node(node)
        {

        }

        const std::pair<Key, Value>& operator*() const
        {
            return _node->keyValuePair;  
        }

        const std::pair<Key, Value>* operator->() const
        {
            return &(_node->keyValuePair);
        }

        ConstIterator operator++()
        {
            if (_node->right != nullptr)
            {
                _node = _node->right;

                while (_node->left != nullptr)
                {
                    _node = _node->left;
                }

                return ConstIterator(_node);
            }

            
            Node* y = _node->parent;
            
            while ((y != nullptr) && (_node == y->right))
            {
                _node = y;
                y = _node->parent; 
            }

            _node = y;

            return ConstIterator(_node);

        }

        ConstIterator operator++(int)
        {
            ConstIterator it(*this);
            ++(*this);
            return it;
        }

        ConstIterator operator--()
        {
            if (_node->left != nullptr)
            {
                _node = _node->left;

                while (_node->right != nullptr)
                {
                    _node = _node->right;
                }

                return ConstIterator(_node);
            }

            
            Node* y = _node->parent;

            while ((y != nullptr) && (_node == y->left))
            {
                _node = y;
                y = _node->parent; 
            }

            _node = y;

            return ConstIterator(_node);
        }
        ConstIterator operator--(int)
        {
            ConstIterator it(*this);
            --(*this);
            return it;
        }

        bool operator==(const ConstIterator& other) const
        {
            return (this->_node == other._node); 
        }
        bool operator!=(const ConstIterator& other) const
        {
            return !(this->_node == other._node);
        }

    private:
        const Node* _node;
    };

    // вставить элемент с ключем key и значением value
    void insert(const Key& key, const Value& value)
    {
        _size++;

        Node* insertNode = new Node(key, value);
        Node* y = nullptr;
        Node* x = _root;
        while (x != nullptr)
        {
            y = x;
            if (insertNode->keyValuePair.first < x->keyValuePair.first)
            {
                x = x->left;
            }

            else
            {
                x = x->right;
            }
        }

        insertNode->parent = y;

        if (y == nullptr)
        {
            _root = insertNode;
        }
        else
        {
            if (insertNode->keyValuePair.first < y->keyValuePair.first)
            {
                y->left = insertNode;
            }
            else
            {
                y->right = insertNode;
            }

        }
    }

    // удалить все элементы с ключем key
    void erase(const Key& key)
    {
        Node* node;
        while((node = find(_root, key)) != nullptr)
        {
            deleteNode(node);
        }
    }

    // найти первый элемент в дереве, равный ключу key
    ConstIterator cfind(const Key& key) const
    {
        return ConstIterator(find(_root, key));
    }
    Iterator find(const Key& key)
    {
        return Iterator(find(_root, key));
    }

    // найти все элементы, у которых ключ равен key
    // первый итератор пары - первый элемент в дереве, равный key
    // второй итератор пары - первый элемент в дереве больший, чем key
    // [pair.first, pair.second) - полуинтервал, содержащий все элементы с ключем key
    std::pair<Iterator, Iterator> equalRange(const Key& key)
    {
        auto it = find(key);
        if (it == (Iterator(nullptr)))
        {
            return std::make_pair(it, it);
        }
        auto temp = it;
        while ((*it).first == key)
        {
            it++;
        }
        return std::make_pair(temp, it);
    }

    std::pair<ConstIterator, ConstIterator> equalRange(const Key& key) const
    {
        ConstIterator it = cfind(key);
        if (it == (ConstIterator(nullptr)))
        {
            return std::make_pair(it, it);
        }
        auto temp = it;
        while((*it).first == key)
        {
            it++;
        }
        return std::make_pair(temp, it);
    }

    // получить итератор на минимальное значение в дереве
    ConstIterator min(const Key& key) const
    {
        std::pair<ConstIterator,ConstIterator> range = equalRange(key);
        auto it = range.first;

        ConstIterator result(nullptr);
        Key min = (*it).second;
        while (it != range.second)
        {
            if ((*it).second <= min)
            {
                min = (*it).second;
                result = it;

            }
            it++;
        } 
        return result;
    }
    // получить итератор на максимальное значение в дереве
    ConstIterator max(const Key& key) const
    {
        std::pair<ConstIterator,ConstIterator> range = equalRange(key);
        auto it = range.first;

        ConstIterator result(nullptr);
        Key max = (*it).second;
        while (it != range.second)
        {
            if ((*it).second >= max)
            {
                max = (*it).second;
                result = it;

            }
            it++;
        } 
        return result;
    }

    Iterator begin()
    {
        Node* curr = _root;

        while (curr->left != nullptr)
        {
            curr = curr->left;
        }

        return Iterator(curr);
    }

    Iterator end()
    {
        return Iterator(nullptr);
    }

    ConstIterator cbegin() const
    {
        Node* curr = _root;

        while (curr->left != nullptr)
        {
            curr = curr->left;
        }

        return ConstIterator(curr);
    }
    ConstIterator cend() const
    {
        return ConstIterator(nullptr);
    }


    size_t size() const
    {
        return _size;
    }


public:
    size_t _size = 0;
    Node* _root = nullptr; //!< корневой узел дерева

    void destruct(Node* node)
    {
        if (node == nullptr)
        {
            return;
        }

        destruct(node->left);
        destruct(node->right);
        delete node;
        node = nullptr;
    }

    Node* find(Node* node, const Key key) const
    {
        if (node == nullptr)
        {
            return node;
        }

        while ((node != nullptr) && (node->keyValuePair.first != key))
        {
            if (node->keyValuePair.first < key)
            {
               node = find(node->right,key);
            }
            else
            {
                node = find(node->left, key);
            }
        }

        return node;
    }

    void deleteNode(Node* node)
    {
        _size--;

        if ((node == _root) && (_root->left == nullptr) && (_root->right == nullptr))
        {
            delete node;
            _root = nullptr;
            return;
        }

        Node* x = nullptr;
        Node* y = nullptr;
        
        if ((node->left == nullptr) || (node->right == nullptr))
        {
            y = node;
        }

        else
        {
            y = nodeSuccessor(node);
        }

        if (y->left != nullptr)
        {
            x = y->left;
        } 
        else
        {
            x = y->right;
        }

        if (x != nullptr)
        {
            x->parent = y->parent;
        }   

        if (y->parent == nullptr)
        {
            _root = x;
        }
        else if(y == y->parent->left)
        {
            y->parent->left = x;
        }

        else
        {
            y->parent->right = x;
        }
        if(y != node)
        {
            node->keyValuePair = y->keyValuePair;
            delete y;
        } 

    }
    
    Node* copy(Node* copyNode, Node* parent = nullptr)
    {
        if(copyNode != nullptr)
        {
            Node* node = new Node(copyNode->keyValuePair.first,
                                  copyNode->keyValuePair.second);
            node->parent = parent;
            node->left = copy(copyNode->left, node);
            node->right = copy(copyNode->right, node);
            return node;
        }
        else
        {
            return nullptr; 
        }
    }

    Node* nodeSuccessor(Node* node)
    {
        if(node->right != nullptr)
        {
            return treeMinimum(node->right); 
        }

        Node* parentNode = node->parent;

        while ((parentNode != nullptr) && (node == parentNode->right))
        {
            node = parentNode;
            parentNode = parentNode->parent;
        }

        return parentNode;
    }

    Node* treeMinimum(Node* root)
    {
        while(root->left != nullptr)
        {
            root = root->left;
        }
        return root;
    }

};

/*!
    Имплементация словаря
    Не допускается дублирование ключей (аналог std::map)
*/
template <typename Key, typename Value>
class Map
{
    BinarySearchTree<Key, Value> _tree;
public:
    using MapIterator = typename BinarySearchTree<Key,Value>::Iterator;
    using ConstMapIterator = typename BinarySearchTree<Key,Value>::ConstIterator;

    Map() = default;
    
    explicit Map(const Map& other)
    {
        _tree = other;
    }
    Map& operator=(const Map& other)
    {
        _tree = other;
    }

    explicit Map(Map&& other) noexcept
    {
        _tree = other;
    }
    Map& operator=(Map&& other) noexcept
    {
        _tree = other;
    }

    // вставить элемент с ключем key и значением value
    // если узел с ключем key уже представлен, то заменить его значение на value
    void insert(const Key& key, const Value& value)
    {
        MapIterator it(nullptr);  
        if((it = _tree.find(key)) !=
                     MapIterator(nullptr))
        {
            (*it).second = value;
            return;
        }
        _tree.insert(key, value);
    }

    // удалить элемент с ключем key
    void erase(const Key& key)
    {
        _tree.erase(key);
    }

    // найти элемент, равный ключу key
    ConstMapIterator find(const Key& key) const
    {
        return _tree.cfind(key);
    }
    MapIterator find(const Key& key)
    {
        return _tree.find(key);
    }

    // доступ к элементу по ключу
    // если в момент обращения элемента не существует, создать его, 
    // ключ равен key, value равно дефолтному значению для типа Value
    const Value& operator[](const Key& key) const
    {
        return (*find(key).second);
    }
    Value& operator[](const Key& key)
    {
        MapIterator it;
        if((it = _tree.find(key)) !=
                     MapIterator(nullptr))
        {
            Value value;
            insert(key, value);
        }

        return (*find(key).second);
    }

    MapIterator begin()
    {
        return _tree.begin();
    }
    MapIterator end()
    {
        return _tree.end();
    }

    ConstMapIterator cbegin() const
    {
        return _tree.cbegin();
    }
    ConstMapIterator cend() const
    {
        return _tree.cend();
    }

    size_t size() const
    {
        return _tree.size();
    }
};

 template <typename Value>
class Set
{
    Map<Value, Value> _map;

public:
    using SetIterator = typename Map<Value, Value>::MapIterator;
    using ConstSetIterator = typename Map<Value, Value>::ConstMapIterator;

    Set() = default;

    explicit Set(const Set& other)
    {
        _map = other;
    }
    Set& operator=(const Set& other)
    {
        _map = other;
    }

    explicit Set(Set&& other) noexcept
    {
        _map = other;
    }
    Set& operator=(Set&& other) noexcept
    {
        _map = other;
    }

    ~Set() = default;

    void insert(const Value& value)
    {
        _map.insert(value, value);
    }

    void erase(const Value& value)
    {
        _map.erase(value);
    }

    ConstSetIterator find(const Value& value) const
    {
        return _map.find(value);
    }
    SetIterator find(const Value& value)
    {
        return _map.find(value);
    }

    bool contains(const Value& value) const
    {
        SetIterator it;
        if((it = _map.find(value)) !=
                     SetIterator(nullptr))
        {
            return false;
        }
        return true;
    }
};


