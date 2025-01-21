// first install the package
// npm install @sctg/aga8-js
import AGA8wasm, { gazMixtureInMolePercent } from '@sctg/aga8-js';

// Gas mixture composition (mole fractions, must sum to 1)
const gasMixture: gazMixtureInMolePercent = [
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

const pureNitrogen: gazMixtureInMolePercent = [
    0,        // [0] PLACEHOLDER (must be 0)
    0,        // [1] METHANE
    1,        // [2] NITROGEN
    0,        // [3] CARBON DIOXIDE
    0,        // [4] ETHANE
    0,        // [5] PROPANE
    0,        // [6] ISOBUTANE
    0,        // [7] N-BUTANE
    0,        // [8] ISOPENTANE
    0,        // [9] N-PENTANE
    0,        // [10] N-HEXANE
    0,        // [11] N-HEPTANE
    0,        // [12] N-OCTANE
    0,        // [13] N-NONANE
    0,        // [14] N-DECANE
    0,        // [15] HYDROGEN
    0,        // [16] OXYGEN
    0,        // [17] CARBON MONOXIDE
    0,        // [18] WATER
    0,        // [19] HYDROGEN SULFIDE
    0,        // [20] HELIUM
    0         // [21] ARGON
];

const pureHydrogen: gazMixtureInMolePercent = [
    0,        // [0] PLACEHOLDER (must be 0)
    0,        // [1] METHANE
    0,        // [2] NITROGEN
    0,        // [3] CARBON DIOXIDE
    0,        // [4] ETHANE
    0,        // [5] PROPANE
    0,        // [6] ISOBUTANE
    0,        // [7] N-BUTANE
    0,        // [8] ISOPENTANE
    0,        // [9] N-PENTANE
    0,        // [10] N-HEXANE
    0,        // [11] N-HEPTANE
    0,        // [12] N-OCTANE
    0,        // [13] N-NONANE
    0,        // [14] N-DECANE
    1,        // [15] HYDROGEN
    0,        // [16] OXYGEN
    0,        // [17] CARBON MONOXIDE
    0,        // [18] WATER
    0,        // [19] HYDROGEN SULFIDE
    0,        // [20] HELIUM
    0         // [21] ARGON
];

async function calculateGERGProperties(gaz: gazMixtureInMolePercent): Promise<void> {
    // Initialize GERG-2008 module
    const AGA8 = await AGA8wasm();
    AGA8.SetupGERG();

    // Operating conditions
    const temperature = 273.15+20;   // Kelvin
    const pressure = 101.3;    // kPa

    // Property calculations
    const molarMass = AGA8.MolarMassGERG(gaz);
    const density = AGA8.DensityGERG(0, temperature, pressure, gaz);
    const properties = AGA8.PropertiesGERG(temperature, density.D, gaz);

    //      P - Pressure (kPa)
    //      Z - Compressibility factor
    //   dPdD - First derivative of pressure with respect to density at constant temperature [kPa/(mol/l)]
    // d2PdD2 - Second derivative of pressure with respect to density at constant temperature [kPa/(mol/l)^2]
    // d2PdTD - Second derivative of pressure with respect to temperature and density [kPa/(mol/l)/K]
    //   dPdT - First derivative of pressure with respect to temperature at constant density (kPa/K)
    //      U - Internal energy (J/mol)
    //      H - Enthalpy (J/mol)
    //      S - Entropy [J/(mol-K)]
    //     Cv - Isochoric heat capacity [J/(mol-K)]
    //     Cp - Isobaric heat capacity [J/(mol-K)]
    //      W - Speed of sound (m/s)
    //      G - Gibbs energy (J/mol)
    //     JT - Joule-Thomson coefficient (K/kPa)
    //  Kappa - Isentropic Exponent
    //      A - Helmholtz energy (J/mol)

    // Output results
    console.log('GERG-2008 Gas Properties:');
    console.log(`Molar mass: ${molarMass.toFixed(6)} g/mol`);
    console.log(`Density: ${density.D.toFixed(6)} mol/L`);
    console.log(`Pressure: ${properties.P.toFixed(2)} kPa`);
    console.log(`Compressibility factor: ${properties.Z.toFixed(6)}`);
    console.log(`Internal energy: ${properties.U.toFixed(2)} J/mol`);
    console.log(`Enthalpy: ${properties.H.toFixed(2)} J/mol`);
    console.log(`Entropy: ${properties.S.toFixed(2)} J/mol/K`);
    console.log(`Isobaric heat capacity Cp: ${properties.Cp.toFixed(6)} J/Mol/K`);
    console.log(`Speed of sound: ${properties.W.toFixed(2)} m/s`);
    console.log(`heat Isentropic Factor Cp/Cv: ${properties.Cp / properties.Cv} `);
}

console.log('+++++++++++++ Gas Mixture');
await calculateGERGProperties(gasMixture).catch(console.error);

console.log('');
console.log('+++++++++++++ Pure Nitrogen');
await calculateGERGProperties(pureNitrogen).catch(console.error);

console.log('');
console.log('+++++++++++++ Pure Hydrogen');
await calculateGERGProperties(pureHydrogen).catch(console.error);