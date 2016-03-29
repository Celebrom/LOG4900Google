#!/usr/bin/python

import sys
import getopt
import os
import subprocess

nIteration = 1
outputdire = os.path.dirname(os.path.realpath(__file__))
chromePath = os.path.join('C:\\', 'Program Files (x86)', 'Google', 'Chrome', 'Application', 'chrome.exe')

def CloseChrome():
    os.system("taskkill /IM chrome.exe /f >nul 2>&1")

def LaunchChrome():
    subprocess.call([chromePath, '--kiosk'])


def main(argv):

   try:
      opts, args = getopt.getopt(argv,"hn:c:o:",["nb_iter=","chromePath=","odire="])
   except getopt.GetoptError:
      print 'OnagerLoop.py -n <number of iteration> -o <output Directory>'
      sys.exit(2)
   for opt, arg in opts:
      if opt == '-h':
         print 'OnagerLoop.py -n <number of iteration> -o <output Directory>'
         sys.exit()
      elif opt in ("-n", "--nb_iter"):
         global nIteration
         nIteration = arg
      elif opt in ("-c", "--chromePath"):
         global chromePath
         chromePath = arg
      elif opt in ("-o", "--odire"):
         global outputdire
         outputdire = arg


if __name__ == "__main__":
   main(sys.argv[1:])
   LaunchChrome()










