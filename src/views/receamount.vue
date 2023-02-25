<template>
  <div class="about">
    <div class="top">

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
     
      <div>统计接收的 HTTP request 数量</div>
      <table class="metable" border="1">
        <thead>
            <tr>
                <th>源地址</th>
                <th>request数量</th>
            </tr>
        </thead>
        <tbody id="appproject">
            <tr v-for="li in data">
                <td>{{li.srcip}}</td>
                <td>1</td>
                
               
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
      obj:{}
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
    cacu(){
      for(var i in this.data){
        var j = i.srcip
        if(this.obj[j]){
          this.obj[j] = this.obj[j]+ 1;
        }
        else{
          this.obj[j] = 1;
        }
      console.log(this.obj)
      }
    },
    get() {
      let apiKey = '2WshDvM3a8682c1238af5a34f4eece5319e5a5637618bb7'
      let url = 'http://172.25.180.18/analyse/requestFeature'
      this.axios.get(url)
        .then(res => {
          console.log(res);
          this.data = res.data;
          this.cacu();
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


