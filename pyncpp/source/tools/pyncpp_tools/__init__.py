# Copyright (c) 2024 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause


from .global_values import *


if pyncpp_qt5_support:
    from ._qt_console import Console
