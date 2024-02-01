<template>
    <div id="app" class="container">
        <div class="sidebar">
            <h1 class="title">许愿墙</h1>
            <div class="profile">
                <h2 class="name">{{ muser.name }}</h2>
                <p class="idcode">{{ muser.idcode }}</p>
                <div class="details">
                    <p class="details-text">被点赞数量：{{ muser.likednum }}</p>
                    <p class="details-text">被评论数量：{{ muser.commentnum }}</p>
                </div>
            </div>
            <div class="wish-list">
                <div class="wish-scroll-list">
                <div class="wish-list-top">
                    <h3 class="sub-title">愿望列表</h3>
                    <button class="makewishbtn" @click="showInputui">许个愿</button>
                </div>
                <ul>
                    <li v-for="(wish, index) in muser.wishings" :key="index" @click="selectWish(index)" class="wish-item">
                        {{ wish.Wishing }}
                        <button class="delete-button" @click="deleteItem(index)">删除</button>
                    </li>
                </ul>
                </div>
                <div>
                    <button class="gohomebtn" @click="gohome">返回大厅</button>
                </div>
            </div>
            
        </div>
        <div class="main-content" v-if="selectedWish != null">
            <div>
                <h1>我的愿望</h1>
            </div>
            <div class="wish-detail">
                <div class="comments-top">
                    <h2 class="comments-title">{{ selectedWish.Wishing }}</h2>
                    <likeCheckbox :click="like"/>
                    <!-- <button @click="like">
                        <img src="\src\assets\like.png" alt="">
                    </button> -->
                </div>
                <p class="description">时间：{{ selectedWish.Time }}</p>
                <p class="description">点赞数量：{{ selectedWish.LikeNum }}</p>
            </div>
            <div class="comments">
                <h3 class="comments-sub-title">评论</h3>
                <div class="scroll-list">
                    <div v-for="(comment, index) in selectedWish.Comments" :key="index" class="comment">
                        <p class="comment-name-text">{{comment.Name!=''?comment.Name :"[匿名用户]"}}</p>
                        <p class="comment-text">{{ comment.Comment }}</p>
                        <p class="comment-date">{{ comment.Time }}</p>
                    </div>
                </div>
                <div class="comment-box">
                    <textarea v-model="inputcommentValue" placeholder="写下你的评论..."></textarea>
                    <button @click="commentwish">评论</button>
                </div>
            </div>

        </div>
        <wishCard class="wish-page" v-if="selectedWish == null" v-model="inputValue" :fun="confirmWish"/>
        <!-- <div class="wish-page" v-if="selectedWish == null">
            <div class="wish-card">
                <h1>许个愿吧</h1>
                <div class="input-area">
                    <textarea v-model="inputValue" placeholder="写下你的愿望..."></textarea>
                    <button class="confirm-button" @click="confirmWish">确定</button>
                </div>
            </div>
        </div> -->
    </div>
</template>
  
<script>

import axios from 'axios';
import wishCard from './wish-card.vue';
import likeCheckbox from './like.vue'
export default {
    data() {
        return {
            muser: {
                key: "",
                idcode: "",
                name: "",
                wishnum: 0,
                likednum: 0,
                commentnum: 0,
                wishings: Array
            },
            key: '',
            selectedWish: null,
            inputValue: '',  // 存储许愿值
            inputcommentValue: '',//评论的值
        };
    },
    components:{
        wishCard,
        likeCheckbox,
    },
    mounted() {
        this.key = this.$route.query.key;
        let mkey = encodeURIComponent(this.key);
        let idcode = '';
        axios.get(`${this.$httpurl}/api/users/key?key=${mkey}`)
            .then((response) => {
                // console.log(response.data);
                let redata = response.data;
                this.muser.key = this.userkey;
                this.muser.idcode = redata.idcode;
                this.muser.name = redata.name;
                this.muser.likednum = redata.likenum;
                this.muser.wishnum = redata.wishnum;
                this.muser.commentnum = redata.commentnum;
                axios.get(`${this.$httpurl}/api/wishings?idcode=${redata.idcode}`)
                    .then((response) => {
                        // console.log(response.data);
                        let redata = response.data;
                        this.muser.wishings = redata.wishings;
                    });
            });
    },
    methods: {
        gohome(){
            this.$router.push({path:'/lobby',query: { key: this.key}});
        },
        selectWish(index) {
            this.selectedWish = this.muser.wishings[index];
        },
        showInputui() {
            this.selectedWish = null;
        },
        confirmWish() {
            let mkey = this.key;
            let url = `${this.$httpurl}/api/makewish?key=${mkey}&wish=${this.inputValue}`
            // console.log(url);
            axios.post(url).then((res) => {
                let data = res.data;
                console.log(data);
                if (data.isSuccessful) {
                    location.reload();
                }else{
                    window.alert(`许愿失败：\n${data.message}`);
                }
            })
            // console.log(this.inputValue);  // 打印输入框的值
        },
        deleteItem(index) {//删除愿望
            // this.muser.wishings.splice(index, 1);
            let mkey = encodeURIComponent(this.key);

            let url = `${this.$httpurl}/api/wishing/delete?key=${mkey}&wish=${this.muser.wishings[index].Wishing}`;
            // console.log(url);
            axios.post(url).then((response) => {
                if(response.data.isSuccessful){
                    this.muser.wishings.splice(index, 1);
                    if(this.muser.wishings.length===0){
                        this.selectedWish = null;
                    }
                }
            });
        },
        commentwish() {//评论
            if (this.inputcommentValue != '') {
                let mkey = encodeURIComponent(this.key);
                let url = `${this.$httpurl}/api/commentwish?mkey=${mkey}&comment=${this.inputcommentValue}&idcode=${this.muser.idcode}&wishname=${this.selectedWish.Wishing}`;
                axios.post(url).then((res) => {
                    let data = res.data;
                    if (data.isSuccessful) {
                        let pl=this.inputcommentValue;
                        let name=this.muser.name;
                        let time=new Date().toLocaleString()
                        let idcode=this.muser.idcode;
                        let comment={
                            Comment: pl,
					        Name: name,
					        Time: time,
					        idcode: idcode
                        };
                        if(this.selectedWish.Comments==null){
                            this.selectedWish.Comments=[];
                        }
                        this.selectedWish.Comments.push(comment);
                        this.inputcommentValue = "";
                        console.log('评论成功');
                    }
                }).catch((err) => {
                    console.log(err);
                });
            } else {
                window.alert(`评论不能为空`);
            }
        },
        like(){
            let mkey = encodeURIComponent(this.key);
                let url = `${this.$httpurl}/api/likewish?mkey=${mkey}&idcode=${this.muser.idcode}&wishname=${this.selectedWish.Wishing}`;
                axios.post(url).then((res) => {
                    let data = res.data;
                    if (data.isSuccessful) {
                        this.selectedWish.LikeNum++;
                    }
                }).catch((err) => {
                    console.log(err);
                });
        }
    }
};
</script>
  
<style scoped="">
@import url('./css/wishshow.css');
</style>
  