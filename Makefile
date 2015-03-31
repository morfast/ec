ec: ec.c
	gcc -O0 -g ec.c -o ec

test: ec test.py
	./ec
	./test.py
