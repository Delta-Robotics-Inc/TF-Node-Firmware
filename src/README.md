# Changelog


## Version 1.0.0
- Within active development

### Planned Changes
- [x] Modify pin assignments to Node v1.0 board layout
- [x] Refactor `tf-node-firmware.h` into separate files for functions like TF_Muscle, Command, etc
- [ ] Create settings/configuration implementation stored/loaded in EEPROM and updated via command
- [x] Rewrite command system to align with DeltaLink and ThermoFlex API messages
   - [x] Switch from ascii implementation of commands to binary implementation
- [x] Implement bridge between serial comms and CAN comms


### Next TODO
- [x] Encapsulate node functions within node class
- [x] Handle global variables within class, struct, or namespace
- [x] Define and implement SMAController
- [x] Determine how to define command structure
   - Ideally, this is language agnostic or is an easily translatable format system-system
- [x] Install EmbeddedProto and build `tfnode-messages.proto` file 
- [ ] Build Settings system to encapsulate, save, and load settings for the Node and SMAController
   - Settings still not fully implemented
- [x] Implement command execution
- [x] Implement status response by Node
- [ ] Implement SMAController Status response
- [x] Implement CANInterface and SerialInterface
- [x] Implement command forwarding
- [x] Implement command responses by Node
- [ ] Implement Heartbeat/event messages in DeltaLink
   - Use CANOPen as a reference
- [ ] Come up with auto update method or push via hex file
   - Can we open repo to the public?
   - Need to determine OTA update solution
- [ ] Rebuild protobuf for versioning system
- [ ] Overheat detection with resistance method
- [ ] Resistance control (model in python first)


## Reach Goals
- [ ] Look at ROS & microRos, CANOpen and see if either of these frameworks would be easy to move to.

---


## Building ProtoBuf Files in C++

In order to update protobuf definitions for the firmware, based on the `tfnode-messages.proto`, please follow this tutorial:
**TODO find working solution