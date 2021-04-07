# Missing Computer Science Semester
## Command Line Environment
### Jobs
The UNIX environment uses a *signal* to communicate with processes. Signals are software interrupts which should stop the execution of a process such that it can deal with what the signal says.

SIGINT and SIGQUIT ask the program to quit
SIGSTOP pauses execution, to be resumed with fg or bg
SIGHUP ends the process, sent on terminal closed
SIGKILL always terminates

The jobs command lists all unfinished jobs associated with this terminal session.

### Managing Dotfiles
In your own folder, under version control, symlinked into place using a script.

### SSH
keys: your private key (like a password) is at ~/.ssh/id\_rsa, you give your public key to the machine

## Version Control
### Git
In git terminology, a file is a blob and a directory is a tree. A snapshot is the top level tree.

In git, the history of a project is a directed acyclic graph.

## Debugging
### Logs
Once a program is big enough, move from printf debugging to a log file. It should be colour coded with ANSI escape codes. Logs from other programs should usually be under /var/log.

### Specialised programs
Profilers can help locate unoptimised spots in code

## Metaprogramming
### Make
Use it more often, it's good for small projects
