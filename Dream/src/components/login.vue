<template>
    <div class="bigbox" ref="bigbox">
        <div class="container" :class="[loginshow ? '' : 'right-panel-active']">
            <!-- 注册 -->
            <div class="container_form container--signup">
                <form action="#" class="form" id="form1">
                    <h2 class="form_title">注册</h2>
                    <myinput type="text" v-model="RegistName" placeholder="昵称"/>
                    <myinput type="email" v-model="RegistIDcode" placeholder="邮箱"/>
                    <myinput type="password" v-model="RegistPassword" placeholder="密码"/>
                    <!-- <input type="text" v-model="RegistName" placeholder="昵称" class="input" />
                    <input type="email" v-model="RegistIDcode" placeholder="邮箱" class="input" />
                    <input type="password" v-model="RegistPassword" placeholder="密码" class="input" /> -->
                    <button class="btn" @click="regist">注册</button>
                </form>
            </div>

            <!-- 登录 -->
            <div class="container_form container--signin" >
                <form action="#" class="form" id="form2">
                    <h2 class="form_title">登录</h2>
                    <myinput type="email" v-model="LoginIDcode" placeholder="邮箱"/>
                    <myinput type="password" v-model="LoginPassword" placeholder="密码"/>
                    <!-- <input type="email" v-model="LoginIDcode" placeholder="邮箱" class="input" /> -->
                    <!-- <input type="password" v-model="LoginPassword" placeholder="密码" class="input" /> -->
                    <a href="#" class="link">忘记密码？</a>
                    <button class="btn" @click="login">登录</button>
                </form>
            </div>

            <!-- 浮层 -->
            <div class="container_overlay">
                <div class="overlay">
                    <div class="overlay_panel overlay--left">
                        <img src="./icons/wish.png" alt="">
                        <button class="btn" id="signIn" @click="loginshow=true">去登录</button>
                    </div>
                    <div class="overlay_panel overlay--right">
                        <img src="./icons/wish.png" alt="">
                        <button class="btn" id="signUp" @click="loginshow=false">去注册</button>
                    </div>
                </div>
            </div>
        </div>

        <!-- 背景 -->
        <div class="slidershow">
            <div class="slidershow--image">
                <img src="./backgriundimg/bk4.jpg" alt="">
            </div>
        </div>

    </div>
</template>
  
<script>
import axios from 'axios';
import myinput from './myinput.vue';
export default {
    data() {
        return {
            posurl: '',
            loginshow: true,
            LoginIDcode: '',
            LoginPassword: '',
            RegistIDcode: '',
            RegistPassword: '',
            RegistName: '',
        };
    },
    components:{
        myinput,
    },
    methods: {
        isValidEmail(email) {
            const emailRegex = /^[1-9][0-9]*@qq\.com$/;
            return !emailRegex.test(email);
        },
        showRegist() {
            this.loginshow = false;
        },
        showLogin() {
            this.loginshow = true;
        },
        login() {//登录
            console.log(this.LoginIDcode);
            console.log(this.LoginPassword);
            
            if (this.LoginIDcode == '' || this.LoginPassword == '') {
                window.alert('账号密码不能为空');
                return;
            }

            this.posurl = `${this.$httpurl}/api/login?idcode=${this.LoginIDcode}&pass=${this.LoginPassword}`
            console.log(this.posurl);
            axios.post(this.posurl)
                .then((response) => {
                    let re = response.data;
                    if (response.data.isSuccessful == true) {
                        // window.alert('登录成功');
                        this.$router.push({ path: '/lobby', query: { key: re.key } });
                    } else {
                        window.alert(`登录失败\n${response.data.message}`);
                    }
                    console.log(response.data);
                })
                .catch((error) => {
                    window.alert(`登录失败\n${error}`);
                });
        },
        regist() {//注册
            if (this.RegistIDcode == '' || this.RegistPassword == '' || this.RegistName == '') {
                window.alert('账号,昵称或密码不能为空');
                return;
            }
            if(this.isValidEmail(this.RegistIDcode)){
                console.log(this.RegistIDcode);
                window.alert('请输入正确的邮箱');
                return;
            }
            this.posurl = `${this.$httpurl}/api/regist?idcode=${this.RegistIDcode}&name=${this.RegistName}&pass=${this.RegistPassword}`
            console.log(this.posurl);
            axios.post(this.posurl)
                .then((response) => {
                    if (response.data.isSuccessful == true) {
                        window.alert('注册成功');
                        this.LoginIDcode = this.RegistIDcode;
                        this.LoginPassword = this.RegistPassword;
                        this.showLogin();
                    } else {
                        window.alert(`注册失败\n${response.data.message}`);
                    }

                    console.log(response.data);
                })
                .catch((error) => {
                    window.alert(`注册失败\n${error}`);
                });
        },
        qqlogin() {
            window.alert('功能未实现');
        },
        wxlogin() {
            window.alert('功能未实现');
        }
    }
};
</script>
  
<style scoped>
:root {
	/* 颜色 */
	--white: #e9e9e9;
	--gray: #333;
	--blue: #095c91;
	--blue-r: #315a7491;
	--lightblue: #0393a3;

	/* 圆角 */
	--button-radius: 0.7rem;

	/* 大小 */
	--max-width: 758px;
	--max-height: 420px;

	font-size: 16px;
	font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Oxygen,
		Ubuntu, Cantarell, "Open Sans", "Helvetica Neue", sans-serif;
}

.bigbox {
	align-items: center;
	background-color: #e9e9e9;
	background-attachment: fixed;
	background-position: center;
	background-repeat: no-repeat;
	background-size: cover;
	display: grid;
	height: 100vh;
	place-items: center;
}

.form_title {
	font-weight: 300;
	margin: 0;
	margin-bottom: 1.25rem;
}

.link {
	color: #333;
	font-size: 0.9rem;
	margin: 1.5rem 0;
	text-decoration: none;
}

.container {
	background-color: #e9e9e9;
	border-radius: 0.7rem;
	box-shadow: 0 0.9rem 1.7rem rgba(0, 0, 0, 0.25),
		0 0.7rem 0.7rem rgba(0, 0, 0, 0.22);
	height: 420px;
	max-width: 758px;
	overflow: hidden;
	position: relative;
	width: 100%;
}

.container_form {
	height: 100%;
	position: absolute;
	top: 0;
	transition: all 0.6s ease-in-out;
}

.container--signin {
    
	left: 0;
	width: 50%;
	z-index: 5;
}

.container.right-panel-active .container--signin {
	transform: translateX(100%);
}

.container--signup {
	left: 0;
	opacity: 0;
	width: 50%;
	z-index: 4;
}

.container.right-panel-active .container--signup {
	-webkit-animation: show 0.6s;
	        animation: show 0.6s;
	opacity: 1;
	transform: translateX(100%);
	z-index: 8;
}

.container_overlay {
	height: 100%;
	left: 50%;
	overflow: hidden;
	position: absolute;
	top: 0;
	transition: transform 0.6s ease-in-out;
	width: 50%;
	z-index: 100;
}

.container.right-panel-active .container_overlay {
	transform: translateX(-100%);
}

.overlay {
	background-color: rgba(255, 255, 255, 0.25);
	background-attachment: fixed;
	background-position: center;
	background-repeat: no-repeat;
	background-size: cover;
	height: 100%;
	left: -100%;
	position: relative;
	transform: translateX(0);
	transition: transform 0.6s ease-in-out;
	width: 200%;
}

.container.right-panel-active .overlay {
	transform: translateX(50%);
}

.overlay_panel {
	align-items: center;
	display: flex;
	flex-direction: column;
	height: 100%;
	justify-content: center;
	position: absolute;
	text-align: center;
	top: 0;
	transform: translateX(0);
	transition: transform 0.6s ease-in-out;
	width: 50%;
}

.overlay--left {
	transform: translateX(-20%);
}

.container.right-panel-active .overlay--left {
	transform: translateX(0);
}

.overlay--right {
	right: 0;
	transform: translateX(0);
}

.container.right-panel-active .overlay--right {
	transform: translateX(20%);
}

.btn {
	background-color: #095c91;
	background-image: linear-gradient(90deg, #095c91 0%, #0393a3 74%);
	border-radius: 20px;
	border: 0.2px solid #315a7491;
	color: #e9e9e9;
	cursor: pointer;
	font-size: 0.8rem;
	font-weight: bold;
	letter-spacing: 0.1rem;
	padding: 0.9rem 4rem;
	text-transform: uppercase;
	transition: transform 80ms ease-in;
}

.form > .btn {
	margin-top: 1.5rem;
}

.btn:active {
	transform: scale(0.95);
}

.btn:focus {
	outline: none;
}

.form {
    background-color: #beefdace;
    box-shadow: 0 0 10px 5px #d8dcdf;
    display: flex;
    align-items: center;
    justify-content: center;
    flex-direction: column;
    padding: 0 3rem;
    height: 100%;
    text-align: center;
}

.input {
    background-color: #fff;
    border: none;
    border-radius: 5px;
    padding: 0.9rem 0.9rem;
    margin: 0.5rem 0;
    width: 100%;
}

@-webkit-keyframes show {
    0%,
    49.99% {
        opacity: 0;
        z-index: 4;
    }

    50%,
    100% {
        opacity: 1;
        z-index: 8;
    }
}

@keyframes show {
    0%,
    49.99% {
        opacity: 0;
        z-index: 4;
    }

    50%,
    100% {
        opacity: 1;
        z-index: 8;
    }
}


.slidershow {
    position: absolute;
    width: 100vw;
    height: 100vh;
    overflow: hidden;
}

.slidershow--image {
    position: absolute;
    width: 100%;
    height: 100%;
    background: no-repeat 50% 50%;
    background-size: cover;
    -webkit-animation-name: kenburns;
    animation-name: kenburns;
    -webkit-animation-timing-function: linear;
    animation-timing-function: linear;
    -webkit-animation-iteration-count: infinite;
    animation-iteration-count: infinite;
    -webkit-animation-duration: 16s;
    animation-duration: 16s;
    opacity: 1;
    -webkit-transform: scale(1.2);
    transform: scale(1.2);
}
.slidershow--image img{
    width: 100%;
    height: 100%;
}

</style>
  