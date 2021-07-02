#include <gtest/gtest.h>
#include <string>
#include <unordered_map>
#include <mutex>
#include <shared_mutex>
#include <optional>
#include <iostream>
#include <thread>
#include <chrono>

/*
 * std::shared_mutex предназначен для блокировок двух типов:
 * блокировки для чтени и блокировки для записи.
 * std::lock_guard<std::shared_mutex> обеспечивает монопольный
 * доступ к блокируемому ресурсу - запись.
 * std::shared_lock<std::shared_mutex> обеспечивает разделяемый
 * доступ - для чтения.
 */

using DnsEntry = std::string;

class DnsCache
{
    std::unordered_map<std::string, DnsEntry> entries_;
    mutable std::shared_mutex entryMutex_;

public:
    std::optional<DnsEntry> findEntry(std::string const& domain) const
    {
        std::shared_lock<std::shared_mutex> lock(entryMutex_);
        const auto it = entries_.find(domain);
        return it == entries_.end() ? std::nullopt : std::make_optional(it->second);
    }

    void setEntry(std::string domain, DnsEntry entry)
    {
        std::lock_guard<std::shared_mutex> lock(entryMutex_);
        entries_[std::move(domain)] = std::move(entry);
    }

};

std::mutex coutGuard;
void log(std::string const& value)
{
    std::lock_guard<std::mutex> lock(coutGuard);
    std::cout
        << std::this_thread::get_id() << " "
        << value << std::endl;

}

void readDns(DnsCache const& dns)
{
    for (auto i = 0u; i < 5; ++i) {
        const auto result = dns.findEntry("localhost");
        log(result.value_or("DNS entry not found"));
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void writeDns(DnsCache& dns)
{
    std::this_thread::sleep_for(std::chrono::seconds(2));
    dns.setEntry("localhost", "127.0.0.1");
}

TEST(P3_3_2, L3_13)
{
    DnsCache dns;

    std::thread t1(&readDns, std::ref(dns));
    std::thread t2(&readDns, std::ref(dns));
    std::thread t3(&writeDns, std::ref(dns));

    if (t1.joinable())
        t1.join();

    if (t2.joinable())
        t2.join();

    if (t3.joinable())
        t3.join();
}
