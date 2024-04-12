# Copyright (c) 2024 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause


import sys
from . import *


if pyncpp_qt5_support:
    from ._qt_console import run_standalone as run_console
    from ._pyside_link import link_qt_to_pyside, temporarily_link_pyside_to_qt, unlink_pyside_from_qt


function =  sys.argv[1]
args = sys.argv[2:]


functions = []


if pyncpp_qt5_support:
    functions += [
        'run_console',
        'temporarily_link_pyside_to_qt',
        'unlink_pyside_from_qt',
        'link_qt_to_pyside',
    ]


if function in functions:
    globals()[function](*args)
else:
    raise RuntimeError(f'Function {function} not part of {__package__} command line API.')
