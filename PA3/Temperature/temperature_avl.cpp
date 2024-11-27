#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cassert>
#include <initializer_list>

#include <cstdlib>
#include <cstring>

const long long inf = 0x3f3f3f3f3f3f;
const int MAX_N = 1e5;


struct station;

#define _DEBUG_
#ifdef _DEBUG_
#include"temperature_lib.c"
#else
#include"temperature.h"
#endif

//#define _USING_LIB_  //控制是否使用库

#ifdef _USING_LIB_
#include<vector>
#include<algorithm>
#endif

#ifndef _VECTOR_
template <typename T>
class Vector
{
    size_t _size, _capacity;
    T* _data;
    bool expand()
    {
        if (_size > _capacity)
        {
            while(_size >= _capacity)
                _capacity = (_capacity < 1024) ? _capacity * 4 : _capacity * 2;
            T* new_data = new T[_capacity];
            if (new_data) {
                for (size_t i = 0; i < _size; i++)
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
            while(_size < _capacity / 4)
                _capacity /= 2;
            T* new_data = new T[_capacity];
            if (new_data) {
                for (size_t i = 0; i < _size; i++) {
                    assert(_size < _capacity);
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
    ~Vector() { delete[] _data; }
    bool empty() const noexcept{ return _size == 0; }
    T& front() noexcept { return _data[0]; }
    T& back() const noexcept { return _data[_size-1]; }
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
    void emplace_back(T&& elem)
    {
        _size++;
        if (expand())
            _data[_size - 1] = std::move(elem);
        else{
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
    T& operator[](size_t index)const noexcept
    {
        return _data[index];
    }
    size_t size() const noexcept{ return _size; }
    size_t capacity() const noexcept{ return _capacity; }
    void reserve(size_t new_capacity) {
        if (new_capacity > _capacity) {
            _capacity = new_capacity;
            T* new_data = new T[_capacity];
            if (new_data) {
                assert(_size < _capacity);
                for (size_t i = 0; i < _size; i++)
                    new_data[i] = std::move(_data[i]); // 减少拷贝
                delete[] _data;
                _data = new_data;
            }
            else {
                throw std::runtime_error("Reserve memory failed");
            }
        }
    }
    void resize(size_t new_size, const T& value = T()){
        size_t tmp = _size;
        _size = new_size;
        expand();
        while (tmp < new_size)
            _data[tmp++] = value;
    }

public:
    // 支持 std::initializer_list 的构造函数
    Vector(std::initializer_list<T> init) : _size(init.size()), _capacity(init.size() > 0 ? init.size() * 2 : 1) {
        _data = new T[_capacity];
        size_t i = 0;
        for (const auto& elem : init) {
            if(i<_capacity)
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
        iterator operator+(size_t n) const { return iterator(_ptr + n); }
        iterator operator-(size_t n) const { return iterator(_ptr - n); }
        size_t operator-(const iterator& other) const { return _ptr - other._ptr; }
        bool operator<(const iterator& other) const { return _ptr < other._ptr; }
        bool operator<=(const iterator& other) const { return _ptr <= other._ptr; }
        bool operator>(const iterator& other) const { return _ptr > other._ptr; }
        bool operator>=(const iterator& other) const { return _ptr >= other._ptr; }

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


int64_t sum, count;

struct station {//y and val
    int y, val;

    bool operator>(station &other){return y>other.y;}
    bool operator>=(station &other){return y>=other.y;}
    bool operator<(station &other){return y<other.y;}
    bool operator<=(station &other){return y<=other.y;}
    bool operator==(station &other){return y==other.y;}
    bool operator!=(station &other){return y!=other.y;}
} ;

typedef struct Node {
    int _banlance_facter;
    int64_t _x, _y, _temperature;
    int64_t maxx, minx;

    Node() {
        maxx = -inf;
        minx = inf;
    }

    Vector<long long> ysorted, temp, d;
    Vector<station> xtemp;
    Node *_left_child, *_right_child;
} Node, *Tree;

void LR(Tree *p) { //左旋
    Tree R;
    R = (*p)->_right_child;
    (*p)->_right_child = R->_left_child;
    R->_left_child = (*p);
    *p = R;
}

void RR(Tree *p) { //右旋
    Tree L;
    L = (*p)->_left_child;
    (*p)->_left_child = L->_right_child;
    L->_right_child = (*p);
    *p = L;
}

void LB(Tree *T) {
    Tree L, Lr;
    L = (*T)->_left_child;
    switch (L->_banlance_facter) {
        case 1://新节点插入在T的左孩子的左子树上，做单右旋处理
            (*T)->_banlance_facter = L->_banlance_facter = 0;
            RR(T);
            break;
        case -1://新插入节点在T的左孩子的右子树上，做双旋处理
            Lr = L->_right_child;
            switch (Lr->_banlance_facter) {
                case 1:
                    (*T)->_banlance_facter = -1;
                    L->_banlance_facter = 0;
                    break;
                case 0:
                    (*T)->_banlance_facter = L->_banlance_facter = 0;
                    break;
                case -1:
                    (*T)->_banlance_facter = 0;
                    L->_banlance_facter = 1;
                    break;
            }
            Lr->_banlance_facter = 0;
            LR(&(*T)->_left_child);
            RR(T);
    }
}

void RB(Tree *T) {
    Tree R, Rl;
    R = (*T)->_right_child;
    switch (R->_banlance_facter) {
        case -1://新节点插在T的右孩子的右子树上，要做单左旋处理
            (*T)->_banlance_facter = R->_banlance_facter = 0;
            LR(T);
            break;
        case 1://新节点插在T的右孩子的左子树上，要做双旋处理
            Rl = R->_left_child;
            switch (Rl->_banlance_facter) {
                case 1:
                    (*T)->_banlance_facter = 0;
                    R->_banlance_facter = -1;
                    break;
                case 0:
                    (*T)->_banlance_facter = R->_banlance_facter = 0;
                    break;
                case -1:
                    (*T)->_banlance_facter = 1;
                    R->_banlance_facter = 0;
                    break;
            }
            Rl->_banlance_facter = 0;
            RR(&(*T)->_right_child);
            LR(T);
    }
}

bool insert(Tree *T, int x, int y, int temperature, bool *taller) { //变量taller反应T长高与否
    if (!*T) {
        *T = new Node;
        (*T)->_x = x;
        (*T)->_y = y;
        (*T)->_temperature = temperature;
        (*T)->_left_child = (*T)->_right_child = NULL;
        (*T)->_banlance_facter = 0;
        (*T)->xtemp.push_back({y, temperature});

        *taller = true;
    } else {
        if (x == (*T)->_x) { //不插入,将y和val加入
            *taller = false;
            (*T)->xtemp.push_back({y, temperature});
            return false;
        }
        if (x < (*T)->_x) {
            //以下为左子树插入
            if (!insert(&(*T)->_left_child, x, y, temperature, taller))//未插入
                return false;
            if (*taller) {  //插入左子树，左子树深度增加
                switch ((*T)->_banlance_facter) {
                    case 1://深度若为1，则开始调整
                        LB(T);
                        *taller = false;
                        break;
                    case 0://左右子树等深，左子树变深
                        (*T)->_banlance_facter = 1;
                        *taller = true;
                        break;
                    case -1://右子树比左子树深，左右子树等深
                        (*T)->_banlance_facter = 0;
                        *taller = false;
                        break;
                }
            }
        } else {
            //以下为右子树插入
            if (!insert(&(*T)->_right_child, x, y, temperature, taller))
                return false;
            if (*taller) { //插入右子树，右子树深度增加
                switch ((*T)->_banlance_facter) {
                    case 1://左子树比右子树深，左右子树等深
                        (*T)->_banlance_facter = 0;
                        *taller = false;
                        break;
                    case 0://左右子树等深，右子树变深
                        (*T)->_banlance_facter = -1;
                        *taller = true;
                        break;
                    case -1://深度若为-1，则开始调整
                        RB(T);
                        *taller = false;
                        break;
                }
            }
        }
    }
    return true;
}

inline int64_t Max(int64_t a, int64_t b) {
    if (a > b) return a;
    else return b;
}

inline int64_t Min(int64_t a, int64_t b) {
    if (a < b) return a;
    else return b;
}

static void percolateDown(Tree *T,int idx,int heap_size) {
    auto& _station_pool = (*T)->xtemp;
    int idc = 2 * idx + 1;
    while (idc < heap_size) {
        if (idc < heap_size - 1)
            idc = _station_pool[idc]>= _station_pool[idc + 1] ? idc : idc + 1;
        if (_station_pool[idx] >= _station_pool[idc]) break;
        station tmp = _station_pool[idc];
        _station_pool[idc] = _station_pool[idx];
        _station_pool[idx] = tmp;
        idx = idc;
        idc = 2 * idx + 1;
    }
}
static void heapSort(Tree *T) {
    //create heap;
    size_t station_size = (*T)->xtemp.size();
    auto& _station_pool = (*T)->xtemp;
    int idp = (station_size - 2) / 2;
    int t = station_size;
    while (idp >= 0) 
        percolateDown(T,idp--, t);

    while (--t > 0) {
        station tmp = _station_pool[0];
        _station_pool[0] = _station_pool[t];
        _station_pool[t] = tmp;

        percolateDown(T,0, t);
    }
}

void build(Tree *T) {
    if ((*T) == NULL) return;

    build(&((*T)->_left_child));
    build(&((*T)->_right_child));


    (*T)->maxx = (*T)->_x;
    (*T)->minx = (*T)->_x;
    if ((*T)->_left_child != NULL) {
        (*T)->maxx = Max((*T)->maxx, (*T)->_left_child->maxx);
        (*T)->minx = Min((*T)->minx, (*T)->_left_child->minx);
    }
    if ((*T)->_right_child != NULL) {
        (*T)->maxx = Max((*T)->maxx, (*T)->_right_child->maxx);
        (*T)->minx = Min((*T)->minx, (*T)->_right_child->minx);
    }
    heapSort(T);
    Vector<int64_t> ytemp, valtemp;
    int lsize = 0, rsize = 0, tsize = 0;
    if ((*T)->_left_child != NULL)
        lsize = (*T)->_left_child->ysorted.size() - 1;
    if ((*T)->_right_child != NULL)
        rsize = (*T)->_right_child->ysorted.size() - 1;
    tsize = (*T)->xtemp.size();
    int nowt, nowl, nowr;
    nowt = 0, nowl = 1;
    while (nowt < tsize && nowl < lsize) {
        if ((*T)->_left_child->ysorted[nowl] < (*T)->xtemp[nowt].y) {
            ytemp.push_back((*T)->_left_child->ysorted[nowl]);
            valtemp.push_back((*T)->_left_child->temp[nowl++]);
        } else {
            ytemp.push_back((*T)->xtemp[nowt].y);
            valtemp.push_back((*T)->xtemp[nowt++].val);
        }
    }
    while (nowt < tsize) {
        ytemp.push_back((*T)->xtemp[nowt].y);
        valtemp.push_back((*T)->xtemp[nowt++].val);
    }
    while (nowl < lsize) {
        ytemp.push_back((*T)->_left_child->ysorted[nowl]);
        valtemp.push_back((*T)->_left_child->temp[nowl++]);
    }
    int to = ytemp.size();
    nowr = 1, nowt = 0;
    (*T)->ysorted.push_back(-inf);
    (*T)->temp.push_back(0);
    while (nowr < rsize && nowt < to) {
        if ((*T)->_right_child->ysorted[nowr] < ytemp[nowt]) {
            (*T)->ysorted.push_back((*T)->_right_child->ysorted[nowr]);
            (*T)->temp.push_back((*T)->_right_child->temp[nowr++]);
        } else {
            (*T)->ysorted.push_back(ytemp[nowt]);
            (*T)->temp.push_back(valtemp[nowt++]);
        }
    }
    while (nowr < rsize) {
        (*T)->ysorted.push_back((*T)->_right_child->ysorted[nowr]);
        (*T)->temp.push_back((*T)->_right_child->temp[nowr++]);
    }
    while (nowt < to) {
        (*T)->ysorted.push_back(ytemp[nowt]);
        (*T)->temp.push_back(valtemp[nowt++]);
    }
    to = (*T)->ysorted.size();
    (*T)->ysorted.push_back(inf);
    (*T)->temp.push_back(0);
    (*T)->d.push_back(0);
    for (int i = 1; i < to; i++) {
        (*T)->d.push_back((*T)->d[i - 1] + (*T)->temp[i]);
    }
}

void slove(Tree *T, int y1, int y2) {
    int l = 0, r = (*T)->ysorted.size() - 1;
    if ((*T)->ysorted[1] > y2 || (*T)->ysorted[r - 1] < y1) return;
    int mid;
    int from, to;
    while (l < r) {
        mid = (l + r + 1) >> 1;
        if ((*T)->ysorted[mid] >= y1) r = mid - 1;
        else l = mid;
    }
    from = l;

    l = 0, r = (*T)->ysorted.size() - 1;
    while (l < r) {
        mid = (l + r) >> 1;
        if ((*T)->ysorted[mid] <= y2) l = mid + 1;
        else r = mid;
    }
    to = l;
    count = count + to - from - 1;
    sum = sum + (*T)->d[to - 1] - (*T)->d[from];
}

void slove2(Tree *T, int y1, int y2) {
    for (int i = 0; i < (*T)->xtemp.size(); i++) {
        if ((*T)->xtemp[i].y >= y1 && (*T)->xtemp[i].y <= y2) {
            sum += (*T)->xtemp[i].val;
            count++;
        }
    }
}

void query(Tree *T, int x1, int y1, int x2, int y2) {
    if ((*T) == NULL) return;
    if ((*T)->minx >= x1 && (*T)->maxx <= x2) {
        slove(&(*T), y1, y2);
        return;
    }
    if ((*T)->maxx < x1 || (*T)->minx > x2) return;
    if ((*T)->_x >= x1 && (*T)->_x <= x2) {
        slove2(&(*T), y1, y2);
    }
    query(&(*T)->_left_child, x1, y1, x2, y2);
    query(&(*T)->_right_child, x1, y1, x2, y2);
}

int main() {
    //freopen("in.txt", "r", stdin);
    int n, q;
    n = GetNumOfStation();
    int x1, y1,x2,y2, temp;
    Node * _root = NULL;
    bool taller;
    for (int i = 0; i < n; i++) {
        GetStationInfo(i, &x1, &y1, &temp);
        insert(&_root, x1, y1, temp, &taller);
    }
    build(&_root);
    while (GetQuery(&x1, &y1, &x2, &y2)) {
        sum = 0, count = 0;
        query(&_root, x1, y1, x2, y2);
        if (count == 0) {
            Response(0);
        } else {
            Response(sum / count);
        }
    }
    return 0;
}
