/**
 * \brief Unit tests functions for Lumberjack
 */
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <lumberjack.hpp>

using ::testing::HasSubstr;

//Test timer accuracy
TEST( VersionTest, delay) {
  std::string targetVersion = "0.0.1.-1";
  lumberjack::Lumberjack lj;
  std::string version = lj.getVersion();

  EXPECT_THAT( version, HasSubstr(targetVersion));
  EXPECT_NE( version, targetVersion );

}


int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

