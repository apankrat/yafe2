This is a re-incarnation of 
[yet another fractal explorer](http://swapped.cc/yafe) project
(yafe) 
that adds support for multithreaded rendering on multi-core 
boxes as well as support for distributed rendering over the 
network of participating "worker" instances. It is primarily 
meant for exploring Lyapunov fractals, which still remain as 
computationally expensive as they are 
[intriguing looking](http://swapped.cc/content/yafe/img/lyapunov-2048-thumb-bw.png).

Work in progress
----------------

... *very much* in progress.

It doesn't have the UI, nor can it save the rendering result 
into a viewable file, like a PNG, nor does it actually map 
the `double` matrix of computed fractal values to the RGBA 
space of a bitmap. Network rendering part is not done yet
either. It's all coming up.

What's done is the arrangement for rendering fractals in a
way that allows for transparently parallelizing the process - 
be it by running multiple rendering threads or by offloading
the rendering to slave instances.

Building
--------

Currently builds on Windows (under Visual Studio 2010), but
the code is kept portable with all platform-dependent stuff
isolated to [src/sys](https://github.com/apankrat/yafe2/tree/master/src/sys) directory.

To build on Windows - load `build\yafe2.sln` and hit F7. 

Make sure to build x64 version for 64-bit boxes as it gives 
a hefty speed up over running a 32-bit version under WoW64.

Disclaimer
----------

A weekend pet project. Consider yourselves warned.


