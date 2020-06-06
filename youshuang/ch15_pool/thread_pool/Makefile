all: web_server

web_server: web_server.cpp http_conn.cpp http_conn.h threadpool.h locker.h
	g++ -o web_server web_server.cpp http_conn.cpp http_conn.h threadpool.h locker.h -lpthread

clean:
	rm web_server
