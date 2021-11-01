# File
File IO handler
## Methods
```c++
/**
* Write 's' size of 'Type' data into filename
* @param  Type data type
* @param  Size s
* @param  char File name		
* @return void	
*/
template<typename Type, typename Size>
void write( Type *A, Size s, const char *filename );
/**
* Read 'Type' data from filename
* @param  Type data type
* @param  int  extra data size
* @param  char File name		
* @return size_t size 	
*/
template<typename Type>
size_t read( Type *&A, const char *filename, int extra );
```