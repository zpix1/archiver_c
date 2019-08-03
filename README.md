# C Huffman archiver

This is a very simple command line tool to compress and decompress files.

It uses Huffman's coding algorithm written in clean C.

## File structure
1. main.c
2. coder.c/h provides Huffman's algorithm implementation
3. priority_queue.c/h provides data structure (priority queue based on linked list)

## How to use
Just compile the project using `CMakeLists.txt` or by linking it manually.

Compress:
`./archiver_c c file.txt file.txt.zip`

Extract:
`./archiver_c d file.txt.zip file1.txt`

## Performance
Test results:
```
Testing simple1.txt; C=0:00.00; E=0:00.00; 16.5; (6->99) simple1.txt match
Testing simple2.txt; C=0:00.05; E=0:00.03; 0.156081; (4041991->630876) simple2.txt match
Testing 100MB.bin; C=0:00.68; E=0:00.41; 3.14713e-07; (104857600->33) 100MB.bin match
Testing image.png; C=0:00.18; E=0:00.31; 0.999415; (3966244->3963924) image.png match
Testing bigimage.jpg; C=0:00.32; E=0:00.56; 1.00087; (7214303->7220602) bigimage.jpg match
```

As you can see, Huffman's coding can't compress already compressed images, but it can reduce the size of a text file in 6 times (simple2.txt).

Also, there is no point to use it to compress very small files, when headers are bigger than the original file (simple1.txt).