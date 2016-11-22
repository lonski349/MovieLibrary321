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

#include "MovieDescription.hpp"
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <stdio.h>
#include <vector>

//Constructor
MovieDescription::MovieDescription() {
	Title = "No Description";
	Rating = "No Description";
	Release = "No Description";
	Runtime = "No Description";
	Plot = "No Description";
	Filename = "No Description";
}

//Constructor
MovieDescription::MovieDescription(string newTitle, string newRating, string newRelease, string newRuntime, string newPlot, string newFilename, vector<string> newGenre, vector<string>newActors) {
	Title = newTitle;
	Rating = newRating;
	Release = newRelease;
	Runtime = newRuntime;
	Plot = newPlot;
	Filename = newFilename;
	Genre = newGenre;
	Actors = newActors;
}

//Build your movie description from a json object
MovieDescription::MovieDescription(Json::Value sentJsonObj)
{
	jsonObj = sentJsonObj;
	//Read each of our valued from the json object and save them to their respective locations
	Title = jsonObj["Title"].asString();
	Rating = jsonObj["Rated"].asString();
	Release = jsonObj["Released"].asString();
	Runtime = jsonObj["Runtime"].asString();
	Plot = jsonObj["Plot"].asString();
	Filename = jsonObj["Filename"].asString();
	
	//Genre and actors are both like an array when they are pulled so we need to iterate through the list to collect our values
	Json::Value temp = jsonObj.get("Genre",0);
	for (auto i = temp.begin(); i != temp.end(); i++){
	  Json::Value items = *i;
	  this->Genre.push_back(items.asString());
	}
	temp = jsonObj.get("Actors",0);
	for (auto i = temp.begin(); i != temp.end(); i++){
	  Json::Value items = *i;
	  this->Actors.push_back(items.asString());
	}
}

//Build your movie description from a json string instead of a json object
MovieDescription::MovieDescription(string jsonString){
	string titleStr = "Title";
	string ratingStr = "Rated";
	string releaseStr = "Released";
	string runtimeStr = "Runtime";
	string plotStr = "Plot";
	string filenameStr = "Filename";
	string genreStr = "Genre";
	string actorsStr = "Actors";
	
	Json::Reader reader;
	Json::Value root;
	
	//Parse the json String Provided, save results to root and do not collect comments
	bool parseSuccess = reader.parse(jsonString,root,false);
	jsonObj = root;
	if(parseSuccess){
		Json::Value::Members member = root.getMemberNames();
		for (vector<string>::const_iterator i = member.begin(); i != member.end(); i++){
			Json::Value jsonM = root[*i];
			if (titleStr.compare(*i) == 0){
				Title = jsonM.asString();
			}
			else if (ratingStr.compare(*i) == 0){
				Rating = jsonM.asString();
			}
			else if (releaseStr.compare(*i) == 0){
				Release = jsonM.asString();
			}
			else if (runtimeStr.compare(*i) == 0){
				Runtime = jsonM.asString();
			}
			else if (plotStr.compare(*i) == 0){
				Plot = jsonM.asString();
			}
			else if (filenameStr.compare(*i) == 0){
				Filename = jsonM.asString();
			}
			else if (genreStr.compare(*i) == 0){
				//A method needs to be implemented to complete this portion
				//genre = jsonM.asString();
			}
			else if (actorsStr.compare(*i) == 0){
				//A method needs to be implemented to complete this portion
				//actors = jsonM.asString();
			}
		}
	}

}

//Destructor
MovieDescription::~MovieDescription() {
	Title = "No Description";
	Rating = "No Description";
	Release = "No Description";
	Runtime = "No Description";
	Plot = "No Description";
	Filename = "No Description";
}

//Basic setters and getters for the movie description information
void MovieDescription::setTitle(string newTitle) {
	Title = newTitle;
}

void MovieDescription::setRating(string newRating) {
	Rating = newRating;
}

void MovieDescription::setRelease(string newRelease) {
	Release = newRelease;
}

void MovieDescription::setRuntime(string newRuntime) {
	Runtime = newRuntime;
}

void MovieDescription::setPlot(string newPlot) {
	Plot = newPlot;
};

void MovieDescription::setFilename(string newFilename) {
	Filename = newFilename;
}

void MovieDescription::setGenre(vector<string> newGenre) {
	Genre = newGenre;
}

void MovieDescription::setActors(vector<string> newActors) {
	Actors = newActors;
}

void MovieDescription::fromJson(Json::Value Jval){
 	//Read each of our valued from the json object and save them to their respective locations
	Title = jsonObj["Title"].asString();
	Rating = jsonObj["Rated"].asString();
	Release = jsonObj["Released"].asString();
	Runtime = jsonObj["Runtime"].asString();
	Plot = jsonObj["Plot"].asString();
	Filename = jsonObj["Filename"].asString();
	
	//Genre and actors are both like an array when they are pulled so we need to iterate through 		the list to collect our values
	Json::Value temp = jsonObj.get("Genre",0);
	for (auto i = temp.begin(); i != temp.end(); i++){
	  Json::Value items = *i;
	  this->Genre.push_back(items.asString());
	}
	temp = jsonObj.get("Actors",0);
	for (auto i = temp.begin(); i != temp.end(); i++){
	  Json::Value items = *i;
	  this->Actors.push_back(items.asString());
	}
}

string MovieDescription::getTitle() {
	return Title;
}

string MovieDescription::getRating() {
	return Rating;
}

string MovieDescription::getRelease() {
	return Release;
}

string MovieDescription::getRuntime() {
	return Runtime;
}

string MovieDescription::getPlot() {
	return Plot;
}

string MovieDescription::getFilename() {
	return Filename;
}

vector<string> MovieDescription::getGenre() {
	return Genre;
}

vector<string> MovieDescription::getActors() {
	return Actors;
}

//To string function allows us to print the moviedestription object
string MovieDescription::toString() {
	string genreStr;
	for (int i = 0; i < Genre.size(); i++) {
		genreStr += Genre[i];
		if (i != Genre.size() - 1) {
			genreStr += ", ";
		}
	}

	string actorStr;
	for (int i = 0; i < Actors.size(); i++) {
		actorStr += Actors[i];
		if (i != Actors.size() - 1) {
			actorStr += ", ";
		}
	}

	string returnString =
		"\nTitle: " + Title
		+ "\nRating: " + Rating
		+ "\nRelease: " + Release
		+ "\nRuntime: " + Runtime
		+ "\nPlot: " + Plot
		+ "\nFilename: " + Filename
		+ "\nGenre: " + genreStr
		+ "\nActors: " + actorStr;
	return returnString;
}

Json::Value MovieDescription::getJson(){
	return jsonObj;
}

/*
 * This toJson method needs to be rethought.
 * The issue parts are the genre and actors, they are not built in a way that 
 * the json methods will pull them back out. left for future iterations
 */
Json::Value MovieDescription::toJson()
{
	//Save each of our groups to their json location, building a new json object for us to write to a new file.
        Json::Value jsonObj;
	jsonObj["Title"] = Title;
	jsonObj["Rated"] = Rating;
	jsonObj["Released"] = Release;
	jsonObj["Runtime"] = Runtime;
	jsonObj["Plot"] = Plot;
	jsonObj["Filename"] = Filename;
	
	//We will need to iterate through genre and actors because they are both vectors
	string genre = "";
	for(auto i = Genre.begin(); i != Genre.end(); i++){
	  genre.append(*i);
	  if (i < Genre.end()-1){
	    genre.append(",");
	  }
	}
	jsonObj["Genre"] = genre;
	
	string actors = "";
	for(auto i = Actors.begin(); i!= Actors.end(); i++){
	  actors.append(*i);
	  if (i < Actors.end()-1){
	    actors.append(",");
	  }
	  
	}
	jsonObj["Actors"] = actors;
	
	return jsonObj;
}
