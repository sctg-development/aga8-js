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
import { ref, onMounted } from "vue";
export interface Props {
  /**
   * Minimum value of the slider range (optional)
   */
  min?: number
  /**
   * Maximum value of the slider range (optional)
   */
  max?: number
  /**
   * Step size of the slider range (optional)
   */
  step?: number
  /**
   * Default minimum value of the slider range (optional)
   */
  defaultMin?: number
  /**
   * Default maximum value of the slider range (optional)
   */
  defaultMax?: number
  /**
   * Color of the slider (optional)
   */
  sliderColor?: string
  /**
   * Color of the slider when off (optional)
   */
  sliderOffColor?: string
  /**
   * Size of the slider handle (optional)
   */
  handleSize?: string
}

// Define and set default values for props using the `withDefaults` and `defineProps` Vue APIs
const props = withDefaults(defineProps<Props>(), {
  min: 0,
  max: 10,
  step: 0.1,
  defaultMin: 2,
  defaultMax: 9,
  sliderColor: '#25daa5',
  sliderOffColor: '#C6C6C6',
  handleSize: '16px'

})

const from = ref(props.defaultMin);
const to = ref(props.defaultMax);

/** Slider for low value */
const fromSlider = ref<HTMLInputElement>();
/** Slider for upper value */
const toSlider = ref<HTMLInputElement>();
/** Input for low value */
const fromInput = ref<HTMLInputElement>();
/** Input for upper value */
const toInput = ref<HTMLInputElement>();

defineExpose({ from, to });

/**
 * Control the from input value and update the slider accordingly
 * @param fromSlider The from slider element
 * @param fromInput The from input element
 * @param toInput The to input element
 * @param controlSlider The control slider element
 */
function controlFromInput(fromSlider: HTMLInputElement, fromInput: HTMLInputElement, toInput: HTMLInputElement, controlSlider: HTMLInputElement) {
  const [_from, _to] = getParsed(fromInput, toInput);
  fillSlider(fromInput, toInput, props.sliderOffColor, props.sliderColor, controlSlider);
  if (_from > _to) {
    fromSlider.value = _to.toString();
    fromInput.value = _to.toString();
  } else {
    fromSlider.value = _from.toString();
  }
  from.value = parseFloat(fromInput.value);
}

/**
 * Control the to input value and update the slider accordingly
 * @param toSlider The to slider element
 * @param fromInput The from input element
 * @param toInput The to input element
 * @param controlSlider The control slider element
 */
function controlToInput(toSlider: HTMLInputElement, fromInput: HTMLInputElement, toInput: HTMLInputElement, controlSlider: HTMLInputElement) {
  const [_from, _to] = getParsed(fromInput, toInput);
  fillSlider(fromInput, toInput, props.sliderOffColor, props.sliderColor, controlSlider);
  setToggleAccessible(toInput);
  if (_from <= _to) {
    toSlider.value = _to.toString();
    toInput.value = _to.toString();
  } else {
    toInput.value = _from.toString();
  }
  to.value = parseFloat(toInput.value);
}

/**
 * Control the from slider value and update the input accordingly
 * @param fromSlider The from slider element
 * @param toSlider The to slider element
 * @param fromInput The from input element
 */
function controlFromSlider(fromSlider: HTMLInputElement, toSlider: HTMLInputElement, fromInput: HTMLInputElement) {
  const [_from, _to] = getParsed(fromSlider, toSlider);
  fillSlider(fromSlider, toSlider, props.sliderOffColor, props.sliderColor, toSlider);
  if (_from > _to) {
    fromSlider.value = _to.toString();
    fromInput.value = _to.toString();
  } else {
    fromInput.value = _from.toString();
  }
  from.value = parseFloat(fromInput.value);
}

/**
 * Control the to slider value and update the input accordingly
 * @param fromSlider The from slider element
 * @param toSlider The to slider element
 * @param toInput The to input element
 */
function controlToSlider(fromSlider: HTMLInputElement, toSlider: HTMLInputElement, toInput: HTMLInputElement) {
  const [_from, _to] = getParsed(fromSlider, toSlider);
  fillSlider(fromSlider, toSlider, props.sliderOffColor, props.sliderColor, toSlider);
  setToggleAccessible(toSlider);
  if (_from <= _to) {
    toSlider.value = _to.toString();
    toInput.value = _to.toString();
  } else {
    toInput.value = _from.toString();
    toSlider.value = _from.toString();
  }
  to.value = parseFloat(toInput.value);
}

/**
 * Get the parsed values of the from and to elements
 * @param currentFrom The from element
 * @param currentTo The to element
 * @returns An array containing the parsed from and to values
 */
function getParsed(currentFrom: HTMLInputElement, currentTo: HTMLInputElement) {
  const from = parseFloat(currentFrom.value);
  const to = parseFloat(currentTo.value);
  return [from, to];
}

/**
 * Fill the slider with the correct colors
 * @param from The from element
 * @param to The to element
 * @param sliderColor The color of the slider
 * @param rangeColor The color of the range
 * @param controlSlider The control slider element
 */
function fillSlider(from: HTMLInputElement, to: HTMLInputElement, sliderColor: string, rangeColor: string, controlSlider: HTMLInputElement) {
  const rangeDistance = parseFloat(to.max) - parseFloat(to.min);
  const fromPosition = parseFloat(from.value) - parseFloat(to.min);
  const toPosition = parseFloat(to.value) - parseFloat(to.min);
  controlSlider.style.background = `linear-gradient(
      to right,
      ${sliderColor} 0%,
      ${sliderColor} ${(fromPosition) / (rangeDistance) * 100}%,
      ${rangeColor} ${((fromPosition) / (rangeDistance)) * 100}%,
      ${rangeColor} ${(toPosition) / (rangeDistance) * 100}%, 
      ${sliderColor} ${(toPosition) / (rangeDistance) * 100}%, 
      ${sliderColor} 100%)`;
}

function setToggleAccessible(currentTarget: HTMLInputElement) {
  const toSlider = document.querySelector('#toSlider') as HTMLInputElement;
  if (parseInt(currentTarget.value) <= 0) {
    toSlider.style.zIndex = "2";
  } else {
    toSlider.style.zIndex = "0";
  }
}

onMounted(() => {
  if (fromSlider.value && toSlider.value) {
    fillSlider(fromSlider.value, toSlider.value, props.sliderOffColor, props.sliderColor, toSlider.value);
    setToggleAccessible(toSlider.value);
  }
})

</script>

<template>
  <div class="flex flex-col w-4/5 mx-auto my-24">
    <div class="relative min-h-3">
      <input
        id="fromSlider"
        ref="fromSlider"
        type="range"
        :value="props.defaultMin"
        :step="props.step"
        :min="props.min"
        :max="props.max"
        @input="() => fromSlider && toSlider && fromInput && controlFromSlider(fromSlider, toSlider, fromInput)"
      >
      <input
        id="toSlider"
        ref="toSlider"
        type="range"
        :value="props.defaultMax"
        :step="props.step"
        :min="props.min"
        :max="props.max"
        @input="() => fromSlider && toSlider && toInput && controlToSlider(fromSlider, toSlider, toInput)"
      >
    </div>
    <div class="relative flex justify-between text-gray-900">
      <div>
        <div>
          Min
        </div>
        <input
          id="fromInput"
          ref="fromInput"
          type="number"
          :value="props.defaultMin"
          :min="props.min"
          :max="props.max"
          @input="() => fromSlider && toSlider && fromInput && toInput && controlFromInput(fromSlider, fromInput, toInput, toSlider)"
        >
      </div>
      <div>
        <div>
          Max
        </div>
        <input
          id="toInput"
          ref="toInput"
          type="number"
          :value="props.defaultMax"
          :min="props.min"
          :max="props.max"
          @input="() => fromSlider && toSlider && fromInput && toInput && controlToInput(toSlider, fromInput, toInput, toSlider)"
        >
      </div>
    </div>
  </div>
</template>

<style scoped>
input[type=range]::-webkit-slider-thumb {
    -webkit-appearance: none;
    pointer-events: all;
    width: v-bind('props.handleSize');
    height: v-bind('props.handleSize');
    background-color: #fff;
    border-radius: 50%;
    box-shadow: 0 0 0 1px v-bind('props.sliderOffColor');
    cursor: pointer;
}

input[type=range]::-moz-range-thumb {
    -webkit-appearance: none;
    pointer-events: all;
    width: v-bind('props.handleSize');
    height: v-bind('props.handleSize');
    background-color: #fff;
    border-radius: 50%;
    box-shadow: 0 0 0 1px v-bind('props.sliderOffColor');
    cursor: pointer;
}

input[type=range]::-webkit-slider-thumb:hover {
    background: #f7f7f7;
}

input[type=range]::-webkit-slider-thumb:active {
    box-shadow: inset 0 0 3px #387bbe, 0 0 9px #387bbe;
    -webkit-box-shadow: inset 0 0 3px #387bbe, 0 0 9px #387bbe;
}

input[type="number"] {
    color: #8a8383;
    border: none;
}

input[type=number]::-webkit-inner-spin-button,
input[type=number]::-webkit-outer-spin-button {
    opacity: 1;
}

input[type="range"] {
    -webkit-appearance: none;
    appearance: none;
    height: 2px;
    width: 100%;
    position: absolute;
    background-color: v-bind('props.sliderOffColor');
    pointer-events: none;
}

#fromSlider {
    height: 0;
    z-index: 1;
}
</style>