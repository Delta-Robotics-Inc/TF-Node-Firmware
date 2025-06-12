# Sensor Validation Plan

This document describes the procedure for validating voltage and current readouts across multiple ThermoFlex Nodes. The goal is to ensure that every controller reports consistent sensor values so that global voltage and current limits can be applied safely.

## Objectives

1. Verify that supply voltage measurements are consistent across Nodes.
2. Measure no-load current draw of each Node for comparison.
3. Compare current pulse responses between Nodes to check for calibration issues.

## Test Scripts

Three Python scripts located in `test/node_validation` implement these checks using the [ThermoFlex Python API](https://github.com/Delta-Robotics-Inc/ThermoFlex-Python-API):

| Script | Purpose |
|-------|---------|
| `read_voltage.py` | Logs supply voltage for all detected Nodes over a user defined time interval. |
| `no_load_current.py` | Measures the idle current of each muscle port with no load attached. |
| `current_pulse.py` | Applies a configurable current setpoint and records the resulting current curve. |

Each script outputs a CSV file with time stamps, Node ID and measured values. The CSVs can be imported into Excel for further analysis or plotting.

### Example Usage

```bash
python read_voltage.py --duration 10 --outfile voltages.csv
python no_load_current.py --duration 10 --outfile idle_current.csv
python current_pulse.py --target 2.5 --pulse 1.0 --duration 5 --outfile pulse.csv
```

## Data Analysis

1. Import the CSV results from multiple devices into a spreadsheet.
2. Plot the recorded curves with separate colors for each Node.
3. Compare voltage and current readings to identify any offsets or discrepancies.
4. If readings match across devices, production firmware can enforce global limits; otherwise a calibration step is required for each Node.

