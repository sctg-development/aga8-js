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
import AGA8wasm, {gazMixtureInMolePercent} from '../dist/aga8';

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
    // METHANE = 1,
    // NITROGEN = 2,
    // CARBON_DIOXIDE = 3,
    // ETHANE = 4,
    // PROPANE = 5,
    // ISOBUTANE = 6,
    // N_BUTANE = 7,
    // ISOPENTANE = 8,
    // N_PENTANE = 9,
    // N_HEXANE = 10,
    // N_HEPTANE = 11,
    // N_OCTANE = 12,
    // N_NONANE = 13,
    // N_DECANE = 14,
    // HYDROGEN = 15,
    // OXYGEN = 16,
    // CARBON_MONOXIDE = 17,
    // WATER = 18,
    // HYDROGEN_SULFIDE = 19,
    // HELIUM = 20,
    // ARGON = 21

    // Input Parameters
    const x: gazMixtureInMolePercent = [
      0, // padding initial
      0.77824, 0.02, 0.06, 0.08, 0.03, 0.0015, 0.003, 0.0005,
      0.00165, 0.00215, 0.00088, 0.00024, 0.00015, 0.00009,
      0.004, 0.005, 0.002, 0.0001, 0.0025, 0.007, 0.001
    ];

    const T = 300;
    const P = 10000;
    const Td = 273.15;
    const Th = 298.15;
    const Pd = 101.325;

    // Calculations
    const mm = AGA8.MolarMassGross(x);
    const grossHvResult = AGA8.GrossHv(x);
    const { xGrs, HN, HCH } = grossHvResult;

    const densityResult = AGA8.DensityGross(T, P, xGrs, HCH);
    const D = densityResult.D;

    const pressureResult = AGA8.PressureGross(T, D, xGrs, HCH);
    const pp = pressureResult.P;
    const Z = pressureResult.Z;

    // Assertions with the same accuracy as the C++ test
    expect(Math.abs(mm_reference - mm)).toBeLessThan(EPSILON);
    expect(Math.abs(D_reference - D)).toBeLessThan(EPSILON);
    expect(Math.abs(P_reference - pp)).toBeLessThan(EPSILON);
    expect(Math.abs(Z_reference - Z)).toBeLessThan(EPSILON);
  });
});