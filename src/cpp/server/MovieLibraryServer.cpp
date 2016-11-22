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
* @version October 2016
*/

#include <jsonrpccpp/server.h>
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <json/json.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <csignal>

#include "movielibraryserverstub.h"
#include "MovieLibrary.hpp"

using namespace jsonrpc;
using namespace std;

class MovieLibraryServer : public movielibraryserverstub {
public:
  /*
   * All the mothods and constructors of this class will have a small cout
   * section that is used to print to the console so we know what the server
   * is doing since we are interacting with it through another machine
   */
  MovieLibraryServer(AbstractServerConnector &connector, int port);
  virtual bool resetFromJsonFile(const std::string& jsonFileName);
  virtual bool saveToJsonFile(const std::string& jsonFileName);
  virtual bool remove(const std::string& aTitle);
  virtual bool testService();
  virtual Json::Value get(const std::string& aTitle);
  virtual Json::Value getTitles();
  
private:
  MovieLibrary *movieLibrary;
  int portNum;
  
};

//Initilize out server, no need for address just port number
MovieLibraryServer::MovieLibraryServer(AbstractServerConnector &connector, int port) : movielibraryserverstub(connector) {
  movieLibrary = new MovieLibrary("movies.json");
  std::cout << "Server is being constructed." << std::endl;
  portNum = port;
}


bool MovieLibraryServer::saveToJsonFile(const std::string& jsonFileName){
  cout << "Saving the movie library to " << jsonFileName << endl;
  bool retrn = movieLibrary->toJsonFile(jsonFileName);
  return retrn;
}

bool MovieLibraryServer::resetFromJsonFile(const std::string& jsonFileName){
  cout << "Resetting the movie library to " << jsonFileName << endl;
  bool retrn = movieLibrary->resetFromJson(jsonFileName);
  return retrn;
}

bool MovieLibraryServer::remove(const string& aTitle){
  cout << "Removing " << aTitle << endl;
  bool retrn = movieLibrary->remove(aTitle);
  return retrn;
}

//Just sends a true bool back to make sure communication is working
bool MovieLibraryServer::testService(){
  cout << "Client checking server connection" << endl;
  return true;
}

Json::Value MovieLibraryServer::get(const string& aTitle){
  cout << "Getting " << aTitle << endl;
  return movieLibrary->get(aTitle);
}

Json::Value MovieLibraryServer::getTitles(){
  cout << "Getting a list of the movie titles" << endl;
  return movieLibrary->getTitles();
}

void exiting(){
  cout << "Server has been terminated. Exiting normally" << endl;
}

/**
 * main Method, 
 * thsi will take the input from that starting command from the user 
 * initilize and start the server and then wait for commands from clients 
 * until the process is killed or the enter key is pressed.
 */
int main(int argc, char * argv[]) {
  int port = 8080;
  
  if(argc > 1){
    port = atoi(argv[1]);
  }
  HttpServer httpserver(port);
  MovieLibraryServer movieServer(httpserver, port);
  cout << "Movie library server listening on port " << port
       << " use ps to get pid. To quit: kill -9 pid or press enter key " << endl;
  movieServer.StartListening();
  int c = getchar();
  //If the server is not closed properly you may have to reboot the computer
  //to make the port available again.
  movieServer.StopListening();
  return 0;
}

