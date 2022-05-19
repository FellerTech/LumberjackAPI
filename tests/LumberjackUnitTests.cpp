/**
 * \brief Unit tests functions for Lumberjack
 */
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <lumberjack.hpp>

//Macro for custom messages
#define GTEST_COUT std::cerr << "[ MSG      ] [ INFO ]"


using ::testing::HasSubstr;

/////////////////////////////////////////////
//Test version 
/////////////////////////////////////////////
TEST( VersionTest, delay) {
  std::string targetVersion = "0.0.1.-1";
  lumberjack::Lumberjack lj;
  std::string version = lj.getVersion();

  EXPECT_THAT( version, HasSubstr(targetVersion));
  EXPECT_NE( version, targetVersion );
}

/////////////////////////////////////////////
//Test getTimestamp
/////////////////////////////////////////////
TEST( Lumberjack, getTimestamp) {
  lumberjack::Lumberjack lj;
  double time = lj.getTimestamp();
  GTEST_COUT << "Time: "<<time << "\n";
}


/////////////////////////////////////////////
// main function
/////////////////////////////////////////////
int main(int argc, char **argv)
{
  GTEST_COUT << "valid GTEST_COUT macro\n";
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

