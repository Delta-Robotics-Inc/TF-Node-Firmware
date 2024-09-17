## Changelog

**NOTE:** We need to implement a X.X.X versioning system soon.
[Firmware GitHub Issue Link](https://github.com/Delta-Robotics-Inc/ThermoFlex/issues/2)

### Version 0.2.1
- Moved code into src and test folders
- Rewrote resistive control to allow resistance measurements even at low duty cycles.  Also switched to 100 Hz from 20 taking advantage of this new method and driving narrower pulse widths.


### Version 0.2.0
- Clone from v0.1 for new DEV board PCB
- Change amperage measurement function to new amplifier implementation
- Add resistance control mode using PID controller