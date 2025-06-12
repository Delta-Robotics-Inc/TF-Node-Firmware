## Running Python Serial Tests

In order to run these test scripts, or create your own, make sure that you have installed the TF-Python-Serial library to your python environment.  This environment exposes the API for maintaining a session with the ThermoFlex node system.

These tests execute sequences of commands and monitor the response from the Node device.

---

### Installing Library

Inside of your terminal, run the following.  I recommend running this in a venv to test it out.

```bash
git clone https://github.com/Delta-Robotics-Inc/TF-Python-Serial

# Replace X.X.X with the library version
pip install ./TF-Python-Serial/python-serial/build/dist/thermoflex-X.X.X.tar.gz
```

You can also install for testing directly from pip:

```bash
pip install thermoflex
```

This method is easier if you do not have to make changes to the python API as well.
