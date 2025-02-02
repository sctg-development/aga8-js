<script lang="ts" setup>
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

/**
 * GitHub corner component props interface
 * @interface Props
 * @property {string} url - The URL to link to (required)
 * @property {string} [title="View source on GitHub"] - The title attribute for the link
 * @property {string} [target="_blank"] - The target attribute for the link
 * @property {string} [classFgColor="fill-white"] - The foreground color class
 * @property {string} [classBgColor="fill-black"] - The background color class
 * @property {string} [classHoverColor="fill-black"] - The hover color class
 */
export interface Props {
  url: string,
  title?: string, 
  target?: string,
  classFgColor?: string,
  classBgColor?: string,
  classHoverColor?: string,
}

const props = withDefaults(defineProps<Props>(), {
  title: 'View source on GitHub',
  target: '_blank',
  classFgColor: 'fill-white',
  classBgColor: 'fill-black',
  classHoverColor: 'fill-black',
})
</script>

<template>
  <a
    :href="props.url"
    class="github-corner"
    :title="props.title"
    :target="props.target"
    rel="noopener noreferrer"
  >
    <svg
      viewBox="0 0 250 250"
      width="50px"
      height="50px"
      style="position: absolute; top: 0; border: 0; right: 0; z-index: 99;"
      aria-hidden="true"
    >
      <path :class="[props.classBgColor,`hover:${classHoverColor}`]" d="M0,0 L115,115 L130,115 L142,142 L250,250 L250,0 Z" />
      <path
        d="M128.3,109.0 C113.8,99.7 119.0,89.6 119.0,89.6 C122.0,82.7 120.5,78.6 120.5,78.6 C119.2,72.0 123.4,76.3 123.4,76.3 C127.3,80.9 125.5,87.3 125.5,87.3 C122.9,97.6 130.6,101.9 134.4,103.2"
        style="transform-origin: 130px 106px;"
        class="fill-white octo-arm"
      />
      <path
        d="M115.0,115.0 C114.9,115.1 118.7,116.5 119.8,115.4 L133.7,101.6 C136.9,99.2 139.9,98.4 142.2,98.6 C133.8,88.0 127.5,74.4 143.8,58.0 C148.5,53.4 154.0,51.2 159.7,51.0 C160.3,49.4 163.2,43.6 171.4,40.1 C171.4,40.1 176.1,42.5 178.8,56.2 C183.1,58.6 187.2,61.8 190.9,65.4 C194.5,69.0 197.7,73.2 200.1,77.6 C213.8,80.2 216.3,84.9 216.3,84.9 C212.7,93.1 206.9,96.0 205.4,96.6 C205.1,102.4 203.0,107.8 198.3,112.5 C181.9,128.9 168.3,122.5 157.7,114.1 C157.9,116.9 156.7,120.9 152.7,124.9 L141.0,136.5 C139.8,137.7 141.6,141.9 141.8,141.8 Z"
        :class="props.classFgColor"
        class="octo-body"
      />
    </svg></a>
</template>

<style>
.github-corner:hover .octo-arm {
    animation: octocat-wave 560ms ease-in-out
}

@keyframes octocat-wave {

    0%,
    100% {
        transform: rotate(0)
    }

    20%,
    60% {
        transform: rotate(-25deg)
    }

    40%,
    80% {
        transform: rotate(10deg)
    }
}

@media (max-width:500px) {
    .github-corner:hover .octo-arm {
        animation: none
    }

    .github-corner .octo-arm {
        animation: octocat-wave 560ms ease-in-out
    }
}
</style>