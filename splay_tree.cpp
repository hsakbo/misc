#include <iostream>
#include <random>

//splay tree, bottom-up splay with efficient add.

using namespace std;

template <typename T>
class splay_tree
{
  struct node
  {
    T data;
    node* parent;
    node* left;
    node* right;
  };

  node* root{NULL};
  node* t1{NULL};
  node* t2{NULL};

  node* grandp(node* x);  
  void rotate_right(node* x);
  void rotate_left(node* x);
  void splay(node* x);
  int (*comp)(T, T); //a == b is 0, a < b is +1, a > b is -1
  node* rec_find(T, node*);
  void inorder_print(node* n);
  int height(node* n);
  int size(node* n);
  node* successor(node*);
  node* predecessor(node*);
  void split(T);
  void join();
  
public:
 
  splay_tree(int (*func)(T, T)) : comp(func){};
  void add(T);
  void del(T);
  bool find(T);
  T in_acc(T (*func)(T, T), T l, T r);
  void inorder_print() {inorder_print(root);}
  int height() {return height(root);}
  int size() {return size(root); }

  void DEBUG();
};

template <class T>
void splay_tree<T>::rotate_right(node* x)
{
  node* y = x->parent;
  if (y == NULL)
    return;

  y->left = x->right;
  if (x->right)
    {
      y->left->parent = y;
    }

  x->right = y;
  x->parent = y->parent;
  y->parent = x;

  if (x->parent)
    {
      if (y == x->parent->left)
	x->parent->left = x;

      else
	x->parent->right = x;
    }
}

template <class T>
auto splay_tree<T>::grandp(node* x) -> node*
  {
    if (!x->parent)
      return NULL;
    
    if (x->parent->parent)
      return x->parent->parent;
    
    return NULL;
  }

template <class T>
void splay_tree<T>::rotate_left(node* x)
{
  node* y = x->parent;
  if (y == NULL)
    return;

  y->right = x->left;

  if (x->left)
    {
      y->right->parent = y;
    }
  
  x->left = y;
  x->parent = y->parent;
  y->parent = x;

  if (x->parent)
    {
      //assert(y == x->parent->left || y == x->parent->right);
      if (y == x->parent->left)
	x->parent->left = x;

      else
	x->parent->right = x;
    }  
}

template <typename T>
void splay_tree<T>::splay(node* x)
{
  if (!x)
    return;
  
  node* p_node = x->parent;
  if (!p_node)
    return;

  node* g_node = grandp(x);
  while (p_node)
    {
      if (x == p_node->left)
	{
	  if (!g_node)
	    rotate_right(x);

	  else if (p_node == g_node->left)
	    {
	      rotate_right(p_node);
	      rotate_right(x);
	    }

	  else
	    {
	      rotate_right(x);
	      rotate_left(x);
	    }
	}

      else
	{
	  if (!g_node)
	    rotate_left(x);

	  else if (p_node == g_node->right)
	    {
	      rotate_left(p_node);
	      rotate_left(x);
	    }

	  else
	    {
	      rotate_left(x);
	      rotate_right(x);
	    }
	}

      p_node = x->parent;
      g_node = grandp(x);
    }

  this->root = x;
}

//NO INITIAL NULL CHECK
template <typename T>
typename splay_tree<T>::node* splay_tree<T>::rec_find(T item, node* nd)
{
  int direction = comp(item, nd->data);
  if (!direction)
    return nd;

  else if (direction > 0)
    {
      if (!nd->left)
	return nd;
	  
      return rec_find(item, nd->left);
    }

  if (!nd->right)
    return nd;
  
  return rec_find(item, nd->right);
}

template <typename T>
bool splay_tree<T>::find(T val)
{
  if (!root)
    return false;
  
  node* nd = rec_find(val, root);
  splay(nd);

  //assert(nd == root);
  if (!comp(root->data, val))
    return true;
  
  return false;
}

template <typename T>
void splay_tree<T>::add(T val)
{
  if (!root)
    {
      root = new node{val, NULL, NULL, NULL};
      return;
    }

  node* closest = rec_find(val, root);
  int dir = comp(val, closest->data);

  if (!dir)
    {
      splay(closest);
      return;
    }

  else if (dir > 0)
    {
      closest->left = new node{val, closest, NULL, NULL};
      splay(closest->left);
      return;
    }

  closest->right = new node{val, closest, NULL, NULL};
  splay(closest->right);
  return;
}

template <typename T>
void splay_tree<T>::inorder_print(node* n)
{
  if (!n)
    return;

  inorder_print(n->left);
  cout << "DEBUG (inorder): " << n->data << endl;
  inorder_print(n->right);
}

template <typename T>
int splay_tree<T>::height(node* n)
{
  if (!n)
    return 0;

  int l = height(n->left);
  int r = height(n->right);

  if (l > r)
    return l+1;
  
  return r+1;
}

template <typename T>
int splay_tree<T>::size(node* n)
{
  if (!n)
    return 0;

  int l = size(n->left);
  int r = size(n->right);
  return l+r+1;
}

template <typename T>
typename splay_tree<T>::node* splay_tree<T>::successor(node* n)
{
  if (!n || !n->right)
    return nullptr;

  n = n->right;
  while (n->left)
    {
      n = n->left;
    }
  return n;
}

template <typename T>
auto splay_tree<T>::predecessor(node* n) -> node*
{
  if (!n || !n->left)
    return nullptr;

  n = n->left;
  while (n->right)
    {
      n = n->right;
    }
  return n;
}

template <typename T>
void splay_tree<T>::split(T item)
{
  if (!root)
    return;

  node* closest = rec_find(item, root);
  splay(closest);
  int bias = comp(item, root->data);

  if (bias > 0)
    {
      t1 = root->left;
      if (t1)
	t1->parent = NULL;
      
      root->left = NULL;
      t2 = root;
      return;
    }

  t2 = root->right;
  if (t2)
    t2->parent = NULL;

  root->right = NULL;
  t1 = root;
  return;
}

template <typename T>
void splay_tree<T>::join()
{
  if (!t1)
    {
      root = t2;
      return;
    }

  node* largest = t1;
  while (largest->right)
    {
      largest = largest->right;
    }

  splay(largest);
  root->right = t2;

  if (t2)
    {
      t2->parent = root;
    }
  
  return;
}

template <typename T>
void splay_tree<T>::del(T item)
{
  if (!root)
    return;

  node* ret = rec_find(item, root);
  int dir = comp(item, ret->data);
  splay(ret);
  
  if (!dir)
    {
      t1 = ret->left;
      t2 = ret->right;

      if (!t1 && !t2)
	{
	  root = NULL;
	  return;
	}
      
      else if (!t1)
	{
	  root = t2;
	  root->parent = NULL;
	  return;
	}

      else if (!t2)
	{
	  root = t1;
	  root->parent = NULL;
	  return;
	}

      t1->parent = NULL;
      t2->parent = NULL;
      join();
      return;
    }  
}

template <typename T>
void splay_tree<T>::DEBUG()
{
  cout << "Inside DEBUG method" << endl;
  cout << root->data << endl;
}






int main()
{
  auto comp = [](int a, int b) -> int
	      {
		if (a == b)
		  return 0;
		
		else if (a < b)
		  return 1;
		
		return -1;
	      };
  
  splay_tree<int> test(comp);

  test.add(5);
  test.add(10);
  test.add(15);
  test.add(7);
  test.add(12);
  test.add(20);
  
  test.del(15);

  test.add(30);
  test.add(11);
  test.find(7);  
  test.inorder_print();

  
  cout << test.size() << endl;
  cout << test.height() << endl;

  random_device seed;
  mt19937 gen(seed());
  uniform_int_distribution<int> dist(-1000000,1000000);

  for (int i = 0; i < 100000; i++)
    {
      int num = dist(gen);
      test.add(num);
      test.height();
      test.find(dist(gen));
      test.size();
      test.del(dist(gen));
      test.height();

      if (i % 1000 == 0)
	cout << i << endl;
    }

  test.inorder_print();
  
  return 0;
}



