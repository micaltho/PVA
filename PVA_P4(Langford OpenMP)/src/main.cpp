#include <iostream>
#include <valarray>
#include <bitset>
#include <unistd.h>
#include <vector>
#include <atomic>
#include <omp.h>
#include <chrono>
#include <ctime>

/**
 * \fn langfordSeq
 * \brief Calculates the number of langford sequencies sequentially.
 * \param n number of different numbers/colors
 * \param linkedSeq linked sequency of root or parent root
 * \param pos positions in bits
 * \return number of sequencies
 */
size_t langfordSeq(size_t n, size_t j, size_t linkedSeq, size_t checkSeq, size_t pos = 0, size_t shifted = 0, size_t maxShift = 0)
{
    size_t numSequencies = 0;

    // check if root
    if (pos == 0)
    {
        pos = (1 << (n + 1)) + 1; // e.g. for n = 4: 00100001
        maxShift = n / 2 - 1;
        return langfordSeq(n, j, linkedSeq, checkSeq, pos, shifted, maxShift);
    }
    if (shifted > maxShift)
    {
        return 0;
    }

    // check if positions are free
    if ((linkedSeq & pos) == 0)
    {
        // check if every bit in the linked sequency is 1
        if ((linkedSeq | pos) == checkSeq)
        {
            return 1; // if yes, we found a langford sequence
        }

        if (j < 1)
        {
            return 0;
        }

        size_t maxShiftNew = (2 * n) - j - 1;
        // check if second layer and cut for even number
        if (((n - 1) == (j - 1)) && (n % 2 == 0) && (shifted == maxShift))
        {
            maxShiftNew = maxShiftNew / 2;
        }
        return langfordSeq(n, j, linkedSeq, checkSeq, (pos << 1), (shifted + 1), maxShift)             // continue shifting
               + langfordSeq(n, (j - 1), (linkedSeq | pos), checkSeq, (1 << (j)) + 1, 0, maxShiftNew); // go to next node
    }
    else
    {
        shifted += 1;
        pos <<= 1;
        return numSequencies + langfordSeq(n, j, linkedSeq, checkSeq, pos, shifted, maxShift);
    }
}

bool checkResult(const std::valarray<bool> &seq)
{
    bool equals = true;
    for (size_t item : seq)
    {
        equals &= item;
    }
    return equals;
}

void LangfordSequence(size_t n, size_t j, size_t linkedSeq, size_t checkSeq, size_t pos, size_t c, size_t nChilds, std::atomic_uint64_t *numSeq)
{
    // check if root
    if (pos == 0)
    {
        pos = (1 << (n + 1)) + 1; // e.g. for n = 4: 00100001
        nChilds = n / 2;

        //#pragma omp parallel for
        for (size_t c = 0; c < nChilds; ++c)
        {
#pragma omp task
            {
                LangfordSequence(n, j, linkedSeq, checkSeq, (pos << c), c, nChilds, numSeq);
            }
        }
//#pragma omp taskwait
        return;
    }

    // check if every bit in the linked sequency is 1
    linkedSeq = linkedSeq | pos;
    if (linkedSeq == checkSeq)
    {
        ++(*numSeq); // if yes, we found a langford sequence
        return;
    }

    // stop creating childs with a distance less than 1
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
    //#pragma omp parallel for
    for (size_t c = 0; c < nChildsNew; ++c)
    {
        size_t newPos = (pos << c);
        // check if positions for next child are free
        if ((linkedSeq & newPos) == 0)
        {
#pragma omp task
            {
                LangfordSequence(n, (j - 1), linkedSeq, checkSeq, newPos, c, nChildsNew, numSeq);
            }
        }
    }
//#pragma omp taskwait
    return;
}


int main()
{
    /*if (argc != 2)
    {
        std::cout << "Please enter the number of different colors/digits.\n";
        return 1;
    }*/
    std::cout << "Please enter the number of different colors/digits.\n";
    size_t n;
    std::cin >> n;
    // size_t n = std::stoul(argv[1]);
    std::cout << "Calculating the langford sequence for " << n << " different colors/numbers...\n";

    // root of the state space
    size_t linkedSeq = 0;
    size_t numBits = 2 * n;
    size_t checkSeq = 1;
    for (size_t i = 1; i < numBits; ++i)
    {
        checkSeq = (checkSeq << 1) + 1;
    }
    std::cout << "Initialized check sequency.\n";
    /*
        // sequentially
        auto start = std::chrono::steady_clock::now();
        size_t numSequencies = langfordSeq(n, n, linkedSeq, checkSeq);
        auto end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start);

        std::cout << "\nFound " << numSequencies << " sequencies sequentially in " << elapsed.count() << " s\n";
    */
    // parallely
    std::atomic_uint64_t numSeq;
    numSeq.store(0);
    linkedSeq = 0;
    auto start = std::chrono::steady_clock::now();
    #pragma omp parallel
    //#pragma omp single nowait
    LangfordSequence(n, n, linkedSeq, checkSeq, 0, 0, 0, &numSeq);

    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Found " << numSeq.load() << " sequencies parallely in " << elapsed.count() << " s\n";

    return 0;
}
