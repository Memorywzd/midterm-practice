# 中期实践任务

基于 libpcap 的 WLAN 帧 HTTP 载荷分析系统的设计与实现

## 第零部分：前期准备

- IDE：
  - C/C++ CLion或Visual Studio (Code)，禁止使用dev c，在有条件的情况下建议使用copilot、ChatGPT协助代码编写
  - python PyCharm Pro，注意不要使用无法开发web系统的community版本
  - JavaScript 任意
- 版本控制：
  - 掌握基本的Git、GitHub使用。如未使用过相关内容，参考教程[Git教程 - 廖雪峰的官方网站](https://www.liaoxuefeng.com/wiki/896043488029600)
  - 仓库地址 [Memorywzd/midterm-practice](https://github.com/Memorywzd/midterm-practice)，完成clone后转为private仓库，单独授权
  - master分支不允许提交，只接受来自其他分支的merge；后端程序、web程序、前端页面分别在不同分支上提交，互不影响；前端分支属系统附件，不向master合并
  - 无法使用GitHub的解决方案：
    1. 有科学上网环境：`git config --global http.proxy protocol://localhost:port` 其中protocol取值http或socks5
    2. 无科学上网环境：[docmirror/dev-sidecar](https://github.com/docmirror/dev-sidecar)
- 环境依赖：
  - Ubuntu Server 20.04虚拟机一台备用。除非本人特别精通其他Linux发行版，统一使用此版本。镜像下载地址：[清华源](https://mirrors6.tuna.tsinghua.edu.cn/ubuntu-releases/focal/ubuntu-20.04.5-live-server-amd64.iso)
  - （后端开发成员）C/C++环境自备，注意检查Linux开发所需的头文件
  - （Web系统开发成员）python 3.6+
  - （前端开发成员）node.js 16+，参考环境安装教程：[安装NodeJs和NPM | Jetzihan](https://jetzihan.netlify.app/docs/frontend/fetricks/install-nodejs-and-npm/)

----

## 第一部分：后端抓包C/C++程序
- package filter
- 报文解析

----

## 第二部分：Web系统后端

- 数据库设计
- api实现

----

## 第三部分：前端页面，文档撰写

- 选用vue或react
- 参照api doc编写，对api提出需求
    /analyse/reqtfea (请求报文的特征)<br>
    /analyse/reqtype (访问类型 - 饼图)<br>
    /analyse/respfea(响应报文的特征)<br>
    /analyse/resptype(载荷类型 - 饼图)<br>
    /analyse/sendhttp(发送http数量)<br>
    /analyse/recehttp(接收http数量)<br>
    /analyse/reqlength(请求报文的帧长)<br>
    /analyse/resplength(响应报文的帧长)<br>
    <br>
    /whois/hostin(请求报文的host兴趣)<br>
    /whois/htmltxt(网页内容性质)<br>
    /whois/domaintype(域名类型)<br>
- 参照任务书2.4节制作页面
- 先期完成文档的序言部分
