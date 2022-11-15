# 🖥️ webserv
![image](https://user-images.githubusercontent.com/55249229/201527151-9e127078-d960-4f3f-b116-e41d24980f41.png)
<p align="center"><i>Fig.1 - Great illustration of how a server works by <a href="https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa">Screw Everything</a> </i></p>    
     
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
    
📌 [Subject](https://github.com/jgonfroy42/webserv/blob/master/webserv_subject.pdf) (July 2021 version)    
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
The program starts by reading the configuration file (or the default one if none is provided) that will determine how the server is gonna work. In a `.conf` file, much like in a Nginx configuration file, you will find `server` blocks containing different declarations such as `listen`, `location`, `error_page` etc.   
Here's the default configuration file as a simple server example.
```
server {
	listen localhost:8080;
	server_name Jeancerver1.0;

	error_page 404 ./error_pages/404_error.html;
	error_page 400 ./error_pages/400_error.html;
	error_page 403 ./error_pages/403_error.html;

    root /files/jeancerveur_files;
	index index.html;

	location /nope {
		redirect 301 http://0.0.0.0:8080/blog/;
	}

	location /blog {
		methods GET POST;
		index blog.html;
		cgi_extension .php;
		cgi_path ./files/cgi/php-cgi;
	}
	
	location /uploaded {
		methods POST GET;
		upload ./files/jeancerveur_files/uploaded;
		autoindex on;
	}
}
```
Once lauched, the server will listen to external connexions, waiting for a client. Once a client reaches the server, its request is processed and the adequate response is sent.     
<p align="center"> <img alt="Simple animation of a client sending a request and the server answering with a 404 page not found error" src="server_explained_asset.gif" /> </p>      

## 📖 Ressources
To start tackling the beautiful challenge of making an HTTP server, we higly recommend reading [Screw Everything](https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa)'s tutorial which covers about everything you need to know.  
