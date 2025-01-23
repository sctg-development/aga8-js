<template>
  <Header />
  <div class="mx-auto max-w-screen-xl px-4 py-8 sm:px-6 sm:py-12 lg:px-8">
    <Detail />
  </div>
  <Footer />
</template>
<script setup lang="ts">
import Detail from './cards/Detail.vue';
import Header from './components/header.vue';
import Footer from './components/footer.vue';
</script>
