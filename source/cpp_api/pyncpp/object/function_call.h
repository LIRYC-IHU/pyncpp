// Copyright (c) 2022, 2024 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#ifndef PYNCPP_FUNCTION_CALL_H
#define PYNCPP_FUNCTION_CALL_H

/// This class represents a function call. The AbstractObject it implements is
/// the return value of the call. The actual call is only triggered when the
/// return value is needed (by any function that uses the value of the
/// object or when assigning this object to another), or, if the return value is
/// not used or assigned, when the instance goes out of scope. The 'kwargs'
/// function can be used to add keyword arguments to the call. For example, the
/// following Python code:
///
///    employee = db.getEmployee("John", "Doe", branch="marketing", category=2)
///
/// would be written in C++ as:
///
///    Object employee = db.callMethod("getEmployee", "John", "Doe")
///                                   .kwarg("branch", "marketing")
///                                   .kwarg("category", 2);
///

#include "../external/cpython.h"

#include <vector>

#include "../export.h"

#include "attribute_accessor.h"
#include "object.h"
#include "standard_objects.h"

namespace pyncpp
{

struct FunctionCallPrivate;

class PYNCPP_EXPORT FunctionCall : public AbstractObject
{
public:
    FunctionCall(const AbstractObject& callable, const AbstractObject& args);
    virtual ~FunctionCall() noexcept(false);

    /// Add a keyword argument to the call
    ///
    FunctionCall& kwarg(const char* name, const AbstractObject& value);

    /// Add a keyword argument to the call. The type must be eligible to the
    /// 'Object::create' function.
    ///
    template <class TYPE, typename = std::enable_if_t<!std::is_base_of_v<AbstractObject, TYPE> > >
    FunctionCall& kwarg(const char* name, const TYPE& value);

protected:
    PyObject* getReference() const override;
    void setReference(PyObject* reference) override;

private:
    FunctionCallPrivate* const d;

    PyObject* evaluate() const;
};

template <class TYPE, typename>
FunctionCall& FunctionCall::kwarg(const char* name, const TYPE& value)
{
    return kwarg(name, Object(value));
}

// The following AbstractObject template functions are defined here to avoid
// circular dependency issues:

template <class... ARGS>
FunctionCall AbstractObject::operator()(ARGS... args) const
{
    std::list<Object> argsList;
    (argsList.push_back(Object(args)), ...); // C++ fold expression
    return FunctionCall(*this, tuple(argsList));
}

template <class... ARGS>
FunctionCall AbstractObject::callMethod(const char* name, ARGS... args)
{
    return attribute(name).operator()(args...);
}

} // namespace pyncpp

#endif // PYNCPP_FUNCTION_CALL_H
