import os, shutil

try:
    shutil.rmtree("output")
    shutil.rmtree("outputImage")
except FileNotFoundError:
    os.mkdir("output")
    os.mkdir("outputImage")
try:
    os.mkdir("output")
    os.mkdir("outputImage")
except FileExistsError:
    pass