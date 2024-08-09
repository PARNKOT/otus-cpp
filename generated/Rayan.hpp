#pragma once
#include <string>
#include <vector>

struct Rayan {

    bool bool_var;    

    int8_t int8_var;    

    int16_t int16_var;    

    int32_t int32_var;    

    int64_t int64_var;    

};


class Rayan_encryptor {
public:
    using encryptor_out_t = std::vector<uint8_t>;

    Rayan_encryptor() {}

    encryptor_out_t encrypt(const Rayan& user_struct) {
        encryptor_out_t out;

        // ...

        return out;
    }

private:
};


class Rayan_decryptor {
public:
    using decryptor_in_t = std::vector<uint8_t>;

    Rayan_decryptor() {}

    Rayan decrypt(const decryptor_in_t& msg) {
        Rayan user_struct;

        // ...

        return user_struct;
    }

private:
};
