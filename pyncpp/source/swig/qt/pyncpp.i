#pragma SWIG nowarn=503,509,602,325,312,314,317,362,361

%ignore operator=;
%ignore operator>>;
%ignore operator<<;
%ignore operator==;
%ignore operator[];
%ignore operator!=;
%ignore operator*=;
%ignore operator/=;
%ignore operator bool;
%ignore operator int;
%ignore operator float;
%ignore operator double;
%ignore operator double*;

#define decltype(x) int

#undef DTKCORESUPPORT_EXPORT
#define DTKCORESUPPORT_EXPORT

#undef QDOC_PROPERTY
#define QDOC_PROPERTY(x)

#undef QT_WARNING_DISABLE_GCC
#define QT_WARNING_DISABLE_GCC(x)

#undef QT_WARNING_POP
#define QT_WARNING_POP

#undef QT_WARNING_PUSH
#define QT_WARNING_PUSH

#undef Q_ALLOC_SIZE
#define Q_ALLOC_SIZE(x)

#undef Q_DECLARE_METATYPE
#define Q_DECLARE_METATYPE(x)

#undef Q_DECLARE_SHARED
#define Q_DECLARE_SHARED(x)

#undef Q_DECL_COLD_FUNCTION
#define Q_DECL_COLD_FUNCTION

#undef Q_DECL_CONSTEXPR
#define Q_DECL_CONSTEXPR

#undef Q_DECL_CONST_FUNCTION
#define Q_DECL_CONST_FUNCTION

#undef Q_DECL_EQ_DELETE
#define Q_DECL_EQ_DELETE

#undef Q_DECL_IMPORT
#define Q_DECL_IMPORT

#undef Q_DECL_NOEXCEPT
#define Q_DECL_NOEXCEPT

#undef Q_DECL_NOTHROW
#define Q_DECL_NOTHROW

#undef Q_DECL_OVERRIDE
#define Q_DECL_OVERRIDE

#undef Q_DECL_PURE_FUNCTION
#define Q_DECL_PURE_FUNCTION

#undef Q_DECL_RELAXED_CONSTEXPR
#define Q_DECL_RELAXED_CONSTEXPR

#undef Q_DECL_NOEXCEPT_EXPR
#define Q_DECL_NOEXCEPT_EXPR(x)

#undef Q_DECL_UNUSED
#define Q_DECL_UNUSED

#undef Q_ENUM
#define Q_ENUM(x)

#undef Q_FLAG
#define Q_FLAG(x)

#undef Q_GADGET
#define Q_GADGET

#undef Q_INVOKABLE
#define Q_INVOKABLE

#undef Q_NORETURN
#define Q_NORETURN

#undef Q_OBJECT
#define Q_OBJECT \
    public: \
        static const QMetaObject staticMetaObject; \
        virtual const QMetaObject *metaObject() const;

#undef Q_PRIVATE_SLOT
#define Q_PRIVATE_SLOT(...)

#undef Q_PROPERTY
#define Q_PROPERTY(...)

#undef Q_REQUIRED_RESULT
#define Q_REQUIRED_RESULT

#undef Q_SIGNALS
#define Q_SIGNALS public

#undef Q_SLOTS
#define Q_SLOTS

#define slots
#define signals private

%{
    #include <QObject>
    #include <QMetaObject>
    #include "pyncpp.h"

    template <class SENDER_TYPE>
    void connect_noargs(SENDER_TYPE* sender, void (SENDER_TYPE::*signal)(), PyObject* receiver)
    {
        pyncpp::Object object = pyncpp::Object::borrowed(receiver);
        sender->connect(sender, signal, [=]() { object();});
    }

    template <class SENDER_TYPE, class... ARGS>
    void connect_args(SENDER_TYPE* sender, void (SENDER_TYPE::*signal)(ARGS...), PyObject* receiver)
    {
        pyncpp::Object object = pyncpp::Object::borrowed(receiver);
        sender->connect(sender, signal, [=](ARGS... args) { object(args...); });
    }
%}

%feature("director:except")
{
    if ($error != nullptr)
    {
        PYNCPP_ACQUIRE_GIL;
        PYNCPP_THROW_ERROR;
        PYNCPP_RELEASE_GIL;
    }
}

%exception
{
    try
    {
        $action
    }
    catch (pyncpp::Exception& e)
    {
        //pyncpp::raiseError(e.nativeClass(), e.what());
        SWIG_fail;
    }
}

%define %pyncpp_typemaps(TYPE)

    %typemap(in) TYPE
    {
        PYNCPP_ACQUIRE_GIL;
        pyncppToCPP($input, $1);
        PYNCPP_CHECK_ERROR;
        PYNCPP_RELEASE_GIL;
    }

    %typemap(directorout) TYPE
    {
        PYNCPP_ACQUIRE_GIL;
        pyncppToCPP($input, $result);
        PYNCPP_CHECK_ERROR;
        PYNCPP_RELEASE_GIL;
    }

    %typemap(out) TYPE
    {
        PYNCPP_ACQUIRE_GIL;
        pyncppToPython($1, &$result);
        PYNCPP_CHECK_ERROR;
        PYNCPP_RELEASE_GIL;
    }

    %typemap(directorin) TYPE (PyObject* temp)
    {
        PYNCPP_ACQUIRE_GIL;
        pyncppToPython($1, &temp);
        PYNCPP_CHECK_ERROR;
        $input = temp;
        PYNCPP_RELEASE_GIL;
    }

    %typemap(in, numinputs = 0) TYPE* OUTPUT (TYPE temp)
    {
        $1 = &temp;
    }

    %typemap(argout) TYPE* OUTPUT
    {
        PyObject* output;
        pyncppToPython(*$1, &output);
        $result = SWIG_Python_AppendOutput($result, output);
    }

%enddef
