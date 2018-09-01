#define _CRT_SECURE_NO_WARNINGS
#include <gtest/gtest.h>
#include "../src/strftime.h"

#include "fixtures.h"

namespace {
	/*
	* Can it print the composite formats?
	* only basic functionality is tested as the parts are tested in their own test cases
	*/
	class Composite : public MemoryGuardFixture<64> {
	};

	class Composite6Bytes : public MemoryGuardFixture<6> {
		// to test the oversize behaviour
	};

	TEST_F(Composite, DateTimePref) {
		curTm.tm_year = 2013 - 1900;
		curTm.tm_mon = 10;
		curTm.tm_mday = 3;
		curTm.tm_wday = 1;
		curTm.tm_hour = 15;
		curTm.tm_min = 35;
		curTm.tm_sec = 59;
		static const char* exp = "Mon Nov  3 15:35:59 2013";
		EXPECT_MEMGUARD_RESULT("%c", strlen(exp), exp);
	}

	TEST_F(Composite, DateSlash) {
		curTm.tm_year = 2015 - 1900;
		curTm.tm_mon = 3;
		curTm.tm_mday = 28;
		static const char* exp = "04/28/15";
		EXPECT_MEMGUARD_RESULT("%D", strlen(exp), exp);
	}

	TEST_F(Composite, DatePref) {
		curTm.tm_year = 2015 - 1900;
		curTm.tm_mon = 3;
		curTm.tm_mday = 28;
		static const char* exp = "04/28/15";
		EXPECT_MEMGUARD_RESULT("%x", strlen(exp), exp);
	}

	TEST_F(Composite, DateISO) {
		curTm.tm_year = 2015 - 1900;
		curTm.tm_mon = 3;
		curTm.tm_mday = 28;
		static const char* exp = "2015-04-28";
		EXPECT_MEMGUARD_RESULT("%F", strlen(exp), exp);
	}

	TEST_F(Composite, Time12Hours) {
		curTm.tm_hour = 15;
		curTm.tm_min = 23;
		curTm.tm_sec = 45;
		static const char* exp = "03:23:45 PM";
		EXPECT_MEMGUARD_RESULT("%r", strlen(exp), exp);
	}

	TEST_F(Composite, Time24Hours) {
		curTm.tm_hour = 23;
		curTm.tm_min = 15;
		static const char* exp = "23:15";
		EXPECT_MEMGUARD_RESULT("%R", strlen(exp), exp);
	}

	TEST_F(Composite, Time24HoursWithSec) {
		curTm.tm_hour = 23;
		curTm.tm_min = 15;
		curTm.tm_sec = 42;
		static const char* exp = "23:15:42";
		EXPECT_MEMGUARD_RESULT("%T", strlen(exp), exp);
	}

	TEST_F(Composite6Bytes, Time24HoursWithSecOverLength) {
		curTm.tm_hour = 23;
		curTm.tm_min = 15;
		curTm.tm_sec = 42;
		static const char* exp = "23:15";
		EXPECT_MEMGUARD_RESULT("%T", 0, exp);
	}

	TEST_F(Composite6Bytes, Time24HoursWithSecOverLength2) {
		curTm.tm_hour = 23;
		curTm.tm_min = 15;
		curTm.tm_sec = 42;
		static const char* exp = "T23:1";
		EXPECT_MEMGUARD_RESULT("T%T", 0, exp);
	}

	TEST_F(Composite, TimePref) {
		curTm.tm_hour = 23;
		curTm.tm_min = 15;
		curTm.tm_sec = 42;
		static const char* exp = "23:15:42";
		EXPECT_MEMGUARD_RESULT("%X", strlen(exp), exp);
	}
}