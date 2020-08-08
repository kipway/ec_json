/*!
\file ec_jsonx.h
\author	jiangyong
\email  kipway@outlook.com
\update 2020.8.8

eclibe fast json parse class, no copy

eclib 2.0 Copyright (c) 2017-2020, kipway
source repository : https://github.com/kipway/eclib

Licensed under the Apache License, Version 2.0 (the "License");
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
*/
#pragma once
#include <string.h>
#include <string>
#include <vector>
#ifndef MAXSIZE_JSONX_KEY
#	define MAXSIZE_JSONX_KEY 63
#endif
namespace ec
{
	struct txt {
		const char* _str;
		size_t _size;
		txt() : _str(nullptr), _size(0)
		{
		}
		txt(const char *s) : _str(s), _size(strlen(s))
		{
		}
		txt(const char *s, size_t size) : _str(s), _size(size)
		{
		}
		inline static int tolow(int c)
		{
			return (c >= 'A' && c <= 'Z') ? c + 'a' - 'A' : c;
		}
		inline void clear()
		{
			_str = nullptr;
			_size = 0;
		}
		inline bool empty() const
		{
			return !_size || !_str;
		}
		void trimleft()
		{
			while (_size > 0 && (*_str == '\x20' || *_str == '\t' || *_str == '\r' || *_str == '\n')) {
				++_str;
				--_size;
			}
		}
		void trimright()
		{
			const char* s = _str + _size - 1;
			while (_size > 0 && (*s == '\x20' || *s == '\t' || *s == '\r' || *s == '\n')) {
				--s;
				--_size;
			}
		}
		inline void trim()
		{
			trimleft();
			trimright();
		}
		bool eq(txt* p) const // equal
		{
			if (!p || !_str || !_size || p->_size != _size)
				return false;
			size_t i = _size;
			const char *s1 = p->_str, *s2 = _str;
			while (i) {
				if (*s1++ != *s2++)
					return false;
				--i;
			}
			return true;
		}
		bool eq(const txt &v) const // equal
		{
			if (!_str || !_size || v._size != _size)
				return false;
			size_t i = _size;
			const char *s1 = v._str, *s2 = _str;
			while (i) {
				if (*s1++ != *s2++)
					return false;
				i--;
			}
			return true;
		}

		bool eq(const char* s) const // equal
		{
			if (!s || !_str || !_size)
				return false;
			const char* sr = _str;
			size_t i = _size;
			while (i && *s) {
				if (*s++ != *sr++)
					return false;
				i--;
			}
			return !*s && !i;
		}
		bool ieq(const txt &v) const // Case insensitive equal
		{
			if (!_str || !_size || v._size != _size)
				return false;
			size_t i = _size;
			const char *s1 = v._str, *s2 = _str;
			while (i) {
				if (tolow(*s1++) != tolow(*s2++))
					return false;
				i--;
			}
			return true;
		}
		bool ieq(const char* s) const // Case insensitive equal
		{
			if (!s || !_str || !_size)
				return false;
			const char* sr = _str;
			size_t i = _size;
			while (i && *s) {
				if (tolow(*s++) != tolow(*sr++))
					return false;
				i--;
			}
			return !*s && !i;
		}
		bool ieq(const txt* p) const // Case insensitive equal
		{
			if (!p || !_str || !_size || p->_size != _size)
				return false;
			size_t i = _size;
			const char *s1 = p->_str, *s2 = _str;
			while (i) {
				if (tolow(*s1++) != tolow(*s2++))
					return false;
				i--;
			}
			return true;
		}
		bool tochar(char c) // move to c
		{
			while (_size && *_str != c) {
				_str++;
				_size--;
			}
			return _size != 0;
		}
		bool tochar(const char* cs) //move to char oneof cs
		{
			while (_size && !strchr(cs, *_str)) {
				_str++;
				_size--;
			}
			return _size != 0;
		}
		bool skip() // skip space and point to a none space char
		{
			while (_size && (*_str == '\x20' || *_str == '\t' || *_str == '\r' || *_str == '\n')) {
				_str++;
				_size--;
			}
			return _size != 0;
		}
		inline bool skipto(char c) // skip space and check the first none space char is c
		{
			return (skip() && *_str == c);
		}
		inline bool skipto(const char* cs) // skip space and check the first none space char is c
		{
			return (skip() && strchr(cs, *_str));
		}
		bool tonext()
		{
			if (_size && _str) {
				--_size;
				++_str;
				return true;
			}
			return false;
		}
		inline int stoi() const// atoi()
		{
			return (int)stoll();
		}
		long long stoll() const // atoll()
		{
			char s[32];
			if (!_str || !_size)
				return 0;
			size_t n = _size > 31 ? 31 : _size;
			memcpy(s, _str, n);
			s[n] = 0;
			return atoll(s);
		}
		inline double stof() const
		{
			char s[88];
			if (!_str || !_size)
				return 0;
			size_t n = _size > 80 ? 80 : _size;
			memcpy(s, _str, n);
			s[n] = 0;
			return atof(s);
		}
		int get(char *pout, size_t sizeout) const // return get chars
		{
			if (!_str || _size >= sizeout)
				return 0;
			memcpy(pout, _str, _size);
			pout[_size] = 0;
			return (int)_size;
		}
		bool json_tochar(int c) // for json
		{
			while (_size) {
				if (*_str == c && *(_str - 1) != '\\')
					break;
				_str++;
				_size--;
			}
			return _size != 0;
		}
		bool json_tochar(const char* cs) // for json
		{
			while (_size) {
				if (strchr(cs, *_str) && *(_str - 1) != '\\')
					break;
				++_str;
				--_size;
			}
			return _size != 0;
		}
		inline bool json_toend(char cs, char ce) // for json;  v:[0,1,2,3,4];  _str point to 0, move to ;
		{
			int nk = 1;
			char cp = 0;
			while (_size && nk) {
				if (*_str == '"' && *(_str - 1) != '\\') {
					cp = cp == '"' ? 0 : '"';
				}
				if (*_str == cs && !cp && *(_str - 1) != '\\')
					++nk;
				else if (*_str == ce && !cp && *(_str - 1) != '\\')
					--nk;
				++_str;
				--_size;
			}
			return !nk;
		}

		bool isintstr() const
		{
			if (!_str || !_size)
				return true;
			size_t i, ns = 0;
			for (i = 0u; i < _size; i++) {
				if (_str[i])
					break;
			}
			ns = i;
			for (; i < _size; i++) {
				if (_str[i] >= '0' && _str[i] <= '9')
					continue;
				if (_str[i] == '-') {
					if (i != ns)
						return false;
				}
				else
					return false;
			}
			return true;
		}
	};

	class json // parse json object, fast no copy
	{
	public:
		enum jtype {
			jstring = 0,
			jobj = 1,
			jarray = 2,
			jnumber = 3
		};

		struct t_kv {
			txt _k, _v;
			int _type;
		};

	public:
		std::vector<t_kv> _kvs;
	public:
		json(const json&) = delete;
		json& operator = (const json&) = delete;
		json()
		{
			_kvs.reserve(128);
		}
		~json()
		{
		}
		inline size_t size()
		{
			return _kvs.size();
		}

		const t_kv* at(size_t i)
		{
			if (i < _kvs.size())
				return &_kvs[i];
			return nullptr;
		}
		const t_kv* getkv(const char* key)
		{
			for (const auto &i : _kvs) {
				if (i._k.ieq(key))
					return &i;
			}
			return nullptr;
		}

		const t_kv& operator [](size_t pos)
		{
			return _kvs[pos];
		}

		const txt* getval(const char *key)
		{
			for (const auto &i : _kvs) {
				if (i._k.ieq(key))
					return &i._v;
			}
			return nullptr;
		}

		const txt* getval(const txt *key)
		{
			for (const auto &i : _kvs) {
				if (i._k.ieq(key))
					return &i._v;
			}
			return nullptr;
		}

		const txt* getval(const txt &key)
		{
			for (const auto &i : _kvs) {
				if (i._k.ieq(key))
					return &i._v;
			}
			return nullptr;
		}

		bool getvalstr(const char* skey, std::string &sout)
		{
			const txt* pv = getval(skey);
			if (!pv || pv->empty()) {
				sout.clear();
				return false;
			}
			sout.assign(pv->_str, pv->_size);
			return true;
		}

		bool from_str(txt &s)
		{
			_kvs.clear();
			if (s.empty())
				return false;
			if (!s.skip())
				return false;
			if (*s._str == '[')
				return from_array(s);
			else if (*s._str == '{')
				return from_obj(s);
			return false;
		}

		inline bool from_str(const char* s, size_t size)
		{
			txt t(s, size);
			return from_str(t);
		}

		static bool load_file(const char *sfile, std::string &sout)
		{
			if (!sfile)
				return false;
			FILE *pf = fopen(sfile, "rt");
			if (!pf)
				return false;
			int c = fgetc(pf), c2 = fgetc(pf), c3 = fgetc(pf);
			if (!(c == 0xef && c2 == 0xbb && c3 == 0xbf)) // not utf8 with bom
				fseek(pf, 0, SEEK_SET);
			char s[1024 * 8];

			sout.reserve(1024 * 16);
			size_t sz;
			while ((sz = fread(s, 1, sizeof(s), pf)) > 0)
				sout.append(s, sz);
			fclose(pf);
			return true;
		}

		static void del_comment(const char* pin, size_t inlen, std::string &sout)
		{
			size_t n = inlen;
			const char* s = pin, *sp = s;
			while (n) {
				if (*s == '/') {
					if (s != pin && *(s - 1) == '*' && !sp)  // */
						sp = s + 1;
				}
				else if (*s == '*') {
					if (s != pin && *(s - 1) == '/') { // /*
						if (sp && s > sp + 1)
							sout.append(sp, s - sp - 1);
						sp = nullptr;
					}
				}
				s++;
				n--;
			}
			if (sp && s > sp + 1)
				sout.append(sp, s - sp);
		}
	private:
		bool from_obj(txt &s)
		{
			t_kv  it;
			if (*s._str != '{')
				return false;
			s.tonext();
			while (!s.empty()) {
				if (!s.skip()) // to key start
					return false;
				if (*s._str == ',') {
					s.tonext();
					continue;
				}
				else if (*s._str == '}')
					return true;
				if (*s._str != '"')
					return false;
				it._k.clear();
				it._v.clear();
				s.tonext();
				it._k = s; // key
				if (!s.json_tochar('"'))  //to key end
					return false;
				it._k._size = s._str - it._k._str;
				if (!it._k._size || it._k._size > MAXSIZE_JSONX_KEY) //check key
					return false;
				if (!s.json_tochar(':')) // move to valuse
					return false;
				s.tonext();
				if (!s.skip())
					return false;
				if (*s._str == '"') { //string
					s.tonext();
					it._type = jstring;
					it._v = s;
					if (!s.json_tochar('"'))
						return false;
					it._v._size = s._str - it._v._str;
					s.tonext();
				}
				else if (*s._str == '{') { // object
					it._v = s;
					it._type = jobj;
					s.tonext();
					if (!s.json_toend('{', '}'))
						return false;
					it._v._size = s._str - it._v._str;
				}
				else if (*s._str == '[') { // array
					it._v = s;
					it._type = jarray;
					s.tonext();
					if (!s.json_toend('[', ']'))
						return false;
					it._v._size = s._str - it._v._str;
				}
				else { //number
					it._v = s;
					it._type = jnumber;
					if (!s.json_tochar(",}"))
						return false;
					it._v._size = s._str - it._v._str;
				}
				_kvs.push_back(it);
			}
			return false;
		}
		bool from_array(txt &s)
		{
			t_kv  it;
			if (*s._str != '[')
				return false;
			s.tonext();
			while (!s.empty()) {
				if (!s.skip())
					return false;
				it._v.clear();
				if (*s._str == ']') // end
					return true;
				else if (*s._str == ',') {
					s.tonext();
					continue;
				}
				else if (*s._str == '"') { //string
					s.tonext();
					it._type = jstring;
					it._v = s;
					if (!s.json_tochar('"'))
						return false;
					it._v._size = s._str - it._v._str;
					s.tonext();
				}
				else if (*s._str == '{') { // object
					it._v = s;
					it._type = jobj;
					s.tonext();
					if (!s.json_toend('{', '}'))
						return false;
					it._v._size = s._str - it._v._str;
				}
				else if (*s._str == '[') { // array
					it._v = s;
					it._type = jarray;
					s.tonext();
					if (!s.json_toend('[', ']'))
						return false;
					it._v._size = s._str - it._v._str;
				}
				else { // number valuse
					it._v = s;
					it._type = jnumber;
					if (!s.json_tochar(",]"))
						return false;
					it._v._size = s._str - it._v._str;
				}
				if (!it._v.empty())
					_kvs.push_back(it);
			}
			return false;
		}
	};//json
}// ec