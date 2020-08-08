/*!
\file ezjson.cpp
test ec_json with C++ 11

\author	jiangyong
\email  kipway@outlook.com
\date   2020.8.8
*/

#ifdef _WIN32
#ifndef _CRT_SECURE_NO_WARNINGS
#	define _CRT_SECURE_NO_WARNINGS 1
#endif
#	pragma warning (disable : 4995)
#	pragma warning (disable : 4996)
#	pragma warning (disable : 4200)
#	ifndef _WIN32_WINNT
#		define _WIN32_WINNT 0x0600	//0x600 = vista/2008; 0x601=win7 ;  0x0501=windows xp ;  0x0502=windows 2003
#	endif
#else
#	include <termios.h>
#	include <unistd.h>
#endif
#include <cstdio>
#include <cstdint>
#include <string>
#include "ec_jsonx.h"

inline void prt_space(int nspace)
{
	while (nspace > 0) {
		std::printf("\x20");
		nspace--;
	}
}

void prtstr(const char* s, size_t size, int ntype = -1)
{
	std::printf("%.*s", size, s);
	switch (ntype) {
	case ec::json::jtype::jstring:
		std::printf("(string)");
		break;
	case ec::json::jtype::jobj:
		std::printf("(object)");
		break;
	case ec::json::jtype::jarray:
		std::printf("(array)");
		break;
	case ec::json::jtype::jnumber:
		std::printf("(number)");
		break;
	}
}

void prtjson(ec::json &js, int nspace = 0)// Recursive print json object
{
	int i = 0;
	for (auto & vi : js._kvs) {
		if (ec::json::jtype::jarray != vi._type && ec::json::jtype::jobj != vi._type) { // vals
			prt_space(nspace);
			if (!vi._k.empty()) {
				prtstr(vi._k._str, vi._k._size, vi._type);
				std::printf(":");
				prtstr(vi._v._str, vi._v._size);
				std::printf("\n");
			}
			else {
				std::printf("[%d] ", i++);
				prtstr(vi._v._str, vi._v._size);
				std::printf("\n");
			}
		}
		else {
			ec::json js;
			if (!js.from_str(vi._v)) {
				prt_space(nspace);
				std::printf("err-> ");
				prtstr(vi._k._str, vi._k._size, vi._type);
				printf(":");
				prtstr(vi._v._str, vi._v._size);
				std::printf("\n");
			}
			else {
				prt_space(nspace);
				if (!vi._k.empty()) {
					if (ec::json::jtype::jarray == vi._type) {
						prtstr(vi._k._str, vi._k._size, vi._type);
						std::printf(":[ \n");
					}
					else {
						prtstr(vi._k._str, vi._k._size, vi._type);
						std::printf(":{ \n");
					}
				}
				else {
					if (ec::json::jtype::jarray == vi._type)
						std::printf("[%d] [\n ", i++);
					else
						std::printf("[%d] {\n ", i++);
				}
				prtjson(js, nspace + 4);
				prt_space(nspace);
				if (ec::json::jtype::jarray == vi._type)
					std::printf("]\n");
				else
					std::printf("}\n");
			}
		}
	};
}

int main()
{
	//1. load from file
	const char* sfile = "./js1.txt";
	std::string src;
	if (!ec::json::load_file(sfile, src)) {
		std::printf("load file %s failed.\n", sfile);
		return 0;
	}

	//2. delete comment, not required
	std::string jstr;
	jstr.reserve(src.size() + 8 - src.size() % 8);
	ec::json::del_comment(src.data(), src.size(), jstr);

	// 3.parse and print all node
	ec::json js;
	if (!js.from_str(jstr.data(), jstr.size())) {
		std::printf("parse failed.\n");
		return 0;
	}
	prtjson(js);
}