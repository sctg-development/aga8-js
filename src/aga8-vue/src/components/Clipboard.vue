<template>
  <span v-if="!left" :id="uniqueId" ref="spanRef">
    <slot />
  </span>
  <button
    :id="`btn${uniqueId}`"
    ref="btnRef"
    :data-tooltip-target="`tp${uniqueId}`"
    data-tooltip-trigger="click"
    :data-clipboard-target="`#${uniqueId}`"
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
  {{ left ? "&nbsp;" : "" }}<span v-if="left" :id="uniqueId" ref="spanRef">{{ !left ? "&nbsp;" : "" }}
    <slot />
  </span>
</template>
<script setup lang="ts">
import { onMounted, ref } from 'vue';
import { generateUniqueId } from '../utilities/viteHelper';
import { initFlowbite } from 'flowbite'
import ClipboardJS from 'clipboard';
export interface Props {
  msg?: string
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