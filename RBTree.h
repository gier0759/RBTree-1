#include<iostream>
#include<queue>

using namespace std;

enum Colour
{
	RED,
	BLACK,
};

template<class K,class V>
struct RBTreeNode
{
	K _key;
	V _value;
	Colour _col;

	RBTreeNode<K, V>* _left;
	RBTreeNode<K, V>* _right;
	RBTreeNode<K, V>* _parent;

	RBTreeNode(const K& key, const V& value, Colour col = RED)
		:_key(key)
		, _value(value)
		, _left(NULL)
		, _right(NULL)
		, _parent(NULL)
		, _col(col)
	{}
};

template<class K,class V>
class RBTree
{
	typedef RBTreeNode<K, V> Node;
public:
	RBTree()
		:_root(NULL)
	{}
	bool Insert(const K& key, const V& value)
	{
		if (NULL == _root)
		{
			_root = new Node(key, value, BLACK);
			return true;
		}
		//找位置插入节点
		Node* cur = _root;
		Node* parent = NULL;
		while (cur)
		{
			if (key > cur->_key)
			{
				parent = cur;
				cur = cur->_right;
			}
			else if (key < cur->_key)
			{
				parent = cur;
				cur = cur->_left;
			}
			else
			{
				return false;
			}
		}
		cur = new Node(key, value);
		if (key > parent->_key)
		{
			parent->_right = cur;
			cur->_parent = parent;
		}
		else
		{
			parent->_left = cur;
			cur->_parent = parent;
		}

		//调整(如果父为红)
		while (cur != _root && parent->_col == RED)
		{
			Node* grandpa = parent->_parent;
			if (parent == grandpa->_left)
			{
				Node* uncle = grandpa->_right;
				//情况一(u存在且为红)
				if (uncle && uncle->_col == RED)
				{
					//变色
					parent->_col = uncle->_col = BLACK;
					grandpa->_col = RED;

					cur = grandpa;
					parent = cur->_parent;
				}
				//情况二(u不存在或者存在且为黑)
				else
				{
					if (cur == parent->_right)
					{
						_RotateL(parent);
						_Link(parent);
					}

					//变色
					parent->_col = BLACK;
					grandpa->_col = RED;

					//右单旋(以g为轴)
					_RotateR(grandpa);
					_Link(grandpa);
				}
			}
			else
			{
				Node* uncle = grandpa->_left;
				//情况一(u存在且为红)
				if (uncle && uncle->_col == RED)
				{
					//变色
					parent->_col = uncle->_col = BLACK;
					grandpa->_col = RED;

					cur = grandpa;
					parent = cur->_parent;
				}
				//情况二(u不存在或者存在且为黑)
				else
				{
					if (cur == parent->_left)
					{
						_RotateR(parent);
						_Link(parent);
					}

					//变色
					parent->_col = BLACK;
					grandpa->_col = RED;

					//左单旋(以g为轴)
					_RotateL(grandpa);
					_Link(grandpa);
				}
			}
		}

		_root->_col = BLACK;
		return true;
	}

	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}

	bool IsBlance()
	{
		if (NULL == _root)
		{
			return true;
		}

		if (_root->_col == RED)
		{
			return false;
		}
		return _IsBlance(_root);
	}

protected:
	void _InOrder(Node* root)
	{
		if (NULL == root)
		{
			return;
		}
		_InOrder(root->_left);
		cout << root->_key << " ";
		_InOrder(root->_right);
	}

	/*int _Height(const Node* root)
	{
		if (NULL == root)
		{
			return 0;
		}
		int left = _Height(root->_left) + 1;
		int right = _Height(root->_right) + 1;
		return left > right ? left : right;
	}
	int _ShortPath(const Node* root)
	{
		if (NULL == root)
		{
			return 0;
		}
		
		queue<Node> q;
		int path = 0;
		q.push(root);
		Node* cur;
		while (!q.empty())
		{
			cur = q.front();
			q.pop();
			if (cur->_left == NULL && cur->_right == NULL)
			{
				break;
			}
			if (cur->_left)
			{
				q.push(cur->_left);
			}
			if (cur->_right)
			{
				q.push(cur->_right);
			}
		}
		while (cur)
		{
			path++;
			cur = cur->_parent;
		}
	}*/

	void _FindLeaf(Node* root,queue<Node*>& q)
	{
		if (NULL == root)
		{
			return;
		}
		_FindLeaf(root->_left, q);
		if (root->_left == NULL && root->_right == NULL)
		{
			q.push(root);
		}
		_FindLeaf(root->_right, q);
	}
	bool _IsBlance(Node* root)
	{
		if (NULL == root)
		{
			return true;
		}

		queue<Node*> q;
		_FindLeaf(root, q);
		int numOfBlack = 0;
		Node* cur = q.front();
		q.pop();
		while (cur)
		{
			if (cur->_col == BLACK)
			{
				numOfBlack++;
			}
			if (cur->_parent && cur->_col==RED && cur->_parent->_col==RED)
			{
				return false;
			}
			if (cur == root)
			{
				break;
			}
			cur = cur->_parent;
		}

		while (!q.empty())
		{
			cur = q.front();
			q.pop();

			int num = 0;
			while (cur)   //回溯判断黑节点数
			{
				if (cur->_col == BLACK)
				{
					num++;
				}
				if (cur->_parent && cur->_col == RED && cur->_parent->_col == RED)
				{
					return false;
				}
				if (cur == root)
				{
					break;
				}
				cur = cur->_parent;
			}
			if (num != numOfBlack)
			{
				cout << "有问题：" << cur->_key << endl;
				return false;
			}
		}
		return _IsBlance(root->_left)&&_IsBlance(root->_right);
	}

	void _RotateL(Node*& parent)   ///    '\'   =>   '/\'
	{
		Node* subRight = parent->_right;
		Node* subRightL = subRight->_left;
		parent->_right = subRightL;
		subRight->_left = parent;

		if (subRightL != NULL)
		{
			subRightL->_parent = parent;
		}

		subRight->_parent = parent->_parent;
		parent->_parent = subRight;

		parent = subRight;
	}

	void _RotateR(Node*& parent)    //  '/'  =>  '/\'
	{
		Node* subLeft = parent->_left;
		Node* subLeftR = subLeft->_right;
		parent->_left = subLeftR;
		if (subLeftR)
		{
			subLeftR->_parent = parent;
		}

		subLeft->_right = parent;
		subLeft->_parent = parent->_parent;
		parent->_parent = subLeft;

		parent = subLeft;
	}

	void _Link(Node* parent)
	{
		Node* grandpa = parent->_parent;
		if (grandpa == NULL)
		{
			_root = parent;
		}
		else
		{
			if (grandpa->_key < parent->_key)
			{
				grandpa->_right = parent;
			}
			else
			{
				grandpa->_left = parent;
			}
		}
	}
protected:
	Node* _root;
};