#ifndef FIXTURES_H
#define FIXTURES_H
#include <gtest/gtest.h>
#include <stdlib.h>

#include "../src/strftime.h"

/*
 * SimpleFixture - a constant buffer size and the current time
 */
template<size_t _BufferSize> class SimpleFixture : public testing::Test {
protected:
	static const size_t BufferSize = _BufferSize;

	char buffer[BufferSize];
	time_t curTime;
	struct tm* curTm;

	virtual void SetUp() override {
		curTime = time(nullptr);
		curTm = localtime(&curTime);
	}
};

/*
 * MemoryGuardFixture - a constant buffer size, the current time but an area around 
 * the buffer which gets checked for modifications
 */
template<size_t _BufferSize, size_t _GuardSize = 256> class MemoryGuardFixture : public testing::Test {
public:
	static const size_t BufferSize = _BufferSize;
	static const size_t GuardSize = _GuardSize;
	static const size_t ResultBufferSize = _GuardSize * 2 + BufferSize;

	char* buffer;
	char resultBuffer[ResultBufferSize];
	char compareBuffer[ResultBufferSize];
	size_t expCharsWritten = SIZE_MAX;

	struct tm curTm;

	// a simple shortcut using the member variables
	void c_strftime(const char* format) {
		expCharsWritten = pbl_strftime(buffer, BufferSize, format, &curTm);
	}

	virtual void SetUpTime() {
		time_t curTime = time(nullptr);
		struct tm* curTmPtr = localtime(&curTime);
		memcpy(&curTm, curTmPtr, sizeof(struct tm)); // we want to modify curTm safely
	}

	virtual void SetUp() override {
		SetUpTime();
		
		srand(static_cast<unsigned int>(time(nullptr)));
		for (uint32_t i = 0; i < ResultBufferSize; i++)
			resultBuffer[i] = static_cast<char>(rand());
		memcpy(compareBuffer, resultBuffer, ResultBufferSize);
		buffer = resultBuffer + GuardSize;
	}

	virtual void TearDown() override {
		if (HasFatalFailure())
			return;

		ASSERT_EQ(0, memcmp(compareBuffer, resultBuffer, GuardSize));

		char* compareTailGuard = compareBuffer + GuardSize + BufferSize;
		char* resultTailGuard = resultBuffer + GuardSize + BufferSize;
		ASSERT_EQ(0, memcmp(compareTailGuard, resultTailGuard, GuardSize));

		if (expCharsWritten != SIZE_MAX) {
			ASSERT_TRUE(expCharsWritten < BufferSize); // more a test to not crash the fixtures work :)

			unsigned int remBufferSize = BufferSize - expCharsWritten - 1;
			if (remBufferSize > 0) {
				char* compareTailBuffer = compareBuffer + GuardSize + expCharsWritten + 1;
				char* resultTailBuffer = resultBuffer + GuardSize + expCharsWritten + 1;
				ASSERT_EQ(0, memcmp(compareTailBuffer, resultTailBuffer, remBufferSize));
			}

			ASSERT_EQ(0, resultBuffer[GuardSize + expCharsWritten]); // always check the 0-terminator
		}
	}
};

#define EXPECT_MEMGUARD_RESULT(charsWritten, expectedBuffer) \
	EXPECT_EQ(charsWritten, expCharsWritten); \
	EXPECT_STREQ(expectedBuffer, buffer);

#endif // FIXTURES_H