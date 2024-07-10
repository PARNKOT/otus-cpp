#pragma once

#include <string>
#include <vector>
#include <set>
#include <algorithm>

namespace db {
    struct Person {
        int id;
        std::string name;
    };

    // bool operator<(const Person& left, const Person& right) {
    //     return left.id < right.id;
    // }

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
            //data_.insert(data);
        }

        void clear() {
            data_.clear();
        }

        Data at(std::size_t index) {
            return data_.at(index);
        }

        const std::vector<Data>& data() const {
            return data_;
        }
    private:
        std::string name_;
        std::vector<Data> data_;
        //std::set<Data> data_;
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

        void truncate(table_t& table) {
            table.clear();
        }

        table_t& get_table(const std::string& table_name) {
            for (auto& t : tables_ ) {
                if (t.name() == table_name){
                    return t;
                }
            }

            throw std::runtime_error("Cannot find table");
        }

    private:
        std::vector<table_t> tables_;
    };

    namespace operations {
        template <typename table_type>
        std::vector<std::string> intersection(const table_type& table1, const table_type& table2) {          
            // TODO: implement
            std::vector<std::string> out;

            std::vector<typename table_type::data_type> intersection;
            std::set_intersection(table1.data().cbegin(), table1.data().cend(),
                                  table2.data().cbegin(), table2.data().cend(), std::back_inserter(intersection),
                                  [](const auto& left, const auto& right) {
                                    return left.id == right.id;
                                  });

            for (const auto& el : intersection) {
                std::cout << el.id << ", " << el.name << std::endl;
            }

            return out;
        }

        template <typename table_type>
        std::vector<std::string> symmetric_difference(const table_type& table1, const table_type& table2) {          
            // TODO: implement
            std::vector<std::string> out;
            
            return out;
        }
    }

    namespace sql {
        enum class SqlCommand {
            INSERT,
            TRUNCATE,
            INTERSECTION,
            SYMMETRIC_DIFFERENCE,
            UNKNOWN = -1,
        };

        struct SqlQueryDescription {
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
                    new_pos = query.at(query.size()-1) == '\n' ? query.size() - 1 : query.size();
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

                if (new_pos >= query.size() - 1) {
                    break;
                }

                ws_pos = new_pos + 1;

            }

            return desc;
        }

        template <typename DB>
        bool insert_to_db(DB& db, const SqlQueryDescription& desc) {
            Person person;
            
            if (desc.data.size() < 2) {
                return false;
            }

            person.id = std::stoi(desc.data.at(0), nullptr, 10);
            person.name = desc.data.at(1);

            try {
                auto table = db.get_table(desc.tables.at(0));
                db.insert(table, person);
            } catch (std::exception& ex) {
                std::cout << ex.what() << std::endl;
                return false;
            }

            return true;
        }

        template <typename DB>
        bool truncate_table(DB& db, const SqlQueryDescription& desc) {
            try {
                auto table = db.get_table(desc.tables.at(0));
                db.truncate(table);
            } catch (std::exception& ex) {
                std::cout << ex.what() << std::endl;
                return false;
            }

            return true;
        }

        template <typename DB>
        std::string intersection(DB& db, const std::string& table_name1, const std::string& table_name2) {          
            try {
                auto table1 = db.get_table(table_name1);
                auto table2 = db.get_table(table_name2);
                auto res = db::operations::intersection(table1, table2);
                return "";
            } catch (std::exception& ex) {
                std::cout << ex.what() << std::endl;
                return "ERR: failed to find intersection";
            }
        }

        template <typename DB>
        std::string symmetric_difference(DB& db, const std::string& table_name1, const std::string& table_name2) {
            return "";
        }
        
    }
}