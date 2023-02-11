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

#### 环境说明

*建议使用visual studio开发，clion的远程开发不成熟*

虚拟机中将教师提供的libpcap-1.10.1.tar.gz解压后configure、make、make install，并解决报错中的问题；vs打开项目后连接虚拟机测试编译

- package filter
  - sniffer.cpp 实现数据捕获 （2.11 update：完成wifi帧头定义，需要分别测试，确定帧头长度、内容的正确性，选择最合适的工作模式）
- 报文解析
  - parse.cpp 实现HTTP载荷解析
  - storage.cpp 实现将数据包写入数据库和文件

802.11帧头长度参考资料

| 功能    | To DS字段 | From DS字段 | Address1 | Address2 | Address3 | Address4 |
| ------- | --------- | ----------- | -------- | -------- | -------- | -------- |
| IBSS    | 0         | 0           | DA/RA    | SA/TA    | BSSID    | 未用     |
| TP AP   | 1         | 0           | BSSID/RA | SA/TA    | DA       | 未用     |
| From AP | 0         | 1           | DA/RA    | BSSID/TA | SA       | 未用     |
| WDS     | 1         | 1           | BSSID/RA | BSSID/TA | DA       | SA       |




----

## 第二部分：Web系统后端

- 数据库设计
- api实现

----

## 第三部分：前端页面，文档撰写

- 选用vue或react
- 参照api doc编写，对api提出需求
- 参照任务书2.4节制作页面
- 先期完成文档的序言部分
