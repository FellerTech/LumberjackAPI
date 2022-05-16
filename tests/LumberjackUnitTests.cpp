/**
 * \brief Unit tests functions for Lumberjack
 */
#include <gtest/gtest.h>
#include <lumberjack.hpp>

//Test timer accuracy
TEST( VersionTest, delay) {
  lumberjack::Lumberjack lj;
  std::string version = lj.getVersion();

  EXPECT_EQ( version, "0.0.1");
  //EXPECT_TRUE( IsBetweenInclusive( result, delay - range, delay + range ));
}


int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

