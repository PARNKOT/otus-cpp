#pragma once
#include <string>
#include <vector>

struct Gosling {

    uint8_t uint8_var;    

    uint16_t uint16_var;    

    uint32_t uint32_var;    

    uint64_t uin64_var;    

    double double_var;    

    std::string string_var;    

};


class Gosling_encryptor {
public:
    using encryptor_out_t = std::vector<uint8_t>;

    Gosling_encryptor() {}

    encryptor_out_t encrypt(const Gosling& user_struct) {
        encryptor_out_t out;

        // ...

        return out;
    }

private:
};


class Gosling_decryptor {
public:
    using decryptor_in_t = std::vector<uint8_t>;

    Gosling_decryptor() {}

    Gosling decrypt(const decryptor_in_t& msg) {
        Gosling user_struct;

        // ...

        return user_struct;
    }

private:
};
