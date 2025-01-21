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
import AGA8wasm, {gazMixtureInMolePercent} from '../dist/index.js';

describe('GERG2008', () => {
  test('GERG properties calculation', async () => {
    // Initialisation
    const AGA8 = await AGA8wasm();
    AGA8.SetupGERG();
    
    // Input data
    const x: gazMixtureInMolePercent = [
      0, // padding initial
      0.77824, 0.02, 0.06, 0.08, 0.03, 0.0015, 0.003, 0.0005, 
      0.00165, 0.00215, 0.00088, 0.00024, 0.00015, 0.00009, 
      0.004, 0.005, 0.002, 0.0001, 0.0025, 0.007, 0.001
    ];

    // Calculation of molar mass
    const mm = AGA8.MolarMassGERG(x);

    // Input Parameters
    const T = 400;
    const P = 50000;

    // Calculation of density
    const densityResult = AGA8.DensityGERG(0, T, P, x);
    const D = densityResult.D;

    // Calculation of properties
    const props = AGA8.PropertiesGERG(T, D, x);

    // Checks
    expect(mm).toBeCloseTo(20.54274450160000, 10);
    expect(D).toBeCloseTo(12.79828626082062, 10);
    expect(props.P).toBeCloseTo(50000.00000000001, 10);
    expect(props.Z).toBeCloseTo(1.174690666383717, 10);
    expect(props.dPdD).toBeCloseTo(7000.694030193327, 10);
    expect(props.d2PdD2).toBeCloseTo(1129.526655214841, 10);
    expect(props.dPdT).toBeCloseTo(235.9832292593096, 10);
    expect(props.U).toBeCloseTo(-2746.492901212530, 10);
    expect(props.H).toBeCloseTo(1160.280160510973, 10);
    expect(props.S).toBeCloseTo(-38.57590392409089, 10);
  });
});