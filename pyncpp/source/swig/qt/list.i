%define %qlist_typemaps(CTYPE, PRECEDENCE)

    %feature("novaluewrapper") QList<CTYPE >;
    class QList<CTYPE >;

    %typecheck(PRECEDENCE) QList<CTYPE >
    {
        $1 = PySequence_Check($input) ? 1 : 0;
    }

    %typemap(in) QList<CTYPE >
    {
        pyncppConvert<CTYPE >($input, &$1);
        PYNCPP_CHECK_ERROR;
    }

    %typemap(directorout) QList<CTYPE >
    {
        pyncppConvert<CTYPE >($input, &$result);
        PYNCPP_CHECK_ERROR;
    }

    %typemap(out) QList<CTYPE >
    {
        pyncppConvert<CTYPE >($1, &$result);
        PYNCPP_CHECK_ERROR;
    }

    %typemap(directorin) QList<CTYPE >
    {
        pyncppConvert<CTYPE >($1, $input);
        PYNCPP_CHECK_ERROR;
    }

    %typemap(argout) QList<CTYPE >* OUTPUT
    {
        PyObject* output;
        pyncppConvert<CTYPE >(*$1, &output);
        $result = SWIG_Python_AppendOutput($result, output);
    }

%enddef

%qlist_typemaps(bool, SWIG_TYPECHECK_BOOL_ARRAY)
%qlist_typemaps(long, SWIG_TYPECHECK_INT64_ARRAY)
%qlist_typemaps(double, SWIG_TYPECHECK_DOUBLE_ARRAY)
%qlist_typemaps(QString, SWIG_TYPECHECK_STRING_ARRAY)

%apply QList<QString> { QStringList };
