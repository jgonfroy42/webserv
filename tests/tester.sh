#!/usr/bin/env bash

webserv_path="../"
webserv_bin="webserv"
webserv_exec="${webserv_path}/${webserv_bin}"

EOC="\e[0m"
BOLD="\e[1m"
RED="\e[91m"
GREEN="\e[92m"

# ./$webserv_exec ../config/test.conf &>logs

function test_bad_config() {
	printf "TESTING BAD CONFIGS\n"
	for f in ../config/bad_conf/*.conf; do
		FILENAME=${f##*/}
		TEST=${FILENAME%.*}
		RESP=$(./$webserv_exec $f 2>&1)
		EXPECT=$(cat bad_conf/$TEST.resp)
		if [ "$RESP" == "$EXPECT" ]; then
			printf "%-2s${GREEN}✅${EOC} $TEST\n"
		else
			echo "NOT OK"
		fi
	done
}

function test_request() {
	nc $1 $2 < requests/$3/$4.req > tmp_res
	STATUS_OUR="$(head -n1 tmp_res)"
	STATUS_TRUE="$(head -n1 requests/$3/$4.resp)"
	if [ "$STATUS_OUR" == "$STATUS_TRUE" ]; then
		printf "%-2s${GREEN}✅${EOC} $4\n"
	else
		echo "NOT OK"
	fi
}

function tests() {
	printf "\nTESTING ${1^^}\n"
	for f in requests/$1/*.req; do
		FILENAME=${f##*/}
		TEST=${FILENAME%.*}
		test_request "localhost" "8080" $1 $TEST
	done
}

test_bad_config
tests "bad_requests"
tests "get_method"

rm tmp_res

# siege -v http://localhost:8080
