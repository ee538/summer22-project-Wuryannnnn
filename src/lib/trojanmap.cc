#include "trojanmap.h"

//-----------------------------------------------------
// TODO: Student should implement the following:
//-----------------------------------------------------
/**
 * GetLat: Get the latitude of a Node given its id. If id does not exist, return
 * -1.
 *
 * @param  {std::string} id : location id
 * @return {double}         : latitude
 */
double TrojanMap::GetLat(const std::string &id) { 
  return data[id].lat; }

/**
 * GetLon: Get the longitude of a Node given its id. If id does not exist,
 * return -1.
 *
 * @param  {std::string} id : location id
 * @return {double}         : longitude
 */
double TrojanMap::GetLon(const std::string &id) { 
  return data[id].lon; 
 }

/**
 * GetName: Get the name of a Node given its id. If id does not exist, return
 * "NULL".
 *
 * @param  {std::string} id : location id
 * @return {std::string}    : name
 */
std::string TrojanMap::GetName(const std::string &id) { 
  return data[id].name;  }

/**
 * GetNeighborIDs: Get the neighbor ids of a Node. If id does not exist, return
 * an empty vector.
 *
 * @param  {std::string} id            : location id
 * @return {std::vector<std::string>}  : neighbor ids
 */
std::vector<std::string> TrojanMap::GetNeighborIDs(const std::string &id) {
  std::vector<std::string> res = data[id].neighbors;
  return res;
}

/**
 * GetID: Given a location name, return the id.
 * If the node does not exist, return an empty string.
 *
 * @param  {std::string} name          : location name
 * @return {int}  : id
 */
std::string TrojanMap::GetID(const std::string &name) {
  std::string res = "";
  std::unordered_map<std::string, Node>::iterator iter;
  for (iter = data.begin(); iter != data.end(); ++ iter){
    if (iter -> second.name.compare(name) == 0){
      res.append(iter -> first);
    }
  }
  return res;
}

/**
 * GetPosition: Given a location name, return the position. If id does not
 * exist, return (-1, -1).
 *
 * @param  {std::string} name          : location name
 * @return {std::pair<double,double>}  : (lat, lon)
 */
std::pair<double, double> TrojanMap::GetPosition(std::string name) {
  std::pair<double, double> results(-1, -1);
  std::unordered_map<std::string, Node>::iterator iter;
  for (iter = data.begin(); iter != data.end(); ++ iter){
    if (iter -> second.name.compare(name) == 0){
      results.first = iter -> second.lat;
      results.second = iter -> second.lon;
    }
  }
  return results;
}

/**
 * CalculateEditDistance: Calculate edit distance between two location names
 *
 */
int TrojanMap::CalculateEditDistance(std::string a, std::string b) {
  std::vector<std::vector<int>> dynamic_solution(a.size()+1,std::vector<int>(b.size()+1,0));
  
  dynamic_solution[0][0] = 0;
  for(int i =0; i <= a.size(); i++){
    dynamic_solution[i][0] = i;
  }
  for(int i =0; i <= b.size(); i++){
    dynamic_solution[0][i] = i;
  }
  

  for(int i =0; i < a.size(); i++){
    for(int j = 0; j < b.size();j++){
      if(a[i] == b[j]){
        dynamic_solution[i+1][j+1] = dynamic_solution[i][j];
      }else{
        dynamic_solution[i+1][j+1] = std::min(std::min(dynamic_solution[i][j+1],dynamic_solution[i+1][j]),dynamic_solution[i][j])+1;
      }
    }
  }

  return dynamic_solution[a.size()][b.size()];

}

/**
 * FindClosestName: Given a location name, return the name with smallest edit
 * distance.
 *
 * @param  {std::string} name          : location name
 * @return {std::string} tmp           : similar name
 */

std::string tolower_(std::string a){
  if(a.size() == 0){
    return "Nothing be found";
  }
  for(int i = 0; i < a.size(); i++){
    if(a[i] >= 'A' && a[i] <= 'Z'){
      a[i] += 32;
    }
  }
  return a;
}

std::string TrojanMap::FindClosestName(std::string name) {
  std::string tmp = "";
  std::unordered_map<std::string, Node>::iterator iter;
  for (iter = data.begin(); iter != data.end(); ++iter){
    std::string str = iter -> second.name;
    
    int temp_ = CalculateEditDistance(tolower_(name), tolower_(str));
    
    if(temp_ == 2){
      tmp = str;
      return tmp;
    }
  }
  return tmp;
}

/**
 * Autocomplete: Given a parital name return all the possible locations with
 * partial name as the prefix. The function should be case-insensitive.
 *
 * @param  {std::string} name          : partial name
 * @return {std::vector<std::string>}  : a vector of full names
 */
std::vector<std::string> TrojanMap::Autocomplete(std::string name) {
  std::vector<std::string> results;
  std::unordered_map<std::string, Node>::iterator iter;
  for (iter = data.begin(); iter != data.end(); ++iter){
    std::string str = iter -> second.name;
    transform(str.begin(), str.end(), str.begin(), tolower);
    transform(name.begin(), name.end(), name.begin(), tolower);
    if (str.rfind(name,0) == 0){
      results.push_back(iter -> second.name);
    }
  }
  return results;
}

/**
 * GetAllCategories: Return all the possible unique location categories, i.e.
 * there should be no duplicates in the output.
 *
 * @return {std::vector<std::string>}  : all unique location categories
 */
std::vector<std::string> TrojanMap::GetAllCategories() {
  std::vector<std::string> results;
  std::unordered_map<std::string, Node>::iterator iter;
  for (iter = data.begin(); iter != data.end(); ++iter){
    std::unordered_set<std::string> uo_set = iter -> second.attributes;
    std::unordered_set<std::string>::iterator set_iter;
    for(set_iter = uo_set.begin(); set_iter != uo_set.end(); ++set_iter){
      if(std::find(results.begin(), results.end(), *set_iter) == results.end()){
        results.push_back(*set_iter);
      }
    }
    

  }
  return results;
}

/**
 * GetAllLocationsFromCategory: Return all the locations of the input category (i.e.
 * 'attributes' in data.csv). If there is no location of that category, return
 * (-1, -1). The function should be case-insensitive.
 *
 * @param  {std::string} category          : category name (attribute)
 * @return {std::pair<double, double>}     : (lat, lon)
 */
std::vector<std::pair<double, double>> TrojanMap::GetAllLocationsFromCategory(
  std::string category) {
    std::pair<double, double> results(-1, -1);
    std::vector<std::pair<double, double>> v1;
    std::unordered_map<std::string, Node>::iterator iter;
    for (iter = data.begin(); iter != data.end(); ++iter){
      results.first = iter -> second.lat;
      results.second = iter -> second.lon;
      std::unordered_set<std::string> uo_set = iter -> second.attributes;
      std::unordered_set<std::string>::iterator set_iter;
      for(set_iter = uo_set.begin(); set_iter != uo_set.end(); ++set_iter){
        if(category == *set_iter){
          v1.push_back(results);
        }
      }
    }
    return v1;
  }
/**
 * GetLocationRegex: Given the regular expression of a location's name, your
 * program should first check whether the regular expression is valid, and if so
 * it returns all locations that match that regular expression.
 *
 * @param  {std::regex} location name      : the regular expression of location
 * names
 * @return {std::pair<double, double>}     : (lat, lon)
 */
std::vector<std::pair<double, double>> TrojanMap::GetLocationRegex(std::regex location) {
    std::pair<double, double> results(-1, -1);
    std::vector<std::pair<double, double>> v2;
    std::unordered_map<std::string, Node>::iterator iter;
    for (iter = data.begin(); iter != data.end(); ++iter){
      results.first = iter -> second.lat;
      results.second = iter -> second.lon;
      std::string str = iter -> second.name;
      if(std::regex_match (str, location)){
        v2.push_back(results);
      }
      }
    return v2;
  }
/**
 * CalculateDistance: Get the distance between 2 nodes.
 *
 * @param  {std::string} a  : a_id
 * @param  {std::string} b  : b_id
 * @return {double}  : distance in mile
 */
double TrojanMap::CalculateDistance(const std::string &a_id,
                                    const std::string &b_id) {
  // Do not change this function
  Node a = data[a_id];
  Node b = data[b_id];
  double dlon = (b.lon - a.lon) * M_PI / 180.0;
  double dlat = (b.lat - a.lat) * M_PI / 180.0;
  double p = pow(sin(dlat / 2), 2.0) + cos(a.lat * M_PI / 180.0) *
                                           cos(b.lat * M_PI / 180.0) *
                                           pow(sin(dlon / 2), 2.0);
  double c = 2 * asin(std::min(1.0, sqrt(p)));
  return c * 3961;
}

/**
 * CalculatePathLength: Calculates the total path length for the locations
 * inside the vector.
 *
 * @param  {std::vector<std::string>} path : path
 * @return {double}                        : path length
 */
double TrojanMap::CalculatePathLength(const std::vector<std::string> &path) {
  // Do not change this function
  double sum = 0;
  for (int i = 0; i < int(path.size()) - 1; i++) {
    sum += CalculateDistance(path[i], path[i + 1]);
  }
  return sum;
}

/**
 * CalculateShortestPath_Dijkstra: Given 2 locations, return the shortest path
 * which is a list of id. Hint: Use priority queue.
 *
 * @param  {std::string} location1_name     : start
 * @param  {std::string} location2_name     : goal
 * @return {std::vector<std::string>}       : path
 */
std::vector<std::string> TrojanMap::CalculateShortestPath_Dijkstra(
    std::string location1_name, std::string location2_name) {
  std::string start = GetID(location1_name);
  std::string end = GetID(location2_name);
  std::map<std::string, std::vector<std::string>> visited;
  visited[start].push_back(start);
  std::priority_queue< std::pair<double, std::string>, 
                      std::vector<std::pair<double, std::string>>, 
                      std::greater<std::pair<double, std::string>>> q;
  std::map<std::string, double> dis;
  for (auto o: data){
    dis[o.first] = DBL_MAX;
  }
  dis[start] = 0;
  for (auto e: GetNeighborIDs(start)){
    dis[e] = std::min(dis[e], CalculateDistance(e, start));
  }
  std::pair<double, std::string> p1 = {0,start};
  q.push(p1);

  while(!q.empty()){
    std::string s1 = q.top().second;
    q.pop();
    for (auto s2: GetNeighborIDs(s1)){
      double tot_dis = CalculateDistance(s1, s2);
      if (dis[s1] + tot_dis > dis[s2]){
        continue;
      }
        dis[s2] = dis[s1] + tot_dis;
        visited[s2] = visited[s1];
        visited[s2].push_back(s2);
        std::pair<double, std::string> p2 = {dis[s2], s2};
        q.push(p2);

    }
    if(q.top().second.compare(end) == 0){
      break;
    }
  }
  return visited[end];
}

/**
 * CalculateShortestPath_Bellman_Ford: Given 2 locations, return the shortest
 * path which is a list of id. Hint: Do the early termination when there is no
 * change on distance.
 *
 * @param  {std::string} location1_name     : start
 * @param  {std::string} location2_name     : goal
 * @return {std::vector<std::string>}       : path
 */
std::vector<std::string> TrojanMap::CalculateShortestPath_Bellman_Ford(
    std::string location1_name, std::string location2_name) {
  std::vector<std::string> path;
  std::map<std::string, std::pair<std::string, double>> round;
  std::unordered_map<std::string, Node>::iterator iter;
  for (iter = data.begin(); iter != data.end(); ++iter){
    //std::pair<std::string, double> temp;
    round[iter -> first] =  {"", INT_MAX};
  }
  std::string start = GetID(location1_name);
  std::string end = GetID(location2_name);
  round[start] = {"START", 0};
  bool relax = true;
  for(int i =0; i< data.size(); i++){

    if(relax == false){
      break;
    }
    relax = false;
    for (iter = data.begin(); iter != data.end(); ++iter){

        std::vector<std::string> neighbor = GetNeighborIDs(iter->first);
        for(std::string s: neighbor){
            if(round[iter->first].second > round[s].second + CalculateDistance(s,iter->first)){
              round[iter->first].first = s;
              round[iter->first].second = round[s].second + CalculateDistance(s,iter->first);
              relax = true;
            }
        }
    }
    
  }

  path.push_back(end);
  std::string prev;
  prev = round[end].first;
  std::cout << round[end].second <<std::endl;
  if(prev.compare("") == 0){
    std::vector<std::string> empty;
    return empty;
  }

  while(prev.compare("START") != 0){
    path.push_back(prev);
    prev = round[prev].first;
  }

  
  
  std::reverse(path.begin(),path.end());
  return path;
}



/**
 * Traveling salesman problem: Given a list of locations, return the shortest
 * path which visit all the places and back to the start point.
 *
 * @param  {std::vector<std::string>} input : a list of locations needs to visit
 * @return {std::pair<double, std::vector<std::vector<std::string>>} : a pair of
 * total distance and the all the progress to get final path
 */
void TrojanMap::TravelingTrojan_BF(std::string start,
                        std::vector<std::string> location_ids,
                        std::vector<std::string> cur_path, 
                        double &min_cost,
                        std::vector<std::string> &min_path,
                        std::vector<std::vector<std::string>> &record_list){
    if (cur_path.size() == location_ids.size()){
      cur_path.push_back(start);
      double cost = CalculatePathLength(cur_path);
      if(cost < min_cost){
        min_cost = cost;
        min_path = cur_path;
        record_list.push_back(cur_path);
      }
      return;
    }

    for(int i =0; i< location_ids.size(); i++){
      if(std::find(cur_path.begin(),cur_path.end(), location_ids[i]) != cur_path.end()){
        continue;
      }

      cur_path.push_back(location_ids[i]);
      TravelingTrojan_BF(start, location_ids,cur_path, min_cost, min_path,record_list);
      cur_path.pop_back();
    }
}

std::pair<double, std::vector<std::vector<std::string>>>
TrojanMap::TravelingTrojan_Brute_force(std::vector<std::string> location_ids) {
  //递归法
  /*
  std::vector<std::vector<std::string>> order_list;
  double distance = 100000;
  
  if(location_ids.size() == 0){
    std::pair<double, std::vector<std::vector<std::string>>> records(0,order_list);
    return records;
  }
  std::string current_id = location_ids[0];
  if(location_ids.size() == 1){
    std::vector<std::string> paths;
    paths.push_back(current_id);
    paths.push_back(current_id);
    order_list.push_back(paths);
    std::pair<double, std::vector<std::vector<std::string>>> records(0,order_list);
    return records;
  }

  //执行递归少掉开头的情况
    std::vector<std::string> next_vector;
    for(int j = 1;j < location_ids.size(); j++){
          next_vector.push_back(location_ids[j]);
          //std::cout << location_ids[j] << std::endl;
      }
      std::pair<double, std::vector<std::vector<std::string>>> next;
      next = TravelingTrojan_Brute_force(next_vector);

      std::vector<std::vector<std::string>> next_paths = next.second;
      //遍历所有递归后的结果
      for(int k = 0; k < next_paths.size(); k++){
        
        //根据位置插入current ID
         for(int n = 0; n < next_paths[k].size() - 1; n++){
          std::vector<std::string> paths;
          paths.push_back(current_id);
          for(int m = 0; m < next_paths[k].size() - 1; m++){
            int index = m + n;
            if(index >= next_paths[k].size() - 1){
              index = index - next_paths[k].size() + 1;
            }
            
            paths.push_back(next_paths[k][index]);
          }
          paths.push_back(current_id);


          //计算paths的长度 与最小值做比较
          double temp_d = CalculatePathLength(paths);
          order_list.push_back(paths);
          if(distance > temp_d){
            distance = temp_d;
          }
        }
        
      
    }
    
  std::pair<double, std::vector<std::vector<std::string>>> records(distance,order_list);
  return records;
*/
  //
  std::vector<std::vector<std::string>> order_list;
  if(location_ids.size() == 0){
    std::pair<double, std::vector<std::vector<std::string>>> records(0,order_list);
    return records;
  }
  std::string current_id = location_ids[0];
  if(location_ids.size() == 1){
    std::vector<std::string> paths;
    paths.push_back(current_id);
    paths.push_back(current_id);
    order_list.push_back(paths);
    std::pair<double, std::vector<std::vector<std::string>>> records(0,order_list);
    return records;
  }
  std::vector<std::string> cur_path = {location_ids[0]};
  std::vector<std::string> min_path;
  double min_cost = INT_MAX;
  TravelingTrojan_BF(current_id,location_ids,cur_path,min_cost,min_path,order_list);
  std::pair<double, std::vector<std::vector<std::string>>> records(min_cost,order_list);
  return records;


}

void TrojanMap::TravelingTrojan_BT(std::string start,
                        std::vector<std::string> location_ids,
                        std::vector<std::string> cur_path, 
                        double &min_cost,
                        std::vector<std::string> &min_path,
                        std::vector<std::vector<std::string>> &record_list){
    if (cur_path.size() ==location_ids.size()){
      cur_path.push_back(start);
      double cost = CalculatePathLength(cur_path);
      if(cost < min_cost){
        min_cost = cost;
        min_path = cur_path;
        record_list.push_back(cur_path);
      }
      return;
    }
    double cost = CalculatePathLength(cur_path);
    if(cost >= min_cost){
        return;
    }
    for(int i =0; i< location_ids.size(); i++){
      if(std::find(cur_path.begin(),cur_path.end(), location_ids[i])!= cur_path.end()){
        continue;
      }

      cur_path.push_back(location_ids[i]);
      TravelingTrojan_BT(start, location_ids,cur_path, min_cost, min_path,record_list);
      cur_path.pop_back();
    }
}

std::pair<double, std::vector<std::vector<std::string>>>
TrojanMap::TravelingTrojan_Backtracking(std::vector<std::string> location_ids) {
  std::vector<std::vector<std::string>> order_list;
  if(location_ids.size() == 0){
    std::pair<double, std::vector<std::vector<std::string>>> records(0,order_list);
    return records;
  }
  std::string current_id = location_ids[0];
  if(location_ids.size() == 1){
    std::vector<std::string> paths;
    paths.push_back(current_id);
    paths.push_back(current_id);
    order_list.push_back(paths);
    std::pair<double, std::vector<std::vector<std::string>>> records(0,order_list);
    return records;
  }
  std::vector<std::string> cur_path = {location_ids[0]};
  std::vector<std::string> min_path;
  
  double min_cost = INT_MAX;
  TravelingTrojan_BT(current_id,location_ids,cur_path,min_cost,min_path,order_list);
  std::pair<double, std::vector<std::vector<std::string>>> records(min_cost,order_list);
  return records;
}

std::pair<double, std::vector<std::vector<std::string>>>
TrojanMap::TravelingTrojan_2opt(std::vector<std::string> location_ids) {
  
  std::vector<std::vector<std::string>> order_list;
  std::vector<std::string> min_path = location_ids;
  min_path.push_back(location_ids[0]);
  double min_cost = CalculatePathLength(min_path);
  min_path.pop_back();
  bool improve = true;
  while (improve){
    improve = false;
    for(int i = 1; i< min_path.size()-2;i++){
      for(int j = i+1; j< min_path.size(); j++){
        std::vector<std::string> new_path;
        for(int m = 0; m < i; m++){
          new_path.push_back(min_path[m]);
        }
        for(int m = j; m >= i; m--){
          new_path.push_back(min_path[m]);
        }
        for(int m = j+1; m<min_path.size();m++){
          new_path.push_back(min_path[m]);
        }
        new_path.push_back(min_path[0]);
        double cost = CalculatePathLength(new_path);
        if(cost < min_cost){
          min_cost = cost;
          min_path = new_path;
          order_list.push_back(new_path);
          min_path.pop_back();
          improve = true;
        }


      }
    }
  }

  std::pair<double, std::vector<std::vector<std::string>>> records(min_cost,order_list);
  return records;
}

/**
 * Given CSV filename, it read and parse locations data from CSV file,
 * and return locations vector for topological sort problem.
 *
 * @param  {std::string} locations_filename     : locations_filename
 * @return {std::vector<std::string>}           : locations
 */
std::vector<std::string> TrojanMap::ReadLocationsFromCSVFile(
    std::string locations_filename) {
  std::vector<std::string> location_names_from_csv;
  std::ifstream inFile(locations_filename, std::ios::in);
  std::string lineStr;
  if(inFile.fail()) std::cout << "fail to read the location file." << std::endl;
  while(getline(inFile, lineStr)){
    // std::cout << lineStr << std::endl;
    std::stringstream ss(lineStr);
    std::string str;
    // std::vector <std::string> lineArray;
    while(getline(ss, str, ',')){
      location_names_from_csv.push_back(str);
    }
    
  }
  location_names_from_csv.erase(location_names_from_csv.begin());
  return location_names_from_csv;
}


/**
 * Given CSV filenames, it read and parse dependencise data from CSV file,
 * and return dependencies vector for topological sort problem.
 *
 * @param  {std::string} dependencies_filename     : dependencies_filename
 * @return {std::vector<std::vector<std::string>>} : dependencies
 */
std::vector<std::vector<std::string>> TrojanMap::ReadDependenciesFromCSVFile(
    std::string dependencies_filename) {
  std::vector<std::vector<std::string>> dependencies_from_csv;
  std::fstream fin;
  fin.open(dependencies_filename, std::ios::in);
  std::string lineStr;
  if(fin.fail()) std::cout << "fail to read the dependency file." << std::endl;
  while(getline(fin, lineStr)){
    // std::cout << lineStr << std::endl;
    std::stringstream ss(lineStr);
    std::string str;
    std::vector <std::string> lineArray;
    while(getline(ss, str, ',')){
      lineArray.push_back(str);
    }
  dependencies_from_csv.push_back(lineArray);
  }
  dependencies_from_csv.erase(dependencies_from_csv.begin());
  return dependencies_from_csv;
}

/**
 * DeliveringTrojan: Given a vector of location names, it should return a
 * sorting of nodes that satisfies the given dependencies. If there is no way to
 * do it, return a empty vector.
 *
 * @param  {std::vector<std::string>} locations                     : locations
 * @param  {std::vector<std::vector<std::string>>} dependencies     :
 * prerequisites
 * @return {std::vector<std::string>} results                       : results
 */
std::vector<std::string> TrojanMap::DeliveringTrojan(
    std::vector<std::string> &locations,
    std::vector<std::vector<std::string>> &dependencies) {
      std::map<std::string, int> visited;
      std::map<std::string, int> indegree;
      std::vector<std::string> result;

      for (auto dependency :dependencies){
        for (std::vector<std::string>::iterator iter = ++dependency.begin(); iter != dependency.end(); ++iter){
          if (indegree[*iter] ==0) indegree[*iter] = 1;
          else indegree[*iter] ++;

        }
      }
      std::priority_queue<std::pair<int, std::string>, 
                      std::vector<std::pair<int, std::string>>, 
                      std::greater<std::pair<int, std::string>>> q;
      for (auto i: locations){
        if (indegree[i] == 0){
          std::pair<int, std::string> p1 = {indegree[i], i};
          visited[i] = 1;
          q.push(p1);
        }
      }

      while (!q.empty()){
        std::string s1 = q.top().second;
        q.pop();
        result.push_back(s1);
        visited[s1] = 1;
        for (auto n:dependencies){
          for (std::vector<std::string>::iterator iter2 = n.begin(); iter2 != n.end(); ++iter2){
            if (s1.compare(*iter2) == 0){
              if (iter2 + 1 != n.end()){
                indegree[*(iter2 + 1)] --;
                // std::cout << *iter2 << std::endl;
              }
            }
          }
        }
        for (auto m: locations){
          if (indegree[m] == 0 && visited[m] == 0){
            std::pair<int, std::string> p2 = {indegree[m], m};
            q.push(p2);
          }
        }

      }
      return result;
}


/**
 * inSquare: Give a id return whether it is in square or not.
 *
 * @param  {std::string} id            : location id
 * @param  {std::vector<double>} square: four vertexes of the square area
 * @return {bool}                      : in square or not
 */
bool TrojanMap::inSquare(std::string id, std::vector<double> &square) {
  if(GetLon(id) < square[0] || GetLon(id) > square[1]){
    return false;
  }

  if(square[0] > square[1]){
    return false;
  } 
  if(GetLat(id) > square[2] || GetLat(id) < square[3]){
    return false;
  }
  if(square[3] > square[2]){
    return false;
  }
  return true;
}

/**
 * GetSubgraph: Give four vertexes of the square area, return a list of location
 * ids in the squares
 *
 * @param  {std::vector<double>} square         : four vertexes of the square
 * area
 * @return {std::vector<std::string>} subgraph  : list of location ids in the
 * square
 */
std::vector<std::string> TrojanMap::GetSubgraph(std::vector<double> &square) {
  // include all the nodes in subgraph
  std::vector<std::string> subgraph;
  std::unordered_map<std::string, Node>::iterator iter;
  for (iter = data.begin(); iter != data.end(); ++iter){
      if(inSquare(iter -> first, square)){
          subgraph.push_back(iter -> first);
      }
      
  }

  return subgraph;
}

/**
 * Cycle Detection: Given four points of the square-shape subgraph, return true
 * if there is a cycle path inside the square, false otherwise.
 *
 * @param {std::vector<std::string>} subgraph: list of location ids in the
 * square
 * @param {std::vector<double>} square: four vertexes of the square area
 * @return {bool}: whether there is a cycle or not
 */

bool IfDuHaveCycle(std::vector<std::pair<std::string, int>> &du){
  for(int i = 0; i < du.size(); i++){
    if(du[i].second == 0 || du[i].second == 1){
      return true;
    }
  }
  return false;
}

bool TrojanMap::CycleDetection(std::vector<std::string> &subgraph,
                               std::vector<double> &square) {

  //求所有点的度
  std::vector<std::pair<std::string, int>> du(subgraph.size());
  for(int i = 0; i < subgraph.size(); i++){
    std::string temp_id = subgraph[i];
    du[i].first = temp_id;
    du[i].second = 0;
    std::vector<std::string> temp_neibor = GetNeighborIDs(temp_id);
    
    for(int j =0; j< temp_neibor.size(); j++){
      std::vector<std::string>::iterator iter;
      iter = find(subgraph.begin(),subgraph.end(),temp_neibor[j]);
      if(iter != subgraph.end()){
        du[i].second += 1;
      }
    }
    // std::cout << du[i].first << std::endl;
    // std::cout << du[i].second << std::endl;

  }

  while(IfDuHaveCycle(du)){
    for(int i = 0; i < du.size(); i++){
      if(du[i].second == 0){
        du[i].second = -1;
        break;
      }
      if(du[i].second == 1){
        du[i].second = -1;
        std::vector<std::string> temp_neibor = GetNeighborIDs(du[i].first);
        for(int j =0; j< temp_neibor.size(); j++){
          std::vector<std::string>::iterator iter;
          iter = find(subgraph.begin(),subgraph.end(),temp_neibor[j]);
          if(iter != subgraph.end()){
            for(int k = 0; k < du.size(); k++){
              if(du[k].first.compare(*iter) == 0 && du[k].second >= 1){
                du[k].second -= 1;
              }
            }
          }
        }
      }
    }
  }

  for(int i = 0; i < du.size(); i++){
     //std::cout << du[i].second << std::endl;
    if(du[i].second > 1){
      return true;
    }
  }

  

  
  
  
  return false;
}

/**
 * FindNearby: Given a class name C, a location name L and a number r,
 * find all locations in class C on the map near L with the range of r and
 * return a vector of string ids
 *
 * @param {std::string} className: the name of the class
 * @param {std::string} locationName: the name of the location
 * @param {int} r: search radius
 * @param {int} k: search numbers
 * @return {std::vector<std::string>}: location name that meets the requirements
 */
std::vector<std::string> TrojanMap::FindNearby(std::string attributesName,
                                               std::string name, double r,
                                               int k) {
  std::string id = GetID(name);
  std::vector<std::string> res;
  std::priority_queue< std::pair<double, std::string>, 
                    std::vector<std::pair<double, std::string>>, 
                    std::greater<std::pair<double, std::string>>> q;

  for (auto n : data){
    if (n.second.attributes.count(attributesName) != 0 && CalculateDistance(n.first, id) <= r && n.second.id.compare(id) != 0){
      std::pair<double, std::string> p1 = {CalculateDistance(n.first, id), n.second.id};
      q.push(p1);
    }
  }
  if (q.size() >= k){
    for (int i = 0; i < k; i++){
      res.push_back(q.top().second);
      q.pop();
    }
  } else {
    while(!q.empty()){
      res.push_back(q.top().second);

      q.pop();
    }
  }
  return res;
}

/**
 * CreateGraphFromCSVFile: Read the map data from the csv file
 *
 */
void TrojanMap::CreateGraphFromCSVFile() {
  // Do not change this function
  std::fstream fin;
  fin.open("src/lib/data.csv", std::ios::in);
  std::string line, word;

  getline(fin, line);
  while (getline(fin, line)) {
    std::stringstream s(line);

    Node n;
    int count = 0;
    while (getline(s, word, ',')) {
      word.erase(std::remove(word.begin(), word.end(), '\''), word.end());
      word.erase(std::remove(word.begin(), word.end(), '"'), word.end());
      word.erase(std::remove(word.begin(), word.end(), '{'), word.end());
      word.erase(std::remove(word.begin(), word.end(), '}'), word.end());
      if (count == 0)
        n.id = word;
      else if (count == 1)
        n.lat = stod(word);
      else if (count == 2)
        n.lon = stod(word);
      else if (count == 3)
        n.name = word;
      else {
        word.erase(std::remove(word.begin(), word.end(), ' '), word.end());
        if (isalpha(word[0])) n.attributes.insert(word);
        if (isdigit(word[0])) n.neighbors.push_back(word);
      }
      count++;
    }
    data[n.id] = n;
  }
  fin.close();
}

// define the rule for search for a min value in map
bool cmp_value(const std::pair<std::string, double> left, const std::pair<std::string, double> right){
  return left.second < right.second;
}