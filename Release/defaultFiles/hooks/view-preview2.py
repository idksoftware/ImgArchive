#import subprocess
#print subprocess.check_output([magick convert DSC01277.JPG - resize 650x650 DSC01277_1.JPG])
import os
import sys

os.system('magick convert ' + sys.argv[1] + ' -resize 650x650 ' + sys.argv[2])
