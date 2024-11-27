#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cassert>
#include <initializer_list>
//#include<limits>//std::numeric_limits<T>::infinity()
#define RTREE_INFINITY 0x7F7F7F7F7F7F7F7FLL
#define _DEBUG_
#ifdef _DEBUG_
extern "C"
{
#include"temperature_lib.c"
}

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
            while (_size >= _capacity)
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
            while (_size < _capacity / 4)
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
    void emplace_back(T&& elem)
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
    T& operator[](size_t index)const noexcept
    {
        return _data[index];
    }
    size_t size() const noexcept { return _size; }
    size_t capacity() const noexcept { return _capacity; }
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
    void resize(size_t new_size, const T& value = T()) {
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
        iterator operator+(size_t n) const { return iterator(_ptr + n); }
        iterator operator-(size_t n) const { return iterator(_ptr - n); }
        size_t operator-(const iterator& other) const { return _ptr - other._ptr; }
        bool operator<(const iterator& other) const { return _ptr < other._ptr; }
        bool operator<=(const iterator& other) const { return _ptr <= other._ptr; }
        bool operator>(const iterator& other) const { return _ptr > other._ptr; }
        bool operator>=(const iterator& other) const { return _ptr >= other._ptr; }

        static size_t distance(const iterator& a, const iterator& b) {
            size_t d1 = (size_t)a._ptr, d2 = (size_t)b._ptr;

            return (d2 >= d1 ? d2 - d1 : d1 - d2) / sizeof(T);
        }
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
//typedef double DataType;
typedef int64_t DataType;

struct Station
{
    int _x, _y;
    int64_t _temp;
public:
    explicit Station(int x, int y, size_t temp) :_x(x), _y(y), _temp(temp) { }
    int posX()  const noexcept { return _x; }
    int posY() const noexcept { return _y; }
    int64_t Temperature()  const noexcept { return _temp; }
};

struct Rectangle {
    DataType _min_x, _min_y, _max_x, _max_y;
    explicit Rectangle() :_min_x(0), _min_y(0), _max_x(0), _max_y(0) {};
    explicit Rectangle(DataType min_x, DataType min_y, DataType max_x, DataType max_y) :_min_x(min_x), _min_y(min_y), _max_x(max_x), _max_y(max_y) {};

    bool intersects(const Rectangle& other)const noexcept {
        return !(
            other._min_x > _max_x || other._max_x < _min_x ||
            other._min_y > _max_y || other._max_y < _min_y
            );
    }
    bool include(const Rectangle& other)const noexcept {
        return _min_x <= other._min_x && _min_y <= other._min_y &&
            _max_x >= other._max_x && _max_y >= other._max_y;
    }
    void extend(const Rectangle& other) noexcept {
        _min_x = std::min(other._min_x, _min_x);
        _min_y = std::min(other._min_y, _min_y);
        _max_x = std::max(other._max_x, _max_x);
        _max_y = std::max(other._max_y, _max_y);
    }
    DataType extendArea(const Rectangle& other) const noexcept {
        DataType new_min_x = std::min(_min_x, other._min_x);
        DataType new_min_y = std::min(_min_y, other._min_y);
        DataType new_max_x = std::max(_max_x, other._max_x);
        DataType new_max_y = std::max(_max_y, other._max_y);

        // 计算扩展后的面积
        return (new_max_x - new_min_x) * (new_max_y - new_min_y);
    }
    DataType area() const noexcept { return (_max_x - _min_x) * (_max_y - _min_y); }

    DataType perimeter() const noexcept { return 2 * ((_max_x - _min_x) + (_max_y - _min_y)); }
    DataType overlap(const Rectangle& other) const noexcept {
        // 计算两个矩形的重叠面积
        DataType overlap_min_x = std::max(_min_x, other._min_x);
        DataType overlap_min_y = std::max(_min_y, other._min_y);
        DataType overlap_max_x = std::min(_max_x, other._max_x);
        DataType overlap_max_y = std::min(_max_y, other._max_y);

        if (overlap_min_x < overlap_max_x && overlap_min_y < overlap_max_y) {
            // 如果有重叠，则返回重叠的面积
            return (overlap_max_x - overlap_min_x) * (overlap_max_y - overlap_min_y);
        }
        else {
            return 0; // 没有重叠
        }
    }
    Rectangle& operator=(const Rectangle& other)noexcept {
        _min_x = other._min_x;
        _min_y = other._min_y;
        _max_x = other._max_x;
        _max_y = other._max_y;
        return *this;
    }
    bool operator==(const Rectangle& other)const noexcept {
        return
            _min_x == other._min_x &&
            _min_y == other._min_y &&
            _max_x == other._max_x &&
            _max_y == other._max_y;
    }
    Rectangle operator+(const Rectangle& other) const  noexcept {
        return Rectangle(
            std::min(other._min_x, _min_x),
            std::min(other._min_y, _min_y),
            std::max(other._max_x, _max_x),
            std::max(other._max_y, _max_y)
        );
    }


};
typedef Rectangle MBR;

struct Point :public Rectangle {
    explicit Point() :Rectangle() {}
    explicit Point(DataType x, DataType y) :Rectangle(x, y, x, y) {};
    DataType posX()const noexcept { return _max_x; }
    DataType posY()const noexcept { return _max_y; }
};

template<typename Tdata>
struct RTreeNode;

template<typename TData>
struct RTreeNodeEntry
{
    MBR _mbr;
    RTreeNode<TData>* _child;
    TData* _data;
    explicit RTreeNodeEntry(const MBR& mbr, const TData& data, RTreeNode<TData>* child) :_mbr(mbr), _data(new TData(data)), _child(child) {};
    ~RTreeNodeEntry() { if (_data) delete _data; }
};

template <typename TData>
using EntryPredFunc = bool (*)(RTreeNodeEntry<TData>*, RTreeNodeEntry<TData>*);

#ifndef _ALGORITHM_
// 插入排序实现（适用于小规模数据）
template <typename Iterable, typename PredFunc>
void insertionSort(Iterable first, Iterable last, PredFunc pred) {
    for (auto it = first + 1; it < last; ++it) {
        auto key = *it;
        auto j = it - 1;
        while (j >= first && pred(key, *j)) {
            *(j + 1) = *j;
            --j;
        }
        *(j + 1) = key;
    }
}

// 快速排序实现
template <typename Iterable, typename PredFunc>
void quickSort(Iterable first, Iterable last, PredFunc pred) {
    if (first >= last) return;

    auto pivot = *first;
    auto left = first + 1;
    auto right = last - 1;

    while (left <= right) {
        while (left <= right && pred(*left, pivot)) {
            ++left;
        }
        while (left <= right && !pred(*right, pivot)) {
            --right;
        }
        if (left < right) {
            std::swap(*left, *right);
        }
    }
    std::swap(*first, *right);

    // 递归调用排序
    quickSort(first, right, pred);
    quickSort(left, last, pred);
}

// 排序接口函数（选择策略）
template <typename Iterable, typename PredFunc, typename TData>
static void Sort(Iterable first, Iterable last, PredFunc pred) {
    const size_t threshold = 20;  // 小规模数据的阈值，具体数值可调

    // 判断数据规模
#ifdef _VECTOR_
    size_t size = std::distance(first, last);
#else
    size_t size = Iterable::distance(first, last);
#endif
    if (size <= threshold) {
        // 小规模数据使用插入排序
        insertionSort<Iterable, PredFunc>(first, last, pred);
    }
    else {
        // 大规模数据使用快速排序
        quickSort<Iterable, PredFunc>(first, last, pred);
    }
}
#else
//_CONSTEXPR20 void sort(const _RanIt _First, const _RanIt _Last, _Pr _Pred) { // order [_First, _Last)
#endif

template<typename TData>
struct RTreeNode {
    bool _isLeaf;
    MBR _boundBox;
    Vector<RTreeNodeEntry<TData>*> _entries;
    RTreeNode* _parent = NULL;
    RTreeNode(bool isLeaf) :_isLeaf(isLeaf),
        _boundBox(
            //-std::numeric_limits<DataType>::infinity(),
            //-std::numeric_limits<DataType>::infinity(),//对整数没有效果
            //std::numeric_limits<DataType>::infinity(),
            //std::numeric_limits<DataType>::infinity()
            INFINITY, INFINITY, -INFINITY, -INFINITY
        ) {}
    ~RTreeNode() {
        Vector< RTreeNode*> stack;
        for (auto entry : _entries) {
            if (entry) {
                if (entry->_data)delete entry->_data;
                if (entry->_child) stack.push_back(entry->_child);
                delete entry;
            }
        }
        while (!stack.empty()) {
            RTreeNode* top = stack.back();
            stack.pop_back();
            for (auto entry : top->_entries) {
                if (entry) {
                    if (entry->_data)delete entry->_data;
                    if (entry->_child) stack.push_back(entry->_child);
                    delete entry;
                }
            }
        }
    }
    void insert(RTreeNodeEntry<TData>* new_entry, size_t max_entries) {
        if (!_isLeaf && !new_entry->_child) {//如果新插入的entry需要插入在叶子里面，也就是说这是一个存储实际数据的entry，但是当前节点不是叶子
            //最小面积策略, 选择插入的子树
            RTreeNode* best_child = nullptr;
            //DataType min_area_increase = std::numeric_limits<DataType>::infinity();
            DataType min_area_increase = INFINITY;
            for (RTreeNodeEntry<TData>* entry : _entries) {
                if (entry->_child) {
                    // 计算插入新条目后，父节点的边界框扩展面积
                    DataType new_area = entry->_mbr.extendArea(new_entry->_mbr);
                    DataType area_increase = new_area - entry->_mbr.area();

                    // 如果面积增加最小，选择该子节点
                    if (area_increase < min_area_increase) {
                        min_area_increase = area_increase;
                        best_child = entry->_child;
                    }
                }
            }

            // 如果找到合适的子节点，则递归插入
            //if (best_child) {
            assert(best_child);
            best_child->insert(new_entry, max_entries);
            //}
        }
        else {//要插入的条目不需要插入到叶子（也就是说它是MBR边界索引）, 或者说当前节点就是叶子
            //那么条目必须插入到当前节点
            _entries.push_back(new_entry);
            if (_entries.size() > max_entries) {
                RTreeNode* new_node = split(max_entries);//发生分裂

                //更新父
                if (NULL == _parent) {//创建新的根
                    _parent = new RTreeNode(false);
                    _parent->insert(new RTreeNodeEntry<TData>(_boundBox, 0, this), max_entries);
                    _parent->_boundBox = _boundBox;
                }
                else {
                    for (RTreeNodeEntry<TData>* entry : _parent->_entries) {
                        if (entry->_child == this) {
                            entry->_mbr = _boundBox;
                            break;
                        }
                    }
                }
                new_node->_parent = _parent;
                _parent->insert(new RTreeNodeEntry<TData>(new_node->_boundBox, 0, new_node), max_entries);//尝试向父节点插入MBR边界索引，也只有这个地方能插入边界索引
            }
            else {
                RTreeNode* p = this;
                while (p) {
                    if (p->_boundBox + new_entry->_mbr == p->_boundBox) break;
                    p->_boundBox.extend(new_entry->_mbr);
                    if (p->_parent) {
                        for (auto entry : p->_parent->_entries) {
                            if (entry->_child == this) {
                                entry->_mbr = p->_boundBox;
                                break;
                            }
                        }
                    }
                    p = p->_parent;


                }
            }

        }
    }

    RTreeNode* split(size_t max_entries) {
        Vector<RTreeNodeEntry<TData>*> sorted_entries = _entries;
#ifndef _ALGORITHM_
        Sort<typename Vector<RTreeNodeEntry<TData>*>::iterator, EntryPredFunc<TData>, TData>(sorted_entries.begin(), sorted_entries.end(),
            [](RTreeNodeEntry<TData>* a, RTreeNodeEntry<TData>* b) {
                if (a->_mbr._min_x != b->_mbr._min_x) return  a->_mbr._min_x < b->_mbr._min_x;
                else return a->_mbr._min_y < b->_mbr._min_y;
            });
#else
        std::sort(sorted_entries.begin(), sorted_entries.end(), [](RTreeNodeEntry<TData>* a, RTreeNodeEntry<TData>* b) {
            if (a->_mbr._min_x != b->_mbr._min_x) return  a->_mbr._min_x < b->_mbr._min_x;
            else return a->_mbr._min_y < b->_mbr._min_y;
            });
#endif
        size_t mid = sorted_entries.size() / 2;
        RTreeNode* new_node = new RTreeNode(_isLeaf);
        assert(new_node);
        for (size_t i = mid; i < sorted_entries.size(); i++)
            new_node->_entries.push_back(sorted_entries[i]);
        for (auto entry : new_node->_entries)
            if (entry->_child)
                entry->_child->_parent = new_node;
        new_node->updateBoundBox();
        _entries.resize(mid);
        updateBoundBox();


        return new_node;
    }

    RTreeNode* split2(size_t max_entries) {
        // 先将条目按照X轴和Y轴进行排序
        Vector<RTreeNodeEntry<TData>*> sorted_entries = _entries;

        // X轴排序
#ifndef _ALGORITHM_
        Sort<Vector<RTreeNodeEntry<TData>*>::iterator, EntryPredFunc<TData>, TData>(sorted_entries.begin(), sorted_entries.end(),
            [](RTreeNodeEntry<TData>* a, RTreeNodeEntry<TData>* b) {
                if (a->_mbr._min_x != b->_mbr._min_x) return  a->_mbr._min_x < b->_mbr._min_x;
                else return a->_mbr._min_y < b->_mbr._min_y;
            });
#else
        std::sort(sorted_entries.begin(), sorted_entries.end(), [](RTreeNodeEntry<TData>* a, RTreeNodeEntry<TData>* b) {
            if (a->_mbr._min_x != b->_mbr._min_x) return  a->_mbr._min_x < b->_mbr._min_x;
            else return a->_mbr._min_y < b->_mbr._min_y;
            });
#endif
        // 计算不同分裂点的重叠面积和空白区域面积
        size_t best_split = 0;
        //DataType min_overlap = std::numeric_limits<DataType>::infinity();
        //DataType min_wasted_space = std::numeric_limits<DataType>::infinity();
        DataType min_overlap = INFINITY;
        DataType min_wasted_space = INFINITY;

        for (size_t split_point = 1; split_point < sorted_entries.size(); ++split_point) {
            // 左子节点的边界框
            MBR left_mbr = sorted_entries[0]->_mbr;
            for (size_t i = 1; i < split_point; ++i) {
                left_mbr.extend(sorted_entries[i]->_mbr);
            }

            // 右子节点的边界框
            MBR right_mbr = sorted_entries[split_point]->_mbr;
            for (size_t i = split_point + 1; i < sorted_entries.size(); ++i) {
                right_mbr.extend(sorted_entries[i]->_mbr);
            }

            // 计算重叠面积和空白区域面积
            DataType overlap_area = left_mbr.overlap(right_mbr);
            DataType wasted_space = (left_mbr.area() + right_mbr.area()) - overlap_area;

            // 选择最小重叠和最小空白区域的分裂点
            if (overlap_area < min_overlap || (overlap_area == min_overlap && wasted_space < min_wasted_space)) {
                min_overlap = overlap_area;
                min_wasted_space = wasted_space;
                best_split = split_point;
            }
        }
        // 根据最优分裂点生成两个子节点
        RTreeNode* new_node = new RTreeNode(_isLeaf);
        assert(new_node);

        // 按最优分裂点分配条目
        for (size_t i = best_split; i < sorted_entries.size(); ++i) {
            new_node->_entries.push_back(sorted_entries[i]);
        }

        // 更新新子节点的父子关系
        for (auto entry : new_node->_entries) {
            if (entry->_child) {
                entry->_child->_parent = new_node;
            }
        }

        // 更新两个子节点的边界框
        new_node->updateBoundBox();
        _entries.resize(best_split);
        updateBoundBox();

        return new_node;
    }

    void updateBoundBox()
    {
        //_boundBox._min_x = std::numeric_limits<DataType>::infinity(); 
       // _boundBox._min_y = std::numeric_limits<DataType>::infinity();
       // _boundBox._max_x = -std::numeric_limits<DataType>::infinity(); 
       // _boundBox._min_y = -std::numeric_limits<DataType>::infinity();
        _boundBox._min_x = _boundBox._min_y = RTREE_INFINITY;
        _boundBox._max_x = _boundBox._max_y = - RTREE_INFINITY;
        for (auto& entry : _entries) {
            // _boundBox._min_x = std::min(_boundBox._min_x,entry->_mbr._min_x);
           //  _boundBox._min_y = std::min(_boundBox._min_y,entry->_mbr._min_y);
             //_boundBox._max_x = std::max(_boundBox._max_x,entry->_mbr._max_x);
             //_boundBox._max_y = std::max(_boundBox._max_y,entry->_mbr._max_y);
            _boundBox.extend(entry->_mbr);
        }

        if (_parent)
            for (RTreeNodeEntry<TData>* entry : _parent->_entries) {
                if (entry->_child == this) {
                    entry->_mbr = _boundBox;
                    break;
                }
            }
    }

    void extendParentMBR()
    {
        if (!_parent) {

            for (auto entry : _entries) {
                assert(NULL != _parent);
                _parent->_entries.push_back(entry);
            }
            _parent->updateBoundBox();
            _parent->extendParentMBR();

        }
    }
};

template<typename TData>
using VisitFunc = void (*)(TData*, void* args);

template<typename TData>
class RTree {
private:
    RTreeNode<TData>* _root;
    size_t _max_entries;
    static void rangeQueryInternal(RTreeNode<TData>* root, const MBR& query, VisitFunc<TData> visit, void* args) {
        Vector<RTreeNode<TData>*> stack;
        if (root) stack.push_back(root);

        while (!stack.empty()) {
            RTreeNode<TData>* top = stack.back();
            stack.pop_back();
            if (!top->_boundBox.intersects(query))
                continue;

            if (top->_isLeaf) {
                for (RTreeNodeEntry<TData>*& entry : top->_entries) {
                    if (query.include(entry->_mbr))
                        visit(entry->_data, args);
                }
            }
            else {
                for (RTreeNodeEntry<TData>*& entry : top->_entries) {
                    if (query.intersects(entry->_mbr))
                        if (entry->_child)
                            stack.push_back(entry->_child);
                }
            }

        }
    }
public:
    RTree(size_t max_entries) :_max_entries(max_entries) { _root = new RTreeNode<TData>(true); }
    void insert(const MBR& mbr, TData data)
    {
        RTreeNodeEntry<TData>* entry = new RTreeNodeEntry<TData>(mbr, data, nullptr);
        _root->insert(entry, _max_entries);
        while (_root->_parent) _root = _root->_parent;
    }
    void rangeQuery(const MBR& query, VisitFunc<TData> visit, void* args)
    {
        rangeQueryInternal(_root, query, visit, args);
    }
};

/*****************************/
int main() {
    int x1, x2, y1, y2, temp;
    int n = GetNumOfStation();
    RTree<int64_t> stationTree(5);

    for (int no = 0; no < n; no++)
    {
        GetStationInfo(no, &x1, &y1, &temp);
        stationTree.insert(MBR(x1, y1, x1, y1), temp);
    }
    //stationTree.updateBoundBox();
    while (GetQuery(&x1, &y1, &x2, &y2))
    {
        int64_t count = 0;
        int64_t sum = 0;
        int64_t* args[2] = { &count,&sum };
        stationTree.rangeQuery(MBR(x1, y1, x2, y2), [](int64_t* current, void* args) {
            int64_t* pCount = ((int64_t**)args)[0];
            int64_t* pSum = ((int64_t**)args)[1];
            *pCount += 1;
            *pSum += *current;
            }, args);
        if (0 == n) Response(0);
        else Response(count ? sum / count : 0);
    }
    return 0;
}
