# Server-end-Client-example-in-C
This project show the use of socket in c for the creation of an electronic registration platform for exams.

It consists of two server and two clients , server use fork ( ) to create a copy of the parent process to stabilish 
connection with clients who connect , appeals consist of files that are buffered to SERVERS for backup , to change the appeals open CLIENTS files 
copying its contents into a buffer and then rewrite the file with the ip and time of the change. Each teacher can close 
the appeals by setting the file in read-only mode.
