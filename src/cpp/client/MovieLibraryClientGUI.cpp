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
* Purpose: Practice C++ Programming
* Ser321 Foundations of Distributed Applications
* @author Kaelan Strones kstrones@asu.edu
*         Software Engineering, ASU Poly
* @version August 2016
*/

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Tree.H>
#include <FL/Fl_Tree_Item.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Multiline_Input.H>

//#include <stdio.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <jsonrpccpp/client/connectors/httpclient.h>
#include <json/json.h>
#include "movielibraryclientstub.h"


using namespace std;
using namespace jsonrpc;

class MovieLibraryClientGUI : public Fl_Window {
protected:
   /**
    * tree is the Fl_Tree object that occupies the left side of the window.
    * this tree control provides the ability to add and remove items and to
    * manipulate and query the tree when an exception occurs.
    */
   Fl_Tree * tree;

   /**
    * titleInput is the Fl_Input object labelled Title
    * Its for the user to enter the media tile (prefix filename).
    */
   Fl_Input * titleInput;

   /**
    * ratedInput is the Fl_Input object labelled Rated
    * Its for the user to enter the movie rating for videos
    */
   Fl_Input * ratedInput;

   /**
    * authorInput is the Fl_Input object labelled Artist
    * Its for the user to enter the artist for music, or for entering the
    * primary actor for videos.
    */
   Fl_Input * runtimeInput;

   /**
    * releasedInput is the Fl_Input object labelled Released
    * Its for the user to enter the released date for videos.
    */
   Fl_Input * releasedInput;

   /**
    * releasedInput is the Fl_Input object labelled Released
    * Its for the user to enter the released date for videos.
    */
   Fl_Input * filenameInput;

   /**
    * plotMLIn is the Fl_Multiline_Input object labelled Plot
    * Its for the user to enter the video's story-line.
    */
   Fl_Multiline_Input * plotMLIn;

   /**
    * menubar is the Fl_Menu_Bar object.
    * Its the container for menus.
    */
   Fl_Menu_Bar *menubar;

   /**
    * Genre is the Fl_Choice object labelled Genre.
    * It provides the ability to enter and select genre
    */
   Fl_Choice *genreChoice;

   /**
    * Actors is the Fl_Choice object labelled Actors.
    * It provides the ability to enter and select actors.
    */
   Fl_Choice *actorsChoice;
   
   HttpClient *httpClient;
   movielibraryclientstub *cs;
   
public:
  
  /*
   * Builds a new tree by pulling current movie list from the server
   * Ths is going to be utilized for the initial build of the tree
   * along with refreshing the tree because we need to rebuild the tree
   * when we refresh it
   */  
  void buildTree(std::string treeRoot){
      
    //Clear all the data in the current tree
      tree->clear();
      Json::Value movieTitles = cs->getTitles();
      
      //Add titles to the stream then send it to the tree
      for(int i = 0; i < movieTitles.size(); i++){
         std::stringstream stream;
	 stream << "Videos/";
	 stream << movieTitles[i].asString();
         tree->add(stream.str().c_str());
      }
      cout << endl;
      //Set a root label for the tree and then redraw
      tree->root_label(treeRoot.c_str());
      tree->redraw();
   }
   
   /*
    * Class constructor
    * We are using this class to build our GUI using FLTK along with
    * initilizing our client and connecting our client to the server 
    * at the address they have specified
    */
   MovieLibraryClientGUI(const char * name = "Ser321", const char *serverLocation = "http://127.0.0.1:8080") : Fl_Window(635,350,name) {
     
     string host = serverLocation;
     httpClient = new HttpClient(host);
     cs = new movielibraryclientstub(*httpClient);
     
     cout << "Connecting to the host system at: " << host << endl;
     
     try {
        //Run a quick little test to make sure we are connected before we begin
        bool test = cs->testService();
	if(test){
	  std::cout << "Movie Library service is up we are connected!" << endl;
	}
	
	//Start building our FLTK items
	begin();

	//Menubar items
	menubar = new Fl_Menu_Bar(0, 0, this->w(), 25);
	menubar->add("File/Save");
	menubar->add("File/Restore");
	menubar->add("File/Tree Refresh");
	menubar->add("File/Exit");
	menubar->add("Movie/Remove");
	menubar->add("Movie/Play");

	// create a tree control at position x=10, y=10. Its 150 pixels wide
	// and window height less 20 pixels high. Add some sample tree nodes.
	tree = new Fl_Tree(10, 35, 225, this->h()-45);
	tree->root_label(name);

	/*
	* add a text input control at x=285, y=35 of width 200 pixels and
	* height of 25 pixels. Initialize it contents to media title.
	*/
	titleInput = new Fl_Input(285, 35, 195, 25);
	titleInput->label("Title");
	titleInput->value("");

	ratedInput = new Fl_Input(530, 35, 75, 25);
	ratedInput->label("Rated");
	ratedInput->value("");

	/*
	* add a text input control at x=250, y=35 of width 200 pixels and
	* height of 25 pixels. Initialize its contents to title.
	*/
	runtimeInput = new Fl_Input(315, 75, 85, 25);
	runtimeInput->label("Runtime");
	runtimeInput->value("");

	releasedInput = new Fl_Input(480, 75, 120, 25);
	releasedInput->label("Released");
	releasedInput->value("");

	//Create a box for the plot and give it settings to wrap around for long strings
	plotMLIn = new Fl_Multiline_Input(285, 115, 325, 145);
	plotMLIn->label("Plot");
	plotMLIn->value("");
	plotMLIn->wrap(8);

	filenameInput = new Fl_Input(315, 275, 230, 25);
	filenameInput->label("Filename");
	filenameInput->value("");
  
	// create the media type drop-down (input_choice)
	actorsChoice = new Fl_Choice(295, 315, 150, 25, "Actors");

	// create the media type drop-down (input_choice)
	genreChoice = new Fl_Choice(502, 315, 120, 25, "Genre");

	//Finish building and then display the panel
	end();
	show();
     }
     //Catch json rpc exceptions
     catch (JsonRpcException ex) {
       cerr << ex.what() << endl;
     }
   }
     
};
