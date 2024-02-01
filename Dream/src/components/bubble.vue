<template>
    <div class="bubble" :style="{ left: bubble.left + 'px', animationDuration: bubble.duration + 's' }" 
        @mouseover="bubble.showText = true" @mouseleave="bubble.showText = false">
        <span class="bubble-text">{{name}}</span>
        <div class="wish-card" v-if="bubble.showText">
            <h1>'愿望'</h1>
            <h2>{{ bubble.name+':' }}</h2>
            <h3>{{ bubble.Wishing }}</h3>
            <div class="btndiv">
                <button class="cardbtn" @click="likeWish(bubble)">点赞</button>
                <button class="cardbtn" @click="showmore(bubble.idcode)">更多</button>
            </div>
        </div>
    </div>
</template>
  
<script>
export default {
    props:{
        name:{
            type:String,
            default:''
        },
        bubble:Object,
        likeWish:Function,
        showmore:Function
    },
    data(){
        return{
           
        }
    }
};
</script>
  
<style scoped>

/* 泡泡 */
.bubble{
    bottom: 0;
    text-align: center;
    position: fixed;
    width: 120px;
    height: 120px;
    /* 径向渐变 */
    background: radial-gradient(circle at 75% 30%,#fff 5px,#ff21c0 8%,#5b5b5b 60%,#ff21c0 100%);
    border-radius: 50%;
    /* 阴影 */
    box-shadow: inset 0 0 20px #accfe6,
        inset 10px 0 46px #086dce,
        inset 80px 0 60px #c7dcef,
        inset -20px -60px 100px #c7dcef,
        inset 0 50px 140px #086dce,
        0 0 90px #fff;
    /* 执行动画：动画名 时长 加速后减速 无限次播放 */
    animation: rise -10s ease-in-out infinite,bobble -4s ease-in-out infinite;
}
.bubble:hover {
    animation-play-state: paused;
}
.bubble-text {
    position: absolute;
    top: 50%;
    left: 50%;
    transform: translate(-50%, -50%);
    font-family: "Gochi Hand", cursive;
    text-align: center;
    white-space: nowrap; /* 不换行 */
    overflow: hidden; /* 超出部分隐藏 */
    text-overflow: ellipsis; /* 使用省略号表示超出部分 */
}

.wish-card {
    transform: translateX(-50%);
    position: relative;
    width: 300px;
    min-height: 180px;
    background: #0597ff;
    margin: 30px 10px;
    padding: 20px 15px;
    display: flex;
    flex-direction: column;
    box-shadow: 0 5px 20px rgba(0, 0, 0, 0.5);
    transition: 0.5s ease-in-out;
    font-family: "Gochi Hand", cursive;
}
.wish-card h1{
    width: 100%;
    text-align: left;
    margin-top: -10px;
    padding: 5px 0px;
}
.wish-card h2{
    width: 100%;
    text-align: left;
    margin-top: -20px;
}
.wish-card h3{
    width: 100%;
    text-align:center;
    margin-top: 5px;
}
.wish-card .btndiv{
    width: 100%;
    display: flex;
    position: absolute;
    left: 0;
    bottom: 10px;
    /* background-color: black; */
}
.wish-card .btndiv .cardbtn{
    width: 50%;
    padding: 10px;
    margin: 0px 10px;
    border: none;
    border-radius: 5px;
    background-color: #44b1ff;
    color: white;
    cursor: pointer;
}
.wish-card .btndiv .cardbtn:hover{
    background-color: #0385e2;
    color: white;
}

</style>