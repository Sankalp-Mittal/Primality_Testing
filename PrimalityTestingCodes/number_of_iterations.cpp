#include <iostream>
#include <string>
#include <chrono>
#include <gmp.h>
#include <gmpxx.h>
#include <random>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <map>
#include <utility>


// Perform (base^exp) % mod using GMP
void mod_exp(mpz_t result, const mpz_t base, const mpz_t exp, const mpz_t mod) {
    mpz_powm(result, base, exp, mod);
}

// Returns true if n passes one Miller-Rabin test with base a
bool miller_test(const mpz_t n, const mpz_t d, const mpz_t a) {
    mpz_t x;
    mpz_init(x);
    mod_exp(x, a, d, n);

    mpz_t n_minus_1;
    mpz_init(n_minus_1);
    mpz_sub_ui(n_minus_1, n, 1);

    if (mpz_cmp_ui(x, 1) == 0 || mpz_cmp(x, n_minus_1) == 0) {
        mpz_clear(x);
        // mpz_clear(n_minus_1);
        return true;
    }

    mpz_t temp_d;
    mpz_init_set(temp_d, d);

    while (mpz_cmp(temp_d, n) < 0) {
        mpz_mul_ui(temp_d, temp_d, 2);
        mod_exp(x, a, temp_d, n);

        if (mpz_cmp_ui(x, 1) == 0) {
            mpz_clear(x);
            mpz_clear(temp_d);
            return false;
        }

        if (mpz_cmp(x, n_minus_1) == 0) {
            mpz_clear(x);
            mpz_clear(temp_d);
            return true;
        }
    }

    mpz_clear(x);
    mpz_clear(temp_d);
    mpz_clear(n_minus_1);
    return false;
}

// Custom Miller-Rabin with log-log number of rounds
std::pair<bool,long long> is_prime_miller_rabin(const mpz_t n, int num_digits, int k = -1) {
    // size_t num_digits = mpz_sizeinbase(n, 10);
    if (k == -1) {
        // std::cout<<"num_digits: " << num_digits << std::endl;
        double loglog = std::log2(static_cast<double>(num_digits));
        // std::cout << "loglog: " << loglog << std::endl;
        k = std::max(5, 4 * static_cast<int>(std::ceil(loglog)));
        // std::cout << "k: " << k << std::endl;
    }
    long long num_iter = 0;

    if (mpz_cmp_ui(n, 2) == 0 || mpz_cmp_ui(n, 3) == 0)
        return std::make_pair(true,0);
    if (mpz_cmp_ui(n, 1) <= 0 || mpz_even_p(n))
        return std::make_pair(false,0);

    mpz_t d;
    mpz_init_set(d, n);
    mpz_sub_ui(d, d, 1);  // d = n - 1

    while (mpz_even_p(d))
        mpz_divexact_ui(d, d, 2);

    gmp_randstate_t state;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, std::time(nullptr));

    mpz_t a;
    mpz_init(a);

    for (int i = 0; i < k; ++i) {
        num_iter++;
        mpz_sub_ui(a, n, 3);
        mpz_urandomm(a, state, a);
        mpz_add_ui(a, a, 2);

        if (!miller_test(n, d, a)) {
            mpz_clear(d);
            mpz_clear(a);
            gmp_randclear(state);
            return std::make_pair(false,num_iter);
        }
    }

    mpz_clear(d);
    mpz_clear(a);
    gmp_randclear(state);
    return std::make_pair(true,num_iter);
}

// Generate a random number with `num_digits` digits.
void generate_random_mpz(mpz_t result, gmp_randstate_t state, size_t num_digits) {
    mpz_t lower, upper;
    mpz_inits(lower, upper, NULL);

    mpz_ui_pow_ui(lower, 10, num_digits - 1); // 10^(d-1)
    mpz_ui_pow_ui(upper, 10, num_digits);     // 10^d
    mpz_sub(upper, upper, lower);             // Range = 10^d - 10^(d-1)

    mpz_urandomm(result, state, upper);       // result in [0, range)
    mpz_add(result, result, lower);           // result in [10^(d-1), 10^d)

    mpz_clears(lower, upper, NULL);
}

int main() {
    gmp_randstate_t rand_state;
    gmp_randinit_mt(rand_state);
    gmp_randseed_ui(rand_state, std::chrono::high_resolution_clock::now().time_since_epoch().count());

    int num_trials;
    const std::vector<long long> digit_sizes = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000}; // Customize as needed
    std::map<long long, double> random_iters;
    

    for (int digits : digit_sizes) {
        double total_iterations = 0.0;

        num_trials = 500;
        for (int t = 0; t < num_trials; ++t) {
            mpz_t num;
            mpz_init(num);
            bool isComp = false;
            while(!isComp){
                generate_random_mpz(num, rand_state, digits);
                if(mpz_even_p(num)){
                    continue; // Skip even numbers
                }
                // Check if the number is prime using GMP
                double loglog = std::log2(digits);
                int k = std::max(5, 4 * static_cast<int>(std::ceil(loglog)));
                int result_gmp = mpz_probab_prime_p(num, k);
                // std::cout<<"Digits: "<<digits<<", Result GMP: "<<result_gmp<<std::endl;
                // gmp_printf("The number is: %Zd\n", num);
                if (result_gmp != 2 && result_gmp != 1) {
                    isComp = true; // The number is composite
                }
            }
            

            // Custom Miller-Rabin Benchmark
            std::pair<bool,long long> result_custom = is_prime_miller_rabin(num, digits);
            total_iterations += result_custom.second;

            mpz_clear(num);
        }

        std::cout << "Digits: " << digits << "\n";
        std::cout << "  Avg [Custom Miller-Rabin]: " << (total_iterations / num_trials) << "\n";
        random_iters[digits] = total_iterations / num_trials;
    }
    // Print to file
    std::ofstream file("Primality_Testing/data/miller_rabin_iterations.csv");
    if (file.is_open()) {
        file << "Digits,Iterations\n";
        for (const auto& size : digit_sizes) {
            file << size << "," << random_iters[size] << "\n";
        }
        file.close();
    } else {
        std::cerr << "Unable to open file for writing.\n";
    }

    gmp_randclear(rand_state);
    return 0;
}