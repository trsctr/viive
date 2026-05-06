#pragma once
#include <atomic>

struct LFOScopeBuffer {
	static constexpr int numChannels = 4;
	static constexpr int size = 4096;

	float data[numChannels][size] = {};
	std::atomic<int> writeIndex { 0 };

	void push(float lcL, float lcR, float hcL, float hcR) noexcept {
		int idx = writeIndex.load(std::memory_order_relaxed);
		data[0][idx] = lcL;
		data[1][idx] = lcR;
		data[2][idx] = hcL;
		data[3][idx] = hcR;
		writeIndex.store((idx + 1) & (size - 1), std::memory_order_release);
	}
};
