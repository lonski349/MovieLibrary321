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
import ser321.serialize.downloadServer;


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

public class movieLibraryClient extends MovieLibraryGui implements
							    TreeWillExpandListener,
							    ActionListener,
							    TreeSelectionListener{

    private static movieLibrary ml;
    private static final boolean debugOn = true;
    private boolean stopPlaying;

    private static String host;
    private static String port;
    private static String portDL;
    
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

    public void buildInitialTree(DefaultMutableTreeNode root, String base){
	try{
	    root.setUserObject(base);
	}catch (Exception ex){
	    JOptionPane.showMessageDialog(this, "exception in initial tree: " + ex);
	    ex.printStackTrace();
	}
    }

    public void rebuildTree(){
	try{
	    
	    String[] videoList = ml.getTitles();
	    String[] Genres = new String[videoList.length];
	    //Get the first Genre from each of the movies.
	    //Future implementation could include getting the full list and adding the movie into
	    //every genre it is in. 
	    for (int i = 0; i < videoList.length; i++){
		movieDescription temp = ml.get(videoList[i]);
		Vector<String> tempGenres = temp.getGenre();
		Genres[i] = tempGenres.get(0);
	    }
	    tree.removeTreeSelectionListener(this);
	    DefaultTreeModel model = (DefaultTreeModel)tree.getModel();
	    DefaultMutableTreeNode root = (DefaultMutableTreeNode)model.getRoot();
	    clearTree(root, model);
	    DefaultMutableTreeNode videoNode = new DefaultMutableTreeNode("Video");
	    model.insertNodeInto(videoNode, root, model.getChildCount(root));
	    //Put the nodes into the tree for all the videos
	    for (int i = 0; i < videoList.length; i++){
		String aTitle = videoList[i];
		String aGenre = Genres[i];
		DefaultMutableTreeNode toAdd = new DefaultMutableTreeNode(aTitle);
		DefaultMutableTreeNode subNode = getSubLabelled(videoNode, aGenre);
		if(subNode != null){
		    model.insertNodeInto(toAdd, subNode, model.getChildCount(subNode));
		}
		else{
		    DefaultMutableTreeNode anAlbumNode = new DefaultMutableTreeNode(aGenre);
		    model.insertNodeInto(anAlbumNode, videoNode, model.getChildCount(videoNode));
		    DefaultMutableTreeNode aSubCatNode = new DefaultMutableTreeNode("aSubCat");
		    model.insertNodeInto(toAdd, anAlbumNode, model.getChildCount(anAlbumNode));
		}
	    }
	    //Expand all of the nodes in the tree
	    for(int i = 0; i < tree.getRowCount(); i++){
		tree.expandRow(i);
	    }
	    tree.addTreeSelectionListener(this);
	}catch (Exception ex){
	    JOptionPane.showMessageDialog(this, "exception in tree Refresh: " + ex);
	    ex.printStackTrace();
	}
    }

    private void clearTree(DefaultMutableTreeNode root, DefaultTreeModel model){
	try{
	    DefaultMutableTreeNode next = null;
	    int subs = model.getChildCount(root);
	    for(int i = subs - 1; i >= 0; i --){
		next = (DefaultMutableTreeNode)model.getChild(root, i);
		debug("removing node labelled: " + (String)next.getUserObject());
		model.removeNodeFromParent(next);
	    }
	}catch (Exception ex) {
	    System.out.println("Exception while trying to clear tree: ");
	    ex.printStackTrace();
	}
    }

    private DefaultMutableTreeNode getSubLabelled(DefaultMutableTreeNode root, String label){
	DefaultMutableTreeNode ret = null;
	DefaultMutableTreeNode next = null;
	boolean found = false;
	for(Enumeration e = root.children(); e.hasMoreElements();){
	    next = (DefaultMutableTreeNode)e.nextElement();
	    debug("sub with label: " + (String)next.getUserObject());
	    if (((String)next.getUserObject()).equals(label)){
		debug("found sub with label: " + label);
		found = true;
		break;
	    }
	}

	if (found)
	    ret = next;
	else
	    ret = null;
	return ret;
    }

    public void treeWillCollapse(TreeExpansionEvent tee) {
	debug("In treeWillCollapse with path: " + tee.getPath());
	tree.setSelectionPath(tee.getPath());
    }

    public void treeWillExpand(TreeExpansionEvent tee) {
	debug("In treeWillExpand with path: " + tee.getPath());
    }

    //Update the text boxes for the new movie that was selected by the user
    public void valueChanged(TreeSelectionEvent e) {
	try{
	    tree.removeTreeSelectionListener(this);
	    DefaultMutableTreeNode node = (DefaultMutableTreeNode)
		tree.getLastSelectedPathComponent();
	    if(node!=null){
		String nodeLabel = (String)node.getUserObject();
		debug("In valueChanged. Selected node labelled: "+nodeLabel);
		// Check if we have selected an final node or one of the folders of the tree
		if(node.getChildCount()==0 &&
		   (node != (DefaultMutableTreeNode)tree.getModel().getRoot())){

		    //The node selected is a movie so set all of the text fields for that movie
		    movieDescription temp = ml.get(nodeLabel);
		    
		    plotJTA.setText(temp.getPlot());
		    titleJTF.setText(temp.getTitle());
		    ratedJTF.setText(temp.getRating());
		    releasedJTF.setText(temp.getRelease());
		    runtimeJTF.setText(temp.getRuntime());
		    fileNameJTF.setText(temp.getFilename());

		    genreJCB.removeActionListener(this);
		    Vector<String> genres = temp.getGenre();
		    genreJCB.removeAllItems();

		    //Add the new genres to the list
		    for(int i = 0; i < genres.size(); i++){
			genreJCB.addItem(genres.get(i));
		    }

		    genreJCB.addActionListener(this);

		    actorsJCB.removeActionListener(this);
		    Vector<String> actors = temp.getActors();
		    actorsJCB.removeAllItems();

		    //Add the new actors to the list
		    for(int i = 0; i < actors.size(); i++){
			actorsJCB.addItem(actors.get(i));
		    }

		    actorsJCB.addActionListener(this);
		    
		}else{
		    plotJTA.setText("You selected: " + nodeLabel);

		    //Remove all of the actors and genres in the lists
		    genreJCB.removeActionListener(this);
		    genreJCB.removeAllItems();
		    genreJCB.addActionListener(this);
		    actorsJCB.removeActionListener(this);
		    actorsJCB.removeAllItems();
		    actorsJCB.addActionListener(this);

		    //clear text fields
		    titleJTF.setText("");
		    ratedJTF.setText("");
		    releasedJTF.setText("");
		    runtimeJTF.setText("");
		    fileNameJTF.setText("");
		}
	    }
	}catch (Exception ex){
	    ex.printStackTrace();
	}
	tree.addTreeSelectionListener(this);
    }

       // a method to determine whether a string is already in the combo box
    private boolean contains(JComboBox jcb, String text){
        boolean ret = false;
        for (int i=0; i< jcb.getItemCount(); i++){
            if (((String)jcb.getItemAt(i)).equals(text)){
                ret = true;
	    }
	}
	return ret;
    }

    //Contains all of the programming for any of the buttons that are pressed
    public void actionPerformed(ActionEvent e) {
	try{
	    tree.removeTreeSelectionListener(this);
	    if(e.getActionCommand().equals("Exit")){
		System.exit(0);
	    }
	    else if (e.getActionCommand().equals("NewActor")){
		debug("new actor selected " + (String)actorsJCB.getSelectedItem());
		if(!contains(actorsJCB, (String)actorsJCB.getSelectedItem())){
		    actorsJCB.addItem((String)actorsJCB.getSelectedItem());
		}
		releasedJTF.setText((String)actorsJCB.getSelectedItem());
	    }
	    else if(e.getActionCommand().equals("ClearActors")){
		actorsJCB.removeActionListener(this);
		actorsJCB.removeAllItems();
		actorsJCB.addActionListener(this);
	    }
	    else if(e.getActionCommand().equals("ClearGenre")){
		genreJCB.removeActionListener(this);
		genreJCB.removeAllItems();
		genreJCB.addActionListener(this);
	    }
	    else if(e.getActionCommand().equals("Save")){
		rebuildTree();
		//Define here what we do to save the new information to a new movie description
		debug("Save " + ((true)?"successful":"unsuccessful"));
	    }
	    else if(e.getActionCommand().equals("Restore")) {
		boolean restoreSuccess = ml.resetFromJsonFile("movies.json");
		if(restoreSuccess){
		    System.out.println("Movie Library Restored Successfully");
		    rebuildTree();
		}
	    }
	    else if(e.getActionCommand().equals("Tree Refresh")) {
		rebuildTree();
	    }
	    else if(e.getActionCommand().equals("Add")) {
		//Add a new movie description for the text fields.
		String title = titleJTF.getText();
		String rated = ratedJTF.getText();
		String released = releasedJTF.getText();
		String runtime = runtimeJTF.getText();
		String fileName = fileNameJTF.getText();
		String plot = plotJTA.getText();
		//Vector<String> genre = genreJCB.getText();    Need to figure out how to get genre and actors input
		//Vector<String> actors = actorsJCB.getText();
		System.out.println("Here are the stats of the movie you would like to add: ");
		System.out.println("Title: " + title);
		System.out.println("Rating: " + rated);
		System.out.println("Released: " + released);
		System.out.println("Runtime: " + runtime);
		System.out.println("File Name: " + fileName);
		System.out.println("Plot: " + plot);
	    }
	    else if(e.getActionCommand().equals("Remove")) {
		DefaultMutableTreeNode node = (DefaultMutableTreeNode)
		    tree.getLastSelectedPathComponent();
		String selectedMovie = (String)node.getUserObject();
		//System.out.println("Selected Movie is: " + selectedMovie);

		boolean removeResult = ml.remove(selectedMovie);
		if(removeResult){
		    System.out.println(selectedMovie + " was removed from the library.");
		    rebuildTree();
		}
	    }
	    else if(e.getActionCommand().equals("Play")) {
		try{
		    DefaultMutableTreeNode node = (DefaultMutableTreeNode)
			tree.getLastSelectedPathComponent();
		    String selectedMovie = (String)node.getUserObject();
		    //System.out.println("Selected Movie is: " + selectedMovie);

		    movieDescription temp = ml.get(selectedMovie);
		    String fileName = temp.getFilename();
		    byte[] byteFileName = fileName.getBytes();

		    //Connect to the server and download
		    Socket sock = new Socket(host, Integer.parseInt(portDL));
		    OutputStream outStream = sock.getOutputStream();
		    InputStream inStream = sock.getInputStream();
		    
		    byte[] sendFileName = fileName.getBytes();
		    byte[] recievedByteFile = new byte[10000000];

		    FileOutputStream fos = new FileOutputStream("DataClient/" + fileName);
		    BufferedOutputStream bos = new BufferedOutputStream(fos);

		    //Send the file name to the server
		    outStream.write(byteFileName, 0, byteFileName.length);
		    System.out.println("Sent file name to server. Name: " + fileName);

		    //Read the file
		    int count;
		    while ((count = inStream.read(recievedByteFile, 0, recievedByteFile.length)) > 0){
			bos.write(recievedByteFile, 0, count);
		    }

		    bos.flush();

		    String fileURL = "file://" + System.getProperty("user.dir")+"/DataClient/"+fileName;
		    System.out.println("File location: " + fileURL);
		    playMovie(fileURL, "MY VIDEO");
		    
		}catch(Exception ex){
		    System.out.println("There was a problem playing the media");
		}
	    }
	    tree.addTreeSelectionListener(this);
	}catch(Exception err){
	    err.printStackTrace();
	    System.out.println("Something went wrong talking to server!");
	}
    }

    public boolean sezToStop(){
	return stopPlaying;
    }
    
    public static void main(String args[]){

	host = "localhost";
	port = "8080";
	portDL = "3030";
	
	try{
	    String authorsName = "Kaelan's Library";
	    if(args.length == 2){
		host = args[0];
		port = args[1];
	    }else if(args.length > 2){
		host = args[0];
		port = args[1];
		portDL = args[2];
	    }
	    
	    String url = "http://" + host + ":" + port + "/";
	    System.out.println("The URL is ..\n" + url);
	    ml = (movieLibrary)new MovieHttpProxy(new URL(url));
	    movieLibraryClient myLibraryGUI = new movieLibraryClient(authorsName);

	    //The commented code below was for initial testing the communication between the
	    //cpp server and the java client through command line.
	    /*
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
	    */
	}catch (Exception e){
	    e.printStackTrace();
	    System.out.println("Something went wrong initilizing!");
	}
	    //System.out.println("calling constructor name " + authorName);
    }    
    
}
