#include <vector>

using namespace std;
using vint = vector<int>;

/**
 * Creates a disjoint set forest with constructor taking
 * the size of the set. Nodes are to be referred by their
 * 0-based indexing. How to use: construct with the size of
 * the set and use the order of the indexes in a vector.
 *
 * @author Hiromitsu Arai
 */
class disjoint_set
{
private:
  
  typedef struct node
  {
    node* p;
    unsigned int rank;
    int pos;
  }node;
  
  vector<node> forest;

  node* find_set(node *x)
  {
    if (x != x->p)
      {
	x->p = find_set(x->p);
      }
    return x->p;
  }
  size_t size_val{0}; 
public:
  disjoint_set(int size);
  void merge(size_t s, size_t d);
  bool same(size_t a, size_t b);
  size_t size() {return size_val;} //count of total roots
  int find_root(int pos); //gives the root index of argument
};

disjoint_set::disjoint_set(int size)
{
  forest.reserve(size);
  for (size_t i = 0; i < size; i++)
    {
      size_val++;
      node temp;
      temp.rank = 0;
      forest[i] = temp;
      forest[i].p = &forest[i];
      forest[i].pos = i;
    }
}


void disjoint_set::merge(size_t s, size_t d)
{
  node *source = find_set(&forest[s]);
  node *dest = find_set(&forest[d]);

  if (source != dest)
    {
      if (source->rank > dest->rank)
	{
	  //logic for merge (dest child of source)
	  dest->p = source;
	}
      else
	{
	  //logic for merge (source child of dest)
	  source->p = dest;
	  if (source->rank == dest->rank)
	    dest->rank++;
	}
      size_val--;
    }
}

bool disjoint_set::same(size_t a, size_t b)
{
  node* aptr = find_set(&forest[a]);
  node* bptr = find_set(&forest[b]);
  if (aptr == bptr)
    return true;
  
  return false;
}

int disjoint_set::find_root(int pos)
{
  node* r = find_set(&forest[pos]);
  return r->pos;
}
