#include <iostream>
#include <memory>
#include <vector>
#include <random>

/**
 * creates an open-address hash table map for kmers.
 * This table is designed to efficiently associate
 * data by the prefix-suffix of kmers. 
 *
 * @author Hiromitsu Arai
 */
template <typename T>
class kmap
{
private:
  const std::vector<std::string> &data;
  std::unique_ptr<T []> table;
public:
  kmap(std::vector<std::string> &kmers);
  void DEBUG();
};

template <typename T>
kmap<T>::kmap(std::vector<std::string> &kmers) : data(kmers)
{
  this->table = std::unique_ptr<T[]>(new T[kmers.size()*2]);
}

template <typename T>
void kmap<T>::DEBUG()
{
  std::cout << "entered DEBUG" << std::endl;
  for (std::string a: data) std::cout << a << std::endl;
}


int main()
{
  using namespace std;
  using vstr = vector<string>;
  string rng_string(int);
  void debug();
  debug();
  
  return 0;
}


std::string rng_string(int length)
{
  std::random_device seed;
  std::mt19937 gen(seed());
  std::uniform_int_distribution<int> dist(65, 68);

  std::string retv = "";
  for (int i = 0; i < length; i++)  retv += dist(gen);
  return retv;
}

void debug()
{
  using namespace std;
  using vstr = vector<string>;
  constexpr int SIZE = 100;
  constexpr int LENGTH = 10;
  vstr test(SIZE);
  for (int i = 0; i < SIZE; i++)
    test[i] = rng_string(LENGTH);
    

  kmap<int> mymap(test);
  //mymap.DEBUG();
}
