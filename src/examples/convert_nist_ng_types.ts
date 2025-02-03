/**
 * @copyright Copyright (c) 2024-2025 Ronan LE MEILLAT
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

import * as fs from 'fs';
import * as path from 'path';
import * as csv from 'fast-csv';
import type { GasMixture } from '@sctg/aga8-js';

const fileIn = path.resolve('./NG_Compositions.csv');
const fileOut = path.resolve('./NG_Compositions.json');
export type GasMixtureCsv = {
    methane: string,
    nitrogen: string,
    carbon_dioxide: string,
    ethane: string,
    propane: string,
    isobutane: string,
    n_butane: string,
    isopentane: string,
    n_pentane: string,
    n_hexane: string,
    n_heptane: string,
    n_octane: string,
    n_nonane: string,
    n_decane: string,
    hydrogen: string,
    oxygen: string,
    carbon_monoxide: string,
    water: string,
    hydrogen_sulfide: string,
    helium: string,
    argon: string
}

export type GasMixtureExt = {
    name: string;
    gasMixture: GasMixture;
};

const testGasMixtures = [] as GasMixtureExt[];
fs.createReadStream(fileIn)
    .pipe(csv.parse<GasMixtureCsv, GasMixture>({ headers: true }).transform((row: GasMixtureCsv) => {
        return {
            methane: parseFloat(row.methane)/100,
            nitrogen: parseFloat(row.nitrogen)/100,
            carbon_dioxide: parseFloat(row.carbon_dioxide)/100,
            ethane: parseFloat(row.ethane)/100,
            propane: parseFloat(row.propane)/100,
            isobutane: parseFloat(row.isobutane)/100,
            n_butane: parseFloat(row.n_butane)/100,
            isopentane: parseFloat(row.isopentane)/100,
            n_pentane: parseFloat(row.n_pentane)/100,
            n_hexane: parseFloat(row.n_hexane)/100,
            n_heptane: parseFloat(row.n_heptane)/100,
            n_octane: parseFloat(row.n_octane)/100,
            n_nonane: parseFloat(row.n_nonane)/100,
            n_decane: parseFloat(row.n_decane)/100,
            hydrogen: parseFloat(row.hydrogen)/100,
            oxygen: parseFloat(row.oxygen)/100,
            carbon_monoxide: parseFloat(row.carbon_monoxide)/100,
            water: parseFloat(row.water)/100,
            hydrogen_sulfide: parseFloat(row.hydrogen_sulfide)/100,
            helium: parseFloat(row.helium)/100,
            argon: parseFloat(row.argon)/100
        };
    }))
    .on('error', error => console.error(error))
    .on('data', row => { console.log(row); testGasMixtures.push({ name: `NIST NG#${testGasMixtures.length + 2}`, gasMixture: row }) })
    .on('end', (rowCount: number) => { fs.writeFileSync(fileOut, JSON.stringify(testGasMixtures, null, 2)); console.log(`Parsed ${rowCount} rows`) });
