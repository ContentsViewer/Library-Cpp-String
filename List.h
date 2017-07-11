#ifndef LIST_H
#define LIST_H

#include <stdexcept>

template <typename TYPE> class List
{
private:
    TYPE *array = 0x00;
    int capacity = 0;
    int count = 0;

    int NewCapacity(int targetCount);

    void Unallocate(TYPE *array);

    int SortHelperPartition(int l, int r, int(*comparison)(TYPE, TYPE));

public:
    int Count() { return count; }
    int Capacity() { return capacity; }
    void SetCapacity(int capacity);

    List();
    List(int capacity);
    List(const List &list);
    List(List&& list);
    ~List();

    List& operator=(const List &rList);
    List& operator=(List&& rList);

    //TYPE* operator&() { return array; }

    //
    // 参照演算子[]を使用してアイテムにアクセスします.
    //
    TYPE& operator[] (int index)
    {
        if (index < 0 || index >= count)
        {
            throw std::out_of_range("[List.oprator[]] IndexOutOfRange");
        }
        return array[index];
    }

    TYPE* Array() { return array; }
    void Add(TYPE item);
    void Insert(int index, TYPE item);
    int IndexOf(TYPE item);
    int LastIndexOf(TYPE item);
    bool Contains(TYPE item);
    void CopyTo(int index, TYPE *array, int arrayIndex, int count);
    void RemoveAt(int index);
    bool Remove(TYPE item);
    void Clear();
    void CopyFrom(const List &from);
    void MoveFrom(TYPE *from, int size);
    void Sort(int(*comparison)(TYPE, TYPE));

    void Trim();

};

//デフォルトコンストラクタ
template <typename TYPE>
List<TYPE>::List()
{

}

//要素数指定コンストラクタ
template <typename TYPE>
List<TYPE>::List(int capacity)
{
    array = new TYPE[capacity];
    this->capacity = capacity;
}

//コピーコンストラクタ
template <typename TYPE>
List<TYPE>::List(const List &list)
{
    //printf("Call List CopyConstructor\n");
    CopyFrom(list);
}

//ムーブコンストラクタ
template<typename TYPE>
List<TYPE>::List(List&& list)
{
    Unallocate(array);

    //ポインタの入れ替え
    array = list.array;

    //元のlistのarrayはnull
    list.array = 0x00;

    capacity = list.capacity;
    count = list.count;
}

//デストラクタ
template <typename TYPE>
List<TYPE>::~List()
{
    Unallocate(array);
}

//代入演算子
template <typename TYPE>
List<TYPE>& List<TYPE>::operator=(const List &rList)
{
    CopyFrom(rList);
    return (*this);
}

template <typename TYPE>
List<TYPE>& List<TYPE>::operator=(List&& rList)
{
    Unallocate(array);

    //ポインタの入れ替え
    array = rList.array;

    //元のlistのarrayはnull
    rList.array = 0x00;

    capacity = rList.capacity;
    count = rList.count;

    return *this;
}

//
// コピー元のリストを自身にコピーします
//
// @param &from:
//  コピー元リスト
//
template <typename TYPE>
void List<TYPE>::CopyFrom(const List &from)
{
    int newCapacity = NewCapacity(from.count);

    if (newCapacity > capacity)
    //if (nextCapacity > 0)
    {
        capacity = newCapacity;
        Unallocate(array);
        array = new TYPE[capacity];
    }

    count = from.count;
    for (int i = 0; i < count; i++)
    {
        array[i] = from.array[i];
    }
}

template <typename TYPE>
void List<TYPE>::MoveFrom(TYPE *from, int size)
{
    Unallocate(array);

    count = size;
    capacity = size;
    array = from;
}


//
// Listの容量を設定します.
// 現在の要素数を下回って設定することはできません.
//
template <typename TYPE>
void List<TYPE>::SetCapacity(int capacity)
{
    if (capacity < count)
    {
        return;
    }

    TYPE *newArray = new TYPE[capacity];
    CopyTo(0, newArray, 0, count);
    Unallocate(array);
    array = newArray;

    this->capacity = capacity;
}

//
// Listの末尾に要素を追加します.
//
// @param item:
//  追加するアイテム
//
template <typename TYPE>
void List<TYPE>::Add(TYPE item)
{
    int newCapacity = NewCapacity(++count);

    if (newCapacity > capacity)
    //if (nextCapacity > 0)
    {
        capacity = newCapacity;
        TYPE *newArray = new TYPE[capacity];
        CopyTo(0, newArray, 0, count - 1);
        Unallocate(array);
        array = newArray;
    }

    array[count - 1] = item;
}

template <typename TYPE>
void List<TYPE>::Insert(int index, TYPE item)
{
    if (index < 0 || index >= count)
    {
        return;
    }

    int newCapacity = NewCapacity(++count);
    //if (nextCapacity > 0)
    if (newCapacity > capacity)
    {
        capacity = newCapacity;
        TYPE *newArray = new TYPE[capacity];
        CopyTo(0, newArray, 0, index);
        CopyTo(index + 1, newArray, index, count - index);
        newArray[index] = item;
        Unallocate(array);
        array = newArray;

        return;
    }
    for (int i = 0; i < count - index; i++)
    {
        array[count - 1 - i] = array[count - 2 - i];
    }
    array[index] = item;

}
template <typename TYPE>
void List<TYPE>::CopyTo(int index, TYPE *array, int arrayIndex, int count)
{
    for (int i = 0; i < count; i++)
    {
        array[arrayIndex + i] = this->array[index + i];
    }
}

template <typename TYPE>
void List<TYPE>::RemoveAt(int index)
{
    if (index < 0 || index >= count)
    {
        return;
    }

    for (int i = index + 1; i < count; i++)
    {
        array[i - 1] = array[i];
    }

    int newCapacity = NewCapacity(--count);
    //if (nextCapacity > 0)
    if(newCapacity > capacity)
    {
        capacity = newCapacity;

        TYPE *newArray = new TYPE[capacity];
        CopyTo(0, newArray, 0, count);
        Unallocate(array);
        array = newArray;
    }
}

template <typename TYPE>
void List<TYPE>::Clear()
{
    count = 0;
}

template <typename TYPE>
int  List<TYPE>::IndexOf(TYPE item)
{
    for (int i = 0; i < count; i++)
    {
        if (array[i] == item)
        {
            return i;
        }
    }

    return -1;
}


template <typename TYPE>
int  List<TYPE>::LastIndexOf(TYPE item)
{
    for (int i = count - 1; i >= 0; i--)
    {
        if (array[i] == item)
        {
            return i;
        }
    }

    return -1;
}

template <typename TYPE>
bool  List<TYPE>::Contains(TYPE item)
{
    return IndexOf(item) >= 0;
}

template <typename TYPE>
bool  List<TYPE>::Remove(TYPE item)
{
    int rmvIndex = IndexOf(item);
    if (rmvIndex < 0)
    {
        return false;
    }
    RemoveAt(rmvIndex);

    return true;
}

template<typename TYPE>
void List<TYPE>::Trim()
{
    SetCapacity(count);
}

template <typename TYPE>
int List<TYPE>::SortHelperPartition(int l, int r, int(*comparison)(TYPE, TYPE))
{
    int i, j;
    TYPE pivot;
    TYPE temp;

    i = l - 1;
    j = r;

    //一番右側を枢軸にする
    pivot = array[r];

    for (;;)
    {
        //ポインタiを右に進める
        while (comparison(array[++i], pivot) < 0);

        //ポインタjを左に進める
        while (i < --j && comparison(pivot, array[j]) < 0);

        //ポインタi, jがぶつかるときはループを抜ける
        if (i >= j)
        {
            break;
        }

        //入れ替える
        temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }

    //array[i]と枢軸を入れかえる
    temp = array[i];
    array[i] = array[r];
    array[r] = temp;
    return i;
}

//
//Comparisonについて:
// 各アイテムを比較するための関数を用意する必要がある.
//
// @return 0より小さい:
//  並び替え順序においてAはBの前
//
// @return 0:
//  並び替え順序においてAとBは同じ位置
//
// @return 0より大きい:
//  並び替え順序においてAはBの後ろ
//
template <typename TYPE>
void List<TYPE>::Sort(int(*comparison)(TYPE, TYPE))
{
    int l, r, v;
    int low[30], high[30];
    int sp;

    //スタックを初期化する
    low[0] = 0;
    high[0] = count - 1;
    sp = 1;

    //スタックが空になるまで繰り返す
    while (sp > 0)
    {
        //スタックから整列する範囲を取り出す
        sp--;
        l = low[sp];
        r = high[sp];

        //整列するよう要素が一つなら何もしない
        if (l >= r)
        {

        }
        else
        {
            //枢軸vを基準に分割する
            v = SortHelperPartition(l, r, comparison);

            //左右の部分配列のうち短いほうを先に処理をする
            if (v - l < r - v)
            {
                //左部分配列を先に整列する
                //スタックなので右左の順に積む
                low[sp] = v + 1;
                high[sp++] = r;
                low[sp] = l;
                high[sp++] = v - 1;
            }
            else
            {
                //右部分配列を先に整列する
                //スタックなので左右の順に積む
                low[sp] = l;
                high[sp++] = v - 1;
                low[sp] = v + 1;
                high[sp++] = r;
            }
        }
    }
}

//
// 指定された要素数に対する設定すべき容量値を返します.
// 指定された要素数が現在の容量値を上回っていない場合は, 現在の容量値を返します.
// 
//  
// @param nextCount:
//  設定したいリストの要素数
// 
// @return:
//  確保されるべき容量値
//  この値が現在の容量値を上回っている(より大きい)場合は新しくメモリを確保すべきです.
//
template <typename TYPE>
int List<TYPE>::NewCapacity(int newCount)
{
    if(newCount <= capacity){
        return capacity;
    }
    int pow = 0;

    while (newCount > 0)
    {
        newCount >>= 1;
        pow++;
    }

    int newCapacity = (0x01) << pow;

    // Capacityは最低4にする.
    if (newCapacity < 4)
    {
        newCapacity = 4;
    }

    return newCapacity;
}

template <typename TYPE>
void List<TYPE>::Unallocate(TYPE *array)
{
    if (array != 0x00)
    {
        //printf("deleted: %p\n", array);

        //memo:
        //  配列開放のときは
        //      delete[] インスタンス名
        //  と書かなければならない
        delete[] array;

        array = 0x00;
    }
}
#endif