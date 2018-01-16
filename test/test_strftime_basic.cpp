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
		EXPECT_EQ(0, strftime(nullptr, BufferSize, "abc", curTm));
	}

	TEST_F(ArgumentSanity, SizeIsNull) {
		EXPECT_EQ(0, strftime(buffer, 0, "abc", curTm));
	}

	TEST_F(ArgumentSanity, FormatIsNullptr) {
		EXPECT_EQ(0, strftime(buffer, BufferSize, nullptr, curTm));
	}

	TEST_F(ArgumentSanity, TmIsNullptr) {
		EXPECT_EQ(0, strftime(buffer, BufferSize, "abc", nullptr));
	}

	TEST_F(ArgumentSanity, AllArgsInvalid) {
		EXPECT_EQ(0, strftime(nullptr, 0, nullptr, nullptr));
	}

	/*
	 * Can it copy text?
	 */

	class CopyText : public MemoryGuardFixture<4> {
	};

	TEST_F(CopyText, NoText) {
		expCharsWritten = strftime(buffer, BufferSize, "", curTm);
		EXPECT_EQ(0, expCharsWritten);
		EXPECT_STREQ("", buffer);
	}

	TEST_F(CopyText, SomeText) {
		expCharsWritten = strftime(buffer, BufferSize, "abc", curTm);
		EXPECT_EQ(3, expCharsWritten);
		EXPECT_STREQ("abc", buffer);
	}

	TEST_F(CopyText, TooMuchText) {
		expCharsWritten = strftime(buffer, BufferSize, "abcdefgh", curTm);
		EXPECT_EQ(0, expCharsWritten);
		EXPECT_STREQ("abc", buffer);
	}
}
