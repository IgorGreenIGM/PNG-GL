<h2>📚 Resume</h2>

This is a little plugin written in C, 
Which can read OpenGL Drawing Buffer and store the result into the PNG format.

<h2>🛠️ Features</h2>
 
This plugin just create a really basic PNG output file : 
- IDAT CHUNK
- IHDR CHUNK
- IDAT CHUNK 
- pHYS CHUNK

<h2>⚙️ Building</h2>

Makefile and Windows compiling files are provided, just copy src and includes files in your project folder.<br>
This project use zlib, so to avoid using dll is to directly include zlib source into your project. 
