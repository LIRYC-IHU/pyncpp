%feature("unref") QObject "if(!$this->parent()) delete $this;";
%feature("director") QObject;

struct QMetaObject
{
    const char* className() const;
    const QMetaObject* superClass() const;
    bool inherits(const QMetaObject* metaObject) const;
};

%define SIGNAL_NOARGS(cls, signal)

%{
template void connect_noargs(cls*, void (cls ## :: ## *)(), PyObject*);
%}

%extend cls
{
    void connect_noargs_ ## signal(PyObject* receiver)
    {
        connect_noargs($self, & ## cls ## :: ## signal, receiver);
    }
}

%enddef

%define SIGNAL(cls, signal, ...)

%{
    template void connect_args(cls*, void (cls ## :: ## *)(__VA_ARGS__), PyObject*);
%}

%extend cls
{
    void connect_args_ ## signal(PyObject* receiver)
    {
        connect_args($self, & ## cls ## :: ## signal, receiver);
    }
}

%enddef

%rename(parent) QObject::getParent;

class QObject
{
Q_OBJECT

public:
    virtual const QMetaObject* metaObject() const;
    void deleteLater();
};

%pyncpp_typemaps(QObject*);

%extend QObject
{
    PyObject* _parent()
    {
        PYNCPP_ACQUIRE_GIL;
        PyObject* result = pyncpp::wrapObjectWithSWIG($self->parent());
        PYNCPP_CHECK_ERROR;
        PYNCPP_RELEASE_GIL;
        return result;
    }

    QVariant getProperty(QString name)
    {
        return $self->property(qUtf8Printable(name));
    }

    void connectOldStyle(QString signal, QObject* receiver, QString slot)
    {
        $self->connect($self, qUtf8Printable(signal.prepend("2")), receiver, qUtf8Printable(slot.prepend("1")));
    }

    %pythoncode
    %{
        def connect(self, signal, receiver, slot=None):
            if slot:
                self.connectOldStyle(signal, receiver, slot)
            else:
                connect = getattr(self, f'connect_args_{signal}', None)
                if not connect:
                    connect = getattr(self, f'connect_noargs_{signal}', None)
                if not connect:
                    raise AttributeError(f'\'{self.__class__.__name__}\' object has no signal \'{signal}\'')
                connect(receiver)

        def parent(self):
            return self._parent()
    %}
}

SIGNAL(QObject, destroyed, QObject*)
