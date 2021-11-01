# IntervalArray
Quick interval lookup for internal LCP values.
## Parameters
```c++
const VALUETYPE MAX_INTERVAL_VALUE = 255;//Max interval value
size_t size; //size 
size_t leaf;
VALUETYPE *leaf_ints;
VALUETYPE *intervals;
```
## Methods
```c++
/**
 * Constructor
 *@param VALUETYPE *ints interval value
 *@param size_t n leaf
 */
IntervalArray(VALUETYPE *ints, size_t n);
/**
 *Constructor
 *@param VALUETYPE *ints interval value
 *@param size_t n leaf
 */
void build(VALUETYPE *ints, size_t n);
/** Clear all objects */
void clear();
/** Print all objects */
void print( std::ostream &out );
/** Get the size of all objects */
size_t  getSize();
/**
 *Get Index
 *@param size_t l
 *@param size_t r
 *@return long
 */
long getIndex( size_t l , size_t r );
VALUETYPE fill( size_t l, size_t r );
VALUETYPE* getIntervals();
/**
 *Get value by index
 *@param size_t l
 *@param size_t r
 *@return VALUETYPE 
 */
VALUETYPE  getValue(size_t i, size_t j);
/**Write intervals into filename*/
void dump( const char *filename );
/**Load intervals into filename with the size of filesize*/
void load( const char *filename, size_t filesize );

```
## Class
```c++
class IntervalArray
{
	private:
		size_t size;
		size_t leaf;
		VALUETYPE *leaf_ints;
		VALUETYPE *intervals;
		long getIndex( size_t l , size_t r );
		VALUETYPE fill( size_t l, size_t r );
	public:
		IntervalArray();
		IntervalArray(VALUETYPE *ints, size_t n);
		~IntervalArray();
		void build(VALUETYPE *ints, size_t n);
		void clear();
		void print( std::ostream &out );
		size_t   getSize() ;
		VALUETYPE* getIntervals();
		VALUETYPE  getValue(size_t i, size_t j);
		void dump( const char *filename );
		void load( const char *filename, size_t filesize );
};
```