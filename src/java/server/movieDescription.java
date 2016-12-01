package ser321.movieServer;

import org.json.*;
import java.util.Vector;


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

public class movieDescription {

    //Default values will say no description
    private String Title, Rating, Release, Runtime, Plot, Filename;
    private Vector<String> Genre, Actors;

    /**
     * Constructor taking in no information
     */
    public movieDescription(){
        Title = "No Description";
        Rating = "No Description";
        Release = "No Description";
        Runtime = "No Description";
        Plot = "No Description";
        Filename = "No Description";
    }

    /**
     * constructor taking in all of the movie information
     * @param newTitle
     * @param newRating
     * @param newRelease
     * @param newRuntime
     * @param newPlot
     * @param newFilename
     * @param newGenre
     * @param newActors
     */
    public movieDescription(String newTitle, String newRating,
			    String newRelease, String newRuntime,
			    String newPlot, String newFilename,
			    Vector<String> newGenre, Vector<String> newActors){
        Title = newTitle;
        Rating = newRating;
        Release = newRelease;
        Runtime = newRuntime;
        Plot = newPlot;
        Filename = newFilename;
        Genre = newGenre;
        Actors = newActors;
    }

    /**
     * Constructor that will take in all of the movie information in a JSON file
     * @param jsonObj the new movie file 
     */
    public movieDescription(JSONObject jsonObj){
	//Each parameter can be pulled by their keyword for their respecive area
	Title = jsonObj.getString("Title");
	Rating = jsonObj.getString("Rated");
	Release = jsonObj.getString("Released");
	Runtime = jsonObj.getString("Runtime");
	Plot = jsonObj.getString("Plot");
	Filename = jsonObj.getString("Filename");
	
	//The genre and actors are arrays of items so we will need to iterate through and pull each value
	Genre = new Vector<String>();
	JSONArray jaGenre = jsonObj.getJSONArray("Genre");
	//System.out.println("Getting the genre");
	for (int i = 0; i < jaGenre.length(); i++){
	    //System.out.println(jsonObj.getJSONArray("Genre").getString(i));
	    Genre.add(jaGenre.getString(i));
	}
	
	Actors = new Vector<String>();
	JSONArray jaActors = jsonObj.getJSONArray("Actors");
	for (int i = 0; i< jaActors.length(); i++){
	    Actors.add(jaActors.getString(i));
	}
	
    }
    //Many getters and setters for all of the descriptors of the movies
    public void setTitle(String newTitle){
        Title = newTitle;
    }

    public void setRating(String newRating){
        Rating = newRating;
    }

    public void setRelease(String newRelease){
        Release = newRelease;
    }

    public void setRuntime(String newRuntime){
        Runtime = newRuntime;
    }

    public void setPlot(String newPlot){
        Plot = newPlot;
    }

    public void setFilename(String newFilename){
        Filename = newFilename;
    }

    public void setGenre(Vector<String> newGenre){
        Genre = newGenre;
    }

    public void setActors(Vector<String> newActors){
        Actors = newActors;
    }

    /**
     * gets the title of a movie
     * @return
     */
    public String getTitle(){
        return Title;
    }

    public String getRating(){
        return Rating;
    }

    public String getRelease(){
        return Release;
    }

    public String getRuntime(){
        return Runtime;
    }

    public String getPlot(){
        return Plot;
    }

    public String getFilename(){
        return Filename;
    }

    public Vector<String> getGenre(){

	/*
	String genre = "";
	
	for (int i = 0; i < Genre.size(); i++){
	    genre += Genre.get(i);
	}
    
        return genre;
	*/
	return Genre;
    }

    public Vector<String> getActors(){
	/*
	String actors = "";
        
        for (int i = 0; i < Actors.size(); i++){
	    actors += Actors.get(i);
	}
        
        return actors;
	*/
	return Actors;
    }

    /**
     * converts all of the information of a movie into a string to be printable to the console
     * @return
     */
    public String toString(){
	String genre = "",actors = "";
	
	for (int i = 0; i < Genre.size(); i++){
	    genre += Genre.get(i);
	    if (i != Genre.size() - 1){
		genre += ", ";
	    }
	}
	
	for (int i = 0; i < Actors.size(); i++){
	    actors += Actors.get(i);
	    if (i != Actors.size() - 1){
		actors += ", ";
	    }
	}
	
        String returnString =
                "\nTitle: " + Title +
                "\nRating: " + Rating +
                "\nRelease: " + Release +
                "\nRuntime: " + Runtime +
                "\nPlot: " + Plot +
                "\nFilename: " + Filename +
                "\nGenre: " + genre +
                "\nActors: " + actors + "\n";

        return returnString;
    }
    
    /**
     *  Converts all of the movie informaton back into a JSON object so it can be written to a 
     *  file 
     */
    public JSONObject toJson(){
	JSONObject jsonObj = new JSONObject();
	
	jsonObj.put("Title", Title);
	jsonObj.put("Rated", Rating);
	jsonObj.put("Released", Release);
	jsonObj.put("Runtime", Runtime);
	jsonObj.put("Plot", Plot);
	jsonObj.put("Filename", Filename);
	jsonObj.put("Genre", Genre);
	jsonObj.put("Actors", Actors);
	
	return jsonObj;
    }
}
