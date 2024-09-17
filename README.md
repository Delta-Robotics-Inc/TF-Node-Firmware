Welcome to the Delta Robotics ThermoFlex repository.  Here contains the development of the firmware for the ThermoFlex Node.



This folder contains the development of firmware for the muscle controller.  Currently, this software is deployed to an Arduino Minima.  The microcontroller is configured to utilize our ThermoFlex Node PCB Shield.

---


# Node Commands


The following are the currently supported commands on the ThermoFlex Node controller.  To execute these commands, do the following:

1. Connect the TF Node via USB 
2. Open a serial console with a baud rate of 115200
3. Make sure to connect the console to the COM port that the controller is attached to.
4. Execute commands in lowercase with a space in between parameters (see example below table)


The below commands that specify `<device> .. (default all)` will work without parameters and default execute on all devices.

| Command      | Parameters                                                                                         | Description                                                            |
| ------------ | -------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------- |
| reset        | \<device\> = all, m1, m2 .. etc (default all)                                                      | stops device and resets the device settings to defaults                |
| set-enable   | \<device> = all, m1, m2 .. etc<br>\<state> = true, false                                           | enables/disables the mosfet of one or all devices                      |
| set-mode     | \<device> = all, m1, m2 .. etc<br>\<mode> = percent, amps, volts, degrees                          | sets the operating mode of one or all devices                          |
| set-setpoint | \<device> = all, m1, m2 .. etc<br>\<mode> = percent, amps, volts, degrees<br>\<setpoint> = `float` | sets the value for one or all devices' setpoints at the specified mode |
| status       | \<device\> = all, m1, m2 .. etc (default all)                                                      | displays system info of one or all devices (including the Node itself) |
| stop         | \<device\> = all, m1, m2 .. etc (default all)
| log-mode     | \<device\> = all, node, m1, m2 .. etc<br>\<mode> = 0, 1, etc                                       | sets continuous logging mode for device (0 for no logging)

## Input Examples

Here are some example commands to display the syntax:

```

> set-mode m1 volts      //sets muscle 1 mode to volts
> set-setpoint all volts 5.0 //sets all muscles
> set-enable m2 true      //enables m2
> status         //status for all devices
> status m1      //status for just m1
> reset all      //resets all devices
> reset          //resets all devices
> reset m3       //resets just m3

```
