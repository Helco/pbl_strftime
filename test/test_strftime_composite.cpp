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
		// no overlength test here either
	};

	TEST_F(Composite, DateTimePref) {
		curTm.tm_year = 2013 - 1900;
		curTm.tm_mon = 10;
		curTm.tm_mday = 3;
		curTm.tm_wday = 1;
		curTm.tm_hour = 15;
		curTm.tm_min = 35;
		curTm.tm_sec = 59;
		c_strftime("%c");
		static const char* exp = "Mon Nov  3 15:35:59 2013";
		EXPECT_MEMGUARD_RESULT(strlen(exp), exp);
	}

	TEST_F(Composite, DateSlash) {
		curTm.tm_year = 2015 - 1900;
		curTm.tm_mon = 3;
		curTm.tm_mday = 28;
		c_strftime("%D");
		static const char* exp = "03/28/15";
		EXPECT_MEMGUARD_RESULT(strlen(exp), exp);
	}

	TEST_F(Composite, DateSlashPref) {
		curTm.tm_year = 2015 - 1900;
		curTm.tm_mon = 3;
		curTm.tm_mday = 28;
		c_strftime("%x");
		static const char* exp = "03/28/15";
		EXPECT_MEMGUARD_RESULT(strlen(exp), exp);
	}

	TEST_F(Composite, DateISO) {
		curTm.tm_year = 2015 - 1900;
		curTm.tm_mon = 3;
		curTm.tm_mday = 28;
		c_strftime("%D");
		static const char* exp = "2015-03-28";
		EXPECT_MEMGUARD_RESULT(strlen(exp), exp);
	}

	TEST_F(Composite, Time12Hours) {
		curTm.tm_hour = 15;
		curTm.tm_min = 23;
		curTm.tm_sec = 45;
		c_strftime("%r");
		static const char* exp = "03:23:45 PM";
		EXPECT_MEMGUARD_RESULT(strlen(exp), exp);
	}

	TEST_F(Composite, Time24Hours) {
		curTm.tm_hour = 23;
		curTm.tm_hour = 15;
		c_strftime("%R");
		static const char* exp = "23:15";
		EXPECT_MEMGUARD_RESULT(strlen(exp), exp);
	}

	TEST_F(Composite, Time24HoursWithSec) {
		curTm.tm_hour = 23;
		curTm.tm_hour = 15;
		curTm.tm_sec = 42;
		c_strftime("%T");
		static const char* exp = "23:15:42";
		EXPECT_MEMGUARD_RESULT(strlen(exp), exp);
	}

	TEST_F(Composite, Time24HoursPref) {
		curTm.tm_hour = 23;
		curTm.tm_hour = 15;
		curTm.tm_sec = 42;
		c_strftime("%X");
		static const char* exp = "23:15:42";
		EXPECT_MEMGUARD_RESULT(strlen(exp), exp);
	}
}