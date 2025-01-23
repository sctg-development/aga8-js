import js from "@eslint/js";
import tseslint from "typescript-eslint";
import eslintPluginVue from "eslint-plugin-vue";
import globals from "globals";

export default [
  {
    ignores: ["dist/**", "node_modules/**", ".vite/**"],
  },
  js.configs.recommended,
  ...tseslint.configs.recommended,
  ...eslintPluginVue.configs['flat/recommended'],
  {
    files: ["src/**/*.{js,ts,vue}"],
    languageOptions: {
        ecmaVersion: 'latest',
        sourceType: 'module',
        parserOptions: {
          parser: tseslint.parser,
        },
      globals: {
        ...globals.browser,
        ...globals.es2022,
        ...globals.node,
      },
    },
    plugins: {
      "@typescript-eslint": tseslint.plugin,
      vue: eslintPluginVue,
    },
    rules: {
      "vue/multi-word-component-names": "off",
      "vue/no-v-model-argument": "off",
      "@typescript-eslint/no-explicit-any": "warn",
      "@typescript-eslint/explicit-function-return-type": "off",
      "vue/html-indent": ["error", 2],
      "vue/script-indent": ["error", 2],
      "no-console": ["warn", { allow: ["warn", "error"] }],
      "vue/max-attributes-per-line": [
        "error",
        {
          singleline: 3,
          multiline: 1,
        },
      ],
    },
  },
];
