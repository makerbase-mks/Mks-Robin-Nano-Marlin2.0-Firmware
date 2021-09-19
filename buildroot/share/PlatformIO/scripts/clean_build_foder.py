Import("env")
import os
import shutil
import time


def removeFiles(source, target, env):
	time.sleep(0.5)
	print("*** Cleaning Folder *** ")
	preserved_files = ["assets", "MksWifi.bin", "Robin_nano35.bin"]

	build_path = os.path.join(env.Dictionary("PROJECT_BUILD_DIR"), env.Dictionary("PIOENV"))
	files = os.listdir(build_path)

	for el in files:
		fullpath = os.path.join(build_path, el)
		if os.path.exists(fullpath) and not(el in preserved_files):
			if os.path.isdir(fullpath):
				shutil.rmtree(fullpath)
				print(el + "	-> folder removed")
			else:
				os.remove(fullpath)
				print(el + "	-> file removed")
		else:
			print(el + " OK")

env.AddPostAction("buildprog", removeFiles)
