[build-system]
requires = ["setuptools"]
build-backend = "setuptools.build_meta"


[project]
name = "thermoflex"
dynamic = ["version"]
dependencies = ['pyserial','importlib-serial','importlib-serial.tools.list_ports', 'importlib-time', 'importlib-threading']
description = "This is the repository for the thermoflex muscle by Delta Robotics"
keywords = ["thermoflex" , "Delta", "Delta Robotics"]
requires-python = ">= 3.8"