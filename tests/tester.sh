# TESTEUR

NOTES :
GET /
sans protocole, marche quand meme (200 OK + index nginx)

GET / h
404 not found

GET / H 
400 Bad Request

GET / HTTP
400 Bad Request

bad header
il ignore

# Test bad requests
# Test plusieurs serveurs

source ../webserv config/test.conf