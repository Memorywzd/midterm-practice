
import Vue from 'vue'
import Vuex from 'vuex'
import * as getters from './getters'
import * as mutations from './mutations'
import * as actions from './actions'
Vue.use(Vuex)

export default new Vuex.Store({
  state: {
    username :'',
    password:'',
    rolename:'',
    islogin:'',
    token:'',
    host:'',
    content_type:''
  },
  getters,
  mutations,
  actions
  
})
