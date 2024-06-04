#pragma once
#include <vector>
#include <numeric>
#include <string>
#include <exception>
#include <iostream>
#include <boost/uuid/detail/md5.hpp>

namespace hashing {

enum class HashType {
    MD5,
    CRC32
};

using Byte = unsigned char;
using hash_t = std::string; //std::vector<Byte>;

hash_t calculate_crc32(const std::string& str) {
    return "";
}

hash_t calculate_md5(const std::string& str) {
    using boost::uuids::detail::md5;

    md5 hash;
    md5::digest_type digest;

    hash.process_bytes(str.data(), str.size());
    hash.get_digest(digest);

    std::string out;

    // Для сокращения хэша в один символ
    out.push_back((digest[0] ^ digest[1] ^ digest[2] ^ digest[3]) % 255);

    return out;
}

hash_t calculate(const std::string& str, HashType type) {
    switch (type)
    {
    case HashType::MD5:
        return calculate_md5(str);
    case HashType::CRC32:
        return calculate_crc32(str);
    default:
        throw std::runtime_error("Unknown hashing type");
    }
}

}


