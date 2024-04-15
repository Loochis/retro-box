from PIL import Image

im = Image.open("images/digits/3.png")
f = open("output.txt", "w")

f.write("static const unsigned char TEMP[] = {\n\t0b")

pix = im.load()
ctr = -1
nlctr = 0
for y in range(5):
    for x in range(3):
        ctr += 1
        if ctr == 8:
            ctr = 0
            nlctr += 1
            if nlctr == 12:
                nlctr = 0
                f.write(",\n\t0b")
            else:
                f.write(", 0b")

        if pix[x, y][0] > 100:
            f.write("1")
        else:
            f.write("0")

f.write("\n};")

f.close()
