Import("env")
import os
import shutil


def removeFiles(source, target, env):
	print("*** Cleaning Folder *** ")
	preserved_files = ["assets", "MksWifi.bin", "Robin_nano35.bin"]

	build_path = os.path.join(env.Dictionary("PROJECT_BUILD_DIR"), env.Dictionary("PIOENV"))
	files = os.listdir(build_path)
	print(files)

	for el in files:
		print("checking " + el )
		fullpath = os.path.join(build_path, el)
		if os.path.exists(fullpath) and not(el in preserved_files):
			if os.path.isdir(fullpath):
				shutil.rmtree(fullpath)
				print("	-> folder removed")
			else:
				os.remove(fullpath)
				print("	-> file removed")

env.AddPostAction("buildprog", removeFiles)
