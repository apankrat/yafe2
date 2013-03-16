#ifndef _YAFE2_TYPES_H_
#define _YAFE2_TYPES_H_

/*
 *	import NULL, va_list, va_start and va_end, size_t
 */
#include <stddef.h>
#include <stdarg.h>

/*
 *	size_t synonim for array indices
 */
typedef size_t pos_t;

/*
 *	import uint8_t, uint16_t, uint32_t, uint64_t,
 *	portable printf format specifiers (PRIu32, etc)
 */
#include <stdint.h>

/*
 *	synonims for unsigned
 */
//typedef unsigned char  uchar_t;
//typedef unsigned long  ushort_t;
//typedef unsigned long  ulong_t;
typedef unsigned int   uint_t;
 
/*
 *	missing necessities
 */
#define sizeof_array(arr)  (sizeof(arr)/sizeof(0[arr]))

/*
 *	import stl stuff
 */
//#include <set>
#include <map>
//#include <list>
//#include <deque>
//#include <stack>
#include <string>
#include <vector>
#include <algorithm>

using std::vector;
//using std::set;
//using std::multiset;
using std::map;
//using std::multimap;
//using std::deque;
//using std::list;
//using std::stack;
//using std::pair;
using std::string;

//using std::sort;
using std::swap;

/*
 *	useful macros
 *
 *	__interface  - no explicit instantiation or destruction,
 *	               no copying, empty destructor
 *
 *	__restricted - no explicit instantiation or destruction,
 *	               no copying, non-empty destructor
 *
 *	__no_copying - no copying
 */
#define __interface(type)                         \
	                                          \
	protected:                                \
		type() { }                        \
		type(const type &);               \
		void operator = (const type &);   \
		virtual ~type() { }               \
	public:


#define __restricted(type)                        \
	                                          \
	protected:                                \
		type() { }                        \
		type(const type &);               \
		void operator = (const type &);   \
		virtual ~type();                  \
	public:

#define __no_copying(type)                        \
	                                          \
	protected:                                \
		type(const type &);               \
		type & operator = (const type &); \
	public:

#endif
