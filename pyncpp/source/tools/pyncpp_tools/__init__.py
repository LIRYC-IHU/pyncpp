# Copyright (c) 2024 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause


from .global_values import *


if pyncpp_qt5_support:
    from ._pyside_link import link_qt_to_pyside, temporarily_link_pyside_to_qt, unlink_pyside_from_qt
