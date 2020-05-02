#include "part4/PaddingOracle.h"
#include "part4/PaddingOracleAttack.h"
using namespace std; 
#include <iostream> 
#include <typeinfo>
#include <valarray>


template<typename T>
std::vector<T> slice_vec(std::vector<T> const &v, int m, int n)
{
    auto first = v.cbegin() + m;
    auto last = v.cbegin() + n + 1;

    std::vector<T> vec(first, last);
    return vec;
}

void print(vector<uint8_t> x){
  for (int i = 0; i < x.size(); i++)
	  cout << x[i];
  cout << "\n";
}

std::string PaddingOracleAttack::grader_decrypt(PaddingOracle &o) {
  // NOTE: Please write your code here.

  // first = IV, second = ciphertext
  auto cipher_iv = o.encrypt();

  vector<uint8_t> iv = cipher_iv.first;
  vector<uint8_t> ciphertext = cipher_iv.second;

  // create a vector of blocks
  vector <vector<uint8_t>> blocks;
  for (int j = 0; j<ciphertext.size() - 16 + 1;j+=16){
	  vector<uint8_t> block = slice_vec(ciphertext, j, j+15);
	  blocks.push_back(block);
  }
  
  vector<uint8_t> result;

  int numblock = blocks.size();
  for (int k = 0; k<numblock; k++){
	  vector<uint8_t> block_hypo;

	  vector<uint8_t> target = blocks.at(k);
	  vector<uint8_t> prev = iv;
	  
	  if (k-1 >= 0)
		  prev = blocks.at(k-1);

	  for (int i = 0; i<16; i++){
		  vector<uint8_t> mask(16-1-i,0);
	
		  for (int j = 0; j<i+1; j++)
			  mask.push_back(i+1);
	  
		  int found = 0;
		  uint8_t hypo = 1;
	 
		  vector<uint8_t> v(16-i,0);
		  int insert_idx = v.size()-1;
		  v.insert(v.end(), block_hypo.begin(), block_hypo.end());
	 
		  while (!found){
			  hypo++;
			  v[insert_idx] = hypo;
	
			  valarray<uint8_t> a(prev.data(), prev.size());
			  valarray<uint8_t> b(mask.data(), mask.size());
			  valarray<uint8_t> c(v.data(), v.size());
	
			  valarray<uint8_t> prev_arr = a^b^c;
			  vector<uint8_t> alt_prev;
			  alt_prev.assign(std::begin(prev_arr), std::end(prev_arr));
	  
			  found = o.isValid(alt_prev, target);
		  }

		  block_hypo.insert(block_hypo.begin(), hypo);
	  }
	  result.insert(result.end(), block_hypo.begin(), block_hypo.end());
  }
  result.push_back(0);
  return reinterpret_cast<char*>(result.data());
}
