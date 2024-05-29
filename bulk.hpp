#pragma once
#include <queue>
#include <string>
#include <filesystem>

namespace bulk {
    namespace fs =  std::filesystem;

    using command_t = std::string;
    using commands = std::vector<command_t>;

    struct Printer {
        virtual void print(const commands& cmds) = 0;
        
        protected:
            template <typename Output, typename Iterable>
            void print_(Output& output, Iterable container) {
                if (container.size() == 0) {
                    return;
                }

                output << "bulk: ";
                for (int i = 0; i < container.size(); ++i) {
                    output << container[i];

                    if  (i != (container.size() - 1)){
                        output << ", ";
                    }
                }

                output << std::endl;
            }
    };

    struct ConsolePrinter : public Printer {
        virtual void print(const commands& cmds)  override;
    };

    struct FilePrinter : public Printer {
        FilePrinter(const fs::path& path) : path_{path} {}
        
        virtual void print(const commands& cmds)  override;

        private:
            fs::path path_;
    };

    struct Bulk {
        using printer_ptr = std::shared_ptr<Printer>;

        explicit Bulk(int block_size) : block_size_(block_size) {}
        void execute();

        void add_printer(printer_ptr printer);
        void notify();

    private:
        void execute_commands();

        commands cmds_;
        int block_size_;

        std::vector<printer_ptr> observers_;
    };
}