Summary
------

This is a re-incarnation of [yet another fractal explorer](http://swapped.cc/yafe) (yafe)
that adds support for multithreaded rendering on multi-core 
boxes as well as support for distributed rendering over the 
network of participating "worker" instances. It is primarily 
meant for exploring Lyapunov fractals, which still remain as 
computationally expensive as [intriguing looking](http://swapped.cc/content/yafe/img/lyapunov-2048-thumb-bw.png).

Supported systems
------

Currently builds on Windows (under Visual Studio 2010), but
the code is kept portable with all platform-dependent stuff
isolated to [src/sys](https://github.com/apankrat/yafe2/tree/master/src/sys) directory.

WARNING
------

A weekend pet project. Work very much in progress.


