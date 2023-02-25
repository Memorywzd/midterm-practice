<template>
    <div class="about">
      <div class="top">
        
        <h1>欢迎您{{ this.$store.state.username }}</h1>
      </div>
      <div class='menu'>
        <ul class="nav nav-list">
          <li>
            <a  class="" href="/" >
              <i class=""></i>
              <span class="menu-text" > 报文分析 </span>
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
                <a href="host">
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
       
        <div>根据域名及域名注册公司，指定域名类型（游戏公司、科技公司、政府事业单位、高校、
            医疗机构等）</div>
        <div class="form">
              <div class="item">
                <label>域名：</label><input type="text" name="" v-model="domain" >
                <!-- v-model把输入的值传输给name变量 -->
                <br/>
              </div>
              <div class="item">
                <label>域名公司：</label><input type="" name="" v-model="domaincompany" placeholder="">
                <br/>
              </div>
              <button>search</button>
          </div>
          
          <table class="metable" border="1">
            <thead>
                <tr>
                    <th>域名</th>
                    <th>域名公司</th>
                    <th>域名类型</th>
                    
                </tr>
            </thead>
            <tbody id="appproject">
                <tr v-for="li in data">
                    <td>{{li.domain}}</td>
                    <td>{{li.domaincompany}}</td>
                    <td>{{li.domainType}}</td>
                   
                   
                </tr>
            </tbody>
        </table>
      </div>
    </div>
  
  
  
  </template>
  
  <script>

  
  export default {
    data() {
      return {
        data:'',
        domain:'',
        domaincompany:''
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
        let url = 'http://172.25.180.18/whois/domainType?domain='+this.domain+'&domaincompany='+this.domaincompany
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

    #background{
        width: 100%;
        height: 100%;
        background: url("");
        background-size:100% 100%;
        position: fixed;
        top: 0;
        left: 0;
    }
    
    .container{
      width: 480px;
      height: 300px;
      position: absolute;
      top: 50%;
      left: 50%;
      transform: translate(-50%,-50%);
      background:#00000090;
      text-align: center;
      border-radius: 20px;
      margin-top: 10px;
    }
    .container h1{
      color: rgb(0, 0, 0);
      margin-left: 20px;
    }
    .item {
      color: rgb(0, 0, 0);
      margin-left: 15%;
      margin-top: 35px;
      font-size: 20px;
      text-align: left;
    }
    .item label{
      float:left;
      width: 5em;
      margin-right: 1em;
      text-align: right;
    }
    input{
      margin-left: -5px;
      padding: 4px;
      border: solid 1px #4e5ef3;
      outline: 0;
      font: normal 13px/100% Verdana,Tahoma,sans-serif;
      width: 200px;
      height: 23px;
      background:#f1f1f190;
      box-shadow: rgba(0, 0, 0, 0.1) 0px 0px 8px;
    }
    button{
      position: relative;
      height: 33px;
      width: 100px;
      background: rgba(35, 19, 252, 0.425);
      border-radius: 10px;
      margin-top: 18px;
      box-shadow: none;
      color: white;
      margin-left: 370px;
      margin-right: 10px;
    
    }
    .keep{
      color: white;
    }
    .keep input{
      width: 15px;
      height: 15px;
      margin-top: 7px;
      margin-left: 10px;
      margin-right: 10px;
    }
    
   
    .metable{
      width: 100%;
    }
    .form{
      position: relative;
      left: 300px;
    }
    
    
  </style>
  
  
  