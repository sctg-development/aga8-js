import {AGA8wasm, type GasMixture } from '@sctg/aga8-js';

// Air composition (mole fractions)
const air: GasMixture = {
    methane: 0,
    nitrogen: 0.7808,
    carbon_dioxide: 0.0004,
    ethane: 0,
    propane: 0,
    isobutane: 0,
    n_butane: 0,
    isopentane: 0,
    n_pentane: 0,
    n_hexane: 0,
    n_heptane: 0,
    n_octane: 0,
    n_nonane: 0,
    n_decane: 0,
    hydrogen: 0,
    oxygen: 0.2095,
    carbon_monoxide: 0,
    water: 0.0,      // Assuming dry air
    hydrogen_sulfide: 0,
    helium: 0,
    argon: 0.0093
};

async function calculateSonicNozzleFlow(): Promise<void> {
    // Initialize GERG-2008 module
    const AGA8 = await AGA8wasm();
    AGA8.SetupGERG();

    // Operating conditions
    const temperature = 273.15 + 20;   // 20°C in Kelvin
    const inletPressure = 400;         // 4 bar in kPa
    const outletPressure = 101.325;    // 1 atm in kPa
    const d = 0.050;                   // Throat diameter in mm
    const A = Math.PI * (d/2000)**2;   // Throat area in m²

    // Constants for toroidal nozzle
    const Re_thoroidal_max = 3.2e7; // Maximal Reynolds number for toroidal nozzle
    const Re_thoroidal_min = 2.1e4; // Minimal Reynolds number for toroidal nozzle
    const Cd_a = 0.9959 ; // Constant for toroidal nozzle
    const Cd_b = 2.720 ; // Reynolds number factor for toroidal nozzle
    const Cd_n = 0.5 ;    // Reynolds number exponent for toroidal nozzle
    const Cd_max = Cd_a - Cd_b * Re_thoroidal_min**(Cd_n * (-1));                   // Typical discharge coefficient for toroidal sonic nozzle
    const Cd_min = Cd_a - Cd_b * Re_thoroidal_max**(Cd_n * (-1));                   // Typical discharge coefficient for toroidal sonic nozzle
    const Cd_geometric_mean = Math.sqrt(Cd_max * Cd_min); // Geometric mean of discharge coefficients
    const Cd = Cd_geometric_mean; // Use geometric mean of discharge coefficients
    // Calculate gas properties
    const molarMass = AGA8.MolarMassGERG(air);                          // g/mol
    const { D } = AGA8.DensityGERG(0, temperature, inletPressure, air); // mol/L
    const { D: D_out } = AGA8.DensityGERG(0, temperature, outletPressure, air); // mol/L
    const properties = AGA8.PropertiesGERG(temperature, D, air);
    const molarMassSI = molarMass / 1000; // kg/mol (SI units)
    const densitySI = D * 1000; // mol/m³ (SI units)
    // Extract critical flow factor (Cf)
    const Cf = properties.Cf;
    const R = 8.31446261815324;      // Universal gas constant in J/(mol·K)
    /** Specific gas constant */
    const Rs = R / molarMassSI ;        // J/(kg·K)

    // Maximal outlet pressure (critical flow)
    const p_crit = (inletPressure) * Cf ; // kPa
    // Calculate mass flow rate
    // Q = Cd * Cf * A * P / sqrt(Rs * T)
    const massFlow = Cd * Cf * A * (inletPressure*1000) / Math.sqrt( Rs * temperature ); // kg/s

    const rho = densitySI * molarMassSI; // kg/m³
    const rho_out = D_out * 1000 * molarMassSI; // kg/m³

    // Output results
    console.log('Sonic Nozzle Flow Calculation (ISO 9300:2022)');
    console.log('Input conditions:');
    console.log(`Temperature: ${(temperature-273.15).toPrecision(2)}°C`);
    console.log(`Inlet pressure: ${inletPressure.toPrecision(3)} kPa (${(inletPressure/100).toPrecision(3)} bar)`);
    console.log(`Outlet pressure: ${outletPressure.toPrecision(3)} kPa (${(outletPressure/100).toPrecision(3)} bar)`);
    console.log(`Throat diameter: ${d.toPrecision(4)} mm`);
    console.log(`Throat area: ${(A*1e6).toPrecision(4)}mm² (${(A).toPrecision(4)} m²)`);
    console.log(`Maximal discharge coefficient: ${Cd_max.toPrecision(4)}`);
    console.log(`Minimal discharge coefficient: ${Cd_min.toPrecision(4)}`);
    console.log(`Used discharge coefficients: ${Cd.toPrecision(4)}`);
    console.log('\nGas properties at inlet conditions:');
    console.log(`Molar mass: ${molarMass.toPrecision(4)} g/mol`);
    console.log(`Specific gas constant: ${Rs.toPrecision(4)} J/(kg·K)`);
    console.log(`Density: ${rho.toPrecision(4)} kg/m³`);
    console.log(`Critical flow factor (Cf): ${Cf.toPrecision(6)}`);
    console.log(`Heat capacity ratio (κ): ${properties.Kappa.toPrecision(6)}`);
    console.log('\nGas properties at outlet conditions:');
    console.log(`Density: ${rho_out.toPrecision(4)} kg/m³`);
    console.log('\nResults:');
    console.log(`Outlet pressure must be : <${(p_crit).toPrecision(2)} kPa ${p_crit > outletPressure ? '✅' : '❌'}`);
    console.log(`Mass flow rate: ${massFlow.toPrecision(4)} kg/s`);
    console.log(`Mass flow rate: ${(massFlow*1000*3600).toPrecision(4)} g/h`);
    console.log(`Mass flow rate: ${(massFlow*1000*60).toPrecision(4)} g/min`);
    console.log(`Volume flow at outlet: ${(massFlow/rho_out).toPrecision(4)} m³/s (${(massFlow/rho_out*1000*3600).toPrecision(4)} L/h)`);
}

calculateSonicNozzleFlow().catch(console.error);
