// Copyright (c) 2022-2023 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#ifndef PYNCPP_OBJECT_H
#define PYNCPP_OBJECT_H

/// This is the wrapper class for concrete Python objects. See the
/// AbstractObject header for more information.
///

#include "../external/cpython.h"

#if PYNCPP_QT5_SUPPORT
#include <QString>
#endif // PYNCPP_QT5_SUPPORT

#include "../export.h"
#include "../external/cpython_call.h"

#include "abstract_object.h"

namespace pyncpp
{

struct ObjectPrivate;

class PYNCPP_EXPORT Object : public AbstractObject
{
public:
    /// Converts a C++ object to Python using the associated overload of the
    /// 'pyncppToPython' function.
    ///
    template <class TYPE>
    static Object create(const TYPE& value);

    /// Wraps a Python object and increase its reference count (the caller keeps
    /// ownership of 'reference').
    ///
    static Object borrowed(const PyObject* reference);

    /// Wraps a Python object without increasing its reference count (the caller
    /// loses ownership of 'reference'). If the reference is null then the
    /// Python 'None' object is wrapped instead.
    ///
    Object(PyObject* reference = nullptr);

    /// Wraps the same Python object as the one wrapped by 'other'.
    ///
    Object(const Object& other);

    /// Wraps the same Python object as the one wrapped by 'other'.
    ///
    Object(const AbstractObject& other);

    /// Creates a wrapped Python integer.
    ///
    Object(int value);

    /// Creates a wrapped Python float.
    ///
    Object(double value);

    /// Creates a wrapped Python string.
    ///
    Object(const char* value);

#if PYNCPP_SWIG_SUPPORT
    /// Creates a wrapped Python string.
    ///
    Object(QString value);
#endif // PYNCPP_SWIG_SUPPORT

    virtual ~Object();

    /// Replaces the wrapped object with the one wrapped by 'other'.
    ///
    virtual Object& operator=(const Object& other);

    using AbstractObject::operator=;

protected:
    ObjectPrivate* const d;

    PyObject* getReference() const override;
    void setReference(PyObject* reference) override;

private:
    void internalSetReference(PyObject* reference);
};

template <class TYPE>
Object Object::create(const TYPE& value)
{
    PyObject* reference;
    PYNCPP_ACQUIRE_GIL;

    if (!pyncppToPython(value, &reference))
    {
        propagateCurrentError(PYNCPP_GIL_STATE);
    }

    PYNCPP_RELEASE_GIL;
    return reference;
}

} // namespace pyncpp

#endif // PYNCPP_OBJECT_H
