package ser321.movie;

import javax.swing.*;
import java.io.*;
import javax.sound.sampled.*;
import java.beans.*;
import java.net.*;
import javax.swing.tree.*;
import javax.swing.event.*;
import javax.swing.text.html.*;
import javax.swing.filechooser.*;
import java.awt.event.*;
import java.awt.*;
import java.util.*;
import java.lang.Runtime;
import org.json.JSONObject;
import org.json.JSONArray;
import ser321.movieServer.movieDescription;
import ser321.movieServer.movieLibrary;
import ser321.movieServer.MovieHttpProxy;


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

//public class movieLibraryClient extends MovieLibraryGui implements
//							    TreeWillExpandListener,
//							    ActionListener,
//							    TreeSelectionListener{

public class movieLibraryClient{
    
    private static final boolean debugOn = true;
    private boolean stopPlaying;

    /*
    public movieLibraryClient(String author){
	super(author);    //super is calling the constructor of the super class..
	                  //in this case MovieLibraryGui
	stopPlaying = false;
	
	plotJTA.setText("You have not selected a movie yet!");

	actorsJCB.addActionListener(this);    //Referencing the current object
	actorsJCB.setActionCommand("NewActor");
	actorsClearButton.addActionListener(this);
	actorsClearButton.setActionCommand("ClearActors");

	genreJCB.addActionListener(this);
	genreJCB.setActionCommand("NewGenre");
	genreClearButton.addActionListener(this);
	genreClearButton.setActionCommand("ClearGenre");

	//Because there are multiple menus and menu items we are going to use
	//a nested for loop to initilize all of the menu items.
	for(int i = 0; i < userMenuItems.length; i++){
	    for(int j = 0; j < userMenuItems[i].length; j++){
		userMenuItems[i][j].addActionListener(this);
	    }
	}

	//Try to add the tree listener to this class and then rebuild the tree.
	//The rebuild tree method can be found below
	try{
	    tree.addTreeSelectionListener(this);
	    rebuildTree();
	}catch (Exception ex){
	    JOptionPane.showMessageDialog(this, "Handling " + " Constructor exception: "
					  + ex.getMessage());
	}

	setVisible(true);
    }

    private void debug (String message){
	if (debugOn){
	    System.out.println("debug: " + message);
	}
    }
    */

    public static void main(String args[]){

	String host = "localhost";
	String port = "8080";
	
	try{
	    String suthorName = "Kaelan's Library";
	    if(args.length >= 2){
		host = args[0];
		port = args[1];
	    }
	    String url = "http://" + host + ":" + port + "/";
	    System.out.println("The URL is ..\n" + url);
	    movieLibrary ml = (movieLibrary)new MovieHttpProxy(new URL(url));
	    BufferedReader stdin = new BufferedReader(new InputStreamReader(System.in));
	    System.out.println("Enter getTitles, then get 'title', then remove 'title', then getTitles: ");
	    String inStr = stdin.readLine();
	    StringTokenizer st = new StringTokenizer(inStr);
	    String opn = st.nextToken();
	    while (!opn.equalsIgnoreCase("end")){
		if(opn.equalsIgnoreCase("getTitles")){
		    String [] result = ml.getTitles();
		    System.out.println("Here are the titles: \n");
		    for (int i = 0; i < result.length; i++){
			System.out.println(result[i] + ", ");
		    }
		}
		else if (opn.equalsIgnoreCase("get")){
		    String title = st.nextToken();
		    while (st.hasMoreTokens()){
			if(st.hasMoreTokens()) title = title + " ";
			title = title + st.nextToken();
		    }
		    movieDescription result = ml.get(title);
		    System.out.println("got: " + result.toString());
		}
		else if (opn.equalsIgnoreCase("remove")){
		    String title = st.nextToken();
		    while (st.hasMoreTokens()){
			if(st.hasMoreTokens()) title = title + " ";
			title = title + st.nextToken();
		    }
		    boolean result = ml.remove(title);
		    System.out.println("remove " + title + " result " + result);
		}

		inStr = stdin.readLine();
		st = new StringTokenizer(inStr);
		opn = st.nextToken();
	    }
	}catch (Exception e){
	    e.printStackTrace();
	    System.out.println("Something went wrong!");
	}
	    //System.out.println("calling constructor name " + authorName);
    }    

    

}
