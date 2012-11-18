# glxtension
Build Status: [![Build Status](https://secure.travis-ci.org/kwk/glxtension.png?branch=master)](http://travis-ci.org/kwk/glxtension)

**glextension** let's you see what your current OpenGL setup is capable of in terms of OpenGL Version, Renderer, Vendor and Extensions. Especially the latter is pretty important and that's where the main focus of this application lies on. It presents sort of a tree-list of a extensions grouped by corporation or institution that are currently available on your system:

![Filter the extension tree][filter_extension_tree]

Apart from that you can read the extension's specification either by double-clicking on the extension name in the tree-list or by using the auto-completion combo-box on the *Specifications* tab.

![Read the specification][read_extension_spec]

# Building from source

Check out the source from Github

    git clone https://github.com/kwk/glxtension.git

Create a shadow-build directory and navigate to it

    mkdir glxtension/build
    cd glxtension/build

Create Makefiles

    qmake ..

Run Make

    make

Run the application

    ./glxtension

[filter_extension_tree]: https://raw.github.com/kwk/glxtension/master/resources/screenshots/filter_extension_tree.png "Filter the extension tree"
[read_extension_spec]: https://raw.github.com/kwk/glxtension/master/resources/screenshots/read_extension_spec.png "Read the extension specification"
