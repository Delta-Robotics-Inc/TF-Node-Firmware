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
   - [ ] "/about" ascii command to get firmware version and board ID
- [ ] Implement Heartbeat/event messages in DeltaLink
   - Use CANOPen as a reference
   - [x] Decide and document heartbeat structure
      - Empty DeltaLink packet counts as heartbeat
   - [x] Implement structure in firmware
   - [ ] Test with python library
   - [ ] Have toggle in order to disable for ascii commands
      - [ ] "/heartbeat" ascii command to send a heartbeat signal
      - [ ] "/heartbeat off" to turn heartbeat off
      - [ ] "/heartbeat on" to tuen heartbeat on
- [ ] Implement SMAController Status response
- [ ] Come up with auto update method or push via hex file
   - Can we open repo to the public? - Yes
   - Need to determine OTA update solution
- [ ] Create settings/configuration implementation stored/loaded in EEPROM and updated via command
   - [ ] CAN bus ID


### Backlog Changes

- [ ] Build Settings system to encapsulate, save, and load settings for the Node and SMAController
   - Settings still not fully implemented
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