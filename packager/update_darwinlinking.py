#!/usr/bin/env python

import sys
import glob
import os
import os.path
import fnmatch
import subprocess

def find_files(dir, pattern):
    matches = []
    for root, dirnames, filenames in os.walk(dir):
        for filename in fnmatch.filter(filenames, pattern):
            matches.append(os.path.join(root, filename))
    return matches


def update_deplibs(file):
    # Ignore symbolic links and directories
    if os.path.islink(file) or os.path.isdir(file):
        return

    print("Updating %s..." % file)
    
    p = subprocess.Popen(["otool", "-L", file], stdout=subprocess.PIPE)
    output = p.communicate()[0]
    deplibs = []
    lines = output.split("\t")
    for line in lines[1:]:
        deplibs.append(line.split()[0])
    for deplibOrig in deplibs:
        # Ignore system files
        if deplibOrig.startswith("/usr") or deplibOrig.startswith("/System"):
            continue
        
        deplibName = os.path.split(deplibOrig)[1]
        deplibNew = "@executable_path/../lib/%s" % deplibName
        cmd = "install_name_tool -change %s %s %s" % (deplibOrig, deplibNew, file)
        subprocess.check_call(cmd, shell=True)

    return


def run():
    for bin in glob.glob(os.path.join("bin", "*")):
        update_deplibs(bin)

    dylibs = find_files("lib", "*.dylib")
    for lib in dylibs:
        # Can't change stub files
        if lib.startswith("lib/libgcc_ext"):
            continue

    update_deplibs(lib)
    return


# ------------------------------------------------------------------------------
if __name__ == "__main__":
    run()
