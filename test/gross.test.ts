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
import {AGA8wasm, type GasMixture} from '../dist/index.js';

describe('Gross', () => {
  const EPSILON = 1.0e-8;

  // Reference values
  const mm_reference = 20.54333051000000;
  const D_reference = 5.117641317088482;
  const P_reference = 9999.999999999998;
  const Z_reference = 0.7833795701012788;

  test('Gross properties calculation', async () => {
    const AGA8 = await AGA8wasm();
    AGA8.SetupGross();

    // Input Parameters
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

    const T = 300;
    const P = 10000;
    const Td = 273.15;
    const Th = 298.15;
    const Pd = 101.325;

    // Calculations
    const mm = AGA8.MolarMassGross(x);
    const grossHvResult = AGA8.GrossHv(x);

    const densityResult = AGA8.DensityGross(T, P, grossHvResult.xGrs, grossHvResult.HCH);
    const D = densityResult.D;

    const pressureResult = AGA8.PressureGross(T, D, grossHvResult.xGrs, grossHvResult.HCH);
    const pp = pressureResult.P;
    const Z = pressureResult.Z;

    // Assertions with the same accuracy as the C++ test
    expect(Math.abs(mm_reference - mm)).toBeLessThan(EPSILON);
    expect(Math.abs(D_reference - D)).toBeLessThan(EPSILON);
    expect(Math.abs(P_reference - pp)).toBeLessThan(EPSILON);
    expect(Math.abs(Z_reference - Z)).toBeLessThan(EPSILON);
  });
});