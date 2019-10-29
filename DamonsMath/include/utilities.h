
#ifndef _UTILITILES_H
#define _UTILITILES_H

#include <stdint.h>
#include <assert.h>
#include <algorithm>
#include <cmath>
/// @cond DamonsMath_INTERNAL
#define DAMONSMATH_VECTOR_OPERATION(OP) DAMONSMATH_UNROLLED_LOOP(i, d, OP)
/// @endcond

/// @cond DamonsMath_INTERNAL
#define DAMONSMATH_VECTOR_OPERATOR(OP)           \
  {                                              \
    DVector<T, d> result;                         \
    DAMONSMATH_VECTOR_OPERATION(result[i] = (OP)); \
    return result;                               \
  }
/// @endcond

/// @cond DamonsMath_INTERNAL
#define DAMONSMATH_VECTOR_SELF_OPERATOR(OP) \
  {                                         \
    DAMONSMATH_VECTOR_OPERATION(OP);        \
    return *this;                           \
  }
/// @endcond

/// @cond DamonsMath_INTERNAL
/// Unroll an loop up to 4 iterations, where iterator is the identifier
/// used in each operation (e.g "i"), number_of_iterations is a constant which
/// specifies the number of times to perform the operation and "operation" is
/// the statement to execute for each iteration of the loop (e.g data[i] = v).
/// it means do loop  operation 
#define DAMONSMATH_UNROLLED_LOOP(iterator, number_of_iterations, operation) \
  {                                                                     \
    const int iterator = 0;                                             \
    { operation; }                                                      \
    if ((number_of_iterations) > 1) {                                   \
      const int iterator = 1;                                           \
      { operation; }                                                    \
      if ((number_of_iterations) > 2) {                                 \
        const int iterator = 2;                                         \
        { operation; }                                                  \
        if ((number_of_iterations) > 3) {                               \
          const int iterator = 3;                                       \
          { operation; }                                                \
          if ((number_of_iterations) > 4) {                             \
            for (int iterator = 4; iterator < (number_of_iterations);   \
                 ++iterator) {                                          \
              operation;                                                \
            }                                                           \
          }                                                             \
        }                                                               \
      }                                                                 \
    }                                                                   \
  }
/// @endcond

#define DEplision 1.0e-5

namespace DMath {
	/// @brief Clamp x within [lower, upper].
	/// @anchor mathfu_Clamp
	///
	/// @note Results are undefined if lower > upper.
	///
	/// @param x Value to clamp.
	/// @param lower Lower value of the range.
	/// @param upper Upper value of the range.
	/// @returns Clamped value.
	template <class T>
	T Clamp(const T &x, const T &lower, const T &upper) {
		return std::max<T>(lower, std::min<T>(x, upper));
	}

	/// @brief Check if val is within [range_start..range_end).
	/// @anchor mathfu_InRange
	///
	/// @param val Value to be tested.
	/// @param range_start Starting point of the range (inclusive).
	/// @param range_end Ending point of the range (non-inclusive).
	/// @return Bool indicating success.
	///
	/// @tparam T Type of values to test.
	template <class T>
	bool InRange(T val, T range_start, T range_end) {
		return val >= range_start && val < range_end;
	}

	/// @brief Round a value up to the nearest power of 2.
	///
	/// @param x Value to round up.
	/// @returns Value rounded up to the nearest power of 2.
	template <class T>
	T RoundUpToPowerOf2(T x) {
		return static_cast<T>(
			pow(static_cast<T>(2), ceil(log(x) / log(static_cast<T>(2)))));
	}

	/// @brief Specialized version of RoundUpToPowerOf2 for int32_t.
	template <>
	inline int32_t RoundUpToPowerOf2<>(int32_t x) {
		x--;
		x |= x >> 1;
		x |= x >> 2;
		x |= x >> 4;
		x |= x >> 8;
		x |= x >> 16;
		x++;
		return x;
	}

	/// @brief  Generate a random value of type T.
	/// @anchor mathfu_Random
	///
	/// This method generates a random value of type T, greater than or equal to
	/// 0.0 and less than 1.0.
	///
	/// This function uses the standard C library function rand() from math.h to
	/// generate the random number.
	///
	/// @returns Random number greater than or equal to 0.0 and less than 1.0.
	///
	/// @see RandomRange()
	/// @see RandomInRange()
	template <class T>
	inline T Random() {
		return static_cast<T>(rand()) / static_cast<T>(RAND_MAX);
	}

	/// @cond MATHFU_INTERNAL
	template <>
	inline float Random() {
		return static_cast<float>(rand() >> 8) /
			(static_cast<float>((RAND_MAX >> 8) + 1));
	}
	/// @endcond

	/// @cond MATHFU_INTERNAL
	template <>
	inline double Random() {
		return static_cast<double>(rand()) / (static_cast<double>(RAND_MAX + 1LL));
	}

	template <class T>
	inline T RandomInRange(T range_start, T range_end) {
		return Lerp(range_start, range_end, Random<T>());
	}

	template <class T, class T2>
	T Lerp(const T &range_start, const T &range_end, const T2 &percent) {
		const T2 one_minus_percent = static_cast<T2>(1.0) - percent;
		return range_start * one_minus_percent + range_end * percent;
	}

	/// @brief Linearly interpolate between range_start and range_end, based on
	/// percent.
	/// @anchor mathfu_Lerp2
	///
	/// @param range_start Start of the range.
	/// @param range_end End of the range.
	/// @param percent Value between 0.0 and 1.0 used to interpolate between
	/// range_start and range_end.  Where a value of 0.0 results in a return
	/// value of range_start and 1.0 results in a return value of range_end.
	/// @return Value between range_start and range_end.
	///
	/// @tparam T Type of the range to interpolate over.
	template <class T>
	T Lerp(const T &range_start, const T &range_end, const T &percent) {
		return Lerp<T, T>(range_start, range_end, percent);
	}



#define DAMONS_ALIGNMENT 16

	/// @brief Allocate an aligned block of memory.
	/// @anchor mathfu_AllocateAligned
	///
	/// This function allocates a block of memory aligned to MATHFU_ALIGNMENT
	/// bytes.
	///
	/// @param n Size of memory to allocate.
	/// @return Pointer to aligned block of allocated memory or NULL if
	/// allocation failed.
	inline void *AllocateAligned(size_t n) {
#if defined(_MSC_VER) && _MSC_VER >= 1900  // MSVC 2015
		return _aligned_malloc(n, DAMONS_ALIGNMENT);
#else
		// We need to allocate extra bytes to guarantee alignment,
		// and to store the pointer to the original buffer.
		uint8_t *buf = reinterpret_cast<uint8_t *>(malloc(n + DAMONS_ALIGNMENT));
		if (!buf) return NULL;
		// Align to next higher multiple of MATHFU_ALIGNMENT.
		uint8_t *aligned_buf = reinterpret_cast<uint8_t *>(
			(reinterpret_cast<size_t>(buf) + DAMONS_ALIGNMENT) &
			~(DAMONS_ALIGNMENT - 1));
		// Write out original buffer pointer before aligned buffer.
		// The assert will fail if the allocator granularity is less than the pointer
		// size, or if MATHFU_ALIGNMENT doesn't fit two pointers.
		assert(static_cast<size_t>(aligned_buf - buf) > sizeof(void *));
		*(reinterpret_cast<uint8_t **>(aligned_buf) - 1) = buf;
		return aligned_buf;
#endif  // defined(_MSC_VER) && _MSC_VER >= 1900 // MSVC 2015
	}

	/// @brief Deallocate a block of memory allocated with AllocateAligned().
	/// @anchor mathfu_FreeAligned
	///
	/// @param p Pointer to memory to deallocate.
	inline void FreeAligned(void *p) {
#if defined(_MSC_VER) && _MSC_VER >= 1900  // MSVC 2015
		_aligned_free(p);
#else
		if (p == NULL) return;
		free(*(reinterpret_cast<uint8_t **>(p) - 1));
#endif  // defined(_MSC_VER) && _MSC_VER >= 1900 // MSVC 2015
	}

	/// @def MATHFU_DEFINE_CLASS_SIMD_AWARE_NEW_DELETE
	/// @brief Macro which defines the new and delete for MathFu classes.
#define DAMONS_DEFINE_CLASS_SIMD_AWARE_NEW_DELETE                       \
  static void *operator new(std::size_t n) {                            \
    return DMath::AllocateAligned(n);                                  \
  }                                                                     \
  static void *operator new[](std::size_t n) {                          \
    return DMath::AllocateAligned(n);                                  \
  }                                                                     \
  static void *operator new(std::size_t /*n*/, void *p) { return p; }   \
  static void *operator new[](std::size_t /*n*/, void *p) { return p; } \
  static void operator delete(void *p) { DMath::FreeAligned(p); }      \
  static void operator delete[](void *p) { DMath::FreeAligned(p); }    \
  static void operator delete(void * /*p*/, void * /*place*/) {}        \
  static void operator delete[](void * /*p*/, void * /*place*/) {}

};/// namespace DMath end

#endif // !_UTILITILES_H

