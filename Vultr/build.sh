premake5 gmake
make config=debug -j10

echo ""
echo ""
echo ""
echo ""
echo ""
echo "------------------RUNNING TESTS------------------------"
echo ""
echo ""
echo ""
echo ""
./build/tests/VultrTests --gtest_color=yes
