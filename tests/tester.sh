#!/usr/bin/env bash

webserv_path="../"
webserv_bin="webserv"
webserv_exec="${webserv_path}/${webserv_bin}"

EOC="\e[0m"
BOLD="\e[1m"
RED="\e[91m"
GREEN="\e[92m"

echo -e "\n--------------- TESTEUR JEANCERV 1.0 ----------------\n"
echo -e "Before continuing, please make sure you are running the server as followed :
	./webserv config/test.conf\n"
echo "Enter to continue (or q quit)"
read inputuser
if [ "$inputuser" == "q" ]; then
	exit ;
fi

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
	echo "----------------- TESTING :" $4 >> logs
	cat requests/$3/$4.req >> logs
	echo -e "\n--" >> logs
	cat tmp_res >> logs
	echo -e "\n\n-- EXPECTED :" >> logs
	echo "$(head -n1 requests/$3/$4.resp)" >> logs
	echo -e "\n" >> logs
	STATUS_OUR="$(head -n1 tmp_res)"
	STATUS_TRUE="$(head -n1 requests/$3/$4.resp)"
	if [ "$STATUS_OUR" == "$STATUS_TRUE" ]; then
		printf "%-2s${GREEN}✅${EOC} $4\n"
		echo -e "--------------------------- OK !! \n\n" >> logs
	else
		printf "%-2s${RED}X${EOC} $4\n"
		echo -e "--------------------------- WRONG !! \n\n" >> logs
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

rm logs

test_bad_config

tests "bad_requests"

chmod 000 ../files/jeancerveur_files/no_right.txt
tests "errors"

chmod 010 ../files/jeancerveur_files/write_only.txt
tests "get_method"


chmod 000 ../files/jeancerveur_files/uploaded/no_right.txt
if test -f ../files/jeancerveur_files/uploaded/no_exist.txt; then
	rm ../files/jeancerveur_files/uploaded/no_exist.txt
fi
if test -f ../files/jeancerveur_files/uploaded/chunk.txt; then
	rm ../files/jeancerveur_files/uploaded/chunk.txt
fi
tests "post_method"


YOLO=../files/jeancerveur_files/to_delete/yolo
NO_RIGHT=../files/jeancerveur_files/to_delete/no_right_delete
chmod 000 $NO_RIGHT
tests "delete_method"
if test -f "$YOLO"; then
	printf "%-2s${RED}X${EOC} yolo NOT deleted\n"
else
	printf "%-4s${GREEN}->${EOC} yolo correctly deleted\n"
fi
if test -f "$NO_RIGHT"; then
	printf "%-4s${GREEN}->${EOC} no_right not deleted\n"
else
	printf "%-2s${RED}X${EOC} no_right deleted\n"
fi
touch $YOLO

tests "other"


# --------- Tests manuels

# multiple server
SERVER1="$(nc localhost 8080 < requests/misc/server1.req)"
SERVER2="$(nc localhost 9000 < requests/misc/server2.req)"
if [ "$SERVER1" != "$SERVER2" ]; then
	printf "%-2s${GREEN}✅${EOC} multiple server\n"
else
	printf "%-2s${RED}X${EOC} multiple server\n"
fi

# error page
DEFAULT="$(nc localhost 8080 < requests/misc/error_page_default.req)"
CUSTOM="$(nc localhost 9000 < requests/misc/error_page_custom.req)"
if [ "$CUSTOM" != "$DEFAULT" ]; then
	printf "%-2s${GREEN}✅${EOC} custom error_page\n"
else
	printf "%-2s${RED}X${EOC} custom error_page\n"
fi

#client body
curl -s http://localhost:8080/blog/post.php -d "ahaahaaaaa" -o tmp_res
STATUS_OUR="$(sed -n 3p tmp_res)"
STATUS_TRUE="$(sed -n 3p requests/misc/client_body.resp)"
if [ "$STATUS_OUR" == "$STATUS_TRUE" ]; then
		printf "%-2s${GREEN}✅${EOC} client max body size\n"
else
	printf "%-2s${RED}X${EOC} client max body size\n"
fi

# hostname
curl -s --resolve webserv:9000:127.0.0.1 http://webserv:9000 -o tmp_res
if [ "$(cat tmp_res)" == "$(cat requests/misc/hostname.resp)" ]; then
	printf "%-2s${GREEN}✅${EOC} hostname\n"
else
	printf "%-2s${RED}X${EOC} hostname\n"
fi

chmod 777 $NO_RIGHT
chmod 777 ../files/jeancerveur_files/uploaded/no_right.txt
chmod 777 ../files/jeancerveur_files/write_only.txt
chmod 777 ../files/jeancerveur_files/no_right.txt
rm tmp_res


echo -e "\nReady to start the siege ?\n"
echo "Enter to continue (or q quit)"
read inputuser
if [ "$inputuser" == "q" ]; then
	exit ;
fi
siege http://localhost:8080
