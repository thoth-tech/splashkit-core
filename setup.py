from setuptools import setup, Extension
import os

splashkit_module = Extension(
    name="splashkit",
    sources=[
        os.path.join("languages", "cpp", "splashkit_starter.cpp")
    ],
    include_dirs=[os.path.join("languages", "cpp")],
    language="c++",
)

setup(
    name="splashkit",
    version="0.1.0",
    description="Python bindings for the SplashKit library",
    ext_modules=[splashkit_module],
)
