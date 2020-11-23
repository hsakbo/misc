#include <iostream>
#include <random>

int main()
{
  std::random_device seeds;

  std::mt19937 gen(seeds());
  
  std::uniform_int_distribution<int> dist(0, 100); //int 0-100

  using namespace std;
  for (int i = 0; i < 1000; i++)
    {
      int random_number = dist(gen);
      std::cout << random_number << std::endl;
    }
  
  return  0;
}
