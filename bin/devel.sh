# ----------------------------------------------------------------------------
# None of the lists below can have white space or a dollar sign in an entry.
#
# The person of company that owns the copyright for this package
# (if empty then no copyright for this package).
copyright_owner='Bradley M. Bell'
#
# Web address linked by run_omhelp.sh to the Home icon on each page.
# (if empty, no home icon appears in pages generated by run_omhelp.sh).
image_link='https://coin-or.github.io/CppAD'
#
# List of files, besides CMakeLists.txt, that have a copy of the
# version number (can be empty).
version_files='
    omh/cppad.omh
    configure.ac
'
#
# List of special files, for this repository, that the devel tools ignore.
# The files /.gitignore, /batch_edit.sed, /bin/devel.sh should be in this list.
# Files that are created by a program, and checked into the repository,
# should also be in this list. Names that end with / are directories to ignore.
# Names that begin with a / are relative to top source directory.
# All other names are relative paths somewhere below the top source directory.
ignore_files='
    /.gitignore
    /.coin-or/projDesc.xml

    /ar-lib
    /authors
    /batch_edit.sed
    /bin/devel.sh
    /bin/get_highlight.sh
    /bin/get_omhelp.sh
    /bin/run_omhelp.sh
    /cmake/cppad_uninstall.cmake
    /coin.png
    /COPYING
    /compile
    /configure
    /config.guess
    /config.sub
    /configure
    /depcomp
    /install-sh
    /missing
    makefile.in
'
# ----------------------------------------------------------------------------
echo "copyright_owner=$copyright_owner"
echo "image_link=$version_files"
echo "ignore_files=$ignore_files"
echo "version_files=$version_files"
# ----------------------------------------------------------------------------
