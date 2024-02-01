<template>
    <!-- <water /> -->
    <transition name="slide">   
    <div :class="{sidebar:true,show:showsidebar,close:!showsidebar}">
        <h1>许愿墙</h1>
        <div class="content">
            <h1>个人信息</h1>
            <h3 id="username">{{userdata.name}} : {{ userdata.idcode }}</h3>
            <h3>我的愿望数量：{{ userdata.wishnum }}</h3>
            <h3>被赞数量：{{ userdata.likednum }}</h3>
            <h3>被评论量：{{ userdata.commentnum }}</h3>
        </div>
        <div class="content">
            <button @click="showsidebar=false">愿望大厅</button>
        </div>
        <div class="content">
            <button @click="openself">我的愿望</button>
        </div>
        <div class="content">
            <button @click="set">账号设置</button>
        </div>
        <div class="content">
            <button @click="toabout">关于墙</button>
        </div>
    </div>
    </transition>
    <div class="topbar">
            <button id="menu" @click="showsidebar=!showsidebar">
                <svg fill="#fff" height="512pt" viewBox="-59 0 512 512" width="512pt"><path d="m393.644531 185.714844c0-63.988282-52.058593-116.046875-116.046875-116.046875-63.992187 0-116.046875 52.058593-116.046875 116.046875 0 68.515625 65.117188 129.785156 101.046875 158.375v167.910156h30v-167.910156c35.921875-28.585938 101.046875-89.855469 101.046875-158.375zm-122.945312-43.515625v-30c45.414062 0 82.359375 36.949219 82.359375 82.359375h-30c0-28.871094-23.488282-52.359375-52.359375-52.359375zm0 0"/><path d="m131.546875 185.714844c0-65.414063 43.230469-120.921875 102.617187-139.453125-24.460937-28.316407-60.609374-46.261719-100.875-46.261719-73.496093 0-133.289062 59.792969-133.289062 133.289062 0 87.964844 91.84375 159.011719 118.289062 177.644532v201.066406h30v-201.066406c5.382813-3.789063 13.480469-9.765625 22.949219-17.597656-6.597656-9.101563-12.402343-18.230469-17.378906-27.332032-14.804687-27.097656-22.3125-54.109375-22.3125-80.289062zm0 0"/></svg>
            </button>
    </div>
</template>
  
<script>
import water from './water.vue'
//   import water from './lobby.vue'

export default {
    components: {
        water
    },
    data() {
        return {
            showsidebar:true
        }
    },
    props:{
        userdata:Object
    },
    methods:{
        switchstate(){
            showsidebar=!showsidebar;
        },
        openself(){
            this.$router.push({path:'/self',query: { key: this.userdata.key }});
        },
        toabout(){
            this.$router.push({path:'/about',query: { key: this.userdata.key }});
        },
        set(){
            window.alert(`未开发敬请期待`);
        }

    }
}
</script>
  
<style scoped="">
::-webkit-scrollbar {
    display: none;
}

.sidebar {
    width: 400px;
    height: 100vh;
    /* background-color: hsla(210, 20%, 98%, 0.699); */
    position: fixed;
    opacity: 0.9;
    left: 0;
    top: 0;
    overflow: auto;
    /* box-shadow: 2px 0 5px rgba(0, 0, 0, 0.5); */

    background-color: #f0f0f0; /* 更改背景色为浅灰色 */
    box-shadow: 2px 0 5px rgba(0, 0, 0, 0.2); /* 减小阴影效果 */
}
.show{
    animation: show 0.42s linear both;
}
.close{
    animation: close 0.42s linear both;
}
@keyframes show {
  0%{
    transform: translateX(-100%);;
  }
  100%{
    transform: translateX(0);;
  }
}
@keyframes close {
  0%{
    transform: translateX(0%);;
  }
  100%{
    transform: translateX(-100%);;
  }
}

.sidebar h1 {
    padding: 20px;
    color: #333;
    text-align: center;
    font-size: 64px;
    /* text-shadow: 10px 10px 8px #0d3e617e; */
    /* margin-left: 0px; */
    color: #159dff;
    font-family: 'Times New Roman', Times, serif;
    text-shadow: 2px 2px 4px rgba(0, 0, 0, 0.1); /* 减小文字阴影效果 */
}


.sidebar .content {
    padding: 15px 20px;
    /* border-bottom: 2px solid #ffffff; */
    text-align: center;
    transition: background-color 0.3s ease;
    border-bottom: 1px solid #e0e0e0; /* 更改边框颜色为浅灰色 */
    
}

.sidebar .content h1{
    border-top: #159dff 2px solid;
    padding: 15px;
    color: #333;
    text-align: center;
    font-size: 64px;
    color: #0bbe56;
    text-shadow: 2px 2px 4px rgba(0, 0, 0, 0.1); /* 减小文字阴影效果 */
    /* margin-left: 0px; */
    color: #0bbe56;
    font-family: 'Times New Roman', Times, serif;
}

.topbar {
    text-align: right;
    width: 100%;
    position: absolute;
    top: 0;
    left: 20;
    /* background-color: #333; */
}
#username{
    font-size: 20px;
}

.topbar svg{
    margin: 10px 0;
    width: 40px;
    height: 40px;
}
.sidebar .content button {
    cursor: pointer;
    border-style: none;
    width: 160px;
    height: 50px;
    font-size: 24px;
    border-radius: 10px;

    background-color: #a0ffa0; /* 更改按钮背景色为浅绿色 */
    box-shadow: 0 0 5px rgba(0, 0, 0, 0.1); /* 减小按钮阴影效果 */
}
.sidebar .content button:hover {
    border-radius: 20px;
    box-shadow: 0 0 10px rgba(0, 0, 0, 0.2); /* 增大按钮悬停时的阴影效果 */
}
#menu{
    border-style: none;
    background-color: var(--logincard-color);
}
</style>
  