/*----------------------------------------------------------------------
ViviFire Programming Language
Authors:
2012-2016 Brent D. Thorn <brent@b6sw.com>

You can get the latest version at the Bay Six Software website at
http://www.b6sw.com/

To the extent possible under law, the author(s) have dedicated all 
copyright and related and neighboring rights to this software to the 
public domain worldwide. This software is distributed without any warranty. 
You should have received a copy of the CC0 Public Domain Dedication along 
with this software. 
If not, please consult the website at
http://creativecommons.org/publicdomain/zero/1.0/
----------------------------------------------------------------------*/

#if !defined(_CONTEXT_H__)
#define _CONTEXT_H__

#include <map>

static std::map<int, int> ctx;

class Context {
	int kind;
public:
	explicit Context(int k);
	~Context();
};

extern bool OutOfContext(int);

#endif
