import os, sys


def link_pyside_cl(dest_path, *args):
    i = 0
    kwargs = {} 
    for arg in args:
        kwargs[arg] = True
    link_pyside(dest_path, **kwargs)


def link_pyside(*args, **kwargs):
    _link_pyside(False, *args, **kwargs)


def unlink_pyside_cl(dest_path, *args):
    i = 0
    kwargs = {} 
    for arg in args:
        kwargs[arg] = True
    unlink_pyside(dest_path, **kwargs)


def unlink_pyside(*args, **kwargs):
    _link_pyside(True, *args, **kwargs)


def _link_pyside(unlink, dest_path, replace_existing=False, reverse=False, sparse=False):
    import PySide2
    pyside_path = PySide2.__path__

    if sys.platform == 'win32':
        library_path = PySide2.__path__[0]
    else:
        library_path = os.path.join(PySide2.__path__[0], 'Qt', 'lib')

    if reverse:
        print(f'Linking PySide2 Qt libraries to {dest_path}')
    else:
        print(f'Linking {dest_path} Qt libraries to PySide2')

    if sys.platform == 'linux':
        library_extension = '.so.5'
    elif sys.platform == 'darwin':
        library_extension = '.framework'
    elif sys.platform == 'win32':
        library_extension = '.dll'

    libraries = [library for library in os.listdir(library_path) if library.endswith(library_extension)]

    for library in libraries:
        if sys.platform == 'darwin':
            library_subpath = f'Versions/5/{library.removesuffix(library_extension)}'
            pyside_framework = os.path.join(library_path, library)
            pyside_library = os.path.join(pyside_framework, library_subpath)
            dest_framework = os.path.join(dest_path, library)
            dest_library = os.path.join(dest_framework, library_subpath)
        else:
            pyside_library = os.path.join(library_path, library)
            dest_library =  os.path.join(dest_path, library)
            
        if os.path.isfile(pyside_library):
            if reverse:
                if unlink:
                    old_file = pyside_library + "_tmp"
                    if os.path.exists(old_file):
                        os.remove(pyside_library)
                        os.rename(old_file, pyside_library)
                        print(f'Unlinking {pyside_library}')
                elif os.path.isfile(dest_library) and not os.path.samefile(pyside_library, dest_library):
                    if replace_existing:
                        os.remove(pyside_library)
                    else:
                        os.rename(pyside_library, pyside_library + '_tmp')
                    os.symlink(dest_library, pyside_library)
                    print(f'Created link: {pyside_library} -> {dest_library}')
            else:
                if os.path.isfile(dest_library):
                    if unlink:
                        old_file = dest_library + "_tmp"
                        if os.path.exists(old_file):
                            os.remove(dest_library)
                            os.rename(old_file, dest_library)
                            print(f'Unlinking {dest_library}')
                    elif not os.path.samefile(pyside_library, dest_library):
                        if replace_existing:
                            os.remove(dest_library)
                        else:
                            os.rename(dest_library, dest_library + '_tmp')
                        os.symlink(pyside_library, dest_library)
                        print(f'Created link: {dest_library} -> {pyside_library}')
                elif not unlink and not sparse:
                    if sys.platform == 'darwin':
                        os.makedirs(os.path.dirname(dest_library))
                    os.symlink(pyside_library, dest_library)
                    print(f'Created link: {dest_library} -> {pyside_library}')
