const { defineConfig } = require('@vue/cli-service')
module.exports = defineConfig({
  devServer:{
    host:'0.0.0.0',
    port:8888,
    open:true,
    proxy:{
      '/api':{
        target:'http://152.136.185.210:7878/api/hy66',
        pathRewrite:{'^/api':''},
        ws:true,
        changeOrigion:true
      }
    }
  },
  transpileDependencies: true,
  // plugins:[
  //   new webpack.ProvidePlugin({
  //     $:'jquery',
  //     jQuery:'jquery'
  //   })
  // ]
})
