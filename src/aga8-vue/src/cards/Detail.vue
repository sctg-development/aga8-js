<template>
  <div class="grid grid-cols-1 gap-4 lg:grid-cols-3 lg:gap-8">
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
        <label for="pressure" class="block text-xs font-medium text-gray-700">Pressure in kPa</label>
        <input
          id="pressure"
          v-model="P"
          type="number"
          placeholder="101.325"
          class="mt-1 w-40 rounded-md border-gray-200 shadow-sm sm:text-sm"
        >
      </div>
    </div>
    <div class="w-full h-24 rounded-lg bg-gray-200 flex items-center justify-center">
      <button
        v-if="moduleLoaded"
        :class="!isTotalConcentrationValid(getGasMixture()) ? 'bg-gray-700' : 'bg-teal-600'"
        :disabled="!isTotalConcentrationValid(getGasMixture())"
        class="inline-block rounded px-8 py-3 text-sm font-medium text-white transition hover:scale-110 hover:shadow-xl focus:outline-none focus:ring active:bg-indigo-500"
        @click="computeDetail"
      >
        {{ isTotalConcentrationValid(getGasMixture()) ? 'Compute' :
          `Total must be 100%` }}
      </button>
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
              </th>
              <th class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                Concentration
              </th>
            </tr>
          </thead>
          <tbody class="divide-y divide-gray-200">
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                <label
                  for="methane"
                  class="block text-xs font-medium text-gray-700"
                >Methane in %</label>
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
                <label
                  for="nitrogen"
                  class="block text-xs font-medium text-gray-700"
                >Nitrogen in %</label>
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
                <label
                  for="carbonDioxide"
                  class="block text-xs font-medium text-gray-700"
                >Carbon Dioxide in %</label>
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
                <label
                  for="ethane"
                  class="block text-xs font-medium text-gray-700"
                >Ethane in %</label>
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
                <label
                  for="propane"
                  class="block text-xs font-medium text-gray-700"
                >Propane in %</label>
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
                <label
                  for="isobutane"
                  class="block text-xs font-medium text-gray-700"
                >Isobutane in %</label>
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
                <label
                  for="nButane"
                  class="block text-xs font-medium text-gray-700"
                >n-Butane in %</label>
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
                <label
                  for="isopentane"
                  class="block text-xs font-medium text-gray-700"
                >Isopentane in %</label>
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
                <label
                  for="nPentane"
                  class="block text-xs font-medium text-gray-700"
                >n-Pentane in %</label>
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
                <label
                  for="nHexane"
                  class="block text-xs font-medium text-gray-700"
                >n-Hexane in %</label>
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
                <label
                  for="nHeptane"
                  class="block text-xs font-medium text-gray-700"
                >n-Heptane in %</label>
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
                <label
                  for="nOctane"
                  class="block text-xs font-medium text-gray-700"
                >n-Octane in %</label>
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
                <label
                  for="nNonane"
                  class="block text-xs font-medium text-gray-700"
                >n-Nonane in %</label>
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
                <label
                  for="nDecane"
                  class="block text-xs font-medium text-gray-700"
                >n-Decane in %</label>
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
                <label
                  for="hydrogen"
                  class="block text-xs font-medium text-gray-700"
                >Hydrogen in %</label>
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
                <label
                  for="oxygen"
                  class="block text-xs font-medium text-gray-700"
                >Oxygen in %</label>
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
                <label
                  for="carbonMonoxide"
                  class="block text-xs font-medium text-gray-700"
                >Carbon Monoxide
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
                <label
                  for="water"
                  class="block text-xs font-medium text-gray-700"
                >Water in %</label>
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
                <label
                  for="hydrogenSulfide"
                  class="block text-xs font-medium text-gray-700"
                >Hydrogen
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
                <label
                  for="helium"
                  class="block text-xs font-medium text-gray-700"
                >Helium in %</label>
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
                <label
                  for="argon"
                  class="block text-xs font-medium text-gray-700"
                >Argon in %</label>
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
        <table class="rounded-md min-w-full divide-y-2 divide-gray-200 bg-white text-sm">
          <thead class="ltr:text-left rtl:text-right">
            <tr>
              <th class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                Property
              </th>
              <th class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                Value
              </th>
            </tr>
          </thead>
          <tbody class="divide-y divide-gray-200">
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                Molar mass in g/mol
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                {{ mm }}
              </td>
            </tr>
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                Density in mol/l
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                {{ density }}
              </td>
            </tr>
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                Pressure in kPa
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                {{ properties.P }}
              </td>
            </tr>
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                Compressibility factor
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                {{ properties.Z }}
              </td>
            </tr>
            <tr>
              <td class="whitespace-wrap px-4 py-2 font-medium text-gray-900">
                First derivative of
                pressure with respect to density at constant temperature [kPa/(mol/l)]
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                {{ properties.dPdD }}
              </td>
            </tr>
            <tr>
              <td class="whitespace-wrap px-4 py-2 font-medium text-gray-900">
                Second derivative of
                pressure with respect to density at constant temperature [kPa/(mol/l)^2]
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                {{ properties.d2PdD2 }}
              </td>
            </tr>
            <tr>
              <td class="whitespace-wrap px-4 py-2 font-medium text-gray-900">
                Second derivative of
                pressure with respect to temperature and density [kPa/(mol/l)/K]
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                {{ properties.d2PdTD }}
              </td>
            </tr>
            <tr>
              <td class="whitespace-wrap px-4 py-2 font-medium text-gray-900">
                First derivative of
                pressure with respect to temperature at constant density (kPa/K)
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                {{ properties.dPdT }}
              </td>
            </tr>
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                Internal energy in J/mol
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                {{ properties.U }}
              </td>
            </tr>
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                Enthalpy in J/mol
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                {{ properties.H }}
              </td>
            </tr>
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                Entropy in J/(mol-K)
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                {{ properties.S }}
              </td>
            </tr>
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                Isochoric heat capacity in
                J/(mol-K)
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                {{ properties.Cv }}
              </td>
            </tr>
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                Isobaric heat capacity in
                J/(mol-K)
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                {{ properties.Cp }}
              </td>
            </tr>
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                Speed of sound in m/s
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                {{ properties.W }}
              </td>
            </tr>
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                Gibbs energy in J/mol
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                {{ properties.G }}
              </td>
            </tr>
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                Joule-Thomson coefficient
                in K/kPa
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                {{ properties.JT }}
              </td>
            </tr>
            <tr>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                Isentropic Exponent
              </td>
              <td class="whitespace-nowrap px-4 py-2 font-medium text-gray-900">
                {{ properties.Kappa }}
              </td>
            </tr>
          </tbody>
        </table>
      </div>
    </div>
  </div>
</template>
<script setup lang="ts">
import AGA8wasm, { type MainModule, type gazMixtureInMolePercent, type PropertiesDetailResult } from '@sctg/aga8-js'
import { onMounted, ref } from 'vue';
// type gazMixtureInMolePercent = [number, methane: number, nitrogen: number, carbon_dioxide: number, ethane: number, propane: number, isobutane: number, n_butane: number, isopentane: number, n_pentane: number, n_hexane: number, n_heptane: number, n_octane: number, n_nonane: number, n_decane: number, hydrogen: number, oxygen: number, carbon_monoxyde: number, water: number, hydrogen_sulfide: number, helium: number, argon: number]
const moduleLoaded = ref(false);

const methaneConcentration = ref(0);
const nitrogenConcentration = ref(78.08);
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
const oxygenConcentration = ref(20.95);
const carbonMonoxideConcentration = ref(0);
const waterConcentration = ref(0.04);
const hydrogenSulfideConcentration = ref(0);
const heliumConcentration = ref(0);
const argonConcentration = ref(0.93);

const T = ref(273.15 + 20); // °K
const P = ref(101.325);     // kPa

const mm = ref(0); // Molar mass in g/mol
const density = ref(0);  // Density in mol/l

/**
[out]	P	Pressure in kPa
[out]	Z	Compressibility factor
[out]	dPdD	First derivative of pressure with respect to density at constant temperature [kPa/(mol/l)]
[out]	d2PdD2	Second derivative of pressure with respect to density at constant temperature [kPa/(mol/l)^2]
[out]	d2PdTD	Second derivative of pressure with respect to temperature and density [kPa/(mol/l)/K]
[out]	dPdT	First derivative of pressure with respect to temperature at constant density (kPa/K)
[out]	U	Internal energy in J/mol
[out]	H	Enthalpy in J/mol
[out]	S	Entropy in J/(mol-K)
[out]	Cv	Isochoric heat capacity in J/(mol-K)
[out]	Cp	Isobaric heat capacity in J/(mol-K)
[out]	W	Speed of sound in m/s
[out]	G	Gibbs energy in J/mol
[out]	JT	Joule-Thomson coefficient in K/kPa
[out]	Kappa	Isentropic Exponent
 */
const properties = ref<PropertiesDetailResult>({ P: 0, Z: 0, dPdD: 0, d2PdD2: 0, d2PdTD: 0, dPdT: 0, U: 0, H: 0, S: 0, Cv: 0, Cp: 0, W: 0, G: 0, JT: 0, Kappa: 0 });
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
  AGA8wasm().then((AGA8module) => {
    AGA8 = AGA8module;
    moduleLoaded.value = true;
    console.warn("AGA8 module loaded");
  })
})

/**
 * Returns the gas mixture in mole percent
 * 
 * @returns {gazMixtureInMolePercent} - Array of gas mixture components in mole percent
 */
function getGasMixture(): gazMixtureInMolePercent {
  return [
    0, //Placeholder for the total mole percent
    methaneConcentration.value / 100,
    nitrogenConcentration.value / 100,
    carbonDioxideConcentration.value / 100,
    ethaneConcentration.value / 100,
    propaneConcentration.value / 100,
    isobutaneConcentration.value / 100,
    nButaneConcentration.value / 100,
    isopentaneConcentration.value / 100,
    nPentaneConcentration.value / 100,
    nHexaneConcentration.value / 100,
    nHeptaneConcentration.value / 100,
    nOctaneConcentration.value / 100,
    nNonaneConcentration.value / 100,
    nDecaneConcentration.value / 100,
    hydrogenConcentration.value / 100,
    oxygenConcentration.value / 100,
    carbonMonoxideConcentration.value / 100,
    waterConcentration.value / 100,
    hydrogenSulfideConcentration.value / 100,
    heliumConcentration.value / 100,
    argonConcentration.value / 100];
}

/**
 * Calculates gas properties using AGA8 Detail method
 * 
 * This function performs the following operations when AGA8 module is available:
 * 1. Initializes the AGA8 Detail method
 * 2. Calculates the molar mass of the gas mixture in g/mol
 * 3. Computes the density in mol/l based on temperature and pressure
 * 4. Calculates additional gas properties using the computed density
 * 
 * @requires AGA8 - The AGA8 WASM module must be loaded
 * @requires gasMixture - Reactive reference to the gas composition
 * @requires T - Reactive reference to temperature
 * @requires P - Reactive reference to pressure
 * 
 * @returns {void} - Results are logged to console
 */
function computeDetail() {
  if (AGA8) {
    AGA8.SetupDetail();
    const gasMixture = getGasMixture();
    // Compute the molar mass
    mm.value = AGA8.MolarMassDetail(gasMixture); // g/mol
    console.warn(`Molar mass: ${mm.value} g/mol`);
    // Compute the density in mol/l
    const { D } = AGA8.DensityDetail(T.value, P.value, gasMixture); // mol/l
    density.value = D;
    console.warn(`Density: ${D} mol/l`);
    properties.value = AGA8.PropertiesDetail(T.value, D, gasMixture);
  } else {
    console.warn("AGA8 module is not loaded");
  }
}


/**
 * Calculates the sum of mole percentages for all components in a gas mixture, excluding the first element.
 * 
 * @param {gazMixtureInMolePercent} x - Array of gas mixture components in mole percent
 * @returns {number} - Total concentration as sum of all components except first one
 */
function computeTotalConcentration(x: gazMixtureInMolePercent): number {
  return x.slice(1).reduce((a, b) => a + b, 0);
}

/**
 * Checks if the total concentration of a gas mixture is 100%
 * 
 * @param {gazMixtureInMolePercent} x - Array of gas mixture components in mole percent
 * @returns {boolean} - True if total concentration is 100%, false otherwise
 */
function isTotalConcentrationValid(x: gazMixtureInMolePercent): boolean {
  const concentration = computeTotalConcentration(x);
  if (concentration !== 1) {
    console.error(`Total concentration is not 100%: ${concentration * 100}%`);
  }
  return ((1 - concentration) <= 1e-12);
}
</script>