"""
File: /setup.py
Created Date: Wednesday July 30th 2025
Author: Christian Nonis <alch.infoemail@gmail.com>
-----
Last Modified: Wednesday July 30th 2025 9:36:09 pm
Modified By: the developer formerly known as Christian Nonis at <alch.infoemail@gmail.com>
-----
"""

from setuptools import setup, Extension, find_packages


def get_pybind11_include():
    """Get pybind11 include directory, handling import issues."""
    try:
        import pybind11

        return pybind11.get_include()
    except ImportError:
        return "/placeholder/pybind11/include"
    except AttributeError:
        try:
            from pybind11 import get_include

            return get_include()
        except ImportError:
            return "/placeholder/pybind11/include"


pybind11_include = get_pybind11_include()

ext_modules = [
    Extension(
        "chunker_cpp",
        ["main.cpp", "chunker/chunker.cpp"],
        include_dirs=[pybind11_include],
        language="c++",
        extra_compile_args=["-std=c++17"],
    ),
]

setup(
    name="cpp-chunker",
    version="0.1.0",
    description="C++ chunker",
    ext_modules=ext_modules,
    packages=find_packages(),
    py_modules=["chunker"],
    package_data={
        "": ["*.pyi"],
    },
    zip_safe=False,
    install_requires=[
        "pybind11>=2.10.0",
    ],
    python_requires=">=3.7",
    classifiers=[
        "Development Status :: 3 - Alpha",
        "Intended Audience :: Developers",
        "License :: OSI Approved :: MIT License",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: 3.11",
        "Programming Language :: Python :: 3.12",
        "Programming Language :: C++",
    ],
)
