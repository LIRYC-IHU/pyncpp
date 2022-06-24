%define %qhash_typemaps(KEY_TYPE, VALUE_TYPE, PRECEDENCE)

    %feature("novaluewrapper") QHash<KEY_TYPE, VALUE_TYPE >;
    class QHash<KEY_TYPE, VALUE_TYPE >;

    %typecheck(PRECEDENCE) QHash<KEY_TYPE, VALUE_TYPE >
    {
        $1 = PyMapping_Check($input) ? 1 : 0;
    }

    %typemap(in) QHash<KEY_TYPE, VALUE_TYPE >
    {
        pyncppConvert<KEY_TYPE, VALUE_TYPE >($input, &$1);
        PYNCPP_CHECK_ERROR;
    }

    %typemap(directorout) QHash<KEY_TYPE, VALUE_TYPE >
    {
        pyncppConvert<KEY_TYPE, VALUE_TYPE >($input, &$result);
        PYNCPP_CHECK_ERROR;
    }

    %typemap(out) QHash<KEY_TYPE, VALUE_TYPE >
    {
        pyncppConvert<KEY_TYPE, VALUE_TYPE >($1, &$result);
        PYNCPP_CHECK_ERROR;
    }

    %typemap(directorin) QHash<KEY_TYPE, VALUE_TYPE >
    {
        pyncppConvert<KEY_TYPE, VALUE_TYPE >($1, $input);
        PYNCPP_CHECK_ERROR;
    }

    %typemap(argout) QHash<KEY_TYPE, VALUE_TYPE >* OUTPUT
    {
        PyObject* output;
        pyncppConvert<KEY_TYPE, VALUE_TYPE >(*$1, &output);
        $result = SWIG_Python_AppendOutput($result, output);
    }

%enddef

%qhash_typemaps(QString, QString, SWIG_TYPECHECK_MAP)
