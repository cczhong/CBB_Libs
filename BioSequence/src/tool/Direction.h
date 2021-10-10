
#ifndef DIRECTION_H_
#define DIRECTION_H_

struct Right 
{

};
struct Left 
{ 

};
// x + i right
template<typename _val>
_val GetDir(const _val* x, int i, const Right&)
{
    return *(x + i);
}

//x - i left
template<typename _val>
_val GetDir(const _val* x, int i, const Left&)
{ 
    return *(x-i); 
}
// x + i right
template<typename _val>
const _val* GetDirPtr(const _val* x, int i, const Right&)
{ 
    return x+i;
}
// x - i left
template<typename _val>
const _val* GetDirPtr(const _val* x, int i, const Left&)
{ 
    return x-i;
}
// x + 1 right
template<typename _val>
const _val* IncDir(const _val* x, const Right&)
{ 
    return x+1;
}
// x -1 left
template<typename _val>
const _val* IncDir(const _val* x, const Left&)
{ 
    return x-1;
}

#endif /* DIRECTION_H_ */
