#include "part3/ECBDecrypt.h"
#include "part3/EncryptionOracle.h"
#include <iostream>  
using namespace std; 
#include <typeinfo>


template<typename T>
std::vector<T> slice(std::vector<T> const &v, int m, int n)
{
    auto first = v.cbegin() + m;
    auto last = v.cbegin() + n + 1;

    std::vector<T> vec(first, last);
    return vec;
}

int get_prefix_size(EncryptionOracle &e){
	for (int i = 0; i<16; i++){
		vector<uint8_t> buffer(i+16*5, 'A');
		auto ciphertext = e.encrypt(buffer); 
		vector<vector<unsigned char>> test;

		// create a vector of blocks to check for matching  blocks
		for (int j = 0; j<ciphertext.size() - 16 + 1;j+=16){
			vector<unsigned char> block = slice(ciphertext, j, j+15);
			test.push_back(block);
		}

		// find  5 repeating  blocks
		for (int j = 0; j<test.size() - 4 + 1; j++){
			auto t = test.at(j);
			if (test.at(j)==test.at(j+1)){
				if(test.at(j+1)==test.at(j+2)){
					if(test.at(j+2)==test.at(j+3)){
						if(test.at(j+3)==test.at(j+4)){
							return i;
						}
					}
				}
			}
		}
	}
	return 99;
}
int check_match(vector<uint8_t> vec){
	vector<vector<unsigned char>> test;
	// create a vector of blocks to check for matching  blocks
	for (int j = 0; j<vec.size() - 16 + 1;j+=16){
		vector<unsigned char> block = slice(vec, j, j+15);
		test.push_back(block);
	}

	// find initial repeating  blocks
	int found = 0;
	int test_idx = 0;
	while (!found){
		if (test.at(test_idx)==test.at(test_idx+1))
			found = 1;
		test_idx++;
	}
	test_idx++;

	// check only relevant blocks with hypothesis
	for (int j = test_idx; j<test.size() - 1; j++){
		if (test.at(test_idx)==test.at(j+1))
			if (j+1 == test.size()-1)
				return 2;
			else
				return 1;
	}
	return 0;
}

std::string ECBDecrypt::grader_decrypt(EncryptionOracle &e) {
 
	int pref_size = get_prefix_size(e);
	vector<uint8_t> buffer(pref_size+16*2, 'A');


	vector<uint8_t> result;
	vector<uint8_t> short_buf(15, 'A');
	vector<uint8_t> hypo_buf(15, 'A');
	int complete = 0;

	while(!complete){
		int found = 0;
		uint8_t hypo = -1;
		while (!found){
			hypo++;
			vector<uint8_t> v;
			// make prefix ignorable
			v.insert(v.end(), buffer.begin(), buffer.end());

			// insert hypothesis
			v.insert(v.end(), hypo_buf.begin(), hypo_buf.end());
			v.push_back(hypo);

			// insert short buffer
			v.insert(v.end(), short_buf.begin(), short_buf.end());

			found = check_match(e.encrypt(v));
		}
		hypo_buf.push_back(hypo);
		hypo_buf.erase(hypo_buf.begin());

		if (short_buf.size() == 0)
			short_buf = vector<uint8_t> (15, 'A');
		else
			short_buf.pop_back();

		result.push_back(hypo);
		
		if (found == 2)
			complete = 1;
	}
	result.push_back(0);
	auto res_str =  reinterpret_cast<char*>(result.data());

  return res_str;
}
