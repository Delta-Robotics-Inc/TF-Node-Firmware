# Muscle Cooldown Curve Test Plan

This experiment measures the natural cooling profile of a ThermoFlex dual–coil Nitinol actuator at room temperature (25 °C). Understanding the passive cooldown time is important for estimating cycle rates and designing closed‑loop control.

## Setup

1. Mount the ThermoFlex muscle under no load.
2. Connect the actuator to a Node controller powered by a battery or benchtop supply.
3. Attach temperature sensors (thermocouples or IR camera) to monitor wire temperature.
4. Ensure ambient room temperature is held near 25 °C with minimal airflow.

## Procedure

1. Heat the muscle to a fully contracted state using a constant current setpoint.
2. Disable power and begin logging temperature vs time at 50 ms intervals until the wire returns to ambient temperature.
3. Repeat the test for several cycles to ensure repeatability.
4. Record ambient temperature throughout the experiment.

## Data Logging

Use the `current_pulse.py` script to apply the heating pulse while simultaneously capturing current feedback. Temperature data can be logged using a separate data acquisition system. Store both datasets in a common CSV for analysis.

## Analysis

- Plot temperature vs time for each trial to generate the cooldown curve.
- Fit an exponential decay model to estimate the thermal time constant.
- Compare curves across samples to assess consistency.

This cooldown profile informs actuator modeling and sets expectations for how quickly the muscle can be cycled in typical applications.

