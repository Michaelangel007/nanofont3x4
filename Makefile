all: nanofont3x4 dump_bmp 4x4

nanofont3x4: nanofont3x4.cpp
	g++ $< -o $@ -Wall -Wextra

dump_bmp: dump_bmp.cpp
	g++ $< -o $@ -Wall -Wextra

4x4: all_4x4_bitmaps.cpp
	g++ $< -o $@ -Wall -Wextra

