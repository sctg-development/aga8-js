<!-- eslint-disable vue/no-v-html -->
<script setup lang="ts">
/**
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
import AGA8wasm, { type MainModule, type GasMixture, type PropertiesDetailResult, type PropertiesGERGResult } from '@sctg/aga8-js'
import Clipboard from '../components/Clipboard.vue';
import { onMounted, ref, type VNodeRef, type Ref } from 'vue';
import Temml from 'temml';
import DoubleRange from '../components/DoubleRange.vue';

type Method = 'DETAIL' | 'GERG-2008';
const method = ref<Method>('DETAIL');
const menuOpen = ref(false);
const menu = ref<VNodeRef | null>(null);
const moduleLoaded = ref(false);
const doubleSlider = ref<{ from: Ref<number>, to: Ref<number> }>();
type GasMixtureExt = {
  name: string;
  gasMixture: GasMixture;
};

type AvailableGasMixtures = GasMixtureExt[];

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
      water: 0.000400,
      hydrogen_sulfide: 0,
      helium: 0,
      argon: 0.009300,
    } as GasMixture
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
      argon: 0.001
    }
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
      argon: 0
    }
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
      argon: 0
    }
  }
] as AvailableGasMixtures;


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
const P = ref(400);     // kPa
const Pout = ref(200);  // kPa
// const R = 8.31446261815324;      // J•mol^-1•K^-1)
const mm = ref(0); // Molar mass in g/mol
const density = ref(0);  // Density in mol/l
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
  setGasMixture(availableGasMixtures.find(x => x.name === "Air")?.gasMixture || availableGasMixtures[0].gasMixture);
  AGA8wasm().then((AGA8module) => {
    AGA8 = AGA8module;
    moduleLoaded.value = true;
    console.warn("AGA8 module loaded");
  })
})

/**
 * Compute the discharge coefficient for a toroidal nozzle given the Reynolds number
 * @param Re_thoroidal - Reynolds number for thoroidal nozzle
 */
function getThoroidalNozzleDischargeCoefficient(Re_thoroidal: number): number {
  const Cd_a = 0.9959; // Constant for toroidal nozzle
  const Cd_b = 2.720; // Reynolds number factor for toroidal nozzle
  const Cd_n = 0.5;    // Reynolds number exponent for toroidal nozzle
  const Cd = Cd_a - Cd_b * Re_thoroidal ** (Cd_n * (-1));
  return Cd;
}

/**
 * Compute the maximal outlet pressure for a given inlet pressure and critical flow
 * @param inletPressure 
 * @param criticalFlow 
 */
function getMaximalOutletPressure(inletPressure: number, criticalFlow: number): number {
  const p_crit = (inletPressure) * criticalFlow;
  return p_crit;
}

/**
 * Compute the mass flow rate through a sonic nozzle at sonic conditions
 * @param propertiesMethod - Method to compute properties
 * @param gasMixture - Gas mixture composition
 * @param inletPressure - Inlet pressure in kPa
 * @param temperature - Temperature in K
 * @param orificeDiameter - Orifice diameter in mm
 * @param orificeReynoldsNumber - Reynolds number for the orifice
 * @returns {number} - Mass flow rate in kg/s
 */
function getMassFlowRate(propertiesMethod: Method, gasMixture: GasMixture, inletPressure: number, temperature: number, orificeDiameter: number, orificeReynoldsNumber: number): number {
  const minPressure = doubleSlider.value?.from || DoubleRange.props.defaultMin;
  const maxPressure = doubleSlider.value?.to || DoubleRange.props.defaultMax;
  console.warn(`Pressure range: ${minPressure} to ${maxPressure}`);
  let properties: PropertiesDetailResult | PropertiesGERGResult;
  let molarMass: number;
  if (!AGA8) {
    console.warn("AGA8 module is not loaded");
    throw new Error("AGA8 module is not loaded");
  }
  if (propertiesMethod === 'DETAIL') {
    AGA8.SetupDetail();
    const { D } = AGA8.DensityDetail(temperature, inletPressure, gasMixture); // mol/L
    properties = AGA8.PropertiesDetail(temperature, D, gasMixture);
    molarMass = AGA8.MolarMassDetail(gasMixture);
  } else {
    AGA8.SetupGERG();
    const { D } = AGA8.DensityGERG(0, temperature, inletPressure, gasMixture); // mol/L
    properties = AGA8.PropertiesGERG(temperature, D, gasMixture);
    molarMass = AGA8.MolarMassGERG(gasMixture);
  }
  const molarMassSI = molarMass / 1000; // kg/mol (SI units)
  // Extract critical flow factor (Cf)
  const Cf = properties.Cf;
  const R = 8.31446261815324;      // Universal gas constant in J/(mol·K)
  /** Specific gas constant */
  const Rs = R / molarMassSI;        // J/(kg·K)
  /** Discharge coefficient for thoroidal nozzle */
  const Cd = getThoroidalNozzleDischargeCoefficient(orificeReynoldsNumber); // Discharge coefficient
  /** Orifice area */
  const A = Math.PI * (orificeDiameter / 1000 / 2) ** 2; // m^2
  // Maximal outlet pressure (critical flow)
  // Calculate mass flow rate
  // Q = Cd * Cf * A * P / sqrt(Rs * T)
  const massFlow = Cd * Cf * A * (inletPressure * 1000) / Math.sqrt(Rs * temperature); // kg/s
  console.warn(`Mass flow rate: ${massFlow.toPrecision(4)} kg/s (${massFlow * 3.6e6} g/h)`);
  return massFlow;
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
  }
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
  return (delta <= 1e-12);
}
</script>
<template>
  <div>
    <p class="mt-1.5 text-sm text-gray-500">
      <b>A:</b> Area of the orifice, <b>D:</b> Diameter of the orifice, <b>C<sup>*</sup><sub>p</sub>:</b> Critical flow
      factor,
      <b>C<sub>d</sub>:</b> Discharge coefficient, <b>P<sub>in</sub>:</b> Inlet pressure,𝜅: Heat capacity ratio,
      <b>R<sub>s</sub>:</b>
      Specific gaz constant, <b>T<sub>in</sub>:</b> Inlet temperature, <b>Q:</b> Flow rate.
    </p>
    <div class="mt-1.5 text-xl text-gray-500" v-html="getMathMLFromLatex('A=\\pi\\cdot(\\frac{D}{2})^2')" />
    <div
      class="mt-1.5 text-xl text-gray-500"
      v-html="getMathMLFromLatex('C^*_p = \\sqrt{\\kappa\\left(\\frac{ 2}{\\kappa + 1}\\right)^{\\frac{\\kappa + 1}{\\kappa - 1}}}')"
    />
    <div class="mt-1.5 text-xl text-gray-500" v-html="getMathMLFromLatex('C_d=a-\\frac{b}{R_{e_{nt}}^n}')" />
    <div
      class="mt-1.5 text-xl text-gray-500"
      v-html="getMathMLFromLatex('Q=A \\cdot C_d \\cdot C^*\\frac{P_{in}}{\\sqrt{R_s \\cdot T_{in}}} ')"
    />
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
        <label for="pressure" class="block text-xs font-medium text-gray-700">Inlet pressure in kPa</label>
        <input
          id="pressure"
          v-model="P"
          type="number"
          placeholder="400"
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
          placeholder="400"
          class="mt-1 w-40 rounded-md border-gray-200 shadow-sm sm:text-sm"
        >
      </div>
    </div>
    <div class="relative">
      <div class="inline-flex items-center overflow-hidden rounded-md border bg-white border-gray-200 shadow-sm">
        <button
          :class="!isTotalConcentrationValid(getGasMixture()) ? 'bg-gray-700' : 'bg-teal-600'"
          class="border-e px-4 py-2 text-sm/none text-white hover:bg-teal-500 hover:text-white"
          @click="getMassFlowRate(method, getGasMixture(), P, T, 0.050, 16010500)"
        >
          {{ isTotalConcentrationValid(getGasMixture()) ? `Compute with ${method}` :
            `Total must be 100% (${totalPercent}%)` }}
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
        :ref="menu"
        class="absolute start-0 z-10 mt-2 w-56 rounded-md border border-gray-100 bg-white shadow-lg"
      >
        <div class="p-2">
          <button
            class="block rounded-lg px-4 py-2 text-sm text-gray-500 hover:bg-gray-50 hover:text-gray-700"
            @click="method = 'GERG-2008'; menuOpen = false"
          >
            GERG-2008
          </button>

          <button
            class="block rounded-lg px-4 py-2 text-sm text-gray-500 hover:bg-gray-50 hover:text-gray-700"
            @click="method = 'DETAIL'; menuOpen = false"
          >
            DETAIL
          </button>
        </div>
      </div>
    </div>
  </div>
  <div class="mt-8 grid grid-cols-1 gap-4 lg:grid-cols-2 lg:gap-8">
    <div class="rounded-lg bg-gray-200">
      <div class="m-1 overflow-x-auto ">
        <table class="rounded-md min-w-full divide-y-2 divide-gray-200 bg-white text-sm">
          <thead class="ltr:text-left rtl:text-right">
            <tr>
              <th class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                Gas mixture composition
                <button
                  id="dropdownDefaultButton"
                  data-dropdown-toggle="dropdown"
                  class="border border-gray-200 rounded text-sm text-gray-900 px-1 py-0.5 text-center inline-flex items-center"
                  type="button"
                >
                  Preset <svg
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
                        @click="setGasMixture(gas.gasMixture)"
                      >{{ gas.name }}</a>
                    </li>
                  </ul>
                </div>
              </th>
              <th class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                Concentration
              </th>
            </tr>
          </thead>
          <tbody class="divide-y divide-gray-200">
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <label for="methane" class="block text-xs font-medium text-gray-700">Methane in
                  %</label>
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
                <label for="nitrogen" class="block text-xs font-medium text-gray-700">Nitrogen in
                  %</label>
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
                <label for="carbonDioxide" class="block text-xs font-medium text-gray-700">Carbon
                  Dioxide in %</label>
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
                <label for="propane" class="block text-xs font-medium text-gray-700">Propane in
                  %</label>
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
                <label for="isobutane" class="block text-xs font-medium text-gray-700">Isobutane in
                  %</label>
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
                <label for="nButane" class="block text-xs font-medium text-gray-700">n-Butane in
                  %</label>
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
                <label for="isopentane" class="block text-xs font-medium text-gray-700">Isopentane in
                  %</label>
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
                <label for="nPentane" class="block text-xs font-medium text-gray-700">n-Pentane in
                  %</label>
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
                <label for="nHexane" class="block text-xs font-medium text-gray-700">n-Hexane in
                  %</label>
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
                <label for="nHeptane" class="block text-xs font-medium text-gray-700">n-Heptane in
                  %</label>
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
                <label for="nOctane" class="block text-xs font-medium text-gray-700">n-Octane in
                  %</label>
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
                <label for="nNonane" class="block text-xs font-medium text-gray-700">n-Nonane in
                  %</label>
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
                <label for="nDecane" class="block text-xs font-medium text-gray-700">n-Decane in
                  %</label>
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
                <label for="hydrogen" class="block text-xs font-medium text-gray-700">Hydrogen in
                  %</label>
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
                <label for="carbonMonoxide" class="block text-xs font-medium text-gray-700">Carbon
                  Monoxide
                  in %</label>
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
                <label for="hydrogenSulfide" class="block text-xs font-medium text-gray-700">Hydrogen
                  Sulfide in %</label>
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
        <DoubleRange
          ref="doubleSlider"
          class="text-xs"
          slider-color="oklch(0.6 0.118 184.704)"
          slider-off-color="oklch(0.707 0.022 261.325)"
          handle-size="1rem"
          :min="0"
          :max="100"
          :default-min="2"
          :default-max="10"
        />
      </div>
    </div>
  </div>
</template>
