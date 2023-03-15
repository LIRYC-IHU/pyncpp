// Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#include "container_accessor.h"

namespace pyncpp
{

struct ContainerAccessorPrivate
{
    Object container;
    Object key;
    Object value;
};

ContainerAccessor::ContainerAccessor(const AbstractObject& container, PyObject* key) :
    d(new ContainerAccessorPrivate)
{
    d->container = container;
    d->key = key;
}

PyObject* ContainerAccessor::getReference() const
{
    d->value = cpythonCall(PyObject_GetItem, *d->container, *d->key);
    return *d->value;
}

void ContainerAccessor::setReference(PyObject* reference)
{
    d->value = reference;
    cpythonCall(PyObject_SetItem, *d->container, *d->key, *d->value);
}

} // namespace pyncpp
