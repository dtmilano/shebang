shebang
=======

> From the Jargon Dictionary:
> http://info.astrian.net/jargon/terms/s/shebang.html
> 
> shebang     /sh*-bang/ n.
> The character sequence "#!" that frequently begins executable shell scripts
> under Unix. Probably derived from "shell bang" under the influence of American
> slang "the whole shebang" (everything, the works).

This is the resurrection of an old project (circa 2002).

**Shebang** extends the standard shebang line functionality found on Linux, Mac OSX and Windows Cygwin allowing for use cases that would
be otherwise impossible.

Introduction
------------
Let's start with a brief introduction about what the standard **shebang** really is.

In Unix-like operating systems, when a script containing the two specific characters **#!** as the first 2 of the file
being executed, then the rest of the line is intrpreted as the _interpreter directive_ and the original script file path
is passed to this _interpreter_ as a command line argument.
For this to happen, the original script requires the **execute** attribute be set.

### Examples ###
One of the most common uses is to run shell scripts.

```bash
#! /bin/bash
echo "Hello World!"
```

This may allow you to execute the scripts with a different shell than your login shell.

You may wonder why somebody would like to do something like this.
After all, is there any other shell than bash?

Certainly, there are, even though bash is almost everywhere. If you are interested in some historical facts related with
Unix shells taka a look at
[superuser: why is bash everywhere...](http://superuser.com/questions/61727/why-is-bash-everywhere-in-most-if-not-all-linux-distributions).

To cut this story short, in ancient times, you may have had `csh` as your login shell because of features like history
and job control but you still wrote your scripts in `sh`. In such cases, the **shebang** line came very handy.

There are other cases indeed, not related to login shell, for example to execute scripts that are programmed in a different language than the shell

```awk
#!/usr/bin/awk -f
/localhost/ {print $1}
```

or

```python
#! /usr/bin/env python
print "Hello World!"
```

Limitations
-----------
As we mentioned before, once the _interpreter directive_ is honored then the _interpreter_ is invoked with the original
script file path as an argument.

But, what if the _interpreter_ does not support **`#`** as a comment?
Well, if the _interpreter_ does not support it as a comments you will receive a **syntax error**.

Another limitation is that the **shebang** line is passed verbatim to the _interpreter_, no variable expansion of any
kind is performed. In this respect **shebang**, this ustility, helps by providing some basic variable expansion feature.

Regarding arguments provided in _interpreter_ command lines is not well defined how they would be treated.
Different Unix-like systems treat them in different ways and sometimes the _interpreter_ is invoked with a single argument 
concatenating all the arguments provided in the original shebang command line (see [Results](http://www.in-ulm.de/~mascheck/various/shebang/#results)).

This **shebang** utility also tries to unify shebang and command line arguments treatment across different platforms.

Extensions
----------

permits that programs that accept scripts but don't allow '#' as a
comment, be used with shebang (#!) interpreter method of execution.
This is because the entire script is passed to the interpreter untouched,
including the first (shebang) line. If '#' is not understood as a comment,
probably you get a 'syntax error' from the interpreter.


Now you can write scripts that will be executed by those programs with no
conflict.


Resources
=========
* http://en.wikipedia.org/wiki/Shebang_(Unix)
* http://www.in-ulm.de/~mascheck/various/shebang/
* http://superuser.com/questions/61727/why-is-bash-everywhere-in-most-if-not-all-linux-distributions
* http://stackoverflow.com/questions/12910744/why-should-the-shebang-line-always-be-the-first-line
