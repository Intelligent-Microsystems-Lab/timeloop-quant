#include <boost/test/unit_test.hpp>

#include "isl-wrapper/ctx-manager.hpp"
#include "loop-analysis/spatial-analysis.hpp"

BOOST_AUTO_TEST_CASE(TestSimpleMulticastModel)
{
  using namespace analysis;

  /* Graphically:
   *  ------------ t
   *  ---- x     ---- x 
   * | [0] [1]   | [1] [2]
   * | [1] [2]   | [2] [3]
   * y           y        
   */
  auto fill = Fill(
    {Temporal(), Spatial(0), Spatial(1)},
    isl::map(
      GetIslCtx(),
      "{ [t, x, y] -> [t+x+y] : 0 <= x < 2 and 0 <= y < 2 and 0 <= t < 2}"
    )
  );

  auto occ = Occupancy(fill.dim_in_tags, fill.map);

  auto multicast_model = SimpleMulticastModel();

  auto info = multicast_model.Apply(fill, occ);

  auto& singles = info.compat_access_stats.at(std::make_pair(1, 1));
  BOOST_CHECK(singles.accesses == 2);
  BOOST_CHECK(singles.hops == 4);

  auto& doubles = info.compat_access_stats.at(std::make_pair(2, 1));
  BOOST_CHECK(doubles.accesses == 4);
  BOOST_CHECK(doubles.hops == 3.5);
}

BOOST_AUTO_TEST_CASE(TestSimpleMulticastModel_FlattenedCoords)
{
  using namespace analysis;

  /* Graphically:
   *  ---- x
   * | [(0, 0)] [(0, 1)] [(1, 0)] [(1, 1)]
   * | [(1, 0)] [(1, 1)] [(2, 0)] [(2, 1)]
   * y
   */
  auto fill = Fill(
    {Temporal(), Spatial(0), Spatial(1)},
    isl::map(
      GetIslCtx(),
      "{ [t, x, y] -> [o1, o2] : 0 <= x < 4 and 0 <= y < 2 and 0 <= t < 1"
      "  and o1 = y + floor(x/2) and (o2 - x) mod 2 = 0"
      "  and 0 <= o1 < 3 and 0 <= o2 < 2 }"
    )
  );

  auto occ = Occupancy(fill.dim_in_tags, fill.map);

  auto multicast_model = SimpleMulticastModel();

  auto info = multicast_model.Apply(fill, occ);

  auto& singles = info.compat_access_stats.at(std::make_pair(1, 1));
  BOOST_CHECK(singles.accesses == 4);

  auto& doubles = info.compat_access_stats.at(std::make_pair(2, 1));
  BOOST_CHECK(doubles.accesses == 4);
}