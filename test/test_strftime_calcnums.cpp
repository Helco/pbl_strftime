#define _CRT_SECURE_NO_WARNINGS
#include <gtest/gtest.h>
#include "../src/strftime.h"

#include "fixtures.h"

namespace {

	/*
	 * Can it calculate and print the more complex numbers?
	 */
	class CalculatedNums : public MemoryGuardFixture<7> {
		// use this buffer size to fit the year mixed in with text (and terminator)
	};

	 // Week based year
	TEST_F(CalculatedNums, WeekBasedYear) {
		curTm.tm_year = 1999 - 1900;
		curTm.tm_wday = 6;
		curTm.tm_yday = 16; // Saturday, 17nd January 1999
		EXPECT_MEMGUARD_RESULT("%G", 4, "1999");
	}

	TEST_F(CalculatedNums, WeekBasedYearISOStandard) {
		// easiest explanation: the first week with the majority of its days in the starting year
		curTm.tm_year = 1999 - 1900;
		curTm.tm_wday = 6;
		curTm.tm_yday = 1; // Saturday, 2nd January 1999
		EXPECT_MEMGUARD_RESULT("%G", 4, "1998");

		curTm.tm_year = 1999 - 1900;
		curTm.tm_wday = 1;
		curTm.tm_yday = 0; // Monday, 1nd January 1999
		EXPECT_MEMGUARD_RESULT("%G", 4, "1999");

		curTm.tm_year = 1999 - 1900;
		curTm.tm_wday = 2;
		curTm.tm_yday = 0; // Tuesday, 1nd January 1999
		EXPECT_MEMGUARD_RESULT("%G", 4, "1999");

		// TODO: Add more week based year tests here
	}

	// Note: I used https://www.timeanddate.com to work this out...
	// week number, where weeks start on Sunday, week 1 contains first Sunday in a year, earlier days are in week 0
	TEST_F(CalculatedNums, WeekSundayWZW) {
		curTm.tm_wday = 3;
		curTm.tm_yday = 32;
		curTm.tm_year = 2001 - 1900; // no leap year
		EXPECT_MEMGUARD_RESULT("%U", 2, "05");
	}

	TEST_F(CalculatedNums, WeekSundayWZWOneWeek) {
		curTm.tm_wday = 0;
		curTm.tm_yday = 0; // Year started on Sunday
		curTm.tm_year = 2001 - 1900; // no leap year
		EXPECT_MEMGUARD_RESULT("%U", 2, "01");
	}

	TEST_F(CalculatedNums, WeekSundayWZWZeroWeek) {
		curTm.tm_wday = 6; // Saturday
		curTm.tm_yday = 2; // Year started on Thursday
		curTm.tm_year = 2001 - 1900; // still no leap year
		EXPECT_MEMGUARD_RESULT("%U", 2, "00");
	}

	TEST_F(CalculatedNums, WeekSundayWZWLeapYear) {
		curTm.tm_wday = 1; // Monday
		curTm.tm_yday = 365;
		curTm.tm_year = 2012 - 1900; // 31. December, the year started on Sunday
		EXPECT_MEMGUARD_RESULT("%U", 2, "53");
	}

	// week number, where weeks start on Monday, week 1 contains January 4th, earlier days are in previous year
	TEST_F(CalculatedNums, WeekMonday) {
		curTm.tm_wday = 2; 
		curTm.tm_yday = 156;
		curTm.tm_year = 2012 - 1900; // 5. June
		EXPECT_MEMGUARD_RESULT("%V", 2, "23");
	}

	TEST_F(CalculatedNums, WeekMondayFirstWeek) {
		curTm.tm_wday = 1;
		curTm.tm_yday = 1;
		curTm.tm_year = 2012 - 1900;
		EXPECT_MEMGUARD_RESULT("%V", 2, "01");
	}

	TEST_F(CalculatedNums, WeekMondayLastYear) {
		curTm.tm_wday = 0;
		curTm.tm_yday = 0;
		curTm.tm_year = 2012 - 1900;
		EXPECT_MEMGUARD_RESULT("%V", 2, "52"); // this is the previous year
	}

	// week number, where weeks start on Monday, week 1 contains first Monday, earlier days are in week 0
	TEST_F(CalculatedNums, WeekMondayWZW) {
		curTm.tm_wday = 2;
		curTm.tm_yday = 156;
		curTm.tm_year = 2012 - 1900; // 5. June
		EXPECT_MEMGUARD_RESULT("%W", 2, "23");
	}

	TEST_F(CalculatedNums, WeekMondayWZWFirstWeek) {
		curTm.tm_wday = 1;
		curTm.tm_yday = 1;
		curTm.tm_year = 2012 - 1900;
		EXPECT_MEMGUARD_RESULT("%W", 2, "01");
	}

	TEST_F(CalculatedNums, WeekMondayWZWLastYear) {
		curTm.tm_wday = 0;
		curTm.tm_yday = 0;
		curTm.tm_year = 2012 - 1900;
		EXPECT_MEMGUARD_RESULT("%W", 2, "00");
	}
}