#pragma once
#include <SDL3/SDL.h>
#include <algorithm>
#include <cmath>

namespace neu::math {
	constexpr float pi = 3.1415926535897932384626433832795f;
	constexpr float twoPi = 2 * pi;
	constexpr float halfPi = pi * 0.5f;

	/// <summary>
	/// Converts an angle from radians to degrees.
	/// </summary>
	/// <param name="rad">The angle in radians to convert.</param>
	/// <returns>The equivalent angle in degrees.</returns>
	constexpr float RadToDeg(float rad) { return rad * (180 / pi); }

	/// <summary>
	/// Converts an angle from degrees to radians.
	/// </summary>
	/// <param name="deg">The angle in degrees to convert.</param>
	/// <returns>The angle in radians.</returns>
	constexpr float DegToRad(float deg) { return deg * (pi / 180); }

	/// <summary>
	/// Wraps an integer value into the range [min, max), cycling around if the value is outside the range.
	/// </summary>
	/// <param name="value">The integer value to wrap.</param>
	/// <param name="min">The inclusive lower bound of the range.</param>
	/// <param name="max">The exclusive upper bound of the range.</param>
	/// <returns>The wrapped integer value within the range [min, max).</returns>
	constexpr int Wrap(int value, int min, int max) {
		int range = max - min;				// calculate range of wrap
		int result = (value - min) % range; // shift value so range starts at 0
		if (result < 0) result += range;	// wrap forward if result is negative (value < min)
						
		return min + result; // shift the result back to [min, max) range
	}
		
	/// <summary>
	/// Wraps a floating-point value into the range [min, max).
	/// </summary>
	/// <param name="value">The value to be wrapped.</param>
	/// <param name="min">The inclusive lower bound of the range.</param>
	/// <param name="max">The exclusive upper bound of the range.</param>
	/// <returns>The wrapped value within the range [min, max).</returns>
	inline float Wrap(float value, float min, float max) {
		float range = max - min;						// calculate range of wrap
		float result = std::fmodf(value - min, range);	// shift value so range starts at 0
		if (result < 0) result += range;				// wrap forward if result is negative (value < min)

		return min + result; // shift the result back to [min, max) range
	}

	/// <summary>
	/// Determines the sign of a number.
	/// </summary>
	/// <typeparam name="T">The type of the value to check. Must support comparison with zero and conversion from integer literals.</typeparam>
	/// <param name="v">The value whose sign is to be determined.</param>
	/// <returns>Returns -1 if the value is negative, 1 if positive, and 0 if zero.</returns>
	template<typename T>
	inline T Sign(T v) {
		return (v < 0) ? (T)-1 : (v > 0) ? (T)1 : (T)0;
	}

	//inline float Remap(float min1, float max1, float min2, float max2, float v) {
	//	float t = (v - min1) / (max1 - min1);
	//	return std::lerp(min2, max2, t);
	//}

	/// <summary>
	/// Remaps a value from one range to another.
	/// <summary>
	/// @tparam T Floating-point type (float or double)
	/// @param min1 Minimum value of the source range
	/// @param max1 Maximum value of the source range
	/// @param min2 Minimum value of the target range
	/// @param max2 Maximum value of the target range
	/// @param v Value to remap from source range [min1, max1]
	/// @return Remapped value in target range [min2, max2]
	/// 
	/// @note Values outside [min1, max1] will extrapolate beyond [min2, max2]
	/// 
	/// Example:
	///   Remap(0.0f, 10.0f, 0.0f, 100.0f, 5.0f) returns 50.0f
	///   Remap(0.0f, 10.0f, 0.0f, 100.0f, 15.0f) returns 150.0f (extrapolated)
	template<std::floating_point T>
	inline T Remap(T min1, T max1, T min2, T max2, T v) {
		ASSERT_MSG(max1 != min1, "Source range cannot be zero");

		// Normalize v to [0, 1] within the source range
		T t = (v - min1) / (max1 - min1);

		// Linearly interpolate to the target range
		return std::lerp(min2, max2, t);
	}

	using std::min;
	using std::max;
	using std::clamp;
	using std::sqrt;
	using std::sqrtf;
	using std::acos;
	using std::acosf;
	using std::sin;
	using std::sinf;
	using std::cos;
	using std::cosf;
	using std::atan2;
	using std::atan2f;
	using std::abs;
	using std::fabs;
}
