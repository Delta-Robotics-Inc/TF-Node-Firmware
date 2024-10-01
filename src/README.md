# Changelog


## Version 1.0.0
- Within active development

### Planned Changes
- Modify pin assignments to Node v1.0 board layout
- Refactor `tf-node-firmware.h` into separate files for functions like TF_Muscle, Command, etc
- Create settings/configuration implementation stored/loaded in EEPROM and updated via command
- Rewrite command system to align with Python Serial API
   - Switch from ascii implementation of commands to binary implementation
- Implement bridge between serial comms and CAN comms


### Next TODO
- [x] Encapsulate node functions within node class
- [x] Handle global variables within class, struct, or namespace
- [x] Define and implement SMAController
- [x] Determine how to define command structure
   - Ideally, this is language agnostic or is an easily translatable format system-system
- [ ] Build Settings system to encapsulate, save, and load settings for the Node and SMAController
- [ ] Implement command execution
- [ ] Implement command forwarding
- [ ] Implement command responses by Node

---


## Building ProtoBuf Files in C++

In order to update protobuf definitions for the firmware, based on the `tfnode-messages.proto`, please follow this tutorial:
**TODO find working solution