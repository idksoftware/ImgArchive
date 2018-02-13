#import subprocess
#print subprocess.check_output([magick convert DSC01277.JPG - resize 1200x1200 DSC01277_1.JPG])
import os
import sys

os.system('magick convert ' + sys.argv[1] + ' -resize 1200x1200 ' + sys.argv[2])
