// Copyright (c) 2022-2023 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#ifndef PYNCPP_ABSTRACT_OBJECT_H
#define PYNCPP_ABSTRACT_OBJECT_H

/// This is the main class for the Python wrapping API. The goal of the API is
/// to automate reference counting, error checking and global interpreter
/// locking, and to provide a simplified way to use Python from C++. Most of the
/// functions in this class are direct wrappers of identically-named Python
/// functions and it is best to consult the Python documentation for their use.
/// Any Python function not wrapped here can be called using callMethod.
///
/// IMPORTANT: Instances of this class (and derived) are intended to be handled
/// by value.
///

#include "../external/cpython.h"

#include <vector>

#include "../conversion.h"
#include "../error.h"
#include "../export.h"
#include "../external/cpython_call.h"

namespace pyncpp
{

class AttributeAccessor;
class ContainerAccessor;
class FunctionCall;
class Object;

class PYNCPP_EXPORT AbstractObject
{
public:
    /// Replaces the wrapped object with the one wrapped by 'other'.
    ///
    virtual AbstractObject& operator=(const AbstractObject& other);

    /// Replaces the wrapped object with 'reference'. This function takes
    /// ownership of 'reference' (it does not increase the reference count). If
    /// 'reference' is null then the Python 'None' object is wrapped instead.
    ///
    virtual AbstractObject& operator=(PyObject* reference);

    /// Replaces the wrapped object with 'reference'. This function does not
    /// take ownership of 'reference' (it increases the reference count). If
    /// 'reference' is null then the Python 'None' object is wrapped instead.
    ///
    void borrow(const PyObject* reference);

    /// Returns the wrapped Python reference without increasing the reference
    /// count.
    ///
    PyObject* operator*() const;

    /// Returns the wrapped Python reference with an increased reference count
    /// (the caller takes ownership of the returned reference).
    ///
    PyObject* newReference() const;

    Object type() const;
    const char* typeName() const;

    bool hasAttribute(const char* name) const;
    AttributeAccessor attribute(const char* name);

    #if PYNCPP_SWIG_SUPPORT
    /// Equivalent to the Python expression 'dir(o)'
    ///
    QList<QString> dir() const;
    #endif // PYNCPP_SWIG_SUPPORT

    operator bool() const;
    bool operator!() const;

    bool operator==(const AbstractObject& other) const;
    bool operator!=(const AbstractObject& other) const;
    bool operator<(const AbstractObject& other) const;
    bool operator>(const AbstractObject& other) const;
    bool operator<=(const AbstractObject& other) const;
    bool operator>=(const AbstractObject& other) const;

    Object operator+(const AbstractObject& other) const;
    Object operator-(const AbstractObject& other) const;
    Object operator*(const AbstractObject& other) const;
    Object operator/(const AbstractObject& other) const;

    AbstractObject& operator+=(const AbstractObject& other);
    AbstractObject& operator-=(const AbstractObject& other);
    AbstractObject& operator*=(const AbstractObject& other);
    AbstractObject& operator/=(const AbstractObject& other);

    /// Equivalent to the Python expression 'len(o)'
    ///
    Py_ssize_t length() const;

    /// Equivalent to the Python expression 'o1 in o2'
    ///
    bool contains(const AbstractObject& object) const;

    void append(const AbstractObject& item);

    Object keys() const;
    Object values() const;

    void update(const AbstractObject& other);

    bool isInstance(const AbstractObject& type) const;

    bool isSubClass(const AbstractObject& type) const;

    /// Converts the Python object to a C++ type using the associated overload
    /// of pyncppToCPP. The caller owns the returned object.
    ///
    template <class TYPE>
    TYPE toCPP() const;

//    /// If this object is a SWIG-wrapped C++ object, returns a pointer to the
//    /// object. The caller does not own the pointer.
//    ///
//    template <class TYPE>
//    TYPE cppPointer() const;

//    void* cppPointer() const;

    // Due to ciruclar dependency issues, the following three template functions
    // are defined in the header file of their respective return types.

    template <class ARG>
    ContainerAccessor operator[](const ARG& key);

    template <class... ARGS>
    FunctionCall operator()(ARGS... args) const;

    template <class... ARGS>
    FunctionCall callMethod(const char* name, ARGS... args);

protected:
    virtual PyObject* getReference() const = 0;
    virtual void setReference(PyObject* reference) = 0;

    void unsupportedFunctionError(const char* functionName) const;
};

template <class TYPE>
TYPE AbstractObject::toCPP() const
{
    TYPE result;
    PYNCPP_ACQUIRE_GIL;

    if (!pyncppToCPP(**this, result))
    {
        propagateCurrentError(PYNCPP_GIL_STATE);
    }

    PYNCPP_RELEASE_GIL;
    return result;
}

//template <class TYPE>
//TYPE AbstractObject::cppPointer() const
//{
//    return dynamic_cast<TYPE>(cppPointer());
//}

} // namespace pyncpp

#endif // PYNCPP_ABSTRACT_OBJECT_H
