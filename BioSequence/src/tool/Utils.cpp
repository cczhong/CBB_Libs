#include "Utils.h"


// template<typename _val> _val set_critical(_val v)
// {
// 	return static_cast<unsigned>(v) | 0x80;
// }

// template<typename _val> _val mask_critical(_val v)
// {
// 	return static_cast<unsigned>(v) & 0x7F;
// }

// template<typename _val> bool get_critical(_val v)
// {
// 	return (static_cast<unsigned>(v) & 0x80) != 0;
// }

unsigned filter_treshold(unsigned n)
{
	return VATOptions::hit_cap * 256 / n;
}

template<typename _loc>
bool position_filter(_loc l, unsigned treshold, seed s)
{
	return ((l ^ s) & 0xff) < treshold;
}

void print(const __m128i &x)
{
	char *p=(char*)&x;
	for(unsigned i=0;i<16;++i)
		std::cout << int(*(p++)) << ' ';
	std::cout << std::endl;
}

template<typename _it, typename _key>
vector<size_t> map_partition(_it begin, _it end, const _key& key, size_t min_size, size_t max_segments, size_t min_segments)
{
	const size_t n = end - begin;
	const ::partition p (n, std::max(min_segments, std::min(max_segments, n/min_size)));
	vector<size_t> v (p.parts+1);
	v[0] = p.getMin(0);
	v[p.parts] = p.getMax(p.parts-1);
	for(unsigned i=0;i<p.parts-1;++i) {
		size_t e = p.getMax(i);
		if(v[i] >= e) {
			v[i+1] = v[i];
			continue;
		}
		while(e < n && key(*(begin+e)) == key(*(begin+e-1)))
			++e;
		v[i+1] = e;
	}
	return v;
}

template<typename _t>
_t div_up(_t x, _t m)
{ return (x + (m-1)) / m; }

template<typename _t>
_t round_up(_t x, _t m)
{ 
	return div_up(x, m) * m; 
}

template<typename _val, typename _dir>
void print_seq(const _val* s, const _dir& d)
{
	unsigned i=0;
	while(get_dir(s,i,d) != 0xff) 
	{
		std::cout << mask_critical(get_dir(s,i,d));
		++i;
	}
}

vector<string> tokenize(const char *str, const char *delimiters)
{
	vector<string> out;
	while(*str != 0) {
		while(*str != 0 && strchr(delimiters, *str))
			++str;
		string token;
		while(*str != 0 && strchr(delimiters, *str) == 0)
			token += *(str++);
		if(token.length() > 0)
			out.push_back(token);
	}
	if(out.size() == 0)
		out.push_back(string ());
	return out;
}


size_t find_first_of(const char *s, const char *delimiters)
{
	const char *t = s;
	while(*t && strchr(delimiters, *t) == 0)
		++t;
	return t-s;
}

size_t print_str(char* buf, const char *s, size_t n)
{
	memcpy(buf, s, n);
	*(buf+n) = 0;
	return n;
}

size_t print_str(char *buf, const char *s, const char *delimiters)
{ return print_str(buf, s, find_first_of(s, delimiters)); }

string* GetStr(const char *s, const char *delimiters)
{
	return new string (s, find_first_of(s, delimiters));
}

__m128i _mm_set(int a)
{
	int y = a << 8 | a;
	__m128i z;
	z = _mm_insert_epi16 (z, y, 0);
	z = _mm_insert_epi16 (z, y, 1);
	z = _mm_insert_epi16 (z, y, 2);
	z = _mm_insert_epi16 (z, y, 3);
	z = _mm_insert_epi16 (z, y, 4);
	z = _mm_insert_epi16 (z, y, 5);
	z = _mm_insert_epi16 (z, y, 6);
	z = _mm_insert_epi16 (z, y, 7);
	return z;
}

//add extension; str= para1 + para2
void AppendExtension(string &str, const char *ext)
{
	size_t l = strlen(ext);
	if(str.length() < l || (str.length() >= l && str.substr(str.length()-l, string::npos) != ext))
		str += ext;
}

bool CheckDir(const string &path)
{
	struct stat sb;
	return stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode);
}

string toString(unsigned val)
{
	std::stringstream ss;
	ss << val;
	return ss.str();
}

