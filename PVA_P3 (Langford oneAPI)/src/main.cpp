#include <iostream>
#include <oneapi/tbb/task_group.h>
#include <oneapi/tbb/enumerable_thread_specific.h>
#include <vector>

#define N 7

using ThreadSpecificCounter = oneapi::tbb::enumerable_thread_specific<std::uint64_t>;
using ThreadSpecificLangfordSequence = oneapi::tbb::enumerable_thread_specific<std::vector<std::vector<int>>>;
ThreadSpecificCounter nSolution{0};
ThreadSpecificLangfordSequence  nLangfordSequence;

class LangfordTask{
    public:
        LangfordTask(oneapi::tbb::task_group& _group, std::vector<int> _langfordSequence, int _i):group(_group),
                                                                                                         langfordSequence(_langfordSequence),
                                                                                                         i(_i){}

        void operator()() const {
            //validation
            for(int z = 0; z < i; z++){

                //down
                int stepCount = 0;
                for(int y = z-1; y >= 0; y--){
                    if(langfordSequence[y] != langfordSequence[z]){
                        stepCount++;
                    }
                    else{
                        if(stepCount != langfordSequence[z]){
                            return;
                        }
                        stepCount = 0;
                    }
                }

                //up
                stepCount = 0;
                for(int y = z+1; y < i; y++){
                    if(langfordSequence[y] != langfordSequence[z]){
                        stepCount++;
                    }
                    else{
                        if(stepCount != langfordSequence[z]){
                            return;
                        }
                        stepCount = 0;
                    }
                }
            }

            //check if finished
            if(i >= langfordSequence.size()){
                for(int x = 1; x <= N; x++){
                    int xCount = 0;
                    for(int u = 0; u < i; u++){
                        if(langfordSequence[u] == x){
                            xCount++;
                        }
                    }
                    if(xCount != 2){
                        return;
                    }
                }
                nSolution.local()++;
                nLangfordSequence.local().push_back(langfordSequence);
                return;
            }

            //make next tasks
            for(int k = 1; k <= N; k++){
                std::vector<int> tmp = langfordSequence;
                tmp[i] = k;
                LangfordTask child(group,tmp,i+1);
                group.run(child);
            }
        }

        oneapi::tbb::task_group& group;
        std::vector<int> langfordSequence;
        int i;
};

int main() {
    oneapi::tbb::task_group runner;

    std::vector<int> langfordSequence(N*2);
    LangfordTask root(runner,langfordSequence,0);

    runner.run_and_wait(root);

    int nsolutions = 0;
    auto i=nSolution.begin();
    while (i!=nSolution.end()) {
        nsolutions += *i;
        ++i;
    }
    std::cout << nsolutions << " Lösungen" << std::endl;

    auto z=nLangfordSequence.begin();
    while (z!=nLangfordSequence.end()) {
        for(int i = 0; i < (*z).size(); i++){
            for(int y = 0; y < (*z)[i].size()-1; y++) {
                std::cout << (*z)[i][y] << ", ";
            }
            std::cout << (*z)[i][(*z)[i].size()-1] << std::endl;
        }
        ++z;
    }

    return 0;
}
