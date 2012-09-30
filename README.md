fgrab
=====

A very small NAOqi module to grab and saves raw YUV422 frames from
NAO's main camera.

It was made as a programming exercise for NAOqi 1.12.5.

Assuming you have a NAO v4 running 1.12.5, and all your SDK,
toolchains and whatnot set up as Aldebaran's doc tells you, then, you
can check out this project, place it somewhere in your worktree, cd to
it, and run the following commands to configure, build, and deploy it
to your NAO:

    qibuild configure -c <atom> # replacing <atom> by the name of your
                                # cross-compilation toolchain
    qibuild make -c <atom>
    qibuild deploy -c nao-atom nao@192.168.1.11:naoqi
                                # replacing 192.168.1.11 by the
                                # address of your NAO, of course


Now, ssh to your NAO. On NAO, add the following to ~/naoqi/preferences/autoload.ini, under
the [user] section:

    /home/nao/naoqi/lib/naoqi/libfgrab.so

restart NAOqi:

    nao restart

grab a frame:

    ./naoqi/bin/call_fgrab

It is at ~/raw_frame.yuv. You can copy it to your computer and use,
for instance, pyuv to look at it: 1280x960, YUV 4:2:2, interleaved.

You can check the log in /var/log/naoqi.log. To have a look at the
verbose log, you can stop naoqi

    nao stop

and start it with "verbose" output:

    naoqi -v

## MIT License

Copyright (c) 2012 Alexandre Gravier

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
