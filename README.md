# OSS-Logging-Library

# IMPORTANT NOTES

- The driver program will generate 1 of 4 posible messages each time the program runs
  if the program generates a FATAL message, it will write out to savelog and exit (as requested). 

  Quick Note: If you see the message: "Fatal Message Passed. Writing log to 'savelog' and exiting."
  it is not an error (although you likely already know that, just covering all bases). That is the
  message my logger prints when a Log of type "FATAL" has been passed to let the user know why execution
  is terminating and where the log is stored. 

# Logger Library and Driver: Overview

The LoggerLibrary is a class that contains functions and a struct and vector queue
that allows for easy logging of error, informatonal, fatal, and warning messages.
The Driver program is a driver to test the functionality of the LoggerLibrary library.
To invoke the driver program, you call it with optional command line switches as follows:

./driver [-h] [-t sec] [logfile]


## Getting Started (Compiling)

To compile both the driver program and the Logger library, simple use of 'make'
will compile both for your use. 'make clean' will remove the created object files and the
executable binary that was compiled using the object files

### Information

The program will send messages that are generated using Lorem Ipsum. It will attach a value of either
'warning', 'info', 'error', or 'fatal' to the generated message and push it for addition to the log. 
If a fatal message is received, the program will cease execution and write out to 'savelog'.

Upon execution, the program will send 5 messages at complete random (INLUDING THE POSSIBILITY OF FATAL). 
If a fatal is thrown, the logger will follow proper instruction and write to savelog and cease execution. 
