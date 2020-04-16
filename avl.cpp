#include <iostream>
#include <random>
#include <cstdlib>
#include <set>

using namespace std;

unsigned int _MAX = ~((unsigned int)0) >> 1;

typedef struct avl_tree_node
{
  int data;
  int height{0};
  avl_tree_node *left{NULL};
  avl_tree_node *right{NULL};
  
}node;

class avl
{
private:
  node *root;

  int get_height(node *head);
  node *left_rotate(node *head);
  node *right_rotate(node *head);
  node* avl_insert(node *head, int item);
  int height(node *head);
  void set_height(node *head);
  void init_left(node *head, int item);
  void init_right(node *head, int item);
  void free_nodes(node *head);
  node* correction(node *head);
  void print_inorder(node *head);
  int size(node *head);
  
public:
  avl(int data)
  {
    root = (node*) malloc(sizeof(node));
    root->data = data;
    root->height = 0;
    root->left = NULL;
    root->right = NULL;
  }
  
  int height()
  {
    return get_height(root);
  }

  void insert(int item)
  {
    root = avl_insert(root, item);
  }

  void print_inorder()
  {
    print_inorder(root);
  }

  int size()
  {
    return size(root);
  }

  ~avl()
  {
    free_nodes(root);
  }
};

void avl::free_nodes(node *head)
{
  if (head == NULL)
    return;

  free_nodes(head->left);
  free_nodes(head->right);
  free(head);
  return;
}

int avl::size(node *head)
{
  if (head == NULL)
    return 0;
  return size(head->left) + size(head->right) + 1;
}

int avl::get_height(node *head)
  {
    if (head == NULL)
      return -1;

    int l_height = get_height(head->left);
    int r_height = get_height(head->right);
    
    if (l_height > r_height)
      return l_height+1;

    return r_height+1;
  }

void avl::init_left(node *head, int item)
{
  head->left = (node*) malloc(sizeof(node));
  head->left->data = item;
  head->left->left = NULL;
  head->left->right = NULL;
  head->left->height = 0;  
}

void avl::init_right(node *head, int item)
{
  head->right = (node*) malloc(sizeof(node));
  head->right->data = item;
  head->right->left = NULL;
  head->right->right = NULL;
  head->right->height = 0;  
}

int avl::height(node *head)
{
  if (head == NULL)
    return -1;
  return head->height;
}

void avl::set_height(node *head)
{
  int l = abs(height(head->left));
  int r = abs(height(head->right));

  if (l > r)
    head->height = -l-1;

  else
    head->height = r+1;

  //cout << head->height << endl;
}

node* avl::left_rotate(node *head)
{
  node *y = head->right;
  head->right = head->right->left; //head's right has y's left
  y->left = head; //y's left is head
  set_height(head);
  set_height(y);
  return y;
}

node* avl::right_rotate(node *head)
{
  node *y = head->left;
  head->left = head->left->right;
  y->right = head;
  set_height(head);
  set_height(y);
  return y;
}

node* avl::correction(node *head)
{
  node* ret = head;
  
  int bias = abs(height(head->right)) -
             abs(height(head->left));

  //cout << bias << endl;
 
  if (bias > 1)
    {
      if (height(head->right) >= 0)
	{
	  ret = left_rotate(head);
	  set_height(ret);
	}
      else
	{
	  head->right = right_rotate(head->right);
	  ret = left_rotate(head);
	  set_height(ret);
	}	
    }
  
  else if (bias < -1)
    {
      if (height(head->left) <= 0)
	{
	  ret = right_rotate(head);
	  set_height(ret);
	}
      else
	{
	  head->left = left_rotate(head->left);
	  ret = right_rotate(head);
	  set_height(ret);
	}
    }

  set_height(ret);
  return ret;
}

node* avl::avl_insert(node *head, int item)
{
  if (item > head->data)
    {
      if (head->right == NULL)
	{
	  init_right(head, item);
	  return correction(head);
	}
      head->right = avl_insert(head->right, item);
      return correction(head);      
    }
  else
    {
      if (head->left == NULL)
	{
	  init_left(head, item);
	  return correction(head);
	}
      head->left = avl_insert(head->left, item);
      return correction(head);
    }
}

void avl::print_inorder(node *head)
{
  if (head == NULL)
    return;

  print_inorder(head->left);
  cout << head->data << endl;
  print_inorder(head->right);
}

int main()
{
  avl a(0);
  random_device seed;
  mt19937 gen(seed());
  uniform_int_distribution<int> dist(1, 1000);

  set<int> storage;

  while (storage.size() != 200)
    {
      int rng = dist(gen);
      storage.insert(rng);
    }

  set<int>::iterator it = storage.begin();
  
  for (; it != storage.end(); it++)
    {
      a.insert(*it);
    }
  
  
  //a.print_inorder();
  cout << "final height: " << a.height() << endl;
  cout << "size: " << a.size() << endl;
  return 0;
}
