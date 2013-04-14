# glxtension
Build Status: [![Build Status](https://secure.travis-ci.org/kwk/glxtension.png?branch=master)](http://travis-ci.org/kwk/glxtension)

**glextension** let's you see what your current OpenGL setup is capable of in terms of OpenGL Version, Renderer, Vendor and Extensions. Especially the latter is pretty important and that's where the main focus of this application lies on. It presents sort of a tree-list of a extensions grouped by corporation or institution that are currently available on your system:

![Filter the extension tree][filter_extension_tree]

Apart from that you can read the extension's specification either by double-clicking on the extension name in the tree-list or by using the auto-completion combo-box on the *Specifications* tab.

![Read the specification][read_extension_spec]

# License

glxtension - Show OpenGL information
Copyright (C) 2013 Konrad Kleine

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Building from source

Get the build dependencies (Ubuntu/Debian way):

    sudo apt-get install libqt4-opengl-dev qt4-qmake libqtgui4 libqtwebkit-dev

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
