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

#include"MovieDescription.hpp"
#include <string>
#include <vector>
#include <map>
using namespace std;

class MovieLibrary {
protected:
  std::map<std::string, MovieDescription> myLibrary;	//This map is going to be the locatoin where all the movie descriptions will be stored

public:

	//Functions for the movie library class
	MovieLibrary();
	MovieLibrary(string jsonFileName);
	~MovieLibrary();
	bool resetFromJson(string jsonFileName);
	bool toJsonFile(string jsonFileName);
	bool add(const Json::Value& movie);
	bool remove(string aTitle);
	Json::Value get(string aTitle);
	Json::Value getTitles();
	//vector<string> getGenres();
	
};
