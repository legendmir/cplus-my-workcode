#define _CRT_SECURE_NO_WARNINGS 1

#include"Heap.h"


//ע�⣺�@����Ҫ�����ļ�ѹ����Ŀ��������
template<class T>
struct HuffmanTreeNode
{
	HuffmanTreeNode<T>* _left;
	HuffmanTreeNode<T>* _right;
	HuffmanTreeNode<T>* _parent;
	T _data;                        //�@���dataʵ������Ȩ��(weight)
	HuffmanTreeNode(const T& d)
		:_data(d)
		, _left(NULL)
		, _right(NULL)
		, _parent(NULL)
	{
	}
};
template<class T>
struct WeitLessThan
{
	bool operator()(HuffmanTreeNode<T>* _left, HuffmanTreeNode<T>* _right) {
		return _left->_data < _right->_data;
	}
};

template<class T>
class HuffmanTree
{
	typedef HuffmanTreeNode<T> Node;
private:
	Node* _root;
public:
	HuffmanTree()  //�޲ι���
		:_root(NULL)
	{
	}
	HuffmanTree(T* a, size_t n ,T invalid)         //������һ������   //���ļ�ѹ����Ŀ�������T���͵�invalid�Ƿ�ֵ
	{
		//����С��ѡ��������������С����
		//����
		Heap<Node*, WeitLessThan<T>> minHeap;
		for (size_t i = 0; i < n; i++)
		{
			if (a[i] != invalid) 
			{
				minHeap.Push(new Node(a[i]));
			}
		}
		//��huffman��
		while (minHeap.Size()>1)                  //ע���@��ѭ�������� Ϊʲô��>1
		{
			                                    //ѡ��Ȩֵ��С���������
			Node* left = minHeap.Top();
			minHeap.Pop();
			Node* right = minHeap.Top();
			minHeap.Pop();
			                                     //���츸���Ȼ����������
			Node* parent = new Node(left->_data + right->_data);
			parent->_left = left;
			left->_parent = parent;                //parentָ��
			parent->_right = right;
			right->_parent = parent;               //���Ӻ�֮���ٷŻ�ȥ�Ƚ� // ע�⣺�Ż�ȥ�Ƚ�ûɶ�ر�ģ�Ҫ֪���ǷŻص��������棬���ӺõĲ������ʲôӰ��
			minHeap.Push(parent);
		}
		_root = minHeap.Top();                       //ע�⣺Ϊʲô�@����min.Top();
	}

	Node* GetRoot()
	{
		return _root;
	}
};
