#!/usr/bin/env bash

webserv_path="../"
webserv_bin="webserv"
webserv_exec="${webserv_path}/${webserv_bin}"

# ./$webserv_exec ../config/test.conf &>logs

for f in ../config/bad_conf/*.conf; do echo "$f"; done

# TEST=$(nc localhost 8080 < requests/simple_get)

# echo $TEST