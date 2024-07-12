// Copyright (c) 2022-2024 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#include "object.h"

namespace pyncpp
{

Object Object::borrowed(const PyObject* reference)
{
    if (!reference)
    {
        reference = Py_None;
    }

    PYNCPP_ACQUIRE_GIL;
    Py_INCREF(reference);
    PYNCPP_RELEASE_GIL;

    return const_cast<PyObject*>(reference);
}

Object::Object(PyObject* reference)
{
    internalSetReference(reference);
}

Object::Object(const Object& other)
{
    internalSetReference(other.newReference());
}

Object::Object(const AbstractObject& other)
{
    internalSetReference(other.newReference());
}

Object::~Object()
{
    PYNCPP_ACQUIRE_GIL;

    if (Py_IsInitialized())
    {
        Py_CLEAR(internalReference);
    }

    PYNCPP_RELEASE_GIL;
}

Object& Object::operator=(const Object& other)
{
    return static_cast<Object&>(AbstractObject::operator=(other));
}

PyObject* Object::getReference() const
{
    return internalReference;
}

void Object::setReference(PyObject* reference)
{
    PyObject* oldReference = internalReference;
    internalSetReference(reference);

    PYNCPP_ACQUIRE_GIL;
    Py_CLEAR(oldReference);
    PYNCPP_RELEASE_GIL;
}

void Object::internalSetReference(PyObject* reference)
{
    if (reference)
    {
        internalReference = reference;
    }
    else
    {
        PYNCPP_ACQUIRE_GIL;
        internalReference = Py_NewRef(Py_None);
        PYNCPP_RELEASE_GIL;
    }
}

} // namespace pyncpp
