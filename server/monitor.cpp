#include "monitor.h"

// Class constructor
Monitor::Monitor() {
    this->subscribers.clear();
}


// Custom comparator to sort by second element of a pair
bool sortbySec(const std::pair<sockaddr_in, unsigned long>& a, const std::pair<sockaddr_in, unsigned long>& b) {
    return (a.second < b.second);
}


void Monitor::addSubscriber(sockaddr_in clientAddr, unsigned long timestamp) {
    subscribers.push_back({clientAddr, timestamp});
}


void Monitor::removeSubscribers() {
    auto now = std::chrono::system_clock::now();
    auto epoch = now.time_since_epoch();
    auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
    unsigned long timestamp = value.count();
    int count = 0;

    for (const auto& pair : subscribers) {
        if (pair.second < timestamp) {
            count++;
        } else {
            break;
        }
    }

    for (int i = 0; i < count; i++) {
        subscribers.pop_front();
    }
}


void Monitor::sortSubscribers() {
    std::sort(subscribers.begin(), subscribers.end(), sortbySec);
}


std::deque<std::pair<sockaddr_in, unsigned long>> Monitor::getSubscribers() {
    return subscribers;
}