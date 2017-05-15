# Remote-Code-Publisher
we will focus on the publication functionality of a Remote Repository. We will develop a remote code publisher, local client, and communication channel that supports client access to the publisher from any internet enabled processor.

The communication channel will use sockets and support an HTTP like message structure. 
The Remote Code Publisher will:

Support publishing web pages that are small wrappers around C++ source code files.
Accept source code text files, sent from a local client.
Support building dependency relationships between code files saved in specific repository folders.
Support HTML file creation for all the files in a specified repository folder1, including linking information that displays dependency relationships, and supports and navigation based on dependency relationships.
Delete stored files, as requested by a local client.
Clients of the Remote Code Publisher will provide a Graphical User Interface (GUI) with means to:

Upload one or more source code text files to the Remote Publisher, specifying a category with which those files are associated1.
Display file categories, based on the directory structure supported by the Repository.
Display all the files in any category.
Display all of the files in any category that have no parents.
Display the web page for any file in that file list by clicking within a GUI control. This implies that the client will download the appropriate webpages, scripts, and style sheets and display, by starting a browser with a file cited on the command line2.
On starting, will download style sheet and JavaScript files from the Repository.

# Some code in this project is from and baesd on Professor Fawcett's code.
