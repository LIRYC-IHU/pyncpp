import sys
from . import *


function =  sys.argv[1]
args = sys.argv[2:]


functions = (
    'link_pyside',
    'unlink_pyside',
    )


if function in functions:
    globals()[function + '_cl'](*args)
else:
    print(f'Function {function} not part of {__package__} command line API.')
