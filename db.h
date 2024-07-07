#pragma once

#include <string>
#include <vector>

namespace db {
    struct Person {
        int id;
        std::string name;
    };

    template <typename Data>
    class Table {
    public:
        using data_type = Data;

        explicit Table(const std::string& name) {
            name_ = name;
        }

        const std::string& name() const {
            return name_;
        }

        void push_back(const Data& data) {
            data_.push_back(data);
        }

        Data at(std::size_t index) {
            return data_.at(index);
        }
    private:
        std::string name_;
        std::vector<Data> data_;
    };

    template <typename table_type>
    class Database {
    public:
        using table_t = table_type;

        bool create_table(const std::string& table_name) {
            for (const auto& t : tables_) {
                if (t.name() == table_name) {
                    return false;
                }
            }

            tables_.emplace_back(table_name);
            return true;
        }

        void insert(table_t& table, const table_t::data_type& data) {
            table.push_back(data);
        }

        table_t& get_table(const std::string& table_name) {
            for (auto& t : tables_ ) {
                if (t.name() == table_name){
                    return t;
                }
            }
        }

    private:
        std::vector<table_t> tables_;
    };

    namespace sql {
        enum class SqlCommand {
            INSERT,
            TRUNCATE,
            INTERSECTION,
            SYMMETRIC_DIFFERENCE,
            UNKNOWN = -1,
        };

        struct SqlQueryDescription{
            SqlCommand cmd = SqlCommand::UNKNOWN;
            std::vector<std::string> tables;
            std::vector<std::string> data;
        };

        SqlCommand parse_sql_command(const std::string& cmd) {
            if (cmd == "INSERT") {
                return SqlCommand::INSERT;
            } else if (cmd == "TRUNCATE") {
                return SqlCommand::TRUNCATE;
            } else if (cmd == "INTERSECTION")  {
                return SqlCommand::INTERSECTION;
            } else if (cmd == "SYMMETRIC_DIFFERENCE")  {
                return SqlCommand::SYMMETRIC_DIFFERENCE;
            }

            return SqlCommand::UNKNOWN;
        }

        SqlQueryDescription parse_query(const std::string& query)  {
            enum ParseStage {
                COMMAND,
                TABLE,
                DATA,
            };
            
            SqlQueryDescription desc;

            for (std::size_t ws_pos = 0, stage = COMMAND;;) {
                auto new_pos = query.find(' ', ws_pos);

                if (new_pos == std::string::npos) {
                    break;
                }

                if (new_pos == ws_pos) {
                    ws_pos = new_pos + 1;
                    continue;
                }

                switch (stage)
                {
                case COMMAND: 
                    desc.cmd = parse_sql_command(query.substr(ws_pos, new_pos));
                    stage = TABLE;
                    break;
                case TABLE: {
                    auto table = query.substr(ws_pos, new_pos - ws_pos);
                    desc.tables.push_back(table);
                    stage = DATA;
                    break;
                }
                case DATA: {
                    auto data = query.substr(ws_pos, new_pos - ws_pos);
                    desc.data.push_back(data);
                    break;
                }
                default:
                    break;
                }

                ws_pos = new_pos + 1;

            }

            return desc;
        }
    }
}