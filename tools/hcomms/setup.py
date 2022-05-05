import setuptools

setuptools.setup(
    author='Dave Mohamad',
    name='newton-hcomms',
    packages=setuptools.find_namespace_packages(include=['newton.*']),
    description='python-c++ bindings for serial communication',
)
