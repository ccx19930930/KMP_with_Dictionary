 ///
 /// @file    Dictionary.cc
 /// @author  chinxi(chenchengxi993@gmail.com)
 /// @date    2016-12-03 10:47:12
 ///

#include "Dictionary.h"
#include <iostream>
#include <fstream>
#include <string>
#include <json/json.h>

namespace ccx{

using std::endl;
using std::cout;
using std::pair;
using std::ofstream;
using std::ifstream;

Dictionary::Dictionary()
: _dictionary(new DictElem)
, _conf()
{
	_dictionary->_isend = false;
	_dictionary->_next = 0;
	_pcur = _dictionary;
}

void Dictionary::splitWord(const string & word, vector<string> & characters)
{
	int num = word.size();
	int i = 0;
	while(i < num)
	{
		int size = 1;
		if(word[i] & 0x80)
		{
			char temp = word[i];
			temp <<= 1;
			do{
				temp <<= 1;
				++size;
			}while(temp & 0x80);
		}
		string subWord;
		subWord = word.substr(i, size);
		characters.push_back(subWord);
		i += size;
	}
}

void Dictionary::getKmpNext(const vector<string> & characters, vector<int> & kmpnext)
{
	int size = characters.size();
	for(int i = 0; i < size; ++i)
	{
		kmpnext.push_back(0);
	}
	
	int i = -1;
	int j = 0;
	kmpnext[0] = -1;
	while(j < size)
	{
		if(i == -1 || kmpnext[i] == kmpnext[j])
		{
			++i;
			++j;
			kmpnext[j] = i;
		}else{
			i = kmpnext[i];
		}
	}
	for(i = 0; i < size; ++i)
	{
		kmpnext[i] = i - kmpnext[i];
	}
}

void Dictionary::AddWord(const string & word)
{
	vector<string> characters;
	splitWord(word, characters);
	vector<int> kmpnext;
	getKmpNext(characters, kmpnext);

	vector<int>::iterator it_int;
	it_int = kmpnext.begin();
	vector<string>::iterator it_char;
	it_char = characters.begin();	
	pDictElem root;
	root = _dictionary;
	for(; it_char != characters.end(); ++it_char, ++it_int)
	{
		WordIt it_word;
		it_word = root->_words.find(*it_char);

		if(it_word == root->_words.end())
		{
			pair<string, pDictElem> temp;
			temp.first = *it_char;
			pDictElem dictemp(new DictElem);
			dictemp->_word = *it_char;
			dictemp->_next = *it_int;
			dictemp->_isend = false;
			temp.second = dictemp;
			root->_words.insert(temp);
			root = dictemp;
		}else{
			root = it_word->second;
		}
	}
	if(!root->_isend)
	{
		root->_isend = true;
	}
}

void Dictionary::push(const string & word)
{
	AddWord(word);
}

void Dictionary::push(vector<string> & words)
{
	int size = words.size();
	for(int i = 0; i < size; ++i)
	{
		push(words[i]);
	}
}

bool Dictionary::search(const string & word)
{
	pDictElem root = _dictionary;
	vector<string> temp;
	splitWord(word, temp);

	int ret = search(temp, root);
	int size = temp.size();
	if(ret != size)
	{
		return false;
	}
	return true;
}

int Dictionary::search(vector<string> & characters, pDictElem & root)
{
	vector<string>::iterator it_char;
	it_char = characters.begin();	
	root = _dictionary;
	int i = 0;
	for(; it_char != characters.end(); ++it_char, ++i)
	{
		WordIt it_word;
		it_word = root->_words.find(*it_char);

		if(it_word == root->_words.end())
		{
			break;
		}else{
			root = it_word->second;
		}
	}
	return i;
}

bool Dictionary::associate(const string & word, vector<string> & data)
{
	pDictElem root = _dictionary;
	vector<string> temp;
	splitWord(word, temp);

	int ret = search(temp, root);
	int size = temp.size();
	if(ret != size)
	{
		return false;
	}

	list<WordIt> stackWord;
	list<pDictElem> stackDict;
	next(root, stackWord, stackDict);
	while(root)
	{
		string temp = getCurWord(stackWord);
		data.push_back(temp);	
		next(root, stackWord, stackDict);
	}

	if(!data.size())
	{
		return false;
	}
	return true;
}

string Dictionary::Kmp(const string & word)
{
	vector<string> temp;
	splitWord(word, temp);
	vector<Loc> loc;
	
	if(!Kmp(temp, loc))
	{
		return word;
	}
	int size = loc.size();
	for(int i = 0; i < size; ++i)
	{
		for(int j = loc[i].first; j <= loc[i].second; ++j)
		{
			temp[j] = "*";
		}
	}
	string ret;
	for(auto & elem : temp)
	{
		ret += elem;
	}
	return ret;
}

bool Dictionary::Kmp(vector<string> & word, vector<Loc> & loc)
{
	pDictElem root = _dictionary;
	list<pDictElem> stackDict;

	int start = 0;
	int size = word.size();
	int i = 0;
	while(i < size)	
	{
		WordIt it_word;
		it_word = root->_words.find(word[i]);
		if(it_word == root->_words.end())
		{
			if(stackDict.size())
			{
				int num = root->_next;
				for(int j = 0; j < num - 1; ++j)
				{
					stackDict.pop_back();	
				}
				root = stackDict.back();
				stackDict.pop_back();
				start += num;
			}else{
				++i;
				start = i;
			}
			continue;
		}else{
			stackDict.push_back(root);
			root = it_word->second;
			if(root->_isend)
			{
				Loc loctemp;
				loctemp.first = start;
				loctemp.second = i;
				loc.push_back(loctemp);
				start = i + 1;
			}
		}
		++i;
	}
	return loc.size();
}

//遍历用

void Dictionary::resetPoint(pDictElem pcur)
{
	_pcur = pcur;
	if(_stackDict.size())
	{
		_stackDict.clear();
	}
	if(_stackWord.size())
	{
		_stackWord.clear();
	}
	next();
}

void Dictionary::resetIt()
{
	resetPoint(_dictionary);
}

void Dictionary::next()
{
	next(_pcur, _stackWord, _stackDict);
}

void Dictionary::next(pDictElem & pcur, list<WordIt> & stackWord, list<pDictElem> & stackDict)
{
	while(pcur)
	{
		nextWord(pcur, stackWord, stackDict);
		if(!pcur || pcur->_isend)
		{
			break;
		}
	}
}

void Dictionary::nextWord(pDictElem & pcur, list<WordIt> & stackWord, list<pDictElem> & stackDict)
{
	if(pcur)
	{
		if(pcur->_words.size())
		{
			stackDict.push_back(pcur);
			stackWord.push_back(pcur->_words.begin());
			pcur = stackWord.back()->second;
		}else{
			++(stackWord.back());
		}
		while(stackWord.back() == stackDict.back()->_words.end())
		{
			stackDict.pop_back();
			stackWord.pop_back();
			if(!stackDict.size())
			{
				pcur = NULL;
			}
			++(stackWord.back());
		}
		if(pcur)
		{
			pcur = stackWord.back()->second;
		}	
	}
}

string Dictionary::getCurChar()
{
	return _pcur->_word;
}

string Dictionary::getCurWord()
{
	return getCurWord(_stackWord);
}

string Dictionary::getCurWord(list<WordIt> & stackWord)
{
	string temp;
	list<WordIt>::iterator it_word;	
	it_word = stackWord.begin();	

	for(; it_word != stackWord.end(); ++it_word)
	{
		temp += (*it_word)->first;
	}
	return temp;
}

bool Dictionary::isEnd()
{
	return _pcur == NULL;
}

void Dictionary::leading_in()//导入，失败没必要退出程序
{
	ifstream ifs;
	const char * path = _conf.getDictionaryPath().c_str();
	ifs.open(path);
	if(!ifs.good())
	{
		cout << "open Dictionary.json error(leading_in)" << endl;
	}else{
		Json::Value root;
		Json::Reader reader;

		if(!reader.parse(ifs, root, false))
		{
			cout << "json read Dictionary.json error" << endl;
		}else{
			int size = root.size();
			for(int i = 0; i < size; ++i)
			{
				string word = root[i]["Word"].asString();
				AddWord(word);
			}
		}
	}
}

void Dictionary::leading_out()
{
	Json::Value root;
	Json::FastWriter writer;

	resetIt();

	while(!isEnd())
	{
		Json::Value elem;
		elem["Word"] = getCurWord();
		root.append(elem);
		next();
	}

	string words;
	words = writer.write(root);

	ofstream ofs;
	const char * path = _conf.getDictionaryPath().c_str();
	ofs.open(path);
	if(!ofs.good())
	{
		cout << "open Dictionary.json error(leading_out)" << endl;
		ofs.open("Dictionary.tmp");
		if(!ofs.good())
		{
			exit(EXIT_FAILURE);
		}
	}

	ofs << words;
	ofs.close();
}

}
