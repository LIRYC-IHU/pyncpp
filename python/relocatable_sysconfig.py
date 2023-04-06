# Copyright (c) 2023 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

import os, sys, sysconfig, re

config_module_name = sysconfig._get_sysconfigdata_name()
config_module = __import__(config_module_name, globals(), locals(), ['__file__'], 0)
config_module_path = config_module.__file__
tmp_config_module_path = f'{config_module_path}.tmp'

prefix = sys.argv[1]
relative_prefix = os.path.relpath(prefix, os.path.dirname(config_module_path))

with open(f'{config_module_path}.tmp', 'w') as tmp_config_file:
    tmp_config_file.write('import os\n')
    tmp_config_file.write(f'_prefix = os.path.realpath(f\'{{os.path.dirname(__file__)}}/{relative_prefix}\')\n\n')
    with open(config_module_path, 'r') as config_file:
        line = config_file.readline()
        while line:
            line = re.sub(fr'{prefix}', '{_prefix}', line)
            line = re.sub(r'(\'[^\']*\{_prefix\}[^\']*\')', r'f\1', line)
            tmp_config_file.write(line)
            line = config_file.readline()

os.replace(tmp_config_module_path, config_module_path)
