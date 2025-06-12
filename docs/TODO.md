# Changelog


## Version 1.0.0
- [x] Modify pin assignments to Node v1.0 board layout
- [x] Refactor `tf-node-firmware.h` into separate files for functions like TF_Muscle, Command, etc
- [x] Rewrite command system to align with DeltaLink and ThermoFlex API messages
   - [x] Switch from ascii implementation of commands to binary implementation
- [x] Implement bridge between serial comms and CAN comms
- [x] Encapsulate node functions within node class
- [x] Handle global variables within class, struct, or namespace
- [x] Define and implement SMAController
- [x] Determine how to define command structure
   - Ideally, this is language agnostic or is an easily translatable format system-system
- [x] Install EmbeddedProto and build `tfnode-messages.proto` file 
- [x] Implement CANInterface and SerialInterface
- [x] Implement command execution
- [x] Implement status response by Node
- [x] Implement command forwarding
- [x] Implement command responses by Node


### v1.0.1 Changes
- [x] Change Deltalink syntax for firmware version to MAJOR.MINOR.PATCH
- [x] Fix build issues with platformio on Ubuntu


### IN PROGRESS v1.1.0 Changes
- [x] Add slash commands for normal ascii communication from PC to device
   - [x] Add standard set from v0.2 firmware predating DeltaLink
   - [x] "/about" ascii command to get firmware version and board ID
   - [ ] Update README to present both /command and DeltaLink methods of communicattion
- [x] Implement Heartbeat/event messages in DeltaLink
   - Use CANOPen as a reference
   - [x] Decide and document heartbeat structure
      - Empty DeltaLink packet counts as heartbeat
   - [x] Implement structure in firmware
   - [x] Test with python library
   - [x] Have toggle in order to disable for ascii commands
      - [x] "/heartbeat" ascii command to send a heartbeat signal
      - [x] "/heartbeat off" to turn heartbeat off
      - [x] "/heartbeat on" to turn heartbeat on
- [~] Implement SMAController Status response
- [ ] Come up with auto update method or push via hex file
   - [x] Can we open repo to the public? - Yes
   - [ ] Create Simple Update Process and Automatic Updating Script
- [ ] LED Codes
   - [ ] Connected (with heartbeat) - Orange
   - [ ] Disconnected - Red
   - [ ] Heartbeat disabled - Blue
   - [ ] Enabled - Green
   - [ ] On target - Green/White
- [x] Working Current Control mode
   - [x] Replaced problematic proportional-only MODE_AMPS with PID-based CurrentController
   - [x] Implemented CurrentController class using MiniPID with safety limits (0-75% PWM)
   - [x] Integrated CurrentController into SMAController with proper state management
   - [x] Added basic unit tests (test/test_current_controller.cpp)
   - **Next Steps:**
     - [x] **CRITICAL FIX**: Scale current measurement by PWM duty cycle (peak current → average current)
     - [ ] Verify unit tests compile and run successfully
     - [ ] Bench test with actual hardware to verify PWM response tracks requested current
     - [ ] Tune PID gains (currently KP=0.1, KI=0.02, KD=0.01) based on system response
     - [ ] Address 5A current sensor offset in readMuscleAmps() if calibration is needed
     - [ ] Consider implementing protobuf-based PID gain configuration (in SMAControllerSettings)
     - [ ] Consider hardware low-pass filtering for true average current measurement


### Backlog Changes
- [ ] Add /serial-debug ASCII command to enable verbose debug output (with levels: NONE, ERROR, WARNING, INFO, DEBUG)
- [ ] Move header files to `include/` to adhere to common C++ practices
- [ ] Build Settings system to encapsulate, save, and load settings for the Node and SMAController
   - Settings still not fully implemented
   - [ ] Add heartbeat mode to DeltaLink and implement as a Node setting
- [ ] Store firmware upload timestamp in EEPROM
- [ ] Overheat detection with resistance method
- [ ] Resistance control (model in python first)
- [ ] Base protobuf files on DeltaLink repository as the common location for protocol definitions
   - Add an action to check if firmware is behind DeltaLink version
- [ ] Overhaul DeltaLink to align with lessons from CANOpen and ROS. See notion.


## Reach Goals
- [ ] Look at ROS & microRos, CANOpen and see if either of these frameworks would be easy to move to.

---


## Building ProtoBuf Files in C++

In order to update protobuf definitions for the firmware, based on the `tfnode-messages.proto`, please follow this tutorial:
**TODO find working solution