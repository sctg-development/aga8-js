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
import { onMounted, ref } from 'vue';
import { generateUniqueId } from '../utilities/viteHelper';
import { initFlowbite } from 'flowbite'
import ClipboardJS from 'clipboard';
export interface Props {
  msg?: string
  content: string
  altMsg?: string
  left?: boolean
}
const props = withDefaults(defineProps<Props>(), {
  msg: 'Copied !',
  altMsg: 'Copy to clipboard',
  left: false
})

const uniqueId = generateUniqueId();
const spanRef = ref<HTMLSpanElement | null>(null);
const btnRef = ref<HTMLButtonElement | null>(null);

onMounted(() => {
  initFlowbite();
  if (btnRef.value) {
    let clipboard = new ClipboardJS(btnRef.value);
    clipboard.on('success', function () {
      console.warn(`${spanRef.value?.innerText} copied !`);
    });
  }
});
</script>

<template>
  <span v-if="!props.left" :id="uniqueId" ref="spanRef">
    <slot />
  </span>
  <button
    :id="`btn${uniqueId}`"
    ref="btnRef"
    :data-tooltip-target="`tp${uniqueId}`"
    data-tooltip-trigger="click"
    :data-clipboard-text="props.content"
  >
    <img
      :id="`img${uniqueId}`"
      class="inline-block w-3 h-3 ml-2 cursor-pointer transform contrast-[.05]"
      :class="left ? 'scale-x-[-1]' : ''"
      src="../assets/clippy.svg"
      :alt="props.altMsg"
    >
    <div
      :id="`tp${uniqueId}`"
      role="tooltip"
      class="absolute z-10 invisible inline-block px-3 py-2 text-sm font-medium text-white transition-opacity duration-300 bg-gray-900 rounded-lg shadow-sm opacity-0 tooltip dark:bg-gray-700"
    >
      {{ props.msg }}
      <div class="tooltip-arrow" data-popper-arrow />
    </div>
  </button>
  {{ left ? "&nbsp;" : "" }}<span v-if="props.left" :id="uniqueId" ref="spanRef">{{ !left ? "&nbsp;" : "" }}
    <slot />
  </span>
</template>
