/*
 * A5_all.cpp
 *
 *Author: Yogesh Nimbhorkar
 */
#include<iostream>
#include<algorithm>
#include<vector>
#include<map>
#include<set>
#include <iterator> 
#include <functional>

using namespace std;
/**************************************************
  Description:
  Problem 1 Solution

//Write a function named isPalindrome that receives a string as parameterand deter -
//mines whether the received string is a palindrome.
**************************************************/
bool is_palindrome(const string& phrase) {
	bool result = false;
	string temp;

//		1. use std::remove copy if to move only alphabet characters from phrase to
//		temp; note that temp is empty; hence the need for an inserter iterator
	//remove_copy_if(phrase.begin(), phrase.end(), back_inserter(temp), isNotAlphabet);
	remove_copy_if(phrase.begin(), phrase.end(), inserter(temp,temp.begin()), [](char ch) {return !isalpha(ch); });

//		2. use std::transform to convert all characters in temp to uppercase(or
//			lowercase) (see page 22, Smart Pointers + Move Semantics, Moodle)
	transform(temp.begin(), temp.end(), temp.begin(), ::toupper);

//		3. use std::equal to compare the first half of temp with its second half,
//		moving forward in the first half(starting at temp.begin()) and moving
//		backward in the second half(starting at temp.rbegin())
	result=equal(temp.begin(), temp.end(),temp.rbegin());

//		4. return the outcome of step 3
	return result;
}


void test_is_palindrome()
{
	std::string str_i = std::string("Was it a car or a cat I saw?");
	std::string str_u = std::string("Was it a car or a cat U saw?");
	cout << " the phrase \"" + str_i + "\" is " +
		(is_palindrome(str_i) ? "" : "not ") + "a palindrome \n";
	cout << " the phrase \"" + str_u + "\" is " +
		(is_palindrome(str_u) ? "" : "not ") + "a palindrome \n";
}


/**************************************************
  Description:
  Problem 2 Solution

//Write a function template named second_max to nd the second largest element in
//a container within a given range[start, finish), where startand finish are
//iterators that provide properties of forward iterators.
//Your function template should be prototyped as follows, and may not use STL algo -
//rithms or containers.

************************************************** */

template <typename Iterator>
pair<Iterator, bool> second_max(Iterator start, Iterator finish) {
	pair<Iterator, bool> p;
	Iterator itr = start;
	Iterator temp1 = start, temp2 = start;

	if (finish - start >= 2) {
	while (itr!=finish){
		if (*itr>*temp1){
			temp2 = temp1;
			temp1 = itr;
		}		
		itr++;
	}
		}else p = make_pair(finish, false);

	if (*temp1==*temp2){
		p = make_pair(start, false);
	}else {
		p= make_pair(temp2, true);
	}

	return p;
}


void test_second_max(std::vector <int > vec)
{
	// note : auto in the following statement is deduced as
	// std :: pair <std :: vector <int >:: iterator , bool >
	auto retval = second_max(vec.begin(), vec.end());
	if (retval.second)
	{
		cout << "The second largest element in vec is "
			<< *retval.first << endl;
	}
	else
	{
		if (retval.first == vec.end())
			cout << " List empty , no elements \n";
		else
			cout << " Container 's elements are all equal to "
			<< *retval.first << endl;
	}
}


/**************************************************
  Description:
  Problem 3 Solution

//Write a function named countStrings that takes two parameters : a
//vector<std::string>(by reference), and an integer n.Your function must
//return the number of strings in the vector whose length is equal to n.Your func -
//tions must use the count_if algorithm.

***************************************************/


struct funct
{ 
	int prob3len{};
	funct(int n):prob3len(n){}

	bool operator()(string& str) {
		bool result = false;
		if (str.length() == prob3len) {
			result = true;
		}
		return result;
	}
};

bool checkLength(string& str, int n) {
	bool result = false;
	if (str.length()== n){
		result = true;
	}
	return result;
}

int countStringLambda(vector<string> & vec, int n) {
	int result{0};
	
	result = count_if(vec.begin(), vec.end(), [=](string value) { if (value.length() == n) return true; else return false; });

	return result;
}

int countStringFreeFun(vector<string> vec, int n) {
	int result{0};
	result = count_if(vec.begin(), vec.end(), bind(checkLength, placeholders::_1, n));

	return result;
}

int countStringFunctor(vector<string> vec, int n) {
	int result{0};
	result = count_if(vec.begin(), vec.end(), funct(n));

	return result;
}


/**************************************************
  Description:
  Problem 4 Solution

//Write a function charFrequency() that reads a string of any number of characters
//from the keyboard, storingand counting the frequencies of each input character(except
//	whitespace).
//	Your code must use a map<char, int> container to store the characters(keys) and
//	their frequency counts(values) as pair<char, int> objects.Do not use any loops;
//instead, use the for_each algorithm to extract characters from the input streamand
//reect the charactersand their frequency counts into the map.

***************************************************/

void charFrequency() {
	cout << "Enter one or more lines of text ." << endl;
	cout << "To end input press Ctrl - Z in Windows and Ctrl - D in Linux" << endl;

	map<char, int> freqMap{};
	istream_iterator<char> start{ cin }, finish{};
	
		for_each(start, finish, [&freqMap](char eachChar) mutable {
			auto itr = freqMap.find(eachChar);
			if (itr != freqMap.end()) {
				++(itr->second);
			}
			else { freqMap.insert(pair<char, int>(eachChar, 1)); }
			});
	
		
		for_each(freqMap.begin(), freqMap.end(), [&freqMap] (pair<char, int> i) {cout << i.first << " " << i.second << endl; } );
}


/**************************************************
  Description:
  Problem 5 Solution

//Modify the construction of the strSet set at line 3 so that the resulting function,
//named multisetUsingMyComparator(), outputs:

***************************************************/

void multisetUsingMyComparator()
{
	auto myComparator = [](string a, string b) { 
		if (a.length() != b.length())
			return (a.length() < b.length());
		else
			return a < b;
	};
	std::multiset <std::string, decltype(myComparator) > strSet(myComparator); // an empty set

	//oveloading 

	// a set that uses the default std :: less <int > to sort the set elements
	std::vector <std::string > vec{ "C", "BB", "A", "CC", "A", "B",
	"BB", "A", "D", "CC", "DDD", "AAA" };

	// copy the vector elements to our set .
	 // We must use a general ( not front or back ) inserter
	 // ( set does not have push_front or push_back members ,
	 // so we can 't use front or back inserter )
	std::copy(vec.begin(), vec.end(), // source container range
		std::inserter(strSet, strSet.begin())); // general inserter

		// create an ostream_iterator for writing to cout ,
		// using a space " " as a separator
	std::ostream_iterator <std::string > out(cout, " ");

	// output the set elements to cout separating them with a space
	std::copy(strSet.begin(), strSet.end(), out);
}


int main()
{
	// problem 1:
	test_is_palindrome();
	cout << "\n";
	 //problem 2:
	std::vector <int > v1{ 1 }; // one element
	test_second_max(v1);
	std::vector <int > v2{ 1, 1 }; // all elements equal
	test_second_max(v2);
	std::vector <int > v3{ 1, 1, 3, 3, 7, 7 }; // at least with two distict elements
	test_second_max(v3);
	cout << "\n";
	 //problem 3:
	std::vector <std::string > vecstr
	{ " count_if ", " Returns ", "the", " number ", "of", " elements ", "in", "the",
	" range ", "[ first ", " last )", "for", " which ", " pred ", "is", " true ."
	};
	cout << countStringLambda(vecstr, 5) << endl;
	cout << countStringFreeFun(vecstr, 5) << endl;
	cout << countStringFunctor(vecstr, 5) << endl;
	cout << "\n";
	 //problem 4:
	charFrequency();
	cout << "\n";
	// problem 5:
	multisetUsingMyComparator();
	cout << "\n";
	return 0;
}