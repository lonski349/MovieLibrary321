package ser321.movieServer;

import edu.asu.ser.jsonrpc.common.JsonRpcException;

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
 * Purpose: Practice Client Server Programming
 * Ser321 Foundations of Distributed Applications
 * @author Kaelan Strones kstrones@asu.edu
 *         Software Engineering, ASU Poly
 * @version August 2016
 */

//This file is being used to create the hppt proxy file
public interface movieLibrary {
    public boolean resetFromJsonFile(String jsonFileName) throws JsonRpcException;
    public boolean toJsonFile(String jsonFileName) throws JsonRpcException;
    public boolean add(movieDescription movie) throws JsonRpcException;
    public boolean remove(String aTitle) throws JsonRpcException;
    public movieDescription get(String aTitle) throws JsonRpcException;
    public String [] getTitles()throws JsonRpcException;
}
