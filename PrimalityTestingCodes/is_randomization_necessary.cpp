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
#include <cmath>

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
bool is_prime_miller_rabin(const mpz_t n, int k = -1) {
    size_t num_digits = mpz_sizeinbase(n, 10);
    if (k == -1) {
        double loglog = std::log2(static_cast<double>(num_digits));
        k = std::max(5, 2 * static_cast<int>(std::ceil(loglog)));
    }

    if (mpz_cmp_ui(n, 2) == 0 || mpz_cmp_ui(n, 3) == 0)
        return true;
    if (mpz_cmp_ui(n, 1) <= 0 || mpz_even_p(n))
        return false;

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
        mpz_sub_ui(a, n, 3);
        mpz_urandomm(a, state, a);
        mpz_add_ui(a, a, 2);

        if (!miller_test(n, d, a)) {
            mpz_clear(d);
            mpz_clear(a);
            gmp_randclear(state);
            return false;
        }
    }

    mpz_clear(d);
    mpz_clear(a);
    gmp_randclear(state);
    return true;
}

bool is_prime_deter(mpz_t n, int k=-1) {
    size_t num_digits = mpz_sizeinbase(n, 10);
    if (k == -1) {
        double loglog = std::log2(static_cast<double>(num_digits));
        k = std::max(5, 2 * static_cast<int>(std::ceil(loglog)));
    }

    if (mpz_cmp_ui(n, 2) == 0 || mpz_cmp_ui(n, 3) == 0)
        return true;
    if (mpz_cmp_ui(n, 1) <= 0 || mpz_even_p(n))
        return false;

    mpz_t d;
    mpz_init_set(d, n);
    mpz_sub_ui(d, d, 1);  // d = n - 1

    while (mpz_even_p(d))
        mpz_divexact_ui(d, d, 2);

    // gmp_randstate_t state;
    // gmp_randinit_mt(state);
    // gmp_randseed_ui(state, std::time(nullptr));

    mpz_t a;
    mpz_init(a);

    for (int i = 0; i < k; ++i) {
        mpz_set_ui(a, 2 + i);  // Set a = 2 + i

        if (!miller_test(n, d, a)) {
            mpz_clear(d);
            mpz_clear(a);
            return false;
        }
    }

    mpz_clear(d);
    mpz_clear(a);
    // gmp_randclear(state);
    return true;
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

    int num_trials = 5;
    const std::vector<long long> digit_sizes = {100, 200, 300, 400, 500, 600, 700, 800}; // Customize as needed
    std::map<long long, double> random_times;
    std::map<long long, double> deter_times;

    for (int digits : digit_sizes) {
        double total_custom = 0.0;
        double total_gmp = 0.0;
        double total_deter = 0.0;
        num_trials = 500;
        for (int t = 0; t < num_trials; ++t) {
            mpz_t num;
            mpz_init(num);
            generate_random_mpz(num, rand_state, digits);

            // Custom Miller-Rabin Benchmark
            auto start_custom = std::chrono::high_resolution_clock::now();
            bool result_custom = is_prime_miller_rabin(num);
            auto end_custom = std::chrono::high_resolution_clock::now();
            total_custom += std::chrono::duration<double>(end_custom - start_custom).count();

            // GMP Benchmark
            size_t num_digits = mpz_sizeinbase(num, 10);
            double loglog = std::log2(num_digits);
            int k = std::max(5, 2 * static_cast<int>(std::ceil(loglog)));

            // deterministic test
            auto start_deter = std::chrono::high_resolution_clock::now();
            bool result_deter = is_prime_deter(num);
            auto end_deter = std::chrono::high_resolution_clock::now();
            total_deter += std::chrono::duration<double>(end_deter - start_deter).count();


            auto start_gmp = std::chrono::high_resolution_clock::now();
            int result_gmp = mpz_probab_prime_p(num, k);
            auto end_gmp = std::chrono::high_resolution_clock::now();
            total_gmp += std::chrono::duration<double>(end_gmp - start_gmp).count();

            mpz_clear(num);
        }

        std::cout << "Digits: " << digits << "\n";
        std::cout << "  Avg [Custom Miller-Rabin]: " << (total_custom / num_trials) << " seconds\n";
        std::cout << "  Avg [GMP Built-in]        : " << (total_gmp / num_trials) << " seconds\n\n";
        std::cout << "  Avg [Deterministic]       : " << (total_deter / num_trials) << " seconds\n\n";
        deter_times[digits] = total_deter / num_trials;
        random_times[digits] = total_custom / num_trials;
    }
    // Print to file
    std::ofstream file("Primality_Testing/data/miller_rabin_rand_comparision.csv");
    if (file.is_open()) {
        file << "Digits,Random Time,Deterministic Time\n";
        for (const auto& size : digit_sizes) {
            file << size << "," << random_times[size] << "," << deter_times[size] << "\n";
        }
        file.close();
    } else {
        std::cerr << "Unable to open file for writing.\n";
    }

    gmp_randclear(rand_state);
    return 0;
}