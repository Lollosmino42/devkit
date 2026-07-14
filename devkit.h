#ifndef _DEVKIT_H // _DEVKIT_H
#define _DEVKIT_H

#define DEVKIT_DEV
#ifdef DEVKIT_DEV // For development purposes
#define DEVKIT_IMPLEMENTATION
//#define DEVKIT_NO_FOREACH
//#define DEVKIT_STRIP_PREFIXES
#define DEVKIT_DEBUG
#define DEVKIT_MATH
#define DEVKIT_MATH_IMPLEMENTATION
//#define DEVKIT_INTERFACING
//#define DEVKIT_STATIC

#endif

/* 
 * ##########
 * # DEVKIT #
 * ##########
 *
 * Devkit optional flags:
 *
 * DEVKIT_DEBUG to enable additional safety checks (assertions) in functions
 * DEVKIT_STRIP_PREFIXES to strip 'devkit' prefix from functions and structs
 * DEVKIT_MATH to include math declarations and functions
 * DEVKIT_NO_FOREACH to disable 'enhanced for' implementation
 * DEVKIT_INTERFACING to disable features that wouldn't work when binding 
 *		this header to another language 
 * DEVKIT_STATIC to make everything statically linked, to hide the implementation from
 *		other source files
 * DEVKIT_ENABLE_EXTRA_ITERABLES (Read below for info)
 *
 * -----------------------------------------------------------------
 *
 * Devkit implementation flags:
 *
 * DEVKIT_STRING_IMPLEMENTATION
 * DEVKIT_LIST_IMPLEMENTATION
 * DEVKIT_POINTERS_IMPLEMENTATION
 * DEVKIT_ARRAY_IMPLEMENTATION
 * DEVKIT_IMPLEMENTATION (defines all implementations above)
 *
 * DEVKIT_MATH_IMPLEMENTATION (defines DEVKIT_MATH automatically)
 *
 */


#ifdef DEVKIT_ENABLE_EXTRA_ITERABLES

/*
 * ###################
 * # EXTRA ITERABLES #
 * ###################
 *
 *
 * Unlike typical iterables in "higher" level lanuages,
 * this iterable can be reused! (Every iteration the counter is set to 0)
 *
 * --------------------------------------------------------------
 *
 * To make a structure iterable, make a function as such:
 *
 * DktIterator <func_name>( <struct_t> *<struct>)
 *
 * Then add below:
 * <struct_t> : <func_name>,
 * <struct_t2> : <func_name2>,
 *	.
 *	.
 * <struct_tN> : <func_nameN>
 *
 * Use commas for multiple entries. Last entry must not have a comma
 *
 */

// vvvv ADD YOUR ITERABLES HERE vvvv
#define _DEVKIT_EXTRA_ITERABLES			\



#define _DEVKIT_COMMA ,
#else
#define _DEVKIT_EXTRA_ITERABLES
#define _DEVKIT_COMMA
#endif

#define _DEVKIT_ITERABLES					\
	_DEVKIT_EXTRA_ITERABLES	_DEVKIT_COMMA

#if !defined(__cplusplus) && defined(__STDC__) && __STDC_VERSION__ < 202311
#define nullptr NULL
#include <stdbool.h>
#endif

#ifndef __cplusplus
#define __restrict restrict
#endif




/*
 * Anything below here should not be touched
 */



#ifdef __cplusplus
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <alloca.h>
#else
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <alloca.h>
#endif

#ifdef DEVKIT_STATIC
#define _DEVKIT_STATIC static
#define DEVKIT_IMPLEMENTATION
#else
#define _DEVKIT_STATIC
#endif

#ifdef DEVKIT_INTERFACING
#define DEVKIT_NO_FOREACH
#endif

#ifdef DEVKIT_IMPLEMENTATION

#define DEVKIT_SET_IMPLEMENTATION
#define DEVKIT_HEAP_IMPLEMENTATION
#define DEVKIT_ARRAY_IMPLEMENTATION
#define DEVKIT_STRING_IMPLEMENTATION
#define DEVKIT_POINTERS_IMPLEMENTATION

#endif
#ifdef DEVKIT_MATH_IMPLEMENTATION
#define DEVKIT_MATH
#endif


/* Something useful i guess */

typedef __compar_fn_t DktComparator;
#ifdef DEVKIT_STRIP_PREFIXES
typedef DktComparator Comparator;
#endif






/*
 * #########
 * # FILES #
 * #########
 *
 * Quality macros that stdio.h should have 
 * to define file opening modes so you don't have
 * to memorize the string values 
 *
 * P.S. Now that i think about it, this has to be memorized like
 * the string values, so it's almost pointless...
 */

#define F_READ	         "r"
#define F_WRITE         "w"
#define F_APPEND        "a"
#define F_READ_WRITE    "r+"
#define F_WRITE_READ    "w+"
#define F_APPEND_READ   "a+"
#define F_READ_B        "rb"
#define F_WRITE_B       "wb"
#define F_APPEND_B      "ab"
#define F_READ_WRITE_B  "rb+"
#define F_WRITE_READ_B  "wb+"
#define F_APPEND_READ_B "ab+"







/*
 * ############
 * # ITERATOR #
 * ############
 */

typedef unsigned int DktSize;

#define DKT_VIEW_TEMPLATE	  \
	char *items;			     \
	DktSize typesize, length;

#ifndef DEVKIT_NO_FOREACH

/* 
 * Definition.
 * Used for 'foreach' loops 
 */

#define DKT_ITERATOR_TEMPLATE \
	void (*advance)  (struct DktIterator *); \
	char *ptr, *end_ptr;

// Iterator interface
typedef struct DktIterator {
	DKT_ITERATOR_TEMPLATE;
} DktIterator;

typedef void * DktIteratorType;

#endif








/*
 * ########
 * # VIEW #
 * ########
 *
 * "Template" type for most collections in this library (except String)
 */

typedef unsigned int DktSize;

typedef struct DktViewIterator {
	DKT_ITERATOR_TEMPLATE;
	DktSize typesize;
} DktViewIterator;

struct DktView;


#ifdef __cplusplus
typedef DktView DktViewType;
#else
typedef void DktViewType;
typedef struct DktView DktView;
#endif


/* Take the full view of a view_compatible collection */
_DEVKIT_STATIC DktView dkt_view_all (const DktViewType *v);

/* Take a slice of a view_compatible collection */
_DEVKIT_STATIC DktView dkt_view_of  (const DktViewType *v, DktSize start, DktSize end);

/* Get and set values of a view */
_DEVKIT_STATIC void * dkt_view_at         (const DktViewType *v, DktSize index);
_DEVKIT_STATIC void   dkt_view_set_all    (DktViewType *__restrict v, const void *__restrict value);

_DEVKIT_STATIC bool   dkt_view_contains   (const DktViewType *__restrict v, const void *__restrict value);
_DEVKIT_STATIC void   dkt_view_sort       (DktViewType *__restrict v, DktComparator func);
_DEVKIT_STATIC void   dkt_view_copy       (DktViewType *__restrict dest, DktViewType *__restrict src);
_DEVKIT_STATIC void   dkt_view_copy_array (DktViewType *__restrict dest, DktSize nitems, const void *__restrict values);

#ifndef DEVKIT_NO_FOREACH
_DEVKIT_STATIC DktViewIterator dkt_view_iterator ( const DktViewType *);
_DEVKIT_STATIC void _dkt_view_advance (DktIterator *view);
#endif

struct DktView {
	DKT_VIEW_TEMPLATE;

#ifdef __cplusplus

	DktView (char *items, DktSize typesize, DktSize length);
	DktView (const DktView &)              = default;
	DktView (DktView &&)                   = default;
	DktView & operator = (const DktView &) = default;
	DktView & operator = (DktView &&)      = default;

	inline DktView view(DktSize start, DktSize end) const;
	inline DktView view_all() const;


	template <typename ItemType>
	ItemType & at(DktSize i) const                              { return ((ItemType*)this->items) [i]; }
	template <typename ItemType>
	inline void set_all (const ItemType value)                  { dkt_view_set_all(this, &value); }
	template <typename ItemType>
	inline bool contains (const ItemType value)                 { return dkt_view_contains(this, &value); }
	inline void sort (DktComparator func)                       { dkt_view_sort(this, func); }
	inline void copy (DktView src)                              { dkt_view_copy(this, &src); }
	inline void copy_array (DktSize nitems, const void *values) { dkt_view_copy_array(this, nitems, values); }

	inline DktViewIterator iterator() const                     { return dkt_view_iterator(this); }

#endif
};


#ifdef DEVKIT_STRIP_PREFIXES

typedef DktView View;

#ifdef __cplusplus

constexpr auto view_all        = dkt_view_all;
constexpr auto view_of         = dkt_view_of;
constexpr auto view_at         = dkt_view_at;
constexpr auto view_set_all    = dkt_view_set_all;
constexpr auto view_contains	 = dkt_view_contains;
constexpr auto view_sort       = dkt_view_sort;
constexpr auto view_copy       = dkt_view_copy;
constexpr auto view_copy_array = dkt_view_copy_array;

#else

#define view_all		dkt_view_all
#define view_of			dkt_view_of
#define view_at		dkt_view_at
#define view_set_all	dkt_view_set_all
#define view_contains	dkt_view_contains
#define view_sort		dkt_view_sort
#define view_copy		dkt_view_copy
#define view_copy_array	dkt_view_copy_array

#endif

#endif








/*
 * ##########
 * # STRING #
 * ##########
 */

typedef struct DktString DktString;


typedef const DktString DktStringView;

/* Declarations */

_DEVKIT_STATIC DktString dkt_str_new     (const char *text);
_DEVKIT_STATIC DktString dkt_str_buffer  (DktSize init_size);
_DEVKIT_STATIC DktString dkt_str_copy    (DktString);
_DEVKIT_STATIC DktString dkt_str_release (DktString *s);
_DEVKIT_STATIC void      dkt_str_free    (DktString *);

_DEVKIT_STATIC DktStringView dkt_str_view_all (DktStringView s);
_DEVKIT_STATIC DktStringView dkt_str_view     (DktStringView s, DktSize start, DktSize end);
_DEVKIT_STATIC char* dkt_str_at (DktStringView s, DktSize index);

_DEVKIT_STATIC const char *dkt_str_cstr (DktString s);

_DEVKIT_STATIC void dkt_str_init (DktString *s, const char *text);

_DEVKIT_STATIC void dkt_str_append  (DktString *, DktStringView s);
_DEVKIT_STATIC void dkt_str_cappend	(DktString *, const char *s);
_DEVKIT_STATIC void dkt_str_reverse (DktStringView s);

_DEVKIT_STATIC int dkt_str_cmp (DktStringView a, DktStringView b);

_DEVKIT_STATIC void dkt_str_clear (DktString *);

_DEVKIT_STATIC void _dkt_str_realloc (DktString *, DktSize new_capacity);



#ifndef DEVKIT_NO_FOREACH
_DEVKIT_STATIC DktIterator	_dkt_str_iterator	(DktStringView *);
_DEVKIT_STATIC void _dkt_str_advance (DktIterator *);
#endif

struct DktString {
	char *items;
	DktSize length, capacity;

#ifdef __cplusplus
	using DktStringView = const DktString;

	DktString (char *items, DktSize length, DktSize capacity);
	DktString (const char *text);

	inline DktString copy() const                  { return dkt_str_copy(*this); }
	inline DktString release()                     { return dkt_str_release(this); }
	inline void clear()                            { dkt_str_clear(this); }
	inline const char *cstr()                      { return this->items; }

	inline void append(DktStringView v)   { dkt_str_append(this, v); }
	inline void cappend(const char *cstr) { dkt_str_cappend(this, cstr); }

	inline int cmp (DktStringView &other) const                  { return dkt_str_cmp(*this, other); }
	inline DktStringView view (DktSize start, DktSize end) const { return dkt_str_view(*this, start, end); }
	inline DktStringView view_all () const                       { return dkt_str_view_all(*this); }
	inline void reverse() const                                  { dkt_str_reverse(*this); };

	inline DktIterator iterator() const { return _dkt_str_iterator(this); }

#endif
};


#ifdef DEVKIT_STRIP_PREFIXES

typedef DktString String;
typedef DktStringView StringView;

#define str_new			dkt_str_new

#ifdef __cplusplus

constexpr auto str_buffer		= dkt_str_buffer;
constexpr auto str_init		   = dkt_str_init;
constexpr auto str_append		= dkt_str_append;
constexpr auto str_cappend		= dkt_str_cappend;
constexpr auto str_copy		   = dkt_str_copy;
constexpr auto str_at         = dkt_str_at;
constexpr auto str_view		   = dkt_str_view;
constexpr auto str_view_all   = dkt_str_view;
constexpr auto str_reverse 	= dkt_str_reverse;
constexpr auto str_release    = dkt_str_release;
constexpr auto str_free		   = dkt_str_free;
constexpr auto str_cstr		   = dkt_str_cstr;
constexpr auto str_cmp        = dkt_str_cmp;
constexpr auto str_clear      = dkt_str_clear;

#else

#define str_buffer      dkt_str_buffer
#define str_init		   dkt_str_init
#define str_append		dkt_str_append
#define str_cappend		dkt_str_cappend
#define str_copy		   dkt_str_copy
#define str_at          dkt_str_at
#define str_view		   dkt_str_view
#define str_view_all    dkt_str_view
#define str_reverse 	   dkt_str_reverse
#define str_release 	   dkt_str_release
#define str_free		   dkt_str_free
#define str_cstr		   dkt_str_cstr
#define str_cmp			dkt_str_cmp
#define str_clear			dkt_str_clear

#endif

#endif








/*
 * #########
 * # ARRAY #
 * #########
 */

struct DktArray;
typedef struct DktArray DktArray;

#define DKT_ARR_SIZE(T, capacity) (sizeof(DktArray) + sizeof(T)*(capacity))



/* Allocates a new DktArray on the heap */
_DEVKIT_STATIC DktArray _dkt_arr_new		(DktSize typesize, DktSize length);
#define dkt_arr_new( type, length) _dkt_arr_new( sizeof(type), (length))

_DEVKIT_STATIC void dkt_arr_init			(DktArray *a, DktSize typesize, DktSize length);
_DEVKIT_STATIC void dkt_arr_init_values	(DktArray *a, const void *const values);

_DEVKIT_STATIC void *dkt_arr_at (const DktArray a, DktSize index);

_DEVKIT_STATIC DktView  dkt_arr_view      (const DktArray a);
_DEVKIT_STATIC DktArray dkt_arr_from_view	(const DktViewType *a);

/* Concatenates 'array' and 'other' and copies the buffer into 'dest'.
 * NOTE: arrays must be of same type */
_DEVKIT_STATIC DktArray dkt_arr_concat	(const DktArray a, const DktArray other);

_DEVKIT_STATIC DktArray dkt_arr_release (const DktArray *a);

/* Deallocates item buffer of 'array' if allocated on heap using the standard library,
 * sets all array values to 0 */
_DEVKIT_STATIC void dkt_arr_free			(const DktArray *a);


#ifndef __cplusplus
typedef struct DktArray {
	DKT_VIEW_TEMPLATE;
} DktArray;
#else
struct DktArray : DktView {

	DktArray (char *items, size_t typesize, DktSize length);
	DktArray (const DktView &v);

	inline DktArray release() const                            { return dkt_arr_release((DktArray*) this); }
	inline DktArray concat (const DktArray &v) const           { return dkt_arr_concat(*this, v); }
};
#endif


#ifdef DEVKIT_STRIP_PREFIXES

typedef DktArray Array;

#define ARR_SIZE DKT_ARR_SIZE

#define arr_new		dkt_arr_new

#ifdef __cplusplus
constexpr auto arr_init        = dkt_arr_init;
constexpr auto arr_init_values = dkt_arr_init_values;
constexpr auto arr_view        = dkt_arr_view;
constexpr auto arr_from_view   = dkt_arr_from_view;
constexpr auto arr_concat      = dkt_arr_concat;
constexpr auto arr_free        = dkt_arr_free;
constexpr auto arr_release     = dkt_arr_release;
constexpr auto arr_at          = dkt_arr_at;

#else
#define arr_init		dkt_arr_init
#define arr_init_values	dkt_arr_init_values
#define arr_view        dkt_arr_view
#define arr_from_view dkt_arr_from_view
#define arr_at        dkt_arr_at
#define arr_concat  dkt_arr_concat
#define arr_free    dkt_arr_free
#define arr_release dkt_arr_release
#endif

#endif








/*
 * ########
 * # LIST #
 * ########
 */

/* An approach to variable length arrays in C. */
struct DktList;
typedef struct DktList DktList;


#define DKT_LIST_SIZE(T, capacity) (sizeof(DktList) + sizeof(T)*(capacity))


/* Creates a new list */
_DEVKIT_STATIC DktList _dkt_list_new	(const DktSize typesize, const DktSize capacity);
#define dkt_list_new( type, capacity) _dkt_list_new( sizeof(type), (capacity))

/* Makes a copy of this list/slice */
_DEVKIT_STATIC DktList dkt_list_from_view	(const DktViewType *);
_DEVKIT_STATIC DktList dkt_list_copy      (const DktList);

/* List destructor: calls 'free' on items, struct is set to 0 */
_DEVKIT_STATIC void dkt_list_free	(const DktList *);

/* Add 'nitems' items from 'values' to 'list' */
_DEVKIT_STATIC void dkt_list_nadd	(DktList *__restrict l, DktSize nitems, const void *const values);
#define dkt_list_add( l, var) dkt_list_nadd( (l), 1, (var))

/* Insert 'nitems' of 'values' in 'list' at 'index' */
_DEVKIT_STATIC void dkt_list_ninsert	(DktList *__restrict l, DktSize index, DktSize nitems, const void *const values);
#define dkt_list_insert( l, index, var) dkt_list_ninsert( (l), (index), 1, (var))

/* Remove item at 'index' from 'list' and copy it to 'dest'.
 * If 'dest' is null, the value isn't copied */
_DEVKIT_STATIC void dkt_list_remove	(void *const dest, DktList *l, DktSize index);
_DEVKIT_STATIC void dkt_list_pop	   (void *const dest, DktList *l);

/* Remove 'nitems' items at 'indices' in 'list', copying them into 'dest' if not null */
_DEVKIT_STATIC void dkt_list_nremove	(void *__restrict dest, DktList *l, DktSize nitems, const DktSize *__restrict indices);

/* Add the items of 'src' to 'dest'. Lists must have same item type.
 * If concatenation is successful, returns true */
_DEVKIT_STATIC bool dkt_list_concat  (DktList *__restrict dest, const DktViewType *__restrict src);

/* Allocate more space for 'list' to increase its capacity to 'new_capacity' */
_DEVKIT_STATIC void dkt_list_expand  (DktList *l, DktSize new_capacity);

/* Reduce list capacity to its length to free unneeded memory */
_DEVKIT_STATIC void dkt_list_trim    (DktList *l);

_DEVKIT_STATIC DktList dkt_list_release (const DktList *l);


#ifndef __cplusplus
typedef struct DktList {
	DKT_VIEW_TEMPLATE;
	DktSize capacity;
} DktList;
#else
struct DktList : DktView {
	DktSize capacity;

	inline DktList release() const { return dkt_list_release((DktList*) this); }

	DktList (char *items, size_t typesize, DktSize length, DktSize capacity);

	inline DktList copy () const                                   { return dkt_list_copy(*this); }
	template <typename ItemType>
	inline void add     (const ItemType & value)                   { dkt_list_add(this, &value); }
	template <typename ItemType>
	inline void nadd    (DktSize n, const ItemType values[])       { dkt_list_nadd(this, n, values); }
	inline void remove  (void *dest, DktSize index)                { dkt_list_remove(dest, this, index); }
	inline void nremove (void *dest, DktSize n, DktSize indices[]) { dkt_list_nremove(dest, this, n, indices); }
	inline void pop     (void *dest)                               { dkt_list_pop(dest, this); }
	inline bool concat  (const DktView view)                       { return dkt_list_concat(this, &view); }
	inline void expand  (size_t new_capacity)                      { dkt_list_expand(this, new_capacity); }
	inline void trim    ()                                         { dkt_list_trim(this); }
};
#endif


#ifdef DEVKIT_STRIP_PREFIXES

typedef DktList List;

#define LIST_SIZE DKT_LIST_SIZE

#define list_new	      dkt_list_new

#ifdef __cplusplus
constexpr auto list_from_view	 = dkt_list_from_view;
constexpr auto list_copy       = dkt_list_copy;
constexpr auto list_free       = dkt_list_free;
constexpr auto list_nadd       = dkt_list_nadd;
constexpr auto list_ninsert    = dkt_list_ninsert;
constexpr auto list_remove     = dkt_list_remove;
constexpr auto list_pop        = dkt_list_pop;
constexpr auto list_nremove    = dkt_list_nremove;
constexpr auto list_concat     = dkt_list_concat;
constexpr auto list_expand     = dkt_list_expand;
constexpr auto list_trim       = dkt_list_trim;
constexpr auto list_release    = dkt_list_release;

inline void list_add (DktList *__restrict l, const void *const values) { list_nadd(l, 1, values); }
inline void list_insert (DktList *__restrict l, DktSize index, const void *const values) { list_ninsert(l, index, 1, values); }

#else

#define list_from_view	dkt_list_from_view
#define list_copy	      dkt_list_copy
#define list_free	      dkt_list_free
#define list_add       dkt_list_add
#define list_nadd      dkt_list_nadd
#define list_insert    dkt_list_insert
#define list_ninsert   dkt_list_ninsert
#define list_remove    dkt_list_remove
#define list_pop       dkt_list_pop
#define list_nremove   dkt_list_nremove
#define list_concat    dkt_list_concat
#define list_expand    dkt_list_expand
#define list_trim      dkt_list_trim
#define list_release   dkt_list_release

#endif

#endif








/*
 * ############
 * # POINTERS #
 * ############
 */

#ifndef DEVKIT_NO_FOREACH

_DEVKIT_STATIC DktViewIterator _dkt_iterator (void* a, DktSize length, DktSize typesize);

#ifdef __cplusplus
template <typename T>
inline auto dkt_iterator(T *a, DktSize length) { return _dkt_iterator(a, length, sizeof(*a)); }
#else
#define dkt_iterator( arr, length) _dkt_iterator( (arr), (length), (DktSize) sizeof(*(arr)))
#endif

#endif

/* Unreferences to pointer after casting */
#define dkt_unref(type) *(type*)
/* Creates a stack array of elements of 'type' */
#define dkt_ref(type, ...) ((type[]){__VA_ARGS__})

_DEVKIT_STATIC DktView _dkt_as_view (void *a, size_t length, size_t typesize);
/* Returns an heap allocated array with 'steps' rational numbers from 'start' to 'end' */
_DEVKIT_STATIC void*	_dkt_linspace (double start, double end, size_t steps, bool isfloat);
/* Returns an heap allocated array with integer numbers from 'start' to 'end - 1' */
_DEVKIT_STATIC void*	_dkt_range (long start, long end, bool islong);
/* Convenience function to free multiple pointers at once */
_DEVKIT_STATIC void	_dkt_free_all	(void **ptrs);

#ifdef __cplusplus
template <typename T>
inline auto dkt_as_view (T *a, DktSize length) { return _dkt_as_view(a, length, sizeof(*a)); }
inline auto dkt_linspace (double start, double end, size_t steps) { return (double*)_dkt_linspace(start, end, steps, false); }
inline auto dkt_flinspace (double start, double end, size_t steps) { return (float*)_dkt_linspace(start, end, steps, true); }
inline auto dkt_range (long start, long end) { return (int*)_dkt_range(start, end, false); }
inline auto dkt_lrange (long start, long end) { return (long*)_dkt_range(start, end, true); }
inline auto dkt_free_all(void *ptrs[]...) { _dkt_free_all(ptrs); }

#else
#define dkt_as_view( arr, length)         _dkt_as_view( (arr), (length), sizeof(*(arr)))
#define dkt_linspace( start, end, steps)  (double*)_dkt_linspace( (start), (end), (steps), false)
#define dkt_flinspace( start, end, steps) (float*)_dkt_linspace( (start), (end), (steps), true)
#define dkt_range( start, end) ((int*)    (int*)_dkt_range( (start), (end), false))
#define dkt_lrange( start, end) ((long*)  (long*)_dkt_range( (start), (end), true))
#define dkt_free_all(...) _dkt_free_all( (void*[]) {__VA_ARGS__, nullptr})

#endif



#ifdef DEVKIT_STRIP_PREFIXES

#ifndef DEVKIT_NO_FOREACH
#ifdef __cplusplus
template <typename T> constexpr auto iterator = dkt_iterator<T>;
#else
#define iterator	dkt_iterator
#endif
#endif

#ifdef __cplusplus
template <typename T> 
constexpr auto as_view   = dkt_as_view<T>;
constexpr auto linspace  = dkt_linspace;
constexpr auto flinspace = dkt_flinspace;
constexpr auto range		 = dkt_range;
constexpr auto lrange    = dkt_lrange;
constexpr auto free_all	 = dkt_free_all;

#else
#define as_view	dkt_as_view
#define linspace	dkt_linspace
#define flinspace	dkt_flinspace
#define range		dkt_range
#define lrange		dkt_lrange
#define free_all	dkt_free_all

#endif

#define unref		dkt_unref
#define ref			dkt_ref

#endif







/*
 * ########
 * # HEAP #
 * ########
 */

#ifdef DEVKIT_HEAP_IMPLEMENTATION
#define DEVKIT_LIST_IMPLEMENTATION // Required by Heap to work (uses a list internally)
#endif

typedef DktViewIterator DktHeapIterator;
struct DktHeap;


#ifndef __cplusplus
typedef struct DktHeap DktHeap;
#endif

#define DKT_HEAP_INIT_CAPACITY 10

#define dkt_heap_new(type, cmp) (_dkt_heap_new(sizeof(type), (cmp)))
_DEVKIT_STATIC DktHeap     _dkt_heap_new     (const DktSize typesize, DktComparator cmp);
_DEVKIT_STATIC void        dkt_heap_init     (DktHeap *heap, const DktSize typesize, DktComparator cmp);

_DEVKIT_STATIC void        dkt_heap_push     (DktHeap *__restrict heap, const void *__restrict value);
_DEVKIT_STATIC const void*	dkt_heap_front    (const DktHeap heap);
_DEVKIT_STATIC void        dkt_heap_pop_front   (DktHeap *__restrict heap, void *__restrict dest);
_DEVKIT_STATIC DktSize     dkt_heap_length      (const DktHeap heap);
_DEVKIT_STATIC void        dkt_heap_free     (const DktHeap *heap);
_DEVKIT_STATIC void        dkt_heap_trim     (DktHeap *heap);

_DEVKIT_STATIC DktHeap     dkt_heap_release (DktHeap *heap);

_DEVKIT_STATIC DktSize     _dkt_heap_cleft     (DktSize i);
_DEVKIT_STATIC DktSize     _dkt_heap_cright    (DktSize i);
_DEVKIT_STATIC void        _dkt_heap_move_up   (DktHeap *heap, DktSize i);
_DEVKIT_STATIC void        _dkt_heap_move_down (DktHeap *heap, DktSize i);
_DEVKIT_STATIC DktSize     _dkt_heap_parent    (DktSize i);

#ifndef DEVKIT_NO_FOREACH
_DEVKIT_STATIC DktHeapIterator _dkt_heap_iterator (DktHeap *);
#endif


struct DktHeap {
	DktList inner;
	DktComparator cmp;

#ifdef __cplusplus

	DktHeap (DktList &&inner, DktComparator cmp);

	inline DktHeap release()                          { return dkt_heap_release(this); }

	template <typename ItemType>
	inline void push(ItemType &value)                 { dkt_heap_push(this, &value); }
	template <typename ItemType>
	inline const ItemType &front() const              { return *(ItemType*) dkt_heap_front(*this); }
	inline void   pop_front (void *dest)              { dkt_heap_pop_front(this, dest); }
	inline size_t length ()                           { return inner.length; }
	inline void   trim   ()                           { dkt_heap_trim(this); }

	inline DktHeapIterator iterator()                 { return _dkt_heap_iterator(this); }

#endif
};

#ifdef DEVKIT_STRIP_PREFIXES

typedef DktHeap Heap;

#define heap_new	dkt_heap_new	

#ifdef __cplusplus
constexpr auto heap_init	   = dkt_heap_init;
constexpr auto heap_push	   = dkt_heap_push;
constexpr auto heap_front	= dkt_heap_front;
constexpr auto heap_length	= dkt_heap_length;
constexpr auto heap_free	   = dkt_heap_free;
constexpr auto heap_trim	   = dkt_heap_trim;
constexpr auto heap_release = dkt_heap_release;

#else
#define heap_init	dkt_heap_init	

#define heap_push	dkt_heap_push	
#define heap_front	dkt_heap_front	
#define heap_length	dkt_heap_length	
#define heap_free	dkt_heap_free	
#define heap_trim	dkt_heap_trim	
#define heap_release dkt_heap_release
#endif

#endif







/*
 * #######
 * # SET #
 * #######
 */


typedef size_t (*DktHash)(const void *, const size_t);

struct _dkt_set_bucket { unsigned char length; };

typedef struct {
	DKT_ITERATOR_TEMPLATE;
	struct _dkt_set_bucket *bucket;
	DktSize typesize;
} DktSetIterator;

struct DktSet;
typedef struct DktSet DktSet;


_DEVKIT_STATIC size_t dkt_standard_hash(const void *ptr, const size_t size);

#define dkt_set_new(type, hash_func) _dkt_set_new(sizeof(type), (hash_func))
_DEVKIT_STATIC DktSet _dkt_set_new     (DktSize typesize, DktHash hash);
_DEVKIT_STATIC bool   dkt_set_add      (DktSet *__restrict set, const void *__restrict value);
_DEVKIT_STATIC bool   dkt_set_remove   (DktSet set, const void *value);
_DEVKIT_STATIC bool   dkt_set_contains (const DktSet set, const void *value);
_DEVKIT_STATIC size_t dkt_set_size     (const DktSet *set);
_DEVKIT_STATIC void   dkt_set_free     (const DktSet *set);
_DEVKIT_STATIC DktSet dkt_set_release  (const DktSet *set);
_DEVKIT_STATIC void   _dkt_set_grow    (DktSet *set);

#ifndef DEVKIT_NO_FOREACH

_DEVKIT_STATIC void _dkt_set_advance (DktIterator *);
_DEVKIT_STATIC DktSetIterator _dkt_set_iterator (DktSet *);

#endif


struct DktSet {
	struct _dkt_set_bucket *items;
	DktSize nbuckets;
	DktSize typesize;
	DktHash hash;

#ifdef __cplusplus
	DktSet (_dkt_set_bucket *, DktSize, DktSize, DktHash);

	inline DktSet release() const                       { return dkt_set_release(this); }

	template <typename ItemType>
	inline bool add (const ItemType &value)             { return dkt_set_add(this, &value); }
	template <typename ItemType>
	inline bool remove (const ItemType &value)          { return dkt_set_remove(*this, &value); }
	template <typename ItemType>
	inline bool contains (const ItemType &value) const  { return dkt_set_contains(this, &value); }
	inline auto size () const                           { return dkt_set_size(this); }

	inline DktSetIterator iterator()                    { return _dkt_set_iterator(this); }

#endif
};


#ifdef DEVKIT_STRIP_PREFIXES

typedef DktSet Set;

#define set_new dkt_set_new

#ifdef __cplusplus
constexpr auto set_add      = dkt_set_add;
constexpr auto set_remove   = dkt_set_remove;
constexpr auto set_contains = dkt_set_contains;
constexpr auto set_size     = dkt_set_size;
constexpr auto set_free     = dkt_set_free;
constexpr auto set_release  = dkt_set_release;

#else
#define set_add dkt_set_add
#define set_remove dkt_set_remove
#define set_contains dkt_set_contains
#define set_size dkt_set_size
#define set_free dkt_set_free
#define set_release dkt_set_release

#endif

#endif





/*
 * ########
 * # MATH #
 * ########
 */

#ifdef DEVKIT_MATH // Math module flag condition

#ifdef __cplusplus
#include <cmath>
#else
#include <math.h>
#endif

#ifdef DEVKIT_MATH_USE_DOUBLE
typedef double DktFloat;
#else
typedef float DktFloat;
#endif


/*
 * Stack allocated (const) types
 */

typedef const struct {
	DktFloat x, y;
} DktVec2;

typedef const struct {
	DktFloat x, y, z;
} DktVec3;


/*
 * Heap allocated types
 */
struct DktVector;
struct DktMatrix;

typedef const struct DktVector DktVectorView;
typedef const struct DktMatrix DktMatrixView;
typedef struct DktVector DktVector;
typedef struct DktMatrix DktMatrix;

typedef DktIterator DktVectorIterator;


#ifndef DEVKIT_NO_FOREACH

_DEVKIT_STATIC void _dkt_vec_advance (DktVectorIterator *);
_DEVKIT_STATIC DktVectorIterator _dkt_vec_iterator (DktVector *);
_DEVKIT_STATIC DktVectorIterator _dkt_mat_iterator (DktMatrix *);

#define _DEVKIT_MATH_ITERABLES \
	DktVector:	_dkt_vec_iterator, \
	DktMatrix:	_dkt_mat_iterator,
#endif


/*
 * NOTE:
 * While Vec2 and Vec3 are const and their functions make new structs,
 * heap structs modify the existing structs to avoid numerous heap allocations
 * that would be hard to keep track of.
 */

// VEC2
_DEVKIT_STATIC DktVec2		dkt_vec2_new	(DktFloat x, DktFloat y);
_DEVKIT_STATIC DktVec2		dkt_vec2_sum	(DktVec2 v, DktVec2 w);
_DEVKIT_STATIC DktVec2		dkt_vec2_sub	(DktVec2 v, DktVec2 w);
_DEVKIT_STATIC DktVec2		dkt_vec2_scale	(DktVec2 v, DktFloat scale);

/* Returns the module of this vector */
_DEVKIT_STATIC DktFloat		dkt_vec2_mod	(DktVec2 v);

// VEC3
_DEVKIT_STATIC DktVec3		dkt_vec3_new	(DktFloat x, DktFloat y, DktFloat z);
_DEVKIT_STATIC DktVec3 		dkt_vec3_sum	(DktVec3 v, DktVec3 w);
_DEVKIT_STATIC DktVec3 		dkt_vec3_sub	(DktVec3 v, DktVec3 w);
_DEVKIT_STATIC DktVec3 		dkt_vec3_scale	(DktVec3 v, DktFloat scale);

/* Returns the module of this vector */
_DEVKIT_STATIC DktFloat		dkt_vec3_mod	(DktVec3 v);

// VECTOR

/* Creates a new vector of zeros */
_DEVKIT_STATIC DktVector	dkt_vec_new	(const DktSize length);

/* Initializes allocation of vector with values.
 * Values must be of the same size as vector! */
_DEVKIT_STATIC void      dkt_vec_init        (DktVectorView, const DktSize length, DktFloat values[]);
_DEVKIT_STATIC void      dkt_vec_init_values (DktVectorView, DktFloat values[]);

/* Frees the vector data */
_DEVKIT_STATIC void      dkt_vec_free (DktVector *);

/* Creates a copy of the vector */
_DEVKIT_STATIC DktVector dkt_vec_copy (DktVectorView);

/* Access values of vector */
_DEVKIT_STATIC DktFloat*  dkt_vec_at     (DktVectorView, const DktSize index);

/* Checks whether two vectors are equal */
_DEVKIT_STATIC bool      dkt_vec_equals	(DktVectorView, DktVectorView other);

/* In place sum-subtraction to this vector. Other is unchanged */
_DEVKIT_STATIC void      dkt_vec_sum    (DktVectorView, DktVectorView other);
_DEVKIT_STATIC void      dkt_vec_sub    (DktVectorView, DktVectorView other);

/* In place scale function */
_DEVKIT_STATIC void      dkt_vec_scale  (DktVectorView, const DktFloat scalar);

/* Checks whether this vector is all zeros */
_DEVKIT_STATIC bool      dkt_vec_iszero (DktVectorView);

_DEVKIT_STATIC DktVector dkt_vec_release (DktVector *);

// MATRIX

/* Creates a ('rows'×'cols') matrix of zeros */
_DEVKIT_STATIC DktMatrix    dkt_mat_new       (DktSize rows, DktSize columns);
/* Initialises this matrix with 'values' */
_DEVKIT_STATIC void			dkt_mat_init		(DktMatrix *, const DktSize cols, const DktSize rows, DktFloat values[]);
_DEVKIT_STATIC void			dkt_mat_init_values	(DktMatrix m, DktFloat values[]);

/* Deallocates block of memory allocated for matrix data.
 * If this matrix struct was created on the heap, the use of this function
 * will segfault */
_DEVKIT_STATIC void			dkt_mat_free	(DktMatrix *);
/* Clones the matrix with its data */
_DEVKIT_STATIC DktMatrix	dkt_mat_copy	(const DktMatrix);

/* Access values of matrix at ('row', 'col') */
_DEVKIT_STATIC DktFloat* dkt_mat_at (DktMatrix, DktSize col, DktSize row);

/* Adds 'mat' to this matrix */
_DEVKIT_STATIC void dkt_mat_sum     (DktMatrix dest, DktMatrix other);

/* Subtracts 'mat' to this matrix */
_DEVKIT_STATIC void dkt_mat_sub     (DktMatrix dest, DktMatrix other);

/* Creates a new matrix from the matrix multiplication of A and B */
_DEVKIT_STATIC DktMatrix	dkt_mat_mul (DktMatrixView A, DktMatrixView B);

/* Transposes a matrix.
 * This function assumes the matrix is square-shaped. Be careful! */
_DEVKIT_STATIC void dkt_mat_transpose	(DktMatrix *);

/* Checks whether A and B are equal */
_DEVKIT_STATIC bool dkt_mat_equals	(DktMatrix A, DktMatrix B);

/* Checks whether the matrix is full of zeros */
_DEVKIT_STATIC bool dkt_mat_iszero	(DktMatrixView);

_DEVKIT_STATIC DktMatrix dkt_mat_release (DktMatrix *);

/* Matrix - Vector conversion functions */

// Creates a NEW matrix from vec2 or vec3 (copy semantics)
_DEVKIT_STATIC DktMatrix	dkt_vec2_asmat	(DktVec2);
_DEVKIT_STATIC DktMatrix	dkt_vec3_asmat	(DktVec3);
// Copy semantics also here
_DEVKIT_STATIC DktVec2		dkt_mat_tovec2	(const DktMatrix);
_DEVKIT_STATIC DktVec3 		dkt_mat_tovec3	(const DktMatrix);
_DEVKIT_STATIC DktVec2		dkt_vec_tovec2	(const DktVectorView);
_DEVKIT_STATIC DktVec3 		dkt_vec_tovec3	(const DktVectorView);
// These have REFERENCE semantics: data is not copied
_DEVKIT_STATIC DktVectorView	dkt_vec2_asvec	(DktVec2 v);
_DEVKIT_STATIC DktVectorView	dkt_vec3_asvec	(DktVec3 v);
_DEVKIT_STATIC DktMatrix	dkt_vec_asmat	(DktVectorView);
_DEVKIT_STATIC DktVectorView	dkt_mat_asvec	(DktMatrix);


struct DktVector {

	DktFloat *items;
	const DktSize length;

#ifdef __cplusplus

	DktVector(DktFloat *items, DktSize length);
	DktVector(DktSize);

	inline DktVector copy() const                        { return dkt_vec_copy(*this); }
	inline DktVector release()                           { return dkt_vec_release(this); }

	inline DktFloat & at (DktSize i) const               { return items[i]; }
	inline void sum    (DktVectorView v)          { dkt_vec_sum(*this, v); }
	inline void sub    (DktVectorView v)          { dkt_vec_sub(*this, v); }
	inline void scale  (const DktFloat scalar)    { dkt_vec_scale(*this, scalar); }
	inline bool equals (DktVectorView v) const    { return dkt_vec_equals(*this, v); }
	inline bool iszero () const                   { return dkt_vec_iszero(*this); }
#endif
};

#ifndef __cplusplus
typedef struct DktMatrix {
	DktFloat *items;
	const DktSize length;
	DktSize rows, columns;
} DktMatrix;
#else
typedef struct DktMatrix : public DktVector {
	DktSize rows, columns;

	DktMatrix (DktFloat *items, DktSize length, DktSize rows, DktSize cols);
	DktMatrix (DktSize rows, DktSize cols);

	DktMatrix copy()                                 { return dkt_mat_copy(*this); }

	DktFloat & at (DktSize row, DktSize col) const   { return items[columns*row + col]; }
	DktMatrix release()                              { return dkt_mat_release(this); }

	inline void transpose()                          { dkt_mat_transpose(this); }

} DktMatrix;
#endif

#ifdef DEVKIT_STRIP_PREFIXES

typedef DktVec2 Vec2;
typedef DktVec3 Vec3;
typedef DktVector Vector;
typedef DktMatrix Matrix;

#ifdef __cplusplus
constexpr auto vec2_new        = dkt_vec2_new;
constexpr auto vec3_new        = dkt_vec3_new;
constexpr auto vec_new         = dkt_vec_new;
constexpr auto mat_new         = dkt_mat_new;

constexpr auto vec2_sum        = dkt_vec2_sum;
constexpr auto vec2_sub        = dkt_vec2_sub;
constexpr auto vec2_scale      = dkt_vec2_scale;
constexpr auto vec2_mod        = dkt_vec2_mod;

constexpr auto vec3_sum        = dkt_vec3_sum;
constexpr auto vec3_sub        = dkt_vec3_sub;
constexpr auto vec3_scale	    = dkt_vec3_scale;
constexpr auto vec3_mod        = dkt_vec3_mod;

constexpr auto vec_init        = dkt_vec_init;
constexpr auto vec_init_values = dkt_vec_init_values;
constexpr auto vec_copy        = dkt_vec_copy;
constexpr auto vec_free        = dkt_vec_free;
constexpr auto vec_sum         = dkt_vec_sum;
constexpr auto vec_equals      = dkt_vec_equals;
constexpr auto vec_scale       = dkt_vec_scale;
constexpr auto vec_iszero      = dkt_vec_iszero;
constexpr auto vec_at          = dkt_vec_at;
constexpr auto vec_release     = dkt_vec_release;

constexpr auto mat_free        = dkt_mat_free;
constexpr auto mat_mul         = dkt_mat_mul;
constexpr auto mat_init        = dkt_mat_init;
constexpr auto mat_init_values = dkt_mat_init_values;
constexpr auto mat_at          = dkt_mat_at;
constexpr auto mat_equals      = dkt_mat_equals;
constexpr auto mat_transpose   = dkt_mat_transpose;
constexpr auto mat_sum         = dkt_mat_sum;
constexpr auto mat_iszero      = dkt_mat_iszero;
constexpr auto mat_release     = dkt_mat_release;

constexpr auto vec2_asmat      = dkt_vec2_asmat;
constexpr auto vec3_asmat      = dkt_vec3_asmat;
constexpr auto vec_asmat       = dkt_vec_asmat;
constexpr auto vec2_asvec      = dkt_vec2_asvec;
constexpr auto vec3_asvec      = dkt_vec3_asvec;
constexpr auto mat_asvec       = dkt_mat_asvec;
constexpr auto mat_tovec2      = dkt_mat_tovec2;
constexpr auto mat_tovec3      = dkt_mat_tovec3;

#else

#define vec2_sum         dkt_vec2_sum
#define vec2_sub         dkt_vec2_sub
#define vec2_neg         dkt_vec2_neg
#define vec2_scale       dkt_vec2_scale
#define vec2_mod         dkt_vec2_mod

#define vec3_sum         dkt_vec3_sum
#define vec3_sub         dkt_vec3_sub
#define vec3_neg         dkt_vec3_neg
#define vec3_scale	    dkt_vec3_scale
#define vec3_mod         dkt_vec3_mod

#define vec_init         dkt_vec_init
#define vec_init_values  dkt_vec_init_values
#define vec_copy         dkt_vec_copy
#define vec_free         dkt_vec_free
#define vec_sum          dkt_vec_sum
#define vec_equals       dkt_vec_equals
#define vec_scale        dkt_vec_scale
#define vec_iszero       dkt_vec_iszero
#define vec_at           dkt_vec_at
#define vec_release      dkt_vec_release

#define mat_free         dkt_mat_free
#define mat_getrow       dkt_mat_getrow 
#define mat_getcol       dkt_mat_getcol 
#define mat_mul          dkt_mat_mul
#define mat_init         dkt_mat_init
#define mat_init_values  dkt_mat_init_values
#define mat_at           dkt_mat_at
#define mat_equals       dkt_mat_equals
#define mat_transpose    dkt_mat_transpose
#define mat_sum          dkt_mat_sum
#define mat_iszero       dkt_mat_iszero
#define mat_release      dkt_mat_release

#define vec2_asmat       dkt_vec2_asmat
#define vec3_asmat       dkt_vec3_asmat
#define vec_asmat        dkt_vec_asmat 
#define vec2_asvec       dkt_vec2_asvec
#define vec3_asvec       dkt_vec3_asvec
#define mat_asvec        dkt_mat_asvec 
#define mat_tovec2       dkt_mat_tovec2
#define mat_tovec3       dkt_mat_tovec3

#endif
#endif
#endif


/* 
 * ###################################################################
 * # Loop pool implementation needed for nested 'enhanced for' loops #
 * ###################################################################
 */


/*
 * ################
 * # ENHANCED FOR #
 * ################
 *
 */

#ifndef DEVKIT_NO_FOREACH

/* Bypass for DktIterator in generic selection of _dkt_iterator_cast. With this,
 * arrays can be converted to iterables and used in foreach loops */
static inline DktIterator _dkt_dummy_iterator(DktIterator *iter) {
	return *iter;
}

/* "Cast" to _dkt_iterator_cast.
 * Works with Arrays, Lists, and other structures defined in devkit that have
 * a <...>_asiterable function.
 * Other structures can be compatible with 'foreach' if an 'iterator'-like function
 * is defined for them in the section EXTRA ITERABLES at the start of this header file */
#ifndef DEVKIT_MATH
#define _DEVKIT_MATH_ITERABLES
#endif

#ifndef __cplusplus
#define _dkt_iterator_cast(structure) _Generic( (structure), \
		_DEVKIT_ITERABLES \
		_DEVKIT_MATH_ITERABLES \
		DktIterator: _dkt_dummy_iterator, \
		DktString: _dkt_str_iterator, \
		DktHeap: _dkt_heap_iterator, \
		DktSet: _dkt_set_iterator, \
		default: dkt_view_iterator \
		)( &(structure))
#else
#define _dkt_iterator_cast(structure) (structure).iterator()
#endif


/* 'foreach' macro for 'enhanced for' loops.
 * It is recommended not to use this with items allocated on the stack
 * as it may seg-fault (don't know how to fix it). Most structures that have items allocated on the
 * heap should work */

#define _DKT_LOCAL_ITER(var_name) _dkt_local_iterator_##var_name

#ifdef __cplusplus
#define _DKT_TYPEOF(expr) auto
#else
#define _DKT_TYPEOF(expr) typeof(expr)
#endif

#define dkt_foreach( type, var, iter, ...) {					\
	for ( \
			_DKT_TYPEOF(_dkt_iterator_cast( iter )) _DKT_LOCAL_ITER(var) = _dkt_iterator_cast( iter ); \
			_DKT_LOCAL_ITER(var).ptr != _DKT_LOCAL_ITER(var).end_ptr; \
			_DKT_LOCAL_ITER(var).advance((DktIterator *)&(_DKT_LOCAL_ITER(var)))) \
	{                                              \
		type var = *(type*) (_DKT_LOCAL_ITER(var).ptr); \
		__VA_ARGS__;										  \
	}															  \
}

#define dkt_foreach_ptr( type, var, iter, ...) {					\
	for ( \
			_DKT_TYPEOF(_dkt_iterator_cast( iter )) _DKT_LOCAL_ITER(var) = _dkt_iterator_cast( iter ); \
			_DKT_LOCAL_ITER(var).ptr != _DKT_LOCAL_ITER(var).end_ptr; \
			_DKT_LOCAL_ITER(var).advance((DktIterator *)&(_DKT_LOCAL_ITER(var)))) \
	{                                              \
		type *var = (type*) (_DKT_LOCAL_ITER(var).ptr); \
		__VA_ARGS__;										  \
	}															  \
}

#ifdef DEVKIT_STRIP_PREFIXES
#define foreach     dkt_foreach
#define foreach_ptr dkt_foreach_ptr
#endif

#endif





/* 
 * ###################
 * # IMPLEMENTATIONS #
 * ###################
 */

/* ITERATOR IMPLEMENTATION */
#ifdef __cplusplus


#endif

/* VIEW IMPLEMENTATION */

#if defined(DEVKIT_LIST_IMPLEMENTATION) || defined(DEVKIT_ARRAY_IMPLEMENTATION) || defined(DEVKIT_STRING_IMPLEMENTATION)

#ifndef DEVKIT_NO_FOREACH
_DEVKIT_STATIC void _dkt_view_advance (DktIterator *it) { 
	it->ptr += ((DktViewIterator*)it)->typesize; 
}

_DEVKIT_STATIC DktViewIterator dkt_view_iterator( const DktViewType *view) {
	DktView *v = (DktView*) view;
	return (DktViewIterator) { 
		_dkt_view_advance,
		v->items,
		(char*) dkt_view_at(view, v->length),
		v->typesize,
	};
}


#endif

#ifdef __cplusplus // C++ extensions implementation

DktView::DktView (char *items, DktSize typesize, DktSize length) :
	items (items),
	typesize (typesize),
	length (length) {}


inline DktView DktView::view(DktSize start, DktSize end) const {
	return dkt_view_of(this, start, end);
}

inline DktView DktView::view_all() const {
	return dkt_view_all(this);
}

#endif // C++ extensions implementation

_DEVKIT_STATIC DktView dkt_view_all (const DktViewType *_v) {
	DktView *v = (DktView*) _v;
#ifdef DEVKIT_DEBUG
	assert(v && v->items);
	assert(v->length != 0);
#endif
	return *v;
}

_DEVKIT_STATIC DktView dkt_view_of (const DktViewType *v, DktSize start, DktSize end) {
	DktView *view = (DktView*) v;
#ifdef DEVKIT_DEBUG
	assert(end >= start);
	assert(view && view->items);
	assert(end - start < view->length);
#endif
	return (DktView) {
		view->items + start*view->typesize,
		view->typesize,
		end - start,
	};
}


_DEVKIT_STATIC void* dkt_view_at( const DktViewType *v, DktSize index) {
	DktView *view = (DktView*) v;
#ifdef DEVKIT_DEBUG
	assert(view && view->items);
#endif
	return view->items + view->typesize*index;
}


_DEVKIT_STATIC void dkt_view_set_all( DktViewType *__restrict v, const void *__restrict value) {
	DktView *view = (DktView*) v;
#ifdef DEVKIT_DEBUG
	assert(view && view->items);
	assert(value);
#endif
	for (size_t i = 0; i < view->length; ++i)
		memcpy(view->items + i*view->typesize, value, view->typesize);
}

_DEVKIT_STATIC bool dkt_view_contains(const DktViewType *__restrict v, const void *__restrict value) {
	DktView *view = (DktView*) v;
#ifdef DEVKIT_DEBUG
	assert(view && view->items);
	assert(value);
#endif
	for (size_t i = 0; i < view->length; ++i)
		if (memcmp(view->items + view->typesize*i, value, view->typesize) == 0)
			return true;
	return false;
}

_DEVKIT_STATIC void dkt_view_sort( DktViewType *__restrict v, DktComparator func) {
	DktView *view = (DktView*) v;
#ifdef DEVKIT_DEBUG
	assert(view && view->items);
	assert(func);
#endif
	qsort( view->items, view->length, view->typesize, func);
}

_DEVKIT_STATIC void dkt_view_copy (DktViewType *__restrict dest, DktViewType *__restrict src) {
	DktView
		*d = (DktView*) dest,
		*s = (DktView*) src;
#ifdef DEVKIT_DEBUG
	assert(d && d->items);
	assert(s && s->items);
	assert(d->length >= s->length);
#endif
	memcpy(d->items, s->items, s->typesize*s->length);
}

_DEVKIT_STATIC void dkt_view_copy_array (DktViewType *__restrict _dest, DktSize nitems, const void *__restrict _values) {
	DktView *dest = (DktView*) _dest;
#ifdef DEVKIT_DEBUG
	assert(dest && dest->items);
	assert(_values);
	assert(nitems < dest->length);
#endif
	memcpy(dest->items, _values, nitems*dest->typesize);
}

#endif // VIEW IMPLEMENTATION


/* STRING IMPLEMENTATION */

#ifdef DEVKIT_STRING_IMPLEMENTATION

#ifdef __cplusplus // C++ extension

DktString::DktString(char *items, DktSize length, DktSize capacity) :
	items (items),
	length (length),
	capacity (capacity) {}

DktString::DktString (const char *text) :
	length (strlen(text))
{
	items = (char *) calloc (length + 1, sizeof(char));
	memcpy (items, text, length);
}

#endif // C++ extension

#ifndef DEVKIT_NO_FOREACH
_DEVKIT_STATIC void _dkt_str_advance (DktIterator *it) {
	it->ptr++;
}
_DEVKIT_STATIC DktIterator _dkt_str_iterator( DktStringView *s) {
	return (DktIterator) { 
		_dkt_str_advance,
		s->items,
		dkt_str_at(*s, s->length),
	};
}
#endif

_DEVKIT_STATIC DktString dkt_str_new( const char *text) {
#ifdef DEVKIT_DEBUG
	assert(text);
#endif
	DktSize length = (DktSize) strlen(text);
	char *items = (char*) calloc(length + 1, sizeof(char));
	memcpy (items, text, length);
	return (DktString) { items, length, length };
}

_DEVKIT_STATIC DktString dkt_str_buffer (DktSize init_size) {
	return (DktString) {
		(char*) calloc(init_size, sizeof(char)),
		0,
		init_size,
	};
}


_DEVKIT_STATIC void dkt_str_init ( DktString *s, const char *text) {
#ifdef DEVKIT_DEBUG
	assert(text);
#endif
	memcpy( (void*)&s->length, (DktSize[]){(DktSize)strlen(text)}, sizeof(s->length));
	memcpy( s->items, text, sizeof(char)*s->length);
}


_DEVKIT_STATIC DktStringView dkt_str_view_all (DktString s) {
#ifdef DEVKIT_DEBUG
	assert( s.items);
#endif
	return (DktStringView) {
		s.items,
		s.length,
		0
	};
}

_DEVKIT_STATIC DktStringView dkt_str_view (DktStringView s, DktSize start, DktSize end) {
#ifdef DEVKIT_DEBUG
	assert( s.items);
	assert( end > start );
#endif
	return (DktString) {
		s.items + start,
		end - start,
		0
	};
}

_DEVKIT_STATIC inline char* dkt_str_at (DktStringView s, DktSize index) {
	return s.items + index;
}

_DEVKIT_STATIC void dkt_str_reverse(DktStringView s) {
#if defined(DEVKIT_INTERFACING) || defined(__cplusplus)
	char *buffer = (char*) alloca((s.length) * sizeof(char));
#else
	char buffer[s.length];
#endif
	memcpy( buffer, s.items, s.length);
	for (DktSize idx = 0; idx < s.length; idx++) {
		s.items[idx] = buffer[s.length - idx - 1];
	}
}


_DEVKIT_STATIC const char* dkt_str_cstr( DktString s) {
	return s.items;
}

_DEVKIT_STATIC int dkt_str_cmp(DktStringView a, DktStringView b) {
	// Take minimum length
	DktSize min_length = (a.length <= b.length)
		? a.length
		: b.length;

	for (DktSize i = 0; i < min_length; ++i) {
		if (a.items[i] == b.items[i]) continue;
		return memcmp(a.items + i, b.items + i, sizeof(char));
	}

	return memcmp(&a.length, &b.length, sizeof(DktSize));
}

_DEVKIT_STATIC void dkt_str_clear (DktString *s) {
	memset (s->items, 0, s->capacity * sizeof(char));
	s->length = 0;
}

_DEVKIT_STATIC DktString dkt_str_release (DktString *s) {
	char *cstr = s->items;
	DktSize len = s->length,
			  cap = s->capacity;
	memset ((void*)s, 0, sizeof(*s));
	return (DktString) { cstr, len, cap };
}


_DEVKIT_STATIC DktString dkt_str_copy(DktString sb) {
	char *copy = (char*) calloc(sb.capacity, sizeof(char));
	return (DktString) {
		copy,
		sb.length,
		sb.capacity,
	};
}

_DEVKIT_STATIC void dkt_str_append( DktString *sb, DktStringView s) {
#ifdef DEVKIT_DEBUG
	assert(sb && sb->items);
	assert(s.items);
#endif

	DktSize new_size = sb->length + s.length;

	if ( new_size + 1 > sb->capacity) {
		DktSize new_cap = sb->capacity + 1;
		while (new_cap < new_size) {
			new_cap *= 2;
		}
		_dkt_str_realloc( sb, new_cap);
	}

	strcat(sb->items, s.items);
	sb->length = new_size;
}

_DEVKIT_STATIC void dkt_str_cappend( DktString *sb, const char *s) {
	DktStringView wrap = { (char*)s, (DktSize) strlen(s), 0 };
	dkt_str_append(sb, wrap);
}


_DEVKIT_STATIC void _dkt_str_realloc( DktString *sb, DktSize new_capacity) {
#ifdef DEVKIT_DEBUG
	assert(sb && sb->items);
#endif
	// Clone data into buffer
#if defined(DEVKIT_INTERFACING) || defined(__cplusplus)
	char *buf = (char*) alloca(sb->length);
#else
	char buf[sb->length];
#endif
	memcpy(buf, sb->items, sb->length);

	// Reallocate items buffer and put data back
	sb->items = (char*) realloc((void*)sb->items, new_capacity);

	memcpy((void*)sb->items, buf, sb->length);
	sb->capacity = new_capacity;
}

_DEVKIT_STATIC void dkt_str_free(DktString *sb) {
	if (sb->items) free((void*)sb->items);
	memset((void*)sb, 0, sizeof(*sb));
}


#endif // STRING IMPLEMENTATION


/* LIST IMPLEMENTATION */

#ifdef DEVKIT_LIST_IMPLEMENTATION

#ifdef __cplusplus // C++ extension

DktList::DktList (char *items, size_t typesize, DktSize length, DktSize capacity) :
	DktView (items, typesize, length),
	capacity (capacity) {}

#endif // C++ extension

_DEVKIT_STATIC DktList _dkt_list_new(DktSize typesize, DktSize capacity) {
	return (DktList) {
		(char*) calloc(capacity,typesize),
		typesize,
		0,
		capacity,
	};
}

_DEVKIT_STATIC DktList dkt_list_from_view( const DktViewType *v) {
	DktView *l = (DktView*) v;
	char *items = (char*) calloc( l->length, l->typesize);
	memcpy( items, l->items, l->length*l->typesize);
	return (DktList) {
		items,
		l->typesize,
		l->length,
		l->length,
	};
}

_DEVKIT_STATIC DktList dkt_list_copy (const DktList l) {
	DktList cpy = { 
		(char*) calloc (l.capacity, l.typesize),
		l.typesize, 
		l.length, 
		l.capacity 
	};
	memcpy (cpy.items, l.items, l.length*l.typesize);
	return cpy;
}

_DEVKIT_STATIC void dkt_list_free( const DktList *list) {
#ifdef DEVKIT_DEBUG
	assert(list);
#endif
	if (list->items) free( list->items);
	memset((void*) list, 0, sizeof(*list));
}


_DEVKIT_STATIC void dkt_list_nadd( DktList *__restrict list, DktSize nitems, const void *const values) {
#ifdef DEVKIT_DEBUG
	assert( list && values );
#endif

	DktSize ptr = list->length; // Needed later
	list->length += nitems;

	// Allocate more memory if length increases beyond capacity
	if ( list->length > list->capacity) dkt_list_expand( list, list->length);
	// Copy values in pointers
	memcpy( list->items + ptr*list->typesize, values, nitems*list->typesize);
}


_DEVKIT_STATIC void dkt_list_ninsert( DktList *__restrict list, DktSize index, DktSize nitems, const void *const values) {
#ifdef DEVKIT_DEBUG
	assert( list && values );
#endif

	list->length += nitems;

	// Allocate more memory if needed
	if (list->length > list->capacity) dkt_list_expand( list, list->length);
	// Move following items forward, if there are any
	if (index < list->length) {
		memmove( list->items + (index+nitems)*list->typesize, list->items + index*list->typesize, list->typesize*(list->length-nitems - index) );
	}
	// Insert values at index
	memcpy( list->items + (index)*list->typesize, values, nitems*list->typesize);
}


_DEVKIT_STATIC void dkt_list_remove( void *dest, DktList *list, DktSize index) {
#ifdef DEVKIT_DEBUG
	assert( list && index <= list->length);
#endif

	if (dest) memcpy( dest, list->items + index, list->typesize);

	// If the item isn't last, every following item must be shifted backwards.
	if (index != --list->length) {
		char *_dest = list->items + index*list->typesize;
		char *src = _dest + list->typesize; // list->items + (index+1)*list->typesize
		memmove( _dest, src, list->typesize * (list->length - 1 - index) );
	}
}

_DEVKIT_STATIC void dkt_list_pop (void *const dest, DktList *l) {
	dkt_list_remove (dest, l, l->length - 1);
}


_DEVKIT_STATIC int _devkit_list_cmp(const void *a, const void*b) {
	return memcmp(a,b, sizeof(DktSize));
}

_DEVKIT_STATIC void dkt_list_nremove( 
		void *__restrict dest, 
		DktList *l, 
		DktSize nitems, 
		const DktSize *__restrict indices) 
{
#ifdef DEVKIT_DEBUG
	assert( l && indices );
#endif

#if defined(DEVKIT_INTERFACING) || defined(__cplusplus)
	DktSize *sorted = (DktSize*) alloca(nitems * sizeof(DktSize));
#else
	DktSize sorted[nitems];
#endif
	memcpy( sorted, indices, sizeof(DktSize)*nitems);
	qsort( sorted, nitems, sizeof(DktSize), _devkit_list_cmp);

	for (DktSize item = 0; item < nitems; item++) {
		DktSize index = indices[item] - item;
		if (dest) memcpy( (char*)dest + item*l->typesize, l->items + index*l->typesize, l->typesize);
		
		// If the item isn't last, every following item must be shifted backwards.
		if ( index != --l->length) {
			void *_dest = l->items + index*l->typesize, 
				 *src = l->items + (index+1)*l->typesize;
			memmove( _dest, src, l->typesize * (l->length - index));
		}
	}
}

_DEVKIT_STATIC bool dkt_list_concat( DktList *__restrict l, const DktViewType *__restrict v) {
	DktView *other = (DktView*) v;
#ifdef DEVKIT_DEBUG
	assert(l && l->items);
	assert(other && other->items);
#endif

	// Exit if sizes are different
	if ( l->typesize != other->typesize) return false;
	// Index of concatenation
	size_t concat_pos = l->length * l->typesize;
	l->length += other->length;

	if ( l->length > l->capacity) dkt_list_expand(l, l->length);
	// Copy items
	memcpy( l->items + concat_pos, other->items, other->length*l->typesize);

	return true;
}


_DEVKIT_STATIC void dkt_list_expand( DktList *l, DktSize new_capacity) {
#ifdef DEVKIT_DEBUG
	assert( l );
#endif
	DktSize prev_size = l->capacity*l->typesize;

	char *new_items = (char*) calloc( new_capacity, l->typesize);
#ifdef DEVKIT_DEBUG
	assert(new_items);
#endif
	memcpy( new_items, l->items, prev_size);
	free(l->items);
	l->items = new_items;
	l->capacity = new_capacity;
}

_DEVKIT_STATIC void dkt_list_trim( DktList *l) {
#ifdef DEVKIT_DEBUG
	assert( l );
#endif

	if (l->capacity == l->length) return;
	
	char *trim = (char*) malloc( l->length * l->typesize);
#ifdef DEVKIT_DEBUG
	assert(trim);
#endif
	memcpy( trim, l->items, l->length*l->typesize);
	free(l->items);

	l->items = trim;
	l->capacity = l->length;
}

_DEVKIT_STATIC DktList dkt_list_release (const DktList *l) {
	char *items = l->items;
	DktSize typesize = l->typesize;
	DktSize length = l->length;
	DktSize capacity = l->capacity;
	memset ((void*) l, 0, sizeof(*l));
	return (DktList) { items, typesize, length, capacity };
}

#endif // LIST IMPLEMENTATION




/* ARRAY IMPLEMENTATION */

#ifdef DEVKIT_ARRAY_IMPLEMENTATION

#ifdef __cplusplus // C++ extension

DktArray::DktArray (char *items, size_t typesize, DktSize length) :
		DktView (items, typesize, length) {}

#endif // C++ extension

_DEVKIT_STATIC DktArray _dkt_arr_new( DktSize typesize, DktSize length) {
	char *items = (char*) calloc(length, typesize);
	memset(items, 0, length * typesize);
	return (DktArray) { items, typesize, length };
}

_DEVKIT_STATIC DktView dkt_arr_view (const DktArray a) {
	return (DktView) { a.items, a.typesize, a.length };
}

_DEVKIT_STATIC DktArray dkt_arr_from_view(const DktViewType *v) {
	DktView *a = (DktView*) v;
#ifdef DEVKIT_DEBUG
	assert( a && a->items );
#endif
	char *items = (char*) calloc( a->length, a->typesize);
	memcpy( items, a->items, a->typesize*a->length);
	return (DktArray) { items, a->typesize, a->length };
}

_DEVKIT_STATIC void dkt_arr_init_values( DktArray *a, const void *const values) {
#ifdef DEVKIT_DEBUG
	assert(a && a->items);
	assert(values);
#endif
	memcpy(a->items, values, a->typesize*a->length);
}

_DEVKIT_STATIC void *dkt_arr_at (const DktArray a, DktSize index) {
	return a.items + index * a.typesize;
}


_DEVKIT_STATIC void dkt_arr_init( DktArray *a, DktSize typesize, DktSize length) {
#ifdef DEVKIT_DEBUG
	assert(a);
	assert(length != 0);
#endif
	a->typesize = typesize;
	a->length = length;
	a->items = (char*) (a + 1);
}


_DEVKIT_STATIC DktArray dkt_arr_concat( const DktArray a, const DktArray b) {
#ifdef DEVKIT_DEBUG
	assert( a.typesize == b.typesize);
	assert( a.items );
	assert( b.items );
#endif
	DktSize newlen = a.length + b.length;

	char *concat = (char*) calloc( newlen, a.typesize);
	memcpy( concat, a.items, a.length*a.typesize);
	memcpy( concat + a.length*a.typesize, b.items, b.length*a.typesize);

	return (DktArray) { concat, a.typesize, newlen };
}

_DEVKIT_STATIC DktArray dkt_arr_release (const DktArray *a) {
	char *items = a->items;
	size_t typesize = a->typesize;
	DktSize length = a->length;
	memset ((void*) a, 0, sizeof(*a));
	return (DktArray) { items, typesize, length };
}

_DEVKIT_STATIC void dkt_arr_free(const DktArray *a) {
	if (a->items) free(a->items);
	memset ((void*) a, 0, sizeof(*a));
}

#endif // ARRAY IMPLEMENTATION


/* POINTERS IMPLEMENTATION */

#if defined(DEVKIT_POINTERS_IMPLEMENTATION) //&& !defined(DEVKIT_INTERFACING)

#ifndef DEVKIT_INTERFACING

/* Creates an iterable object associated with the 'array' of 'length' items of 'typesize' */
_DEVKIT_STATIC DktViewIterator _dkt_iterator (void* a, DktSize length, DktSize typesize) {
#ifdef DEVKIT_DEBUG
	assert( a);
#endif
	return (DktViewIterator) {
		_dkt_view_advance,
		(char*) a,
		(char*) a + length*typesize,
		typesize,
	};
}

#endif

/* Gives a set of numbers from 'start' to 'end' - 1 */
_DEVKIT_STATIC void* _dkt_range( long start, long end, bool islong) {
#ifdef DEVKIT_DEBUG
	assert(start != end);
#endif
	int sign = (start < end) ? 1 : -1;
	long delta = labs(end - start);
	if (islong) {
		long *dest = (long*) calloc( delta, sizeof(long));
		for (long l = 0; l != delta; l++, start += sign) {
			dest[l] = start;
		}
		return dest;
	}
	else {
		int *dest = (int*) calloc( delta, sizeof(int));
		for (long l = 0; l != delta; l++, start += sign) {
			dest[l] = (int) start;
		}
		return dest;
	}
}


/* Returns a uniform dkt_range of 'steps' values between 'start' and 'end'.
 * NOTE: steps must be larger or equal than 2 */
_DEVKIT_STATIC void* _dkt_linspace( double start, double end, size_t steps, bool isfloat) {
#ifdef DEVKIT_DEBUG
	assert( steps >= 2);
#endif

	if (isfloat) {
		float delta = (end - start) / (steps - 1);
		float *values = (float*) calloc( steps, sizeof(float));
		for ( size_t step = 0; step < steps; step++) values[step] = start + delta*step;
		return values;
	}
	else {
		double delta = (end - start) / (steps - 1);
		double *values = (double*) calloc( steps, sizeof(double));
		for ( size_t step = 0; step < steps; step++) values[step] = start + delta*step;
		return values;
	}
}


_DEVKIT_STATIC void _dkt_free_all( void **ptrs) {
	for (int i = 0; ptrs[i]; ++i)
		free(ptrs[i]);
}

#endif // POINTER IMPLEMENTATION


/*
 * HEAP IMPLEMENTATION
 */

#ifdef DEVKIT_HEAP_IMPLEMENTATION

#ifdef __cplusplus // C++ extension

DktHeap::DktHeap (DktList &&inner, DktComparator cmp) :
	inner (inner.items, inner.typesize, inner.length, inner.capacity),
	cmp (cmp) {}

#endif // C++ extension

#define DKT_HEAP_IDX(inner_list, idx) ((inner_list)->items + idx*((inner_list)->typesize))

_DEVKIT_STATIC inline DktSize _dkt_heap_parent(DktSize i) {
	return (i) ? (i-1)/2 : 0;
}

_DEVKIT_STATIC inline DktSize _dkt_heap_cleft(DktSize i) {
	return 2*i + 1;
}
_DEVKIT_STATIC inline DktSize _dkt_heap_cright(DktSize i) {
	return 2*i + 2;
}

_DEVKIT_STATIC DktHeap _dkt_heap_new(const DktSize typesize, DktComparator cmp) {
#ifdef DEVKIT_DEBUG
	assert(typesize);
	assert(cmp);
#endif
	return (DktHeap) {
		_dkt_list_new(typesize, DKT_HEAP_INIT_CAPACITY),
		cmp
	};
}


_DEVKIT_STATIC void dkt_heap_init(DktHeap *heap, const DktSize typesize, DktComparator cmp) {
	heap->inner = _dkt_list_new(typesize, DKT_HEAP_INIT_CAPACITY);
	heap->cmp = cmp;
}


_DEVKIT_STATIC void _dkt_heap_move_up(DktHeap *heap, DktSize i) {
	// Return if i is 0 (front)
	if (!i) return;

	DktList *inner = &heap->inner;
	DktComparator cmp = heap->cmp;

#ifdef DEVKIT_DEBUG
	assert(inner && cmp);
	assert(i < inner->length);
	assert(inner->items);
#endif
	DktSize par_i = _dkt_heap_parent(i);
	// Buffer for swapping
#ifdef __cplusplus
	char *tmp = (char*) alloca(inner->typesize);
#else
	char tmp[inner->typesize];
#endif

	while (par_i != i && cmp(DKT_HEAP_IDX(inner, par_i), DKT_HEAP_IDX(inner, i)) > 0) {
		// Swap
		//printf("Items :: i = %d, par_i = %d\n", unref(int, DKT_HEAP_IDX(inner, par_i)), unref(int, DKT_HEAP_IDX(inner, i)));
		//printf("Stuff :: %u, %u\n", i, par_i);
		memcpy(tmp, DKT_HEAP_IDX(inner, i), inner->typesize);
		memcpy(DKT_HEAP_IDX(inner, i), DKT_HEAP_IDX(inner, par_i), inner->typesize);
		memcpy(DKT_HEAP_IDX(inner, par_i), tmp, inner->typesize);

		i		= par_i;
		par_i	= _dkt_heap_parent(par_i);
	}
}

_DEVKIT_STATIC void _dkt_heap_move_down(DktHeap *heap, DktSize i) {
	DktList *inner = &heap->inner;
	DktComparator cmp = heap->cmp;
#ifdef DEVKIT_DEBUG
	assert(inner && cmp);
	assert(i < inner->length);
	assert(heap && heap->inner.items);
#endif
	DktSize cx = 0;
	{
		DktSize cleft	= _dkt_heap_cleft(i);
		DktSize cright = _dkt_heap_cright(i);

		if (cleft >= inner->length) {
			if (cright >= inner->length)
				return;
			cx = cright;
		}
		else if (cright >= inner->length) {
			cx = cleft;
		}
		else {
			cx = (cmp(DKT_HEAP_IDX(inner, cleft), DKT_HEAP_IDX(inner, cright)) <= 0) 
				? cleft 
				: cright;
		}
	}

	if (cmp(DKT_HEAP_IDX(inner, i), DKT_HEAP_IDX(inner, cx)) > 0) {
		{
			// Swap
		#ifdef __cplusplus
			char *tmp = (char*) alloca(inner->typesize);
		#else
			char tmp[inner->typesize];
		#endif
			memcpy(tmp, DKT_HEAP_IDX(inner, i), inner->typesize);
			memcpy(DKT_HEAP_IDX(inner, i), DKT_HEAP_IDX(inner, cx), inner->typesize);
			memcpy(DKT_HEAP_IDX(inner, cx), tmp, inner->typesize);
		}
		_dkt_heap_move_down(heap, cx);
	}
}


_DEVKIT_STATIC void dkt_heap_push(DktHeap *__restrict heap, const void *__restrict value) {
	dkt_list_add(&heap->inner, value);
	_dkt_heap_move_up(heap, heap->inner.length - 1);
}


_DEVKIT_STATIC const void* dkt_heap_front(const DktHeap heap) {
	return (heap.inner.length) ? heap.inner.items : nullptr;
}


_DEVKIT_STATIC void dkt_heap_pop_front(DktHeap *__restrict heap, void *__restrict dest) {
#ifdef DEVKIT_DEBUG
	assert(heap && heap->inner.items);
	assert(dest);
#endif
	DktList *inner = &heap->inner;
	if (!inner->length) return;

	// Take out front (if dest) and put the last element at the front, then move down
	if (dest) memcpy(dest, DKT_HEAP_IDX(inner, 0), inner->typesize);

	--inner->length;
	if (inner->length > 0) {
		memcpy(DKT_HEAP_IDX(inner, 0), DKT_HEAP_IDX(inner, inner->length), inner->typesize);
		memset(DKT_HEAP_IDX(inner, inner->length), 0, inner->typesize);
		_dkt_heap_move_down(heap, 0);
	}
	else {
		memset(DKT_HEAP_IDX(inner, 0), 0, inner->typesize);
	}
}


_DEVKIT_STATIC DktSize dkt_heap_length(const DktHeap heap) {
	return heap.inner.length;
}


_DEVKIT_STATIC void dkt_heap_free(const DktHeap *heap) {
#ifdef DEVKIT_DEBUG
	assert(heap);
#endif
	dkt_list_free(&heap->inner);
}


_DEVKIT_STATIC void dkt_heap_trim(DktHeap *heap) {
	dkt_list_trim(&heap->inner);
}

_DEVKIT_STATIC DktHeap dkt_heap_release (DktHeap *heap) {
	DktComparator cmp = heap->cmp;
	heap->cmp = 0;
	return (DktHeap) { dkt_list_release(&heap->inner), cmp };
}

#ifndef DEVKIT_NO_FOREACH
_DEVKIT_STATIC DktHeapIterator _dkt_heap_iterator (DktHeap *h) {
	return (DktHeapIterator) {
		_dkt_view_advance,
		h->inner.items,
		h->inner.items + h->inner.length*h->inner.typesize,
		h->inner.typesize,
	};
}
#endif

#undef DKT_HEAP_IDX

#endif // HEAP IMPLEMENTATION





/* SET IMPLEMENTATION */

#ifdef DEVKIT_SET_IMPLEMENTATION

#define DKT_SET_INIT_BUCKETS 8
#define DKT_SET_BUCKET_CAPACITY 4

#define DKT_SET_BUCKET_SIZE(type_size) ( sizeof(struct _dkt_set_bucket) + DKT_SET_BUCKET_CAPACITY*(type_size) )
#define DKT_SET_BUCKET_OFFSET(i, set) ((i) * (sizeof(*(set).items) + DKT_SET_BUCKET_CAPACITY*(set).typesize))
#define DKT_SET_BUCKET(i, set) (struct _dkt_set_bucket*)( ((char*)(set).items) + DKT_SET_BUCKET_OFFSET((i), (set)) )


#ifndef DEVKIT_NO_FOREACH

#define _DKT_BUCKET(set_buckets, set_ts, idx) ((struct _dkt_set_bucket*)( (char*)((set_buckets) + (idx)) + ((idx) * DKT_SET_BUCKET_CAPACITY*(set_ts) ) ))

_DEVKIT_STATIC void _dkt_set_advance (DktIterator *_it) {
	DktSetIterator *it = (DktSetIterator *) _it;
	struct _dkt_set_bucket *next_bucket = _DKT_BUCKET(it->bucket, it->typesize, 1);
	
	if (it->ptr - (char*)(it->bucket + 1) == (it->bucket->length - 1)*it->typesize) {
		it->bucket = _DKT_BUCKET(it->bucket, it->typesize, 1);
		it->ptr = (char*)(it->bucket + 1);
	}
	else {
		it->ptr += it->typesize;
	}
}

_DEVKIT_STATIC DktSetIterator _dkt_set_iterator (DktSet *s) {
	return (DktSetIterator) {
		_dkt_set_advance,
		(char*) s->items + 1,
		(char*) (DKT_SET_BUCKET(s->nbuckets, *s) + 1),
		s->items,
		s->typesize,
	};
}

#endif

#ifdef __cplusplus // C++ extension

DktSet::DktSet (_dkt_set_bucket *items, DktSize nbuckets, DktSize typesize, DktHash hash) :
	items (items),
	nbuckets (nbuckets),
	typesize (typesize),
	hash (hash) {}

#endif // C++ extension

DktSet _dkt_set_new (DktSize typesize, DktHash hash) {
	struct _dkt_set_bucket *
		items = (struct _dkt_set_bucket *) calloc(DKT_SET_INIT_BUCKETS, DKT_SET_BUCKET_SIZE(typesize));

	return (DktSet) {
		items,
		DKT_SET_INIT_BUCKETS,
		typesize,
		(hash) ? hash : dkt_standard_hash,
	};
}


void _dkt_set_grow(DktSet *set) {
	// Make a grown version of the set
	DktSet grown_set = { 
		nullptr,
		0,
		set->typesize, 
		set->hash
	};
	DktSize more_buckets = set->nbuckets + set->nbuckets / 2;
	grown_set.items = (struct _dkt_set_bucket *) calloc(more_buckets, DKT_SET_BUCKET_SIZE(set->typesize));
	grown_set.nbuckets = more_buckets;

	// Put items into the grown set
	struct _dkt_set_bucket *bucket;
	for (size_t i = 0; i < set->nbuckets; ++i) {
		bucket = DKT_SET_BUCKET(i, *set);
		
		for (size_t j = 0; j < bucket->length; ++j) {
			dkt_set_add(&grown_set, (char*)(bucket + 1) + j*set->typesize);
		}
	}

	// Override old set with new one, free old allocation
	free(set->items);
	memcpy ((void*)set, &grown_set, sizeof(*set));
}


bool dkt_set_add(DktSet *__restrict set, const void *__restrict value) {
	size_t bucket_idx = set->hash(value, set->typesize) % set->nbuckets;

	struct _dkt_set_bucket *bucket = DKT_SET_BUCKET(bucket_idx, *set);

	if (bucket->length + 1 > DKT_SET_BUCKET_CAPACITY) {
		_dkt_set_grow(set);
		// Reobtain pointer and index as growth invalidates it
		bucket_idx = set->hash(value, set->typesize) % set->nbuckets;
		bucket = DKT_SET_BUCKET(bucket_idx, *set);
	}
	
	for (unsigned short i = 0; i < bucket->length; ++i) {
		// Do not add item if present
		if ( memcmp( (char*)(bucket + 1) + i*set->typesize, value, set->typesize) == 0)
			return false;
	}
	// Add value
	memcpy( (char*)(bucket + 1) + bucket->length*set->typesize, value, set->typesize );
	++bucket->length;
	return true;
}


bool dkt_set_contains(DktSet set, const void *value) {
	const DktSize bucket_idx = set.hash(value, set.typesize) % set.nbuckets;

	struct _dkt_set_bucket *bucket = DKT_SET_BUCKET(bucket_idx, set);

	for (unsigned short i = 0; i < bucket->length; ++i) {
		if ( memcmp( (char*)(bucket + 1) + i*set.typesize, value, set.typesize) == 0)
			return true;
	}
	return false;
}


bool dkt_set_remove(DktSet set, const void *value) {
	const DktSize bucket_idx = set.hash(value, set.typesize) % set.nbuckets;
	struct _dkt_set_bucket *bucket = DKT_SET_BUCKET(bucket_idx, set);

	// Find value
	for (unsigned short i = 0; i < bucket->length; ++i) {
		char *item = (char*)(bucket + 1) + i*set.typesize;
		if ( memcmp( item, value, set.typesize) == 0) {
			// Remove value
			// If not at the end, shift everything on top of it
			if (i != bucket->length - 1) {
				memmove( item, item+set.typesize, (bucket->length - i - 1)*set.typesize);
			}
			--bucket->length;
			return true;
		}
	}
	// No match was found
	return false;
}


_DEVKIT_STATIC void dkt_set_free(const DktSet *set) {
	free(set->items);
	memset((void*) set, 0, sizeof(*set));
}

_DEVKIT_STATIC DktSet dkt_set_release (const DktSet *set) {
	struct _dkt_set_bucket *items = set->items;
	DktSize nbuckets = set->nbuckets;
	DktSize typesize = set->typesize;
	DktHash hash = set->hash;
	memset ((void*)set, 0, sizeof(*set));
	return (DktSet) { items, nbuckets, typesize, hash };
}


#define DKT_DEFAULT_HASH dkt_standard_hash

size_t dkt_standard_hash(const void *ptr, const size_t size) {
	size_t hash = 0;
	const unsigned char *bytes = (const unsigned char *) ptr;
	for (size_t i = 0; i < size; --i) {
		hash <<= 8;
		hash += bytes[i];
	}
	return hash;
}


size_t dkt_set_size(const DktSet *set) {
#ifdef DEVKIT_DEBUG
	assert(set);
#endif
	size_t count = 0;

	struct _dkt_set_bucket *bucket;
	for (size_t i = 0; i < set->nbuckets; ++i) {
		bucket = DKT_SET_BUCKET(i, *set);
		count += bucket->length;
	}
	
	return count;
}

#undef DKT_SET_BUCKET
#undef DKT_SET_BUCKET_CAPACITY
#undef DKT_SET_BUCKET_OFFSET


#endif // SET IMPLEMENTATION





/* MATH IMPLEMENTATION */

#ifdef DEVKIT_MATH_IMPLEMENTATION

#ifdef __cplusplus // C++ extensions implementation

DktVector::DktVector(DktFloat *items, DktSize length) :
	items (items), length (length) {}

DktVector::DktVector(DktSize length) :
	items ( (DktFloat*) calloc(length, sizeof(DktFloat)) ),
	length (length) 
{}


DktMatrix::DktMatrix(DktFloat *items, DktSize length, DktSize rows, DktSize cols) :
	DktVector(items, length),
	rows (rows), 
	columns (cols) 
{ }

DktMatrix::DktMatrix(DktSize rows, DktSize cols) : 
	DktVector ( nullptr, rows*cols),
	rows (rows), 
	columns (cols)
{
	items = (DktFloat*) calloc(this->length, sizeof(DktFloat));
}



#endif // C++ extensions implementation

#ifndef DEVKIT_NO_FOREACH

_DEVKIT_STATIC inline void _dkt_vec_advance (DktIterator *it) {
	it->ptr += sizeof(DktFloat);
}

_DEVKIT_STATIC inline DktVectorIterator _dkt_vec_iterator (DktVector *v) {
	return (DktVectorIterator) {
		_dkt_vec_advance,
		(char*) v->items,
		(char*) dkt_vec_at(*v, v->length),
	};
}

_DEVKIT_STATIC inline DktVectorIterator _dkt_mat_iterator (DktMatrix *m) {
	return _dkt_vec_iterator((DktVector*) m);
}

#endif


_DEVKIT_STATIC DktVec2 dkt_vec2_new( DktFloat x, DktFloat y) {
	return (DktVec2) { x, y};
}
_DEVKIT_STATIC DktVec2 dkt_vec2_sum( DktVec2 v, DktVec2 w) {
	return (DktVec2) { v.x + w.x, v.y + w.y};
}
_DEVKIT_STATIC DktVec2 dkt_vec2_sub( DktVec2 v, DktVec2 w) {
	return (DktVec2) { v.x - w.x, v.y - w.y};
}
_DEVKIT_STATIC DktVec2 dkt_vec2_scale( DktVec2 v, DktFloat scale) {
	return (DktVec2) { v.x * scale, v.y * scale};
}
_DEVKIT_STATIC DktFloat dkt_vec2_mod(DktVec2 v) {
	return sqrt( pow(v.x,2) + pow(v.y,2));
}
_DEVKIT_STATIC DktVectorView dkt_vec2_asvec(DktVec2 v) {
	DktVectorView vector = { (DktFloat*) &v, 2 };
	return vector;
}

_DEVKIT_STATIC inline DktMatrix dkt_vec2_asmat(DktVec2 v) {
	return (DktMatrix) { (DktFloat*) &v, 2, 1, 2 };
}

_DEVKIT_STATIC inline DktVec3 dkt_vec3_new( DktFloat x, DktFloat y, DktFloat z) {
	return (DktVec3) { x, y, z};
}
_DEVKIT_STATIC inline DktVec3 dkt_vec3_sum( DktVec3 v, DktVec3 w) {
	return (DktVec3) { v.x+w.x, v.y+w.y, v.z+w.z};
}
_DEVKIT_STATIC inline DktVec3 dkt_vec3_sub( DktVec3 v, DktVec3 w) {
	return (DktVec3) { v.x-w.x, v.y-w.y, v.z-w.z};
}
_DEVKIT_STATIC inline DktVec3 dkt_vec3_scale( DktVec3 v, DktFloat scale) {
	return (DktVec3) { v.x*scale, v.y*scale, v.z*scale};
}
_DEVKIT_STATIC inline DktFloat dkt_vec3_mod( DktVec3 v) {
	return rootn( pow(v.x,3) + pow(v.y,3) + pow(v.z,3), 3);
}
_DEVKIT_STATIC inline DktVectorView dkt_vec3_asvec( DktVec3 v) {
	return (DktVectorView) { (DktFloat*) &v, 3 };
}

_DEVKIT_STATIC inline DktMatrix dkt_vec3_asmat( DktVec3 v) {
	return (DktMatrix) { (DktFloat*) &v, 3, 1, 3 };
}


_DEVKIT_STATIC inline DktVector dkt_vec_new( const DktSize length) {
#ifdef __cplusplus
	return DktVector {length};
#else
	return (DktVector) {
		.items = (DktFloat *) calloc( length, sizeof(DktFloat)),
		.length = length,
	};
#endif
}

_DEVKIT_STATIC inline void dkt_vec_init( DktVectorView v, const DktSize length, DktFloat values[]) {
#ifdef DEVKIT_DEBUG
	assert(v.items);
	assert(values);
	assert(length != 0);
#endif
	memcpy((DktSize*)&v.length, &length, sizeof(v.length));
	memcpy(v.items, values, length*sizeof(DktFloat));
}

_DEVKIT_STATIC inline void dkt_vec_init_values ( DktVectorView v, DktFloat values[]) {
#ifdef DEVKIT_DEBUG
	assert(v.items);
	assert(values);
#endif
	memcpy(v.items, values, v.length*sizeof(DktFloat));
}


_DEVKIT_STATIC inline void dkt_vec_free( DktVector *v) {
#ifdef DEVKIT_DEBUG
	assert(v && v->items);
#endif
	free( v->items);
	memset((void*)v, 0, sizeof(*v));
}


_DEVKIT_STATIC inline DktMatrix dkt_vec_asmat(DktVectorView v) {
	return (DktMatrix) { v.items, v.length, 1, v.length };
}

_DEVKIT_STATIC inline DktVector dkt_vec_copy(DktVectorView v) {
	DktFloat *copy = (DktFloat *) calloc(v.length, sizeof(DktFloat));
	memcpy(copy, v.items, v.length * sizeof(DktFloat));
	return (DktVector) { copy, v.length };
}


_DEVKIT_STATIC inline DktFloat* dkt_vec_at( DktVectorView v, DktSize index) {
	return v.items + index;
}


_DEVKIT_STATIC bool dkt_vec_equals( DktVectorView v, DktVectorView other) {
	if (v.length != other.length) return false;
	for (size_t idx = 0; idx < v.length; idx++) {
		if ( v.items[idx] != other.items[idx]) 
			return false;
	}
	return true;
}


_DEVKIT_STATIC inline void dkt_vec_sum( DktVectorView v, DktVectorView other) {
#ifdef DEVKIT_DEBUG
	assert(v.items);
	assert(other.items);
	assert(v.length == other.length);
#endif
	for (DktSize idx = 0; idx < v.length; idx++)
		v.items[idx] += other.items[idx];
}

_DEVKIT_STATIC inline void dkt_vec_sub( DktVectorView v, DktVectorView other) {
#ifdef DEVKIT_DEBUG
	assert(v.items);
	assert(other.items);
	assert(v.length == other.length);
#endif
	for (DktSize idx = 0; idx < v.length; idx++)
		v.items[idx] -= other.items[idx];
}


_DEVKIT_STATIC inline void dkt_vec_scale( DktVectorView v, const DktFloat scalar) {
#ifdef DEVKIT_DEBUG
	assert(v.items);
#endif
	for (DktSize idx = 0; idx < v.length; idx++) {
		v.items[idx] *= scalar;
	}
}


_DEVKIT_STATIC bool dkt_vec_iszero( DktVectorView v) {
#ifdef DEVKIT_DEBUG
	assert(v.items);
#endif
	for ( DktSize idx = 0; idx < v.length; idx++) {
		if ( v.items[idx] != 0)
			return false;
	}
	return true;
}

_DEVKIT_STATIC DktVector dkt_vec_release (DktVector *v) {
	DktFloat *items = v->items;
	const DktSize length = v->length;
	memset ((void*)v, 0, sizeof(*v));
	return (DktVector) { items, length };
}


_DEVKIT_STATIC DktMatrix dkt_mat_new( DktSize rows, DktSize columns) {
#ifdef __cplusplus
	return DktMatrix(rows,columns);
#else
	return (DktMatrix) {
		.items = (DktFloat*) calloc( rows*columns, sizeof(long)),
		.length = rows*columns,
		.columns = columns,
		.rows = rows,
	};
#endif
}


_DEVKIT_STATIC inline void dkt_mat_init (DktMatrix *m, const DktSize cols, const DktSize rows, DktFloat values[]) {
#ifdef DEVKIT_DEBUG
	assert(m->items);
	assert(cols != 0 && rows != 0);
	assert(values);
#endif
	// Initialize length with some dirty tricks to bypass the 'const' qualifier
	memcpy((size_t*)&m->length, (size_t[]){cols*rows}, sizeof(size_t));
	m->columns = cols;
	m->rows = rows;
	memcpy(m->items, values, sizeof(DktFloat)*cols*rows);
}

_DEVKIT_STATIC inline void dkt_mat_init_values (DktMatrix m, DktFloat values[]) {
#ifdef DEVKIT_DEBUG
	assert(m.items);
	assert(m.length != 0);
	assert(values);
#endif
	memcpy(m.items, values, sizeof(DktFloat)*m.length);
}


_DEVKIT_STATIC inline void dkt_mat_free(DktMatrix *m) {
#ifdef DEVKIT_DEBUG
	assert(m);
#endif
	if (m->items) free(m->items);
	memset((void*)m, 0, sizeof(*m));
}

_DEVKIT_STATIC inline DktVectorView dkt_mat_asvec(DktMatrix m) {
	return (DktVector) { m.items, m.length };
}

_DEVKIT_STATIC DktMatrix dkt_mat_copy(const DktMatrix m) {
#ifdef DEVKIT_DEBUG
	assert(m.items);
#endif
	DktFloat *copy = (DktFloat*) calloc(m.length, sizeof(DktFloat));
	memcpy(copy, m.items, m.length * sizeof(DktFloat));
	return (DktMatrix) {
		copy,
		m.length,
		m.columns,
		m.rows
	};
}


_DEVKIT_STATIC inline DktFloat* dkt_mat_at(DktMatrix m, DktSize col, DktSize row) {
#ifdef DEVKIT_DEBUG
	assert(m.items);
	assert(col < m.columns);
	assert(row < m.rows);
#endif
	return &m.items[m.columns*row + col];
}


_DEVKIT_STATIC bool dkt_mat_equals( DktMatrix A, DktMatrix B) {
#ifdef DEVKIT_DEBUG
	assert(A.items);
	assert(B.items);
#endif
	return ( A.rows == B.rows && A.columns == B.columns)
		? ( memcmp( A.items, B.items, A.length))
			? true
			: false
		: false;
}


_DEVKIT_STATIC void dkt_mat_transpose( DktMatrix *m) {
#ifdef DEVKIT_DEBUG
	assert(m->items);
#endif
	DktFloat *buffer = (DktFloat *) calloc(m->length, sizeof(DktFloat));
	memcpy( buffer, m->items, m->length*sizeof(DktFloat));

	DktSize temp = m->columns;
	m->columns = m->rows;
	m->rows = temp;

	for ( DktSize row = 0; row < m->rows; row++)
		for ( DktSize col = 0; col < m->columns; col++)
			m->items[m->columns*row + col] = buffer[m->rows*col + row];

	free(buffer);
}


_DEVKIT_STATIC inline void dkt_mat_sum( DktMatrix dest, DktMatrix mat) {
#ifdef DEVKIT_DEBUG
	assert(dest.items);
	assert(mat.items);
	assert(dest.rows == mat.rows && dest.columns == mat.columns);
#endif
	for (size_t i = 0; i < dest.length; ++i)
		dest.items[i] += mat.items[i];
}

_DEVKIT_STATIC inline void dkt_mat_sub (DktMatrix dest, DktMatrix other) {
#ifdef DEVKIT_DEBUG
	assert(dest.items);
	assert(other.items);
	assert(dest.rows == other.rows && dest.columns == other.columns);
#endif
	for (size_t i = 0; i < dest.length; ++i)
		dest.items[i] -= other.items[i];
}


_DEVKIT_STATIC DktMatrix dkt_mat_mul(const DktMatrix A, const DktMatrix B) {
#ifdef DEVKIT_DEBUG
	assert(A.items);
	assert(B.items);
	assert(A.columns == B.rows);
#endif
	DktMatrix result = dkt_mat_new(A.rows, B.columns);
	DktFloat *r;
	for (size_t col = 0; col < result.columns; col++) {
		for (size_t row = 0; row < result.rows; row++) {
			r = result.items+(result.columns*row + col);

			for (size_t idx = 0; idx < A.columns; idx++) {
					*r += A.items[row*A.columns + idx]
						* B.items[col + idx*B.columns];
			}
		}
	}
#ifdef __cplusplus
	return result.release();
#else 
	return result;
#endif
}

_DEVKIT_STATIC bool dkt_mat_iszero( const DktMatrix m) {
	for ( size_t idx = 0; idx < m.length; idx++) {
		if ( m.items[idx] != 0)
			return false;
	}
	return true;
}

_DEVKIT_STATIC DktMatrix dkt_mat_release (DktMatrix *m) {
	DktMatrix copy = *m;
	memset (m, 0, sizeof(*m));
	return copy;
}


_DEVKIT_STATIC DktVec2 dkt_mat_tovec2	(const DktMatrix m) {
#ifdef DEVKIT_DEBUG
	assert(m.items && m.length == 2);
#endif
	return (DktVec2) {
		.x = m.items[0],
		.y = m.items[1]
	};
}
_DEVKIT_STATIC DktVec3 dkt_mat_tovec3	(const DktMatrix m) {
#ifdef DEVKIT_DEBUG
	assert(m.items && m.length == 3);
#endif
	return (DktVec3) {
		.x = m.items[0],
		.y = m.items[1],
		.z = m.items[2]
	};
}

_DEVKIT_STATIC DktVec2	dkt_vec_tovec2(const DktVectorView v) {
#ifdef DEVKIT_DEBUG
	assert(v.items && v.length == 2);
#endif
	return (DktVec2) {
		.x = v.items[0],
		.y = v.items[1]
	};
}

_DEVKIT_STATIC DktVec3 	dkt_vec_tovec3(const DktVectorView v) {
#ifdef DEVKIT_DEBUG
	assert(v.items && v.length == 2);
#endif
	return (DktVec3) {
		.x = v.items[0],
		.y = v.items[1],
		.z = v.items[2]
	};
}

#endif // MATH IMPLEMENTATION

#endif // _DEVKIT_H
