from distutils.core import setup, Extension

module1 = Extension('nytquery',
                    define_macros = [#('MAJOR_VERSION', '1'),
                                     #('MINOR_VERSION', '0')
                                     ],
                    include_dirs = [
                        'C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\include',
                        'C:\Python27\include',
                        ],
                    libraries = ['python27'],
                    library_dirs = ['C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\lib',
                                    'C:\Python27\libs',
                                    ],
                    sources = ['nytquery.cpp'])

setup (name = 'NewYorkTime',
       version = '1.0',
       description = 'Query the Inverted Index',
       author = 'Yan Zhang',
       author_email = 'yz1613@nyu.edu',
       #url = 'https://docs.python.org/extending/building',
       ext_modules = [module1])