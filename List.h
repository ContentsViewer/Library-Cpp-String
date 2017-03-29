#ifndef _INCLUDE_LIST_H_
#define _INCLUDE_LIST_H_

#include <stdexcept>

template <typename TYPE> class List
{
private:
    TYPE *array = 0x00;
    int capacity = 0;
    int count = 0;

    static int Log2(int x);
    static int Pow2(int x);

    int GetStep(int x);
    int NextCapacity(int targetCount);

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
    TYPE& operator[] (int index);

    TYPE* Array() { return array; }
    void Add(TYPE item);
    void Insert(int index, TYPE  item);
    int IndexOf(TYPE item);
    int LastIndexOf(TYPE item);
    void CopyTo(int index, TYPE *array, int arrayIndex, int count);
    void RemoveAt(int index);
    bool Remove(TYPE item);
    void Clear();
    void CopyFrom(const List &from);
    void MoveFrom(TYPE *from, int size);
    void Sort(int(*comparison)(TYPE, TYPE));

    void Trim();

};

//�f�t�H���g�R���X�g���N�^
template <typename TYPE>
List<TYPE>::List()
{

}

//�v�f���w��R���X�g���N�^
template <typename TYPE>
List<TYPE>::List(int capacity)
{
    array = new TYPE[capacity];
    this->capacity = capacity;
}

//�R�s�[�R���X�g���N�^
template <typename TYPE>
List<TYPE>::List(const List &list)
{
    //printf("Call List CopyConstructor\n");
    CopyFrom(list);
}

//���[�u�R���X�g���N�^
template<typename TYPE>
List<TYPE>::List(List&& list)
{
    Unallocate(array);

    //�|�C���^�̓���ւ�
    array = list.array;

    //����list��array��null
    list.array = 0x00;

    capacity = list.capacity;
    count = list.count;
}

//�f�X�g���N�^
template <typename TYPE>
List<TYPE>::~List()
{
    Unallocate(array);
}

//������Z�q
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

    //�|�C���^�̓���ւ�
    array = rList.array;

    //����list��array��null
    rList.array = 0x00;

    capacity = rList.capacity;
    count = rList.count;

    return *this;
}

template <typename TYPE>
TYPE& List<TYPE>::operator[] (int index)
{
    if (index < 0 || index >= count)
    {
        throw std::out_of_range("[List.oprator[]]>> IndexOutOfRange");
    }
    return array[index];
}

template <typename TYPE>
void List<TYPE>::CopyFrom(const List &from)
{
    int nextCapacity = NextCapacity(from.capacity);
    if (nextCapacity > 0)
    {
        capacity = nextCapacity;
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

template <typename TYPE>
void List<TYPE>::Add(TYPE item)
{
    int nextCapacity = NextCapacity(++count);
    if (nextCapacity > 0)
    {
        capacity = nextCapacity;
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

    int nextCapacity = NextCapacity(++count);
    if (nextCapacity > 0)
    {
        capacity = nextCapacity;
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

    int nextCapacity = NextCapacity(--count);
    if (nextCapacity > 0)
    {
        capacity = nextCapacity;

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

    //��ԉE���𐕎��ɂ���
    pivot = array[r];

    for (;;)
    {
        //�|�C���^i���E�ɐi�߂�
        while (comparison(array[++i], pivot) < 0);

        //�|�C���^j�����ɐi�߂�
        while (i < --j && comparison(pivot, array[j]) < 0);

        //�|�C���^i, j���Ԃ���Ƃ��̓��[�v�𔲂���
        if (i >= j)
        {
            break;
        }

        //����ւ���
        temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }

    //array[i]�Ɛ�������ꂩ����
    temp = array[i];
    array[i] = array[r];
    array[r] = temp;
    return i;
}

//Comparison�ɂ���:
//  �Ԃ�l:
//      0��菬����:
//          ���ёւ������ɂ�����A��B�̑O
//
//      0:
//          ���ёւ������ɂ�����A��B�͓����ʒu
//
//      0���傫��:
//          ���ёւ������ɂ�����A��B�̌��
template <typename TYPE>
void List<TYPE>::Sort(int(*comparison)(TYPE, TYPE))
{
    int l, r, v;
    int low[30], high[30];
    int sp;

    //�X�^�b�N������������
    low[0] = 0;
    high[0] = count - 1;
    sp = 1;

    //�X�^�b�N����ɂȂ�܂ŌJ��Ԃ�
    while (sp > 0)
    {
        //�X�^�b�N���琮�񂷂�͈͂����o��
        sp--;
        l = low[sp];
        r = high[sp];

        //���񂷂�悤�v�f����Ȃ牽�����Ȃ�
        if (l >= r)
        {

        }
        else
        {
            //����v����ɕ�������
            v = SortHelperPartition(l, r, comparison);

            //���E�̕����z��̂����Z���ق����ɏ���������
            if (v - l < r - v)
            {
                //�������z����ɐ��񂷂�
                //�X�^�b�N�Ȃ̂ŉE���̏��ɐς�
                low[sp] = v + 1;
                high[sp++] = r;
                low[sp] = l;
                high[sp++] = v - 1;
            }
            else
            {
                //�E�����z����ɐ��񂷂�
                //�X�^�b�N�Ȃ̂ō��E�̏��ɐς�
                low[sp] = l;
                high[sp++] = v - 1;
                low[sp] = v + 1;
                high[sp++] = r;
            }
        }
    }
}

//y = log2(x)
template <typename TYPE>
int List<TYPE>::Log2(int x)
{
    int y = 0;
    while (x > 1)
    {
        x >>= 1;
        y++;
    }
    return y;
}

//y = 2^x
template <typename TYPE>
int List<TYPE>::Pow2(int x)
{
    int y = 0;
    if (x == 0)
    {
        y = 1;
    }
    else
    {
        y = 2 << (x - 1);
    }

    return y;
}

//�֐�:
//  ����:
//      x�̒l���烁�����̊m�ۃX�e�b�v�l��Ԃ��܂�
template <typename TYPE>
int List<TYPE>::GetStep(int x)
{
    if (x == 0)
    {
        return 0;
    }
    else if (x == 1)
    {
        return 1;
    }

    return Log2(x);
}

//�֐�
//  ����:
//      �w�肳�ꂽ�v�f���Ɋ�Â��ă��������Ċm�ۂ��邩���肵�܂�.
//  
//  �Ԃ�l:
//      �m�ۂ����ׂ��v�f��
//
template <typename TYPE>
int List<TYPE>::NextCapacity(int targetCount)
{
    int targetStep = GetStep(targetCount);
    int nowStep = GetStep(capacity);

    if (targetCount <= capacity)
    {
        return -1;
    }

    int nextCapacity = 0;
    if (targetStep == 0)
    {
        return 0;

    }
    else
    {
        nextCapacity = Pow2(targetStep + 1);
    }

    return nextCapacity;
}

template <typename TYPE>
void List<TYPE>::Unallocate(TYPE *array)
{
    if (array != 0x00)
    {
        //printf("deleted: %p\n", array);

        //memo:
        //  �z��J���̂Ƃ���
        //      delete[] �C���X�^���X��
        //  �Ə����Ȃ���΂Ȃ�Ȃ�
        delete[] array;

        array = 0x00;
    }
}
#endif