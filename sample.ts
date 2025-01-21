// first install the package
// npm install @sctg/aga8-js
import AGA8wasm, { gazMixtureInMolePercent } from '@sctg/aga8-js';

async function calculateGERGProperties() {
    // Initialize GERG-2008 module
    const AGA8 = await AGA8wasm();
    AGA8.SetupGERG();

    // Gas mixture composition (mole fractions, must sum to 1)
    const gasMixture:gazMixtureInMolePercent = [
        0,        // [0] PLACEHOLDER (must be 0)
        0.77824,  // [1] METHANE
        0.02,     // [2] NITROGEN
        0.06,     // [3] CARBON DIOXIDE
        0.08,     // [4] ETHANE
        0.03,     // [5] PROPANE
        0.0015,   // [6] ISOBUTANE
        0.003,    // [7] N-BUTANE
        0.0005,   // [8] ISOPENTANE
        0.00165,  // [9] N-PENTANE
        0.00215,  // [10] N-HEXANE
        0.00088,  // [11] N-HEPTANE
        0.00024,  // [12] N-OCTANE
        0.00015,  // [13] N-NONANE
        0.00009,  // [14] N-DECANE
        0.004,    // [15] HYDROGEN
        0.005,    // [16] OXYGEN
        0.002,    // [17] CARBON MONOXIDE
        0.0001,   // [18] WATER
        0.0025,   // [19] HYDROGEN SULFIDE
        0.007,    // [20] HELIUM
        0.001     // [21] ARGON
    ];

    // Operating conditions
    const temperature = 400;   // Kelvin
    const pressure = 50000;    // kPa

    // Property calculations
    const molarMass = AGA8.MolarMassGERG(gasMixture);
    const density = AGA8.DensityGERG(0, temperature, pressure, gasMixture);
    const properties = AGA8.PropertiesGERG(temperature, density.D, gasMixture);

    // Output results
    console.log('GERG-2008 Gas Properties:');
    console.log(`Molar mass: ${molarMass.toFixed(6)} g/mol`);
    console.log(`Density: ${density.D.toFixed(6)} mol/l`);
    console.log(`Pressure: ${properties.P.toFixed(2)} kPa`);
    console.log(`Compressibility factor: ${properties.Z.toFixed(6)}`);
    console.log(`Internal energy: ${properties.U.toFixed(2)} J/mol`);
    console.log(`Enthalpy: ${properties.H.toFixed(2)} J/mol`);
}

calculateGERGProperties().catch(console.error);