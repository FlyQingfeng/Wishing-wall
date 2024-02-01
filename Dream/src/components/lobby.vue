<template>
    <water/>
    
    <div class="wishingshow">
        <bubble v-for="(bubble, index) in bubbles" :key="index" :name="bubble.name" :bubble="bubble"
            :likeWish="likeWish" :showmore="showmore">
        </bubble>
    </div>
    <lobbysidebar :userdata="muser"/>
    <!-- <sidebar :userdata="muser"/> -->
    <div class="search-box">
        <search_box type="email" class="search" placeholder="输入要查找的用户idcode" v-model="searchValue" :click="search" ></search_box>
    </div>
</template>
  
<script>
import axios from 'axios';
import water from './water.vue'
import sidebar from './slide.vue'
import lobbysidebar from './lobbyslide.vue'
import bubble from './bubble.vue'
import search_box from './search_box.vue';

export default {
    data() {
        return {
            muser:{
                key:"",
                idcode:"",
                name:"",
                wishnum:0,
                likednum:0,
                commentnum:0,
            },
            userkey:'',
            num:100,
            bubbles:[],
            searchValue:''
        }
    },
    components:{
        water,
        sidebar,
        lobbysidebar,
        bubble,
        search_box,
    },
    mounted() {
        // 用户登录秘钥
        this.userkey = this.$route.query.key;
        let mkey=encodeURIComponent(this.userkey);
        console.log(this.userkey);
        ///
        axios.get(`${this.$httpurl}/api/users/key?key=${mkey}`)
                .then((response) => {
                    console.log(response.data);
                    let redata=response.data;
                    this.muser.key=this.userkey;
                    this.muser.idcode=redata.idcode;
                    this.muser.name=redata.name;
                    this.muser.likednum=redata.likenum;
                    this.muser.wishnum=redata.wishnum;
                    this.muser.commentnum=redata.commentnum;
                    // muser
                });

        //请求愿望数据
        axios.get(`${this.$httpurl}/api/wishings/random?num=${this.num}`)
                .then((response) => {
                    console.log(response.data);
                    
                    let redata=response.data;
                    for (let i = 0; i < redata.length; i++) {
                        let obj=redata[i];
                        let ws=obj.wishings;
                        let bubble={
                            left: Math.random() * window.innerWidth,
                            name:obj.name,
                            idcode:obj.idcode,
                            duration: Math.random() * 2 + 3,
                            showText: false,
                            Wishing:ws[0].Wishing
                        };
                        this.bubbles.push(bubble);
                    }
                });
    },
    methods: {
        likeWish(bubble){
            let idcode=bubble.idcode;
            let wishname=idcode.wishname;
            axios.post(`${this.$httpurl}/api/likewish?mkey=${this.userkey}&idcode=${idcode}&wishname=${wishname}`)
                .then(()=>{
                    bubble.showText=false;
                })
        },
        showmore(idcode){
            // 用户登录秘钥
            this.userkey = this.$route.query.key;
            this.$router.push({path:'/other',query: { mkey: this.userkey,otherIdcode:idcode}});
        },
        search(){
            axios.get(`${this.$httpurl}/api/users?idcode=${this.searchValue}`)
                .then((res)=>{
                    let data=res.data;
                    console.log(data);
                    if(data.name==''&&data.idcode==''){
                        window.alert('账号不存在');
                    }else{
                        this.showmore(this.searchValue);
                    }
            })
        }
    }
}
</script>
  
<style scoped="">
.search-box{
    width: 100%;
    height: 100%;
    position: absolute;
    top: 0;
    left: 0;
    display: flex;
    pointer-events: none;
}
.search{
    pointer-events:all;
    position: absolute;
    bottom: 0;
}
.bubble {
    text-align: center;
    position: fixed;
    bottom: 0;
    width: 60px;
    height: 60px;
    background: rgba(0, 100, 200, 0.5);
    border-radius: 50%;
    animation: rise 10s infinite;
    z-index: 0;
}
.bubble-text {
    position: absolute;
    top: 50%;
    left: 50%;
    transform: translate(-50%, -50%);
    text-align: center;
    white-space: nowrap; /* 不换行 */
    overflow: hidden; /* 超出部分隐藏 */
    text-overflow: ellipsis; /* 使用省略号表示超出部分 */
}

@keyframes rise {
    0% {
        opacity: 0.2;
        bottom: 0;
    }
    100% {
        opacity: 1;
        bottom: 100%;
    }
}
@keyframes bobble {
    0% {
        transform: scale(1);
    }
    50% {
        transform: scale(1.5);
    }
    100% {
        transform: scale(0.5);
    }
}
</style>
  