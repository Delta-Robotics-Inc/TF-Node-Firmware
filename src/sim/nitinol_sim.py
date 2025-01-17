# Simulate the resistance change of nitinol
import numpy as np
import matplotlib.pyplot as plt

# Constants and parameters
Rho_M = 1e-6  # Resistivity of martensite
Rho_A = 0.8e-6  # Resistivity of austenite
Rho_R = 1.2e-6  # Resistivity of R-phase
M_f, M_s, A_s, A_f, R_s = 60, 70, 75, 90, 73  # Transformation temperatures
E_M, E_A = 28e9, 75e9  # Elastic moduli
Epsilon_L = 0.05  # Transformation strain

Length = 0.1  # Wire length in meters
Diameter_mm = 1.0  # Diameter in millimeters
Area = np.pi * (Diameter_mm / 1000 / 2) ** 2  # Cross-sectional area in square meters
Specific_heat = 450  # J/(kg*K)
Mass = 0.01  # Mass in kg

Heat_loss = 10  # Heat dissipation constant
Ambient_temp = 25  # Ambient temperature in Celsius

# Simulation time and input
time = np.linspace(0, 10, 1000)  # 10 seconds, 1000 steps
current = 10.0 * (time > 2)  # Step input current after 2 seconds

# Initial conditions
temperature = np.ones_like(time) * Ambient_temp
resistance = np.zeros_like(time)
displacement = np.zeros_like(time)

# Simulation loop
for t in range(1, len(time)):
    # Temperature dynamics
    resistance[t - 1] = Rho_A * Length / Area  # Start with austenite resistance
    power = current[t] ** 2 * resistance[t - 1]
    temp_change = power / (Mass * Specific_heat) - Heat_loss * (temperature[t - 1] - Ambient_temp) / (Mass * Specific_heat)
    temperature[t] = temperature[t - 1] + temp_change

    # Phase transitions
    if temperature[t] < M_s:
        phase_fraction = 1  # Fully martensite
    elif temperature[t] > A_f:
        phase_fraction = 0  # Fully austenite
    else:
        phase_fraction = (A_f - temperature[t]) / (A_f - A_s)

    # Resistance calculation
    rho = Rho_M * phase_fraction + Rho_A * (1 - phase_fraction)
    resistance[t] = rho * Length / Area

    # Displacement calculation
    strain = Epsilon_L * phase_fraction
    displacement[t] = strain * Length

# Print results
print("temp:", temperature)
print("res:", resistance)
print("displacement:", displacement)

# Plotting results
plt.figure()
plt.subplot(3, 1, 1)
plt.plot(time, temperature)
plt.ylabel("Temperature (°C)")

plt.subplot(3, 1, 2)
plt.plot(time, resistance)
plt.ylabel("Resistance (Ω)")

plt.subplot(3, 1, 3)
plt.plot(time, displacement)
plt.ylabel("Displacement (m)")
plt.xlabel("Time (s)")

plt.tight_layout()
plt.show()
