


import { createApp } from 'vue'
import App from './App.vue'
import router from './router'//页面跳转组件


const app=createApp(App);

app.config.globalProperties.$httpurl = 'http://47.109.147.101:8080'  //打包环境下
// app.config.globalProperties.$httpurl = 'http://127.0.0.1:8080' //测试环境下
app.use(router);


app.mount('#app');

