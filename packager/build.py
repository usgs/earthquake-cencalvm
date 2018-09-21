#!/usr/bin/env python
#
# ------------------------------------------------------------------------------
# Brad T. Aagaard, U.S. Geological Survey
# ------------------------------------------------------------------------------
#
# Create tarballs with binary package for Linux or Darwin systems.

import os
import shutil
import subprocess
import glob

# ------------------------------------------------------------------------------
def run_cmd(cmd):
    print("Running '{}'...".format(" ".join(cmd)))
    subprocess.check_call(cmd)
    return


# ------------------------------------------------------------------------------
def download(url, file):
    if os.path.isfile(file):
        print("Using local copy of {}".format(file))
    else:
        print("Downloading '{}' from '{}'...".format(file, url))
        cmd = ("curl", "-O", "{}/{}".format(url, file),)
        subprocess.check_call(cmd)
    return

# ------------------------------------------------------------------------------
class BuildConfig(object):

    def __init__(self, src_dir, base_dir, num_threads):
        self.src_dir = os.path.expanduser(src_dir)
        
        base_dir_full = os.path.expanduser(base_dir)
        self.dest_dir = os.path.join(base_dir_full, "dist")
        self.build_dir = os.path.join(base_dir_full, "build")
        self.num_threads = num_threads
        return


# ------------------------------------------------------------------------------
class GccSubpackage(object):

    def __init__(self, name, config):
        self.name = name
        self.version = config.get(name, "version")
        self.suffix = config.get(name, "suffix")
        return

    def download(self, url):
        download(url, self._tarball_filename())
        return

    def unpack(self, dest_dir, src_dir):
        os.chdir(dest_dir)

        src_full = os.path.join(src_dir, self._tarball_filename())
        cmd = ("tar", "-xf", src_full,)
        run_cmd(cmd)

        if not os.path.islink(self.name):
            os.symlink("{name}-{version}".format(name=self.name, version=self.version), self.name)
        return

    def _tarball_filename(self):
        return "{name}-{version}.tar.{suffix}".format(name=self.name, version=self.version, suffix=self.suffix)


# ------------------------------------------------------------------------------
class Gcc(object):
    SUBPKGS = ["mpc", "gmp", "mpfr"]

    def __init__(self, config, build_config):
        self.config = config
        self.build_config = build_config
        return

    def download(self):
        os.chdir(self.build_config.build_dir)
        download(self.config.get("gcc", "url"), self._tarball_filename())
        return

    def unpack(self):
        top_build_dir = self.build_config.build_dir
        os.chdir(top_build_dir)

        cmd = ("tar", "-xf", self._tarball_filename(),)
        run_cmd(cmd)

        for subpkg_name in self.SUBPKGS:
            subpkg = GccSubpackage(subpkg_name, config)
            os.chdir(top_build_dir)
            subpkg.download(self.config.get("gcc", "url"))

            gcc_src_dir = os.path.join(top_build_dir, "gcc-{}".format(self.config.get("gcc", "version")))
            subpkg.unpack(gcc_src_dir, top_build_dir)
        return

    def build(self):
        gcc_build_dir = os.path.join(self.build_config.build_dir, "gcc-build")        
        if not os.path.isdir(gcc_build_dir):
            os.mkdir(gcc_build_dir)
        os.chdir(gcc_build_dir)
        
        version = self.config.get("gcc", "version")
        dest_dir = self.build_config.dest_dir
        cmd = (os.path.join(self.build_config.build_dir, "gcc-{}".format(version), "configure"), "--prefix={}".format(dest_dir), "--disable-multilib", "--enable-languages=c,c++",)
        run_cmd(cmd)

        cmd = ("make", "-j{}".format(self.build_config.num_threads),)
        run_cmd(cmd)
        return

    def install(self):
        gcc_build_dir = os.path.join(self.build_config.build_dir, "gcc-build")
        os.chdir(gcc_build_dir)

        cmd = ("make", "install",)
        run_cmd(cmd)
        return

    def _tarball_filename(self):
        version = self.config.get("gcc", "version")
        suffix = self.config.get("gcc", "suffix")
        return "gcc-{version}.tar.{suffix}".format(version=version, suffix=suffix)


# ------------------------------------------------------------------------------
class Proj(object):

    def __init__(self, config, build_config):
        self.config = config
        self.build_config = build_config
        return

    def download(self):
        os.chdir(self.build_config.build_dir)
        url = self.config.get("proj", "url")
        download(url, self._tarball_filename())
        download(url, self._datum_filename())
        return

    def unpack(self):
        top_build_dir = self.build_config.build_dir
        os.chdir(top_build_dir)

        cmd = ("tar", "-xf", self._tarball_filename(),)
        run_cmd(cmd)
        return

    def build(self):
        proj_build_dir = os.path.join(self.build_config.build_dir, "proj-build")        
        if not os.path.isdir(proj_build_dir):
            os.mkdir(proj_build_dir)
        os.chdir(proj_build_dir)
        
        version = self.config.get("proj", "version")
        dest_dir = self.build_config.dest_dir
        cmd = (os.path.join(self.build_config.build_dir, "proj-{}".format(version), "configure"), "--prefix={}".format(dest_dir), "--with-jni=no",)
        run_cmd(cmd)

        cmd = ("make", "-j{}".format(self.build_config.num_threads),)
        run_cmd(cmd)
        return

    def install(self):
        proj_build_dir = os.path.join(self.build_config.build_dir, "proj-build")
        os.chdir(proj_build_dir)

        cmd = ("make", "install",)
        run_cmd(cmd)

        os.chdir(os.path.join(self.build_config.dest_dir, "share", "proj"))
        cmd = ("unzip", os.path.join(self.build_config.build_dir, self._datum_filename()),)
        run_cmd(cmd)
        return

    def _tarball_filename(self):
        version = self.config.get("proj", "version")
        suffix = self.config.get("proj", "suffix")
        return "proj-{version}.tar.{suffix}".format(version=version, suffix=suffix)

    def _datum_filename(self):
        version = self.config.get("proj", "datum_version")
        return "proj-datumgrid-{version}.zip".format(version=version)


# ------------------------------------------------------------------------------
class Euclid(object):

    def __init__(self, config, build_config):
        self.config = config
        self.build_config = build_config
        return

    def download(self):
        os.chdir(self.build_config.build_dir)
        download(self.config.get("euclid", "url"), self._tarball_filename())
        return

    def unpack(self):
        os.chdir(self.build_config.build_dir)

        cmd = ("tar", "-xf", self._tarball_filename(),)
        run_cmd(cmd)
        return

    def build(self):
        version = self.config.get("euclid", "version")
        euclid_build_dir = os.path.join(self.build_config.build_dir, "euclid{}".format(version), "libsrc")
        os.chdir(euclid_build_dir)

        sysname, hostname, release, version, machine = os.uname()
        if sysname in ["Linux", "Darwin"]:
            shutil.copyfile(os.path.join(self.build_config.src_dir, "makefile_euclid_{}".format(sysname.lower())),
                            os.path.join(euclid_build_dir, "makefile.cencalvm"))

        os.environ["PREFIX"] = self.build_config.dest_dir

        cmd = ("make", "-f", "makefile.cencalvm",)
        run_cmd(cmd)

        return
        
    def install(self):
        version = self.config.get("euclid", "version")
        euclid_build_dir = os.path.join(self.build_config.build_dir, "euclid{}".format(version), "libsrc")
        os.chdir(euclid_build_dir)

        cmd = ("make", "install", "-f", "makefile.cencalvm",)
        run_cmd(cmd)
        return
        
    def _tarball_filename(self):
        version = self.config.get("euclid", "version")
        suffix = self.config.get("euclid", "suffix")
        return "euclid{version}.tar.{suffix}".format(version=version, suffix=suffix)


# ------------------------------------------------------------------------------
class Dependencies(object):

    def __init__(self, config, build_config, gcc=True, euclid=True, proj=True):
        self.build_gcc = gcc
        self.build_euclid = euclid
        self.build_proj = proj
        self.config = config
        self.build_config = build_config
        return

    def build(self):
        if self.build_gcc:
            gcc = Gcc(self.config, self.build_config)
            gcc.download()
            gcc.unpack()
            gcc.build()
            gcc.install()
            
        if self.build_euclid:
            euclid = Euclid(self.config, self.build_config)
            euclid.download()
            euclid.unpack()
            euclid.build()
            euclid.install()

        if self.build_proj:
            proj = Proj(self.config, self.build_config)
            proj.download()
            proj.unpack()
            proj.build()
            proj.install()
        return


# ------------------------------------------------------------------------------
class Cencalvm(object):

    def __init__(self, config, build_config):
        self.config = config
        self.build_config = build_config

        sysname, hostname, release, version, machine = os.uname()
        self.os = sysname
        self.arch = machine
        return

    def configure(self):
        top_src_dir = os.path.join(self.build_config.src_dir, "..")
        dest_dir = self.build_config.dest_dir
        
        # configure
        cppflags = "CPPFLAGS=-I{}".format(os.path.join(dest_dir, "include"))
        ldflags = "LDFLAGS=-L{}".format(os.path.join(dest_dir, "lib"))
        if self.os == "Linux":
            cc = "CC=gcc"
            cxx = "CXX=g++"
        elif self.os == "Darwin":
            cc = "CC=clang"
            cxx = "CXX=clang++"
        else:
            raise ValueError("Unknown os '%s'." % self.os)

        cencalvm_build_dir = os.path.join(self.build_config.build_dir, "cencalvm-build")
        if not os.path.isdir(cencalvm_build_dir):
            os.mkdir(cencalvm_build_dir)
        os.chdir(cencalvm_build_dir)
        cmd = (os.path.join(top_src_dir, "configure"), "--prefix={}".format(dest_dir),
               cppflags, ldflags, cc, cxx,)
        run_cmd(cmd)
        return

    def build(self):
        cencalvm_build_dir = os.path.join(self.build_config.build_dir, "cencalvm-build")
        os.chdir(cencalvm_build_dir)
        
        cmd = ("make", "-j{}".format(self.build_config.num_threads),)
        run_cmd(cmd)
        return

    def install(self):
        cencalvm_build_dir = os.path.join(self.build_config.build_dir, "cencalvm-build")
        os.chdir(cencalvm_build_dir)
        
        cmd = ("make", "install",)
        run_cmd(cmd)
        return
    
# ------------------------------------------------------------------------------
class BinaryApp(object):

    def __init__(self, base_dir, num_threads, config):
        self.build_config = BuildConfig(os.path.abspath(os.path.split(__file__)[0]), base_dir, num_threads)
        self.config = config
        
        sysname, hostname, release, version, machine = os.uname()
        self.os = sysname
        self.arch = machine

        self._set_environ()
        return


    def setup(self):
        dest_dir = self.build_config.dest_dir
        print("Cleaning destination directory '%s'..." % dest_dir)
        if os.path.isdir(dest_dir):
            shutil.rmtree(dest_dir)
        os.makedirs(dest_dir)
        for add_dir in ["lib", "include"]:
            fullpath = os.path.join(dest_dir, add_dir)
            if not os.path.isdir(fullpath):
                os.makedirs(fullpath)

        build_dir = self.build_config.build_dir
        print("Cleaning build directory '%s'..." % build_dir)
        if os.path.isdir(build_dir):
            shutil.rmtree(build_dir)
        os.makedirs(build_dir)
        return


    def build_dependencies(self):
        if self.os == "Linux":
            dependencies = Dependencies(self.config, self.build_config, gcc=True)
        elif self.os == "Darwin":
            dependencies = Dependencies(self.config, self.build_config, gcc=False)
        else:
            raise ValueError("Unknown os '%s'." % self.os)
        dependencies.build()
        return


    def build_cencalvm(self):
        cencalvm = Cencalvm(self.config, self.build_config)
        cencalvm.configure()
        cencalvm.build()
        cencalvm.install()
        return


    def package(self):
        if self.os == "Darwin":
            filename = "setup_darwin.sh"
            libsuffix = ".dylib"
        elif self.os == "Linux":
            filename = "setup_linux.sh"
            libsuffix = ".so"
        else:
            raise ValueError("Unknown os '%s'." % self.os)

        shutil.copyfile(os.path.join(self.build_config.src_dir, filename), os.path.join(self.build_config.dest_dir, "setup.sh"))

        os.chdir(os.path.join(self.build_config.build_dir, "cencalvm-build"))
        from distutils.sysconfig import parse_makefile
        makefile = parse_makefile("Makefile")
        package = makefile["PACKAGE"]
        version = makefile["VERSION"]
                 
        os.chdir(self.build_config.dest_dir)

        if self.os == "Darwin":
            self._update_darwinlinking()

        # Strip symbols from binaries and libraries
        strip = ("strip", "-x",) if self.os == "Darwin" else ("strip",)
        strip_list = glob.glob("bin/*")
        libs = glob.glob("lib/lib*")
        libs += glob.glob("lib64/lib*")
        libs += [file for file in glob.glob("libexec/gcc/*/*/cc1*")]
        libs += [file for file in glob.glob("libexec/gcc/*/*/lto1*")]
        libs += [file for file in glob.glob("libexec/gcc/*/*/lt-wrapper*")]
        for lib in libs:
            if libsuffix in lib and not os.path.islink(lib) and not lib.endswith("_s.so") and not lib.endswith(".py"):
                strip_list.append(lib)
        cmd = strip + tuple(strip_list)
        run_cmd(cmd)
            
        orig_name = os.path.split(self.build_config.dest_dir)[1]
        base_name = "{}-{}".format(package, version)
        os.chdir("..")
        os.rename(orig_name, base_name)
            
        tarball = os.path.join(self.build_config.src_dir, "{package}-{version}.tgz".format(package=package, version=version))
        cmd = ("tar", "-zcf", tarball,
                   "--exclude={}/lib/*.a".format(base_name),
                   "--exclude={}/lib/*.la".format(base_name),
                   base_name,
                   )
        run_cmd(cmd)

        os.rename(base_name, orig_name)
        return


    def _set_environ(self):
        path = (os.path.join(self.build_config.dest_dir, "bin"),
                os.path.join(os.environ["HOME"], "bin"), # utilities for building (e.g., updated version of git)
                "/bin",
                "/usr/bin",
                "/sbin",
                "/usr/sbin",
        )
        os.environ["PATH"] = ":".join(path)

        if self.os == "Linux":
            ldpath = (os.path.join(self.build_config.dest_dir, "lib"),)
            if self.arch == "x86_64":
                ldpath += (os.path.join(self.build_config.dest_dir, "lib64"),)
            os.environ["LD_LIBRARY_PATH"] = ":".join(ldpath)
        return


    def _update_darwinlinking(self):
        os.chdir(self.build_config.dest_dir)

        import update_darwinlinking
        update_darwinlinking.run()
        return


# ======================================================================
if __name__ == "__main__":
    import argparse

    base_dir_default = os.path.join(os.environ["HOME"], "cencalvm-binary")

    parser = argparse.ArgumentParser()
    parser.add_argument("--config", action="store", dest="config", default="build.cfg")
    parser.add_argument("--setup", action="store_true", dest="setup")
    parser.add_argument("--build-dependencies", action="store_true", dest="build_dependencies")
    parser.add_argument("--build-cencalvm", action="store_true", dest="build_cencalvm")
    parser.add_argument("--package", action="store_true", dest="package")
    parser.add_argument("--all", action="store_true", dest="all")
    parser.add_argument("--base-dir", action="store", dest="base_dir", default=base_dir_default)
    parser.add_argument("--make-threads", action="store", dest="make_threads", type=int, default=8)
    args = parser.parse_args()

    import ConfigParser
    config = ConfigParser.SafeConfigParser()
    config.read(args.config)
    app = BinaryApp(args.base_dir, args.make_threads, config)

    if args.setup or args.all:
        app.setup()

    if args.build_dependencies or args.all:
        app.build_dependencies()
        
    if args.build_cencalvm or args.all:
        app.build_cencalvm()
        
    if args.package or args.all:
        app.package()
        
    
# End of file


