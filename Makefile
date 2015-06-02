all: nanofont3x4 dump_bmp

nanofont3x4: nanofont3x4.cpp
	g++ $< -o $@ -Wall -Wextra

dump_bmp: dump_bmp.cpp
	g++ $< -o $@ -Wall -Wextra

