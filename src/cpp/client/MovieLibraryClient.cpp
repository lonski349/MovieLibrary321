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

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string>
#include <sstream>
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
#include <jsonrpccpp/client/connectors/httpclient.h>
#include <json/json.h>
#include "MovieLibraryClientGUI.cpp"
#include "movielibraryclientstub.h"
#include "../server/MovieDescription.hpp"

using namespace jsonrpc;
using namespace std;

class MovieLibraryClient : public MovieLibraryClientGUI {
  std::string appAuthor; //Holds the name for the tree and display
  
public:
  //Construct our movie library class and GUI class
  MovieLibraryClient(std::string name, const char *localhost) : MovieLibraryClientGUI(name.c_str(), localhost){
    callback(ClickedX, (void*)this);
    menubar->callback(Menu_ClickedS, (void*)this);
    appAuthor = name;
    buildTree(appAuthor);
    tree->callback(TreeCallBackS, (void*)this);
  }
  
  //Operation for when the x button is clicked
  static void ClickedX(Fl_Widget *w, void * userData){
    std::cout << "You clicked Exit" << std::endl;
    MovieLibraryClient *o = (MovieLibraryClient*)userData;
    exit(1);
  }
  
  //Static operation for when the tree is clicked
  static void TreeCallBackS(Fl_Widget *w, void *data) {
    MovieLibraryClient *o = (MovieLibraryClient*)data;
    movielibraryclientstub *cls = o->cs;
    //Send to the non-static tree callback method to make it easier to implement
    o->TreeCallback(o,cls);
  }
  
  /*
   * Non-Static tree callback. We have all of the operatoins for the tree in 
   * here so we dont have to define most of our project as static variables
   */
  void TreeCallback(MovieLibraryClient *o, movielibraryclientstub *clientstub){
    Fl_Tree_Item *item = (Fl_Tree_Item*)tree->item_clicked();
    std::cout << "Tree callback. Currentselection is : ";
    if (item) {
      //Print selected movie
      std::cout << item->label() << endl;
      
      
    }
    else {
      //Default values for the text boxes is empty
      std::cout << "none";
      titleInput ->value("");
      ratedInput->value("");
      runtimeInput->value("");
      releasedInput->value("");
      plotMLIn->value("");
      filenameInput->value("");
    }
    std::cout << endl;
    std::string aStr("unknown");
    std::string aTitle(item->label());
    switch (tree->callback_reason()){
      case FL_TREE_REASON_NONE: {aStr = "none"; break;}
      //If part of the tree is opened then lets go ahead and refresh it too
      case FL_TREE_REASON_OPENED: {
	aStr = "opened";
	//Opening the tree so refresh the tree to make sure it is updated
	o->buildTree(appAuthor);
	break;}
      case FL_TREE_REASON_CLOSED: {aStr = "closed"; break;}
      /*
       * Something was selected so we need to empty all of the text boxes of 
       * any previous values and then set those boxes with the data from the
       * movie the user selected
       */
      case FL_TREE_REASON_SELECTED: {
	//Clear out the values in case any of the valeues are undefined for the next movie
	titleInput ->value("");
	ratedInput->value("");
	runtimeInput->value("");
	releasedInput->value("");
	plotMLIn->value("");
	filenameInput->value("");
	genreChoice->clear();
	actorsChoice->clear();
	
	
	aStr = "selection";
	
	std::string movieSelected = item->label();
	//Define the new values for the movie selected
	Json::Value currentMovie = clientstub->get(movieSelected);
	titleInput->value(currentMovie["Title"].asString().c_str());
	ratedInput->value(currentMovie["Rated"].asString().c_str());
	releasedInput->value(currentMovie["Released"].asString().c_str());
	runtimeInput->value(currentMovie["Runtime"].asString().c_str());
	plotMLIn->value(currentMovie["Plot"].asString().c_str());
	filenameInput->value(currentMovie["Filename"].asString().c_str());
	//std::vector<string> genres = currentMovie["Genre"].asString().split
	//cout << currentMovie["Genre"].asString() << endl;
	for (auto i = currentMovie["Genre"].begin(); i != currentMovie["Genre"].end(); i++){
	  Json::Value tempjson = *i;
	  //cout << "value " << tempjson.asString();
	  genreChoice->add(tempjson.asString().c_str());
	}
	//Set the default value to be the values at position 0
	genreChoice->value(0);
	for (auto i = currentMovie["Actors"].begin(); i != currentMovie["Actors"].end(); i++){
	  Json::Value tempjson = *i;
	  //cout << "value " << tempjson.asString();
	  actorsChoice->add(tempjson.asString().c_str());
	}
	//Set the default value to be the values at position 0
	actorsChoice->value(0);
	break;
      }
      case FL_TREE_REASON_DESELECTED: {
	aStr = "deselected";
	break;}
      default: {break;}
    }
    std::cout << "callback reason: " << aStr.c_str() << endl;
  }
  
  /**
   * Static method for the user clicking on any of the menu items
   */
  static void Menu_ClickedS(Fl_Widget *w, void *data) {
    MovieLibraryClient *o = (MovieLibraryClient*)data;
    movielibraryclientstub *cls = o->cs;
    //Transfer over to the menu_clicked function to complete the menu action
    o->Menu_Clicked(o, cls);
  }
  
  /**
   * Non-static method that will do any of the operations defined in the menus
   * menu items that have not been implemented have been left with jsut a cout 
   * item that gets printed letting us know the option is working.
   */
  void Menu_Clicked(MovieLibraryClient *o, movielibraryclientstub *clientStub){
    char picked[80];
    menubar->item_pathname(picked, sizeof(picked) - 1);
    string selectPath(picked);
    cout << "Selected Menu Path: " << selectPath << endl;
    
    if(selectPath.compare("File/Save") == 0){
      cout << "Save selected" << endl;
    }
    else if(selectPath.compare("File/Restore") == 0){
      cout << "Restore movies from json file" << endl;
      bool restoreSuccess = clientStub->resetFromJsonFile("movies.json");
      if (restoreSuccess){
	cout << "Movie library has been restored, rebuilding tree." << endl;
	o->buildTree(appAuthor);
      }
      else{
	cout << "The movie library could not be restored, the tree will not be rebuilt" << endl;
      }
    }
    else if(selectPath.compare("File/Tree Refresh") == 0){
      o->buildTree(appAuthor);
      cout << "Tree Refreshed" << endl;
    }
    else if(selectPath.compare("File/Exit") == 0){
      cout << "Exit Selected. Bye" << endl;
      exit(0);
    }
    else if(selectPath.compare("Movie/Remove") == 0){
      std::string movieTitle = titleInput->value();
      cout << "Removing Movie: " << movieTitle << endl;
      bool removeResult = clientStub->remove(movieTitle);
      //cout << "Remove " << ((removeResult)?"successful":"unsuccessful") << endl;
      
      if (removeResult){
	cout << "Remove successful, building new tree" << endl;
	o->buildTree(appAuthor);
      }
      else{
	cout << "Removal failed, tree will not be refreshed" << endl;
      }
      
    }
    else if(selectPath.compare("Movie/Play") == 0){
      cout << "Play movie selected." << endl;
    }
  }
  
};

/*
 * main method for this program. Retrieves our address from the command
 * and then initilizes our client classes based on the information provided
 * by the user
 */
int main(int argc, char * argv[]) {
  string host = "http://127.0.0.1:8080";
  if (argc > 1){
    host = string(argv[1]);
    cout << "New host is: " << host << endl;
  }
  std::string nameStr = "Kaelan's Library";
  MovieLibraryClient mc(nameStr, host.c_str());
  return (Fl::run());
}
