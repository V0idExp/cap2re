#include "Common.h"
#include "CameraManagerWrapper.h"
#include "CameraWrapper.h"

#ifdef HAVE_GPHOTO2
#include "Gphoto2Camera.h"
#include "Gphoto2CameraManager.h"
#endif

#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include <iostream>
using namespace boost;
using namespace std;

void func(const String &s)
{
    cout << s << endl;
}

class MyClass
{
public:
    void func()
    {
        cout << "MyClass::func()" << endl;
    }

    vector<String>& strings()
    {
        vector<String> *s = new vector<String>;
        s->push_back("hello");
        s->push_back("world");
        s->push_back("of");
        s->push_back("python");

        return *s;
    }
};

class Base
{
public:
    virtual void func() = 0;
};

class Derived : public Base
{
public:
    virtual void func()
    {
        cout << "Derived::func()"<< endl;
    }
};


BOOST_PYTHON_MODULE(cap2re)
{
    using namespace python;
    // export func()
    def("test", &func);

    // export MyClass
    class_<MyClass>("MyClass")
            .def("func", &MyClass::func)
            .def("strings", &MyClass::strings, return_value_policy<copy_non_const_reference>());

    // export StringVec
    class_<vector<String> >("StringVec")
            .def(vector_indexing_suite<vector<String> >());
}
