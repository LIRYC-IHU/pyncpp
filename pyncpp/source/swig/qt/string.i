%typemap(typecheck) QString = char *;

%pyncpp_typemaps(QString);

%apply QString { const QString };

%typemap(typecheck) const QString& = char *;

%typemap(in) const QString& (QString temp)
{
    PYNCPP_ACQUIRE_GIL;
    pyncppToCPP($input, temp);
    PYNCPP_CHECK_ERROR;
    $1 = &temp;
    PYNCPP_RELEASE_GIL;
}

%typemap(directorout) const QString& (QString temp)
{
    PYNCPP_ACQUIRE_GIL;
    pyncppToCPP($input, temp);
    PYNCPP_CHECK_ERROR;
    $result = temp;
    PYNCPP_RELEASE_GIL;
}

%typemap(out) const QString&
{
    PYNCPP_ACQUIRE_GIL;
    pyncppToPython(*$1, &$result);
    PYNCPP_CHECK_ERROR;
    PYNCPP_RELEASE_GIL;
}

%typemap(directorin) const QString& (PyObject* temp)
{
    PYNCPP_ACQUIRE_GIL;
    pyncppToPython($1, &temp);
    PYNCPP_CHECK_ERROR;
    $input = temp;
    PYNCPP_RELEASE_GIL;
}
