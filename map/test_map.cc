#include "map.hpp"
#include <gtest/gtest.h>
#include <string>
#include <set>
#include <memory.h>
#include <boost/random.hpp>
TEST(map, construct){
  nanahan::Map<int, int> m;
  nanahan::Map<uint64_t, std::string> n;
  nanahan::Map<std::string, std::string > o;
  nanahan::Map<bool, bool> p;
}

TEST(map, is_empty){
  nanahan::Map<int, int> m;
  ASSERT_TRUE(m.empty());
  for (int i = 0; i<1000; ++i){
    ASSERT_EQ(m.find(i), m.end());
  }
}


TEST(map, insert_one){
  nanahan::Map<int, int> m;
  {
    std::pair<nanahan::Map<int, int>::iterator, bool> result =
    m.insert(std::make_pair(8, 9));
    ASSERT_TRUE(result.second);
    ASSERT_EQ(m.find(8)->first, 8);
    ASSERT_EQ(m.find(8)->second, 9);
  }
  {
    std::pair<nanahan::Map<int, int>::iterator, bool> result =
      m.insert(std::make_pair(8, 9));
    ASSERT_FALSE(result.second);
  }
  ASSERT_EQ(m.find(2), m.end());
}

TEST(map, insert_many){
  nanahan::Map<int, int> m;
  for (int i = 1; i<8; i += 2){
    m.insert(std::make_pair(i, i*i));
  }
  for (int i = 0; i<8; i += 2){
    ASSERT_EQ(m.find(i), m.end());
    ASSERT_NE(m.find(i+1), m.end());
    ASSERT_EQ(m.find(i+1)->first, i+1);
    ASSERT_EQ(m.find(i+1)->second, (i+1)*(i+1));
  }
}


TEST(map, insert_eight){
  nanahan::Map<int, int> m;
  for (int i = 0; i<8; ++i){
    m.insert(std::make_pair(i, i*i));
  }
  //m.dump();
  for (int i = 0; i<8; ++i){
    ASSERT_NE(m.find(i), m.end());
    ASSERT_EQ(m.find(i)->first, i);
    ASSERT_EQ(m.find(i)->second, i*i);
  }
}

TEST(map, insert_eight_string){
  nanahan::Map<std::string, int> m;
  char orig[] = "hogedsfdsaa";
  for (int i = 0; i<8; ++i){
    orig[2] = i + '0';
    orig[4] = i + '0';
    m.insert(std::make_pair(std::string(orig), i*i));
    //m.dump();
  }
  //m.dump();
  for (int i = 0; i<8; ++i){
    orig[2] = i + '0';
    orig[4] = i + '0';
    nanahan::Map<std::string, int>::iterator it = m.find(std::string(orig));
    ASSERT_NE(it, m.end());
    ASSERT_NE(m.find(std::string(orig)), m.end());
    ASSERT_EQ(m.find(std::string(orig))->first, std::string(orig));
    ASSERT_EQ(m.find(std::string(orig))->second, i*i);
  }
}

TEST(map, bucket_extend){
  nanahan::Map<int, int> m;
  const int size = 100000;
  for (int i = 0; i<size; ++i){
    m.insert(std::make_pair(i, i*i));
    /*
      for (int j= 0; j < i; ++j){
      if(m.find(j) == m.end()){
        std::cout << "i = " << i <<
          " not found :" << j <<std::endl;
      }
      ASSERT_NE(m.find(j), m.end());
    }
    */
  }
  //m.dump();
  for (int i = 0; i<size; ++i){
    ASSERT_NE(m.find(i), m.end());
    ASSERT_EQ(m.find(i)->first, i);
    ASSERT_EQ(m.find(i)->second, i*i);
  }
}

TEST(map, erase){
  nanahan::Map<int, int> m;
  std::pair<nanahan::Map<int,int>::iterator, bool> result =
    m.insert(std::make_pair(8, 9));
  ASSERT_NE(m.find(8), m.end());
  ASSERT_EQ(m.find(8)->first, 8);
  ASSERT_EQ(m.find(8)->second, 9);
  ASSERT_EQ(m.find(2), m.end());
  //result.first.dump();
  m.erase(result.first);
  ASSERT_EQ(m.find(8), m.end());
}


TEST(map, erase_many){
  nanahan::Map<int, int> m;
  const int size = 1000;
  for (int i = 0; i<size; ++i){
    m.insert(std::make_pair(i, i*i));
  }
  for (int i = 0; i<size; ++i){
    m.erase(m.find(i));
  }
  for (int i = 0; i<size; ++i){
    ASSERT_EQ(m.find(i), m.end());
  }
}

TEST(map, erase_half){
  nanahan::Map<int, int> m;
  const int size = 10000;
  for (int i = 0; i<size; ++i){
    m.insert(std::make_pair(i, i*i));
  }
  for (int i = 0; i<size; i += 2){
    m.erase(m.find(i));
  }
  for (int i = 0; i<size; i += 2){
    ASSERT_EQ(m.find(i), m.end());
    ASSERT_NE(m.find(i+1), m.end());
    ASSERT_EQ(m.find(i+1)->first, i+1);
    ASSERT_EQ(m.find(i+1)->second, (i+1)*(i+1));
  }
}

TEST(map, erase_half_string){
  nanahan::Map<std::string, uint32_t> m;
  const uint32_t size = 200;
  char buff[8];
  memcpy(buff, "buff", 4);
  for (uint32_t i = 0; i<size; ++i){
    memcpy(&buff[4], &i, 4);
    m.insert(std::make_pair(std::string(buff), i));
  }
  //m.dump();
  for (uint32_t i = 0; i<size; i += 2){
    memcpy(&buff[4], &i, 4);
    if(m.erase(m.find(std::string(buff))) == m.end()){
      std::cout << "failed to erase " <<i << std::endl;
      //m.dump();
      ASSERT_FALSE(true);
    }
  }
  //m.dump();
  for (uint32_t i = 0; i<size; i += 2){
    memcpy(&buff[4], &i, 4);
    ASSERT_EQ(m.find(std::string(buff)), m.end());

    uint32_t j = i+1;
    memcpy(&buff[4], &j, 4);
    ASSERT_NE(m.find(std::string(buff)), m.end());
    ASSERT_EQ(m.find(std::string(buff))->first, std::string(buff));
    ASSERT_EQ(m.find(std::string(buff))->second, (i+1));
  }
}

TEST(map, size){
  nanahan::Map<int, int> m;
  const size_t size = 10000;
  for (size_t i = 0; i<size; ++i){
    ASSERT_EQ(m.size(), i);
    m.insert(std::make_pair(i, i*i));
    ASSERT_EQ(m.size(), i+1);
  }
}

TEST(map, not_empty){
  nanahan::Map<int, int> m;
  ASSERT_TRUE(m.empty());
  m.insert(std::make_pair(2, 3));
  ASSERT_FALSE(m.empty());
}

size_t slow_bitcount(uint64_t bits){
  size_t result = 0;
  while(bits){
    result += bits & 1;
    bits >>= 1;
  }
  return result;
}
size_t slow_bitcount(uint32_t bits){
  size_t result = 0;
  while(bits){
    result += bits & 1;
    bits >>= 1;
  }
  return result;
}

TEST(bitcount, 32){
  boost::mt19937 gen( static_cast<unsigned long>(0) );
  boost::uniform_smallint<> dst( 0, 1 << 31 );
  boost::variate_generator<boost::mt19937&, boost::uniform_smallint<>
                           > rand( gen, dst );
  for(int i = 0; i < 100000; ++i){
    uint32_t r = rand();
    ASSERT_EQ(slow_bitcount(r), nanahan::detail::bitcount(r));
  }
}
TEST(bitcount, 64){
  boost::mt19937 gen( static_cast<uint32_t>(0) );
  boost::uniform_smallint<> dst( 0, 1 << 31 );
  boost::variate_generator<boost::mt19937&, boost::uniform_smallint<>
                           > rand( gen, dst );
  for(int i = 0; i < 10000; ++i){
    uint64_t r = rand() * rand();
    ASSERT_EQ(slow_bitcount(r), nanahan::detail::bitcount(r));
  }
}
TEST(map, random_int){
  const int size = 4000;
  const int tries = 1000;
  for(int j = 0; j < tries; ++j){
    nanahan::Map<int, int> m;
    {
      boost::mt19937 gen( static_cast<unsigned long>(j) );
      boost::uniform_smallint<> dst( 0, 1 << 30 );
      boost::variate_generator<boost::mt19937&, boost::uniform_smallint<>
                               > rand( gen, dst );
      for(int i = 0; i < size; ++i){
        m.insert(std::make_pair(rand(), i));
      }
    }
    /*
    std::cout << "insert done" << std::endl;
    m.dump();
    //*/
    {
      boost::mt19937 gen( static_cast<unsigned long>(j) );
      boost::uniform_smallint<> dst( 0, 1 << 30 );
      boost::variate_generator<boost::mt19937&, boost::uniform_smallint<>
                               > rand( gen, dst );
      for(int i = 0; i < size; ++i){
        //std::cout << i;
        ASSERT_NE(m.find(rand()), m.end());
        //std::cout << ".";
      }
    }
  }
}
TEST(operator, copy){
  nanahan::Map<int, int> m, o;
  const int size = 10000;
  for (int i = 0; i<size; ++i){
    m.insert(std::make_pair(i, i*i));
  }
  o = m;
  for (int i = 0; i<size; i += 2){
    m.erase(m.find(i));
  }
  for (int i = 0; i<size; i += 2){
    ASSERT_EQ(m.find(i), m.end());
    ASSERT_NE(m.find(i+1), m.end());
    ASSERT_EQ(m.find(i+1)->first, i+1);
    ASSERT_EQ(m.find(i+1)->second, (i+1)*(i+1));

    ASSERT_NE(o.find(i), o.end());
    ASSERT_EQ(o.find(i)->first, i);
    ASSERT_EQ(o.find(i)->second, i*i);
    ASSERT_NE(o.find(i+1), o.end());
    ASSERT_EQ(o.find(i+1)->first, i+1);
    ASSERT_EQ(o.find(i+1)->second, (i+1)*(i+1));
  }
}

TEST(copy, construct){
  nanahan::Map<int, int> m;
  const int size = 10000;
  for (int i = 0; i<size; ++i){
    m.insert(std::make_pair(i, i*i));
  }
  nanahan::Map<int, int> o(m);
  for (int i = 0; i<size; i += 2){
    m.erase(m.find(i));
  }
  for (int i = 0; i<size; i += 2){
    ASSERT_EQ(m.find(i), m.end());
    ASSERT_NE(m.find(i+1), m.end());
    ASSERT_EQ(m.find(i+1)->first, i+1);
    ASSERT_EQ(m.find(i+1)->second, (i+1)*(i+1));

    ASSERT_NE(o.find(i), o.end());
    ASSERT_EQ(o.find(i)->first, i);
    ASSERT_EQ(o.find(i)->second, i*i);
    ASSERT_NE(o.find(i+1), o.end());
    ASSERT_EQ(o.find(i+1)->first, i+1);
    ASSERT_EQ(o.find(i+1)->second, (i+1)*(i+1));
  }
}

TEST(operator, equal){
  nanahan::Map<int, int> m,n,o;
  const int size = 10000;

  boost::mt19937 gen( static_cast<unsigned long>(0));
  boost::uniform_smallint<> dst( 0, 1 << 30 );
  boost::variate_generator<boost::mt19937&, boost::uniform_smallint<>
                           > rand( gen, dst );
  for (int i = 0; i < size; ++i){
    const int r = rand();
    m.insert(std::make_pair(r, i));
    n.insert(std::make_pair(r, i));
    o.insert(std::make_pair(r, i+1));
  }
  EXPECT_TRUE(m==n);
  EXPECT_FALSE(m==o);
  EXPECT_FALSE(o==n);
}