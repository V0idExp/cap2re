#ifndef COMMONWRAPPER_H
#define COMMONWRAPPER_H

#include <vector>
#include <boost/python.hpp>
#include "Error.h"

#include <iostream>
using namespace std;

template<class ValueType>
struct Vector2List
{
    static ValueType
    getitem(const std::vector<ValueType> &self, int index)
    {
        if(index < 0)
            index += self.size();

        cout << "Requesting element " << index << endl;
        if(index >= 0 && index < self.size())
            return self[index];

        throw RuntimeError("index out of range");

    }

    static int
    len(const std::vector<ValueType> &self)
    {
        return self.size();
    }

    iterator(const std::vector<ValueType> &self)
    {

    }

    static void
    indexError()
    {
        PyErr_SetString(PyExc_IndexError, "Index out of range");
    }
};

#endif // COMMONWRAPPER_H
