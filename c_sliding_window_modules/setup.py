from setuptools import Extension, setup
import numpy

module = Extension("sandia",
				   sources=[
					   'sandia.c',
					   'slidingwindow.c'
				   ], include_dirs=[numpy.get_include()])
setup(name='sandia',
	  version='1.0',
	  description='Python wrapper for Sandia functions',
	  ext_modules=[module]
	  )
