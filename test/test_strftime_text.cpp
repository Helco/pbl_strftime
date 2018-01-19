#define _CRT_SECURE_NO_WARNINGS
#include <gtest/gtest.h>
#include "../src/strftime.h"

#include "fixtures.h"

namespace {
	/*
	 * Can it print the simple text formats?
	 */
	class SimpleTexts : public MemoryGuardFixture<8> {
		// be careful, longer names like December or Thursday do not fit
	};

	// Abbreviated weekday
	TEST_F(SimpleTexts, AbbrDay) {
		curTm.tm_wday = 0;
		c_strftime("%a");
		EXPECT_MEMGUARD_RESULT(3, "Sun");
	}

	TEST_F(SimpleTexts, AbbrDayLast) {
		curTm.tm_wday = 6;
		c_strftime("%a");
		EXPECT_MEMGUARD_RESULT(3, "Sat");
	}

	// Full weekday
	TEST_F(SimpleTexts, FullDay) {
		curTm.tm_wday = 1;
		c_strftime("%A");
		EXPECT_MEMGUARD_RESULT(6, "Monday");
	}

	TEST_F(SimpleTexts, FullDayTooLong) {
		curTm.tm_wday = 4;
		c_strftime("%A");
		EXPECT_MEMGUARD_RESULT(0, "Thursda");
	}

	// Abbreviated month
	TEST_F(SimpleTexts, AbbrMonth) {
		curTm.tm_mon = 0;
		c_strftime("%b");
		EXPECT_MEMGUARD_RESULT(3, "Jan");
	}

	TEST_F(SimpleTexts, AbbrMonthLast) {
		curTm.tm_mon = 11;
		c_strftime("%b");
		EXPECT_MEMGUARD_RESULT(3, "Dec");
	}

	TEST_F(SimpleTexts, AbbrMonthAlt) {
		curTm.tm_mon = 5;
		c_strftime("%h");
		EXPECT_MEMGUARD_RESULT(4, "June");
	}

	// Full month
	TEST_F(SimpleTexts, FullMonth) {
		curTm.tm_mon = 3;
		c_strftime("%B");
		EXPECT_MEMGUARD_RESULT(5, "April");
	}

	TEST_F(SimpleTexts, FullMonthTooLong) {
		curTm.tm_mon = 11;
		c_strftime("%B");
		EXPECT_MEMGUARD_RESULT(0, "Decembe");
	}

	// Newline character
	TEST_F(SimpleTexts, NewLine) {
		c_strftime("%n");
		EXPECT_MEMGUARD_RESULT(1, "\n");
	}

	TEST_F(SimpleTexts, NewLineMixed) {
		c_strftime("a%nbc%n%nd");
		EXPECT_MEMGUARD_RESULT(7, "a\nbc\n\nd");
	}

	// 12 hour specifier
	TEST_F(SimpleTexts, Hour12Spec) {
		curTm.tm_hour = 11;
		c_strftime("%p");
		EXPECT_MEMGUARD_RESULT(2, "AM");
	}

	TEST_F(SimpleTexts, Hour12SpecPM) {
		curTm.tm_hour = 19;
		c_strftime("%p");
		EXPECT_MEMGUARD_RESULT(2, "PM");
	}

	// Tab character
	TEST_F(SimpleTexts, Tab) {
		c_strftime("%t");
		EXPECT_MEMGUARD_RESULT(1, "\t");
	}

	TEST_F(SimpleTexts, TabMixed) {
		c_strftime("a%tbc%t%td");
		EXPECT_MEMGUARD_RESULT(7, "a\tbc\t\td");
	}

	// Timezone name (not supported yet)
	TEST_F(SimpleTexts, Timezone) {
		c_strftime("%Z");
		EXPECT_MEMGUARD_RESULT(0, "");
	}

	// Percent character
	TEST_F(SimpleTexts, Percent) {
		c_strftime("%%");
		EXPECT_MEMGUARD_RESULT(1, "%");
	}

	TEST_F(SimpleTexts, PercentMixed) {
		c_strftime("a%%bc%%%%d");
		EXPECT_MEMGUARD_RESULT(7, "a%bc%%d");
	}
}
