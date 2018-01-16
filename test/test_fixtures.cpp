#define _CRT_SECURE_NO_WARNINGS
#include <gtest/gtest.h>
#include <gtest/gtest-spi.h>
#include "../src/strftime.h"

#include "fixtures.h"

namespace {

	/*
	* Test the less obvious fixtures
	*/

	class FixturesTest : public MemoryGuardFixture<4> {
	};

	TEST_F(FixturesTest, MemoryGuardNoMod) {
		// Luigi style: don't do anything, does this work?
	}

	TEST_F(FixturesTest, MemoryGuardZeroChars) {
		expCharsWritten = 0;
		*buffer = 0;
	}

	TEST_F(FixturesTest, MemoryGuardSomeChars) {
		expCharsWritten = 2;
		buffer[0] = 'a';
		buffer[1] = 'b';
		buffer[2] = 0;
	}

	TEST_F(FixturesTest, MemoryGuardAllChars) {
		expCharsWritten = 3;
		buffer[0] = 'a';
		buffer[1] = 'b';
		buffer[2] = 'c';
		buffer[3] = 0;
	}

	class MemoryGuardFixtureTest : public MemoryGuardFixture<4> {
		virtual void TestBody() override { }
	};

	TEST(FixturesDeathTest, MemoryGuardFailZeroChars) {
		static MemoryGuardFixtureTest fix;
		fix.SetUp();

		fix.expCharsWritten = 0;
		// no 0-terminator here
		EXPECT_FATAL_FAILURE({ fix.TearDown(); }, "Expected");
	}

	TEST(FixturesDeathTest, MemoryGuardFailTooManyChars) {
		static MemoryGuardFixtureTest fix;
		fix.SetUp();

		fix.expCharsWritten = 4;
		memset(fix.buffer, 0, 5);
		EXPECT_FATAL_FAILURE({ fix.TearDown(); }, "Expected");
	}

	TEST(FixturesDeathTest, MemoryGuardFailLeadGuard) {
		static MemoryGuardFixtureTest fix;
		fix.SetUp();

		fix.expCharsWritten = 0;
		*fix.buffer = 0;

		fix.buffer[-2] = fix.buffer[-2] + 35; // make sure it is a different value
		EXPECT_FATAL_FAILURE({ fix.TearDown(); }, "Expected");
	}

	TEST(FixturesDeathTest, MemoryGuardFailTailGuard) {
		static MemoryGuardFixtureTest fix;
		fix.SetUp();

		fix.expCharsWritten = 0;
		*fix.buffer = 0;

		fix.buffer[10] = fix.buffer[10] + 35;
		EXPECT_FATAL_FAILURE({ fix.TearDown(); }, "Expected");
	}
}