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
**- Encapsulate node functions within node class
**- Handle global variables within class, struct, or namespace
**- Define and implement SMAController
- Determine how to define command structure
   - Ideally, this is language agnostic or is an easily translatable format system-system
- Build Settings system to encapsulate, save, and load settings for the Node and SMAController
