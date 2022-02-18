import os
import sys
import struct


filename = sys.argv[1]
f = open(filename, "rb")
name=filename.split(".pcm")[0]
out = "#include <stdint.h> \n"
out += "// 16bit, mono, 16khz \n"
out += "uint16_t " + name +"[]={"
while (b := f.read(2)):
    w = struct.unpack("H", b)[0]
    out += "{0:#0{1}x}".format(w,6)
    out += ","

out = out[:-1]
out += "};"

print(out)

