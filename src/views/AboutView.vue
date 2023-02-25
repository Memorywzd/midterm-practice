<!-- 统计请求报文访问类型（饼图） -->
<!-- list 出所有请求报文的特征，并在 Host 字段处链接显示另一页面，list 出该 host 域名信息 -->
<template>
  <div class="about">
    <div class="top">
      <h1>This is an about page</h1>
      <h1>欢迎您{{ this.$store.state.username }}</h1>
      
    </div>
    <div class='menu'>
      <ul class="nav nav-list">
        <li>
          <a class="" href="/">
            <i class=""></i>
            <span class="menu-text"> 报文分析 </span>
            <b class=""></b>
          </a>

          <ul class="submenu">
            <li>
              <a href="/about">
                <i class=""></i>
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
      <!-- list 出所有请求报文的特征，并在 Host 字段处链接显示另一页面，list 出该 host 域名信息->8 -->
      <div>请求报文的特征</div>

      <table class="metable" border="1">
        <thead>
            <tr>
                <th>请求报文的特征</th>
            </tr>
        </thead>
        <tbody id="appproject">
            <tr v-for="li in data">
                <td>{{li.srcip}}</td>
                <td>{{li.destip}}</td>
                <td>{{li.Framelength}}</td>
                <td>{{li.Accesstype}}</td>
                <td>{{li.interest}}</td>
               
            </tr>
        </tbody>
    </table>

      <!-- 统计请求报文访问类型（饼图）-->
      <div class="echart" id="mychart" :style="myChartStyle"></div>
    </div>
  </div>



</template>

<script>
import * as echarts from "echarts";

export default {
  data() {
    return {
      data:'',
      list:'',
      myChart: {},
      pieData: [
        {
          value: 463,
          name: "1"
        },
        {
          value: 395,
          name: "2"
        },
        {
          value: 157,
          name: "3"
        },
        {
          value: 149,
          name: "4"
        },
        {
          value: 147,
          name: "5"
        }
      ],
      pieName: [],
      myChartStyle: {  width: "100%", height: "400px" } //图表样式
    };
  },
  created(){
      this.isLogin();
      this.ver();
      this.get();
    },
  mounted() {
    this.initDate(); //数据初始化
    this.initEcharts();
  },
  methods: {
    host(e){
      sessionStorage.setItem("host",e)
    },
    get() {
      let apiKey = '2WshDvM3a8682c1238af5a34f4eece5319e5a5637618bb7'
      let url = 'http://172.25.180.18/analyse/requestFeature'
      this.axios.get(url)
        .then(res => {
          console.log(res);
          this.data = res.data;
          console.log(this.data)
          this.list = res.data;
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
    initDate() {
      for (let i = 0; i < this.pieData.length; i++) {
        // this.xData[i] = i;
        // this.yData =this.xData[i]*this.xData[i];
        this.pieName[i] = this.pieData[i].name;
      }
    },
    initEcharts() {
      // 饼图
      const option = {
        legend: {
          // 图例
          data: this.pieName,
          right: "10%",
          top: "30%",
          orient: "vertical"
        },
        title: {
          // 设置饼图标题，位置设为顶部居中
          text: "统计请求报文访问类型",
          top: "0%",
          left: "center"
        },
        series: [
          {
            type: "pie",
            label: {
              show: true,
              formatter: "{b} : {c} ({d}%)" // b代表名称，c代表对应值，d代表百分比
            },
            radius: "30%", //饼图半径
            data: this.pieData
          }
        ]
      };
      console.log(this.seriesData);
      const optionFree = {
        xAxis: {},
        yAxis: {},
        series: [
          {
            data: this.seriesData,
            type: "line",
            smooth: true
          }
        ]
      };
      this.myChart = echarts.init(document.getElementById("mychart"));
      this.myChart.setOption(option);
      //随着屏幕大小调节图表
      window.addEventListener("resize", () => {
        this.myChart.resize();
      });
    }
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


