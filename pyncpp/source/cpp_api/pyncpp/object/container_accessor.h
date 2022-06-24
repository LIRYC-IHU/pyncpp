// Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#ifndef PYNCPP_CONTAINER_ACCESSOR_H
#define PYNCPP_CONTAINER_ACCESSOR_H

/// This class represents a container location mapped by a specific key. The
/// AbstractObject it implements is the contained object. It allows the use of
/// the subscript operator such as:
///
///     Object foo = bar["baz"];
///
/// or:
///
///     bar[i] += Object(2);
///

#include "../external/cpython.h"

#include "../export.h"
#include "../external/cpython_call.h"

#include "object.h"

namespace pyncpp
{

struct ContainerAccessorPrivate;

class PYNCPP_EXPORT ContainerAccessor : public AbstractObject
{
public:
    ContainerAccessor(const AbstractObject& container, PyObject* key);

    using AbstractObject::operator=;

protected:
    PyObject* getReference() const override;
    void setReference(PyObject* reference) override;

private:
    ContainerAccessorPrivate* const d;
};

// The following AbstractObject template function is defined here to avoid
// circular dependency issues:

template <class ARG>
ContainerAccessor AbstractObject::operator[](const ARG& key)
{
    PYNCPP_ACQUIRE_GIL;

    PyObject* pythonKey;
    pyncppToPython(key, &pythonKey);
    PYNCPP_CHECK_ERROR;

    PYNCPP_RELEASE_GIL;
    return ContainerAccessor(*this, pythonKey);
}

} // namespace pyncpp

#endif // PYNCPP_CONTAINER_ACCESSOR_H
