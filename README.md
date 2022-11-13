# 🖥️ webserv
![image](https://user-images.githubusercontent.com/55249229/201527151-9e127078-d960-4f3f-b116-e41d24980f41.png)
A [42 school](https://www.42.fr/) project where the goal is to write an HTTP server in C++98 compliant with the RFC 7230 to 7235 (HTTP 1.1). Similar to Nginx's behaviour, the program lets you configure a server that handles the following headers :    
<table border="0" align="center">
 <tr>
    <td>Accept-Charsets</td>
    <td>Content-Location</td>
    <td>Referer</td>
 </tr>
 <tr>
    <td>Accept-Language</td>
    <td>Content-Type</td>
    <td>Retry-After</td>
 </tr>
 <tr>
    <td>Allow</td>
    <td>Date</td>
    <td>Server</td>
 </tr>
 <tr>
    <td>Authorization</td>
    <td>Host</td>
    <td>Transfer-Encoding</td>
 </tr>
 <tr>
    <td>Content-Language</td>
    <td>Last-Modified</td>
    <td>User-Agent</td>
 </tr>
  <tr>
    <td>Content-Length</td>
    <td>Location</td>
    <td>WWW-Authenticate</td>
 </tr>
</table>
    
📌 [Subject](https://github.com/jgonfroy42/webserv) (July 2021 version)    
By [amartin-menadier](https://github.com/amartin-menadier), [jgonfroy42](https://github.com/jgonfroy42) and [celeloup](https://github.com/celeloup).

## 🛠️ Usage
```shell
# Use our Makefile to compile
make

# Launch the server
./minishell [config_file]

# Launch our tests
./test/tester.sh
```
Note that if no configuration file is provided, `config/default.conf` will be used as the default configuration for the server.

## How it works
[WIP]  
## 📖 Ressources
[WIP]   
