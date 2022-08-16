#include "gtest/gtest.h"
#include "src/lib/trojanmap.h"

TEST(TrojanMapTest, Autocomplete)
{
  TrojanMap m;
  // Test the simple case
  auto names = m.Autocomplete("Chi");
  std::unordered_set<std::string> gt = {"Chick-fil-A", "Chipotle", "Chinese Street Food"}; // groundtruth for "Ch"
  int success = 0;
  for (auto &n : names)
  {
    EXPECT_EQ(gt.count(n) > 0, true) << n + " is not in gt.";
    if (gt.count(n) > 0)
    {
      success++;
    }
  }
  EXPECT_EQ(success, gt.size());
  names = m.Autocomplete("chi");
  success = 0;
  for (auto &n : names)
  {
    EXPECT_EQ(gt.count(n) > 0, true) << n + " is not in gt.";
    if (gt.count(n) > 0)
    {
      success++;
    }
  }
  EXPECT_EQ(success, gt.size());
  // Test the lower and upper case
  names = m.Autocomplete("cHi");
  success = 0;
  for (auto &n : names)
  {
    EXPECT_EQ(gt.count(n) > 0, true) << n + " is not in gt.";
    if (gt.count(n) > 0)
    {
      success++;
    }
  }
  EXPECT_EQ(success, gt.size());
  // Test the upper case
  names = m.Autocomplete("CHI");
  success = 0;
  for (auto &n : names)
  {
    EXPECT_EQ(gt.count(n) > 0, true) << n + " is not in gt.";
    if (gt.count(n) > 0)
    {
      success++;
    }
  }
  EXPECT_EQ(success, gt.size());
}



TEST(TrojanMapTest, FindPosition)
{
  TrojanMap m;

  // Test Chick-fil-A
  auto position = m.GetPosition("Chick-fil-A");
  std::pair<double, double> gt1(34.0167334, -118.2825307); // groundtruth for "Chick-fil-A"
  EXPECT_EQ(position, gt1);
  // Test Ralphs
  position = m.GetPosition("Ralphs");
  std::pair<double, double> gt2(34.0317653, -118.2908339); // groundtruth for "Ralphs"
  EXPECT_EQ(position, gt2);
  // Test Target
  position = m.GetPosition("Target");
  std::pair<double, double> gt3(34.0257016, -118.2843512); // groundtruth for "Target"
  EXPECT_EQ(position, gt3);
  // Test Unknown
  position = m.GetPosition("XXX");
  std::pair<double, double> gt4(-1, -1);
  EXPECT_EQ(position, gt4);
}

// // Test CalculateEditDistance function
// TEST(TrojanMapTest, CalculateEditDistance) {sd
//   EXPECT_EQ(m.FindClosestName("Targeety"), "Target");
// }

TEST(TrojanMapTest, CalculateEditDistance)
{
  TrojanMap m;
  EXPECT_EQ(m.CalculateEditDistance("horse", "ros"), 3);
  EXPECT_EQ(m.CalculateEditDistance("intention", "execution"), 5);
  EXPECT_EQ(m.CalculateEditDistance("Ralphs", "Raiphs"), 1);
}

TEST(TrojanMapTest, FindClosestName)
{
  TrojanMap m;
  EXPECT_EQ(m.FindClosestName("Rolphs"), "Ralphs");
  EXPECT_EQ(m.FindClosestName("Targeety"), "Target");
  EXPECT_EQ(m.FindClosestName("Starbacks"), "Starbucks");
  EXPECT_EQ(m.FindClosestName(""), "");
}

TEST(TrojanMapTest, GetAllLocationsFromCategory)
{
  TrojanMap m;
  std::string input1 = "";
  std::pair<double, double> exp1(-1,-1);
  auto temp = m.GetAllLocationsFromCategory(input1);
  auto act1 = temp[0];
  EXPECT_EQ(act1,exp1);

  std::string input2 = "bank";
  std::pair<double, double> exp2(34.0223,-118.280);
  auto temp2 = m.GetAllLocationsFromCategory(input2);
  auto act2 = temp3[0];
  EXPECT_EQ(act2,exp2);

  std::string input3 = "library";
  std::pair<double, double> exp3(34.0025,-118.256);
  auto temp3 = m.GetAllLocationsFromCategory(input3);
  auto act3 = temp2[0]; 
  EXPECT_EQ(act3,exp3);
}

TEST(TrojanMapTest, CalculateShortestPath_Dijkstra1)
{
  TrojanMap m;
  // Test from Ralphs to Chick-fil-A
  auto path = m.CalculateShortestPath_Dijkstra("Ralphs", "Chick-fil-A");
  std::vector<std::string> gt{
      "2578244375", "4380040154", "4380040153", "4380040152", "4380040148", "6818427920", "6818427919",
      "6818427918", "6818427892", "6818427898", "6818427917", "6818427916", "7232024780", "6813416145",
      "6813416154", "6813416153", "6813416152", "6813416151", "6813416155", "6808069740", "6816193785",
      "6816193786", "123152294", "4015203136", "4015203134", "4015203133", "21098539", "6389467809",
      "4015203132", "3195897587", "4015203129", "4015203127", "6352865690", "6813379589", "6813379483",
      "3402887081", "6814958394", "3402887080", "602606656", "4872897515", "4399697589", "6814958391",
      "123209598", "6787673296", "122728406", "6807762271", "4399697304", "4399697302", "5231967015",
      "1862347583", "3233702827", "4540763379", "6819179753", "6820935900", "6820935901", "6813379556",
      "6820935898", "1781230450", "1781230449", "4015405542", "4015405543", "1837212104", "1837212107",
      "2753199985", "6820935907", "1837212100", "4015372458", "6813411588", "1837212101", "6814916516",
      "6814916515", "6820935910", "4547476733"}; // Expected path
  // Print the path lengths
  std::cout << "My path length: " << m.CalculatePathLength(path) << "miles" << std::endl;
  std::cout << "GT path length: " << m.CalculatePathLength(gt) << "miles" << std::endl;
  EXPECT_EQ(path, gt);

  // Reverse the input from Ralphs to Chick-fil-A
  path = m.CalculateShortestPath_Dijkstra("Chick-fil-A", "Ralphs");
  std::reverse(gt.begin(), gt.end()); // Reverse the path

  // Print the path lengths
  std::cout << "My path length: " << m.CalculatePathLength(path) << "miles" << std::endl;
  std::cout << "GT path length: " << m.CalculatePathLength(gt) << "miles" << std::endl;
  EXPECT_EQ(path, gt);
}

TEST(TrojanMapTest, CalculateShortestPath_Dijkstra2)
{
  TrojanMap m;

  // Test from Ralphs to Target
  auto path = m.CalculateShortestPath_Dijkstra("Ralphs", "Target");
  std::vector<std::string> gt{
      "2578244375", "4380040154", "4380040158", "4380040167", "6805802087", "8410938469",
      "6813416131", "7645318201", "6813416130", "6813416129", "123318563", "452688940",
      "6816193777", "123408705", "6816193774", "452688933", "452688931", "123230412",
      "6816193770", "6787470576", "4015442011", "6816193692", "6816193693", "6816193694",
      "4015377691", "544693739", "6816193696", "6804883323", "6807937309", "6807937306",
      "6816193698", "4015377690", "4015377689", "122814447", "6813416159", "6813405266",
      "4015372488", "4015372487", "6813405229", "122719216", "6813405232", "4015372486",
      "7071032399", "4015372485", "6813379479", "6813379584", "6814769289", "5237417650"}; // Expected path
  // Print the path lengths
  std::cout << "My path length: " << m.CalculatePathLength(path) << "miles" << std::endl;
  std::cout << "GT path length: " << m.CalculatePathLength(gt) << "miles" << std::endl;
  EXPECT_EQ(path, gt);

  // Reverse the input from Ralphs to Target
  path = m.CalculateShortestPath_Dijkstra("Target", "Ralphs");
  std::reverse(gt.begin(), gt.end()); // Reverse the path

  // Print the path lengths
  std::cout << "My path length: " << m.CalculatePathLength(path) << "miles" << std::endl;
  std::cout << "GT path length: " << m.CalculatePathLength(gt) << "miles" << std::endl;
  EXPECT_EQ(path, gt);
}

TEST(TrojanMapTest, CalculateShortestPath_Dijkstra3)
{
  TrojanMap m;

  // Test from Leavey Popeyes to Target
  auto path = m.CalculateShortestPath_Dijkstra("Popeyes", "Target");
  std::vector<std::string> gt{
      // Expected path
      "5695236164", "4835551075", "3403035506", "5680945537", "6814916550", "3398621871",
      "6813565323", "3403035498", "6816193705", "5556118325", "5680945525", "7693467754",
      "3403035499", "3403035500", "7863689394", "7863689395", "4835551084", "2613117900",
      "3403034586", "4835551093", "2613117862", "3403034590", "2613117890", "2613117915",
      "4835551105", "6813565334", "2613117867", "6787803635", "6807905595", "6813379406",
      "2613117879", "3398578893", "3398574892", "6813565290", "3398578898", "6813379398",
      "6813379505", "6813379519", "4399698005", "3398578900", "4399698015", "6813379425",
      "3398578901", "6813379479", "6813379584", "6814769289", "5237417650"};

  // Print the path lengths
  std::cout << "My path length: " << m.CalculatePathLength(path) << "miles" << std::endl;
  std::cout << "GT path length: " << m.CalculatePathLength(gt) << "miles" << std::endl;
  EXPECT_EQ(path, gt);

  // Reverse the input from Target to Popeyes
  path = m.CalculateShortestPath_Dijkstra("Target", "Popeyes");
  std::reverse(gt.begin(), gt.end()); // Reverse the path

  // Print the path lengths
  std::cout << "My path length: " << m.CalculatePathLength(path) << "miles" << std::endl;
  std::cout << "GT path length: " << m.CalculatePathLength(gt) << "miles" << std::endl;
  EXPECT_EQ(path, gt);
}


TEST(TrojanMapTest, CalculateShortestPath_Bellman_Ford1)
{
  TrojanMap m;

  // Test from Ralphs to Chick-fil-A
  auto path = m.CalculateShortestPath_Bellman_Ford("Ralphs", "Chick-fil-A");
  std::vector<std::string> gt{
      "2578244375", "4380040154", "4380040153", "4380040152", "4380040148", "6818427920", "6818427919",
      "6818427918", "6818427892", "6818427898", "6818427917", "6818427916", "7232024780", "6813416145",
      "6813416154", "6813416153", "6813416152", "6813416151", "6813416155", "6808069740", "6816193785",
      "6816193786", "123152294", "4015203136", "4015203134", "4015203133", "21098539", "6389467809",
      "4015203132", "3195897587", "4015203129", "4015203127", "6352865690", "6813379589", "6813379483",
      "3402887081", "6814958394", "3402887080", "602606656", "4872897515", "4399697589", "6814958391",
      "123209598", "6787673296", "122728406", "6807762271", "4399697304", "4399697302", "5231967015",
      "1862347583", "3233702827", "4540763379", "6819179753", "6820935900", "6820935901", "6813379556",
      "6820935898", "1781230450", "1781230449", "4015405542", "4015405543", "1837212104", "1837212107",
      "2753199985", "6820935907", "1837212100", "4015372458", "6813411588", "1837212101", "6814916516",
      "6814916515", "6820935910", "4547476733"}; // Expected path
  // Print the path lengths
  std::cout << "My path length: " << m.CalculatePathLength(path) << "miles" << std::endl;
  std::cout << "GT path length: " << m.CalculatePathLength(gt) << "miles" << std::endl;
  EXPECT_EQ(path, gt);

  // Reverse the input from Ralphs to Chick-fil-A
  path = m.CalculateShortestPath_Bellman_Ford("Chick-fil-A", "Ralphs");
  std::reverse(gt.begin(), gt.end()); // Reverse the path

  // Print the path lengths
  std::cout << "My path length: " << m.CalculatePathLength(path) << "miles" << std::endl;
  std::cout << "GT path length: " << m.CalculatePathLength(gt) << "miles" << std::endl;
  EXPECT_EQ(path, gt);
}


TEST(TrojanMapTest, CalculateShortestPath_Bellman_Ford2)
{
  TrojanMap m;

  // Test from Ralphs to Target
  auto path = m.CalculateShortestPath_Bellman_Ford("Ralphs", "Target");
  std::vector<std::string> gt{
      "2578244375", "4380040154", "4380040158", "4380040167", "6805802087", "8410938469",
      "6813416131", "7645318201", "6813416130", "6813416129", "123318563", "452688940",
      "6816193777", "123408705", "6816193774", "452688933", "452688931", "123230412",
      "6816193770", "6787470576", "4015442011", "6816193692", "6816193693", "6816193694",
      "4015377691", "544693739", "6816193696", "6804883323", "6807937309", "6807937306",
      "6816193698", "4015377690", "4015377689", "122814447", "6813416159", "6813405266",
      "4015372488", "4015372487", "6813405229", "122719216", "6813405232", "4015372486",
      "7071032399", "4015372485", "6813379479", "6813379584", "6814769289", "5237417650"}; // Expected path
  // Print the path lengths
  std::cout << "My path length: " << m.CalculatePathLength(path) << "miles" << std::endl;
  std::cout << "GT path length: " << m.CalculatePathLength(gt) << "miles" << std::endl;
  EXPECT_EQ(path, gt);

  // Reverse the input from Ralphs to Target
  path = m.CalculateShortestPath_Bellman_Ford("Target", "Ralphs");
  std::reverse(gt.begin(), gt.end()); // Reverse the path

  // Print the path lengths
  std::cout << "My path length: " << m.CalculatePathLength(path) << "miles" << std::endl;
  std::cout << "GT path length: " << m.CalculatePathLength(gt) << "miles" << std::endl;
  EXPECT_EQ(path, gt);
}

TEST(TrojanMapTest, CalculateShortestPath_Bellman_Ford3)
{
  TrojanMap m;

  // Test from Popeyes to Target
  auto path = m.CalculateShortestPath_Bellman_Ford("Popeyes", "Target");
  std::vector<std::string> gt{
      // Expected path
      "5695236164", "4835551075", "3403035506", "5680945537", "6814916550", "3398621871",
      "6813565323", "3403035498", "6816193705", "5556118325", "5680945525", "7693467754",
      "3403035499", "3403035500", "7863689394", "7863689395", "4835551084", "2613117900",
      "3403034586", "4835551093", "2613117862", "3403034590", "2613117890", "2613117915",
      "4835551105", "6813565334", "2613117867", "6787803635", "6807905595", "6813379406",
      "2613117879", "3398578893", "3398574892", "6813565290", "3398578898", "6813379398",
      "6813379505", "6813379519", "4399698005", "3398578900", "4399698015", "6813379425",
      "3398578901", "6813379479", "6813379584", "6814769289", "5237417650"};

  // Print the path lengths
  std::cout << "My path length: " << m.CalculatePathLength(path) << "miles" << std::endl;
  std::cout << "GT path length: " << m.CalculatePathLength(gt) << "miles" << std::endl;
  EXPECT_EQ(path, gt);

  // Reverse the input from Target to Popeyes
  path = m.CalculateShortestPath_Bellman_Ford("Target", "Popeyes");
  std::reverse(gt.begin(), gt.end()); // Reverse the path

  // Print the path lengths
  std::cout << "My path length: " << m.CalculatePathLength(path) << "miles" << std::endl;
  std::cout << "GT path length: " << m.CalculatePathLength(gt) << "miles" << std::endl;
  EXPECT_EQ(path, gt);
}

TEST(TrojanMapTest, TopologicalSort1)
{
  TrojanMap m;

  // test case1
  std::vector<std::string> location_names1 = {"Ralphs", "Chick-fil-A", "KFC", "Bank of America", "Arco"};
  std::vector<std::vector<std::string>> dependencies1 = {{"Ralphs", "KFC"}, {"Ralphs", "Chick-fil-A"}, {"KFC", "Chick-fil-A"}, 
                                                          {"Arco", "Ralphs"}, {"Bank of America", "Arco"}, {"Arco", "KFC"},
                                                          {"Bank of America", "KFC"}};
  auto result1 = m.DeliveringTrojan(location_names1, dependencies1);
  std::vector<std::string> gt1 = {"Bank of America", "Arco", "Ralphs", "KFC", "Chick-fil-A"};
  EXPECT_EQ(result1, gt1);

  // test case2
  std::vector<std::string> location_names2 = {"Popeyes 1", "Burger King 1", "Arco"};
  std::vector<std::vector<std::string>> dependencies2 = {{"Arco", "Popeyes 1"}, {"Popeyes 1", "Burger King 1"}, {"Arco", "Burger King 1"}};
  auto result2 = m.DeliveringTrojan(location_names2, dependencies2);
  std::vector<std::string> gt2 = {"Arco", "Popeyes 1", "Burger King 1"};
  EXPECT_EQ(result2, gt2);

  // test case3
  std::vector<std::string> location_names3 = {"Ralphs", "Bank of America", "KFC"};
  std::vector<std::vector<std::string>> dependencies3 = {{"Ralphs", "KFC"}, {"Bank of America", "Ralphs"}, {"Bank of America", "KFC"}};
  auto result3 = m.DeliveringTrojan(location_names3, dependencies3);
  std::vector<std::string> gt3 = {"Bank of America", "Ralphs", "KFC"};
  EXPECT_EQ(result3, gt3);
}

TEST(TrojanMapTest, CycleDetection)
{
  TrojanMap m;

  // Test case 1
  std::vector<double> square1 = {-118.299, -118.264, 34.032, 34.011};
  auto sub1 = m.GetSubgraph(square1);
  bool result1 = m.CycleDetection(sub1, square1);
  EXPECT_EQ(result1, true);

  // Test case 2
  std::vector<double> square2 = {-118.294, -118.265, 34.031, 34.012};
  auto sub2 = m.GetSubgraph(square2);
  bool result2 = m.CycleDetection(sub2, square2);
  EXPECT_EQ(result2, true);

  // Test case 3
  std::vector<double> square3 = {-118.290, -118.289, 34.030, 34.020};
  auto sub3 = m.GetSubgraph(square3);
  bool result3 = m.CycleDetection(sub3, square3);
  EXPECT_EQ(result3, false);

}

TEST(TrojanMapTest, TSP1)
{
  TrojanMap m;

  std::vector<std::string> input{"6819019976", "6820935923", "122702233", "8566227783", "8566227656", "6816180153", "1873055993", "7771782316"}; // Input location ids
  auto result = m.TravelingTrojan_Brute_force(input);
  std::cout << "My path length: " << result.first << "miles" << std::endl;                                                                                  // Print the result path lengths
  std::vector<std::string> gt{"6819019976", "1873055993", "8566227656", "122702233", "8566227783", "6816180153", "7771782316", "6820935923", "6819019976"}; // Expected order
  std::cout << "GT path length: " << m.CalculatePathLength(gt) << "miles" << std::endl;                                                                     // Print the gt path lengths
  bool flag = false;
  if (gt == result.second[result.second.size() - 1]) // clockwise
    flag = true;
  std::reverse(gt.begin(), gt.end()); // Reverse the expected order because the counterclockwise result is also correct
  if (gt == result.second[result.second.size() - 1])
    flag = true;

  EXPECT_EQ(flag, true);
}

TEST(TrojanMapTest, TSP2)
{
  TrojanMap m;

  std::vector<std::string> input{"6819019976", "6820935923", "122702233", "8566227783", "8566227656", "6816180153", "1873055993", "7771782316"}; // Input location ids
  auto result = m.TravelingTrojan_Backtracking(input);
  std::cout << "My path length: " << result.first << "miles" << std::endl;                                                                                  // Print the result path lengths
  std::vector<std::string> gt{"6819019976", "1873055993", "8566227656", "122702233", "8566227783", "6816180153", "7771782316", "6820935923", "6819019976"}; // Expected order
  std::cout << "GT path length: " << m.CalculatePathLength(gt) << "miles" << std::endl;                                                                     // Print the gt path lengths
  bool flag = false;
  if (gt == result.second[result.second.size() - 1]) // clockwise
    flag = true;
  std::reverse(gt.begin(), gt.end()); // Reverse the expected order because the counterclockwise result is also correct
  if (gt == result.second[result.second.size() - 1])
    flag = true;

  EXPECT_EQ(flag, true);
}

TEST(TrojanMapTest, TSP3)
{
  TrojanMap m;

  std::vector<std::string> input{"6819019976", "6820935923", "122702233", "8566227783", "8566227656", "6816180153", "1873055993", "7771782316"}; // Input location ids
  auto result = m.TravelingTrojan_2opt(input);
  std::cout << "My path length: " << result.first << "miles" << std::endl;                                                                                  // Print the result path lengths
  std::vector<std::string> gt{"6819019976", "1873055993", "8566227656", "122702233", "8566227783", "6816180153", "7771782316", "6820935923", "6819019976"}; // Expected order
  std::cout << "GT path length: " << m.CalculatePathLength(gt) << "miles" << std::endl;                                                                     // Print the gt path lengths
  bool flag = false;
  if (gt == result.second[result.second.size() - 1]) // clockwise
    flag = true;
  std::reverse(gt.begin(), gt.end()); // Reverse the expected order because the counterclockwise result is also correct
  if (gt == result.second[result.second.size() - 1])
    flag = true;

  EXPECT_EQ(flag, true);
}

TEST(TrojanMapTest, TSP4)
{
  TrojanMap m;

  std::vector<std::string> input{"6819019976", "6820935923", "122702233", "8566227783", "8566227656", "6816180153", "1873055993", "7771782316"}; // Input location ids
  auto result = m.TravelingTrojan_Genetic(input);
  std::cout << "My path length: " << result.first << "miles" << std::endl;                                                                                  // Print the result path lengths
  std::vector<std::string> gt{"6819019976", "1873055993", "8566227656", "122702233", "8566227783", "6816180153", "7771782316", "6820935923", "6819019976"}; // Expected order
  std::cout << "GT path length: " << m.CalculatePathLength(gt) << "miles" << std::endl;                                                                     // Print the gt path lengths
  bool flag = false;
  if (gt == result.second[result.second.size() - 1]) // clockwise
    flag = true;
  std::reverse(gt.begin(), gt.end()); // Reverse the expected order because the counterclockwise result is also correct
  if (gt == result.second[result.second.size() - 1])
    flag = true;

  EXPECT_EQ(flag, true);
}

TEST(TrojanMapTest, FindNearby)
{
  // Test case 1
  TrojanMap m_1;
  std::string attributesName_1 = "artwork";
  std::string name_1 = "Trader Joes";
  double r_1 = 20;
  int k_1 = 5;
  auto result_1 = m_1.FindNearby(attributesName_1, name_1, r_1, k_1);
  std::vector<std::string> anticipate_1 = {"5237417648", "4536993737", "2305853438", "4399693645", "6813379578"};
  EXPECT_EQ(result_1, anticipate_1);

  // Test case 2
  TrojanMap m_2;
  std::string attributesName_2 = "bar";
  std::string name_2 = "Trader Joes";
  double r_2 = 10;
  int k_2 = 5;
  auto result_2 = m_2.FindNearby(attributesName_2, name_2, r_2, k_2);
  std::vector<std::string> anticipate_2 = {"6045035789", "5567714035", "6045038065"};
  EXPECT_EQ(result_2, anticipate_2);

    // Test case 3
  TrojanMap m_3;
  std::string attributesName_3 = "bank";
  std::string name_3 = "Trader Joes";
  double r_3 = 10;
  int k_3 = 10;
  auto result_3 = m_3.FindNearby(attributesName_3, name_3, r_3, k_3);
  std::vector<std::string> anticipate_3 = {"9591449465", "5237417651", "9591449441"};
  EXPECT_EQ(result_3, anticipate_3);
}
