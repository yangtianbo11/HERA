#include <iostream>

extern std::ostream &trace;
extern std::ostream &debug;

/* streams.h and streams.cpp are meant to help us control output streams for a program running on Linux,
   to reduce confusion when we're trying to run our standard tests with your program and understand the
   output we're seeing. To help us test, please use the following four streams appropriately:

     cout << "something that a user of your program would need to see if using your program correctly"
     cerr << "an error message meant for the user, if they've given incorrect input"
     debug << "variable values or other information about something that's going wrong in your code"
     trace << "status information about the program that helps you keep track of which step it's in"

   C-Lion (and the cs245-cmake script) will normally place your compiled program in the folder cmake-build-debug,
   so, after building it, you should be able to run it on the command-line by typing

      cmake-build-debug/myProgram

   if the compiled program is called myProgram. Usually the program has the same name as the C-Lion project, but
   C-Lion sometimes turns characters into "_" or removes them, so if you have a project called Dictionary-C++,
   you might find that the compiled program is called Dictionary_C__ or Dictionary_C. You can use the command
      ls cmake-build-debug
   (or your file browser) to look at what's available in that folder.

   Once you've confirmed that you can run your command on the command-line, you can play with the output.

   If you're running the usual linux command-interpreter shell (known as "bash"), you can separately control
   the standard output (cout) and standard error (cerr) by using the ">" symbol. There are many variants of
   this, but the basic usage can create a file (or _replace_ the contents of an existing file) with the
   text you've sent to cout and/or cerr. To "redirect" cout in this way, you put ">" after the command name,
   and to redirect cerr, you put "2>"; either of these is followed by a file name into which to put the data:

      cmake-build-debug/myProgram > myOutput.txt 2> myErrors.txt # creates files myOutput.txt and myErrors.txt

   If you omit one of the above redirects, that stream will still go where it would have gone, i.e., to the
   terminal in which you ran the program. If you want to just throw away the stream, use "/dev/null" as the
   file name.

   The above instructions should work for any standard linux program, whether or not for CS245. Some of the
   CS245 projects let you further categorize parts of your output as "trace" or "debug" information, and you
   can direct it separately; these streams aren't part of the linux/bash standard, so you can't use ">", but
   we have made use of a different mechanism, described below.

   First, note that the default for both trace and debug is to send to cerr, so error, trace, and debug
   text would _all_ go into myErrors.txt in the example above. If you want to send the four results to four
   different files "out.txt", "err.txt", "trace.txt", "debug.txt", you would write this:

      TRACE_OUTPUT=trace.txt DEBUG_OUTPUT=debug.txt cmake-build-debug/myProgram > out.txt 2> err.txt

   Some important/useful details:

   * For now, don't try to send multiple streams to the same file; they'll just overwrite each other.
     There are basic tools to do this, which sometimes result in weird interleaving of things sent to
     different streams (you may have noticed that C-Lion seems to be using this for cout and cerr),
     and advanced ways that don't do output quite as efficiently but preserve the relative ordering
     of lines (or, if you really don't care about efficiency, individual characters) sent to different
     streams, but those are well beyond the scope of this course; I think streambuf::setbuf is probably
     part of it, start with https://gcc.gnu.org/onlinedocs/libstdc++/manual/streambufs.html if you care).

   * If, for some reason, you want to send errors into a file but see, e.g., DEBUG on the terminal,
     use the file name "/dev/tty" for debug, e.g.

      DEBUG_OUTPUT=/dev/tty cmake-build-debug/myProgram > out.txt 2> errAndTrace.txt


   Some or all of these instructions will probably work on MacOS (especially if you're running bash),
   and perhaps even on MS Windows (if running PowerShell), but they have not been tested there.
 */
