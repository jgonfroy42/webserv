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
		printf "%-2s${RED}X${EOC} $4\n"
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

tests "errors"

tests "get_method"

tests "post_method"

tests "delete_method"
YOLO=../files/jeancerveur_files/to_delete/yolo
NO_RIGHT=../files/jeancerveur_files/to_delete/no_right
if test -f "$YOLO"; then
	printf "%-2s${RED}X${EOC} yolo NOT deleted\n"
else
	printf "%-4s${GREEN}->${EOC} yolo correctly deleted\n"
fi
if test -f "$NO_RIGHT"; then
	printf "%-2s${RED}X${EOC} no_right NOT deleted\n"
else
	printf "%-4s${GREEN}->${EOC} no_right correctly deleted\n"
fi
touch $YOLO
touch $NO_RIGHT
chmod 000 $NO_RIGHT

rm tmp_res

# siege -v http://localhost:8080
