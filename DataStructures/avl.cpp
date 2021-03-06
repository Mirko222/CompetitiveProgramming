#include <algorithm>

/** BBST (AVL) */
template <class T>
class AVL_Tree
{
	private:

	///number of nodes in the tree
	int treeSize;

	/** single node of the AVL */
	struct Node
	{
		T elem; ///element of the node
		int h; ///height of the node (distance from any leaf)
		int sz; ///size of the node (number of children + 1)
 		Node *l; ///left child
 		Node *r; ///right child
		Node(T e): elem(e), l(nullptr), r(nullptr), h(0), sz(1){}
	};

	Node*root; ///root of the BBST

	/** gives the height of a node (null nodes are -1)*/
	int height(Node*n)
	{
		return n == nullptr ? -1 : n->h;
	}

	/**gives the size of a node (null nodes are 0)*/
	int size(Node*n)
	{
		return n == nullptr ? 0 : n->sz;
	}

	/**performs left rotation on node x*/
	Node* leftRotate(Node*x)
	{
		Node*y = x->r;
		x->r = y->l;
		y->l = x;
		x->h = std::max(height(x->l), height(x->r)) + 1; //recompute heights
		y->h = std::max(height(y->l), height(y->r)) + 1;
		x->sz = size(x->l) + size(x->r) + 1; //recompute sizes
		y->sz = size(y->l) + size(y->r) + 1;
		return y;
	}

	/**performs right rotation on node x*/
	Node* rightRotate(Node*&x)
	{
		Node*y = x->l;
		x->l = y->r;
		y->r = x;
		x->h = std::max(height(x->l), height(x->r)) + 1; //recompute heights
		y->h = std::max(height(y->l), height(y->r)) + 1;
		x->sz = size(x->l) + size(x->r) + 1; //recompute sizes
		y->sz = size(y->l) + size(y->r) + 1;
		return y;
	}

	///insert element (recursively)
	void insert(T e, Node*& x)
	{
		if(x == nullptr)
		{
			x = new Node(e);
			return;
		}

		if(e < x->elem)
			insert(e, x->l);
		else
			insert(e, x->r);

		x->h = std::max(height(x->l), height(x->r)) + 1;
		x->sz = size(x->l) + size(x->r) + 1;
		rebalance(x);
	}

	///insert element at the given position (recursively)
	void insertAt(T e, int ind, Node*&x)
	{
		if(x == nullptr)
		{
			x = new Node(e);
			return;
		}

		if(size(x->l) + 1 < ind)
			insertAt(e, ind - size(x->l) - 1, x->r);
		else
			insertAt(e, ind, x->l);

		x->h = std::max(height(x->l), height(x->r)) + 1;
		x->sz = size(x->l) + size(x->r) + 1;
		rebalance(x);
	}

	///remove element (recursively)
	void remove(T e, Node*&x, Node*&start,int dir = 0)
	{
    if(x->elem == e)
		{
			if(x->l == nullptr && x->r == nullptr)
			{
				start->elem = x->elem;
				delete x;
				x = nullptr;
			}
			else
				switch(dir)
				{
					case 0: //can go both left or right
            if(x->l != nullptr)
							remove(x->l->elem, x->l, x, 1);
						else
							remove(x->r->elem, x->r, x, 2);
					break;

					case 1: //must go right (find max)
						if(x->r != nullptr)
							remove(x->r->elem, x->r, start, 1);
						else
						{
              start->elem = x->elem;
              remove(x->l->elem, x->l, x, 1);
						}
					break;

					case 2: //must go left (find min)
						if(x->l != nullptr)
							remove(x->l->elem, x->l, start, 2);
						else
						{
							start->elem = x->elem;
							remove(x->r->elem, x->r, x, 2);
						}
					break;
				}
		}
		else
			if(e < x->elem)
				remove(e, x->l, x->l, 0);
			else
				remove(e, x->r, x->r, 0);

		if(x != nullptr)
		{
			x->h = std::max(height(x->l), height(x->r)) + 1;
			x->sz = size(x->l) + size(x->r) + 1;
			rebalance(x);
		}
	}

	///remove element at the given position (recursively)
	void removeAt(int ind, Node*&x, Node*&start, int dir = 0)
	{
    if(size(x->l) + 1 == ind)
		{
			if(x->l == nullptr && x->r == nullptr)
			{
				start->elem = x->elem;
				delete x;
				x = nullptr;
			}
			else
				switch(dir)
				{
					case 0: //can go both left or right
            if(x->l != nullptr)
							removeAt(size(x->l->l) + 1, x->l, x, 1);
						else
							removeAt(size(x->r->l) + 1, x->r, x, 2);
					break;

					case 1: //must go right (find max)
						if(x->r != nullptr)
							removeAt(size(x->r->l) + 1, x->r, start, 1);
						else
						{
              start->elem = x->elem;
              removeAt(size(x->l->l) + 1, x->l, x, 1);
						}
					break;

					case 2: //must go left (find min)
						if(x->l != nullptr)
							removeAt(size(x->l->l) + 1, x->l, start, 2);
						else
						{
							start->elem = x->elem;
							removeAt(size(x->r->l) + 1, x->r, x, 2);
						}
					break;
				}
		}
		else
			if(ind < size(x->l) + 1)
				removeAt(ind, x->l, x->l, 0);
			else
				removeAt(ind - size(x->l) - 1, x->r, x->r, 0);

		if(x != nullptr)
		{
			x->h = std::max(height(x->l), height(x->r)) + 1;
			x->sz = size(x->l) + size(x->r) + 1;
			rebalance(x);
		}
	}

	/**performs rotations to rebalance the tree, if needed*/
	void rebalance(Node*&x)
	{
		if(std::abs(height(x->l) - height(x->r)) > 1) //unbalanced -> there are 4 cases to consider
		{
			if(height(x->r) > height(x->l) && height(x->r->l) > height(x->r->r))
				x->r = rightRotate(x->r);

			if(height(x->l) > height(x->r) && height(x->l->r) > height(x->l->l))
				x->l = leftRotate(x->l);

			if(height(x->r) > height(x->l) && height(x->r->r) > height(x->r->l))
				x = leftRotate(x);

			if(height(x->l) > height(x->r) && height(x->l->l) > height(x->l->r))
				x = rightRotate(x);
		}
	}

	///find the given element (recursively)
	T* find(T e, Node*&x)
	{
		if(x == nullptr) return nullptr;
		if(x->elem == e) return &(x->elem);
		if(e < x->elem)
			return find(e, x->l);
		else
			return find(e, x->r);
	}

	///find the element at the given position (recursively)
	T* findAt(int ind, Node*&x)
	{
    if(x == nullptr) return nullptr;
    if(size(x->l) + 1 == ind) return &(x->elem);
    if(size(x->l) + 1 < ind)
			return findAt(ind - size(x->l) - 1, x->r);
		else
			return findAt(ind, x->l);
	}

	///returns the index of the given element (1-based)
	int getIndexOf(T e, Node*&x)
	{
		if(x == nullptr) return 0;
    if(x->elem == e) return size(x->l) + 1;
    if(e < x->elem) return getIndexOf(e, x->l);
		return size(x->l) + 1 + getIndexOf(e, x->r);
	}

	public:

	AVL_Tree()
	{
		treeSize = 0;
		root = nullptr;
	}

	/**insert element e by its value (can't handle duplicates!!!). Warning: CAN'T BE MIXED WITH insertAt()*/
	void insert(T e)
	{
		if(!find(e))
		{
			insert(e, root);
			treeSize++;
		}
	}

	/**insert element e at the given index (1-based). Warning: CAN'T BE MIXED WITH insert()*/
	void insertAt(T e, int ind)
	{
		if(ind > 0 && ind <= treeSize+1)
		{
			insertAt(e, ind, root);
			treeSize++;
		}
	}

	/**remove the given element. Warning: CAN'T BE MIXED WITH removeAt()*/
	void remove(T e)
	{
		if(find(e))
		{
			remove(e, root, root);
			treeSize--;
		}
	}

	/**remove the element at the given position (1-based)*/
	void removeAt(int ind)
	{
		if(ind > 0 && ind <= treeSize)
		{
      removeAt(ind, root, root);
			treeSize--;
		}
	}

	/**find the element e in the tree (returns a pointer to that element: useful for structs)*/
	T* find(T e)
	{
		return find(e, root);
	}

	/**get the element at the given index (1-based)*/
	T* findAt(int ind)
	{
		if(ind > -1 && ind <= treeSize)
			return findAt(ind, root);
		return nullptr;
	}

	/**finds the index (1-based) of the given element (BST property needed: use insert() and remove())*/
	int getIndexOf(T e)
	{
		return getIndexOf(e, root);
	}

	/**returns the size of the AVL*/
	int getTreeSize()
	{
		return treeSize;
	}
};
