/**
 * AGA8-2008 Sample Gas Properties Calculation
 * 
 * @copyright Copyright (c) 2024 Ronan LE MEILLAT
 * @license AGPL-3.0-or-later
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

// first install the package
// npm install --save @sctg/aga8-js
import AGA8wasm, { type GasMixture } from '@sctg/aga8-js';

// Gas mixture composition (mole fractions, must sum to 1)
// NIST Reference Gas Mixture
const gasMixture: GasMixture = {
    methane: 0.77824,
    nitrogen: 0.02,
    carbon_dioxide: 0.06,
    ethane: 0.08,
    propane: 0.03,
    isobutane: 0.0015,
    n_butane: 0.003,
    isopentane: 0.0005,
    n_pentane: 0.00165,
    n_hexane: 0.00215,
    n_heptane: 0.00088,
    n_octane: 0.00024,
    n_nonane: 0.00015,
    n_decane: 0.00009,
    hydrogen: 0.004,
    oxygen: 0.005,
    carbon_monoxide: 0.002,
    water: 0.0001,
    hydrogen_sulfide: 0.0025,
    helium: 0.007,
    argon: 0.001
}
const pureNitrogen: GasMixture = {
    methane: 0,
    nitrogen: 1,
    carbon_dioxide: 0,
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
    oxygen: 0,
    carbon_monoxide: 0,
    water: 0,
    hydrogen_sulfide: 0,
    helium: 0,
    argon: 0
}
const pureHydrogen: GasMixture = {
    methane: 0,
    nitrogen: 0,
    carbon_dioxide: 0,
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
    hydrogen: 1,
    oxygen: 0,
    carbon_monoxide: 0,
    water: 0,
    hydrogen_sulfide: 0,
    helium: 0,
    argon: 0
}

async function calculateGERGProperties(gaz: GasMixture): Promise<void> {
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