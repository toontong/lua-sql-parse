if [ "$1" = '-f' ]; then 
	rm CMakeFiles/ -fr
	rm CMakeCache.txt -f 
	rm Makefile  -f
	rm cmake_install.cmake  -f
	cmake .
fi

make
lua test_parse.lua

