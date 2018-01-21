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
		EXPECT_MEMGUARD_RESULT("%a", 3, "Sun");
	}

	TEST_F(SimpleTexts, AbbrDayLast) {
		curTm.tm_wday = 6;
		EXPECT_MEMGUARD_RESULT("%a", 3, "Sat");
	}

	// Full weekday
	TEST_F(SimpleTexts, FullDay) {
		curTm.tm_wday = 1;
		EXPECT_MEMGUARD_RESULT("%A", 6, "Monday");
	}

	TEST_F(SimpleTexts, FullDayTooLong) {
		curTm.tm_wday = 4;
		EXPECT_MEMGUARD_RESULT("%A", 0, "Thursda");
	}

	// Abbreviated month
	TEST_F(SimpleTexts, AbbrMonth) {
		curTm.tm_mon = 0;
		EXPECT_MEMGUARD_RESULT("%b", 3, "Jan");
	}

	TEST_F(SimpleTexts, AbbrMonthLast) {
		curTm.tm_mon = 11;
		EXPECT_MEMGUARD_RESULT("%b", 3, "Dec");
	}

	TEST_F(SimpleTexts, AbbrMonthAlt) {
		curTm.tm_mon = 5;
		EXPECT_MEMGUARD_RESULT("%h", 3, "Jun");
	}

	// Full month
	TEST_F(SimpleTexts, FullMonth) {
		curTm.tm_mon = 3;
		EXPECT_MEMGUARD_RESULT("%B", 5, "April");
	}

	TEST_F(SimpleTexts, FullMonthTooLong) {
		curTm.tm_mon = 11;
		EXPECT_MEMGUARD_RESULT("%B", 0, "Decembe");
	}

	// Newline character
	TEST_F(SimpleTexts, NewLine) {
		EXPECT_MEMGUARD_RESULT("%n", 1, "\n");
	}

	TEST_F(SimpleTexts, NewLineMixed) {
		EXPECT_MEMGUARD_RESULT("a%nbc%n%nd", 7, "a\nbc\n\nd");
	}

	// 12 hour specifier
	TEST_F(SimpleTexts, Hour12Spec) {
		curTm.tm_hour = 11;
		EXPECT_MEMGUARD_RESULT("%p", 2, "AM");
	}

	TEST_F(SimpleTexts, Hour12SpecPM) {
		curTm.tm_hour = 19;
		EXPECT_MEMGUARD_RESULT("%p", 2, "PM");
	}

	// Tab character
	TEST_F(SimpleTexts, Tab) {
		EXPECT_MEMGUARD_RESULT("%t", 1, "\t");
	}

	TEST_F(SimpleTexts, TabMixed) {
		EXPECT_MEMGUARD_RESULT("a%tbc%t%td", 7, "a\tbc\t\td");
	}

	// Timezone name (not supported yet)
	TEST_F(SimpleTexts, Timezone) {
		EXPECT_MEMGUARD_RESULT("%Z", 0, "");
	}

	// Percent character
	TEST_F(SimpleTexts, Percent) {
		EXPECT_MEMGUARD_RESULT("%%", 1, "%");
	}

	TEST_F(SimpleTexts, PercentMixed) {
		EXPECT_MEMGUARD_RESULT("a%%bc%%%%d", 7, "a%bc%%d");
	}
}
