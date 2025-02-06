<!-- eslint-disable vue/no-v-html -->
<script setup lang="ts">
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
import AGA8wasm, {
  type MainModule,
  type GasMixture,
  type PropertiesDetailResult,
  type PropertiesGERGResult,
} from "@sctg/aga8-js";
import { initFlowbite } from "flowbite";
import { Chart } from "chart.js/auto";
import { onMounted, ref, type Ref, useTemplateRef } from "vue";
import type { CellValue, Worksheet } from "exceljs";
import { ScientificNotation } from "../utilities/scientific";
import Temml from "temml";
import DoubleRange from "../components/DoubleRange.vue";
import _nistGasMixture from "../../../examples/NG_Compositions.json" with { type: "json" };
import { Polyfit, type NumberArray } from "@sctg/polyfitjs";

type GasMixtureExt = {
  name: string;
  gasMixture: GasMixture;
};

type CellDefinition = {
  name: string;
  value: CellValue;
};

const THOROIDAL_RE_NUMBER = 16010500;
const nistGasMixture = _nistGasMixture as GasMixtureExt[];
const availableGasMixtures = [
  {
    name: "Air",
    gasMixture: {
      methane: 0,
      nitrogen: 0.7808,
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
      oxygen: 0.2095,
      carbon_monoxide: 0,
      water: 0.0004,
      hydrogen_sulfide: 0,
      helium: 0,
      argon: 0.0093,
    } as GasMixture,
  },
  {
    /**
     *  NIST Reference Gas Mixture composition in mole fraction
     */
    name: "NIST Reference Gas Mixture",
    gasMixture: {
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
      argon: 0.001,
    },
  },
  {
    /**
     * Pure methane gas mixture in mole fraction
     */
    name: "Methane",
    gasMixture: {
      methane: 1,
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
      hydrogen: 0,
      oxygen: 0,
      carbon_monoxide: 0,
      water: 0,
      hydrogen_sulfide: 0,
      helium: 0,
      argon: 0,
    },
  },
  {
    /**
     * Pure nitrogen gas mixture in mole fraction
     */
    name: "Nitrogen",
    gasMixture: {
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
      argon: 0,
    },
  },
  {
    /**
     * Custom gas mixture in mole fraction
     */
    name: "Custom",
    gasMixture: {
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
      hydrogen: 0,
      oxygen: 0,
      carbon_monoxide: 0,
      water: 0,
      hydrogen_sulfide: 0,
      helium: 0,
      argon: 0,
    },
  },
  ...nistGasMixture,
] as AvailableGasMixtures;

type Method = "DETAIL" | "GERG-2008";
type MassFlowRate = {
  massFlowRate: number; // mass flow rate in kg/s
  volumeFlowRateAtOutputPressure: number; // volume flow rate in m³/s
  temperature: number; // temperature in K
  pressure: number; // pressure in kPa
  specificNozzleCoefficient: number; // specific nozzle coefficient
  crticalPresure: number; // critical pressure in kPa
  M: number; // Molar mass
  kappa: number; // Isentropic exponent
  Cf: number; //	Critical Flow coefficient
};

const method = ref<Method>("DETAIL");
const menuOpen = ref(false);
const menuExcelOpen = ref(false);
const menu = useTemplateRef<HTMLDivElement>("menu");
const menuExcel = useTemplateRef<HTMLDivElement>("menuExcel");
const moduleLoaded = ref(false);
const doubleSlider = ref<{ from: Ref<number>; to: Ref<number> }>();
const flowChartKgS: Ref<HTMLCanvasElement | null> = ref(null);
const flowChartLs: Ref<HTMLCanvasElement | null> = ref(null);
const orificeDiameter = ref(0.05);
const selectedGasMixtureExt = ref<GasMixtureExt>(availableGasMixtures[0]);
const showGasDetails = ref(false);
const linearPolynomials = useTemplateRef<HTMLDivElement>("linearPolynomials");
let chartKgS: Chart | null = null;
let chartLs: Chart | null = null;
const R = 8.31446261815324; // Universal gas constant in J/(mol·K)
const nbGraphSteps = 1000;  // Number of steps for the graph
type AvailableGasMixtures = GasMixtureExt[];


/**
 * Reactive references to gas mixture components in mole percent
 */
const methaneConcentration = ref(0);
const nitrogenConcentration = ref(0);
const carbonDioxideConcentration = ref(0);
const ethaneConcentration = ref(0);
const propaneConcentration = ref(0);
const isobutaneConcentration = ref(0);
const nButaneConcentration = ref(0);
const isopentaneConcentration = ref(0);
const nPentaneConcentration = ref(0);
const nHexaneConcentration = ref(0);
const nHeptaneConcentration = ref(0);
const nOctaneConcentration = ref(0);
const nNonaneConcentration = ref(0);
const nDecaneConcentration = ref(0);
const hydrogenConcentration = ref(0);
const oxygenConcentration = ref(0);
const carbonMonoxideConcentration = ref(0);
const waterConcentration = ref(0);
const hydrogenSulfideConcentration = ref(0);
const heliumConcentration = ref(0);
const argonConcentration = ref(0);

const T = ref(273.15 + 20); // °K
const Pout = ref(250); // kPa
// const R = 8.31446261815324;      // J•mol^-1•K^-1)

const totalPercent = ref(100); // Total percentage

let AGA8: MainModule | null = null;

/**
 * Loads the AGA8 WASM module
 *
 * This function is called when the component is mounted
 *
 * @requires AGA8wasm - The AGA8 WASM module
 * @requires moduleLoaded - Reactive reference to the module load status
 */
onMounted(() => {
  initFlowbite();
  setGasMixture(
    availableGasMixtures.find((x) => x.name === "Air")?.gasMixture ||
      availableGasMixtures[0].gasMixture
  );
  AGA8wasm().then((AGA8module) => {
    AGA8 = AGA8module;
    moduleLoaded.value = true;
    console.warn("AGA8 module loaded");
  });
});

/**
 * Compute the discharge coefficient for a toroidal nozzle given the Reynolds number
 * cf: https://www.methyinfra.ptb.de/fileadmin/documents/empir-2021/methyinfra/documents/MetHyInfra_Nozzle_WS_MacDonald_Introduction_to_Sonic_Nozzles_and_ISO_9300.pdf p16
 * @param Re_thoroidal - Reynolds number for thoroidal nozzle
 */
function getThoroidalNozzleDischargeCoefficient(Re_thoroidal: number): number {
  const Cd_a = 0.9959; // Constant for toroidal nozzle
  const Cd_b = 2.72; // Reynolds number factor for toroidal nozzle
  const Cd_n = 0.5; // Reynolds number exponent for toroidal nozzle
  const Cd = Cd_a - Cd_b * Re_thoroidal ** (Cd_n * -1);
  return Cd;
}

/**
 * Compute the maximal outlet pressure for a given inlet pressure and critical flow
 * @param inletPressure
 * @param criticalFlow
 */
function getMaximalOutletPressure(
  inletPressure: number,
  criticalFlow: number
): number {
  const p_crit = inletPressure * criticalFlow;
  return p_crit;
}

/**
 * Compute the mass flow rate through a sonic nozzle at sonic conditions
 * @param propertiesMethod - Method to compute properties
 * @param gasMixture - Gas mixture composition
 * @param inletPressure - Inlet pressure in kPa
 * @param outletPressure - Outlet pressure in kPa
 * @param temperature - Temperature in K
 * @param orificeDiameter - Orifice diameter in mm
 * @param orificeReynoldsNumber - Reynolds number for the orifice
 * @returns {number} - Mass flow rate in kg/s
 */
function getMassFlowRateDataset(
  propertiesMethod: Method,
  gasMixture: GasMixture,
  inletPressureRange: { min: number; max: number },
  outletPressure: number,
  temperature: number,
  orificeDiameter: number,
  orificeReynoldsNumber: number
): MassFlowRate[] {
  const output: MassFlowRate[] = [];
  
  const minPressure = inletPressureRange.min; //doubleSlider.value?.from || DoubleRange.props.defaultMin;
  const maxPressure = inletPressureRange.max; //doubleSlider.value?.to || DoubleRange.props.defaultMax;
  console.warn(`Pressure range: ${minPressure} to ${maxPressure}`);
  if (!AGA8) {
    console.warn("AGA8 module is not loaded");
    throw new Error("AGA8 module is not loaded");
  }

  let properties: PropertiesDetailResult | PropertiesGERGResult;
  let molarMass = 0; // Molar mass in g/mol
  /** Discharge coefficient for thoroidal nozzle */
  const Cd = getThoroidalNozzleDischargeCoefficient(orificeReynoldsNumber); // Discharge coefficient
  /** Orifice area */
  const A = Math.PI * (orificeDiameter / 1000 / 2) ** 2; // m^2
  const SetupFunction =
    propertiesMethod === "DETAIL" ? AGA8.SetupDetail : AGA8.SetupGERG;
  const MolarMassFunction =
    propertiesMethod === "DETAIL" ? AGA8.MolarMassDetail : AGA8.MolarMassGERG;
  const PropertiesFunction =
    propertiesMethod === "DETAIL" ? AGA8.PropertiesDetail : AGA8.PropertiesGERG;
  SetupFunction();
  molarMass = MolarMassFunction(gasMixture); // g/mol
  const molarMassSI = molarMass / 1000; // kg/mol (SI units)
  const { D: D_out } =
    propertiesMethod === "DETAIL"
      ? AGA8.DensityDetail(temperature, outletPressure, gasMixture)
      : AGA8.DensityGERG(2, temperature, outletPressure, gasMixture); // mol/l
  const rho_out = D_out * 1000 * molarMassSI; // kg/m³
  for (let i = 0; i < nbGraphSteps; i++) {
    const P = minPressure + (i * (maxPressure - minPressure)) / nbGraphSteps;
    const { D } =
      propertiesMethod === "DETAIL"
        ? AGA8.DensityDetail(temperature, P, gasMixture)
        : AGA8.DensityGERG(2, temperature, P, gasMixture); // mol/l
    properties = PropertiesFunction(temperature, D, gasMixture);
    // Extract critical flow factor (Cf)
    const Cf = properties.Cf;
    /** Specific gas constant */
    const Rs = R / molarMassSI; // J/(kg·K)
    // Nozzle specific coefficient
    const Kn = (Cd * Cf * (P * 1000)) / Math.sqrt(Rs * temperature);
    // Calculate mass flow rate
    // Q = Cd * Cf * A * P / sqrt(Rs * T)
    // Q = Kn * A

    const massFlow = getMaximalOutletPressure(P, Cf) < outletPressure ? NaN : Kn * A; // kg/s
    output.push({ massFlowRate: massFlow,
                  volumeFlowRateAtOutputPressure:massFlow/rho_out, 
                  temperature, 
                  pressure: P, 
                  crticalPresure: getMaximalOutletPressure(P, Cf), 
                  specificNozzleCoefficient: Kn, 
                  kappa: properties.Kappa, 
                  Cf: properties.Cf, 
                  M: molarMassSI });
    //Kn is used for computing the diameter knowing the mass flow rate
    // A = Q / Kn
    // ∏ * (D/2)^2 = Q / Kn
    // D = sqrt(4 * Q / (Kn * ∏))
  }
  console.warn(
    `Mass flow rate at mid range: ${output[
      nbGraphSteps / 2
    ].massFlowRate.toPrecision(4)} kg/s 
    Volume flow rate at mid range: ${output[
      nbGraphSteps / 2
    ].volumeFlowRateAtOutputPressure.toPrecision(4)}m³/s Pressure: ${output[nbGraphSteps / 2].pressure
    } kPa, Temperature: ${output[nbGraphSteps / 2].temperature} K
      Critical pressure: ${output[nbGraphSteps / 2].crticalPresure} kPa`
  );
  // Polyfit
  const x:number[] = [];
  const y:number[] = [];
  for (let i = 0; i < output.length; i++) {
    if (!isNaN(output[i].massFlowRate)) {
      x.push(output[i].pressure);
      y.push(output[i].massFlowRate);
    }
  }
  createPolynomialDisplay(output);
  return output;
}

function getPolyfitMassFlow(dataset: MassFlowRate[]): {terms: NumberArray, correlation: number} {
  const x:number[] = [];
  const y:number[] = [];
  for (let i = 0; i < dataset.length; i++) {
    if (!isNaN(dataset[i].massFlowRate)) {
      x.push(dataset[i].pressure);
      y.push(dataset[i].massFlowRate);
    }
  }
  const _x64 = new Float64Array(x as number[]);
  const _y64 = new Float64Array(y as number[]);
  const polyfit = new Polyfit(_x64, _y64);
  const terms = polyfit.computeBestFit(100,0.999);
  return {terms, correlation: polyfit.correlationCoefficient(terms)};
}

function getPolyfitVolumeFlow(dataset: MassFlowRate[]): {terms: NumberArray, correlation: number} {
  const x:number[] = [];
  const y:number[] = [];
  for (let i = 0; i < dataset.length; i++) {
    if (!isNaN(dataset[i].volumeFlowRateAtOutputPressure)) {
      x.push(dataset[i].pressure);
      y.push(dataset[i].volumeFlowRateAtOutputPressure);
    }
  }
  const _x64 = new Float64Array(x as number[]);
  const _y64 = new Float64Array(y as number[]);
  const polyfit = new Polyfit(_x64, _y64);
  const terms = polyfit.computeBestFit(100,0.999);
  return {terms, correlation: polyfit.correlationCoefficient(terms)};
}

/**
 * Get the LaTeX representation of a polynomial
 * @param terms - Polynomial terms
 */
function getLatexPolynomial(terms : NumberArray): string {
  let latex = "";
  for(let i = terms.length - 1; i > 0; i--) {
    const coefficient = ScientificNotation.toScientificNotationLatex(terms[i], 4);
    latex += `${coefficient.startsWith('-') || (i !== terms.length) ? '' : '+'}${coefficient}\\cdot ${i > 1 ? `x^${i}` : 'x'} `;
  }
  const lastTerm = ScientificNotation.toScientificNotationLatex(terms[0], 4);
  latex += `${lastTerm.startsWith('-') ? '' : '+'}${lastTerm}`;
  return latex;
}

function createPolynomialDisplay(dataset: MassFlowRate[]){
  const {terms: massFlowTerms,correlation: massFlowCorrelation} = getPolyfitMassFlow(dataset);
  const {terms: volumeFlowTerms, correlation: volumeFlowCorrelation} = getPolyfitVolumeFlow(dataset);
  let massFlowLatex = "Q_{kg/s} \\left( x_{kPa} \\right) \\simeq ";
  massFlowLatex += getLatexPolynomial(massFlowTerms);

  let volumeFlowLatex = "Q_{(m^3\\cdot s^{-1})_{P_{out}}}\\left( x_{kPa} \\right) \\simeq ";
  volumeFlowLatex += getLatexPolynomial(volumeFlowTerms);

  if (linearPolynomials.value)
  {  linearPolynomials.value.innerHTML = "<p>" + 
    getMathMLFromLatex(massFlowLatex) + "<br/>" + 
    "<span class='text-xs'>" + getMathMLFromLatex(`R_{mass} = ${massFlowCorrelation.toPrecision(8)}`) + "</span><br/>" +
    getMathMLFromLatex(volumeFlowLatex) + "<br/>" +
    "<span class='text-xs'>" + getMathMLFromLatex(`R_{vol} = ${volumeFlowCorrelation.toPrecision(8)}`) + "</span><br/></p>";}
}

/**
 * Get the MathML representation of a LaTeX string
 * @param latex - LaTeX string
 */
function getMathMLFromLatex(latex: string): string {
  return Temml.renderToString(latex);
}

/**
 * Returns the gas mixture in mole percent
 *
 * @returns {GasMixture} - Array of gas mixture components in mole percent
 */
function getGasMixture(): GasMixture {
  return {
    methane: methaneConcentration.value / 100,
    nitrogen: nitrogenConcentration.value / 100,
    carbon_dioxide: carbonDioxideConcentration.value / 100,
    ethane: ethaneConcentration.value / 100,
    propane: propaneConcentration.value / 100,
    isobutane: isobutaneConcentration.value / 100,
    n_butane: nButaneConcentration.value / 100,
    isopentane: isopentaneConcentration.value / 100,
    n_pentane: nPentaneConcentration.value / 100,
    n_hexane: nHexaneConcentration.value / 100,
    n_heptane: nHeptaneConcentration.value / 100,
    n_octane: nOctaneConcentration.value / 100,
    n_nonane: nNonaneConcentration.value / 100,
    n_decane: nDecaneConcentration.value / 100,
    hydrogen: hydrogenConcentration.value / 100,
    oxygen: oxygenConcentration.value / 100,
    carbon_monoxide: carbonMonoxideConcentration.value / 100,
    water: waterConcentration.value / 100,
    hydrogen_sulfide: hydrogenSulfideConcentration.value / 100,
    helium: heliumConcentration.value / 100,
    argon: argonConcentration.value / 100,
  };
}

/**
 * Reset the gas mixture to a specific composition
 * @param {GasMixture} x - Array of gas mixture components in mole percent
 */

function setGasMixture(x: GasMixture): void {
  methaneConcentration.value = x.methane * 100;
  nitrogenConcentration.value = x.nitrogen * 100;
  carbonDioxideConcentration.value = x.carbon_dioxide * 100;
  ethaneConcentration.value = x.ethane * 100;
  propaneConcentration.value = x.propane * 100;
  isobutaneConcentration.value = x.isobutane * 100;
  nButaneConcentration.value = x.n_butane * 100;
  isopentaneConcentration.value = x.isopentane * 100;
  nPentaneConcentration.value = x.n_pentane * 100;
  nHexaneConcentration.value = x.n_hexane * 100;
  nHeptaneConcentration.value = x.n_heptane * 100;
  nOctaneConcentration.value = x.n_octane * 100;
  nNonaneConcentration.value = x.n_nonane * 100;
  nDecaneConcentration.value = x.n_decane * 100;
  hydrogenConcentration.value = x.hydrogen * 100;
  oxygenConcentration.value = x.oxygen * 100;
  carbonMonoxideConcentration.value = x.carbon_monoxide * 100;
  waterConcentration.value = x.water * 100;
  hydrogenSulfideConcentration.value = x.hydrogen_sulfide * 100;
  heliumConcentration.value = x.helium * 100;
  argonConcentration.value = x.argon * 100;
}

/**
 * Calculates the sum of mole percentages for all components in a gas mixture, excluding the first element.
 *
 * @param {GasMixture} x - Array of gas mixture components in mole percent
 * @returns {number} - Total concentration as sum of all components except first one
 */
function computeTotalConcentration(x: GasMixture): number {
  return Object.values(x).reduce((acc, val) => acc + val, 0);
}

/**
 * Checks if the total concentration of a gas mixture is 100%
 *
 * @param {GasMixture} x - Array of gas mixture components in mole percent
 * @returns {boolean} - True if total concentration is 100%, false otherwise
 */
function isTotalConcentrationValid(x: GasMixture): boolean {
  const concentration = computeTotalConcentration(x);
  const delta = Math.abs(1 - concentration);
  if (delta > 1e-12) {
    console.error(`Total concentration is not 100%: ${concentration * 100}%`);
  }
  totalPercent.value = concentration * 100;
  return delta <= 1e-12;
}

/**
 * Fill two colums with the content of data
 * @param sheet
 * @param topLeftCell
 * @param data
 */
function createNameValueColumns(sheet: Worksheet, topLeftCell: string, data: CellDefinition[]) {
  const [col, row] = topLeftCell.match(/([A-Z]+)(\d+)/)!.slice(1);
  const nameCol = col;
  const valueCol = String.fromCharCode(col.charCodeAt(0) + 1);
  
  data.forEach((item, index) => {
    const currentRow = parseInt(row) + index;
    
    // Column with the names
    const nameCell = sheet.getCell(`${nameCol}${currentRow}`);
    nameCell.value = item.name;
    nameCell.font = { bold: true };
    
    // Column with the values
    const valueCell = sheet.getCell(`${valueCol}${currentRow}`);
    valueCell.value = item.value;
    valueCell.protection = { locked: true };
  });
}

/**
 * Create an Excel file with the computed mass flow rate
 * @param data 
 */
async function getExcel(data: MassFlowRate[]): Promise<void>{
  try {
    const ExcelJS = (await import('exceljs')).default;
    const workbook = new ExcelJS.Workbook();
    workbook.title = 'Sonic flow rate calculation';
    workbook.subject = `${selectedGasMixtureExt.value.name} computation sheet`;
    workbook.creator = 'SCTG Development Team';
    workbook.description = `Build using https://sonic.lasersmart.work\nSource code available at https://github.com/sctg-development/aga8-js`;
    workbook.lastModifiedBy = 'https://sonic.lasersmart.work';
    workbook.created = new Date();
    workbook.modified = new Date();
    workbook.lastPrinted = new Date();
    // Set workbook dates to 1904 date system
    workbook.properties.date1904 = true;
    workbook.calcProperties.fullCalcOnLoad = true;

    const tableRows = [];
    for (let i = 0; i < data.length; i++) {
      tableRows.push([data[i].pressure, 
                      !isNaN(data[i].massFlowRate) ? {formula:`PI()*($G$1/2000)^2*D${i+2}`}: -1,
                      !isNaN(data[i].volumeFlowRateAtOutputPressure) ? data[i].volumeFlowRateAtOutputPressure : -1, 
                      data[i].specificNozzleCoefficient , 
                      {formula:`SQRT(4*$G$32/(D${i+2}*PI()))*1000`}]);
    };

    const sheet = workbook.addWorksheet(`Gas - ${selectedGasMixtureExt.value.name} at ${Pout.value/100} bar`);
    sheet.addTable({
      name: 'SonicFlowTable',
      ref: 'A1',
      headerRow: true,
      totalsRow: false,
      style: {
        theme: 'TableStyleMedium2',
        showRowStripes: true,
      },
      columns: [
        { name: 'Pressure (kPa)', filterButton: true },
        { name: 'Mass flow rate (kg/s) for G1 nozzle', filterButton: true },
        { name: `Volume flow rate (L/s) at ${Pout.value} kPa`, filterButton: true },
        { name: 'Specific nozzle coefficient', filterButton: true },
        { name: 'Nozzle (mm) for G32 target flow rate', filterButton: true }
      ],
      rows: tableRows,
    });
    // for(let i=0; i<data.length; i++){
    //   sheet2.getCell(`A${i+2}`).note = "Pressure in kPa";
    //   sheet2.getCell(`B${i+2}`).note = "Mass flow rate in kg/s\nQ = Kn * ∏*(D/2)^2";
    //   sheet2.getCell(`C${i+2}`).note = "Specific nozzle coefficient\nKn = Cd * Cf * P / sqrt(Rs * T)";
    //   sheet2.getCell(`D${i+2}`).note = "Nozzle diameter in mm\nD = sqrt(4 * Q / (Kn * ∏))";
    // }
    sheet.getColumn('A').width = 16;
    sheet.getColumn('A').style = { numFmt: '0.0', alignment: {horizontal: "right"} };
    sheet.getColumn('B').width = 30;
    sheet.getColumn('B').style = { numFmt: '0.000E+00', alignment: {horizontal: "right"} };
    sheet.getColumn('C').width = 30;
    sheet.getColumn('C').style = { numFmt: '0.000E+00', alignment: {horizontal: "right"} };
    sheet.getColumn('D').width = 30;
    sheet.getColumn('D').style = { numFmt: '0.000', alignment: {horizontal: "right"} };
    sheet.getColumn('E').width = 32;
    sheet.getColumn('E').style = { alignment: {horizontal: "right"} };
    sheet.getColumn('F').width = 22;
    sheet.getColumn('H').width = 22;
    sheet.getColumn('H').width = 14;
    const gasMixtureData: CellDefinition[] = [
      { name: "Nozzle diameter in mm", value: orificeDiameter.value },
      { name: "Inlet temperature in K", value: T.value },
      { name: "Inlet pressure min in kPa", value: doubleSlider.value?.from as unknown as number|| 0 },
      { name: "Inlet pressure max in kPa", value: doubleSlider.value?.to as unknown as number || 0 },
      { name: "Gas mixture composition", value: "Concentration (%)" },
      { name: "Methane", value: methaneConcentration.value },
      { name: "Nitrogen", value: nitrogenConcentration.value },
      { name: "Carbon dioxide", value: carbonDioxideConcentration.value },
      { name: "Ethane", value: ethaneConcentration.value },
      { name: "Propane", value: propaneConcentration.value },
      { name: "Isobutane", value: isobutaneConcentration.value },
      { name: "n-Butane", value: nButaneConcentration.value },
      { name: "Isopentane", value: isopentaneConcentration.value },
      { name: "n-Pentane", value: nPentaneConcentration.value },
      { name: "n-Hexane", value: nHexaneConcentration.value },
      { name: "n-Heptane", value: nHeptaneConcentration.value },
      { name: "n-Octane", value: nOctaneConcentration.value },
      { name: "n-Nonane", value: nNonaneConcentration.value },
      { name: "n-Decane", value: nDecaneConcentration.value },
      { name: "Hydrogen", value: hydrogenConcentration.value },
      { name: "Oxygen", value: oxygenConcentration.value },
      { name: "Carbon monoxide", value: carbonMonoxideConcentration.value },
      { name: "Water", value: waterConcentration.value },
      { name: "Hydrogen sulfide", value: hydrogenSulfideConcentration.value },
      { name: "Helium", value: heliumConcentration.value },
      { name: "Argon", value: argonConcentration.value },
      { name: "Total concentration", value: { formula: "SUM(G6:G26)" } },
      { name: "Kappa", value: data[0].kappa},
      { name: "Cf", value: data[0].Cf},
      { name: "Molar mass kg/mol", value: data[0].M},
      { name: "Cd", value: getThoroidalNozzleDischargeCoefficient(THOROIDAL_RE_NUMBER)},
      { name: "Target mass flow rate in kg/s", value: 0.000002 }
    ];
    createNameValueColumns(sheet, 'F1', gasMixtureData);
    const buffer = await workbook.xlsx.writeBuffer();
    const blob = new Blob([buffer], { type: 'application/vnd.openxmlformats-officedocument.spreadsheetml.sheet' });
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = `sonic_flow_rate_${selectedGasMixtureExt.value.name.replaceAll(' ','_')}.xlsx`;
    a.click();
    URL.revokeObjectURL(url);  }
  catch (error) {
    console.error("Error while loading ExcelJS", error);
  }
}

/**
 * Create all the charts
 * @param data 
 */
function createCharts(data: MassFlowRate[]): void {
  if (flowChartKgS.value && flowChartLs.value)
  {  createChartKgS(flowChartKgS.value,data);
     createChartLs(flowChartLs.value,data);}
}
/**
 * Create a chart.js for the mass flow rate in kg/s  as a function of the pressure in kPa
 * @param canvas The target element for hosting the chart
 * @param data a MassFlowRate[] array
 */
function createChartKgS(canvas: HTMLCanvasElement | null, data: MassFlowRate[]): void {
  if (!canvas) {
    return;
  }
  const ctx = canvas;
  if (!ctx) {
    return;
  }
  const labels = data.map((x) => x.pressure);
  const values = data.map((x) => x.massFlowRate);
  if (chartKgS) {
    chartKgS.destroy();
  }
  chartKgS = new Chart(ctx, {
    type: "line",
    data: {
      labels,
      datasets: [
        {
          label: "Mass flow rate in kg/s",
          data: values,
          borderColor: "rgb(75, 192, 192)",
          tension: 0,
          fill: false,
          pointStyle: "circle",
          pointBorderWidth: 0.1,
          pointRadius: 1,
          borderWidth: 1,
        },
      ],
    },
    options: {
      responsive: true,
      plugins: {
        tooltip: {
          callbacks: {
            label: function (context) {
              return `${context.dataset.label}: ${ScientificNotation.toScientificNotationString(context.parsed.y)} kg/s`;
            }
          }
        },
      },
      scales: {
        x: {
          ticks: {
            // For a category axis, the val is the index so the lookup via getLabelForValue is needed
            callback: function (_val, index) {
              // Hide every decimal tick label
              return (labels[index]) % 1 === 0 ? (labels[index]).toString() + ' kPa' : '';
            },
          }
        },
        y: {
          beginAtZero: true,
          ticks: {
            callback: function (value) {
              return ScientificNotation.toScientificNotationString(value as number) + ' kg/s';
            },
          },
        },
      },
    },
  });
}

/**
 * Create a chart.js linear chart for the volume flow rate in m³/s as a function of the pressure in kPa
 * @param canvas The target element for hosting the chart
 * @param data a MassFlowRate[] array
 */
function createChartLs(canvas: HTMLCanvasElement | null, data: MassFlowRate[]): void {
  if (!canvas) {
    return;
  }
  const ctx = canvas;
  if (!ctx) {
    return;
  }
  const labels = data.map((x) => x.pressure);
  const values = data.map((x) => x.volumeFlowRateAtOutputPressure*1000);
  const valuesStd = data.map((x) => x.volumeFlowRateAtOutputPressure*1000*Pout.value/101.325);
  if (chartLs) {
    chartLs.destroy();
  }
  chartLs = new Chart(ctx, {
    type: "line",
    data: {
      labels,
      datasets: [
        {
          label: `Volume flow rate in L/s at ${Pout.value} kPa`,
          data: values,
          borderColor: "rgb(75, 192, 192)",
          tension: 0,
          fill: false,
          pointStyle: "circle",
          pointBorderWidth: 0.1,
          pointRadius: 1,
          borderWidth: 1,
        },
        {
          label: `Volume flow rate in L/s at 1 atm`,
          data: valuesStd,
          borderColor: "rgb(192, 192, 192)",
          tension: 0,
          fill: false,
          pointStyle: "circle",
          pointBorderWidth: 0.1,
          pointRadius: 1,
          borderWidth: 1,
        },
      ],
    },
    options: {
      responsive: true,
      plugins: {
        tooltip: {
          callbacks: {
            label: function (context) {
              return `${context.dataset.label}: ${ScientificNotation.toScientificNotationString(context.parsed.y)}`;
            }
          }
        },
      },
      scales: {
        x: {
          ticks: {
            // For a category axis, the val is the index so the lookup via getLabelForValue is needed
            callback: function (_val, index) {
              // Hide every decimal tick label
              return (labels[index]) % 1 === 0 ? (labels[index]).toString() + ' kPa' : '';
            },
          }
        },
        y: {
          beginAtZero: true,
          ticks: {
            callback: function (value) {
              return ScientificNotation.toScientificNotationString(value as number) + ' L/s';
            },
          },
        },
      },
    },
  });
}
</script>
<template>
  <div>
    <p class="mt-1.5 text-sm text-gray-500">
      <b>A:</b> Area of the orifice, <b>D:</b> Diameter of the orifice,
      <b>C<sup>*</sup><sub>p</sub>:</b> Critical flow factor,
      <b>C<sub>d</sub>:</b> Discharge coefficient, <b>P<sub>in</sub>:</b> Inlet
      pressure,<b>𝜅</b>: Heat capacity ratio, <b>M</b>: Molar mass, <b>R</b>: Universal gas constant, 
      <b>R<sub>s</sub>:</b>
      Specific gaz constant, <b>T<sub>in</sub>:</b> Inlet temperature,
      <b>Q:</b> Flow rate.
    </p>
    <div class="flex flex-col lg:flex-row lg:items-center mt-1.5 text-xl text-gray-500">
      <div class="w-full lg:w-1/2">
        <div v-html="getMathMLFromLatex('R_s = \\frac{R}{M}')" />
        <div v-html="getMathMLFromLatex('A=\\pi \\cdot \\left( \\frac{D}{2} \\right)^2')" />
        <div v-html="getMathMLFromLatex('C_d=a-\\frac{b}{R_{e_{nt}}^n}')" />
        <div
          v-html="getMathMLFromLatex(
            'C^*_p = \\sqrt{\\kappa\\left(\\frac{ 2}{\\kappa + 1}\\right)^{\\frac{\\kappa + 1}{\\kappa - 1}}}'
          )
          "
        />
        <div v-html="getMathMLFromLatex('C_d=a-\\frac{b}{R_{e_{nt}}^n}')" />
        <div
          v-html="getMathMLFromLatex(
            'Q=A \\cdot C_d \\cdot C^*\\frac{P_{in}}{\\sqrt{R_s \\cdot T_{in}}} '
          )
          "
        />
      </div>
      <div class="w-full lg:w-1/2">
        <div ref="linearPolynomials" />
      </div>
    </div>
  </div>
  <div class="grid grid-cols-1 gap-4 lg:grid-cols-4 lg:gap-8">
    <div class="w-full h-24 rounded-lg bg-gray-200 flex items-center justify-center">
      <div>
        <label for="temperature" class="block text-xs font-medium text-gray-700">Temperature in K</label>
        <input
          id="temperature"
          v-model="T"
          type="number"
          placeholder="293.15"
          class="mt-1 w-40 rounded-md border-gray-200 shadow-sm sm:text-sm"
        >
      </div>
    </div>
    <div class="w-full h-24 rounded-lg bg-gray-200 flex items-center justify-center">
      <div>
        <label for="orifice" class="block text-xs font-medium text-gray-700">Orifice diameter in mm</label>
        <input
          id="orifice"
          v-model="orificeDiameter"
          type="number"
          placeholder="0.050"
          class="mt-1 w-40 rounded-md border-gray-200 shadow-sm sm:text-sm"
        >
      </div>
    </div>
    <div class="w-full h-24 rounded-lg bg-gray-200 flex items-center justify-center">
      <div>
        <label for="pressure" class="block text-xs font-medium text-gray-700">Outlet pressure in kPa</label>
        <input
          id="pressure"
          v-model="Pout"
          type="number"
          placeholder="250"
          class="mt-1 w-40 rounded-md border-gray-200 shadow-sm sm:text-sm"
        >
      </div>
    </div>
    <div class="relative">
      <div class="inline-flex items-center overflow-hidden rounded-md border bg-white border-gray-200 shadow-sm">
        <button
          :class="!isTotalConcentrationValid(getGasMixture())
            ? 'bg-gray-700'
            : 'bg-teal-600'
          "
          class="border-e px-4 py-2 text-sm/none text-white hover:bg-teal-500 hover:text-white"
          @click="
            createCharts(getMassFlowRateDataset(
              method,
              getGasMixture(),
              { min: doubleSlider?.from as unknown as number, max: doubleSlider?.to as unknown as number },
              Pout,
              T,
              orificeDiameter,
              THOROIDAL_RE_NUMBER
            ))
          "
        >
          {{
            isTotalConcentrationValid(getGasMixture())
              ? `Compute with ${method}`
              : `Total must be 100% (${totalPercent}%)`
          }}
        </button>
        <button class="h-full p-2 text-gray-600 hover:bg-gray-50 hover:text-gray-700" @click="menuOpen = !menuOpen">
          <span class="sr-only">Method</span>
          <svg
            xmlns="http://www.w3.org/2000/svg"
            class="size-4"
            viewBox="0 0 20 20"
            fill="currentColor"
          >
            <path
              fill-rule="evenodd"
              d="M5.293 7.293a1 1 0 011.414 0L10 10.586l3.293-3.293a1 1 0 111.414 1.414l-4 4a1 1 0 01-1.414 0l-4-4a1 1 0 010-1.414z"
              clip-rule="evenodd"
            />
          </svg>
        </button>
      </div>

      <div
        v-if="menuOpen"
        ref="menu"
        class="absolute start-0 z-10 mt-2 w-56 rounded-md border border-gray-100 bg-white shadow-lg"
      >
        <div class="p-2">
          <button
            class="block rounded-lg px-4 py-2 text-sm text-gray-500 hover:bg-gray-50 hover:text-gray-700"
            @click="
              method = 'GERG-2008';
              menuOpen = false;
            "
          >
            GERG-2008
          </button>

          <button
            class="block rounded-lg px-4 py-2 text-sm text-gray-500 hover:bg-gray-50 hover:text-gray-700"
            @click="
              method = 'DETAIL';
              menuOpen = false;
            "
          >
            DETAIL
          </button>
        </div>
      </div>
      <div class="inline-flex items-center overflow-hidden rounded-md border bg-white border-gray-200 shadow-sm">
        <button
          :class="!isTotalConcentrationValid(getGasMixture())
            ? 'bg-gray-700'
            : 'bg-teal-600'
          "
          class="border-e px-4 py-2 text-sm/none text-white hover:bg-teal-500 hover:text-white"
          @click="
            getExcel(getMassFlowRateDataset(
              method,
              getGasMixture(),
              { min: doubleSlider?.from as unknown as number, max: doubleSlider?.to as unknown as number },
              Pout,
              T,
              orificeDiameter,
              THOROIDAL_RE_NUMBER
            ))
          "
        >
          {{
            isTotalConcentrationValid(getGasMixture())
              ? `Excel with ${method}`
              : `Total must be 100% (${totalPercent}%)`
          }}
        </button>
        <button class="h-full p-2 text-gray-600 hover:bg-gray-50 hover:text-gray-700" @click="menuExcelOpen = !menuExcelOpen">
          <span class="sr-only">Method</span>
          <svg
            xmlns="http://www.w3.org/2000/svg"
            class="size-4"
            viewBox="0 0 20 20"
            fill="currentColor"
          >
            <path
              fill-rule="evenodd"
              d="M5.293 7.293a1 1 0 011.414 0L10 10.586l3.293-3.293a1 1 0 111.414 1.414l-4 4a1 1 0 01-1.414 0l-4-4a1 1 0 010-1.414z"
              clip-rule="evenodd"
            />
          </svg>
        </button>
      </div>

      <div
        v-if="menuExcelOpen"
        ref="menuExcel"
        class="absolute start-0 z-10 mt-2 w-56 rounded-md border border-gray-100 bg-white shadow-lg"
      >
        <div class="p-2">
          <button
            class="block rounded-lg px-4 py-2 text-sm text-gray-500 hover:bg-gray-50 hover:text-gray-700"
            @click="
              method = 'GERG-2008';
              menuExcelOpen = false;
            "
          >
            GERG-2008
          </button>

          <button
            class="block rounded-lg px-4 py-2 text-sm text-gray-500 hover:bg-gray-50 hover:text-gray-700"
            @click="
              method = 'DETAIL';
              menuExcelOpen = false;
            "
          >
            DETAIL
          </button>
        </div>
      </div>
    </div>
  </div>
  <div class="mt-8 grid grid-cols-1 gap-4 lg:grid-cols-2 lg:gap-8">
    <div class="rounded-lg bg-gray-200">
      <div class="m-1 overflow-x-auto">
        <div class="flex flex-col items-start mt-0.5 mx-1.5">
          <label for="doubleSlider" class="block text-xs font-medium text-gray-700">Inlet pressure range in kPa</label>
          <DoubleRange
            id="doubleSlider"
            ref="doubleSlider"
            class="w-full text-xs my-8"
            slider-color="oklch(0.6 0.118 184.704)"
            slider-off-color="oklch(0.707 0.022 261.325)"
            handle-size="1rem"
            :min="0"
            :max="10000"
            :default-min="200"
            :default-max="1000"
          />
        </div>
      </div>
      <div class="m-1 overflow-x-auto">
        <table class="rounded-md min-w-full divide-y-2 divide-gray-200 bg-white text-sm">
          <thead class="ltr:text-left rtl:text-right">
            <tr>
              <th class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <span @click="showGasDetails = !showGasDetails">Gas mixture composition</span>
                <button
                  id="dropdownDefaultButton"
                  data-dropdown-toggle="dropdown"
                  class="border border-gray-200 rounded text-sm text-gray-900 px-1 py-0.5 text-center inline-flex items-center"
                  type="button"
                >
                  {{ selectedGasMixtureExt.name }}
                  <svg
                    class="w-2.5 h-2.5 ms-3"
                    aria-hidden="true"
                    xmlns="http://www.w3.org/2000/svg"
                    fill="none"
                    viewBox="0 0 10 6"
                  >
                    <path
                      stroke="currentColor"
                      stroke-linecap="round"
                      stroke-linejoin="round"
                      stroke-width="2"
                      d="m1 1 4 4 4-4"
                    />
                  </svg>
                </button>

                <!-- Dropdown menu -->
                <div
                  id="dropdown"
                  class="z-10 hidden bg-white divide-y divide-gray-100 rounded-lg shadow-sm w-56 dark:bg-gray-700"
                >
                  <ul class="py-2 text-sm text-gray-700 dark:text-gray-200" aria-labelledby="dropdownDefaultButton">
                    <li v-for="gas in availableGasMixtures" :key="gas.name">
                      <a
                        href="#"
                        class="block px-4 py-2 hover:bg-gray-100 dark:hover:bg-gray-600 dark:hover:text-white"
                        @click="selectedGasMixtureExt = availableGasMixtures.find(_gas => _gas.name === gas.name); setGasMixture(gas.gasMixture)"
                      >{{
                        gas.name }}</a>
                    </li>
                  </ul>
                </div>
              </th>
              <th class="whitespace-nowrap px-4 py-2 font-medium text-gray-900" @click="showGasDetails = !showGasDetails">
                Concentration
              </th>
            </tr>
          </thead>
          <tbody v-if="(selectedGasMixtureExt.name === 'Custom') || showGasDetails" class="divide-y divide-gray-200">
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <label for="methane" class="block text-xs font-medium text-gray-700">Methane in %</label>
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <input
                  id="methane"
                  v-model="methaneConcentration"
                  type="number"
                  placeholder="0"
                  class="mt-1 w-40 rounded-md border-gray-200 shadow-sm sm:text-sm"
                >
              </td>
            </tr>
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <label for="nitrogen" class="block text-xs font-medium text-gray-700">Nitrogen in %</label>
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <input
                  id="nitrogen"
                  v-model="nitrogenConcentration"
                  type="number"
                  placeholder="1"
                  class="mt-1 w-40 rounded-md border-gray-200 shadow-sm sm:text-sm"
                >
              </td>
            </tr>
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <label for="carbonDioxide" class="block text-xs font-medium text-gray-700">Carbon Dioxide in %</label>
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <input
                  id="carbonDioxide"
                  v-model="carbonDioxideConcentration"
                  type="number"
                  placeholder="0"
                  class="mt-1 w-40 rounded-md border-gray-200 shadow-sm sm:text-sm"
                >
              </td>
            </tr>
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <label for="ethane" class="block text-xs font-medium text-gray-700">Ethane in %</label>
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <input
                  id="ethane"
                  v-model="ethaneConcentration"
                  type="number"
                  placeholder="0"
                  class="mt-1 w-40 rounded-md border-gray-200 shadow-sm sm:text-sm"
                >
              </td>
            </tr>
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <label for="propane" class="block text-xs font-medium text-gray-700">Propane in %</label>
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <input
                  id="propane"
                  v-model="propaneConcentration"
                  type="number"
                  placeholder="0"
                  class="mt-1 w-40 rounded-md border-gray-200 shadow-sm sm:text-sm"
                >
              </td>
            </tr>
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <label for="isobutane" class="block text-xs font-medium text-gray-700">Isobutane in %</label>
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <input
                  id="isobutane"
                  v-model="isobutaneConcentration"
                  type="number"
                  placeholder="0"
                  class="mt-1 w-40 rounded-md border-gray-200 shadow-sm sm:text-sm"
                >
              </td>
            </tr>
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <label for="nButane" class="block text-xs font-medium text-gray-700">n-Butane in %</label>
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <input
                  id="nButane"
                  v-model="nButaneConcentration"
                  type="number"
                  placeholder="0"
                  class="mt-1 w-40 rounded-md border-gray-200 shadow-sm sm:text-sm"
                >
              </td>
            </tr>
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <label for="isopentane" class="block text-xs font-medium text-gray-700">Isopentane in %</label>
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <input
                  id="isopentane"
                  v-model="isopentaneConcentration"
                  type="number"
                  placeholder="0"
                  class="mt-1 w-40 rounded-md border-gray-200 shadow-sm sm:text-sm"
                >
              </td>
            </tr>
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <label for="nPentane" class="block text-xs font-medium text-gray-700">n-Pentane in %</label>
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <input
                  id="nPentane"
                  v-model="nPentaneConcentration"
                  type="number"
                  placeholder="0"
                  class="mt-1 w-40 rounded-md border-gray-200 shadow-sm sm:text-sm"
                >
              </td>
            </tr>
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <label for="nHexane" class="block text-xs font-medium text-gray-700">n-Hexane in %</label>
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <input
                  id="nHexane"
                  v-model="nHexaneConcentration"
                  type="number"
                  placeholder="0"
                  class="mt-1 w-40 rounded-md border-gray-200 shadow-sm sm:text-sm"
                >
              </td>
            </tr>
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <label for="nHeptane" class="block text-xs font-medium text-gray-700">n-Heptane in %</label>
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <input
                  id="nHeptane"
                  v-model="nHeptaneConcentration"
                  type="number"
                  placeholder="0"
                  class="mt-1 w-40 rounded-md border-gray-200 shadow-sm sm:text-sm"
                >
              </td>
            </tr>
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <label for="nOctane" class="block text-xs font-medium text-gray-700">n-Octane in %</label>
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <input
                  id="nOctane"
                  v-model="nOctaneConcentration"
                  type="number"
                  placeholder="0"
                  class="mt-1 w-40 rounded-md border-gray-200 shadow-sm sm:text-sm"
                >
              </td>
            </tr>
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <label for="nNonane" class="block text-xs font-medium text-gray-700">n-Nonane in %</label>
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <input
                  id="nNonane"
                  v-model="nNonaneConcentration"
                  type="number"
                  placeholder="0"
                  class="mt-1 w-40 rounded-md border-gray-200 shadow-sm sm:text-sm"
                >
              </td>
            </tr>
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <label for="nDecane" class="block text-xs font-medium text-gray-700">n-Decane in %</label>
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <input
                  id="nDecane"
                  v-model="nDecaneConcentration"
                  type="number"
                  placeholder="0"
                  class="mt-1 w-40 rounded-md border-gray-200 shadow-sm sm:text-sm"
                >
              </td>
            </tr>
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <label for="hydrogen" class="block text-xs font-medium text-gray-700">Hydrogen in %</label>
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <input
                  id="hydrogen"
                  v-model="hydrogenConcentration"
                  type="number"
                  placeholder="0"
                  class="mt-1 w-40 rounded-md border-gray-200 shadow-sm sm:text-sm"
                >
              </td>
            </tr>
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <label for="oxygen" class="block text-xs font-medium text-gray-700">Oxygen in %</label>
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <input
                  id="oxygen"
                  v-model="oxygenConcentration"
                  type="number"
                  placeholder="0"
                  class="mt-1 w-40 rounded-md border-gray-200 shadow-sm sm:text-sm"
                >
              </td>
            </tr>
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <label for="carbonMonoxide" class="block text-xs font-medium text-gray-700">Carbon Monoxide in %</label>
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <input
                  id="carbonMonoxide"
                  v-model="carbonMonoxideConcentration"
                  type="number"
                  placeholder="0"
                  class="mt-1 w-40 rounded-md border-gray-200 shadow-sm sm:text-sm"
                >
              </td>
            </tr>
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <label for="water" class="block text-xs font-medium text-gray-700">Water in %</label>
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <input
                  id="water"
                  v-model="waterConcentration"
                  type="number"
                  placeholder="0"
                  class="mt-1 w-40 rounded-md border-gray-200 shadow-sm sm:text-sm"
                >
              </td>
            </tr>
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <label for="hydrogenSulfide" class="block text-xs font-medium text-gray-700">Hydrogen Sulfide in
                  %</label>
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <input
                  id="hydrogenSulfide"
                  v-model="hydrogenSulfideConcentration"
                  type="number"
                  placeholder="0"
                  class="mt-1 w-40 rounded-md border-gray-200 shadow-sm sm:text-sm"
                >
              </td>
            </tr>
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <label for="helium" class="block text-xs font-medium text-gray-700">Helium in %</label>
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <input
                  id="helium"
                  v-model="heliumConcentration"
                  type="number"
                  placeholder="0"
                  class="mt-1 w-40 rounded-md border-gray-200 shadow-sm sm:text-sm"
                >
              </td>
            </tr>
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <label for="argon" class="block text-xs font-medium text-gray-700">Argon in %</label>
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <input
                  id="argon"
                  v-model="argonConcentration"
                  type="number"
                  placeholder="0"
                  class="mt-1 w-40 rounded-md border-gray-200 shadow-sm sm:text-sm"
                >
              </td>
            </tr>
          </tbody>
        </table>
      </div>
    </div>
    <div class="rounded-lg bg-gray-200">
      <div class="m-1 overflow-x-auto">
        <canvas id="flowChartKgS" ref="flowChartKgS" />
      </div>
    </div>
  </div>
  <div>
    <canvas id="flowChartLs" ref="flowChartLs" />
  </div>
</template>
