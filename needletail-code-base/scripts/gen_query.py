#!/usr/bin/env python2
"""
Every query will be a nested dict/list combination. The top level will have two
attributes - 'num_samples' and 'q'. 'q' will represent a dictionary of things
to filter for. Every node in 'q' is represnted by a dict. There are 2 kinds of
dicts possible for 'q'. If the '__op__' key exists, the dict is an combinator
node. The type of combination is given by the value of '__op__' and the nodes
to be combined are given by the '__nodes__' key. If the '__op__' key does not
exist, then it is a singleton dict whose key corresponds to an attribute in the
sdb and attribute value represents the attribute value we want to filter for.
"""

import os
import sys

import yaml

q0 = {
        'num_samples': 10,
        'targets': ['c0'],
        'q': {
            'd': 0
            }
        }

q1 = {
        'num_samples': 10,
        'targets': ['c0'],
        'q': {
            '__op__': 'and',
            '__nodes__': 
                {'d': 0}
            }
        }

q2 = {
        'num_samples': 10,
        'targets': ['c0'],
        'q': {
            '__op__': 'and',
            '__nodes__': 
                {'d': 0}
            }
        }

q4 = {
        'num_samples': 10,
        'targets': ['c0'],
        'q': {
            '__op__': 'and',
            '__nodes__': 
                {'d': 0}
            }
        }
# (OR (AND d0=0 d1=1) d2=2 (AND d3=3 d4=4))
# ((d0 = 0 AND d1 = 1) OR d2 = 2 OR (d3 = 3 AND d4 = 4))
q3 = {
        'num_samples': 10,
        'targets': ['c0'],
        'q': {
            '__op__': 'or',
            '__nodes__': [
                {
                    '__op__': 'and',
                    '__nodes__': [
                        {'d0': 0},
                        {'d1': 1},
                        ]
                    },
                {'d2': 2},
                {
                    '__op__': 'and',
                    '__nodes__': [
                        {'d3': 3},
                        {'d4': 4},
                        ]
                    },
                ]
            }
        }



def main():
    '''
    if len(sys.argv) != 2:
        print 'Usage: %s <dirpath>' % sys.argv[0]
        sys.exit(2)

    dirpath = sys.argv[1]

    for i in range(4):
        qname = 'q' + str(i)
        q = getattr(sys.modules[__name__], qname)
        f = open(os.path.join(dirpath, qname), 'w')
        f.write(yaml.dump(q))
        f.close()
    '''
    f = open('/home/srahman7/incvisage-cpp/needletail-code-base/scripts/qt2', 'w')

    
    f.write(yaml.dump(q1))
    f.close()

    f = open('/home/srahman7/incvisage-cpp/needletail-code-base/scripts/qt1', 'w')

    

    f.write(yaml.dump(q0))
    f.close()

    f = open('/home/srahman7/incvisage-cpp/needletail-code-base/scripts/qh2', 'w')

    
    f.write(yaml.dump(q4))
    f.close()

    f = open('/home/srahman7/incvisage-cpp/needletail-code-base/scripts/qh1', 'w')

    

    f.write(yaml.dump(q1))
    f.close()


if __name__ == '__main__':
    main()
