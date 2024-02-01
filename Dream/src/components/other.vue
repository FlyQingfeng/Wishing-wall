<template>
    <div id="app" class="container">
        <div class="sidebar">
            <h1 class="title">许愿墙</h1>
            <div class="profile">
                <h2 class="name">{{ otheruser.name }}</h2>
                <p class="idcode">{{ otheruser.idcode }}</p>
                <div class="details">
                    <p class="details-text">被点赞数量：{{ otheruser.likednum }}</p>
                    <p class="details-text">被评论数量：{{ otheruser.commentnum }}</p>
                </div>
            </div>
            <div class="wish-list">
                <div class="wish-scroll-list">
                <div class="wish-list-top">
                    <h3 class="sub-title">愿望列表</h3>
                </div>
                <ul>
                    <li v-for="(wish, index) in otheruser.wishings" :key="index" @click="selectWish(index)" class="wish-item">
                        {{ wish.Wishing }}
                        <!-- <button class="delete-button" @click="deleteItem(index)">删除</button> -->
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
                <h1>TA的愿望</h1>
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
    </div>
</template>
  
<script>

import axios from 'axios';

import likeCheckbox from './like.vue'
export default {
    components:{
        likeCheckbox,
    },
    data() {
        return {
            otheruser: {
                idcode: "",
                name: "",
                wishnum: 0,
                likednum: 0,
                commentnum: 0,
                wishings: Array
            },
            mkey: '',
            mname:'',
            othericode:'',
            selectedWish: null,
            inputValue: '',  // 存储许愿值
            inputcommentValue: '',//评论的值
        };
    },
    mounted() {
        this.mkey = this.$route.query.mkey;
        this.otheridcode = this.$route.query.otherIdcode;

        axios.get(`${this.$httpurl}/api/users/key?key=${this.mkey}`)
            .then((response) => {
                let redata = response.data;
                this.mname = redata.name;
            });
        axios.get(`${this.$httpurl}/api/users?idcode=${this.otheridcode}`)
            .then((response) => {
                let redata = response.data;
                this.otheruser.name = redata.name;
                this.otheruser.idcode = redata.idcode;
                this.otheruser.likednum = redata.likenum;
                this.otheruser.wishnum = redata.wishnum;
                this.otheruser.commentnum = redata.commentnum;
            });
        axios.get(`${this.$httpurl}/api/wishings?idcode=${this.otheridcode}`)
            .then((response) => {
                console.log(response.data);
                let redata = response.data;
                this.otheruser.wishings = redata.wishings;
                this.selectedWish = redata.wishings!=null?this.otheruser.wishings[0]:[];
            });
    },
    methods: {
        gohome(){
            this.$router.push({path:'/lobby',query: { key: this.mkey}});
        },
        selectWish(index) {
            this.selectedWish = this.otheruser.wishings[index];
        },
        commentwish() {//评论
            if (this.inputcommentValue != '') {
                let mkey = encodeURIComponent(this.mkey);
                let url = `${this.$httpurl}/api/commentwish?mkey=${mkey}&comment=${this.inputcommentValue}&idcode=${this.otheruser.idcode}&wishname=${this.selectedWish.Wishing}`;
                axios.post(url).then((res) => {
                    let data = res.data;
                    if (data.isSuccessful) {
                        let pl=this.inputcommentValue;
                        let name=this.mname;
                        let time=new Date().toLocaleString()
                        let idcode=this.otheruser.idcode;
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
            let mkey = encodeURIComponent(this.mkey);
                let url = `${this.$httpurl}/api/likewish?mkey=${mkey}&idcode=${this.otheruser.idcode}&wishname=${this.selectedWish.Wishing}`;
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
  