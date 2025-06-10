# Closed-Loop Current Control Investigation

## Observed Issues

- In `SMAController::update()` the case for `MODE_AMPS` simply divides the desired setpoint by the measured current to choose the PWM duty cycle. This is a proportional approach without any feedback stability and can easily saturate when the measured current is near zero.
- `readMuscleAmps()` notes an unexplained 5 A offset in the sensed current. This makes accurate control difficult without calibration.
- No dedicated PID or other closed loop logic exists for current control unlike the `ResistiveController` used for resistance mode.

## Plan of Action

1. **Calibrate Current Measurement**
   - Determine the zero-current offset of the amplifier and add a constant (or runtime calibration) to correct `readMuscleAmps()`.
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
