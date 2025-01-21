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

describe('Detail', () => {
  const EPSILON = 1.0e-8;
  
  // Valeurs de référence
  const references = {
    mm: 20.54333051,
    D: 12.80792403648801,
    P: 50000.0,
    Z: 1.173801364147326,
    dPdD: 6971.387690924090,
    d2PdD2: 1118.803636639520,
    dPdT: 235.6641493068212,
    U: -2739.134175817231
  };

  test('Detail properties calculation', async () => {
    const AGA8 = await AGA8wasm();
    AGA8.SetupDetail();

    // Composition du gaz
    const x: gazMixtureInMolePercent = [
      0, // padding initial
      0.77824, 0.02, 0.06, 0.08, 0.03, 0.0015, 0.003, 0.0005,
      0.00165, 0.00215, 0.00088, 0.00024, 0.00015, 0.00009,
      0.004, 0.005, 0.002, 0.0001, 0.0025, 0.007, 0.001
    ];

    // Paramètres d'entrée
    const T = 400;
    const P = 50000;

    // Calcul de la masse molaire
    const mm = AGA8.MolarMassDetail(x);

    // Calcul de la densité
    const densityResult = AGA8.DensityDetail(T, P, x);
    const D = densityResult.D;

    // Calcul des propriétés
    const props = AGA8.PropertiesDetail(T, D, x);

    // Vérifications
    expect(Math.abs(references.mm - mm)).toBeLessThan(EPSILON);
    expect(Math.abs(references.D - D)).toBeLessThan(EPSILON);
    expect(Math.abs(references.P - props.P)).toBeLessThan(EPSILON);
    expect(Math.abs(references.Z - props.Z)).toBeLessThan(EPSILON);
    expect(Math.abs(references.dPdD - props.dPdD)).toBeLessThan(EPSILON);
    expect(Math.abs(references.d2PdD2 - props.d2PdD2)).toBeLessThan(EPSILON);
    expect(Math.abs(references.dPdT - props.dPdT)).toBeLessThan(EPSILON);
    expect(Math.abs(references.U - props.U)).toBeLessThan(EPSILON);
  });
});