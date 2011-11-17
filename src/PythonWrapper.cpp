#include "CommonWrapper.h"
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
class Base
{
public:
    virtual void func() = 0;
};

class Derived : public Base
{
public:
    Derived()
    {
        _number = ++count;
    }

    virtual void func()
    {
        cout << "[" << _number << "] Derived::func()"<< endl;
    }

private:
    int _number;
    static int count;
};

int Derived::count = 0;

// wrapper for Base
struct BaseWrapper : Base, python::wrapper<Base>
{
    virtual void func()
    {
        this->get_override("func");
    }
};


Base* makeDerived()
{
    return new Derived;
}

vector<Base*>* makeDerivedVec()
{
    vector<Base*> *v = new vector<Base*>;
    v->push_back(new Derived);
    v->push_back(new Derived);
    v->push_back(new Derived);
    return v;
}

BOOST_PYTHON_MODULE(cap2re)
{
    using namespace python;
    // export Base
    class_<BaseWrapper, noncopyable>("Base", no_init)
            .def("func", pure_virtual(&Base::func));

    class_<vector<Base*> >("BasePtrVec")
            .def("__len__", &Vector2List<Base*>::len)
            .def("__getitem__", &Vector2List<Base*>::getitem, return_value_policy<reference_existing_object>())
            .def("__iter__", python::iterator<vector<Base*> >());

    // export Derived
    class_<Derived, bases<Base> >("Derived");

    // export makeDerived()
    def("makeDerived", &makeDerived, return_value_policy<manage_new_object>());

    // export makeDerivedVec()
    def("makeDerivedVec", &makeDerivedVec, return_value_policy<manage_new_object>());
}
