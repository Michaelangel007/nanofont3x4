# nanofont3x4
The world's smallest readable 3x4 font.  Includes:

* Upper case (3x3) plus 1 pixel leading,
* Lower case (2x2!)
* All ASCII symbols

A "practical" example would be rendering "book pages" with real text instead of placeholder blurry pixels that don't even look close to being the glyphs scaled down.

Why attempt to do the "impossible" of creating lowercase letters in a 2x2 cell ?  That's only 16 choices for 26 lowercase letters!

Partially for the challenge, but mostly because only by pushing a craft to its maximum limits does it really force oneself to take a step back and really analyze what the goal is.  A readable glyph.  Since there are no "extraneous" pixels to "fallback" onto, every pixel becomes that much more important.  Even a 1 pixel mistakes really stands out.


* Texture Atlas:

<img src="https://raw.githubusercontent.com/Michaelangel007/nanofont3x4/master/nanofont3x4.bmp">


* Forced upper case output: `nanofont -u`

<img src="https://raw.githubusercontent.com/Michaelangel007/nanofont3x4/master/output_declaration_upper3x4.bmp"> 

* Normal case output:

<img src="https://raw.githubusercontent.com/Michaelangel007/nanofont3x4/master/output_declaration_lower3x4.bmp">


* Upper case on its own source code with default 0 px leading:
   `nanofont3x4 -u nanofont3x4.cpp`

<img src="https://raw.githubusercontent.com/Michaelangel007/nanofont3x4/master/output_upper3x4_0.bmp"> 

* Forced upper case outwith with 1 px leading: `nanofont -u -1`

<img src="https://raw.githubusercontent.com/Michaelangel007/nanofont3x4/master/output_upper3x4_1.bmp"> 

* Forced upper case outwith with 2 px leading: `nanofont -u -2`

<img src="https://raw.githubusercontent.com/Michaelangel007/nanofont3x4/master/output_upper3x4_2.bmp"> 

* Fake Bold

The reason for the funny dimensions is so that the resolution maps 1:1 on the iPhone 5.

<img src="https://raw.githubusercontent.com/Michaelangel007/nanofont3x4/master/output_bold_sources_ken.png">

# Uber 4x4 Texture Atlas All Permutations 

In case you are interested, there are a total of 65,536 4x4 monochrome glyphs. Here is a uber texture atlas that shows all of them with our glyphs highlighted (red) where they are in the table.  Blue borders are used to show the cell boundaries.

<img src="https://raw.githubusercontent.com/Michaelangel007/nanofont3x4/master/1289x1290_8bit_textureatlas.bmp">
