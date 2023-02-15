import Vue from 'vue'
import App from './App.vue'
import router from './router'
import store from './store'
import jquery from 'jquery'
import VueSession from 'vue-session'
import {MessageBox} from 'element-ui'
import './utils/axios'
Vue.use(VueSession)
Vue.prototype.$ = jquery
Vue.config.productionTip = false
Vue.prototype.$confirm = MessageBox.confirm
new Vue({
  router,
  store,
  render: h => h(App)
}).$mount('#app')
