<template>
    <div class="about">
      <div class="top">
        <h1>This is an about page</h1>
        <h1>欢迎您{{ this.$store.state.username }}</h1>
      </div>
      <div class='menu'>
        <ul class="nav nav-list">
          <li>
            <a  class=""  href="/">
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
                <a href="hostin">
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
       
      <div>统计各目的地址接收到的 HTTP 报文的数量</div>
        
      <table class="metable" border="1">
        <thead>
            <tr>
                <th>项目名称</th>
            </tr>
        </thead>
        <tbody id="appproject">
            <tr v-for="li in list">
                <td>{{li.miniWallkey}}</td>
                <td>{{li.maitKey}}</td>
                <td>{{li.title}}</td>
               
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
        list:''
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
        // let apiKey = '2WshDvM3a8682c1238af5a34f4eece5319e5a5637618bb7'
        let url = 'http://152.136.185.210:7878/api/hy66/category'
        this.axios.get(url)
          .then(res => {
            console.log(res);
            this.data = res.data;
            this.list = res.data.data.category.list
            console.log(this.list)
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
  .m-table-wrap {
    color: rgba(0, 0, 0, 0.65);
    font-size: 14px;
    line-height: 1.5;
   
          border-top-right-radius: 4px;

}
.m-loading {
  position: absolute;
  width: 100%;
  height: 100%;
}
.m-empty {
  padding: 48px 16px;
  color: rgba(0, 0, 0, 0.25);
  font-size: 14px;
  text-align: center;
  background: #fff;
  border-bottom: 1px solid #e8e8e8;
  border-radius: 0 0 2px 2px;
}
.m-empty {
  padding: 48px 16px;
  color: rgba(0, 0, 0, 0.25);
  font-size: 14px;
  text-align: center;
  background: #fff;
  border-bottom: 1px solid #e8e8e8;
  border-radius: 0 0 2px 2px;}
  .m-empty {
    padding: 48px 16px;
    color: rgba(0, 0, 0, 0.25);
    font-size: 14px;
    text-align: center;
    background: #fff;
    border-bottom: 1px solid #e8e8e8;
    border-radius: 0 0 2px 2px;}
  .u-empty-icon {
      width: 64px;
      height: 41px;
      margin-bottom: 8px;
    }
    .u-empty-desc {
      color: rgba(0, 0, 0, 0.25);
      font-size: 14px;
    }
.m-body {
      position: relative;
        
        
         
          
        }
        td {
          padding: 16px;
          border-bottom: 1px solid #e8e8e8;
          transition: background .3s;
          overflow: hidden;
          white-space: nowrap;
          text-overflow: ellipsis;
        }
tbody tr {
      transition: background .3s;
        
        
  }

  </style>
  
  
  