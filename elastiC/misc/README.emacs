		      elastiC mode for (X)Emacs
		      =========================

elastiC now has a major mode for Emacs and XEmacs.
It is still experimental and incomplete, but it seems to work quite
well. It has been put together by reusing pieces of the objc, java and
pike modes.

INSTALLATION
------------

You have to add `elastic.el' in a suitable place searched by the emacs
load path. I personally keep it inside the subdirectory $HOME/emacs,
and I have the following lines inside my .emacs:

(setq load-path (cons "/home/panta/emacs" load-path))
(require 'elastic)

NOTE
----

I have tried the elastiC mode only with XEmacs, but it should work
also with classical Emacs.
Please report any problem to panta@elasticworld.org

Patches and suggestions are welcome, as usual.

Enjoy,
Marco Pantaleoni
31 March 2001
