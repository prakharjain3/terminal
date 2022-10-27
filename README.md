# OSN Assignment 2
OSN Assignment 2 made by Prakhar Jain (2022121008)   

<!-- ## Commands -->
### Instructions to Run
- Inside `2022121008_Assignment2 folder` , run `make` and then ,`./shell` to start the shell

### Prompt
- the shell starts with the prompt `<USERNAME@HOSTNAME: CURRENT_WORKING_DIRECTORY>`
### Builtin Commands
- `cd` - changes the directoy . cd acommand  also supports `'.', '..', '-', '~'` . works on both `relative` and `absolute path`

- `pwd` - prints the `current working directory`

- `echo` - prints the strings as it is after handling tabs and spaces

- `ls` - lists all the information of a file or files inside a directory also suppports -a (prints hidden files) flag  and -l (prints information in long  format) flag 
- `exit or quit` - these commands will kill the terminal
 
### Proesses
#### All other commands are treated as system commands
- `background` - commands ending with '&' are treated as background processes. After exiting the process name along with the pid is printed with status normally or abnormally.

- `foreground` - all other commands are treated as foreground commands. The time interval for which the command executes is also printed. 


### pinfo
- syntax -  `pinfo <pid>` pid is optional

- If `nothing` is given as argument information about  `all child processes is given`.

- If `pid` is given as argument then the information about the given process  is printed.

### discover
- Discover command searches for a file or a directory.

- `-d` - searches for all the directories.
- `-f` - Searches for all the files.

### history
- History command prints the last 10 commands entered in the shell.
