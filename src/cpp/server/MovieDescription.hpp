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

#include <string>
#include <jsoncpp/json/json.h>
#include <vector>

using namespace std;

class MovieDescription {
public:
	//Data for the movie descriptions
	string Title, Rating, Release, Runtime, Plot, Filename;
	vector<string> Genre;
	vector<string> Actors;
	Json::Value jsonObj;

	//Basic getters and setters for the movie description class
	MovieDescription();
	MovieDescription(string newTitle, string newRating, string mewRelease, string newRuntime, string newPlot, string newFilename, vector<string> newGenre, vector<string> newActors);
	MovieDescription(Json::Value jsonObj);
	MovieDescription(string jsonString);
	~MovieDescription();
	void setTitle(string newTitle);
	void setRating(string newRating);
	void setRelease(string newRelease);
	void setRuntime(string newRuntime);
	void setPlot(string newPlot);
	void setFilename(string newFilename);
	void setGenre(vector<string> newGenre);
	void setActors(vector<string> newActors);
	void fromJson(Json::Value Jval);
	string getTitle();
	string getRating();
	string getRelease();
	string getRuntime();
	string getPlot();
	string getFilename();
	vector<string> getGenre();
	vector<string> getActors();
	string toString();
	string toJsonString();
	Json::Value getJson();
	Json::Value toJson();
};
