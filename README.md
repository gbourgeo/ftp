# FTP
*This project involves the implementation of a client and a File Transfer Protocol (FTP) server for TCP / IP network file transfer.*


### Create the Server, the Client or both
```sh
$> make server
$> ./server -h
server: -- Server Help

NAME
	server - FTP server

USAGE
	./server [OPTIONS]... PORT

DESCRIPTION
	Launch an FTP server.

	-4	   Server allows IpV4 address only.

	-6	   Server allows IpV6 address only.

	-d	   Every registered users will have his personal directory created. Works only with -u.

	-h, --help
		   Print help and exit.

	-i	   Interactive server. Prints information on STDOUT.

	-p, --path [path]
		   Server working path.

	-u, --user
		   Enables registered users mode.

	port	Port to listen to.
```
```sh
$> make client
$> ./client -h
client: -- Help

NAME
	client - ftp client

USAGE
	./client [OPTIONS]... ADDR PORT

DESCRIPTION
	Start a File Transfert Protocol client.

	-v	    More verbose client.

	-h, --help
		    Print help and exit.

	addr	Address to connect to.


	port	Port to connect to.
```
 Or simply,
```sh
$> make
```

## DESIGN
- A customer specific prompt.
- Can not go down to a level below the server's runtime directory.
- Display of SUCCESS or ERROR + explanation messages after each request.

## COMMANDS
Those commands are recognised by the server:
```
- CDUP		Change to parent directory
- CWD		Change working directory
- DELE		Remove file from server
- HELP		Display available commands
- MKD		Create directory on server
- NLST		Short catalog
- LIST		File listing
- PASS		Give the user password
- PASV		Server enters passive mode
- PORT		Specify Data port to open
- QUIT		Quit the server
- REIN		Reinitialize the user
- RETR		Retreive file from server
- RMD		Remove directory from server
- STOR		Store data
- STOU		Store data in an unique file
- TYPE		Data representation type
- USER		Identify the user
- PWD		Current working directory path
- REGT		Register a new account
```

Those commands are recognised by the client:
```
?			Help of local commands
\\			Start a local command
cd			Change working directory
connect		Connect to a server
clear		Clear Server chat screen
exit		Quit the client
get			Get file from server
help		Help of server commands
ls			List files or directory
mkdir		Create directory
nlst		List files or directory
pass		Sends password to server
put			Put file to server
pwd			Print working directory
quit		Quit the server
refresh		Refresh list Windows
rm			Remove file from server
rmdir		Remove directory from server
```
All commands, except '?' and '\\', are translated by the client to
communicate with the server.

## EXTRA
- RFC 959 compliant
- IPv6 supported
- login/password management
- userspace management

## AUTHOR
- gbourgeo

