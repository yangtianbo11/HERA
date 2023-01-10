COURSES=/home/courses
CFLAGS =-I ${COURSES}/lib/HERA-C -w -g

Test: Test-Tiger-stdlib-stack
	./Test-Tiger-stdlib-stack

Test-Tiger-stdlib-stack: Test-Tiger-stdlib-stack.cc Tiger-stdlib-stack-data.hera Tiger-stdlib-stack.hera
	g++ ${CFLAGS}     Test-Tiger-stdlib-stack.cc   -o Test-Tiger-stdlib-stack
