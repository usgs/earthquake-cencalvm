cencalvm=`pwd`

if test ! -f bin/cencalvmquery; then
    echo
    echo "*** Error! ***"
    echo
    echo "Source this script from the top-level cencalvm directory:"
    echo
    echo "    cd [directory containing 'setup.sh']"
    echo "    source setup.sh"
    echo 
else
    export PATH="$cencalvm/bin:/bin:/usr/bin:/sbin/:/usr/sbin:$PATH"
    echo "Ready to run cencalvmquery."
fi
