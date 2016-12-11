 ///
 /// @file    test.cc /// @author  chinxi(chenchengxi993@gmail.com)
 /// @date    2016-12-03 11:06:16
 ///

#include "Dictionary.h"
#include <iostream>
#include <string>
#include <vector>
using std::cout;
using std::endl;
using std::string;
using std::vector;

int test1()
{
	ccx::Dictionary words;
	string word1 = "编程入门";	
	string word2 = "编程软件";	
	string word3 = "编程学习";	
	string word4 = "编程学习网站";	
	
	words.push(word1);	
	words.push(word2);	
	words.push(word3);	
	words.push(word4);	

	words.resetIt();
	
	while(!words.isEnd())
	{
		cout << "\tchar : " << words.getCurChar() 
			<< "\tword : " << words.getCurWord() << endl;
		words.next();
	}
	
	words.leading_out();
	return 0;
}


int test2()
{
	ccx::Dictionary words;
	words.leading_in();


	string word("编程训练");
	words.push(word);
	words.resetIt();

	while(!words.isEnd())
	{
		cout << "\tchar : " << words.getCurChar() 
			<< "\tword : " << words.getCurWord() << endl;
		words.next();
	}
	string tmp = "编程学习";	
	int id = words.search(tmp);
	if(-1 == id)
	{
		cout << "no such word like \"" << tmp << "\"" << endl;
	}else{
		cout << "find ID : " << id
			<< "\tword : " << tmp << endl;
	}

	cout << endl;
	cout << "associate \"编程\" : " << endl;

	vector<string> data;	
	string temp = "编程";
	
	if(words.associate(temp, data))
	{
		cout << "find!" << endl;
		for(auto & elem : data)
		{
			cout << elem << endl;
		}
	}else{
		cout << "can't find" << endl;
	}


	return 0;
}

void test3()
{
	ccx::Dictionary words;
	words.leading_in();
	
	string s = "我不要好好 玩耍好好进行编程学习然后建一个编程编程编程学    习网站给编程纩编程软件者使用进行编程训练与编程学习";

	cout << s << endl;
	s = words.Kmp(s);
	cout << s << endl;
}

int main()
{
#if 0
	cout << "test1" << endl;
	test1();
	cout << endl;
	cout << "test2" << endl;
	test2();
	cout << endl;
#endif
	cout << "test3" << endl;
	test3();
	cout << endl;
}
