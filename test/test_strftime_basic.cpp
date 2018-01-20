#define _CRT_SECURE_NO_WARNINGS
#include <gtest/gtest.h>
#include "../src/strftime.h"

#include "fixtures.h"

namespace {

	/*
	 * What happens if somebody puts obviously invalid values in the arguments?
	 */

	class ArgumentSanity : public SimpleFixture<4> {
	};

	TEST_F(ArgumentSanity, BufferIsNullptr) {
		EXPECT_EQ(0, pbl_strftime(nullptr, BufferSize, "abc", curTm));
	}

	TEST_F(ArgumentSanity, SizeIsNull) {
		EXPECT_EQ(0, pbl_strftime(buffer, 0, "abc", curTm));
	}

	TEST_F(ArgumentSanity, FormatIsNullptr) {
		EXPECT_EQ(0, pbl_strftime(buffer, BufferSize, nullptr, curTm));
	}

	TEST_F(ArgumentSanity, TmIsNullptr) {
		EXPECT_EQ(0, pbl_strftime(buffer, BufferSize, "abc", nullptr));
	}

	/*
	 * Can it copy text?
	 */

	class CopyText : public MemoryGuardFixture<4> {
	};

	TEST_F(CopyText, NoText) {
		expCharsWritten = pbl_strftime(buffer, BufferSize, "", &curTm);
		EXPECT_MEMGUARD_RESULT("", 0, "");
	}

	TEST_F(CopyText, SomeText) {
		EXPECT_MEMGUARD_RESULT("abc", 3, "abc");
	}

	TEST_F(CopyText, TooMuchText) {
		EXPECT_MEMGUARD_RESULT("abcdefgh", 0, "abc");
	}

	TEST_F(CopyText, EscapedPercent) {
		EXPECT_MEMGUARD_RESULT("%%", 1, "%");
	}

	TEST_F(CopyText, EscapedSinglePercent) {
		EXPECT_MEMGUARD_RESULT("%", 1, "%");
	}

	TEST_F(CopyText, EscapedPercentMixed) {
		EXPECT_MEMGUARD_RESULT("a%%b", 3, "a%b");
	}

	TEST_F(CopyText, EscapedPercentAtEnd) {
		EXPECT_MEMGUARD_RESULT("ab%%", 3, "ab%");
	}

	TEST_F(CopyText, EscapedPercentOverLength) {
		EXPECT_MEMGUARD_RESULT("abc%%", 3, "abc");
	}

	/*
	 * Can it print the simple numeric values
	 */
	class SimpleNumerics : public MemoryGuardFixture<7> {
		// use this buffer size to fit the year mixed in with text (and terminator)
	};

	// a small shortcut for all tooless/toomuch tests, also tests padding
#define TEST_SIMPLENUMERICS_LIMITS(name, var, format, min, max, padding) \
	TEST_F(SimpleNumerics, name##Min) { \
		curTm.var = min; \
		EXPECT_MEMGUARD_RESULT(format, strlen(padding #min), padding #min); \
	} \
	TEST_F(SimpleNumerics, name##max) { \
		curTm.var = max; \
		const char* expected = (max > 9 ? padding #max : #max); \
		EXPECT_MEMGUARD_RESULT(format, strlen(expected), expected); \
	} // remember: pbl_strftime behaviour is undefined when providing illegal values

	// Century
	TEST_F(SimpleNumerics, Century) {
		curTm.tm_year = 1234 - 1900;
		EXPECT_MEMGUARD_RESULT("%C", 2, "12");
	}

	TEST_F(SimpleNumerics, CenturyPadded) {
		curTm.tm_year = 234 - 1900;
		EXPECT_MEMGUARD_RESULT("%C", 2, "02");
	}

	TEST_F(SimpleNumerics, CenturyMixed) { // these are tested only once for simple numerics
		curTm.tm_year = 1234 - 1900;
		EXPECT_MEMGUARD_RESULT("a%Cb", 4, "a12b");
	}

	TEST_F(SimpleNumerics, CenturyAtEnd) {
		curTm.tm_year = 1234 - 1900;
		EXPECT_MEMGUARD_RESULT("abcd%C", 6, "abcd12");
	}

	TEST_F(SimpleNumerics, CenturyOverLength) {
		curTm.tm_year = 234 - 1900;
		EXPECT_MEMGUARD_RESULT("abcde%C", 6, "abcde0");
	}

	// Day of month
	TEST_F(SimpleNumerics, DayOfMonth) {
		curTm.tm_mday = 14;
		EXPECT_MEMGUARD_RESULT("%d", 2, "14");
	}

	TEST_SIMPLENUMERICS_LIMITS(DayOfMonth, tm_mday, "%d", 1, 31, "0");

	// Hour 24-hour style, zero based and padding
	TEST_F(SimpleNumerics, Hour24) {
		curTm.tm_hour = 12;
		EXPECT_MEMGUARD_RESULT("%H", 2, "12");
	}

	TEST_SIMPLENUMERICS_LIMITS(Hour24, tm_hour, "%H", 0, 23, "0");

	// Hour 24-hour style, space padding
	TEST_F(SimpleNumerics, Hour24SpacePadding) {
		curTm.tm_hour = 3;
		EXPECT_MEMGUARD_RESULT("%k", 2, " 3");
	}

	// Hour 12-hour style, one based, zero padding
	TEST_F(SimpleNumerics, Hour12) {
		curTm.tm_hour = 12;
		EXPECT_MEMGUARD_RESULT("%I", 2, "12");
	}

	TEST_F(SimpleNumerics, Hour12Padded) {
		curTm.tm_hour = 1;
		EXPECT_MEMGUARD_RESULT("%I", 2, "01");
	}

	TEST_F(SimpleNumerics, Hour12Zero) {
		curTm.tm_hour = 0;
		EXPECT_MEMGUARD_RESULT("%I", 2, "12");
	}

	TEST_F(SimpleNumerics, Hour12Overflowed) {
		curTm.tm_hour = 23;
		EXPECT_MEMGUARD_RESULT("%I", 2, "11");
	}

	// Hour 12-hour style, space padding
	TEST_F(SimpleNumerics, Hour12SpacePadding) {
		curTm.tm_hour = 2;
		EXPECT_MEMGUARD_RESULT("%l", 2, " 3");
	}

	// Month number, one based
	TEST_F(SimpleNumerics, Month) {
		curTm.tm_mon = 3;
		EXPECT_MEMGUARD_RESULT("%m", 2, "04");
	}

	TEST_F(SimpleNumerics, MonthMin) {
		curTm.tm_mon = 0;
		EXPECT_MEMGUARD_RESULT("%m", 2, "01");
	}

	TEST_F(SimpleNumerics, MonthMax) {
		curTm.tm_mon = 11;
		EXPECT_MEMGUARD_RESULT("%m", 2, "12");
	}

	// Minutes
	TEST_F(SimpleNumerics, Minutes) {
		curTm.tm_min = 34;
		EXPECT_MEMGUARD_RESULT("%M", 2, "34");
	}

	TEST_SIMPLENUMERICS_LIMITS(Minutes, tm_min, "%M", 0, 59, "0");

	// Seconds
	TEST_F(SimpleNumerics, Seconds) {
		curTm.tm_sec = 42;
		EXPECT_MEMGUARD_RESULT("%S", 2, "42");
	}

	TEST_SIMPLENUMERICS_LIMITS(Seconds, tm_sec, "%S", 0, 61, "0");

	// Weekday as number, one based from Monday
	TEST_F(SimpleNumerics, WeekdayOne) {
		curTm.tm_wday = 3;
		EXPECT_MEMGUARD_RESULT("%u", 1, "4");
	}

	TEST_F(SimpleNumerics, WeekdayOneMin) {
		curTm.tm_wday = 0;
		EXPECT_MEMGUARD_RESULT("%u", 1, "7");
	}

	TEST_F(SimpleNumerics, WeekdayOneMax) {
		curTm.tm_wday = 6;
		EXPECT_MEMGUARD_RESULT("%u", 1, "1");
	}

	// Weekday as number, zero based from Sunday
	TEST_F(SimpleNumerics, WeekdayZero) {
		curTm.tm_wday = 3;
		EXPECT_MEMGUARD_RESULT("%w", 1, "3");
	}

	TEST_SIMPLENUMERICS_LIMITS(WeekdayZero, tm_wday, "%w", 0, 6, "");

	// Last two digits of year
	TEST_F(SimpleNumerics, YearCentPhase) {
		curTm.tm_year = 2134 - 1900;
		EXPECT_MEMGUARD_RESULT("%y", 2, "34");
	}

	TEST_F(SimpleNumerics, YearCentPhasePadding) {
		curTm.tm_year = 3407 - 1900;
		EXPECT_MEMGUARD_RESULT("%y", 2, "07");
	}
	
	TEST_F(SimpleNumerics, YearCent5Characters) {
		curTm.tm_year = 12345 - 1900;
		EXPECT_MEMGUARD_RESULT("%y", 4, "45");
	}

	// Full year
	TEST_F(SimpleNumerics, FullYear) {
		curTm.tm_year = 2134 - 1900;
		EXPECT_MEMGUARD_RESULT("%y", 4, "2134");
	}

	TEST_F(SimpleNumerics, FullYearPadding) {
		curTm.tm_year = 407 - 1900;
		EXPECT_MEMGUARD_RESULT("%y", 4, "0407");
	}

	TEST_F(SimpleNumerics, Full5Characters) {
		curTm.tm_year = 12345 - 1900;
		EXPECT_MEMGUARD_RESULT("%y", 4, "12345");
	}
}
