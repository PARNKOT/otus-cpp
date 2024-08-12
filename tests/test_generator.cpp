#include <iostream>

#define BOOST_TEST_MODULE test_generator
#include <boost/test/unit_test.hpp>

#include "generated/Gosling.hpp"
#include "generated/Rayan.hpp"


BOOST_AUTO_TEST_SUITE(GENERATOR_TEST) 

void fill_gosling(Gosling& gosling) {
    gosling.uint8_var = 0xFF;
    gosling.uint16_var = 0xFFFF;
    gosling.uint32_var = 0xFFFFFFFF;
    gosling.uint64_var = 0xFFFFFFFFFFFFFFFF;
    gosling.double_var = 1.5;
    gosling.string_var = "Rayan";
}

template<typename Iterable>
void print_bytes(const Iterable& bytes) {
    std::cout << std::hex;
    for (const auto& byte : bytes) {
        std::cout << (int)byte << ", ";
    }

    std::cout << "\b\b" << std::endl;
}

Gosling_encryptor::encryptor_out_t encrypted_gosling_desired = {
    0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x3F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x05, 0x52, 0x61, 0x79, 0x61, 0x6E,
};

BOOST_AUTO_TEST_CASE(GoslingEncryptionTest) {
    Gosling gosling;

    fill_gosling(gosling);

    auto encrypted_gosling = Gosling_encryptor().encrypt(gosling);

    BOOST_CHECK(encrypted_gosling.size() == encrypted_gosling_desired.size());
    BOOST_CHECK_EQUAL_COLLECTIONS(encrypted_gosling.begin(), encrypted_gosling.end(),
                                  encrypted_gosling_desired.begin(), encrypted_gosling_desired.end());
}

BOOST_AUTO_TEST_CASE(GoslingDecryptionTest) {
    auto gosling_decrypted = Gosling_decryptor().decrypt(encrypted_gosling_desired);

    BOOST_CHECK(gosling_decrypted.uint8_var == 0xFF);
    BOOST_CHECK(gosling_decrypted.uint16_var == 0xFFFF);
    BOOST_CHECK(gosling_decrypted.uint32_var == 0xFFFFFFFF);
    BOOST_CHECK(gosling_decrypted.uint64_var == 0xFFFFFFFFFFFFFFFF);
    BOOST_CHECK_CLOSE(gosling_decrypted.double_var, 1.5, 0.01);
    BOOST_CHECK_EQUAL(gosling_decrypted.string_var, "Rayan");
}

void fill_rayan(Rayan& rayan) {
    rayan.bool_var = true;
    rayan.int8_var = 80;
    rayan.int16_var = -1024;
    rayan.int32_var = -21845;
    rayan.int64_var = 123456789123;
}

Rayan_encryptor::encryptor_out_t encrypted_rayan_desired = {
    0x01,
    0x50,
    0xFC, 0x00,
    0xFF, 0xFF, 0xAA, 0xAB,
    0x00, 0x00, 0x00, 0x1C, 0xBE, 0x99, 0x1A, 0x83,
};

BOOST_AUTO_TEST_CASE(RayanEncryptionTest) {
    Rayan rayan;

    fill_rayan(rayan);

    auto encrypted_rayan = Rayan_encryptor().encrypt(rayan);
    
    BOOST_CHECK(encrypted_rayan.size() == encrypted_rayan_desired.size());
    BOOST_CHECK_EQUAL_COLLECTIONS(encrypted_rayan.begin(), encrypted_rayan.end(),
                                  encrypted_rayan_desired.begin(), encrypted_rayan_desired.end());

}

BOOST_AUTO_TEST_CASE(RayanDecryptionTest) {
    auto rayan_decrypted = Rayan_decryptor().decrypt(encrypted_rayan_desired);

    BOOST_CHECK(rayan_decrypted.bool_var);
    BOOST_CHECK(rayan_decrypted.int8_var == 80);
    BOOST_CHECK(rayan_decrypted.int16_var == -1024);
    BOOST_CHECK(rayan_decrypted.int32_var == -21845);
    BOOST_CHECK(rayan_decrypted.int64_var == 123456789123);
}

BOOST_AUTO_TEST_SUITE_END()