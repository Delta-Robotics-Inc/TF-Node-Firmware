#include "globals.hpp"
#include "SMAController.hpp"
#include "TFNode.hpp"
#include "config.hpp"

TFNode::TFNode(const NodeAddress& addr)
    : address(addr),
    commandProcessor(nullptr), // Initialize to nullptr
    statusMode(tfnode::DeviceStatusMode::STATUS_NONE),
    statusInterface(nullptr),
    smaController0(tfnode::Device::DEVICE_PORT1, "M1_PORT0", M1_MOS_TRIG, M1_CURR_RD, M1_VLD_RD, VLD_SCALE_FACTOR_M1, VLD_OFFSET_M1),
    smaController1(tfnode::Device::DEVICE_PORT2, "M2_PORT1", M2_MOS_TRIG, M2_CURR_RD, M2_VLD_RD, VLD_SCALE_FACTOR_M2, VLD_OFFSET_M2),
    n_error(0xFF), // Assuming all errors are cleared at start
    n_vSupply(0.0f),
    pot_val(0.0f),
    log_start(millis()),
    log_timer(millis()) 
    { }

NodeAddress TFNode::getAddress() const {
    return address;
}

void TFNode::begin() {
    // TODO Initialize settings
    //EEPROM.begin(sizeof(NodeSettings));
    //settings.load();

    pinMode(STATUS_RGB_RED, OUTPUT);
    pinMode(STATUS_RGB_GREEN, OUTPUT);
    pinMode(STATUS_RGB_BLUE, OUTPUT);

    pinMode(STATUS_SOLID_LED, OUTPUT);
    digitalWrite(STATUS_SOLID_LED, LOW);  // This pin currently goes high on errors

    pinMode(AUX_BUTTON, INPUT_PULLUP); // Set button pin as input with internal pull-up resistor

    // Initialize controllers
    //*********************************************************/
    // Serial.print("SMA 0 pointer: ");
    // Serial.println((uintptr_t)smaControllers[0].get(), HEX);
    smaController0.begin();
    // Serial.print("SMA 1 pointer: ");
    // Serial.println((uintptr_t)smaControllers[1].get(), HEX);
    smaController1.begin();

    log_timer = millis();
}

void TFNode::init_finished() {
    digitalWrite(STATUS_RGB_RED, LOW);
    digitalWrite(STATUS_RGB_GREEN, HIGH);
    digitalWrite(STATUS_RGB_BLUE, HIGH);
}


void TFNode::update() {
    // Poll sensors
    n_vSupply = getSupplyVolts();
    pot_val = getPotVal();

    // Update controllers
    //*********************************************************/
    smaController0.update();
    smaController1.update();
    //Serial.println("TFNode::update() - Finished SMAController::update()");

    checkErrs();

    // Check auxillary gpio
    // TODO change to interrupt
    if(!digitalRead(AUX_BUTTON)) {
        optButtonStopFunc();
    }

    // Process commands
    //commandProcessor.update();

    // TODO implement logger (should it be encapsulate within class or external function?)
    // Every certain interval (LOG_MS), log/report data to console
    if(millis() - log_timer > LOG_MS) {
        sendStatusResponse(statusMode);
        //smaController0.sendStatusResponse(statusMode);
        //smaController1.sendStatusResponse(statusMode);
        //digitalWrite(STATUS_SOLID_LED, !digitalRead(STATUS_SOLID_LED));
    }
}

void TFNode::toggleRGBStatusLED() {

    ledState =  (ledState == RED) ? GREEN :
                (ledState == GREEN) ? BLUE :
                RED;

    switch(ledState) {
        case RED:
            digitalWrite(STATUS_RGB_RED, LOW);
            digitalWrite(STATUS_RGB_GREEN, HIGH);
            digitalWrite(STATUS_RGB_BLUE, HIGH);
            break;
        case GREEN:
            digitalWrite(STATUS_RGB_RED, HIGH);
            digitalWrite(STATUS_RGB_GREEN, LOW);
            digitalWrite(STATUS_RGB_BLUE, HIGH);
            break;
        case BLUE:
            digitalWrite(STATUS_RGB_RED, HIGH);
            digitalWrite(STATUS_RGB_GREEN, HIGH);
            digitalWrite(STATUS_RGB_BLUE, LOW);
            break;
    }
}


//=============================================================================
// Command Handlers
//=============================================================================

tfnode::ResponseCode TFNode::CMD_setStatusMode(tfnode::Device device, tfnode::DeviceStatusMode mode, bool repeating, NetworkInterface* iface) {

    // For now, we assume device is DEVICE_NODE or DEVICE_ALL
    if (device == tfnode::Device::DEVICE_NODE || device == tfnode::Device::DEVICE_ALL) {
        statusInterface = iface;
        if(repeating) {
            // Set status mode for the node for repeating status
            statusMode = mode;
           //statusInterface = iface; // Keep track of the interface
        }
        else {
            // Nonrepeating status means set the statusMode to none
            statusMode = tfnode::DeviceStatusMode::STATUS_NONE;
            Serial.println("Sending Single Status response...");
            sendStatusResponse(mode);  // Send a single status response
        }
    }

    // Optionally, handle status mode for SMAControllers
    if (device == tfnode::Device::DEVICE_PORT1 || device == tfnode::Device::DEVICE_PORTALL || device == tfnode::Device::DEVICE_ALL) {
        // Set status mode on the specific SMAController
        // Not implemented in this example
    }
    if(device == tfnode::Device::DEVICE_PORT2 || device == tfnode::Device::DEVICE_PORTALL || device == tfnode::Device::DEVICE_ALL) {
        // Set status mode on the specific SMAController
        // Not implemented in this example
    }

    return tfnode::ResponseCode::RESPONSE_SUCCESS;
}

tfnode::ResponseCode TFNode::CMD_resetDevice(tfnode::Device device) {
    if (device == tfnode::Device::DEVICE_NODE || device == tfnode::Device::DEVICE_ALL) {
        // Reset node-specific settings
    }
    if (device == tfnode::Device::DEVICE_PORT1 || device == tfnode::Device::DEVICE_ALL || device == tfnode::Device::DEVICE_PORTALL) {
        smaController0.CMD_reset();
    }
    if (device == tfnode::Device::DEVICE_PORT2 || device == tfnode::Device::DEVICE_ALL || device == tfnode::Device::DEVICE_PORTALL) {
        smaController1.CMD_reset();
    }

    return tfnode::ResponseCode::RESPONSE_SUCCESS;
}

tfnode::ResponseCode TFNode::CMD_enableDevice(tfnode::Device device) {
    if (device == tfnode::Device::DEVICE_PORT1) {
        smaController0.CMD_setEnable(true);
    } else if (device == tfnode::Device::DEVICE_PORT2) {
        smaController1.CMD_setEnable(true);
    }
    else if (device == tfnode::Device::DEVICE_ALL || device == tfnode::Device::DEVICE_PORTALL) {
        smaController0.CMD_setEnable(true);
        smaController1.CMD_setEnable(true);
    }
    // Handle DEVICE_ALL and DEVICE_PORTALL as needed

    return tfnode::ResponseCode::RESPONSE_SUCCESS;
}

// Implement other command handlers similarly



//=============================================================================
// Node Status Functions
//=============================================================================

// Send Status Response based on current Status Mode
void TFNode::sendStatusResponse(tfnode::DeviceStatusMode mode) {
    if (mode == tfnode::DeviceStatusMode::STATUS_NONE || !commandProcessor || !statusInterface) {
        // No status to send or no interface to send on
        return;
    }
    tfnode::NodeResponse response;

    tfnode::StatusResponse& statusResponse = response.mutable_status_response();
    statusResponse.set_device(tfnode::Device::DEVICE_NODE); // Set the device sending the response
    Serial.println("Sending CMD response...");


    switch (mode) {
        case tfnode::DeviceStatusMode::STATUS_COMPACT: {
            tfnode::NodeStatusCompact compactStatus = getStatusCompact();
            statusResponse.mutable_node_status_compact() = compactStatus;
            break;
        }
        case tfnode::DeviceStatusMode::STATUS_DUMP: {
            tfnode::NodeStatusDump dumpStatus = getStatusDump();
            statusResponse.mutable_node_status_dump() = dumpStatus;
            break;
        }
        case tfnode::DeviceStatusMode::STATUS_DUMP_READABLE: {
            // Special case to send status straight to serial
            String readableStatus = getStatusReadable();
            commandProcessor->sendSerialString(readableStatus);
            return;
        }
        default:
            // Handle other status modes
            return;
    }

    // if (commandProcessor && statusInterface) {
            commandProcessor->sendResponse(response, statusInterface);
                // }
}

tfnode::NodeStatusCompact TFNode::getStatusCompact() {
    tfnode::NodeStatusCompact status;

    status.set_uptime(millis() / 1000); // Uptime in seconds
    status.set_error_code(n_error);
    status.set_v_supply(n_vSupply);
    status.set_pot_val(pot_val);

    // Add other fields as necessary

    return status;
}


// Returns a NodeStatusDump, which details extensively the configuration and variables of this node
tfnode::NodeStatusDump TFNode::getStatusDump() {
    tfnode::NodeStatusDump status;

    // Set compact status
    tfnode::NodeStatusCompact compactStatus = getStatusCompact();
    status.mutable_compact_status() = compactStatus;

    // Get loaded settings (if any)
    status.mutable_loaded_settings() = settings;

    // Set other detailed fields
    //status.set_firmware_version(CFG_FIRMWARE_VERSION);
    //status.set_board_version(SHIELD_VERSION);
    status.set_firmware_version(0);
    status.set_board_version(0);
    status.set_muscle_cnt(SMA_CONTROLLER_CNT); // Number of SMAControllers
    status.set_log_interval_ms(LOG_MS);
    status.set_vrd_scalar(VRD_SCALE_FACTOR);
    status.set_vrd_offset(VRD_OFFSET);
    status.set_max_current(MAX_CURRENT);
    status.set_min_v_supply(MIN_VSUPPLY);

    // Add other fields as needed

    return status;
}


// Returns a readable string of the status dump.
// This is a large amount of data but useful when other side does not understand the TF Node messaging system
String TFNode::getStatusReadable()
{
    String stat_str = 
                "========================================\n";
    stat_str += "LOG TIME: " + String(millis() - log_start) + "\n"; // Display time since log start
    stat_str += "Battery Volts: " + String(n_vSupply, 6) + " V\n";
    stat_str += "Error State: " + String(n_error, BIN) + "\n";
    stat_str += "Pot Val: " + String(pot_val, 6) + "\n";
    return stat_str;
}



//=============================================================================
// Error Handling
//=============================================================================

void TFNode::optButtonStopFunc() {
    //******************************************************** */
    smaController0.CMD_setEnable(false);
    smaController1.CMD_setEnable(false);
    errRaise(ERR_EXTERNAL_INTERRUPT);
}

void TFNode::checkErrs() {
      // Low Power Condition
    if(n_vSupply < MIN_VSUPPLY && n_vSupply > IGNORE_SUPPLY) {
        //errRaise(ERR_LOW_VOLT);
        //TF_Muscle::setEnableAll(false);
        //light up a low battery LED
    }
}

// Raise Error by anding current error val with 0 bitshifted by index
void TFNode::errRaise(NodeError err_code) {
    n_error = n_error & (0b11111111 ^ (1<<err_code));
    digitalWrite(STATUS_SOLID_LED, HIGH);
}
void TFNode::errClear() {
    n_error = 0b11111111;
    digitalWrite(STATUS_SOLID_LED, LOW);
}
void TFNode::errClear(NodeError err_code) {
    n_error = n_error | (1<<err_code);
    digitalWrite(STATUS_SOLID_LED, LOW);
}


//=============================================================================
// Sensor Read Functions
//=============================================================================

float TFNode::getSupplyVolts() {
    float value=0.0,samples=0.0,avg_value=0.0,raw=0.0;
    
    for (int x = 0; x < 30; x++){   // Get 30 samples
      value = analogRead(VRD_PIN);  // Read voltage divider value   
      samples += value;          // Add samples together
    }
    raw=samples/30.0;  // Taking Average of Samples

    float volts = raw * VRD_SCALE_FACTOR + VRD_OFFSET;
    return volts;
    //return raw;
}

// Returns a value (0.0->1.0) based on pot value
float TFNode::getPotVal() {
    float value=0.0,samples=0.0,avg_value=0.0,raw=0.0;
    
    for (int x = 0; x < 3; x++){   // Get 30 samples
      value = analogRead(MANUAL_MODE_POT);  // Read potentiometer
      samples += value;          // Add samples together
    }
    raw=samples/3.0;  // Taking Average of Samples

    float percent = raw / 1024.0;
    percent = percent > MANUAL_MODE_THRESHOLD ? percent : 0.0; // Limit lowest measurable value
    return percent;
    //float temp_sensor_val = -0.0499 * (raw)*(raw) + 97.084 * (raw) - 47088;
    //return temp_sensor_val;
    //return raw; //TODO Switch back to percent after running tests with this value
}

