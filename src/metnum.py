#!/usr/bin/python
from scripts.fabricate import *
from scripts.settings import *
from scripts.utils import listfiles
from sys import argv

CXXFLAGS=['-Wall', '-Wextra', '-pedantic', '-std=c++11', '-O2', '-ggdb', '-masm=intel']

# Acciones
def build():
  compile()
  link()

def compile():
  for source in sources:
    run(compiler, CXXFLAGS, '-c', source+'.cpp', '-o', source+'.o')

def link():
  objects = [s+'.o' for s in sources]
  run(compiler, CXXFLAGS, '-o', executable, objects)

def clean():
  autoclean()

def test():
  #build()
  import unittest
  unittest.main(module='scripts.tptests', exit=False, argv=argv[:1], verbosity=3)

main()
