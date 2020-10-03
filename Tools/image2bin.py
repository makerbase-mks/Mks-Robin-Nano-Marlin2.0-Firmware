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


source = os.getcwd() + '/bitmap'
target = os.getcwd() + '/assets'

for filename in os.listdir(source):
	if filename.endswith(".png"): 
		print('processing ' + filename)

		pngfile = os.path.join(source, filename)
		pngdata = Image.open(pngfile)
		binfile = os.path.join(target, os.path.splitext(filename)[0]) + '.bin'
		bindata = open(binfile, 'wb')

		print('width: ', pngdata.width)
		print('height: ', pngdata.height)

		pixels = pngdata.load()

		if header == 1:
			bindata.write(pack('B', int('04')))  # descriptor 04
			bindata.write(pack('B', int('212'))) # descriptor d4
			bindata.write(pack('B', int('129'))) # descriptor 81
			bindata.write(pack('B', int('17')))  # descriptor 11

		for y in range(pngdata.size[1]):
			for x in range(pngdata.size[0]):
				R = pixels[x, y][0] >> 3
				G = pixels[x, y][1] >> 2
				B = pixels[x, y][2] >> 3
				rgb = (R << 11) | (G << 5) | B
				strHex = "%x" % rgb
				if len(strHex) == 3:
					strHex = '0' + strHex[0:3]
				elif len(strHex) == 2:
					strHex = '00' + strHex[0:2]
				elif len(strHex) == 1:
					strHex = '000' + strHex[0:1]
				if strHex[2:4] != '':
					bindata.write(pack('B', int(strHex[2:4], 16)))
				if strHex[0:2] != '':
					bindata.write(pack('B', int(strHex[0:2], 16)))
		bindata.close()

print('bin files saved to assets folder')
