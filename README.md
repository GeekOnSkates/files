# Files

## Overview

This is a text-mode file manager.  Don't get me wrong:
I think everyone should get to know ls, and rm, and mv,
and rmdir, and the bazillion other commands and their
parameters, if they're serious about learning Linux.
But that's a lot to ask of newbies.  Sorry, RTFM crowd
(lol).

This app was (is being) built with accessibility as a
pretty high priority.  Even if you know all those cool
file commands, the truth is Orca (the screen reader in
most distros) just reads all the data in one big rant.
There's no navigation, no time to hear what files there
are or their properties.  This file manager comes with
built-in text-to-speech and plenty of keyboard shortcuts
so users who are blind can use it like anyone else.

But it's not JUST for users who are blind.  I'm giving
it a TUI (text user interface) and eventually, hopefully,
mouse support.  This may be a lofty goal, but I want this
to become as common for managing files as nano or vi are
for writing text files.  This is still very much a work
in progress, so that's far from a reality.  But as the
great Wayne Gretzky once said, you miss 100% of the shots
you don't take. :)

## Building

This program has two dependencies:
1. **ncurses:** This is the TUI library
2. **eSpeak:** This is the text-to-speech engine

To install these on Debian-based systems, run:

```
	sudo apt-get update
	sudo apt-get upgrade
	sudo apt-get install libncurses-dev
	sudo apt-get install libespeak-dev
```

To make things a little easier, I've included a
shell script (build.sh).  I'm still learning the
GNU build system (make and CMake and all that)
but for now this gets the job done.  I've also
included a pre-compiled binary for x86 devices
(desktops and laptops), Once version 1.0 is done,
I'll be creating a development branch for future
work, and the master branch will always include
this.  Eventually, I'd like to put it on whatever
public repo apt-get uses, include a man page and
all that - but I have no idea how to do that, so
again, this will do for now. :)
