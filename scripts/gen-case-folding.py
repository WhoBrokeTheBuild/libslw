
import os
import requests

# Move to repository root
os.chdir(os.path.dirname(os.path.dirname(__file__)))

URL = 'https://www.unicode.org/Public/17.0.0/ucd/CaseFolding.txt'
response = requests.get(URL)
lines = response.content.decode().splitlines()

file = open('Public/slw/unicode.inc.hpp', 'wt')

for line in lines:
    line = line.strip()
    if len(line) == 0 or line[0] == '#':
        continue

    code, status, mapping, name = line.split(';')
    code = int(code.strip(), 16)
    status = status.strip()
    mapping = [ int(value, 16) for value in mapping.strip().split() ]
    name = name.removeprefix(' # ')

    if status not in ['C', 'S']:
        continue

    file.write('    {{ 0x{:04X}, 0x{:04X} }}, // {}\n'.format(code, mapping[0], name))

    # if status not in ['C', 'F']:
    #     continue

    # file.write('    {{ 0x{:04X}, {{ 0x{:04X}, 0x{:04X}, 0x{:04X} }} }}, // {}\n'.format(code, mapping[0], mapping[1], mapping[2], name))
