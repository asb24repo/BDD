#pragma once

#include <vector>
#include <array>
#include "two_dimensional_variable_array.hxx"

namespace LPMP {

    template<typename REAL>
    std::vector<REAL> min_marginal_differences(const two_dim_variable_array<std::array<REAL,2>>& min_marginals, const REAL eps)
    {
        assert(eps >= 0.0);
        std::vector<REAL> mmd;
        mmd.reserve(min_marginals.size());
        for(size_t var=0; var<min_marginals.size(); ++var)
        {
            const size_t nr_bdds = min_marginals.size(var);
            REAL diff_sum = 0.0; //std::numeric_limits<REAL>::infinity(); 
            bool negative = true;
            bool positive = true;
            for(size_t i=0; i<nr_bdds; ++i)
            {
                diff_sum += std::abs(min_marginals(var,i)[1] - min_marginals(var,i)[0]);
                assert(std::isfinite(min_marginals(var,i)[0]));
                assert(std::isfinite(min_marginals(var,i)[1]));
                if(min_marginals(var,i)[1] - min_marginals(var,i)[0] > eps)
                {
                    negative = false; 
                }
                else if(min_marginals(var,i)[1] - min_marginals(var,i)[0] < -eps)
                {
                    positive = false;
                }
                else
                {
                    negative = false;
                    positive = false; 
                }
            }
            assert(nr_bdds == 0 || (!(positive == true && negative == true)));
            if(negative)
                mmd.push_back(-diff_sum);
            else if(positive)
                mmd.push_back(diff_sum);
            else
                mmd.push_back(0.0);
        }

        const size_t nr_positive_min_marg_differences = std::count_if(mmd.begin(), mmd.end(), [&](const float x) { return x > eps; });
        const size_t nr_negative_min_marg_differences = std::count_if(mmd.begin(), mmd.end(), [&](const float x) { return x < -eps; });
        const size_t nr_zero_min_marg_differences = mmd.size() - nr_positive_min_marg_differences - nr_negative_min_marg_differences;
        std::cout << "%zero min margs = " << 100.0 * double(nr_zero_min_marg_differences) / double(mmd.size()) << "\n";
        std::cout << "#zero min margs = " << nr_zero_min_marg_differences << "\n";
        std::cout << "%positive min margs = " << 100.0 * double(nr_positive_min_marg_differences) / double(mmd.size()) << "\n";
        std::cout << "#positive min margs = " << nr_positive_min_marg_differences << "\n";
        std::cout << "%negative min margs = " << 100.0 * double(nr_negative_min_marg_differences) / double(mmd.size()) << "\n";
        std::cout << "#negative min margs = " << nr_negative_min_marg_differences << "\n";

        std::cout << "zero min marg vars:\n";
        for(size_t i=0; i<mmd.size(); ++i)
            if(mmd[i] <= eps && mmd[i] >= -eps)
                std::cout << " " << i;
        std::cout << "\n";
        return mmd;
    }

}