#!c:\python26
import fnmatch, os, string

files = os.listdir(os.curdir)

for filename in fnmatch.filter(files, "*.dat"):
	#fn = string.split(filename, '.')
	print("CytusRip " + filename)
	os.system("CytusRip " + filename)
