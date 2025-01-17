import math

# Geometry Parameters
diameter_mm = 2  # Diameter in millimeters
diameter_m = diameter_mm / 1000  # Convert to meters
length = 0.3  # Length in meters

# Material Parameters
niti_grade = "NiTi #5"
percentage_nickel = 0.495  # Percentage of nickel (as a fraction)
percentage_titanium = 0.505  # Percentage of titanium (as a fraction)
density_nickel = 8908  # kg/m^3
density_titanium = 4507  # kg/m^3



# Calculate density of Nitinol
density_nitinol = (percentage_nickel * density_nickel + 
                   percentage_titanium * density_titanium)

# Calculate volume of the wire
volume = math.pi * (diameter_m / 2)**2 * length

# Calculate mass of the wire
mass_kg = volume * density_nitinol
mass_g = mass_kg * 1000  # Convert to grams

print(f"Density of Nitinol ({niti_grade}): {density_nitinol:.2f} kg/m^3")
print(f"Volume of the wire: {volume:.2e} m^3")
print(f"Mass of the wire: {mass_g:.3f} g")
