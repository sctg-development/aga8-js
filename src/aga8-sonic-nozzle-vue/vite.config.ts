import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import { githubPagesSpa } from "@sctg/vite-plugin-github-pages-spa";

// https://vite.dev/config/
export default defineConfig({
  plugins: [vue(), githubPagesSpa()],
  optimizeDeps: {
    exclude: ['@sctg/aga8-js']
  },
})
