// Array.h

#ifndef _ARRAY_H
#define _ARRAY_H

// C++ headers
#include <iostream.h>

#define Array_MinAlloc 2

template <class Type>
class Array {
public:
  Array(int initial = 0) : size(0), allocated(0), data(0)
      { build(0,initial); }
  Array(const Type *array, int initial) : size(0), allocated(0), data(0)
      { build (array,initial); }
  Array(const Array<Type> &ref) : size(0), allocated(0), data(0)
      { build(ref.data, ref.size); }
  ~Array() { if (data) delete [] data; }
  Array<Type>& operator=(const Array<Type>&);
  int length() const { return size; }
  void resize(int newsize);
  void expand_by(int add)
      { resize(size + add); }
  
  Type& operator[](int index);
  const Type& operator[](int index) const;
  
private:
  void build(const Type *source, int buildsize);
  
  int	size;
  int	allocated;
  Type	*data;
};

template <class Type>
void Array<Type>::build(const Type *source, int initial)
{
  resize(initial);
  if (source) {
    for(int i = 0; i < initial; i++) {
      data[i] = source[i];
    }
  }
}

template <class Type>
void Array<Type>::resize(int newsize)
{
  Type *newdata;
  int newalloc;

  if (newsize > allocated) {
    newalloc = newsize + newsize / 2;	// OK, if you insist!
    if (newalloc < Array_MinAlloc) {
      newalloc = Array_MinAlloc;
    }
    newdata = new Type[newalloc];

    if (data) { // Copy the old data first..
      for (int i = 0; i < size; i++) {
	newdata[i] = data[i];
      }
      delete [] data;
    }

    data = newdata;
    allocated = newalloc;
  }
  size = newsize;
}

template <class Type>
Array<Type>& Array<Type>::operator=(const Array<Type>& src)
{
  if (this == &src)
    return *this; // Assignment from self!

  if (data) {
    delete [] data;
    data=0;
    allocated=0;
  }

  build(src.data, src.size);
  return *this;
}


template <class Type>
Type& Array<Type>::operator[](int index)
{
  if (index < 0) {
    cerr << "FATAL: Illegal index on Array" << endl;
  }

  if (data && index < size) {
    return data[index];
  } else {
    resize(index+1);
    return data[index];
  }
}

template <class Type>
const Type& Array<Type>::operator[](int index) const
{

  if ((index < 0) || (index>=size)) {
    cerr << "FATAL: Illegal index on Array" << endl;
  }

  return data[index];
}

#endif // _ARRAY_H

