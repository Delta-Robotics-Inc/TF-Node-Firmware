# Closed-Loop Current Control Investigation

## Observed Issues

- In `SMAController::update()` the case for `MODE_AMPS` simply divides the desired setpoint by the measured current to choose the PWM duty cycle. This is a proportional approach without any feedback stability and can easily saturate when the measured current is near zero.
- `readMuscleAmps()` notes an unexplained 5 A offset in the sensed current. This makes accurate control difficult without calibration.
- No dedicated PID or other closed loop logic exists for current control unlike the `ResistiveController` used for resistance mode.

## Plan of Action

1. **Calibrate Current Measurement**
   - Determine the zero-current offset of the amplifier and add a constant (or runtime calibration/trim) to correct `readMuscleAmps()`.
   - Verify scaling based on `R_SNS` and `AMP_GAIN` to ensure accurate amp readings.
2. **Create a Current PID Controller**
   - Implement a `CurrentController` class similar to `ResistiveController` using the existing `MiniPID` library.
   - Configure PID gains and output limits (0‑1 PWM duty) via `SMAControllerSettings`.
3. **Integrate PID in `SMAController`**
   - Add an instance of `CurrentController` to `SMAController`.
   - Replace the `MODE_AMPS` logic with PID update steps: `pid.update(curr_val)` and assign `pwm_duty_percent` to the controller output.
   - Include anti‑windup and smoothing of sensor data as needed.
4. **Testing**
   - Unit-test the new controller with simulated currents.
   - Compile firmware and perform bench tests to verify the PWM response tracks the requested current.

This approach will allow true closed-loop current regulation and eliminate the large errors currently observed.

## Implemented Architecture (as of 2025-06-12)

### CurrentController Class

- **Architecture**: PID-based controller using the existing `MiniPID` library
- **Configuration**:
  - Output limits: 0-75% PWM (safety constraint)
  - Ramp rate: 10 (prevents sudden changes)
  - Output filtering: 0.1 (smooths response)
  - Max I output: 20% PWM (anti-windup protection)
- **Interface**: Mirrors `ResistiveController` pattern for consistency
  - `update(float newCurrent)` - Updates PID with current measurement
  - `getOutput()` - Returns PWM duty cycle (0.0-1.0) with safety bounds
  - `setSetpoint(float newTargetCurrent)` - Updates target current
  - `Reset()` - Clears PID state for clean transitions

### SMAController Integration

- **PID Gains**: Hardcoded constants (KP_cc=0.1, KI_cc=0.02, KD_cc=0.01)
- **Initialization**: `currController` created in `begin()` method alongside `resController`
- **MODE_AMPS Implementation** (lines 38-42):
  ```cpp
  case tfnode::SMAControlMode::MODE_AMPS:
      // Scale measured current by PWM duty cycle to get average current delivered to load
      // This compensates for measuring peak current during PWM "on" time only
      float avg_current = curr_val * pwm_duty_percent;
      
      currController->setSetpoint(setpoint[(int)tfnode::SMAControlMode::MODE_AMPS]);
      currController->update(avg_current);             // Update pid controller with duty-cycle-scaled current measurement
      pwm_duty_percent = currController->getOutput(); // Get PID output for PWM duty cycle
      break;
  ```
- **State Management**: `currController->Reset()` called in `CMD_setEnable()` for clean transitions

### Critical PWM-Current Measurement Issue (Fixed)

- **Problem**: The current sensor reads the same instantaneous current (peak) regardless of PWM duty cycle because it only samples during the PWM "On" period (after a `measure_delay_us` of 400μs). This breaks the PID feedback loop because the controller can't "see" the effect of changing PWM duty.
- **Solution (Implemented)**: Scale the instantaneous current measurement by PWM duty cycle (i.e. multiply `curr_val` by `pwm_duty_percent`) to approximate the average current delivered to the SMA actuator. This restores proper PID feedback so that the controller "sees" the average current (e.g. a 5 A peak at 10% PWM is scaled to 0.5 A average).
- **Next Steps (Hardware)**: Consider adding a low-pass RC filter (e.g. 10 ms time constant) on the current sense amplifier output so that the ADC "sees" the true average current automatically. (This is a "preferred" hardware fix.)

### Verification Testing

- **Unit Tests** (test/test_current_controller.cpp) cover:
  - Zero current (large error) response
  - Near-target (reduced output) response
  - At-target (minimal output) response
  - Setpoint changes (dynamic response)
  - Reset (state clearing) functionality
- **Validation**: All outputs constrained to 0.0–1.0 PWM bounds.

### Next Steps (as of 2024-03-xx)

- **Verify** (unit tests) that the PWM-scaled current (avg_current) is computed and fed into the PID update (i.e. that the "critical" fix is working as intended).
- **Bench test** (with actual hardware) to confirm that the PWM duty cycle (pwm_duty_percent) "tracks" the requested (setpoint) current.
- **Tune PID gains** (currently KP=0.1, KI=0.02, KD=0.01) based on bench test (system) response.
- **Calibrate** (if needed) the 5 A offset (or "trim") in readMuscleAmps().
- **Consider** (future) a hardware low-pass filter (or "Option 2") for a "true" average current measurement (or "Option 3" – time-based integration – if more accuracy is required).
