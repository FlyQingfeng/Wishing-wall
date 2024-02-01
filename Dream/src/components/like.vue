<template>
    <label class="love" for="checkbox">
            <input type="checkbox" id="checkbox" hidden @click="click">
            <svg t="1639041971928" class="icon" viewBox="0 0 1024 1024" version="1.1" xmlns="http://www.w3.org/2000/svg" p-id="3128"><path d="M512 896a42.666667 42.666667 0 0 1-30.293333-12.373333l-331.52-331.946667a224.426667 224.426667 0 0 1 0-315.733333 223.573333 223.573333 0 0 1 315.733333 0L512 282.026667l46.08-46.08a223.573333 223.573333 0 0 1 315.733333 0 224.426667 224.426667 0 0 1 0 315.733333l-331.52 331.946667A42.666667 42.666667 0 0 1 512 896z" p-id="3129" id="heart"></path></svg>
            <span></span>
    </label>
</template>
  
<script>
export default {
    props: {
        click: {
            type: Function,
            default: ()=>{},
        }
    },
    methods:{
        gohome(){
            this.$router.push({path:'/lobby',query: { key: this.key }});
        }
    }
};
</script>
  
<style scoped>
*{
    /* 初始化 */
    margin: 0;
    padding: 0;
}
.body{
    /* 100%窗口高度 */
    height: 100vh;
    /* 弹性布局 居中 */
    display: flex;
    justify-content: center;
    align-items: center;
    /* 自定义属性，可通过var函数对其调用 */
    --c: #ff6b81;
}
.love{
    --c: #ff6b81;
    position: relative;
    height: 30px;
    /* background-color: aqua; */
}
svg{
    width: 30px;
    /* 相对定位 */
    position: relative;
    /* z-index: 10; */
}
#heart{
    /* 填充颜色 */
    fill: #eee;
    /* stroke属性可应用于任何种类的线条，文字和元素，就像一个圆的轮廓 */
    stroke: var(--c);
    /* 线条宽度 */
    stroke-width: 40px;
    /* 设置线条为虚线，虚线的长度 */
    stroke-dasharray: 2600;
    /* 线条的位移 */
    stroke-dashoffset: 2600;
    /* 端点为圆头 */
    stroke-linecap: round;
}
span{
    left: 12.5px;
    top: 12.5px;
    width: 25px;
    height: 25px;
    background-color: transparent;
    border-radius: 50%;
    /* 绝对定位 居中 */
    position: absolute;
    transform:scale(0);
    /* 设置各个方向的阴影 */
    /* 通过var函数调用自定义属性--c,设置阴影颜色 */
    box-shadow: 0 -160px 0 var(--c),
    0 160px 0 var(--c),
    -160px 0 0 var(--c),
    160px 0 0 var(--c),
    -120px -120px 0 var(--c),
    120px -120px 0 var(--c),
    120px 120px 0 var(--c),
    -120px 120px 0 var(--c);
}
/* 勾选复选框执行各个动画 */
#checkbox:checked + svg #heart{
    /* 执行动画: 动画名 时长 线性的 停留在最后一帧 */
    animation: drawHeart 1s linear forwards;
}
#checkbox:checked ~ span{
    /* 执行动画: 动画名 时长 加速后减速 停留在最后一帧 */
    animation: blink 0.5s ease-in-out forwards;
    /* 动画延迟时间 */
    animation-delay: 0.85s;
}
#checkbox:checked + svg{
    /* 执行动画: 动画名 时长 线性的 停留在最后一帧 */
    animation: beat 1s linear forwards;
}
label{
    /* 鼠标移入,光标变小手 */
    cursor: pointer;
}

/* 定义动画 */
/* 画心的动画 */
@keyframes drawHeart {
    0%{
        stroke-dashoffset: 2600;
    }
    80%{
        fill: #eee;
        stroke-dashoffset: 0;
    }
    100%{
        fill: var(--c);
        stroke-dashoffset: 0;
    }
}
/* 小圆点闪出的动画 */
@keyframes blink {
    0%{
        transform:  scale(0.5);
        opacity: 0.8;
    }
    50%{
        transform:  scale(1);
        opacity: 1;
    }
    100%{
        transform: scale(1.1);
        opacity: 0;
    }
}
/* 心跳动的动画 */
@keyframes beat {
    0%{
        transform: scale(1);
    }
    70%{
        transform: scale(1);
    }
    80%{
        transform: scale(1.2);
    }
    100%{
        transform: scale(1);
    }
}
</style>