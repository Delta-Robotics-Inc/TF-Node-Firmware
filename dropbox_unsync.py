#!/usr/bin/env python3

"""
  This script, if run within a PlatformIO project directory, will mark .vscode
  and .pio/build directories as "ignored" so that Dropbox will not sync them
  (they will be local only).
  
  This works by setting extended file attributes on those directories that the
  Dropbox client will see.
  
  See: https://help.dropbox.com/sync/ignored-files
  
  If you store your entire PlatformIO directory in Dropbox, then save this file
  there (next to the Projects directory), and add this to your environment in
  platformio.ini:
    extra_scripts = post:../../dropbox_unsync.py
  
  With that, when the .pio/build directory is re-created, this script will run
  immediately afterward, just before anything is put into that directory, so
  Dropbox will never try to sync it.
"""

import sys
import os
import platform
import subprocess


vscode_dir = os.path.abspath( "./.vscode" )
build_dir  = os.path.abspath( "./.pio/build" )

if not os.path.isfile( "./platformio.ini" ):
	print( "Did not see a platformio.ini file in the current directory -- is this a PlatformIO project?" )
	sys.exit()

if not os.path.isdir( vscode_dir ):
	os.makedirs( vscode_dir )

if not os.path.isdir( build_dir ):
	os.makedirs( build_dir )

osType = platform.system()

if osType == "Darwin":
	# These are for older versions of Dropbox or MacOS 12.3 and earlier
	subprocess.run( [ "xattr", "-w", "com.dropbox.ignored", "1", build_dir ] )
	subprocess.run( [ "xattr", "-w", "com.dropbox.ignored", "1", vscode_dir ] )

	# These are for newer versions of Dropbox on MacOS 12.5 and later
	subprocess.run( [ "xattr", "-w", "com.apple.fileprovider.ignore#P", "1", build_dir ] )
	subprocess.run( [ "xattr", "-w", "com.apple.fileprovider.ignore#P", "1", vscode_dir ] )

elif osType == "Windows":
	subprocess.Popen( [ "powershell.exe", "Set-Content", "-Path", f"'{build_dir}'", "-Stream", "com.dropbox.ignored", "-Value", "1" ] )
	subprocess.Popen( [ "powershell.exe", "Set-Content", "-Path", f"'{vscode_dir}'", "-Stream", "com.dropbox.ignored", "-Value", "1" ] )

elif osType == "Linux":
	subprocess.run( [ "attr", "-s", "com.dropbox.ignored", "-V", "1", build_dir ] )
	subprocess.run( [ "attr", "-s", "com.dropbox.ignored", "-V", "1", vscode_dir ] )

print( "Done" )