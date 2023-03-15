// Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#ifndef PYNCPP_ATTRIBUTE_ACCESSOR_H
#define PYNCPP_ATTRIBUTE_ACCESSOR_H

/// This class represents an object attribute. The AbstractObject it implements
/// is the attribute value.
///

#include "../external/cpython.h"

#include "../export.h"

#include "object.h"

namespace pyncpp
{

struct AttributeAccessorPrivate;

class PYNCPP_EXPORT AttributeAccessor : public AbstractObject
{
public:
    AttributeAccessor(const AbstractObject& object, const char* attribute);
    AttributeAccessor(const Object& object, const char* attribute);

    using AbstractObject::operator=;

protected:
    PyObject* getReference() const override;
    void setReference(PyObject* reference) override;

private:
    AttributeAccessorPrivate* const d;
};

} // namespace pyncpp

#endif // PYNCPP_ATTRIBUTE_ACCESSOR_H
