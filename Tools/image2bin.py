#!/usr/bin/env python
# -*- coding: utf-8 -*-

from PIL import Image
from struct import pack
import binascii
import os.path
import sys


source = os.getcwd() + '/bitmap'
target = os.getcwd() + '/assets'

for filename in os.listdir(source):
	if filename.endswith(".png"): 
		print('processing ' + filename)

		pngfile = os.path.join(source, filename)
		pngdata = Image.open(pngfile)
		binfile = os.path.join(target, os.path.splitext(filename)[0]) + '.bin'
		bindata = open(binfile, 'wb')

		picwidth = pngdata.width
		print('width: ', picwidth)
		picheight = pngdata.height
		print('height: ', picheight)

		pixels = pngdata.load()

		if picwidth == 480 and picheight == 320:
			bindata.write(binascii.unhexlify('04800728'))
		elif picwidth == 117 and picheight == 140:
			bindata.write(binascii.unhexlify('04d48111'))

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

		bindata.write(pack('B', int('10')))  # endbyte 0A
		bindata.close()

print('bin files saved to assets folder')
