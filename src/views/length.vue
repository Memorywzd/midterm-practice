<template>
    <div class="about">
      <div class="top">

        <h1>欢迎您{{ this.$store.state.username }}</h1>
      </div>
      <div class='menu'>
        <ul class="nav nav-list">
          <li>
            <a  class="" href="/">
              <i class=""></i>
              <span class="menu-text" @click="function(){this.$router.push('/')}"> 报文分析 </span>
              <b class=""></b>
            </a>
  
            <ul class="submenu">
              <li>
                <a href="/about">
                  请求报文
                </a>
              </li>
  
              <li>
                <a href="/aboutresp">
                  <i class="icon-double-angle-right"></i>
                  响应报文
                </a>
              </li>
                              <li>
                <a href="/sendamount">
                  <i class="icon-double-angle-right"></i>
                  请求报文数量
                </a>
              </li>
                              <li>
                <a href="/receamount">
                  <i class="icon-double-angle-right"></i>
                  接收报文数量
                </a>
              </li>
                          
              <li>
                <a href="/length">
                  <i class="icon-double-angle-right"></i>
                  帧长度数据
                </a>
              </li>
                             
            </ul>
          </li>
  
          <li>
            <a href="/" class="">
              <i class="icon-list"></i>
              <span class="menu-text"> Whois模块 </span>
  
              <b class=""></b>
            </a>
  
            <ul class="submenu">
              <li>
                <a href="/hostin">
                  <i class=""></i>
                  请求报文Host兴趣
                </a>
              </li>
  
              <li>
                <a href="/htmltxt">
                  <i class=""></i>
                  HTML&TXT分析
                </a>
              </li>
                            
              <li>
                <a href="/host">
                  <i class=""></i>
                  域名性质分析
                </a>
              </li>
                              
            </ul>
          </li>
  
  
        </ul>
      </div>
      <div>
        <div>统计请求报文的最大帧大小、最小帧大小、平均帧大小</div>
        <h3>最大帧大小</h3>
        {{ max1 }}
        <h3>最小帧大小</h3>
        {{ min1 }}
        <h3>平均帧大小</h3>
        {{ l1 }}
        <div>统计响应报文的最大帧大小、最小帧大小、平均帧大小</div>
        <h3>最大帧大小</h3>
        {{ max2 }}
        <h3>最小帧大小</h3>
        {{ min1 }}
        <h3>平均帧大小</h3>
        {{l2}}
      </div>
    </div>
  
  
  
  </template>
  
  <script>

  
  export default {
    data() {
      return {
        data1:'',
        m1:0,
        l1:0,
        min1:500,
        max1:0,
        data2:'',
        l2:0,
        min2:500,
        max2:0
      };
    },
    created(){
        this.isLogin();
        this.ver();
        this.get1();
        this.get2();
      },
    mounted() {
      
    },
    methods: {
      beforeRouteEnter: (to, from, next) => {
            next(vm => {
            // vm 就是当前组件的实例相当于上面的 this，所以在 next 方法里你就可以把 vm 当 this 来用了。
                // vm.$store.dispatch("setUser", null);
            });
        },
      cacu1(){

       for(var i = 0 ;i < this.data1.length;++i ){
       
        this.l1 = this.l1 + Number(this.data1[i].Framelength);
        if(Number(this.data1[i].Framelength) > this.max1){
          this.max1 = this.data1[i].Framelength;
        }
        if(Number(this.data1[i].Framelength) < this.min1){
          this.min1 = this.data1[i].Framelength;
        }
       } 
       this.l1 = this.l1/this.data1.length;
      },
      cacu2(){

      for(var i = 0 ;i < this.data2.length;++i ){
       
       this.l2 = this.l2 + Number(this.data2[i].frameLength);
       if(Number(this.data2[i].frameLength) > this.max2){
         this.max2 = this.data2[i].frameLength;
       }
       if(Number(this.data2[i].frameLength) < this.min2){
         this.min2 = this.data2[i].frameLength;
       }
      } 
      this.l2 = this.l2/this.data2.length;
      },
      get1() {
        let apiKey = '2WshDvM3a8682c1238af5a34f4eece5319e5a5637618bb7'
        let url = 'http://172.25.180.18/analyse/requestFeature'
        this.axios.get(url)
          .then(res => {
            console.log(res);
            this.data1 = res.data;
            this.cacu1()
          })
         
        },
        get2() {
        let apiKey = '2WshDvM3a8682c1238af5a34f4eece5319e5a5637618bb7'
        let url = 'http://172.25.180.18/analyse/responseFeature'
        this.axios.get(url)
          .then(res => {
            console.log(res);
            this.data2 = res.data;
            this.cacu2();
          })
        },
        isLogin(){
              if(sessionStorage.getItem('user')!=null && sessionStorage.getItem('userToken')){
                this.$store.commit("userStatus",sessionStorage.getItem("user"));
              }else{
                this.$store.commit("userStatus",null)
              }
              return this.$store.getters.islogin
            },
        ver(){
  
              if(this.$store.state.islogin){
                this.println = this.$store.state.username,
                console.log('登陆');
              }else{
                
                console.log('未登陆')
                this.$router.push('/')
              }
              },
     
    }
  };
  </script>
  <style>
  .top{
    background-color: rgb(248, 248, 248);
    border-bottom: 2px solid rgb(243, 243, 243);
    padding: 1px;
  }
  li a{
    color:rgba(107, 150, 194, 0.913);
    text-decoration: none;
  }
  </style>
  
  
  