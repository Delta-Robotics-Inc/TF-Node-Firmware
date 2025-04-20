Welcome to the Delta Robotics ThermoFlex repository.  Here contains the development of the firmware for the ThermoFlex Node muscle controller.  Currently, this software is deployed to an Arduino Minima.  The microcontroller is configured to utilize our ThermoFlex Node PCB Shield.

**IMPORTANT:** You will not be able to use the Arduino IDE to upload this project.  Instead, please install [platformio](https://platformio.org/), which is an Embedded IDE environment for VS Code that is much better for building large-scale Arduino projects.

## `Branch Firmware Version: v1.1.0`

### Instructions for Push
- Make sure to update firmware version above and in the `src/config.hpp` file.
- You cannot push code to the main branch without updating the firmware version.
- The firmware version must be updated every time a Node exists in the wild with a version of firmware that exists with newly developed code.  Versions should only be released to customers (the wild) after testing and push to the main branch.  We will come up with a process of sending beta versions (perhaps a `vX.X.X-beta` tag)


# Node Commands

There are 2 methods for controlling the Node. One is through the [ThermoFlex-Python-API](https://github.com/Delta-Robotics-Inc/ThermoFlex-Python-API), which you can find instructions for on its repository. This method is recommended as the easiest to use and develop with.
The second is through the following ASCII commands. This method is the fastest and best for troubleshooting the controller when python isn't working out. To execute these commands, do the following:

1. Connect the TF Node via USB 
2. Open a serial console with a baud rate of 115200
3. Make sure to connect the console to the COM port that the controller is attached to.
4. Execute commands in lowercase with a space in between parameters (see example below table)


The below commands that specify `<device> .. (default: all)` will work without parameters and default to all devices.

| Command       | Parameters                                                       | Description                                                         |
|---------------|------------------------------------------------------------------|---------------------------------------------------------------------|
| /help         | (none)                                                           | Display available commands                                          |
| /about        | (none)                                                           | Display firmware version, node ID, and timestamp                    |
| /reset        | `<device>` (default: all &#124; node &#124; m1 &#124; m2)                                         | Stop and reset the specified device(s)                              |
| /set-enable   | `<device>`, `<state>` (`true` &#124; `false`)                    | Enable or disable the specified device(s)                           |
| /set-mode     | `<device>`, `<mode>` (`percent` &#124; `amps` &#124; `volts` &#124; `ohms`) | Set the operating mode for the specified device(s)                  |
| /set-setpoint | `<device>`, `<mode>`, `<value>` (`float`)                        | Set the setpoint value for the specified device(s)                  |
| /status       | `<device>` (default: all &#124; node &#124; m1 &#124; m2)                                        | Display status of the specified device(s), including the Node       |
| /heartbeat    | `[on` &#124; `off]` _(optional)_                                   | Send a heartbeat manually or toggle automatic heartbeats            |

## Input Examples

Here are some example commands to display the syntax:

```bash

/help                        # display help message
/about                       # display firmware version, node ID, and timestamp
/set-mode m1 volts           # sets muscle 1 mode to volts
/set-setpoint all volts 5.0  # sets all muscles
/set-enable m2 true          # enables m2
/status                      # status for all devices
/status m1                   # status for just m1
/reset all                   # resets all devices
/reset                       # resets all devices
/reset m3                    # resets just m3

# Heartbeat commands
/heartbeat                   # send a heartbeat immediately
/heartbeat on                # enable automatic heartbeat
/heartbeat off               # disable automatic heartbeat

```
