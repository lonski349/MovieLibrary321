Author: Kaelan Strones
Date: 11/30/2016
Purpose: Practice client server programs in both java and c++

There are many ways to compile this program and run it. The easist way that compiles all
of the necessiscary items to run the c++ server is ant build.server. For building the
client use ant build.client. 
For compiling the java portion use ant build.java.client. This dependencies are setup in
such a way that it compiles all of the needed files for running the client. Building the
client also compiles the download server. The download server can also be compiles with 
ant build.java.dserver.

For the movie files replace the movies.json file with the desired movies. Make sure that 
all of the movies in the movies.json file are available in the DataServer folder and the 
file names match those that are in the movies.json file. If these file names do not match 
you may recieve an error while trying to download a file from the server.