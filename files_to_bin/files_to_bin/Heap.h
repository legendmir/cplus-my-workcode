#define _CRT_SECURE_NO_WARNINGS 1

#include<iostream>
#include<string>
#include<vector>
#include<assert.h>
#include<algorithm>
using namespace std;

template<class T>
struct LessThan                                  //ʹ�÷º���  
{
	T left;
	T right;
	bool operator()(const T& left, const T& right)
	{
		return left < right;
	}
};

template<class T>
struct MoreThan
{
	T left;
	T right;                                        //�@��ĳ�Ա������дҲûʲô  ������Ҳ����ֻ�г�Ա�������һ���Ĳ�������һ��Ҫ�г�Ա����
	bool operator()(const T& left, const T& right)
	{
		return left > right;
	}
};


template<class T, class Compare = LessThan<T>>   //Ĭ��С��
class Heap
{
private:
	vector<T> _a;                 //�ѵı���ʵ�����Ƕ�vector�Ĳ���
public:
	Heap()     //�޲ι��캯�� ʵ��������vector�Լ�����ȱʡ�Ĺ��캯��
	{
	}
	Heap(T* a, size_t n)    //���캯�����������ֵһһ��vector
	{
		_a.reserve(n);
		for (size_t i = 0; i < n; i++)
		{
			_a.push_back(a[i]);
		}
		//    ����    
		for (int i = (_a.size() - 2) / 2; i >= 0; i--)     
		{
			AdjustDown(i);   
		}
	}

	void AdjustDown(int  root)          //[*]  
	{
		Compare con;                  //ע��Ҫʵ����   �տ�ʼ������ 
		int parent = root;
		int child = parent * 2 + 1;     
		while (child<_a.size())                  
		{
			
			if (child != _a.size() - 1 && con(_a[child + 1], _a[child]))
			{
				child++;
			}
			if (con(_a[child], _a[parent]))
			{
				swap(_a[parent], _a[child]);
				parent = child;
				child = child * 2 + 1;
			}
			else
			{
				break;
			}
		}
	}
	void AdjustUp(int root)   //���ϵ����������β������һ�����ݣ�����Ϊ��� (�����β������)
	{
		Compare con;
		int child = root;
		int parent = (child - 1) / 2;                //������������������������������-1��/2���ǵ��ڸ���� ��һ��ʼ��Ϊ��������-1��/2��ֵ��������/2��һ���� ���Ǹ���㣬��Ϊ����������Ҳ��һ�������Ƿ��������������/2���𰸲��Ǹ����
		while (parent >= 0)
		{
			if (con(_a[child],_a[parent]))
			{
				swap(_a[child], _a[parent]);          //�@���swap�����������
				child = parent;
				parent = (parent - 1) / 2;
			}
			else
				break;
		}
	}

	void Push(const T& d)
	{
		_a.push_back(d);
		AdjustUp(_a.size() - 1);              //�����һ�����������ϵ���

	}

	void Pop()         //ɾ����Ͻ��
	{
		swap(_a[_a.size() - 1], _a[0]);
		_a.pop_back();
		AdjustDown(0);
	}

	T& Top()
	{
		return _a[0];
	}
	size_t Size()
	{
		return _a.size();
	}
};
