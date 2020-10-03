#!/usr/bin/env python
# -*- coding: utf-8 -*-

from PIL import Image
from struct import pack
import os.path
import sys


# TFT define
header = 1
picwidth = 0
picheight = 0


# TFT32 splash
# picwidth = 320
# picheight = 240

# TFT32 little
# picwidth = 32
# picheight = 36

# TFT32 small
# picwidth = 60
# picheight = 50

# TFT32 normal
# picwidth = 78
# picheight = 104


# TFT35 splash
# picwidth = 480
# picheight = 320

# TFT35 preview
# picwidth = 200
# picheight = 200

# TFT35 back
# picwidth = 70
# picheight = 40

# TFT35 arrow
# picwidth = 10
# picheight = 16

# TFT35 line
# picwidth = 82
# picheight = 1

# TFT35 blank
# picwidth = 70
# picheight = 28

# TFT35 operate
# picwidth = 150
# picheight = 80

# TFT35 navigate
# picwidth = 117
# picheight = 92

# TFT35 small
# picwidth = 45
# picheight = 45

# TFT35 normal
# picwidth = 117
# picheight = 140


source = os.getcwd() + '/assets'
target = os.getcwd() + '/bitmap'

if picwidth == 0 or picheight == 0:
	print('you must define valid picwidth and picheight')
	sys.exit()

for filename in os.listdir(source):
	if filename.endswith(".bin"): 
		print('processing ' + filename)

		pngfile = os.path.join(target, os.path.splitext(filename)[0]) + '.png'
		image = Image.new('RGB', (picwidth, picheight))

		binfile = os.path.join(source, filename)
		bindata = open(binfile, 'rb')
		content = bindata.read()

		x = 0
		y = 0

		for i in range(0, len(content), 2):
			two = "%x" % ord(content[i])
			one = "%x" % ord(content[i + 1])
			rg = bin(int(one, 16)).replace('0b', '')
			gb = bin(int(two, 16)).replace('0b', '')
			while len(rg) < 8:
				rg = '0' + rg
			while len(gb) < 8:
				gb = '0' + gb
			r = int(rg[0:5] + '000', 2)
			g = int(rg[5:8] + gb[0:3] + '00', 2)
			b = int(gb[3:8] + '000', 2)
			image.putpixel((x, y), (r, g, b))
			x = x+1
			if x >= picwidth:
				y = y+1
				x = 0
			if y >= picheight:
				break
		image.save(pngfile, 'png')
		pngdata = Image.open(pngfile)

		print('width: ', pngdata.width)
		print('height: ', pngdata.height)

print('png files saved to images folder')
