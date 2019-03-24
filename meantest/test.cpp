#include "pch.h"
#include <vector>
#include <numeric>

float meanit(std::vector<float>::const_iterator begin, std::vector<float>::const_iterator end)
{
	float result = 0.0f;
	const size_t len = std::distance(begin, end);
	if (len > 0)
	{
		if (len == 1)
		{
			result = *begin;
		}
		else
		{
			const float sum = std::accumulate(begin, end, 0.0f);
			if ((sum < std::numeric_limits<float>::max()) &&
				(sum > -std::numeric_limits<float>::max()))
			{
				result = sum / len;
			}
			else
			{
				std::vector<float>::const_iterator mid = begin + (len / 2);
				const float weight1 = (float)std::distance(begin, mid) / (float)len;
				const float weight2 = (float)std::distance(mid, end) / (float)len;
				result =
					(weight1 * meanit(begin, mid)) +
					(weight2 * meanit(mid, end));
			}
		}
	}
	return result;
}

#if 0
float mean(const std::vector<float>& fvec)
{
	return meanit(flist.cbegin(), flist.cend());
}
#else
float mean(const std::vector<float>& fvec)
{
	double sum = 0.0;
	if (!fvec.empty())
	{
		for (auto f : fvec)
		{
			sum += f;
		}
		return sum / fvec.size();
	}
	return sum;
}
#endif

static const float big = std::numeric_limits<float>::max();
static const float small = std::numeric_limits<float>::min();

TEST(TestMean, EmptyIsZero) {
	ASSERT_FLOAT_EQ(0.0, mean({}));
}

TEST(TestMean, OneOneIsOne) {
	ASSERT_FLOAT_EQ(1.0, mean({ 1.0f }));
}

TEST(TestMean, TestTwo) {
	ASSERT_FLOAT_EQ(2.0, mean({ 1.0f, 3.0f }));
}

TEST(TestMean, TestTen) {
	ASSERT_FLOAT_EQ(50.0, mean({ 0.0, 10.0, 20.0, 30.0, 40.0, 60.0, 70.0, 80.0, 90.0, 100.0 }));
}

TEST(TestMean, OneBig) {
	ASSERT_FLOAT_EQ(big, mean({ big }));
}

TEST(TestMean, NegBig) {
	ASSERT_FLOAT_EQ(-big, mean({ -big }));
}

TEST(TestMean, TwoBig) {
	ASSERT_FLOAT_EQ(big, mean({ big, big }));
}

TEST(TestMean, TwoNegBig) {
	ASSERT_FLOAT_EQ(-big, mean({ -big, -big }));
}

TEST(TestMean, LotsaBigs) {
	std::vector<float> bigs(100, big);
	ASSERT_FLOAT_EQ(big, mean(bigs));
}

TEST(TestMean, LotsaNegBigs) {
	std::vector<float> negbigs(100, -big);
	ASSERT_FLOAT_EQ(-big, mean(negbigs));
}

TEST(TestMean, TwoBalancedBigs) {
	ASSERT_FLOAT_EQ(0.0f, mean({ big, -big }));
}

TEST(TestMean, ThreeUnbalancedBigs2A) {
	ASSERT_FLOAT_EQ(big * (1.0f / 3.0f), mean({ big, big, -big }));
}

TEST(TestMean, ThreeUnbalancedBigs2B) {
	ASSERT_FLOAT_EQ(big * (1.0f / 3.0f), mean({ big, -big, big }));
}

TEST(TestMean, ThreeUnbalancedBigs2C) {
	ASSERT_FLOAT_EQ(big * (1.0f / 3.0f), mean({ -big, big, big }));
}

TEST(TestMean, FourBalancedBigs) {
	ASSERT_FLOAT_EQ(0.0f, mean({ big, big, -big, -big }));
}

TEST(TestMean, SixBalancedBigs) {
	ASSERT_FLOAT_EQ(0.0f, mean({ big, -big, big, big, -big, -big }));
}

TEST(TestMean, OneSmall) {
	ASSERT_FLOAT_EQ(small, mean({ small }));
}

TEST(TestMean, NegSmall) {
	ASSERT_FLOAT_EQ(-small, mean({ -small }));
}

TEST(TestMean, TwoSmall) {
	ASSERT_FLOAT_EQ(small, mean({ small, small }));
}

TEST(TestMean, TwoNegSmall) {
	ASSERT_FLOAT_EQ(-small, mean({ -small, -small }));
}

TEST(TestMean, BalancedSmall) {
	ASSERT_FLOAT_EQ(0.0f, mean({ small, -small }));
}

TEST(TestMean, FourBalancedSmalls) {
	ASSERT_FLOAT_EQ(0.0f, mean({ small, small, -small, -small }));
}

#if 1
TEST(TestMean, FPPrecisionA) {
	std::vector<float> fvec{ -big, -small, small, 1.0, big};
	
	float fmean = mean(fvec);
	ASSERT_FLOAT_EQ(1.0f/fvec.size(), mean(fvec));
}

TEST(TestMean, FPPrecisionB) {
	std::vector<float> fvec{ big, -big, 1.0, small, -small};

	float fmean = mean(fvec);
	ASSERT_FLOAT_EQ(1.0f / fvec.size(), mean(fvec));
}
#endif
