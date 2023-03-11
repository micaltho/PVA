#include <iostream>
#include <valarray>
#include <bitset>
#include <unistd.h>
#include <vector>
#include <atomic>
#include <omp.h>
#include <chrono>
#include <ctime>


size_t langfordSeq(size_t n, size_t j, size_t linkedSeq, size_t checkSeq, size_t pos = 0, size_t shifted = 0, size_t maxShift = 0)
{
    // Überprüfen, ob Wurzel
    if (pos == 0)
    {
        pos = (1 << (n + 1)) + 1; // z.B. für n = 4: 00100001
        maxShift = n / 2 - 1;
        return langfordSeq(n, j, linkedSeq, checkSeq, pos, shifted, maxShift);
    }

    if (shifted > maxShift)
    {
        return 0;
    }

    size_t numSequencies = 0;

    if ((linkedSeq & pos) == 0) // Überprüfen, ob Positionen frei sind
    {
        if ((linkedSeq | pos) == checkSeq) // Überprüfen, ob jede Position in der verknüpften Sequenz 1 ist
        {
            return 1;
        }

        if (j < 1)
        {
            return 0;
        }

        size_t maxShiftNew = (2 * n) - j - 1;

        // Überprüfen, ob zweite Ebene und für gerade Zahlen abschneiden
        if (((n - 1) == (j - 1)) && (n % 2 == 0) && (shifted == maxShift))
        {
            maxShiftNew /= 2;
        }

        numSequencies += langfordSeq(n, j, linkedSeq, checkSeq, (pos << 1), (shifted + 1), maxShift); // Weiter verschieben
        numSequencies += langfordSeq(n, (j - 1), (linkedSeq | pos), checkSeq, (1 << (j)) + 1, 0, maxShiftNew); // Zur nächsten Ebene gehen
    }
    else
    {
        shifted += 1;
        pos <<= 1;
        numSequencies += langfordSeq(n, j, linkedSeq, checkSeq, pos, shifted, maxShift);
    }

    return numSequencies;
}

bool checkResult(const std::valarray<bool> &seq)
{
    bool equals = true;
    for (bool item : seq)
    {
        equals &= item;
    }
    return equals;
}


void LangfordSequence(size_t n, size_t j, size_t linkedSeq, size_t checkSeq, size_t pos, size_t c, size_t nChilds, std::atomic_uint64_t *numSeq)
{
    // Check if root
    if (pos == 0)
    {
        pos = (1 << (n + 1)) + 1; // e.g. for n = 4: 00100001
        nChilds = n / 2;

#pragma omp parallel for
        for (size_t c = 0; c < nChilds; ++c)
        {
#pragma omp task
            LangfordSequence(n, j, linkedSeq, checkSeq, (pos << c), c, nChilds, numSeq);
        }

#pragma omp taskwait
        return;
    }

    // Check if every bit in the linked sequency is 1
    linkedSeq = linkedSeq | pos;
    if (linkedSeq == checkSeq)
    {
        (*numSeq)++; // If yes, we found a Langford sequence
        return;
    }

    // Stop creating child nodes with a length less than 1
    if ((j - 1) < 1)
    {
        return;
    }

    size_t nChildsNew = (2 * n) - j;
    if (((n - 1) == (j - 1)) && (n % 2 == 0) && (c == (nChilds - 1)))
    {
        nChildsNew = nChildsNew / 2;
    }

    pos = (1 << j) + 1;

#pragma omp parallel for
    for (size_t c = 0; c < nChildsNew; ++c)
    {
        size_t newPos = (pos << c);

        // Check if positions for next child are free
        if ((linkedSeq & newPos) == 0)
        {
#pragma omp task
            LangfordSequence(n, (j - 1), linkedSeq, checkSeq, newPos, c, nChildsNew, numSeq);
        }
    }

#pragma omp taskwait
    return;
}


int main()
{
    std::cout << "Please enter the number of different colors/digits.\n";
    size_t n;
    std::cin >> n;
    std::cout << "Calculating the Langford sequence for " << n << " different colors/numbers...\n";

    // Root of the state space
    size_t linkedSeq = 0;
    size_t numBits = 2 * n;
    size_t checkSeq = (1 << numBits) - 1;
    std::cout << "Initialized check sequence.\n";

    // Parallel version
    std::atomic_uint64_t numSeq;
    numSeq.store(0);
    linkedSeq = 0;
    auto start = std::chrono::steady_clock::now();
    #pragma omp parallel
    LangfordSequence(n, n, linkedSeq, checkSeq, 0, 0, 0, &numSeq);
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Found " << numSeq.load() << " sequences in " << elapsed.count() << " s\n";

    return 0;
}
