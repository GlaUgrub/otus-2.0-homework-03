#include <cassert>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <range/v3/all.hpp>
#include "ip_tools.h"

ip_addr_str_t split(const std::string &str, char d)
{
    ip_addr_str_t r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

void read_from_file(const std::string& file_name, ip_pool_t& ip_pool)
{
    std::ifstream file_stream(file_name);

    for(std::string line; std::getline(file_stream, line);)
    {
        ip_addr_str_t v = split(line, '\t');
        ip_pool.push_back(split(v.at(0), '.'));
    }
}

void read_from_console(ip_pool_t& ip_pool)
{
    for(std::string line; std::getline(std::cin, line);)
    {
        std::vector<std::string> v = split(line, '\t');
        ip_pool.push_back(split(v.at(0), '.'));
    }
}

bool check_ip(const uint8_t*)
{
    return true;
}

template <typename ... Types>
bool check_ip(const uint8_t* ip_array, uint8_t val, Types ... args)
{
    return (*ip_array == val) && check_ip(ip_array + 1, args...);
}

uint64_t time_ms() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

int main(int argc, char** argv)
{
    try
    {
        // 1. Read IPs from source
        ip_pool_t ip_pool_str;
        if (argc == 1)
        {
            read_from_console(ip_pool_str);
        }
        else
        {
            std::string file_name("/media/glaugrub/Data/git/otus-homeworks/otus-2.0-homework-02/ip_filter.tsv");
            if (argc == 2)
            {
                file_name = argv[1];
            }
            read_from_file(file_name, ip_pool_str);
        }

        const uint64_t start_ms = time_ms();

        std::vector<ip_addr_num_t> ip_pool_num;
        ip_pool_num.reserve(ip_pool_str.size());

        ip_addr_num_t ip_num;

        // 2. Convert literal IPs to numerical IPs
        for (auto& ip_str : ip_pool_str)
        {
            get_numerical_ip(ip_str, ip_num);
            ip_pool_num.push_back(ip_num);
        }

        // 3. Do reverse lexicoprapthical sort using ranges. Print all IPs.
        ranges::sort(ip_pool_num, ranges::lexicographical_compare);
        ranges::reverse(ip_pool_num);
        ranges::for_each(ip_pool_num, print_ip);


        // 4. Print IPs starting from 1
        uint8_t first = 1;
        uint8_t second = 0;

        auto check_first = [first](ip_addr_num_t& ip){return ip.at(0) == first;};
        ranges::for_each(ip_pool_num | ranges::view::filter(check_first), print_ip);

        first = 46;
        second = 70;

        // 5. Print IPs starting from 46, 70
        auto check_first_and_second = [first, second](ip_addr_num_t& ip){return ip.at(0) == first && ip.at(1) == second;};
        ranges::for_each(ip_pool_num | ranges::view::filter(check_first_and_second), print_ip);

        // 6. Print IPs having 46
        auto check_any = [first](ip_addr_num_t& ip){return ip.at(0) == first || ip.at(1) == first || ip.at(2) == first || ip.at(3) == first;};
        ranges::for_each(ip_pool_num | ranges::view::filter(check_any), print_ip);

        const uint64_t end_ms = time_ms();

        std::cout << "Duration in ms is " << end_ms - start_ms << std::endl;
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}