#include <iostream>
#include <random>

using namespace std;

template <typename T>
class fibqueue
{
  struct node
  {
    T data;
    node* left;
    node* right;
    node* up;
    node* down;

    bool mark;
    size_t degree;
  };
  
  node* rlist{NULL}; //min node
  size_t rl_size{0};
  size_t bound{48}; //assumption n < 32 bit unsigned max
  void disconnect(node*);
  void promote(node*);
  void free_all(node* nd);
  void heap_link(node* src, node* parent);
  void consolidate();
  void cut(node* src);
  void cascade_cut(node* src);
  
public:
  fibqueue() {}
  void set_bound(unsigned int b) {bound = b;}
  void* push(T );
  T pop();
  void decrease(void* , T );
  void DEBUG();
  ~fibqueue() { free_all(rlist); }
};

template <typename T>
void fibqueue<T>::decrease(void* nd, T item)
{ 
  node* target = static_cast<node*>(nd);

  if (target->data <= item)
    return;
  
  target->data = item;

  if (target->up)
    {
      cut(target);
      cascade_cut(target->up);
    }

  if (item < rlist->data)
    rlist = target;
}

template <typename T>
void fibqueue<T>::cascade_cut(node* y)
{
  node* parent = y->up;
  if (parent)
    {
      if (!y->mark)
	y->mark = true;

      else
	{
	  cut(y);
	  cascade_cut(parent);
	}
    }
}

//NO NULL CHECK!
template <typename T>
void fibqueue<T>::cut(node* src)
{
  rl_size++;
  disconnect(src);
  src->left = rlist;
  src->right = rlist->right;
  src->up = NULL;
  src->left->right = src;
  src->right->left = src;
  src->mark = false;
}

template <typename T>
void* fibqueue<T>::push(T data)
{
  rl_size++;
  node* nd = new node();
  nd->data = data;
  nd->up = NULL;
  nd->down = NULL;
  nd->mark = false;
  nd->degree = 0;
  
  if (!rlist)
    {     
      nd->left = nd;
      nd->right = nd;
      rlist = nd;
      return nd;
    }

  nd->left = rlist;
  nd->right = rlist->right;

  rlist->right = nd;
  nd->right->left = nd;

  if (rlist->data > nd->data)
    rlist = nd;
  
  return nd;
}

template <typename T>
void fibqueue<T>::disconnect(node* nd)
{
  node* left = nd->left;
  node* right = nd->right;

  if (left == nd)
    {
      if (!nd->up)
	{
	  //rl_size--;
	  rlist = NULL;
	  return;
	}
      nd->up->degree--;
      nd->up->down = NULL;
      return;
    }

  left->right = right;
  right->left = left;

  if (nd->up)
    {
      nd->up->degree--;
      nd->up->down = left;
      return;
    }
  //rl_size--;
}

//NO NULL CHECK!
template <typename T>
void fibqueue<T>::heap_link(node* src, node* parent)
{
  rl_size--;
  disconnect(src);
  parent->degree++;
  src->up = parent;
  src->mark = false;
  if (!parent->down)
    {
      src->left = src;
      src->right = src;
      parent->down = src;
      return;
    }

  src->left = parent->down;
  src->right = parent->down->right;

  parent->down->right = src;
  src->right->left = src; 
}

//NO NULL CHECK!
template <typename T>
void fibqueue<T>::consolidate()
{
  node* arr[bound] = {NULL};
  node* x = rlist;
  size_t d;
  node* smallest = rlist;
  node* roots[rl_size];
  size_t size = rl_size;
  for (int i = 0; i < size; i++)
    {
      roots[i] = x;
      x = x->right;
    }
  
  for (int i = 0; i < size; i++)
    {
      x = roots[i];
      d = x->degree;
            
      while (arr[d] && arr[d] != x)
	{
	  if (x->data > arr[d]->data)
	    {
	      node* temp = x;
	      x = arr[d];
	      arr[d] = temp;
	    }
	  
	  heap_link(arr[d], x);
	  arr[d] = NULL;
	  d++;
	}
      arr[d] = x;
      
      if (smallest->data >= x->data)
	smallest = x;
    }

  rlist = smallest;
}

template <typename T>
void fibqueue<T>::promote(node* parent)
{
  while (parent->down)
    {
      node* child = parent->down;
      disconnect(child);
      child->up = NULL;
      child->left = rlist;
      child->right = rlist->right;
      rlist->right = child;
      child->right->left = child;
      rl_size++;
    }
}

template <typename T>
T fibqueue<T>::pop()
{
  promote(rlist);
  node* z = rlist;
  rl_size--;
  if (z->right == z)
    {
      rlist = NULL;  
      return z->data;
    }
  
  disconnect(rlist);
  rlist = z->right;
  consolidate();
  return z->data;  
}

template <typename T>
void fibqueue<T>::free_all(node* nd)
{
  if (nd == NULL)
    return;

  node* current = nd;
  node* temp;
  do
    {
      free_all(current->down);
      temp = current;
      current->right;
      delete temp;
    }while (current != nd);
}

int main()
{
  fibqueue<int> Q;
  // random_device seed;
  // mt19937 gen(seed());
  // uniform_int_distribution<int> dist(-10000, 100000);
  // constexpr int r_size = 100000;
  // uniform_int_distribution<int> index(0, r_size-1);
  // int prev  = -100000;
  // vector<void*> dict(r_size);
  
  //  while (true)
  //   {      
  //     for (int i = 0; i < r_size; i++)
  //  	{
  //  	  int temp = dist(gen);
  //  	  //cout << "DEBUG: input-> " << temp << endl;
  //  	  dict[i] = Q.push(temp);
  //  	}

  //     for (int i = 0; i < r_size; i++)
  //  	{
  //  	  int temp = dist(gen);
  //  	  int ind = index(gen);
  //  	  Q.decrease(dict[ind], temp);
  //  	}

  //     prev = -100000;
  //     for (int i = 0; i < r_size; i++)
  //  	{
  //  	  //Q.DEBUG();
  //  	  int current = Q.pop();
  //  	  if (current < prev)
  //  	    {
  //  	      cout << "BUG found" << endl;
  //  	      return 1;
  //  	    }
  //  	  prev = current;
  //  	}
  //   }

  void* p1 = Q.push(10);
  void* p2 = Q.push(-4);
  void* p3 = Q.push(1000);
  void* p4 = Q.push(30);
  void* p5 = Q.push(8);
  void* p6 = Q.push(9);
  void* p7 = Q.push(100);
  void* p8 = Q.push(-30);

  Q.decrease(p3, 50);
  Q.decrease(p8, -300);

  for (int i = 0; i < 8; i++)
    {
      cout << Q.pop() << endl;
    }
  
  return 0;
}

template <typename T>
void fibqueue<T>::DEBUG()
{
  cout << "Entered DEBUG: "  << rl_size << endl;
  node* r = rlist;
  do
    {
      cout << r->data << ", degree: " << r->degree << endl;
      r = r->right;

    } while (r != rlist);
  cout << "Exiting DEBUG: " << endl;
}
