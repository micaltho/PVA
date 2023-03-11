#include <iostream>
#include <vector>
#include <cstdint>
#include <thread>
#include <mutex>
#include <future>

#define N 7

using Counter = std::uint64_t;
using LangfordSequence = std::vector<std::vector<int>>;

class LangfordTask {
public:
    LangfordTask(std::vector<int> langfordSequence, int i, Counter& nSolution, LangfordSequence& nLangfordSequence)
    : langfordSequence(langfordSequence), i(i), nSolution(nSolution), nLangfordSequence(nLangfordSequence) {}

    void operator()() {
        // Validate the sequence
        for (int z = 0; z < i; z++) {
            int stepCount = 0;

            // Check the sequence going down
            for (int y = z - 1; y >= 0; y--) {
                if (langfordSequence[y] != langfordSequence[z]) {
                    stepCount++;
                }
                else {
                    if (stepCount != langfordSequence[z]) {
                        return;
                    }
                    stepCount = 0;
                }
            }

            // Check the sequence going up
            stepCount = 0;
            for (int y = z + 1; y < i; y++) {
                if (langfordSequence[y] != langfordSequence[z]) {
                    stepCount++;
                }
                else {
                    if (stepCount != langfordSequence[z]) {
                        return;
                    }
                    stepCount = 0;
                }
            }
        }

        // If the sequence is complete, check if it's a solution
        if (i >= langfordSequence.size()) {
            for (int x = 1; x <= N; x++) {
                int xCount = 0;
                for (int u = 0; u < i; u++) {
                    if (langfordSequence[u] == x) {
                        xCount++;
                    }
                }
                if (xCount != 2) {
                    return;
                }
            }

            // If it's a solution, add it to the counter and sequence
            std::lock_guard<std::mutex> lock(m);
            nSolution++;
            nLangfordSequence.push_back(langfordSequence);

            return;
        }

        // If the sequence is not complete, make the next set of tasks
        std::vector<std::future<void>> futures;
        for (int k = 1; k <= N; k++) {
            std::vector<int> tmp = langfordSequence;
            tmp[i] = k;
            futures.push_back(std::async(std::launch::async, LangfordTask(tmp, i + 1, std::ref(nSolution), std::ref(nLangfordSequence))));
        }
        for (auto& future : futures) {
            future.wait();
        }
    }

private:
    std::vector<int> langfordSequence;
    int i;
    Counter& nSolution;
    LangfordSequence& nLangfordSequence;
    static std::mutex m;
};

std::mutex LangfordTask::m;

int main() {
    Counter nSolution = 0;
    LangfordSequence nLangfordSequence;

    std::vector<int> langfordSequence(N * 2);
    std::vector<std::future<void>> futures;

    for (int k = 1; k <= N; k++) {
        std::vector<int> tmp = langfordSequence;
        tmp[0] = k;
        futures.push_back(std::async(std::launch::async, LangfordTask(tmp, 1, std::ref(nSolution), std::ref(nLangfordSequence))));
    }

    return 0;
}
