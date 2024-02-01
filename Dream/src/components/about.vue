<template>
    <div class="body">
        <div class="container">
            <div class="div-card">
                <div class="div-name">关于许愿墙</div>
                <div class="div-wechat">作者：清风</div>
                <div class="div-description">
                    许愿墙是一个让人们分享他们的愿望和梦想的平台。在这里，你可以写下你的愿望，也可以看到其他人的愿望。
                    <br>
                    我们相信，分享愿望可以帮助我们更好地理解自己和他人，也可以激发我们实现愿望的动力。
                    <br>
                    让我们一起分享愿望，实现梦想吧！
                </div>
                <div class="div-bar-type" style="--i: 1; --w: 1;"></div>
                <div class="div-bar-type" style="--i: 2; --w: 2;"></div>
                <div class="div-bar-type" style="--i: 3; --w: 3;"></div>
                <div class="div-bar-type" style="--i: 4; --w: 4;"></div>
                <div class="div-bar-type" style="--i: 5; --w: 5;"></div>
                
                <div class="f">
                    <img src="./icons/wish.png" alt="">
                    <button @click="gohome">返回主页</button>
                </div>
            </div>
        </div>
    </div>
</template>
  
<script>
import water from './water.vue'
export default {
    components: {
        water
    },
    data() {
        return {
            key: '',
        }
    },
    mounted() {
        this.key = this.$route.query.key;
    },
    methods: {
        gohome() {
            this.$router.push({ path: '/lobby', query: { key: this.key } });
        }
    }
};
</script>
  
<style scoped>
.body {
    margin: 0;
    height: 100vh;
    display: flex;
    align-items: center;
    justify-content: center;
    background-color: rgb(192, 199, 228);
    background-image: linear-gradient(to top, #63beff 0%, #63beff 1%, rgb(241, 235, 243) 100%);
}

.container {
    width: 600px;
    height: 390px;
    display: flex;
    align-items: center;
    justify-content: center;
    border-radius: 15px;
    background-color: #fff;
}

.div-card {
    width: 570px;
    height: 360px;
    position: relative;
    border-radius: 10px;
    background-color: #86a8e7;
    overflow: hidden;
}

.div-name {
    width: 200px;
    color: #fff;
    font: 900 40px '';
    position: absolute;
    top: 40px;
    left: 30px;
    transition: 1s .1s;
}

.div-wechat {
    width: 200px;
    color: lightsalmon;
    font: 900 25px '';
    position: absolute;
    top: 105px;
    left: 30px;
    transition: 1s .1s;
    opacity: 0;
}

.div-description {
    width: 200px;
    color: #fff;
    font-size: 15px;
    position: absolute;
    top: 115px;
    left: 35px;
    transition: 1s .1s;
}

.div-bar-type {
    width: calc(200px + var(--w)*70px);
    height: 50px;
    background-color: cornsilk;
    position: absolute;
    transform: rotateZ(145deg) translate(calc(-300px + var(--i)*15px), calc(var(--i)*-50px - 90px));
    border-radius: 25px;
    transition: 1s calc(var(--i)*0.1s);
}

.div-bar-type:nth-child(8) {
    background-color: lightsalmon;
}

.div-bar-type:nth-child(7) {
    background-color: lightseagreen;
}

.div-bar-type:nth-child(6) {
    background-color: lightpink;
}

.div-bar-type:nth-child(5) {
    background-color: lightcyan;
}

.div-card:hover .div-bar-type {
    transform: rotateZ(145deg) translate(calc(-600px + var(--i)*15px), calc(var(--i)*-50px - 90px));
}

.div-card:hover .div-name {
    left: 330px;
}

.div-card:hover .div-wechat {
    left: 330px;
    opacity: 1;
}

.div-card:hover .div-description {
    left: 335px;
    top: 150px;
}

.f {
    display: flex;
    justify-content: center;
    align-items: center;
    width: 230px;
    height: 230px;
    background-size: cover;
    text-align: center;
    margin: 60px 40px;
    border: 1px solid black;
    border-radius: 10px;
    opacity: 0;
    transition: .7s .3s;
}
.f button{
    position: absolute;
    bottom: 25px;
    border: none;
    background-color: #86a8e7;
    border-radius: 8px;
    transition: .1s;
    color: #fff;
    padding: 7px;
    /* box-shadow: 0 0 5px 5px #fff; */
}
.f button:hover{
    box-shadow: 0 0 5px 5px #fff;
}
.div-card:hover .f {
    opacity: 1;
}
</style>