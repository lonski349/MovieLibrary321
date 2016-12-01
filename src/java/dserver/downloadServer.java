package ser321.serialize;

import java.net.*;
import java.io.*;
import java.util.*;


/**
* Copyright 2016 Kaelan Strones,
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* Purpose: Practice Client-Server Programming
* Ser321 Foundations of Distributed Applications
* @author Kaelan Strones kstrones@asu.edu
*         Software Engineering, ASU Poly
* @version August 2016
*/

public class downloadServer extends Thread{

    public static String fileName = "DataServer/";
    private Socket sock;
    private static int id;

    public downloadServer(Socket newSock, int usrID){
	this.sock = newSock;
	this.id = usrID;
    }

    public void run(){
	try {

	    //Get the connections to the sreams
	    OutputStream outStream = sock.getOutputStream();
	    InputStream inStream = sock.getInputStream();

	    //Read the users chosen file name
	    byte clientData[] = new byte[1024];
	    int num = inStream.read(clientData, 0, 1024);
	    String movieName = new String(clientData, 0, num);
	    String fullFileName = fileName + movieName;
	    System.out.println("Downloading: " + fullFileName);

	    //Read in the file that the user wants to watch
	    File outFile = new File(fullFileName);
	    byte[] FileInByte = new byte[(int)outFile.length()];
	    FileInputStream fis = new FileInputStream(outFile);
	    BufferedInputStream bis = new BufferedInputStream(fis);
	    bis.read(FileInByte, 0, FileInByte.length);
	    //Write the file on the outstream to the user
	    outStream.write(FileInByte, 0, FileInByte.length);
	    outStream.flush();
	    System.out.println("The file has been written");
	    //Write Complete, close streams
	    inStream.close();
	    outStream.close();
	    bis.close();
	    sock.close();
	}catch (IOException e){
	    System.out.println("Problem uploading to client");
	    e.printStackTrace();
	}
    }

    public static void main(String [] args){
	id = 0;
	int port = 3030;
	try {
	    if(args.length > 0){
		port = Integer.parseInt(args[0]);
	    }

	    //Start the server
	    ServerSocket server = new ServerSocket(port);

	    //In this loop we will be connecting clients and starting new threads for them.
	    while (true){
		System.out.println("Server waiting for a user to connect");
		Socket sock = server.accept();
		System.out.println("The server has connected to a client with id " + id);
		downloadServer downloadThread = new downloadServer(sock, id);
		//Start the download server on a new thread. the thread will then call the run method
		downloadThread.start();
		id++;
	    }
	}catch(Exception e) {
	    e.printStackTrace();
	}
    }

}
