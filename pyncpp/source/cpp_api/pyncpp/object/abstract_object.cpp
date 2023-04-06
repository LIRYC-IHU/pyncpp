// Copyright (c) 2022-2023 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#include "abstract_object.h"

#include "attribute_accessor.h"
#include "function_call.h"
#include "object.h"

namespace pyncpp
{

AbstractObject& AbstractObject::operator=(const AbstractObject& other)
{
    setReference(other.newReference());
    return *this;
}

AbstractObject& AbstractObject::operator=(PyObject* reference)
{
    if (!reference)
    {
        PYNCPP_ACQUIRE_GIL;
        reference = Py_NewRef(Py_None);
        PYNCPP_RELEASE_GIL;
    }

    setReference(reference);
    return *this;
}

void AbstractObject::borrow(const PyObject* reference)
{
    if (!reference)
    {
        reference = Py_None;
    }

    PYNCPP_ACQUIRE_GIL;
    Py_INCREF(reference);
    PYNCPP_RELEASE_GIL;

    setReference(const_cast<PyObject*>(reference));
}

PyObject* AbstractObject::operator*() const
{
    return getReference();
}

PyObject* AbstractObject::newReference() const
{
    PyObject* reference = getReference();

    PYNCPP_ACQUIRE_GIL;
    Py_INCREF(reference);
    PYNCPP_RELEASE_GIL;

    return reference;
}

Object AbstractObject::type() const
{
    return cpythonCall(PyObject_Type, **this);
}

const char* AbstractObject::typeName() const
{
    return Py_TYPE(**this)->tp_name;
}

bool AbstractObject::hasAttribute(const char* name) const
{
    return cpythonCall(PyObject_HasAttrString, **this, name);
}

AttributeAccessor AbstractObject::attribute(const char* name)
{
    return AttributeAccessor(*this, name);
}

QList<QString> AbstractObject::dir() const
{
    Object dir = cpythonCall(PyObject_Dir, **this);
    return dir.toCPP<QList<QString> >();
}

AbstractObject::operator bool() const
{
    return cpythonCall(PyObject_IsTrue, **this);
}

bool AbstractObject::operator!() const
{
    return cpythonCall(PyObject_Not, **this);
}

bool AbstractObject::operator==(const AbstractObject& other) const
{
    return cpythonCall(PyObject_RichCompareBool, **this, *other, Py_EQ);
}

bool AbstractObject::operator!=(const AbstractObject& other) const
{
    return cpythonCall(PyObject_RichCompareBool, **this, *other, Py_NE);
}

bool AbstractObject::operator<(const AbstractObject& other) const
{
    return cpythonCall(PyObject_RichCompareBool, **this, *other, Py_LT);
}

bool AbstractObject::operator>(const AbstractObject& other) const
{
    return cpythonCall(PyObject_RichCompareBool, **this, *other, Py_GT);
}

bool AbstractObject::operator<=(const AbstractObject& other) const
{
    return cpythonCall(PyObject_RichCompareBool, **this, *other, Py_LE);
}

bool AbstractObject::operator>=(const AbstractObject& other) const
{
    return cpythonCall(PyObject_RichCompareBool, **this, *other, Py_GE);
}

Object AbstractObject::operator+(const AbstractObject& other) const
{
    return cpythonCall(PyNumber_Add, **this, *other);
}

Object AbstractObject::operator-(const AbstractObject& other) const
{
    return cpythonCall(PyNumber_Subtract, **this, *other);
}

Object AbstractObject::operator*(const AbstractObject& other) const
{
    return cpythonCall(PyNumber_Multiply, **this, *other);
}

Object AbstractObject::operator/(const AbstractObject& other) const
{
    return cpythonCall(PyNumber_TrueDivide, **this, *other);
}

AbstractObject& AbstractObject::operator+=(const AbstractObject& other)
{
    setReference(cpythonCall(PyNumber_InPlaceAdd, **this, *other));
    return *this;
}

AbstractObject& AbstractObject::operator-=(const AbstractObject& other)
{
    setReference(cpythonCall(PyNumber_InPlaceSubtract, **this, *other));
    return *this;
}

AbstractObject& AbstractObject::operator*=(const AbstractObject& other)
{
    setReference(cpythonCall(PyNumber_InPlaceMultiply, **this, *other));
    return *this;
}

AbstractObject& AbstractObject::operator/=(const AbstractObject& other)
{
    setReference(cpythonCall(PyNumber_InPlaceTrueDivide, **this, *other));
    return *this;
}

Py_ssize_t AbstractObject::length() const
{
    return cpythonCall(PyObject_Size, **this);
}

bool AbstractObject::contains(const AbstractObject& object) const
{
    bool result = false;
    PYNCPP_ACQUIRE_GIL;

    if (PyMapping_Check(**this))
    {
        result = PyMapping_HasKey(**this, *object);
    }
    else if (PySequence_Check(**this))
    {
        result = PySequence_Contains(**this, *object);
    }
    else
    {
        PYNCPP_RELEASE_GIL;
        unsupportedFunctionError(__func__);
    }

    PYNCPP_CHECK_ERROR;
    PYNCPP_RELEASE_GIL;
    return result;
}

void AbstractObject::append(const AbstractObject& item)
{
    PYNCPP_ACQUIRE_GIL;

    if (PyList_Check(**this))
    {
        PyList_Append(**this, *item);
        PYNCPP_CHECK_ERROR;
        PYNCPP_RELEASE_GIL;
    }
    else
    {
        PYNCPP_RELEASE_GIL;
        unsupportedFunctionError(__func__);
    }
}

Object AbstractObject::keys() const
{
    return cpythonCall(PyMapping_Keys, **this);
}

Object AbstractObject::values() const
{
    return cpythonCall(PyMapping_Values, **this);
}

void AbstractObject::update(const AbstractObject& other)
{
    PYNCPP_ACQUIRE_GIL;

    if (PyDict_Check(**this))
    {
        PyDict_Merge(**this, *other, 1);
        PYNCPP_CHECK_ERROR;
        PYNCPP_RELEASE_GIL;
    }
    else
    {
        PYNCPP_RELEASE_GIL;
        unsupportedFunctionError(__func__);
    }
}

bool AbstractObject::isInstance(const AbstractObject& type) const
{
    return cpythonCall(PyObject_IsInstance, **this, *type);
}

bool AbstractObject::isSubClass(const AbstractObject& type) const
{
    return cpythonCall(PyObject_IsSubclass, **this, *type);
}

void* AbstractObject::cppPointer() const
{
    void* result = nullptr;

    if (PyCapsule_CheckExact(**this))
    {
        result = cpythonCall(PyCapsule_GetPointer, **this, nullptr);
    }

    return result;
}

void AbstractObject::unsupportedFunctionError(const char* functionName) const
{
    std::string message = std::string(typeName())
                          + " does not support " + functionName + ".";
    throw TypeError(message.c_str());
}

} // namespace pyncpp
