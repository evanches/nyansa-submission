#include <unordered_map>
#include <map>
#include <set>
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>

using namespace std;
/**
 * The key for Tracking Dates, used instead of a int as the ctime class can
 * in theory handle odd extra time functions like leap-seconds, rather than dividing
 * the epoch number by (60 sec * 60 min * 24 hours) to save storage
 */
struct Date {
    int year;
    int month;
    int day;

    bool operator<(const Date &d) const {
        if (year != d.year)
            return year < d.year;
        if (month != d.month)
            return month < d.month;
        return day < d.day;
    }

    Date(int y, int m, int d) : year(y), month(m), day(d) {}


    std::string to_string() const {
        std::stringstream ss;
        ss << std::setw(2) << std::setfill('0') << month;
        ss << "/";
        ss << std::setw(2) << std::setfill('0') << day;
        ss << "/";
        ss << std::setw(4) << std::setfill('0') << (year + 1900) << " GMT";
        return ss.str();
    }
};

/**
 * Complexity Analysis:
 *     D is the number of unique dates
 *     U is the number of unique URLs
 *     C is the number of unique cardinalities
 *     N is the number of RAM lines
 *
 * Time complexity:
 *     Insert is O(N log D) as the input into a map is log D the size D << N, ~O(log N).
 *     Because the storage of URLs in an unordered_map, insertion and search is O(1) on average
 *     case.  Iteration through the map is less than O(N), as the entries can only be reduced
 *     in size as the maximum case is that each unique date and url pairing appears only once
 *     in the original file.
 *     
 *
 * Storage complexity:
 *     The storage complexity is basically O(D * U), because each date could possibly store
 *     one entry per each URL.  The hope is that this fits in main memory, as U was specified
 *     as fitting in main memory, and D is fairly small.
 */
int main(int ac, char **av) {
    // Not sure what desired error behavior is
    if (ac != 2)
        return 1;
    ifstream in(av[1]);
    if (!in)
        return 2;

    /**
     * Map of Date keys to pairs of hitcount.  Unordered to reduce insert impact for urls.
     * Date is ordered because it is to be printed in order.
     */
    std::map<Date, unordered_map<string, int>> all_data;
    std::string line;
    while (getline(in, line)) {
        auto pos = line.find("|");
        // Not sure what desired error behavior is.  Continuining on invalid input
        if (pos >= line.size()) continue;
        string epoch = line.substr(0, pos);
        string url = line.substr(pos + 1);

        // Not sure what desired error behavior is.  Continuining on invalid input
        if (url.empty())
            continue;
        int epoch_i;

        // Not sure what desired error behavior is.  Continuining on invalid input
        try {
           epoch_i = stoi(epoch);
        } catch (std::invalid_argument) {
            continue;
        }
        std::time_t epoch_t = epoch_i;
        tm *time_info = gmtime(&epoch_t);
        Date d(time_info->tm_year, (time_info->tm_mon + 1), time_info->tm_mday);
        auto &entry = all_data[d];
        if (entry.count(url))
            entry[url]++;
        else
            entry[url] = 1;
    }

    for (auto entry : all_data) {
        cout << entry.first.to_string() << endl;
        // Reverse the keys and values in the unordered_map, urls kept as a set to be
        // printed alphabetically, though not sure if that is a requirement
        std::map<int, std::set<string>> entry_by_count;
        for (auto url_count : entry.second) {
            entry_by_count[url_count.second].insert(url_count.first);
        }

        for (auto it = entry_by_count.rbegin(); it != entry_by_count.rend(); it++) {
            for (auto local_url : it->second) {
                cout << local_url << " " << it->first << endl;
            }
        }
    }
    return 0;
}
