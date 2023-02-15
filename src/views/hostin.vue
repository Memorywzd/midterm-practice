<template>
    <div class="about">
      <div class="top">
        <h1>This is an about page</h1>
        <h1>欢迎您{{ this.$store.state.username }}</h1>
      </div>
      <div class='menu'>
        <ul class="nav nav-list">
          <li>
            <a  class="" >
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
            <a href="#" class="">
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
       
        <div>根据请求报文,list 出每个源地址的 host 兴趣爱好（公司类型）
        </div>
       
       
      </div>
    </div>
  
  
  
  </template>
  
  <script>

  
  export default {
    data() {
      return {
        data:'',
      };
    },
    created(){
        this.isLogin();
        this.ver();
        this.get();
      },
    mounted() {
      
    },
    methods: {
      get() {
        let apiKey = '2WshDvM3a8682c1238af5a34f4eece5319e5a5637618bb7'
        let url = 'http://152.136.185.210:7878/api/hy66/category'
        this.axios.get(url)
          .then(res => {
            console.log(res);
            this.data = res.data;
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
  
  
  