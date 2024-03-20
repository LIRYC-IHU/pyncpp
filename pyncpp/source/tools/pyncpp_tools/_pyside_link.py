# Copyright (c) 2024 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause


import os, sys


def temporarily_link_pyside_to_qt(qt_library_path):
    print(f'Linking PySide2 to Qt libraries in {qt_library_path}')
    pyside_library_path = _get_pyside_library_path()
    pyside_libraries, qt_libraries = _get_libraries(pyside_library_path, qt_library_path)

    for pyside_library, qt_library in zip(pyside_libraries, qt_libraries):
        if os.path.exists(qt_library):
            tmp_file = _temporary_file(pyside_library)
            if not os.path.exists(tmp_file):
                os.rename(pyside_library, tmp_file)
                os.symlink(qt_library, pyside_library)
                print(f'Created link: {pyside_library} -> {qt_library}')


def unlink_pyside_from_qt():
    print(f'Unlinking PySide2 libraries from Qt')
    pyside_library_path = _get_pyside_library_path()
    pyside_libraries, _ = _get_libraries(pyside_library_path)

    for pyside_library in pyside_libraries:
        tmp_file = _temporary_file(pyside_library)
        if os.path.exists(tmp_file):
            os.remove(pyside_library)
            os.rename(tmp_file, pyside_library)
            print(f'Unlinked: {pyside_library}')


def link_qt_to_pyside(qt_library_path):
    print(f'Linking Qt libraries in {qt_library_path} to PySide2')
    pyside_library_path = _get_pyside_library_path()
    pyside_libraries, qt_libraries = _get_libraries(pyside_library_path, qt_library_path)

    for pyside_library, qt_library in zip(pyside_libraries, qt_libraries):
        if os.path.exsts(qt_library):
            tmp_file = _temporary_file(pyside_library)
            if os.path.exists(tmp_file):
                os.remove(pyside_library)
                os.rename(tmp_file, pyside_library)
            os.remove(qt_library)
            os.symlink(pyside_library, qt_library)
            print(f'Created link: {pyside_library} -> {qt_library}')


def _get_pyside_library_path():
    import PySide2
    pyside_path = PySide2.__path__

    if sys.platform == 'win32':
        library_path = PySide2.__path__[0]
    else:
        library_path = os.path.join(PySide2.__path__[0], 'Qt', 'lib')

    return library_path


def _get_libraries(pyside_library_path, qt_library_path=None):
    extension = _library_or_framework_extension()
    libraries = [library for library in os.listdir(pyside_library_path) if library.endswith(extension)]

    if sys.platform == 'darwin':
        libraries = [os.path.join(library, f'Versions/5/{library.removesuffix(".framework")}') for library in libraries]

    pyside_libraries = [os.path.join(pyside_library_path, library) for library in libraries]
    qt_libraries = [os.path.join(qt_library_path, library) for library in libraries] if qt_library_path else None

    return pyside_libraries, qt_libraries


def _library_or_framework_extension():
    if sys.platform == 'linux':
        extension = '.so.5'
    elif sys.platform == 'darwin':
        extension = '.framework'
    elif sys.platform == 'win32':
        extension = '.dll'

    return extension


def _temporary_file(file):
    return file + '_tmp'
