#pragma warning(disable : 4996)
#include<iostream>

//rangetree oj 100/100

//用于VSCode中代码调试
//#define _DEBUG_
#ifdef _DEBUG_
#include"temperature_lib.c"
#else
#include"temperature.h"
int n, m;
#endif
#define RANGE_TREE_INFINITY 0x7F7F7F7F7F7F7F7FLL

//#define _USING_LIB_  标准库用于调试中排查问题
#ifdef _USING_LIB_
#include<vector>
#include<algorithm>
#endif
#ifndef _VECTOR_
//标准库std::vector的手动实现
template <typename T>
class Vector
{
    int64_t _size, _capacity;
    T* _data;
    bool expand()
    {
        if (_size > _capacity)
        {
            while (_size >= _capacity)
                _capacity = (_capacity < 1024) ? _capacity * 4 : _capacity * 2;
            T* new_data = new T[_capacity];
            if (new_data) {
                for (int64_t i = 0; i < _size; i++)
                    new_data[i] = std::move(_data[i]);
                delete[] _data;
                _data = new_data;

                return true;
            }
            else return false;
        }
        return true;
    }
    bool shrink()
    {
        if (_size > 0 && _size < _capacity / 4)
        {
            while (_size < _capacity / 4)
                _capacity /= 2;
            T* new_data = new T[_capacity];
            if (new_data) {
                for (int64_t i = 0; i < _size; i++) {
                    new_data[i] = std::move(_data[i]);//不会溢出 忽略C6386
                }
                delete[] _data;
                _data = new_data;
                return true;
            }
            else return false;
        }
        return true;
    }
public:
    Vector() : _size(0), _capacity(1) { _data = new T[_capacity]; }
    Vector(const Vector& other) {
        // if (this == &other) return;构造函数不需要检查自赋值
        _size = other._size; _capacity = other._capacity;
        _data = new T[_capacity];
        for (int64_t i = 0; i < _size; i++)
            _data[i] = other._data[i];//忽略C6386
        //std::memcpy(_data, other._data, _size * sizeof(T)); 不安全
    }
    ~Vector() { delete[] _data; }
    bool empty() const noexcept { return _size == 0; }
    T& front() noexcept { return _data[0]; }
    T& back() const noexcept { return _data[_size - 1]; }
    void push_back(const T& elem)
    {
        _size++;
        if (expand())
            _data[_size - 1] = elem;
        else {
            _size--;
            throw std::runtime_error("Error In Vector::push_back: push failed\n");
        }
    }
    void emplace_back(T& elem)
    {
        _size++;
        if (expand())
            _data[_size - 1] = std::move(elem);
        else {
            _size--;
            throw std::runtime_error("Error In Vector::emplace_back: push failed\n");
        }
    }
    void pop_back()
    {
        if (empty())
            throw std::runtime_error("Error In Vector::pop_back: attempts to pop from an empty Vector!\n");
        T tmp = _data[--_size];
        shrink();
    }
    T& operator[](int64_t index)const noexcept
    {
        return _data[index];
    }
    Vector& operator=(const Vector& other) {
        if (this == &other) return *this;
        _size = other._size; _capacity = other._capacity;
        _data = new T[_capacity];
        for (int64_t i = 0; i < _size; i++)
            _data[i] = other._data[i];//忽略C6386
        //std::memcpy(_data, other._data, _size * sizeof(T)); 不安全
        return *this;
    }
    int64_t size() const noexcept { return _size; }
    int64_t capacity() const noexcept { return _capacity; }
    void reserve(int64_t new_capacity) {
        if (new_capacity > _capacity) {
            _capacity = new_capacity;
            T* new_data = new T[_capacity];
            if (new_data) {
                for (int64_t i = 0; i < _size; i++)
                    new_data[i] = std::move(_data[i]); // 减少拷贝
                delete[] _data;
                _data = new_data;
            }
            else {
                throw std::runtime_error("Reserve memory failed");
            }
        }
    }
    void resize(int64_t new_size, const T& value = T()) {
        int64_t tmp = _size;
        _size = new_size;
        expand();
        while (tmp < new_size)
            _data[tmp++] = value;
    }

public:
    // 支持 std::initializer_list 的构造函数
    Vector(std::initializer_list<T> init) : _size(init.size()), _capacity(init.size() > 0 ? init.size() * 2 : 1) {
        _data = new T[_capacity];
        int64_t i = 0;
        for (const auto& elem : init) {
            if (i < _capacity)
                _data[i] = elem;
            i++;
        }
    }
    // 嵌套迭代器类
    class iterator {
        T* _ptr; // 指向当前元素的指针
    public:
        explicit iterator(T* ptr) : _ptr(ptr) {}

        // 解引用
        T& operator*() const { return *_ptr; }
        T* operator->() const { return _ptr; }

        // 前置递增
        iterator& operator++() {
            ++_ptr;
            return *this;
        }

        // 后置递增
        iterator operator++(int) {
            iterator temp = *this;
            ++_ptr;
            return temp;
        }

        // 前置递减（可选）
        iterator& operator--() {
            --_ptr;
            return *this;
        }

        // 后置递减（可选）
        iterator operator--(int) {
            iterator temp = *this;
            --_ptr;
            return temp;
        }

        // 比较操作
        bool operator==(const iterator& other) const { return _ptr == other._ptr; }
        bool operator!=(const iterator& other) const { return _ptr != other._ptr; }

        // 随机访问（可选）
        iterator operator+(int64_t n) const noexcept{ return iterator(_ptr + n); }
        iterator operator-(int64_t n) const noexcept{ return iterator(_ptr - n); }
        int64_t operator-(const iterator& other) const noexcept{ return _ptr - other._ptr; }
        bool operator<(const iterator& other) const noexcept{ return _ptr < other._ptr; }
        bool operator<=(const iterator& other) const noexcept{ return _ptr <= other._ptr; }
        bool operator>(const iterator& other) const noexcept{ return _ptr > other._ptr; }
        bool operator>=(const iterator& other) const noexcept{ return _ptr >= other._ptr; }

    };
    iterator begin() {
        return iterator(_data);
    }
    iterator end() {
        return iterator(_data + _size);
    }
};
#else
template<typename T>
using Vector = std::vector<T>;
#endif
//模板化的堆排序下滤操作
template <typename TData>
static void percolateDown(Vector<TData>& target, int64_t idx, int64_t heap_size) {
    int64_t idc = 2 * idx + 1;
    while (idc < heap_size) {
        if (idc < heap_size - 1)
            idc = target[idc] >= target[idc + 1] ? idc : idc + 1;
        if (target[idx] >= target[idc]) break;//C2676
        auto tmp = target[idc];
        target[idc] = target[idx];
        target[idx] = tmp;
        idx = idc;
        idc = 2 * idx + 1;
    }
}
//模板化的堆排序实现
template <typename TData>
void heapSort(Vector<TData>& target) {
    //create heap;
    int64_t size = target.size();
    int64_t idp = (size - 2) / 2;
    int64_t t = size;
    while (idp >= 0)
        percolateDown<TData>(target, idp--, t);

    while (--t > 0) {
        auto tmp = target[0];
        target[0] = target[t];
        target[t] = tmp;
        percolateDown<TData>(target, 0, t);
    }
}
//模板化的有序向量归并，用于归并左右子树的排序y值
template<typename T>
Vector<T> merge(const Vector<T>& v1, const Vector<T>& v2) {
    if (v2.empty()) return v1;
    if (v1.empty()) return v2;
    Vector<T> merged;
    merged.reserve(v1.size() + v2.size());
    int64_t id1 = 0, id2 = 0;
    while (id1 < v1.size() || id2 < v2.size()) {
        if (id2 >= v2.size() || (id1 < v1.size() && v1[id1] <= v2[id2])) {
            merged.push_back(v1[id1++]);
        }
        else {
            merged.push_back(v2[id2++]);
        }
    }
    return merged;
}
//小于等于函数的模板定义,用于二分查找
template<typename T1, typename T2 >
using PredLE = bool (*)(const T1&, const T2&);
//模板化的二分查找函数
template<typename T1, typename T2 >
int64_t binSearch(Vector<T1>& sorted_vector, int64_t lo, int64_t hi, T2 elem, PredLE<T1, T2> le) {

    while (lo < hi) {
        int64_t mi = (lo + hi) / 2;
        if (le(sorted_vector[mi], elem))
            lo = mi + 1;
        else hi = mi;
    }
    return lo - 1;
}
//站点结构体，只用在了参数传递中
struct Station {
    int x, y, temp;
};
//用于存储y值与对应的温度.在累加量中_temp也用于存储温度累加值
struct y_and_temp {//y and val
    int64_t _y, _temp;
    bool operator>=(const y_and_temp& other) const noexcept { return _y >= other._y; }
    bool operator>(const y_and_temp& other) const noexcept { return _y > other._y; }
    bool operator<=(const y_and_temp& other) const noexcept { return _y <= other._y; }
    bool operator<(const y_and_temp& other) const noexcept { return _y < other._y; }
    bool operator==(const y_and_temp& other) const noexcept { return _y == other._y; }
    bool operator!=(const y_and_temp& other) const noexcept { return _y != other._y; }
};
//树节点
struct RTNode {
    enum Color {//红黑树
        RED = 0,
        BLACK = 1
    } _color;
    int64_t _x;
    int64_t _min_x, _max_x;//子树的x范围
    RTNode* _left, * _right, * _parent;//遍历指针

    Vector< y_and_temp> _node_y_temp_data;//同一_x值的y与temp数据
    Vector< y_and_temp>_accumulated, _sorted_subtree_y;//左右子树包括自己的，y_temp值，以及相应的累加量 
public:
    RTNode() :_x(-1), _left(0), _right(0), _parent(0), _color(BLACK), _min_x(RANGE_TREE_INFINITY), _max_x(-RANGE_TREE_INFINITY) {};
    static void connect34(RTNode* t1, RTNode* t2, RTNode* t3, RTNode* t4, RTNode* a, RTNode* b, RTNode* c) {//3+4重构
        a->_parent = b, c->_parent = b;
        b->_left = a, b->_right = c;
        if (t1)t1->_parent = a;
        if (t2)t2->_parent = a;
        if (t3)t3->_parent = c;
        if (t4)t4->_parent = c;

        a->_left = t1, a->_right = t2, c->_left = t3, c->_right = t4;
        a->_color = c->_color = RED;
        b->_color = BLACK;
    }
    static void reColor(RTNode* node) {//红黑树平衡
        if (node->_parent && node->_parent->_color == RED && node->_color == RED) {
            RTNode* p = node->_parent;
            RTNode* g = p->_parent;
            RTNode* u = (g->_left == p) ? g->_right : g->_left;
            //根据不同的拓扑关系，使用相同的3+4同构
            if (!u || u->_color == BLACK) {
                if (p == g->_left) {
                    if (node == p->_left) {
                        RTNode* t1 = node->_left, * t2 = node->_right, * t3 = p->_right, * t4 = g->_right;
                        RTNode* tmp = g->_parent;
                        if (tmp) (tmp->_left == g ? tmp->_left : tmp->_right) = p;
                        p->_parent = tmp;
                        connect34(t1, t2, t3, t4, node, p, g);

                    }
                    else {
                        RTNode* t1 = p->_left, * t2 = node->_left, * t3 = node->_right, * t4 = g->_right;
                        RTNode* tmp = g->_parent;
                        if (tmp) (tmp->_left == g ? tmp->_left : tmp->_right) = node;
                        node->_parent = tmp;
                        connect34(t1, t2, t3, t4, p, node, g);
                    }
                }
                else {//p == g->right
                    if (node == p->_left) {
                        RTNode* t1 = g->_left, * t2 = node->_left, * t3 = node->_right, * t4 = p->_right;
                        RTNode* tmp = g->_parent;
                        if (tmp) (tmp->_left == g ? tmp->_left : tmp->_right) = node;
                        node->_parent = tmp;
                        connect34(t1, t2, t3, t4, g, node, p);
                    }
                    else {//node == p->right
                        RTNode* t1 = g->_left, * t2 = p->_left, * t3 = node->_left, * t4 = node->_right;
                        RTNode* tmp = g->_parent;
                        if (tmp) (tmp->_left == g ? tmp->_left : tmp->_right) = p;
                        p->_parent = tmp;
                        connect34(t1, t2, t3, t4, g, p, node);
                    }
                }
            }
            else {//u->_color == RED
                g->_color = RED;
                u->_color = BLACK;
                p->_color = BLACK;
                if (g->_parent) {//g如果发生双红缺陷，它必有祖父gg
                    reColor(g);
                }
                else {
                    g->_color = BLACK;//根节点必须是黑色 如果g是根，把g染黑意味着整棵树高度都增加了1，仍然是平衡的
                }
            }
        }
    }
    //从Station添加数据,只是在相同x的节点中添加进向量，并没有排序
    static RTNode* addStation(RTNode* root, RTNode* parent, const Station& s) {
        if (NULL == root) {
            root = new RTNode();
            if (NULL == parent)
                root->_color = BLACK;
            else root->_color = RED;

            root->_parent = parent;
            root->_x = s.x;

            y_and_temp t; t._y = s.y, t._temp = s.temp;
            root->_node_y_temp_data.push_back(t);

            if (parent) {
                if (parent->_x > root->_x)
                    parent->_left = root;
                else
                    parent->_right = root;
            }
            reColor(root);
            return root;
        }
        else {
            if (root->_x == s.x) {
                y_and_temp t; t._y = s.y, t._temp = s.temp;
                root->_node_y_temp_data.push_back(t);
                return root;
            }
            else {
                if (root->_x > s.x)
                    return addStation(root->_left, root, s);
                else return addStation(root->_right, root, s);
            }

        }

    }
    //原来的函数，用于堆节点的数据进行排序以进行二分查找
    void updateY() {//decrepted
        Vector<RTNode*>stack;
        stack.push_back(this);

        while (!stack.empty()) {
            RTNode* p = stack.back();
            stack.pop_back();
            heapSort<y_and_temp>(p->_node_y_temp_data);
            if (p->_left) stack.push_back(p->_left);
            if (p->_right) stack.push_back(p->_right);
        }
    }
    //新的策略，每个父节点，增加了自己的子树的所有节点信息,预先算出累加值，使得当前子树累加操作可能在log时间内完成
    void updateRangeAccumulation() {
        Vector<RTNode*> stack_in, stack_out;
        stack_in.push_back(this);
        while (!stack_in.empty()) {
            auto top = stack_in.back();
            stack_in.pop_back();
            if (top->_right) stack_in.push_back(top->_right);
            if (top->_left)stack_in.push_back(top->_left);
            stack_out.push_back(top);
        }
        while (!stack_out.empty()) {
            RTNode* top = stack_out.back();
            stack_out.pop_back();

            //下面都是针对top的操作，如果有任何一个没有通过top->访问的成员，那么一定是错误
            top->_min_x = top->_max_x = top->_x;
            if (top->_left) {
                top->_min_x = std::min(top->_left->_min_x, top->_min_x);
                top->_max_x = std::max(top->_left->_max_x, top->_max_x);
            }
            if (top->_right) {
                top->_min_x = std::min(top->_right->_min_x, top->_min_x);
                top->_max_x = std::max(top->_right->_max_x, top->_max_x);
            }
            heapSort<y_and_temp>(top->_node_y_temp_data);
            top->_sorted_subtree_y = top->_node_y_temp_data;
            if (top->_left)//include sun_tree data
                top->_sorted_subtree_y = merge<y_and_temp>(top->_sorted_subtree_y, top->_left->_sorted_subtree_y);
            if (top->_right)//include sun_tree data
                top->_sorted_subtree_y = merge<y_and_temp>(top->_sorted_subtree_y, top->_right->_sorted_subtree_y);
            if (top->_sorted_subtree_y.size()) {
                top->_accumulated.reserve(top->_sorted_subtree_y.size());
                top->_accumulated.push_back(top->_sorted_subtree_y[0]);
                for (int64_t i = 1; i < top->_sorted_subtree_y.size(); ++i) {
                    y_and_temp tmp = top->_sorted_subtree_y[i];
                    tmp._temp += top->_accumulated.back()._temp;
                    top->_accumulated.emplace_back(tmp);
                }
            }

        }
    }
    //旧的策略，超时
    int64_t getSum(int64_t x1, int64_t y1, int64_t x2, int64_t y2, int64_t* count) {
        int64_t sum = 0;
        if (_x >= x1 && _x <= x2) {
            int64_t id = binSearch<y_and_temp, int64_t>(_node_y_temp_data, 0, _node_y_temp_data.size(), y2, [](const y_and_temp& a, const int64_t& b) {return a._y <= b; }) + 1;
            while (--id >= 0) {
                if (_node_y_temp_data[id]._y >= y1 && _node_y_temp_data[id]._y <= y2) {
                    sum += _node_y_temp_data[id]._temp;
                    (*count)++;
                }
                else break;
            }
        }
        if (_x > x1 && _left)
            sum += _left->getSum(x1, y1, x2, y2, count);
        if (_x < x2 && _right)
            sum += _right->getSum(x1, y1, x2, y2, count);
        return sum;
    }
    int64_t getSumFast(int64_t x1, int64_t y1, int64_t x2, int64_t y2, int64_t* count) {
        Vector<RTNode*> stack;
        int64_t sum = 0;
        PredLE<y_and_temp, int64_t> pred = [](const y_and_temp& a, const int64_t& b) {return a._y <= b; };//小于等于函数，用于binSearch
        stack.push_back(this);
        while (!stack.empty()) {
            RTNode* top = stack.back();
            stack.pop_back();
            // 剪枝操作：如果当前节点的 x 范围与查询范围没有重叠，则跳过
            if (top->_max_x < x1 || top->_min_x > x2)
                continue;  // 没有重叠，跳过当前节点
            if (top->_min_x >= x1 && top->_max_x <= x2) {//新的策略，如果当前子树所有的x满足要求，通过二分与累加值快速找到符合条件的y对应的temp的和
                int64_t id2 = binSearch<y_and_temp, int64_t>(top->_accumulated, 0, top->_accumulated.size(), y2, pred);
                int64_t id1 = binSearch<y_and_temp, int64_t>(top->_accumulated, 0, top->_accumulated.size(), y1 - 1, pred);
                if (id1 > -1)
                    sum += top->_accumulated[id2]._temp - top->_accumulated[id1]._temp;
                else
                    if (id2 > -1)
                        sum += top->_accumulated[id2]._temp;
                (*count) += id2 - id1;
                continue;//少了继续向子树递归的开销
            }
            else {//剪枝，实际上就是旧的策略
                if (top->_x >= x1 && top->_x <= x2) {
                    int64_t id = binSearch<y_and_temp, int64_t>(top->_node_y_temp_data, 0, top->_node_y_temp_data.size(), y2, pred);
                    while (id >= 0) {
                        if (top->_node_y_temp_data[id]._y <= y2 && top->_node_y_temp_data[id]._y >= y1) {
                            sum += top->_node_y_temp_data[id--]._temp;
                            (*count)++;
                        }
                        else break;
                    }
                }
                if (top->_left != nullptr && top->_left->_max_x >= x1)
                    stack.push_back(top->_left);  // 左子树可能有重叠，继续遍历
                if (top->_right != nullptr && top->_right->_min_x <= x2)
                    stack.push_back(top->_right);  // 右子树可能有重叠，继续遍历
            }
        }
        return sum;
    }
    void destroyTree() {//销毁子树
        if (_left)_left->destroyTree();
        if (_right)_right->destroyTree();
        delete this;//!!!
    }
};
//范围树的封装(不要太在意这是不是范围树
class RangeTree {
    RTNode* _root;
public:
    RangeTree() :_root(NULL) {};
    ~RangeTree() { _root->destroyTree(); };
    void  addStation(Station& s) {
        if (!_root)
            _root = _root->addStation(_root, NULL, s);
        else
            _root->addStation(_root, NULL, s);
        while (_root && _root->_parent)_root = _root->_parent;
    }
    void updateRangeAccumulation() {//更新累加值，调用getSumFast的前提
        _root->updateRangeAccumulation();
    }
    void updateY() {//getSum的前提
        _root->updateY();
    }
    int64_t getSumFast(int x1, int y1, int x2, int y2, int64_t* count) {
        if (!_root) return 0;
        return _root->RTNode::getSumFast(x1, y1, x2, y2, count);
    }
    int64_t getSum(int x1, int y1, int x2, int y2, int64_t* count) {
        if (!_root) return 0;
        return _root->RTNode::getSum(x1, y1, x2, y2, count);
    }
};

int main()
{
    int x1, y1, x2, y2;
    Station s;
    n = GetNumOfStation();
    RangeTree tree;
    for (int no = 0; no < n; no++)
    {
        GetStationInfo(no, &s.x, &s.y, &s.temp);
        tree.addStation(s);
    }
    tree.updateRangeAccumulation();
    int64_t count = 0;
    while (GetQuery(&x1, &y1, &x2, &y2))
    {
        if (n == 0) {
            Response(0);
            continue;
        }
        int64_t count = 0;
        int64_t sum = tree.getSumFast(x1, y1, x2, y2, &count);
        Response(count ? sum / count : 0);
    }
    return 0;
}