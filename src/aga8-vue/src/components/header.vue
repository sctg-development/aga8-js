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
import GithubCorner from './GithubCorner.vue';
</script>