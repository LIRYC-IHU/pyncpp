// Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#include "attribute_accessor.h"

#include <string>

#include "../external/cpython_call.h"

namespace pyncpp
{

struct AttributeAccessorPrivate
{
    Object object;
    std::string attribute;
};

AttributeAccessor::AttributeAccessor(const AbstractObject& object, const char* attribute) :
    d(new AttributeAccessorPrivate)
{
    d->object = object;
    d->attribute = attribute;
}

AttributeAccessor::AttributeAccessor(const Object& object, const char* attribute) :
    d(new AttributeAccessorPrivate)
{
    d->object = object;
    d->attribute = attribute;
}

PyObject* AttributeAccessor::getReference() const
{
    return cpythonCall(PyObject_GetAttrString, *d->object, d->attribute.c_str());
}

void AttributeAccessor::setReference(PyObject* reference)
{
    cpythonCall(PyObject_SetAttrString, *d->object, d->attribute.c_str(), reference);
}

} // namespace pyncpp
