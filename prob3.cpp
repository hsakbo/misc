#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <random>
#include <cctype>
#include <unordered_map>

using namespace std;
typedef long long int lint;

void DEBUGvl(vector<lint>); //use this to print a vector with matching type
void DEBUGop(vector<char>); //use this to print a vector with matching type

lint _MAX = ~((unsigned long long int)0) >> 1; //maximum int64 val
lint _MIN = ~_MAX; //minimum int 64 value

typedef struct min_and_max
{
  lint min{_MAX};
  lint max{_MIN};
}mm; //the array table is filled using these

typedef unordered_map<string, mm> memo;

void DEBUGmax(vector<vector<mm>> &);
void DEBUGmin(vector<vector<mm>> &);
mm naive_alg(vector<lint> &, vector<char> &, int, int, memo &);

lint naive_main(const string &);

lint eval(lint a, lint b, char op)
{

  if (op == '*') 
    return a * b;

  else if (op == '+') 
    return a + b;

  else if (op == '-')
    return a - b;

  else 
    assert(0);
}

//the inner most loop is here in the tabulation approach
//note that the array indexing is different from the lectures.
//I suspect this is what causes the error, but I cannot prove it.
//try using my debug functions to see the array.
const mm find_mm(const vector<vector<mm>> arr, int i, int j,
                    const vector<char> &ops)
{
  mm retval;
  
  for (int k = 0; k < i; k++)
    {
      mm p1 = arr[k][k+j-i];
      mm p2 = arr[i-1-k][j];
      int k1 = j-i+k;

      //four possibility check below
      int tmp = eval(p1.max, p2.max, ops[k1]);
      if (tmp > retval.max)
	retval.max = tmp;
      else if (tmp < retval.min)
	retval.min = tmp;

      tmp = eval(p1.min, p2.max, ops[k1]);

      if (tmp > retval.max)
	retval.max = tmp;
      else if (tmp < retval.min)
	retval.min = tmp;
      
      tmp = eval(p1.max, p2.min, ops[k1]);
      
      if (tmp > retval.max)
	retval.max = tmp;
      else if (tmp < retval.min)
	retval.min = tmp;

      tmp = eval(p1.min, p2.min, ops[k1]);
      
      if (tmp > retval.max)
	retval.max = tmp;
      else if (tmp < retval.min)
	retval.min = tmp;       
      
    }

  return retval;
}

//handles the main logic for tabulation approach
lint main_alg(const vector<lint> &nums, const vector<char> &ops)
{
  vector<vector<mm>> arr(ops.size()+1, vector<mm>(nums.size()));

  for (int i = 0; i < nums.size(); i++)
    {
      arr[0][i].min = arr[0][i].max = nums[i];
    }
  
  for (int i = 1; i < ops.size()+1; i++)
    {
      for (int j = i; j < nums.size(); j++)
	{
	  arr[i][j] = find_mm(arr, i, j, ops);
	}
    }

  return arr[ops.size()][nums.size()-1].max;  
}

//wrapper function for tabulation approach
lint get_maximum_value(const string &exp) {
  //write your code here
  vector<lint> nums{};
  vector<char> ops{};

  string tmp = "";
  for (int i = 0; i < exp.length(); i++)
    {
      if (isdigit(exp[i]))
	tmp += exp[i];

      else if(exp[i] == '*' || exp[i] == '-' || exp[i] == '+')
	{
	  nums.push_back(stoi(tmp));
	  ops.push_back(exp[i]);
	  tmp = "";
	}
    }
  nums.push_back(stoi(tmp));
  
  return main_alg(nums, ops);
}

//debug utils
void test1();
void test2();

int main()
{
  //test1();
  //test2();
  string s;
  getline(cin, s);

  cout << naive_main(s) << '\n';
}

//hard coded testing
void test1()
{
  
  string s1 = "3 - 7 - 1 - 1 * 2";
  string s2 = "4 - 9 + 0 - 7";
  string s3 = "5 - 8 + 7 * 4 - 8 + 9";
  string s4 = "2 * 4 + 3";
  
  cout << naive_main(s2) << endl;
  cout << get_maximum_value(s2) << endl;

}

//debug utils -- the inifinite loop change size (make sure its even)
void test2()
{
  random_device seed;
  mt19937 gen(seed());
  uniform_int_distribution<int> dist(0, 9);
  uniform_int_distribution<int> distop(0, 2);
  int size = 30;
  
  string start;
  int wrong, correct;
  lint status = 0;
  while (true)
    {
      cout << "status: " << ++status << "\n";
      start = "";
      int tmp = dist(gen);
      start += to_string(tmp);
      for (int i = 0; i < size; i++)
	{
	  if (i % 2)
	    {
	      start += " ";
	      start += to_string(dist(gen));
	    }
	  else
	    {
	      start += " ";
	      int rng = distop(gen);
	      switch (rng)
		{
		case 0:
		  start += "+";
		  break;

		case 1:
		  start += "-";
		  break;

		case 2:
		  start += "*";
		  break;

		default:
		  assert(0);
		}
	    }
	}

      wrong = get_maximum_value(start);
      correct = naive_main(start);

      if (wrong != correct)
	break;
    }

  cout << "DEBUG: " << start << endl;
  cout << "expected:\t" << correct << endl;
  cout << "returned:\t" << wrong << endl;
}

//debug utils
void DEBUGvl(vector<lint> a)
{
  cout << "DEBUGvl (" << a.size() << "): ";

  for (lint item: a)
    {
      cout << item << " ";
    }
  cout << endl;
}

//debug utils
void DEBUGop(vector<char> a)
{
  cout << "DEBUGop (" << a.size() << "): ";

  for (char item: a)
    {
      cout << item << " ";
    }
  cout << endl;
}

//debug utils
void DEBUGmax(vector<vector<mm>> &arr)
{
  cout << "DEBUG max (" << arr.size() << "x" << arr[0].size()
       << "):" << endl;

  for (auto v: arr)
    {
      for (mm item: v)
	{
	  if (item.max != _MIN)
	    cout << item.max << "\t";
	  else
	    cout << "\t";
	}
      cout << endl;
    }
}

//debug utils
void DEBUGmin(vector<vector<mm>> &arr)
{
  cout << "DEBUG min (" << arr.size() << "x" << arr[0].size()
       << "):" << endl;

  for (auto v: arr)
    {
      for (mm item: v)
	{
	  if (item.min != _MAX)
	    cout << item.min << "\t";
	  else
	    cout << "\t";
	}
	
      cout << endl;
    }
}

//top down approach
mm naive_alg(vector<lint> &nums, vector<char> &ops, int i, int j, memo &data)
{
  mm retval;
  string l_str = to_string(i) + " " + to_string(j);

  if (i == j)
    {
      retval.min = nums[i];
      retval.max = nums[j];
      data.insert(pair<string,mm>(l_str,retval));
      
      return retval;
    }
  else if (i+1 == j)
    {
      retval.max = eval(nums[i], nums[j], ops[i]);
      retval.min = retval.max;
      data.insert(pair<string,mm>(l_str,retval));
      return retval;
    }

  for (int k = i; k < j; k++)
    {
      mm r_left, r_right;
      
      string l = to_string(i) + " " + to_string(k);
      string r = to_string(k+1) + " " + to_string(j);


      if (data.find(l) == data.end())
	r_left= naive_alg(nums, ops, i, k, data);
	
      else
	r_left = data[l];
	
      if (data.find(r) == data.end())
	r_right = naive_alg(nums, ops, k+1, j,data);
	
      else
	r_right = data[r];

      //the code below checks the four possibilities.
      int tmp = eval(r_left.min, r_right.min, ops[k]);
      if (retval.min > tmp)
	retval.min = tmp;
      else if (retval.max < tmp)
	retval.max = tmp;

      tmp = eval(r_left.min, r_right.max, ops[k]);
      if (retval.min > tmp)
	retval.min = tmp;
      else if (retval.max < tmp)
	retval.max = tmp;

      tmp = eval(r_left.max, r_right.min, ops[k]);
      if (retval.min > tmp)
	retval.min = tmp;
      else if (retval.max < tmp)
	retval.max = tmp;

      tmp = eval(r_left.max, r_right.max, ops[k]);
      if (retval.min > tmp)
	retval.min = tmp;
      else if (retval.max < tmp)
	retval.max = tmp;
      
    }
  
  data.insert(pair<string,mm>(l_str,retval));
  return retval;
}


//wrapper function for the recursive top-down algorithm above
lint naive_main(const string &exp)
{
  //write your code here
  vector<lint> nums{};
  vector<char> ops{};

  string tmp = "";
  for (int i = 0; i < exp.length(); i++)
    {
      if (isdigit(exp[i]))
	tmp += exp[i];

      else if(exp[i] == '*' || exp[i] == '-' || exp[i] == '+')
	{
	  nums.push_back(stoi(tmp));
	  ops.push_back(exp[i]);
	  tmp = "";
	}
    }
  nums.push_back(stoi(tmp));
  memo data;
  
  lint ret = naive_alg(nums, ops, 0, nums.size()-1, data).max;
  return ret;
}
