#!/bin/bash

mkdir -p pic

nanofont3x4 -u -0 nanofont3x4.cpp
mv output_upper3x4.bmp pic/output_nanofont3x4_upper_0.bmp

nanofont3x4 -u -1 nanofont3x4.cpp
mv output_upper3x4.bmp pic/output_nanofont3x4_upper_1.bmp

nanofont3x4 -u -2 nanofont3x4.cpp
mv output_upper3x4.bmp pic/output_nanofont3x4_upper_2.bmp

nanofont3x4       nanofont3x4.cpp
mv output_lower3x4.bmp pic/output_nanofont3x4_lower.bmp

mv nanofont3x.4bmp pic/nanofont3x4.bmp

nanofont3x4 -u
mv output_upper3x4.bmp pic/output_declaration_upper3x4.bmp

nanofont3x4
mv output_lower3x4.bmp pic/output_declaration_lower3x4.bmp
