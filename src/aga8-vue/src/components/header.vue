<template>
  <header id="header" class="bg-white">
    <GithubCorner url="https://github.com/sctg-development/aga8-js/tree/main/src/aga8-vue" class-bg-color="fill-teal-600" class-hover-color="fill-teal-500" />
    <div class="mx-auto max-w-screen-xl px-4 py-8 sm:px-6 sm:py-12 lg:px-8">
      <div class="flex flex-col items-start gap-4 md:flex-row md:items-center md:justify-between">
        <div>
          <h1 class="text-2xl font-bold text-gray-900 sm:text-3xl">
            AGA8 DETAIL or GERG-2008 computation
          </h1>

          <p class="mt-1.5 text-sm text-gray-500">
            Implements the DETAIL and GERG-2008 equations of state described in AGA Report No. 8, Part 1, Third Edition, April 2017.
          </p>
        </div>

        <div class="flex items-center gap-4">
          <a href="https://github.com/sctg-development"><img
            src="/logo.svg"
            alt="AGA8"
            class="w-12 h-12"
          ></a>
        </div>
      </div>
    </div>
  </header>
</template>
<script setup lang="ts">
import GithubCorner from './GithubCorner.vue';
</script>