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
import AGA8wasm, {type GasMixture} from '../dist/index.js';

describe('Detail', () => {
  const EPSILON = 1.0e-8;
  
  // References values from Detail.cpp main() function
  const references = {
    mm: 20.54333051,
    D: 12.80792403648801,
    P: 50000.0,
    Z: 1.173801364147326,
    dPdD: 6971.387690924090,
    d2PdD2: 1118.803636639520,
    dPdT: 235.6641493068212,
    U: -2739.134175817231,
    H: 1164.699096269404,
    S: -38.54882684677111,
    Cv: 39.12076154430332,
    Cp: 58.54617672380667,
    W: 712.6393684057903,
    G: 16584.22983497785,
    JT: 7.432969304794577E-05,
    Kappa: 2.672509225184606,
    Cf: 0.8388544508582146,
  };

  test('Detail properties calculation', async () => {
    const AGA8 = await AGA8wasm();
    AGA8.SetupDetail();

    // Composition du gaz
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
    // Pressure and temperature
    const T = 400;
    const P = 50000;

    // Compute molar mass
    const mm = AGA8.MolarMassDetail(x);

    // Compute density
    const densityResult = AGA8.DensityDetail(T, P, x);
    const D = densityResult.D;

    // Compute properties
    const props = AGA8.PropertiesDetail(T, D, x);

    // Check results
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