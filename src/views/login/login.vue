<template>
    <div id="background">
        <div class="container">
            <form action="">
              <h1>系统登陆</h1>
              <div class="form">
                  <div class="item">
                    <label>用户名：</label><input type="text" name="" v-model="username" :placeholder="println">
                    <!-- v-model把输入的值传输给name变量 -->
                    <br/>
                  </div>
                  <div class="item">
                    <label>密码：</label><input type="password" name="" v-model="password" placeholder="请输入密码">
                    <br/>
                  </div>
                  <div class="keep">
                    <input  id="yes" type="radio" value="0" ><!-- 点击选中 -->
                    <label for="yes">保持登录状态</label>
                  </div>
              </div>
              
            </form>
                  <button  type="submit" @click="handlelogin">登录</button>
                  <button  type="submit" @click="open">登出</button>
                  <!-- v-on点击按钮触发handlelogin方法 -->
                  <button>注册</button>
              <router-view></router-view>
        </div>
    </div>
    </template>
    <script>
    export default{
        data(){
            return {
                username:'',
                password:'',
                println:''
                
            }
        },
           
        //to: Route: 即将要进入的目标路由对象
        //from: Route: 当前导航正要离开的路由
        //next()：必须执行
        beforeRouteEnter: (to, from, next) => {
            next(vm => {
            // vm 就是当前组件的实例相当于上面的 this，所以在 next 方法里你就可以把 vm 当 this 来用了。
                vm.$store.dispatch("setUser", null);
            });
        },


        methods:{
            handlelogin(){
              let resp = {username : this.username,password : this.password}
              sessionStorage.setItem("user",JSON.stringify(resp))
              sessionStorage.setItem('userToken',resp.password)
              this.$store.dispatch('setUser',JSON.stringify(resp))
              this.$store.dispatch('setToken',resp.password)
              this.$router.push('/about')
                
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
              this.println = "请输入用户名"
              console.log('未登陆')
            }
          },

          open() {
            this.$confirm('确定退出当前账号, 是否继续?', '提示', {
                confirmButtonText: '确定',
                cancelButtonText: '取消',
                type: 'warning'
            }).then(() => {
                this.println = "请输入用户名"
                console.log('success')
            }).catch(() => {
                this.exits = false;
                console.log('false')
            });
        },



    },
    created(){
      this.isLogin();
      this.ver();
    }
    }
    </script>
    //css
    <style scoped>
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
      color: aliceblue;
      margin-left: 20px;
    }
    .item {
      color: white;
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
      margin-left: 40px;
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
    
    </style>
    
    