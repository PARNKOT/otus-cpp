#pragma once
#include <queue>
#include <string>
#include <filesystem>
#include <mutex>
#include <future>

namespace bulk {
    namespace fs =  std::filesystem;

    using command_t = std::string;
    using commands = std::vector<command_t>;

    struct Printer {
        virtual void print(const commands& cmds) = 0;
        
        virtual bool is_console_printer() { return false; }

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
        virtual bool is_console_printer() override {
            return true;
        }
    };

    struct FilePrinter : public Printer {
        FilePrinter(const fs::path& path) : path_{path} {}
        
        virtual void print(const commands& cmds)  override;

    private:
        fs::path path_;
    };

    struct AsyncPrinter {
        AsyncPrinter(std::shared_ptr<Printer> printer) : printer_{printer} {}

        void print(const command_t& cmd);
        void print(const commands& cmds);
        void worker();
        void run();
        void stop();

        bool is_console_printer() { return printer_->is_console_printer(); }

        private:
            std::future<void> f_;
            std::shared_ptr<Printer> printer_ = nullptr;
            std::queue<command_t> cmds_;
            std::mutex cmds_mutex_;
            std::atomic_bool need_stop_ = false;
    };

    struct Bulk {
        using printer_ptr = std::shared_ptr<AsyncPrinter>;

        explicit Bulk(int block_size) : block_size_(block_size) {}
        ~Bulk() {
            for (auto& observer : observers_) {
                if constexpr (std::is_same_v<std::remove_reference_t<decltype(observer)>::element_type, AsyncPrinter>) {
                    observer->stop();
                }
            }
        }

        void execute(const commands& cmds);

        void add_printer(printer_ptr printer);
        void notify();
        void notify(const commands& cmds);

    private:
        void execute_commands();

        commands cmds_;
        int block_size_;

        std::vector<printer_ptr> observers_;
    };
}