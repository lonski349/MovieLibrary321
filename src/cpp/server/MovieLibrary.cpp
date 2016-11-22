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

#include "MovieLibrary.hpp"
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <jsoncpp/json/json.h>
#include <jsonrpccpp/server.h>
#include <jsonrpccpp/server/connectors/httpserver.h>

//Constructor
MovieLibrary::MovieLibrary() {
}

//Constructor for file name of .json file 
MovieLibrary::MovieLibrary(string jsonFileName)
{
  //ResetFromFile is allready implemented so just call that instead of reimplementing a parse function
  resetFromJson(jsonFileName);
}

//Deconstructor
MovieLibrary::~MovieLibrary() {
	myLibrary.clear();
}

/*
 * resetFromJson will take a .json file and parse it, break it up, and 
 * build movie descriptions from that file that will all get put into out movie library vector
 */
bool MovieLibrary::resetFromJson(string jsonFileName){
	bool retrn = false;
	Json::Reader reader;
	Json::Value movieData;
	std::ifstream jsonFile(jsonFileName.c_str(), std::ifstream::binary);
	//Read through the json file and load it into movie data
	bool parseSuccess = reader.parse(jsonFile,movieData,false);
	if(parseSuccess){
	  Json::Value::Members mbr = movieData.getMemberNames();
	  for (vector<string>::const_iterator i = mbr.begin(); i != mbr.end(); i++){
	    Json::Value jsonMedia = movieData[*i];
	    MovieDescription * aMovie = new MovieDescription(jsonMedia);
	    myLibrary[*i] = *aMovie;
	  }
	}
	return true;
}

/*
 * Method will create a .json file using the json format that could be picked up 
 * another time by any program language.
 */
bool MovieLibrary::toJsonFile(string jsonFileName)
{
	bool success = false;
	string outputData;
	//Setup out output stream to write to a file
	ofstream jsonObjOut;
	//To format the file we use the style writer 
	Json::StyledWriter jsonFormat;
	
	Json::Value jsonOut;
	cout << endl << "The files are being written to a file" << endl;
	//Iterate through our movie list and write each of the movies to a new json object
	for (map<string,MovieDescription>::iterator i = myLibrary.begin(); i != myLibrary.end(); i++){
	  
	  //Write the first value of the string.
	  if (i == myLibrary.begin()){
	    jsonOut[i->first] = i->second.getJson();
	    outputData = jsonFormat.write(jsonOut);
	  }
	  //Add the other values without over writing the first value
	  else{
	    jsonOut[i->first] = i->second.getJson();
	    outputData += jsonFormat.write(jsonOut);
	  }
	}
	
	//Open or create a file with the name the user gave us. 
	jsonObjOut.open(jsonFileName);
	jsonObjOut << outputData;
	//The datat has been transfered so terminate the file connection
	jsonObjOut.close();	
	cout << "The Json file has been written and is named " << jsonFileName << endl;
	  
}

//Add a movie description to the library from a json
bool MovieLibrary::add(const Json::Value& movie){
  bool retrn = false;
  MovieDescription aMovie(movie);
  myLibrary[aMovie.Title] = aMovie;
  return retrn;
}

//Removes a movie description from the library using the movie title and returns a true/false upon completion
bool MovieLibrary::remove(string aTitle) {
	bool sucessfullDelete = false;
	bool inLibrary = false;

	//Check if the movie is actually in the library
	if (myLibrary.find(aTitle) != myLibrary.end()) {
		inLibrary = true;
		cout << "The movie is in the library and will be erased" << endl;
	}

	//If the movie is in the library then delete it and then test to make sure it was deleted
	if (inLibrary) {
		myLibrary.erase(aTitle);

		if (myLibrary.find(aTitle) == myLibrary.end()) {
			sucessfullDelete = true;
		}
	}

	return sucessfullDelete;
}

//Gets the movie description data for the user from the movie library
Json::Value MovieLibrary::get(string aTitle) {
	
	MovieDescription description = myLibrary[aTitle];
	return description.getJson();
}

//Retrieves a list of all the titles in the movie library
Json::Value MovieLibrary::getTitles()
{
	vector<string> titles;	//Use a vector to hold the values instead of an array, dunamically sized.
	map<string, MovieDescription>::iterator mapIterator;
	for (mapIterator = myLibrary.begin(); mapIterator != myLibrary.end(); mapIterator++) { //Iterate through the length of the vector to grab each title
		titles.push_back(mapIterator->first);
	}

	Json::Value jsonTitles(Json::arrayValue);
	for (std::vector<string>::iterator vecIt = titles.begin(); vecIt != titles.end(); vecIt++){
	  jsonTitles.append(Json::Value(*vecIt));
	}
	
	return jsonTitles;
}

/*
 * Probably not needed, available for possible future implementation.
 * After implementing this I could use it to build the tree so it would
 * be seperated more by using the genres of the movies. Multiple genre
 * movies may have to be duplicated for multiple genres but that shoudl 
 * be ok
 */
//vector<string> MovieLibrary::getGenres(){
//	vector<string> genres;
	
//}


