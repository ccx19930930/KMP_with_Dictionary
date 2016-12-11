 ///
 /// @file    DictionaryData.h
 /// @author  chinxi(chenchengxi993@gmail.com)
 /// @date    2016-12-03 10:16:51
 ///

#ifndef __DICTIONARYDATA_H__
#define __DICTIONARYDATA_H__

#include <string>
#include <unordered_map>
#include <memory>

namespace ccx{

using std::string;
using std::unordered_map;
using std::shared_ptr;

struct DictElem
{
	string _word;
	bool _isend;//是否到词尾
	int _next;//KMP next值 此处有修改，存的是弹栈数量
	unordered_map<string, shared_ptr<DictElem> > _words;
};

typedef shared_ptr<DictElem> pDictElem;

}

#endif
