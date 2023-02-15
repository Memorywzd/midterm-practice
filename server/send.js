const express = require('express');
const app = express();
const os = require('os');
app.use(express.static("./"));
app.post('/ms/coupon/template/pageList', function (req, res) {
    res.setHeader('Content-Type', 'text/plain;charset=utf8');
    res.end(JSON.stringify({
        code: 0,
        data: {
            list:[
                {
                    couponTemplateId: 1,
                    couponName: '小布丁代金券',
                }
            ],
            pageIndex: 1,
            pageSize: 10,
            total: 3
        },
        msg: 'success',
        success: true
    }))
})
// 添加链接保存
app.get('/ms/coupon/template/get', function (req, res) {
    res.setHeader('Content-Type', 'text/plain;charset=utf8');
    res.end(JSON.stringify({
        code: 0,
        data: {
          Name: 'abcd',
          Data: '123',
        },
        msg: 'success',
        success: true
    }))
})
const server = app.listen(8888, function (req, res, next) {
    let host = server.address().address
    let port = server.address().port
    console.log("http://%s:%s", host, port)
})

