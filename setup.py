from setuptools import setup, Extension

modulo_parqueadero = Extension(
    name='_parqueadero',
    sources=['parqueadero.i', 'parqueadero.cpp'],
    swig_opts=['-c++'],
    libraries=['ws2_32'],
    extra_compile_args=['-std=c++17']
)

setup(
    name='parqueadero',
    version='1.0',
    description='Modulo SWIG de Parqueadero en C++',
    ext_modules=[modulo_parqueadero],
    py_modules=['parqueadero']
)