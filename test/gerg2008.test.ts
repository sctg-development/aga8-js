/**
 * Copyright (C) 2025 Ronan LE MEILLAT
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */
import { describe, expect, test } from '@jest/globals';
import AGA8wasm, { type GasMixture, PropertiesGERGResult } from '../dist/index.js';

describe('GERG2008', () => {
  const EPSILON = 1.0e-8;

  // Reference values from GERG2008.cpp main() function
  const references = {
    mm: 20.5427445016,
    D: 12.79828626082062,
    P: 50000.0,
    Z: 1.174690666383717,
    dPdD: 7000.694030193327,
    d2PdD2: 1129.526655214841, // Note that GERG2008.cpp has 1130.481239114938, 1129.526655214841 is the result from the C++ test
    dPdT: 235.9832292593096,
    U: -2746.492901212530,
    H: 1160.280160510973,
    S: -38.57590392409089,
    Cv: 39.02948218156372,
    Cp: 58.45522051000366,
    W: 714.4248840596024,
    G: 16590.64173014733,
    JT: 7.155629581480913E-05,
    Kappa: 2.683820255058032,
    Cf: 0.8398521837767355,
  };

  test('GERG2008 properties calculation', async () => {
    const AGA8 = await AGA8wasm();
    AGA8.SetupGERG();

    // Input data
    const x: GasMixture = {
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
    };

    // Input parameters
    const T = 400;
    const P = 50000;

    // Calculations
    const mm = AGA8.MolarMassGERG(x);
    const densityResult = AGA8.DensityGERG(0, T, P, x);
    const D = densityResult.D;
    const props: PropertiesGERGResult = AGA8.PropertiesGERG(T, D, x);

    // Checks with the same precision as C++ testing
    expect(Math.abs(references.mm - mm)).toBeLessThan(EPSILON);
    expect(Math.abs(references.D - D)).toBeLessThan(EPSILON);
    expect(Math.abs(references.P - props.P)).toBeLessThan(EPSILON);
    expect(Math.abs(references.Z - props.Z)).toBeLessThan(EPSILON);
    expect(Math.abs(references.dPdD - props.dPdD)).toBeLessThan(EPSILON);
    expect(Math.abs(references.d2PdD2 - props.d2PdD2)).toBeLessThan(EPSILON);
    expect(Math.abs(references.dPdT - props.dPdT)).toBeLessThan(EPSILON);
    expect(Math.abs(references.U - props.U)).toBeLessThan(EPSILON);
    expect(Math.abs(references.H - props.H)).toBeLessThan(EPSILON);
    expect(Math.abs(references.S - props.S)).toBeLessThan(EPSILON);
    expect(Math.abs(references.Cv - props.Cv)).toBeLessThan(EPSILON);
    expect(Math.abs(references.Cp - props.Cp)).toBeLessThan(EPSILON);
    expect(Math.abs(references.W - props.W)).toBeLessThan(EPSILON);
    expect(Math.abs(references.G - props.G)).toBeLessThan(EPSILON);
    expect(Math.abs(references.JT - props.JT)).toBeLessThan(EPSILON);
    expect(Math.abs(references.Kappa - props.Kappa)).toBeLessThan(EPSILON);
    expect(Math.abs(references.Cf - props.Cf)).toBeLessThan(EPSILON);
  });
});