#!/usr/bin/python

# This file is looking into the python tracing support.

#from __future__ import print_function

import sys
import threading
import logging
from testcov import testfact

logger = None
# Enable Logger
logging.basicConfig(level = logging.DEBUG)
logger = logging.getLogger(__name__)
logger.propagate = False

# Create file handler
handler = logging.FileHandler(str(__file__) + '_tracing.log')
handler.setLevel(logging.DEBUG)

# Create a logging format
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
handler.setFormatter(formatter)

# Add the handlers to the logger
logger.addHandler(handler)

def profilefunc(frame, event, arg):
    code = frame.f_code
    #print("%s:%d:%s:%s" %(code.co_filename, frame.f_lineno, code.co_name, event))
    # Logger doesn't work with multithreaded Python 2.x environment.
    # It seems to be an issue with python internals.
    # Logger seems to work under Python 3.6.1
    if 'threading.py' not in code.co_filename.lower():
        logger.debug("%s:%d:%s:%s" %(code.co_filename, frame.f_lineno, code.co_name, event))

def fibo(n):
    if n == 0:
        return 0
    elif n == 1:
        return 1
    else:
        return fibo(n - 2) + fibo(n - 1)

def main():
    for i in range(0, 20):
        print("fibo(%d): %d, fact(%d): %d" %(i, fibo(i), i, testfact.fact(i)))

if __name__ == '__main__':
    #threading.setprofile(profilefunc)
    sys.setprofile(profilefunc)
    logger.info('Start Logging')
    main()
    logger.info('Closing Logging')

