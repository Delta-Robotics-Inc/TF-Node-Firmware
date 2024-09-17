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