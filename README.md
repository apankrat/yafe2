This is a re-incarnation of [yet another fractal explorer](http://swapped.cc/yafe) (yafe)
that adds support for multithreaded rendering on multi-core 
boxes as well as support for distributed rendering over the 
network of participating "worker" instances. It is primarily 
meant for exploring Lyapunov fractals, which still remain as 
computationally expensive as they are 
[intriguing looking](http://swapped.cc/content/yafe/img/lyapunov-2048-thumb-bw.png).

Building
-------

Currently builds on Windows (under Visual Studio 2010), but
the code is kept portable with all platform-dependent stuff
isolated to [src/sys](https://github.com/apankrat/yafe2/tree/master/src/sys) directory.

To build on Windows - load build\yafe2.sln and hit F9. 

Make sure to build x64 version for 64-bit boxes as it
gives a hefty speed up over running 32-bit process in
WoW64.

Disclaimer
----------

A weekend pet project. Work very much in progress.

Consider yourselves warned.


