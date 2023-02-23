import Vue from 'vue'
import VueRouter from 'vue-router'
import HomeView from '../views/HomeView.vue'
import Login from '../views/login/login.vue'

Vue.use(VueRouter)

const routes = [
  {
    path:'/',
    redirect:'login'
  },
  {
    path: '/login',
    name: 'login',
    component: Login
  },
  {
    path: '/about',
    name: 'about',
    component: () => import(/* webpackChunkName: "about" */ '../views/AboutView.vue')
  },
  {
    path: '/reqdetail',
    name: 'reqdetail',
    component: () => import(/* webpackChunkName: "about" */ '../views/reqdetail.vue')
  },
  {
    path: '/aboutresp',
    name: 'aboutresp',
    component: () => import(/* webpackChunkName: "about" */ '../views/Aboutresp.vue')
  },
  {
    path: '/respdetail',
    name: 'respdetail',
    component: () => import(/* webpackChunkName: "about" */ '../views/respdetail.vue')
  },
  {
    path: '/sendamount',
    name: 'sendamount',
    component: () => import(/* webpackChunkName: "about" */ '../views/sendamount.vue')
  },
  {
    path: '/receamount',
    name: 'receamount',
    component: () => import(/* webpackChunkName: "about" */ '../views/receamount.vue')
  },
  {
    path: '/htmltxt',
    name: 'htmltxt',
    component: () => import(/* webpackChunkName: "about" */ '../views/htmltxt.vue')
  },
  {
    path: '/host',
    name: 'host',
    component: () => import(/* webpackChunkName: "about" */ '../views/host.vue')
  },
  {
    path: '/hostin',
    name: 'hostin',
    component: () => import(/* webpackChunkName: "about" */ '../views/hostin.vue')
  },

  {
    path:'/length',
    name:'length',
    component: () => import(/* webpackChunkName: "about" */ '../views/length.vue')
  },

  {
    path: '/analysize',
    name: 'analysize',
    component: () => import(/* webpackChunkName: "about" */ '../views/analysize/analysize.vue'),
    children:[
      {
        path:'/analysize/in',
        name:'in',
        component: () => import(/* webpackChunkName: "about" */ '../views/analysize/in/in.vue'),
      },
      {
        path:'/out',
        name:'out',
        component: () => import(/* webpackChunkName: "about" */ '../views/AboutView.vue'),
      },
    ]
  },
  {
    path: '/whois',
    name: 'whois',
    component: () => import(/* webpackChunkName: "about" */ '../views/AboutView.vue')
  },

]
const isLogin = true
//路由守卫

const router = new VueRouter({
  mode: 'history',
  base: process.env.BASE_URL,
  routes
})

export default router
