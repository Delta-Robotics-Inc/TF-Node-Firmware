# ThermoFlex Node Status Request Protocol

This document explains how the Python library should construct status requests and handle status responses when communicating with ThermoFlex Nodes using the established DeltaLink packet protocol.

## Overview

The ThermoFlex Node firmware uses a protobuf-based command/response system where:
- **Status Requests** are sent as `NodeCommand` messages containing `GetStatusCommand`
- **Status Responses** are returned as `NodeResponse` messages containing `StatusResponse`
- **Device Targeting** allows requests for specific devices (Node, Port0, Port1, All)
- **Status Modes** provide different levels of detail in responses

## Prerequisites

This guide assumes your Python library already has:
- Working `Packet` class implementation for DeltaLink protocol
- Protobuf message classes generated from `tfnode-messages.proto`
- Network interface for sending/receiving packets

## Status Request Construction

### 1. Basic Status Request Structure

```python
# Create the outer command container
node_command = tfnode.NodeCommand()

# Create the status request
status_command = tfnode.GetStatusCommand()
status_command.device = tfnode.Device.DEVICE_ALL  # Target device
status_command.mode = tfnode.DeviceStatusMode.STATUS_COMPACT  # Detail level
status_command.repeating = False  # Single request vs continuous

# Embed the status command in the node command
node_command.status = status_command
```

### 2. Device Targeting Options

```python
# Request status from all devices (Node + all muscle controllers)
status_command.device = tfnode.Device.DEVICE_ALL

# Request status from only the node
status_command.device = tfnode.Device.DEVICE_NODE

# Request status from specific muscle controller
status_command.device = tfnode.Device.DEVICE_PORT0  # First muscle
status_command.device = tfnode.Device.DEVICE_PORT1  # Second muscle

# Request status from all muscle controllers (not node)
status_command.device = tfnode.Device.DEVICE_PORTALL
```

### 3. Status Mode Options

```python
# Compact status - essential data only
status_command.mode = tfnode.DeviceStatusMode.STATUS_COMPACT

# Full status dump - all configuration and state data
status_command.mode = tfnode.DeviceStatusMode.STATUS_DUMP

# Human-readable status - formatted text (sent via serial, not DeltaLink network)
status_command.mode = tfnode.DeviceStatusMode.STATUS_DUMP_READABLE
```

### 4. Repeating vs Single Requests

```python
# Single status request (most common)
status_command.repeating = False

# Continuous status reporting (for monitoring/logging)
status_command.repeating = True
```

## Packet Construction and Transmission

### 1. Serialize Command to Packet

```python
def send_status_request(device, mode, repeating=False):
    # Build the command
    node_command = tfnode.NodeCommand()
    status_command = tfnode.GetStatusCommand()
    status_command.device = device
    status_command.mode = mode
    status_command.repeating = repeating
    node_command.status = status_command
    
    # Serialize the protobuf message
    serialized_data = node_command.serialize()
    
    # Create DeltaLink packet
    packet = Packet()
    packet.senderId = [0, 0, 0] # Master ID
    packet.destinationId = target_node_address  # Target node address
    packet.data = serialized_data
    packet.packetLength = packet.calculatePacketLength()
    packet.checksum = packet.calculateChecksum()
    
    # Send via network interface
    network_interface.sendPacket(packet)
```

## Status Response Handling

### 1. Response Message Structure

Status responses come back as `NodeResponse` messages with the following structure:

```python
# Received packet contains NodeResponse
node_response = tfnode.NodeResponse()
node_response.deserialize(packet.data)

# Check response type
if node_response.HasField('status_response'):
    status_response = node_response.status_response
    
    # Check which device responded
    responding_device = status_response.device
    
    # Extract status data based on response type
    if status_response.HasField('node_status_compact'):
        handle_node_compact_status(status_response.node_status_compact)
    elif status_response.HasField('sma_status_compact'):
        handle_muscle_compact_status(status_response.sma_status_compact)
    # ... other status types
```

### 2. Device Response Identification

The firmware sends separate responses for each targeted device:

```python
def handle_status_response(packet):
    node_response = tfnode.NodeResponse()
    node_response.deserialize(packet.data)
    
    if node_response.HasField('status_response'):
        status_response = node_response.status_response
        
        # Identify responding device
        if status_response.device == tfnode.Device.DEVICE_NODE:
            print("Received Node status response")
            
        elif status_response.device == tfnode.Device.DEVICE_PORT0:
            print("Received Muscle Port 0 status response")
            
        elif status_response.device == tfnode.Device.DEVICE_PORT1:
            print("Received Muscle Port 1 status response")
```

### 3. Node Status Response Handling

```python
def handle_node_compact_status(node_status):
    """Handle compact node status response"""
    print(f"Node Uptime: {node_status.uptime} seconds")
    print(f"Supply Voltage: {node_status.v_supply}V")
    print(f"Error Code: {node_status.error_code}")
    print(f"Pot Value: {node_status.pot_val}")

def handle_node_dump_status(node_dump):
    """Handle full node status dump"""
    # Includes compact status plus additional data
    compact = node_dump.compact_status
    handle_node_compact_status(compact)
    
    # Additional dump data
    print(f"Firmware Version: {node_dump.firmware_version_major}.{node_dump.firmware_version_minor}.{node_dump.firmware_version_patch}")
    print(f"Board Version: {node_dump.board_version}")
    print(f"Muscle Count: {node_dump.muscle_cnt}")
    print(f"Max Current: {node_dump.max_current}A")
```

### 4. Muscle Status Response Handling

```python
def handle_muscle_compact_status(sma_status):
    """Handle compact muscle controller status response"""
    port_name = "Port 0" if sma_status.device_port == tfnode.Device.DEVICE_PORT0 else "Port 1"
    
    print(f"=== {port_name} Status ===")
    print(f"Enabled: {sma_status.enabled}")
    print(f"Mode: {get_mode_string(sma_status.mode)}")
    print(f"Setpoint: {sma_status.setpoint}")
    print(f"PWM Output: {sma_status.output_pwm * 100:.1f}%")
    print(f"Load Current: {sma_status.load_amps:.3f}A")
    print(f"Load Voltage: {sma_status.load_vdrop:.2f}V")
    print(f"Load Resistance: {sma_status.load_mohms:.1f}mΩ")

def get_mode_string(mode):
    """Convert SMA control mode enum to string"""
    mode_map = {
        tfnode.SMAControlMode.MODE_PERCENT: "Percent",
        tfnode.SMAControlMode.MODE_AMPS: "Current",
        tfnode.SMAControlMode.MODE_VOLTS: "Voltage", 
        tfnode.SMAControlMode.MODE_OHMS: "Resistance",
        tfnode.SMAControlMode.MODE_TRAIN: "Training"
    }
    return mode_map.get(mode, "Unknown")
```

## Complete Example Implementation

```python
class ThermoFlexStatusManager:
    def __init__(self, network_interface):
        self.network_interface = network_interface
        self.pending_requests = {}  # Track pending status requests
        
    def request_status(self, target_address, device=tfnode.Device.DEVICE_ALL, 
                      mode=tfnode.DeviceStatusMode.STATUS_COMPACT, repeating=False):
        """Send status request to a ThermoFlex node"""
        
        # Build command
        node_command = tfnode.NodeCommand()
        status_command = tfnode.GetStatusCommand()
        status_command.device = device
        status_command.mode = mode
        status_command.repeating = repeating
        node_command.status = status_command
        
        # Create and send packet
        packet = Packet()
        packet.senderId = NodeAddress(id=[0x00, 0x00, 0x00], idType=NodeAddress.IDType.NodeID)
        packet.destinationId = target_address
        packet.data = node_command.serialize()
        packet.packetLength = packet.calculatePacketLength()
        packet.checksum = packet.calculateChecksum()
        
        self.network_interface.sendPacket(packet)
        
        # Track request for response matching
        request_id = f"{target_address}_{device}_{mode}"
        self.pending_requests[request_id] = {
            'timestamp': time.time(),
            'device': device,
            'mode': mode
        }
        
    def handle_response_packet(self, packet):
        """Process incoming status response packet"""
        try:
            node_response = tfnode.NodeResponse()
            node_response.deserialize(packet.data)
            
            if node_response.HasField('status_response'):
                self.process_status_response(node_response.status_response, packet.senderId)
                
        except Exception as e:
            print(f"Error processing status response: {e}")
            
    def process_status_response(self, status_response, sender_address):
        """Process a status response based on device and content type"""
        
        device = status_response.device
        print(f"Status response from {sender_address} device {device}")
        
        # Route to appropriate handler
        if status_response.HasField('node_status_compact'):
            self.handle_node_compact_status(status_response.node_status_compact)
            
        elif status_response.HasField('node_status_dump'):
            self.handle_node_dump_status(status_response.node_status_dump)
            
        elif status_response.HasField('sma_status_compact'):
            self.handle_muscle_compact_status(status_response.sma_status_compact)
            
        elif status_response.HasField('sma_status_dump'):
            self.handle_muscle_dump_status(status_response.sma_status_dump)
            
    # ... implement handler methods as shown above
```

## Firmware Debug Output

When the firmware receives and processes status requests, you should see debug output like:

```
SMA Controller Port 0 received status request - Mode: 1, Repeating: false
Sending single status response for Port 0
Sending SMA Status response for Port 0
```

This confirms the request was received and a response was generated.

## Troubleshooting

### No Status Responses Received
1. Verify packet addressing is correct (sender=0.0.0, destination=target node)
2. Check that status requests are properly serialized
3. Monitor firmware debug output to confirm request reception
4. Verify network interface is properly receiving response packets

### Incomplete Status Data
1. Ensure proper device targeting (request specific devices)
2. Check protobuf message field access (use `HasField()` before accessing)
3. Verify status mode matches expected response type

### Multiple Responses
When requesting `DEVICE_ALL`, expect separate response packets for:
- Node status (device=DEVICE_NODE)
- Port 0 status (device=DEVICE_PORT0) 
- Port 1 status (device=DEVICE_PORT1)

Each response must be handled individually as they arrive in separate packets.
