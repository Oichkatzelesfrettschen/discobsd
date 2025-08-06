import os
import sys
sys.path.insert(0, os.path.abspath('../tools'))

project = 'Trace Tools'
extensions = ['sphinx.ext.autodoc']
autodoc_typehints = 'description'
html_theme = 'alabaster'
