// Copyright (c) 2022-2023 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#include "object.h"

namespace pyncpp
{

struct ObjectPrivate
{
    PyObject* reference;
};

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

Object::Object(PyObject* reference) :
    d(new ObjectPrivate)
{
    internalSetReference(reference);
}

Object::Object(const Object& other) :
    d(new ObjectPrivate)
{
    internalSetReference(other.newReference());
}

Object::Object(const AbstractObject& other) :
    d(new ObjectPrivate)
{
    internalSetReference(other.newReference());
}

Object::Object(int value) :
    d(new ObjectPrivate)
{
    PyObject* reference;
    PYNCPP_ACQUIRE_GIL;

    if (!pyncppToPython(value, &reference))
    {
        propagateCurrentError(PYNCPP_GIL_STATE);
    }

    PYNCPP_RELEASE_GIL;
    internalSetReference(reference);
}

Object::Object(double value) :
    d(new ObjectPrivate)
{
    PyObject* reference;
    PYNCPP_ACQUIRE_GIL;

    if (!pyncppToPython(value, &reference))
    {
        propagateCurrentError(PYNCPP_GIL_STATE);
    }

    PYNCPP_RELEASE_GIL;
    internalSetReference(reference);
}

Object::Object(const char* value) :
    d(new ObjectPrivate)
{
    PyObject* reference;
    PYNCPP_ACQUIRE_GIL;

    if (!pyncppToPython(value, &reference))
    {
        propagateCurrentError(PYNCPP_GIL_STATE);
    }

    PYNCPP_RELEASE_GIL;
    internalSetReference(reference);
}

Object::Object(QString value) :
    d(new ObjectPrivate)
{
    PyObject* reference;
    PYNCPP_ACQUIRE_GIL;

    if (!pyncppToPython(value, &reference))
    {
        propagateCurrentError(PYNCPP_GIL_STATE);
    }

    PYNCPP_RELEASE_GIL;
    internalSetReference(reference);
}

Object::~Object()
{
    PYNCPP_ACQUIRE_GIL;

    if (Py_IsInitialized())
    {
        Py_CLEAR(d->reference);
    }

    PYNCPP_RELEASE_GIL;
    delete d;
}

Object& Object::operator=(const Object& other)
{
    return static_cast<Object&>(AbstractObject::operator=(other));
}

PyObject* Object::getReference() const
{
    return d->reference;
}

void Object::setReference(PyObject* reference)
{
    PyObject* oldReference = d->reference;
    internalSetReference(reference);

    PYNCPP_ACQUIRE_GIL;
    Py_CLEAR(oldReference);
    PYNCPP_RELEASE_GIL;
}

void Object::internalSetReference(PyObject* reference)
{
    if (reference)
    {
        d->reference = reference;
    }
    else
    {
        PYNCPP_ACQUIRE_GIL;
        d->reference = Py_NewRef(Py_None);
        PYNCPP_RELEASE_GIL;
    }
}

} // namespace pyncpp
