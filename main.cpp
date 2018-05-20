/*
* Some parts of the wrapper and utility functions are the property of others, and must not be copied or reused.
* All methods and utility functions used for implementin the AStar class are covered by the MIT license and are owned by Devendra Rai
* who can be reached at devenrai@gmail.com
*/
#include <fstream>
#include <string>
#include <vector>
#include <exception>
#include <chrono>
#include <iostream>
#include <iomanip>
#include "visualizer/include/visualizer.h"
#include "Node.hpp"
#include "SearchPath.hpp"
#include <thread>
#include <future>

#ifdef _MSC_VER
static const char *PATH_SEP = "\\";
#else
static const char *PATH_SEP = "/";
#endif

#define DEBUG

#include <cassert>

// Externally defined function: Unit tests
extern void performUnitTest();



// Bits used in the overrides image bytes
enum OverrideFlags
{
  OF_RIVER_MARSH = 0x10,
  OF_INLAND = 0x20,
  OF_WATER_BASIN = 0x40
};

// Some constants
enum
{
  IMAGE_DIM = 2048, // Width and height of the elevation and overrides image
  ROVER_X = 159,
  ROVER_Y = 1520,
  BACHELOR_X = 1303,
  BACHELOR_Y = 85,
  WEDDING_X = 1577,
  WEDDING_Y = 1294
};

std::ifstream::pos_type fileSize(const std::string &filename)
{
  std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
  if (!in.good())
  {
    throw std::exception();
  }
  return in.tellg();
}

std::vector<uint8_t> loadFile(const std::string &filename,
                              size_t expectedFileSize)
{
  size_t fsize = fileSize(filename);
  if (fsize != expectedFileSize)
  {
    throw std::exception();
  }
  std::vector<uint8_t> data(fsize);
  std::ifstream ifile(filename, std::ifstream::binary);
  if (!ifile.good())
  {
    throw std::exception();
  }
  ifile.read((char *)&data[0], fsize);
  return data;
}

// Used for marking the locations on the bitmap file.
bool donut(int x, int y, int x1, int y1)
{
  int dx = x - x1;
  int dy = y - y1;
  int r2 = dx * dx + dy * dy;
  return r2 >= 150 && r2 <= 400;
}

int main(int argc, char **argv)
{

  performUnitTest();

  const size_t expectedFileSize = IMAGE_DIM * IMAGE_DIM;
  // Address assets relative to application location
  std::string anchor = std::string(".") + PATH_SEP;
  std::string pname = argv[0];
  auto lastpos = pname.find_last_of("/\\");
  if (lastpos != std::string::npos)
  {
    anchor = pname.substr(0, lastpos) + PATH_SEP;
  }

  // Elevation data and Override information is loaded in parallel.

  // Elevation data provides the height of each location of the map. 
  // The elevation data is used for estimating the cost of travel between two end points.
  // It is assumed in the solution that the cost of movement gets higher when the vehicle is required to ascend.
  auto returnFromThread1 = std::async(std::launch::async, loadFile, anchor + "../assets" + PATH_SEP + "elevation.data",
                                      expectedFileSize);

  // Overrides provide information about geographical features which prevent the ego vehicle from travelling at a given location
  // on the map. The overrides map is a bit-encoded stream, with each byte indicating the geographical features at a location l.
  // where l = (width of map * rows down from top) + (columns from left). The origin of the map is taken to be at the top left corner.
  // the interpretation of the byte associated with location l is give by OverrideFlags defined above.
  auto returnFromThread2 = std::async(std::launch::async, loadFile, anchor + "../assets" + PATH_SEP + "overrides.data",
                                      expectedFileSize);
  returnFromThread1.wait();
  returnFromThread2.wait();
  std::vector<uint8_t> elevation = returnFromThread1.get();
  std::vector<uint8_t> overrides = returnFromThread2.get();

  /*
   * ------------------------ Start of the Solution ------------------------------
   */

  // define the freedome of movement of the rover as (deltaX, deltaY) pair
  std::vector<std::pair<int, int>> moves = {
      std::make_pair(0, 1),   // go down
      std::make_pair(0, -1),  // go up
      std::make_pair(1, 1),   // go down-and-right
      std::make_pair(-1, 1),  // go down-and-left
      std::make_pair(-1, -1), // go up and left
      std::make_pair(1, -1)   // go up-and-right
  };

  // Where the Rover is initially at
  Node rover(159, 1520);

  // The location of the bachelor
  Node bachelor(1303, 85);

  // The location of the wedding
  Node Wedding(1577, 1294);

  // The masks indicating places where the Rover cannot go
  uint8_t mask = static_cast<uint8_t>(OF_WATER_BASIN | OF_RIVER_MARSH);

  // create variables to get back results
  std::vector<Node> pathFromRoverToBachelor;
  std::vector<Node> pathFromBachelorToWedding;
  float totalDistanceTravelled_from_RoverToBachelor;
  float totalDistanceTravelled_from_BachelorToWedding;
  float totalTimeOfTravel_from_RoverToBachelor;
  float totalTimeOfTravel_from_from_BachelorToWedding;

  // --------------------------------- End of Wrapper ------------------------------------
  // --------------------------- Start of AStar Solution ---------------------------------


  float weight; // metric to scale up the heuristic -- helps speed up path finding.
  // start the timer
  std::chrono::time_point<std::chrono::steady_clock> start =
      std::chrono::steady_clock::now();

  // Find the path from Rover to Bachelor
  weight = 1.0f;
  auto returnFromThread3 = std::async(std::launch::async, searchPath, rover, bachelor, IMAGE_DIM, IMAGE_DIM,
                                      std::ref(elevation), std::ref(overrides), mask, weight, std::ref(moves), std::ref(pathFromRoverToBachelor),
                                      std::ref(totalDistanceTravelled_from_RoverToBachelor), std::ref(totalTimeOfTravel_from_RoverToBachelor));

  // Find the path from the bachelor to the wedding
  weight = 1.0f;
  auto returnFromThread4 = std::async(std::launch::async, searchPath, bachelor, Wedding, IMAGE_DIM, IMAGE_DIM,
                                      std::ref(elevation), std::ref(overrides), mask, weight, std::ref(moves), std::ref(pathFromBachelorToWedding),
                                      std::ref(totalDistanceTravelled_from_BachelorToWedding), std::ref(totalTimeOfTravel_from_from_BachelorToWedding));

  returnFromThread3.wait();
  returnFromThread4.wait();

  // stop the timer
  std::chrono::time_point<std::chrono::steady_clock> end =
      std::chrono::steady_clock::now();

  // compute the elapsed time
  std::chrono::milliseconds diff = std::chrono::duration_cast<
      std::chrono::milliseconds>(end - start);

  // flash header
  std::cout << std::right << std::setw(30) << "Summary" << std::endl;
  std::cout << std::left << std::setw(40) << "Trip";
  std::cout << std::left << std::setw(10) << "Distance";
  std::cout << std::left << std::setw(10) << "Time";
  std::cout << std::endl;

  // information: Travel from Rover to Bachelor
  std::cout << std::left << std::setw(40) << "Rover to Bachelor";
  std::cout << std::left << std::setw(10) << std::setprecision(2) << std::fixed << totalDistanceTravelled_from_RoverToBachelor;
  std::cout << std::left << std::setw(10) << std::setprecision(2) << std::fixed << totalTimeOfTravel_from_RoverToBachelor;
  std::cout << std::endl;

  // information: Travel from Bachelor to Wedding
  std::cout << std::left << std::setw(40) << "Bachelor to Wedding";
  std::cout << std::left << std::setw(10) << std::setprecision(2) << std::fixed << totalDistanceTravelled_from_BachelorToWedding;
  std::cout << std::left << std::setw(10) << std::setprecision(2) << std::fixed << totalTimeOfTravel_from_from_BachelorToWedding;
  std::cout << std::endl;
  std::cout << std::endl;

  std::cout << "Total Wallclock Time Required for Computation: " << diff.count() / 1000.0f << " s." << std::endl;

  // prepare the data for quicker plotting
  uint8_t plotMatrix[IMAGE_DIM][IMAGE_DIM];
  for (unsigned x = 0; x < IMAGE_DIM; ++x)
  {
    for (unsigned y = 0; y < IMAGE_DIM; ++y)
    {
      plotMatrix[x][y] = 0;
    }
  }
#ifdef DEBUG
  std::ofstream outfile;
  outfile.open("trace.txt", std::ofstream::out);

#endif
  for (Node n : pathFromRoverToBachelor)
  {
    plotMatrix[n.get_X()][n.get_Y()] = 1;
    outfile << n.get_X() << " , " << n.get_Y() << std::endl;
  }

  for (Node n : pathFromBachelorToWedding)
  {
    plotMatrix[n.get_X()][n.get_Y()] = 1;
    outfile << n.get_X() << " , " << n.get_Y() << std::endl;
  }

  outfile.flush();
  outfile.close();

  /*
   * -------------------------- End of Solution -----------------------------------
   * -------------------------- Start of Wrapper ----------------------------------
   */

  std::ofstream of;
  of.open("output.bmp");

  visualizer::writeBMP(
      of, &elevation[0], IMAGE_DIM, IMAGE_DIM,
      [&](size_t x, size_t y, uint8_t elevation) {


        // Route
        if (plotMatrix[x][y] == 1)
        {
          return uint8_t(visualizer::IPV_PATH);
        }

        // Marks interesting positions on the map
        if (donut(x, y, ROVER_X, ROVER_Y) ||
            donut(x, y, BACHELOR_X, BACHELOR_Y) ||
            donut(x, y, WEDDING_X, WEDDING_Y))
        {
          return uint8_t(visualizer::IPV_PATH);
        }

        // Signifies water
        if ((overrides[y * IMAGE_DIM + x] & (OF_WATER_BASIN | OF_RIVER_MARSH)) ||
            elevation == 0)
        {
          return uint8_t(visualizer::IPV_WATER);
        }

        // Signifies normal ground color
        if (elevation < visualizer::IPV_ELEVATION_BEGIN)
        {
          elevation = visualizer::IPV_ELEVATION_BEGIN;
        }
        return elevation;
      });

  of.flush();
  of.close();

  std::cout << "Done. " << std::endl;

#if __APPLE__
  auto res = system("open pic.bmp");
  (void)res;
#endif
  return 0;
}
