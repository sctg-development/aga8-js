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
import AGA8wasm, { gazMixtureInMolePercent, PropertiesGERGResult } from '../dist/index.js';

describe('GERG2008', () => {
  const EPSILON = 1.0e-8;

  // Reference values
  const references = {
    mm: 20.5427445016,
    D: 12.79828626082062,
    P: 50000.0,
    Z: 1.174690666383717,
    dPdD: 7000.694030193327,
    d2PdD2: 1129.526655214841,
    dPdT: 235.9832292593096,
    U: -2746.492901212530,
    H: 1160.280160510973
  };

  test('GERG2008 properties calculation', async () => {
    const AGA8 = await AGA8wasm();
    AGA8.SetupGERG();

    // Gas composition
    const x: gazMixtureInMolePercent = [
      0, // padding initial
      0.77824, 0.02, 0.06, 0.08, 0.03, 0.0015, 0.003, 0.0005,
      0.00165, 0.00215, 0.00088, 0.00024, 0.00015, 0.00009,
      0.004, 0.005, 0.002, 0.0001, 0.0025, 0.007, 0.001
    ];

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
  });
});