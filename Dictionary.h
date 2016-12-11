 ///
 /// @file    Dictionary.h
 /// @author  chinxi(chenchengxi993@gmail.com)
 /// @date    2016-12-03 10:26:41
 ///

#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include "DictionaryData.h"
#include "DictionaryConf.h"

#include <memory>
#include <vector>
#include <list>

namespace ccx{

using std::shared_ptr;
using std::vector;
using std::list;

class Dictionary
{
	typedef unordered_map<string, pDictElem>::iterator WordIt;
	public:
		Dictionary();
		void push(const string & word);
		void push(vector<string> & words);
		int search(const string & word);
		bool associate(const string & word, vector<string> & data);
	private:
		void AddWord(const string & word, int wordId);
		void splitWord(const string & word, vector<string> & characters);//把词拆成字
		int search(vector<string> & data, pDictElem & pcur);
		pDictElem _dictionary;
		DictionaryConf _conf;	

//遍历
	public:
		string getCurChar();
		string getCurWord();
		int getCurWordId();
		bool isEnd();
		void resetIt();
		void next();
	private:
		void resetPoint(pDictElem pcur);
		void next(pDictElem & pcur, list<WordIt> & stackWord, list<pDictElem> & stackDict);
		void nextWord(pDictElem & pcur, list<WordIt> & stackWord, list<pDictElem> & stackDict);
		string getCurWord(list<WordIt> & stackWord);
		
		pDictElem _pcur;
		WordIt _itcur;
		
//用list实现栈，遍历时方便
		list<WordIt> _stackWord;
		list<pDictElem> _stackDict;

//导入导出
	public:
		void leading_in();
		void leading_out();
};

}

#endif
